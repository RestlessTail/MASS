//Background.h : 此文件用于生成背景图，同时储存了地图上可部署位点的坐标等信息

#pragma once
#include "RectangleObject.h"

class Scene : public RectangleObject{
public:
	Scene(unsigned int t);
	void paint() override;
	bool unloading;
	bool loading;

	float transparency;
private:
};