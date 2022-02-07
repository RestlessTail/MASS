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
	void updateyPos(); //����curCharacter����������

	int curCharacter; //�Ѿ���ʾ���ڼ�����
	int addCharacterDelay; //��������ʾ��һ����
	int curDelay; //����Ѿ���ȥ�˶��
	int rowSpace; //�о�
	LineColor color;
	LineAlignment alignment;

	float xPos; //��һ���ֵĺ�����
	float yPos; //��һ���ֵ�������
	float linePosX;
	float linePosY;

	//static glm::mat4 projection;

	std::vector<LineCharacter*> characters;

	bool visible;
	float scale;
	//��ȡ��n���ַ�������
	//void GetCharacterVertexAt(int n, VertexCoord* topLeft, VertexCoord* topRight, VertexCoord* bottomLeft, VertexCoord* bottomRight);
};

