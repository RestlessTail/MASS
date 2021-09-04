//RectangleObject.h : 此文件用于调用OpenGL绘制矩形物体

#pragma once
#include <vector>
#include <string>
#include "TextureLoader.h"

struct VertexCoord {
	float x;
	float y;
	float z;
};

struct TextureCoord {
	float x;
	float y;
};

struct GlyphRange {
	TextureCoord topLeft;
	TextureCoord topRight;
	TextureCoord bottomLeft;
	TextureCoord bottomRight;
};

struct CoordRange {
	VertexCoord topLeft;
	VertexCoord topRight;
	VertexCoord bottomLeft;
	VertexCoord bottomRight;
};

class RectangleObject{
public:
	unsigned int VAO;//储存顶点信息
	RectangleObject();
	~RectangleObject();
	virtual void paint();
	virtual void setupVertex(VertexCoord topLeft, VertexCoord topRight, VertexCoord bottomLeft, VertexCoord bottomRight);//加载顶点信息，注意只能调用一次，不然内存不能释放
	virtual void setupTextures(std::string path);//从路径加载材质
	virtual void useTexture(unsigned int t); //加载现有的材质
	void setTextureVertex(GlyphRange range);
	void setVertex(VertexCoord topLeft, VertexCoord topRight, VertexCoord bottomLeft, VertexCoord bottomRight);

	unsigned int texture;

	void applyTexture();//将材质应用到OpenGL，在paint()中调用
	void applyVertex();//将顶点（VAO）应用到OpenGL，在paint()中调用
	void callOpenGL();//调用OpenGL绘图

	bool visible;
	inline float* getVertex() {
		return vertex;
	}

private:
	unsigned int VBO;
	unsigned int EBO;
	float vertex[20];
	
};

