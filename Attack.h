#pragma once
class Attack
{
public:
	Attack();
	void paint();
	void enable(char c);//����cӦ����L��R������˵����˭�ڹ���
	void disable();
	bool disabled;
	bool finished;
	float x; //���ڼ������ȡ���ɫƫ�����ĺ������Ա���
	char actorPos;
};

