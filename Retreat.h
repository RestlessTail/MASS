#pragma once
class Retreat
{
public:
	Retreat();
	float x;//用于计算透明度、角色偏移量的函数的自变量
	void paint();
	void enable(char c);//参数c应当填L或R，用以说明是谁在撤退
	void disable();
	bool disabled;
	bool finished;
	char actorPos;

};

