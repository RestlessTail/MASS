#pragma once
class Attack
{
public:
	Attack();
	void paint();
	void enable(char c);//参数c应当填L或R，用以说明是谁在攻击
	void disable();
	bool disabled;
	bool finished;
	float x; //用于计算亮度、角色偏移量的函数的自变量
	char actorPos;
};

