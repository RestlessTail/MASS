#pragma once
#include <string>
#include <list>
#include <vector>
#include "RectangleObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class LineCharacter : public RectangleObject {
public:
	void paint() override;
};

struct LineColor {
	float r;
	float g;
	float b;
};

enum class LineAlignment {
	LEFT,
	RIGHT
};

//typedef std::list<Line> LineBuffer;

class LineControl {
public:
	LineControl(float posx, float posy);
	std::wstring buffer;
	void paint();
	void clear();
	void nextLine();
	void addCharacter();
	void skip();
	void resetCharacterDelay();
	void updateyPos(); //根据curCharacter更新纵坐标

	int curCharacter; //已经显示到第几个字
	int addCharacterDelay; //间隔多久显示下一个字
	int curDelay; //间隔已经过去了多久
	int rowSpace; //行距
	LineColor color;
	LineAlignment alignment;

	float xPos; //下一个字的横坐标
	float yPos; //下一个字的纵坐标
	float linePosX;
	float linePosY;

	//static glm::mat4 projection;

	std::vector<LineCharacter*> characters;

	bool visible;
	float scale;
	//获取第n个字符的坐标
	//void GetCharacterVertexAt(int n, VertexCoord* topLeft, VertexCoord* topRight, VertexCoord* bottomLeft, VertexCoord* bottomRight);
};

