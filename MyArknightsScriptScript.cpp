#include "MyArknightsScriptScript.h"
#include "interpreter/syntax.h"
#include "Scene.h"
#include <windows.h>
//#include <gl/GL.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Symbol.h"
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

#pragma warning(disable:4996)

extern SymbolTableType SymbolTable;
extern ActionTableType ActionTable;
ActionTableType::iterator curAction;

extern SceneTableType SceneTable;
extern ActorTableType ActorTable;
extern SoundTableType SoundTable;

std::filesystem::path execPath;
std::filesystem::path scriptPath;

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

MyArknightsScriptScript::MyArknightsScriptScript(const char* filename)
{
#ifdef _DEBUG
    execPath = std::filesystem::current_path();
#else
    TCHAR exePathCstr[256] = { 0 };
    GetModuleFileName(0, exePathCstr, 255);
    execPath = std::filesystem::path(exePathCstr);
    execPath = execPath.parent_path();
    std::filesystem::current_path(execPath);
#endif
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
        /*LineControl::projection = glm::ortho(0.0f,
            static_cast<float>(MyArknightsScriptScript::windowWidth),
            0.0f,
            static_cast<float>(MyArknightsScriptScript::windowHeight)
        );*/
    });
    //glfwSetInputMode(MainWindow, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);//粘滞鼠标模式，当鼠标点下后，在事件被处理前一直为GLFW_PRESS

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
    running = true;

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
        { -0.8f, 0.9f, 0.0f },
        { -0.75f, 0.9f, 0.0f },
        { -0.8f, 0.8f, 0.0f },
        { -0.75f, 0.8f, 0.0f }
    );
    buttonHideUI->setupTextures("assets/button/hideUI.png");
    buttonHideUI->useDefaultResponseRange();
    buttonHideUI->disabled = false;
    ButtonLogMode* buttonLogMode = new ButtonLogMode;
    buttonLogMode->setupVertex(
        { -0.9f, 0.9f, 0.0f },
        { -0.85f, 0.9f, 0.0f },
        { -0.9f, 0.8f, 0.0f },
        { -0.85f, 0.8f, 0.0f }
    );
    buttonLogMode->setupTextures("assets/button/logMode.png");
    buttonLogMode->useDefaultResponseRange();
    buttonLogMode->disabled = false;
    buttonGroup.push_back(buttonHideUI);
    buttonGroup.push_back(buttonLogMode);
    
    logView = new LogView;

    buttonBack = new ButtonBack;
    buttonBack->setupVertex(
        { -1.0f, 0.95f, 0.0f },
        { -0.95f, 0.95f, 0.0f },
        { -1.0f, 0.85f, 0.0f },
        { -0.95f, 0.85f, 0.0f }
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
    delayFrame = 0;

    setlocale(LC_ALL, "chs");
    scriptPath = std::filesystem::path(filename);
    scriptPath = scriptPath.parent_path();

    SoundEngine = irrklang::createIrrKlangDevice();

    FILE* fp = fopen(filename, "r");
    std::filesystem::current_path(scriptPath);
    Parse(fp);
    fclose(fp);
    Symbol::LoadSymbols();
    curAction = ActionTable.begin();
    //std::filesystem::current_path(execPath);
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

	/*for (ActionTableItem action : ActionTable) {
		switch (action.type) {
		case ActionType::USESCENE:
		}
	}*/
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
    ++curAction;
    MyArknightsScriptScript::ActionInitFlag = false;
}

