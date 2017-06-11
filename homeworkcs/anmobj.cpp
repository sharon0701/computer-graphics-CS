#include "stdafx.h"
#include<gl/glut.h>
#include "anmobj.h"

//using namespace std;
int dongzuo[7]={0,2,2,7,12,14,14};


anmobj::anmobj()
{
}
anmobj::~anmobj()
{}
void anmobj::getobj(std::string na)//
{
	anim[0]=anim[1]=NULL;	
	
	if(model[0]!=NULL)
	{
		delete[] model[0];
		model[0]=NULL;
	}
	
	if(model[1]!=NULL)
	{
		delete[] model[1];
		model[1]=NULL;
	}
	std::string str = na + "tris.md2";
	model[0]   = md2_readModel(str.c_str());
	str = na + "weapon.md2";
	model[1]   = md2_readModel(str.c_str());
	

	if(model[0]!=NULL) 
	{
		size[0] = md2_getAnimationCount(model[0]);
		// ��ȡ��������
		animations (model[0],0);//
	}

	if(model[1]!=NULL)
	{
		size[1] = md2_getAnimationCount(model[1]);
		animations (model[1],1);
	}


}

animation* anmobj::animations (md2_model_t* model,int p)
{	
	if (size[p] == 0) 
		return 0;//

	if(anim[p]!=NULL) 
		delete[] anim[p];//
	
	anim[p] = new animation[size[p]];//
	//���ø�ģ�͵����ж�����Ϣ
	for (int i=0; i<size[p]; i++)//
	{
		//�õ���������
		strcpy (anim[p][i].name,md2_getAnimationName(model,i));//
		//�õ�������ʼ����֡
		md2_getAnimationFrames(model,i,&anim[p][i].start,&anim[p][i].end);//
	}
	return 0;//
}

void anmobj::drawModel(int id,float x,float y,float z,int iframe,int iAngle)
{
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();

	glTranslatef(x,y,z);
	//��ת
	glRotatef(iAngle*90,0,1,0);
	//��С
	glScaled(0.8f,0.8f,0.8f);

	//role ģ�Ͳ�Ϊ�գ��� iframeС�����֡��
	if(model[0]!=NULL && model[0]->header.numFrames>iframe)
	{
		md2_drawModel (model[0],iframe,0,0);
	}
	//weapon
	if(model[1]!=NULL && model[1]->header.numFrames>iframe)
	{	
		md2_drawModel (model[1],iframe,0,0);
	}

	glPopMatrix();
	glPopAttrib();
}