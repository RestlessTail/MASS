#pragma once
#include <string>
namespace MASSAction {
	class Action {
	public:
		virtual void exec() = 0;
		virtual void Export(std::wstring& str) = 0;
	};

	class Finish : public Action {
	public:
		void exec() override;
		void Export(std::wstring& str) override;
	};

	class Voiceover : public Action {
	public:
		void exec() override;
		void Export(std::wstring& str) override;
		std::wstring content;
	};

	class Say : public Action {
	public:
		void exec() override;
		void Export(std::wstring& str) override;
		std::wstring key;
		std::wstring content;
	};

	class UseScene : public Action {
	public:
		void exec() override;
		void Export(std::wstring& str) override;
		std::wstring key;
	};

	class Enter : public Action {
	public:
		void exec() override;
		void Export(std::wstring& str) override;
		std::wstring key;
		wchar_t position;
	};

	class Exit : public Action {
	public:
		void exec() override;
		void Export(std::wstring& str) override;
		std::wstring id;
		wchar_t flag;
	};

	class Attack : public Action {
	public:
		void exec() override;
		void Export(std::wstring& str) override;
		std::wstring id;
	};

	class Delay : public Action {
	public:
		void exec() override;
		void Export(std::wstring& str) override;
		int val;
	};

	class Retreat : public Action {
	public:
		void exec() override;
		void Export(std::wstring& str) override;
		std::wstring id;
	};
}
