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
	md2_model_t*	model[2];	//����MD2ģ��
	animation*	anim[2];	//����ģ�͵Ķ�������
	int		size[2];	//��������
	
	//��ʼ��ģ��
	void getobj(std::string na);
	//��ȡģ�͵Ķ�����Ϣ
	animation*		animations (md2_model_t* model,int p);
	//����ģ��
	//��� ID	���� ֡
	void drawModel(int id,float x,float y,float z,int iframe,int iAngle);

};
