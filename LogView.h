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
	static float cursor;//��¼��ǰ��Ļ���϶˵������е����϶��м���˼����еľ���
	void addLog(std::wstring name, std::wstring& content);//name����ΪL""����contentһ����Ϊ�ա�����name���ܴ�����
	std::vector<LogItem> allLogs;
	std::vector<LineCharacter*> characters;
	float scale;
	bool visible;

	static bool allowScroll;
};

