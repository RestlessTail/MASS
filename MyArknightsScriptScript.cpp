#include "MyArknightsScriptScript.h"
#include "Scene.h"
#include <windows.h>
//#include <gl/GL.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stdio.h"
#include <stb_image.h>
#include <iostream>
#include "Line.h"
#include <locale>
#include <codecvt>
#include "Shadow.h"
#include "ButtonHideUI.h"
#include "LogView.h"
#include "ButtonLogMode.h"
#include "ButtonBack.h"
#include "Mask.h"
#include "Attack.h"
#include "Retreat.h"
#include <filesystem>
#include <irrKlang.h>
#include "Action.h"

#pragma warning(disable:4996)

Actor* MyArknightsScriptScript::ActiveCharacterL = nullptr;
Actor* MyArknightsScriptScript::ActiveCharacterR = nullptr;
LineControl* MyArknightsScriptScript::lineControl = nullptr;
LineControl* MyArknightsScriptScript::nameControl = nullptr;
Shadow* MyArknightsScriptScript::shadow = nullptr;
GLFWwindow* MyArknightsScriptScript::MainWindow = nullptr;
GLSLLoader* MyArknightsScriptScript::loader = nullptr;
int MyArknightsScriptScript::windowHeight = 720;
int MyArknightsScriptScript::windowWidth = 1280;
bool MyArknightsScriptScript::AcceptUserInput = false;
bool MyArknightsScriptScript::WaitForNextAction = false;
bool MyArknightsScriptScript::ActionInitFlag = false;
bool MyArknightsScriptScript::KeyReleaseFlag = true;
LogView* MyArknightsScriptScript::logView = nullptr;
std::list<Button*> MyArknightsScriptScript::buttonGroup;
ButtonBack* MyArknightsScriptScript::buttonBack = nullptr;
Mask* MyArknightsScriptScript::mask = nullptr;
Attack* MyArknightsScriptScript::attack = nullptr;
Retreat* MyArknightsScriptScript::retreat = nullptr;
Scene* MyArknightsScriptScript::ActiveScene = nullptr;
bool MyArknightsScriptScript::running = true;
int MyArknightsScriptScript::delayFrame = 0;
irrklang::ISoundEngine* MyArknightsScriptScript::irrklangSoundEngine = nullptr;
MASSSoundEngine* MyArknightsScriptScript::soundEngine = new MASSSoundEngine;
std::map<int, SoundSource_t*> MyArknightsScriptScript::AllSoundSource;
std::map<int, Actor_t*> MyArknightsScriptScript::AllActor;

MyArknightsScriptScript::MyArknightsScriptScript()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    MainWindow = glfwCreateWindow(windowWidth, windowHeight, "MASS", NULL, NULL);
    if (MainWindow == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(MainWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glViewport(0, 0, windowWidth, windowHeight);
    glfwSetFramebufferSizeCallback(MainWindow, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    });

    loader = new GLSLLoader;
    loader->loadGLSLProgram("shader/default.vert", "shader/default.frag");
    loader->loadGLSLProgram("shader/text.vert", "shader/text.frag");
    loader->loadGLSLProgram("shader/actor.vert", "shader/actor.frag");
    loader->loadGLSLProgram("shader/mask.vert", "shader/mask.frag");
    loader->SetActiveProgram(0);

    stbi_set_flip_vertically_on_load(true);//OpenGL的y坐标和大部分y坐标的方向是相反的，所以在加载图片的时候要反过来

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glfwSetKeyCallback(MainWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods) {//处理“按任意键继续”
        if (action == GLFW_RELEASE) {
            MyArknightsScriptScript::KeyReleaseFlag = true;
        }
        else if (MyArknightsScriptScript::KeyReleaseFlag) {
            if (MyArknightsScriptScript::WaitForNextAction) {
                MyArknightsScriptScript::WaitForNextAction = false;
                MyArknightsScriptScript::nextAction();
            }
            MyArknightsScriptScript::KeyReleaseFlag = false;
        }
    });

    glfwSetScrollCallback(MainWindow, [](GLFWwindow* window, double xoffset, double yoffset) {
        if (LogView::allowScroll) {
            float newCursor = LogView::cursor - static_cast<float>(yoffset) * 0.4f;
            if (newCursor > 0 && newCursor < MyArknightsScriptScript::logView->allLogs.size() - 1) {
                LogView::cursor = newCursor;
            }
        }
    });

    lineControl = new LineControl(-0.4f, -0.74f);
    nameControl = new LineControl(-0.48f, -0.74f);
    nameControl->scale = 1.1f;
    nameControl->alignment = LineAlignment::RIGHT;
    nameControl->color = { 0.5f, 0.5f, 0.5f };
    nameControl->addCharacterDelay = 0;

    ActionInitFlag = false;

    shadow = new Shadow;
    shadow->setupVertex(
        { -1.0f, 1.0f, 0.0f },
        { 1.0f, 1.0f, 0.0f },
        { -1.0f, -1.0f, 0.0f },
        { 1.0f, -1.0f, 0.0f }
    );
    shadow->setupTextures("assets/mask.png");

    ButtonHideUI* buttonHideUI = new ButtonHideUI;
    buttonHideUI->setupVertex(
        { -0.8f, 0.9f, 0.1f },
        { -0.75f, 0.9f, 0.1f },
        { -0.8f, 0.8f, 0.1f },
        { -0.75f, 0.8f, 0.1f }
    );
    buttonHideUI->setupTextures("assets/button/hideUI.png");
    buttonHideUI->useDefaultResponseRange();
    buttonHideUI->disabled = false;
    ButtonLogMode* buttonLogMode = new ButtonLogMode;
    buttonLogMode->setupVertex(
        { -0.9f, 0.9f, 0.1f },
        { -0.85f, 0.9f, 0.1f },
        { -0.9f, 0.8f, 0.1f },
        { -0.85f, 0.8f, 0.1f }
    );
    buttonLogMode->setupTextures("assets/button/logMode.png");
    buttonLogMode->useDefaultResponseRange();
    buttonLogMode->disabled = false;
    buttonGroup.push_back(buttonHideUI);
    buttonGroup.push_back(buttonLogMode);
    
    logView = new LogView;

    buttonBack = new ButtonBack;
    buttonBack->setupVertex(
        { -1.0f, 0.95f, 0.1f },
        { -0.95f, 0.95f, 0.1f },
        { -1.0f, 0.85f, 0.1f },
        { -0.95f, 0.85f, 0.1f }
    );
    buttonBack->setupTextures("assets/button/back.png");
    buttonBack->useDefaultResponseRange();
    buttonBack->disabled = true;
    buttonBack->visible = false;

    mask = new Mask;
    mask->setupVertex(
        { -1.0f, 1.0f, 0.0f },
        { 1.0f, 1.0f, 0.0f },
        { -1.0f, -1.0f, 0.0f },
        { 1.0f, -1.0f, 0.0f }
    );

    attack = new Attack;

    retreat = new Retreat;

    MouseReleaseFlag = true;
    ActiveScene = nullptr;
    FinishActionNow = false;

    setlocale(LC_ALL, "chs");

    irrklangSoundEngine = irrklang::createIrrKlangDevice();

    curAction = nullptr;
}

