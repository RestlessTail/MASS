#pragma once
#include <string>
#include "Sound.h"
#include <map>

//TODO: 剧情选择框
//TODO: 窗口抖动
//TODO: 人物表情切换
//TODO: 中央字幕

struct __declspec(dllexport) Actor_t {
	std::wstring name;
	int activeTexture;
	std::map<std::wstring, int> textureMap;
};
typedef irrklang::ISoundSource SoundSource_t;

namespace MASSAction {
	class __declspec(dllexport) Action {
	public:
		virtual void exec() = 0;
	};

	class __declspec(dllexport) Finish : public Action {
	public:
		Finish();
		void exec() override;
	};

	class __declspec(dllexport) Voiceover : public Action {
	public:
		Voiceover(std::wstring& s);
		Voiceover(const wchar_t* s);
		void exec() override;
		std::wstring content;
	};

	class __declspec(dllexport) Say : public Action {
	public:
		Say(std::wstring& n, std::wstring& c);
		Say(const wchar_t* n, const wchar_t* c);
		Say(int actor, std::wstring& c);
		Say(int actor, const wchar_t* c);
		void exec() override;
		std::wstring name;
		std::wstring content;
	};

	class __declspec(dllexport) UseScene : public Action {
	public:
		UseScene(int texture);
		void exec() override;
		unsigned int textureID;
	};

	class __declspec(dllexport) Enter : public Action {
	public:
		Enter(int a, char pos);
		void exec() override;
		Actor_t* actor;
		char position;
	};

	class __declspec(dllexport) Exit : public Action {
	public:
		Exit(char pos);
		Exit(int actor);
		void exec() override;
		char position;
	};

	class __declspec(dllexport) Attack : public Action {
	public:
		Attack(int a);
		Attack(char pos);
		void exec() override;
		char position;
	};

	class __declspec(dllexport) Delay : public Action {
	public:
		Delay(int value);
		void exec() override;
		int val;
	};

	class __declspec(dllexport) Retreat : public Action {
	public:
		Retreat(int a);
		Retreat(char pos);
		void exec() override;
		char position;
	};

	class __declspec(dllexport) PlaySound : public Action {
	public:
		PlaySound(int soundID, int trackIndex, bool isLoop = false);
		void exec() override;
		bool loop;
		int track;
		int soundSource;
	};

	class __declspec(dllexport) StopSound : public Action {
	public:
		StopSound(int trackIndex);
		void exec() override;
		int track;
	};
}
