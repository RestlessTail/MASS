#pragma once
#include "RectangleObject.h"
class Button : public RectangleObject{
public:
	Button();
	bool isClicked(float x, float y);
	virtual void onClicked() = 0;
	void setResponseRange(VertexCoord topLeft, VertexCoord topRight, VertexCoord bottomLeft, VertexCoord bottomRight);
	void useDefaultResponseRange();

	bool disabled;
	CoordRange responseRange;
};

