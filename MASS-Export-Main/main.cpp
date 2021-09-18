#include "../MyArinightsScriptScript/MyArknightsScriptScript.h"
#include <stdio.h>
#include <filesystem>
#include <Windows.h>

#pragma comment(linker, "/subsystem:\"windows\"  /entry:\"mainCRTStartup\"")
#pragma warning(disable:4996)

extern std::filesystem::path execPath;
extern std::filesystem::path scriptPath;

int main(int argc, char* argv[]) {

#ifdef _DEBUG
	execPath = std::filesystem::current_path();
#else
	TCHAR exePathCstr[256] = { 0 };
	GetModuleFileName(0, exePathCstr, 255);
	execPath = std::filesystem::path(exePathCstr);
	execPath = execPath.parent_path();
	std::filesystem::current_path(execPath);
#endif

	MyArknightsScriptScript m("script\\main.dat");
	m.exec();
	return 0;

}