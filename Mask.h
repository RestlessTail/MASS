#pragma once
#include "RectangleObject.h"
class Mask : public RectangleObject{
public:
	Mask();
	void setColor(float R, float G, float B, float A);
	void paint() override;
	float r;
	float g;
	float b;
	float a;
};

