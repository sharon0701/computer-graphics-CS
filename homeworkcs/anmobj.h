#pragma once
#include "stdafx.h"
#include"md2.h"
#include<string>
struct animation
{	char name[16];
	int	 start;		
	int	 end;		
};
class anmobj
{
public:
	anmobj();
	virtual ~anmobj();

public:
	md2_model_t*	model[2];	//两个MD2模型
	animation*	anim[2];	//各个模型的动画数据
	int		size[2];	//动作数量
	
	//初始化模型
	void getobj(std::string na);
	//读取模型的动画信息
	animation*		animations (md2_model_t* model,int p);
	//绘制模型
	//入参 ID	坐标 帧
	void drawModel(int id,float x,float y,float z,int iframe,int iAngle);

};
