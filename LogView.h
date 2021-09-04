#pragma once
#include "RectangleObject.h"
#include <string>
#include <vector>
#include "Line.h"

struct LogItem {
	wchar_t* name;
	wchar_t* content;
};

class LogView
{
public:
	LogView();
	void paint();
	static float cursor;//记录当前屏幕最上端到所有行的最上端中间差了几个行的距离
	void addLog(std::wstring name, std::wstring& content);//name可能为L""，而content一定不为空。所以name不能传引用
	std::vector<LogItem> allLogs;
	std::vector<LineCharacter*> characters;
	float scale;
	bool visible;

	static bool allowScroll;
};