void MyArknightsScriptScript::processAction()
{
    switch (curAction->type) {
    case ActionType::USESCENE:{
        //虽然在更新了Scene后其透明度的初始值也是0.0f，但在调用paint()后已经增加了一个步长，因此并不会在这里被删掉
        if (ActiveScene == nullptr || ActiveScene->transparency <= 0.0f) {
            delete ActiveScene;
            std::wstring textureKey = *(curAction->data.begin());
            unsigned int textureID = SceneTable.find(textureKey)->second;
            ActiveScene = new Scene(textureID);
            nextAction();
            break;
        }
        if (ActiveScene->unloading == false) {
            ActiveScene->loading = false;
            ActiveScene->unloading = true;
        }
        break;
    }
    case ActionType::VOICEOVER: {
        if (!ActionInitFlag) {
            lineControl->clear();
            nameControl->clear();
            if (FinishActionNow) { //若收到快进指令，则将两个字符显示的间隔设成0
                lineControl->addCharacterDelay = 0;
            }
            auto itr = curAction->data.begin();
            std::wstring content = *itr;
            lineControl->buffer = content;
            logView->addLog(L"", content);
            ActionInitFlag = true;
        }
        
        if (lineControl->curCharacter + 1 >= lineControl->buffer.size()) { //执行下一条命令的条件是buffer中的字符已经全部显示    
            WaitForNextAction = true;
        }
        break;
    }
    case ActionType::SAY: {
        if (!ActionInitFlag) {
            lineControl->clear();
            nameControl->clear();
            auto itr = curAction->data.begin();
            std::wstring name = ActorTable.find(*itr)->second.Name;
            ++itr;
            std::wstring content = *itr;
            nameControl->buffer = name;
            lineControl->buffer = content;
            logView->addLog(name, content);
            ActionInitFlag = true;
        }
        if (ActiveCharacterL) {
            if (ActiveCharacterL->name == nameControl->buffer) {
                ActiveCharacterL->brightness = 1.0f;
            }
            else {
                ActiveCharacterL->brightness = 0.5f;
            }
        }
        if (ActiveCharacterR) {
            if (ActiveCharacterR->name == nameControl->buffer) {
                ActiveCharacterR->brightness = 1.0f;
            }
            else {
                ActiveCharacterR->brightness = 0.5f;
            }
        }

        if (lineControl->curCharacter + 1 >= lineControl->buffer.size() || nameControl->curCharacter + 1 >= nameControl->buffer.size()) { //执行下一条命令的条件是buffer中的字符已经全部显示    
            WaitForNextAction = true;
        }
        break;
    }
    case ActionType::FINISH: {
        lineControl->clear();
        nameControl->clear();
        //虽然在更新了Scene后其透明度的初始值也是0.0f，但在调用paint()后已经增加了一个步长，因此并不会在这里被删掉
        if (ActiveCharacterL) {
            ActiveCharacterL->unloading = true;
        }
        if (ActiveCharacterR) {
            ActiveCharacterR->unloading = true;
        }
        if (ActiveScene == nullptr || ActiveScene->transparency <= 0.0f) {
            delete ActiveScene;
            running = false;
        }
        else if (ActiveScene->unloading == false) {
            ActiveScene->loading = false;
            ActiveScene->unloading = true;
        }
        break;
    }
    case ActionType::ENTER: {
        auto itr = curAction->data.begin();
        std::wstring actorKey = *itr;
        ++itr;
        std::wstring position = *itr;

        if (position == L"L") {
            //将unloading不断设置成true，直到透明度小于等于0。在此之前不会执行下一道命令。
            if (ActiveCharacterL == nullptr || ActiveCharacterL->transparency <= 0.0f) {
                delete ActiveCharacterL;
                auto data = ActorTable.find(actorKey);
                unsigned int textureID = data->second.Texture;
                ActiveCharacterL = new Actor(textureID);
                ActiveCharacterL->setupVertex({ -1.2f, 0.7f, 0.0f }, { 0.3f, 0.7f, 0.0f }, { -1.2f, -1.5f, 0.0f }, { 0.3f, -1.5f, 0.0f });
                ActiveCharacterL->reflect = true;
                ActiveCharacterL->name = data->second.Name;

                //确保这里的操作只进行一次
                nextAction();
            }
            else if (ActiveCharacterL->unloading == false) {
                ActiveCharacterL->loading = false;
                ActiveCharacterL->unloading = true;
            }
        }
        else {
            if (ActiveCharacterR == nullptr || ActiveCharacterR->transparency <= 0.0f) {
                delete ActiveCharacterR;
                auto data = ActorTable.find(actorKey);
                unsigned int textureID = data->second.Texture;
                ActiveCharacterR = new Actor(textureID);
                ActiveCharacterR->setupVertex({ -0.3f, 0.7f, 0.0f }, { 1.2f, 0.7f, 0.0f }, { -0.3f, -1.5f, 0.0f }, { 1.2f, -1.5f, 0.0f });
                ActiveCharacterR->name = data->second.Name;

                nextAction();
            }
            else if (ActiveCharacterR->unloading == false) {
                ActiveCharacterR->loading = false;
                ActiveCharacterR->unloading = true;
            }
        }
        break;
    }
    case ActionType::EXIT: {
        std::wstring arg1;
        std::wstring arg2;
        auto itr = curAction->data.begin();
        arg1 = *itr;
        ++itr;
        arg2 = *itr;

        if (arg2 == L"K") {
            if (arg1 == L"L") {
                ActiveCharacterL->unloading = true;
            }
            else {
                ActiveCharacterR->unloading = true;
            }
        }
        else {
            auto data = ActorTable.find(arg1);
            std::wstring name = data->second.Name;

            if (ActiveCharacterL->name == name) {
                ActiveCharacterL->unloading = true;
            }
            else if (ActiveCharacterR->name == name) {
                ActiveCharacterR->unloading = true;
            }
            else {
                std::wstring msg = L"No such character named " + name + L". No character will exit.";
                RTWarning(msg.c_str());
            }
        }
        nextAction();
        break;
    }
    case ActionType::ATTACK: {
        if (!ActionInitFlag) {
            auto itr = curAction->data.begin();
            std::wstring name = ActorTable.find(*itr)->second.Name;

            mask->visible = true;
            if (ActiveCharacterL->name == name) {
                attack->enable('L');
            }
            else {
                attack->enable('R');
            }
            
            ActionInitFlag = true;
        }

        if (attack->finished) {
            mask->visible = false;
            attack->disable();
            nextAction();
            break;
        }
        break;
    }
    case ActionType::DELAY: {
        if (!ActionInitFlag) {
            auto itr = curAction->data.begin();
            std::wstring delayMs = *itr;
            delayFrame = static_cast<int>(0.06f * _wtoi(delayMs.c_str()));

            ActionInitFlag = true;
        }
        if (delayFrame <= 0) {
            nextAction();
        }
        else {
            --delayFrame;
        }
        
        break;
    }
    case ActionType::RETREAT: {
        if (!ActionInitFlag) {
            auto itr = curAction->data.begin();
            std::wstring name = ActorTable.find(*itr)->second.Name;

            if (ActiveCharacterL->name == name) {
                retreat->enable('L');
                //ActiveCharacterL->unloading = true;
            }
            else {
                retreat->enable('R');
                //ActiveCharacterL->unloading = true;
            }

            ActionInitFlag = true;
        }

        if (retreat->finished) {
            retreat->disable();
            nextAction();
            break;
        }
        break;
    }
    case ActionType::PLAYSOUND: {
        if (!ActionInitFlag) {
            std::wstring id = *(curAction->data.begin());
            InMemorySoundData data = SoundTable.find(id)->second;

            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
            if (data.loop) {
                std::string f = converter.to_bytes(data.Filename);
                    //SoundEngine = irrklang::createIrrKlangDevice();
                    SoundEngine->play2D(f.c_str(), GL_TRUE);
            }
            else {
                std::string f = converter.to_bytes(data.Filename);
                SoundEngine->play2D(f.c_str(), GL_FALSE);
            }
        }
        nextAction();
        break;
    }
    default:
        nextAction();
    }
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
