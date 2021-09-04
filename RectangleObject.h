//RectangleObject.h : ���ļ����ڵ���OpenGL���ƾ�������

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
	unsigned int VAO;//���涥����Ϣ
	RectangleObject();
	~RectangleObject();
	virtual void paint();
	virtual void setupVertex(VertexCoord topLeft, VertexCoord topRight, VertexCoord bottomLeft, VertexCoord bottomRight);//���ض�����Ϣ��ע��ֻ�ܵ���һ�Σ���Ȼ�ڴ治���ͷ�
	virtual void setupTextures(std::string path);//��·�����ز���
	virtual void useTexture(unsigned int t); //�������еĲ���
	void setTextureVertex(GlyphRange range);
	void setVertex(VertexCoord topLeft, VertexCoord topRight, VertexCoord bottomLeft, VertexCoord bottomRight);

	unsigned int texture;

	void applyTexture();//������Ӧ�õ�OpenGL����paint()�е���
	void applyVertex();//�����㣨VAO��Ӧ�õ�OpenGL����paint()�е���
	void callOpenGL();//����OpenGL��ͼ

	bool visible;
	inline float* getVertex() {
		return vertex;
	}

private:
	unsigned int VBO;
	unsigned int EBO;
	float vertex[20];
	
};