void MyArknightsScriptScript::paint(){
    if (ActiveScene) {
        loader->SetActiveProgram(0);
        ActiveScene->paint();
    }
    if (ActiveCharacterL) {
        MyArknightsScriptScript::loader->SetActiveProgram(2);
        ActiveCharacterL->paint();
    }
    if (ActiveCharacterR) {
        MyArknightsScriptScript::loader->SetActiveProgram(2);
        ActiveCharacterR->paint();
    }
    loader->SetActiveProgram(0);
    if (shadow->visible) {
        shadow->paint();
    }
    if (lineControl->visible) {
        lineControl->paint();
    }
    if (nameControl->visible) {
        nameControl->paint();
    }
    for (auto b : buttonGroup) {
        if (b->visible) {
            b->paint();
        }
    }
    if (!attack->disabled) {
        attack->paint();
    }
    if (!retreat->disabled) {
        retreat->paint();
    }
    if (mask->visible) {
        mask->paint();
    }
    if (logView->visible) {
        //loader->SetActiveProgram(3);
        logView->paint();
    }
    if (buttonBack->visible) {
        loader->SetActiveProgram(0);
        buttonBack->paint();
    }
}

void MyArknightsScriptScript::exec(){
    while (!glfwWindowShouldClose(MainWindow) && running) {
        glClear(GL_COLOR_BUFFER_BIT);
        paint();

        processAction();
        processInput();

        glfwSwapBuffers(MainWindow);
        glfwPollEvents();
        SleepForaWhile();
    }
}

void MyArknightsScriptScript::SleepForaWhile(){
    static int loopStartTime = static_cast<int>(1000 * glfwGetTime());

    int time = static_cast<int>(1000 * glfwGetTime());
    int sleepTime = 16 - (time - loopStartTime);
    sleepTime = sleepTime < 0 ? 0 : sleepTime;
    loopStartTime = time;
    Sleep(sleepTime);
}

void MyArknightsScriptScript::nextAction(){
    //++curAction;
    MyArknightsScriptScript::ActionInitFlag = false;
    MyArknightsScriptScript::running = false;
}

void MyArknightsScriptScript::processAction()
{
    curAction->exec();
}

void MyArknightsScriptScript::RTError(const wchar_t* msg)
{
    printf("Runtime Error: %ls", msg);
}

void MyArknightsScriptScript::processInput()
{
    if (glfwGetMouseButton(MainWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (MouseReleaseFlag) {
            double xPos, yPos;
            glfwGetCursorPos(MainWindow, &xPos, &yPos);
            int w, h;
            glfwGetWindowSize(MainWindow, &w, &h);

            float coordX = static_cast<float>((xPos / w * 2) - 1);
            float coordY = static_cast<float>(1 - (yPos / h * 2));

            for (auto b : buttonGroup) {
                if (b->disabled == false) {
                    if (b->isClicked(coordX, coordY)) {
                        b->onClicked();
                    }
                }
            }

            if (buttonBack->disabled == false) {
                if (buttonBack->isClicked(coordX, coordY)) {
                    buttonBack->onClicked();
                }
            }

            MouseReleaseFlag = false;
        }
    }
    else {
        MouseReleaseFlag = true;
    }
}

void MyArknightsScriptScript::RTWarning(const wchar_t* msg)
{
    printf("Runtime Warning: %ls", msg);
}
