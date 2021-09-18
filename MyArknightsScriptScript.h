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
	static bool running;//�Ƿ��뿪ѭ�����˳�����
	static GLSLLoader* loader;
	static bool AcceptUserInput;//��ʱ�����Ƿ�����û�����
	static bool ActionInitFlag;//�����ĳ�ʼ�������Ƿ�ִ�й�
	static bool KeyReleaseFlag;//���̰��¡��ٷſ��ż�������һ�Σ�����һ�γ���������������������������������¼���̰���֮����û�зſ�����
	static bool WaitForNextAction;//�Ƿ�ȴ��û����룬Ȼ��ִ����һ������
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

