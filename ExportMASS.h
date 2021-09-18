#pragma once
#include "./interpreter/lexer.h"
#include "./interpreter/syntax.h"
#include <string>
#include <filesystem>
class ExportMASS
{
public:
	ExportMASS(const char* src, const char* dest);
	void Export();
	void ExportDef();
	void ExportEssential();
	std::wstring scriptContent;
	std::filesystem::path destPath;
	std::filesystem::path destScenePath;
	std::filesystem::path destScriptPath;
	std::filesystem::path destCharacterPath;
	std::error_code errCode;
	std::wstring projName;
};

