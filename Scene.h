//Background.h : ���ļ��������ɱ���ͼ��ͬʱ�����˵�ͼ�Ͽɲ���λ����������Ϣ

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