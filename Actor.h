//Character.h : ���ļ����������и�Ա�͵��˹��е���Ϊ

#pragma once
#include "RectangleObject.h"
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Actor : public RectangleObject{
public:
	Actor(unsigned int t);
	void paint() override;
	float brightness;
	float transparency;
	bool unloading;
	bool loading;
	bool reflect;

	glm::mat4 transform;
	std::wstring name;
};

