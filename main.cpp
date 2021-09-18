#include "MyArknightsScriptScript.h"
#include <stdio.h>
#include "ExportMASS.h"
#include <Windows.h>
#include "main.h"

#pragma warning(disable:4996)

extern std::filesystem::path execPath;
extern std::filesystem::path scriptPath;

bool ExportScript;
bool ShowHelpInfo;
bool RunMainProgram;
std::string ExportPath;
std::string ScriptPath;

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

	int res = ParseArgs(argc, argv);
	switch (res) {
	case 0: ExecMASS(); return 0;
	case -1: printf("Fatal error: no input file.\n\n"); ShowHelp(); return -1;
	case -2: printf("Fatal error: misused argument.\n\n"); ShowHelp(); return -1;
	}

}

//返回值0：正确解析
//返回值-1：参数太少
//返回值-2：语法错误
int ParseArgs(int argc, char* argv[])
{
	ExportScript = false;
	ShowHelpInfo = false;
	RunMainProgram = true;

	if (argc <= 1) {
		return -1;
	}
	else {
		ScriptPath = argv[1];
	}
	if (argc == 2) {
		return 0;
	}
	else {
		int curArg = 2;
		do {
			if (!strcmp(argv[curArg], "-o")) {
				curArg = ParseExport(argc, argv, curArg);
			}
			else {
				return -2;
			}
		} while (curArg != -1 && curArg < argc);
		return 0;
	}
}

int ParseExport(int argc, char* argv[], int curArg)
{
	if (curArg + 1 >= argc) {
		return -2;
	}
	ExportScript = true;
	RunMainProgram = false;
	++curArg;
	ExportPath = argv[curArg];
	++curArg;
	return curArg;
}

int ParseHelp(int argc, char* argv[], int curArg)
{
	ShowHelpInfo = true;
	++curArg;
	return curArg;
}

int ExecMASS()
{
	if (ExportScript) {
		ExportMASS e(ScriptPath.c_str(), ExportPath.c_str());
		e.Export();
	}
	if (RunMainProgram) {
		MyArknightsScriptScript m(ScriptPath.c_str());
		m.exec();
	}
	if (ShowHelpInfo) {
		ShowHelp();
	}
	return 0;
}

int ShowHelp()
{
	printf("Usage:\n");
	printf("    MASS inputfile [options]\n\n");
	printf("Options:\n");
	printf("    -o exportpath\tExport the script.\n");
	return 0;
}
