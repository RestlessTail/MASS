#pragma once
#include "Scene.h"
#include "Actor.h"
#include "GLSLLoader.h"
#include "Line.h"
#include <list>
#include <irrKlang.h>

struct GLFWwindow;
struct ActionTableItem;
class Shadow;
class Button;
class LogView;
class ButtonBack;
class Mask;
class Attack;
class Retreat;

class MyArknightsScriptScript{
public:
	static Actor* ActiveCharacterL;
	static Actor* ActiveCharacterR;
	static GLFWwindow* MainWindow;
	static LineControl* lineControl;
	static LineControl* nameControl;
	static Shadow* shadow;
	MyArknightsScriptScript(const char* filename);
	static Scene* ActiveScene;
	bool FinishActionNow;
	static bool running;//是否离开循环，退出程序
	static GLSLLoader* loader;
	static bool AcceptUserInput;//此时程序是否接受用户输入
	static bool ActionInitFlag;//动作的初始化代码是否执行过
	static bool KeyReleaseFlag;//键盘按下、再放开才记作触发一次，否则一次长按会造成连续触发。这个变量是用来记录键盘按下之后有没有放开过。
	static bool WaitForNextAction;//是否等待用户输入，然后执行下一个动作
	bool MouseReleaseFlag;
	static int windowHeight;
	static int windowWidth;
	static void nextAction();
	void RTError(const wchar_t* msg);
	void RTWarning(const wchar_t* msg);
	void SleepForaWhile();
	void exec();
	void paint();
	void processAction();
	static std::list<Button*> buttonGroup;
	static ButtonBack* buttonBack;
	void processInput();
	static LogView* logView;
	static Mask* mask;
	static Attack* attack;
	static Retreat* retreat;
	static int delayFrame;
	irrklang::ISoundEngine* SoundEngine;
};

