#pragma once
class Retreat
{
public:
	Retreat();
	float x;//���ڼ���͸���ȡ���ɫƫ�����ĺ������Ա���
	void paint();
	void enable(char c);//����cӦ����L��R������˵����˭�ڳ���
	void disable();
	bool disabled;
	bool finished;
	char actorPos;

};

