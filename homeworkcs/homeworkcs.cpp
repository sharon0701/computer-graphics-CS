// homeworkcs.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include<string>
#include<iostream>
#include<gl/glut.h>
#include <stdio.h>
#include <stdexcept>
#include<opencv\cv.h>
#include<opencv2\highgui.hpp>
#include"game.h"
#include"Map.h"
#include"MDLModel.h"
#include"GLFont.h"

#define MAX_MAP_OBJECT 20
TMDLModel *tmd;
using namespace std;
Game game;
int life = 2;
#define ENEMY_MODEL_Y 25.0f
extern int enemynummax;
extern int enemyNum;
extern GLuint wall;
extern int mapnum;
extern GLuint ground;
extern Box maprect[20];
extern GLuint SkyboxTexture[6];
extern  ROLE MapEnemyArray[6];
extern GLuint enemyTexture;
extern GLuint enemyDisplayTexture;
float bulletx,bullety,bulletz,bulletcita,bulletgamma;
bool isFire;
int iAttackLife;
int iAttackMaxLife;
int a = 0;

void showEnemyHp()
{
	int i;
	unsigned char rasterallf[12] = {
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff
	};

	glLoadIdentity();
	//属性进栈
	glPushAttrib(GL_CURRENT_BIT);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	//颜色条
	//坐标移动
	glTranslatef(-0.1f, 0.15, -1.0);
	//绿色显示生命
	glColor3f(0.0, 1.0, 0.0);
	glRasterPos3f(0.0, 0.0, 0.0);
	for (i = 0; i<iAttackLife / 10; i++)
	{
		glBitmap(8, 8, 0.0, 0.0, 8.0, 0.0, rasterallf);
	}
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glPopAttrib();
}


void showTarget()
{
	unsigned char rasterallf[12] = {
		0xff, 0xff,0xff, 0xff,0xff, 0xff,
		0xff, 0xff,0xff, 0xff,0xff, 0xff
	};

	glLoadIdentity();
	//属性进栈
	glPushAttrib(GL_CURRENT_BIT);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	//指定颜色
	glColor3f(0.0, 1.0, 0.0f);
	//坐标转换，移动
	glTranslatef(0.0, -0.033, -1.0f);

	//在RC中输出
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//设置光栅位置
	glRasterPos2i(0, 0);
	glBitmap(2, 12, 0.0, 0.0, 0.0, 23.0, rasterallf);
	glBitmap(2, 12, 0.0, 0.0, -16.0, -6.0, rasterallf);
	glBitmap(12, 2, 0.0, 0.0, 22.0, 0.0, rasterallf);
	glBitmap(12, 2, 0.0, 0.0, 0.0, 0.0, rasterallf);
	glEnable(GL_TEXTURE_2D);
	glPopAttrib();
	glEnable(GL_LIGHTING);

}
void MousePress(int button, int state, int x, int y) {
	static bool fire=false;
	if (button == GLUT_LEFT_BUTTON) {
		switch (state){
		case GLUT_DOWN:
			isFire = true;
			bulletx = game.camera.posx;
			bullety = game.camera.posy;
			bulletz = game.camera.posz;
			bulletcita = game.camera.cita;
			bulletgamma = game.camera.gamma;
			if(!fire)
				tmd->SetSequence(1);
			break;
		case GLUT_UP:
			tmd->SetSequence(0);
			fire = false;
			break;
		}
	}
	glutPostRedisplay();
}

void init() {
	tmd = new  TMDLModel();
	glEnable(GL_DEPTH);
	tmd->Init(".\\data\\mdl\\qian1.mdl");
	tmd->SetSequence(0);
	LoadTexture(SkyboxTexture, "data/texture/orangecream_front.jpg", SKYFRONT);
	LoadTexture(SkyboxTexture, "data/texture/orangecream_back.jpg", SKYBACK);
	LoadTexture(SkyboxTexture, "data/texture/orangecream_left.jpg", SKYLEFT);
	LoadTexture(SkyboxTexture, "data/texture/orangecream_right.jpg", SKYRIGHT);
	LoadTexture(SkyboxTexture, "data/texture/orangecream_top.jpg", SKYUP);
	LoadTexture(SkyboxTexture, "data/texture/bottom.bmp", SKYDOWN);
	LoadTexture("data/texture/box0.bmp", &wall);
	LoadTexture("data/texture/map5.bmp", &ground);
	LoadTexture("data/images/TRIX1.bmp", &enemyTexture);
	LoadTexture("data/images/h1.bmp", &enemyDisplayTexture);
	LoadMap();
	game.m_anmobj = new anmobj;
	game.m_anmobj->model[0] = NULL;
	game.m_anmobj->model[1] = NULL;
	game.m_anmobj->getobj("data/md2/role/");
	bulletx = bullety = bulletz;
	isFire = false;
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glShadeModel(GL_SMOOTH);
	GLfloat mat_specular[] = { 1, 1, 1, 1 };
	GLfloat mat_shininess[] = { 50 };
	GLfloat white_light[] = { 1, 1, 1, 0 };
	GLfloat lmodel_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat light_position[] = { 10.0, 10.0, 3.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//glClearDepth(1);
	//glClearColor(0, 0, 0, 1);

}

int hittest()
{
	int i;
	int iHit = 0;
	float x, y, z;

	//原理：子弹每次刷新坐标,增加距离10.0
	//		检测时，从当前坐标循环10次,每次增加一个单位做检测
	if (isFire)
	{
		x = bulletx;
		y = bullety;
		z = bulletz;
		iHit = 0;
		for (int k = 0; k<10; k++)
		{
			for (i = 0; i<enemynummax; i++)
			{
				if (!MapEnemyArray[i].show)
					continue;

				if (MapEnemyArray[i].hp <= 0)
					continue;
				if (abs(MapEnemyArray[i].x - x)<10.f
					&&	abs(MapEnemyArray[i].y + ENEMY_MODEL_Y - y)<30.f
					&&	abs(MapEnemyArray[i].z - z)<10.f)
				{
					MapEnemyArray[i].hp -= 20;
					cout<<"hp "<< MapEnemyArray[i].hp<<endl;
					if (MapEnemyArray[i].hp <= 0)
					{
						//设定动作
						MapEnemyArray[i].iframe = 178;	//death frame
						MapEnemyArray[i].iCurrentAni = ENEMY_DEATH_ANI;
						enemyNum--;
					}
					//攻击对象提示
			
					iAttackLife = MapEnemyArray[i].hp;
					iAttackMaxLife = MapEnemyArray[i].hpMax;
					//param1 = 0;
					iHit = 1;		//已经检测到碰撞
					break;
				}
			}
			if (iHit)
			{
				isFire = 0;	//	
				break;//停止检测
			}

			x -= 10.0f*sin(bulletcita/180.0f*PI);
			z += 10.0f*cos(bulletcita /180.0f*PI);
			y += 10.0f*sin(bulletgamma / 180.0f*PI);
		}
	}//end 子弹和enemy的检测

	 //子弹和障碍物的检测
	if (isFire)
	{
		x =bulletx;
		y = bullety;
		z = bulletz;
		iHit = 0;
		for (int k = 0; k<6; k++)
		{
			if (!checkCamera(x, z, maprect))
			{
				iHit = 1;
			}
			if (iHit)
			{
				//清除
				isFire = 0;	
				break;//碰到障碍
			}
			x += 1.0f*cos(bulletcita / 180.0f*PI);
			z -= 1.0f*sin(bulletcita / 180.0f*PI);
			y += 1.0f*sin(bulletgamma / 180.0f*PI);
		}
	}//end 子弹和障碍物的检测

	 //enemy和玩家的碰撞
	for (i = 0; i<enemynummax; i++)
	{
		if (!MapEnemyArray[i].show)
			continue;

		//不是跑动状态, 不检测
		if (ENEMY_RUN_ANI != MapEnemyArray[i].iCurrentAni)
			continue;

		if (abs(MapEnemyArray[i].x - game.camera.posx)<10.0f
			&&	abs(MapEnemyArray[i].z - game.camera.posz)<10.0f)
		{
			game.iHp -= 10;	//和enemy碰撞,生命值减少
		 	cout <<"hp"<< game.iHp << endl;
			if (game.iHp == 0)
			{
				//	printf("gameover");
				//	system("pause");
				life -= 1;
				cout << "life" << life << endl;
				game.iHp = 100;
				game.camera.posx = 100;
				game.camera.posz = 25;
				break;
			}
		}
	}
	isFire = false;
	return 0;
}
void display() {
	if (life == 0 || enemyNum == 0){
		return;
	}
	checkRole();
	glClearColor(0, 0, 0, 1);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	game.camera.setCamerLook();
	glColor3f(1, 1, 1);
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	Draw_Skybox(0, 0, 0, Sky_width, 150, Sky_height, SkyboxTexture);
	glPopMatrix();
	glPushMatrix();
	drawMap();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(game.camera.posx, game.camera.posy, game.camera.posz);
	glRotated(-90, 1, 0, 0);
	glRotated(270 - game.camera.cita, 0, 0, 1);
	glRotated(-game.camera.gamma, 0, 1, 0);

	tmd->AdvanceFrame2();
	tmd->DrawModel();
	glPopMatrix();

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, enemyTexture);
	for (int i = 0; i<enemynummax; i++)
	{
		if (MapEnemyArray[i].show )
		{
			game.m_anmobj->drawModel(MapEnemyArray[i].id,
				MapEnemyArray[i].x,
				MapEnemyArray[i].y + ENEMY_MODEL_Y,
				MapEnemyArray[i].z,
				MapEnemyArray[i].iframe,
				MapEnemyArray[i].iAngle);
		}
	}
	glPopMatrix();
//
	hittest();
	showTarget();

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glColor3f(0.0f, 0.0f, 0.0f);
	glRasterPos3f(0.0f, 0.5f, 0.0f);
	char s1[20];
	sprintf(s1, "Enemy Number : %d", enemyNum);
	drawString(s1);
	glColor3f(0.0f, 0.0f, 0.0f);
	glRasterPos3f(-0.8f, -0.4f, 0.0f);
	char s2[20];
	sprintf(s2, "My life : %d", life);
	drawString(s2);

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glColor3f(0.0f, 1.0f, 1.0f);
	if (enemyNum == 0)
	{
		glColor3f(0.0f, 0.0f,0.0f);
		glRasterPos3f(-0.2f, 0.0f, 0.0f);
		drawCNString("通关成功，游戏结束！");
	}

	if (life == 0)
	{
		glColor3f(0.0f, 0.0f, 0.0f);
		glRasterPos3f(-0.2f, 0.0f, 0.0f);
		drawCNString("通关失败，游戏结束！");
	}
	if (iAttackLife != 0){
		showEnemyHp();
		glColor3f(1, 1, 1);
	}

	glutSwapBuffers();

	
}
void MousePassive(int x, int y) {
	game.camera.MousePasive(x, y);
	glutPostRedisplay();
}

void reshape(int w,int h) {
	if (w < 1) w = 1;
	if (h <1) h = 1;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)w / (GLfloat)h, 1,1000);
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}
void Keyboard(unsigned char key, int x, int y) {
	int step = 1;

	switch (key) {
	case 'w':
		game.camera.CameraFoward(step);
		break;
	case's':
		game.camera.CameraBackward(step);
		break;
	case'a':
		game.camera.CameraLeft(step);
		break;
	case'd':
		game.camera.CameraRight(step);
		break;
	case'j':
		game.camera.CameraRotateY(-5);
		break;
	case'l':
		game.camera.CameraRotateY(+5);
		break;
	case 27:
		exit(0);
		break;

	}

	glutPostRedisplay();
}

int main(int argc,char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGB);
	glutCreateWindow(argv[0]);
	init();
	glutFullScreen();
	glutDisplayFunc(display);
	glutPassiveMotionFunc(MousePassive);
	glutMouseFunc(MousePress);
	glutIdleFunc(display);
	glutKeyboardFunc(Keyboard);
	glutReshapeFunc(reshape);
	glutMainLoop();
    return 0;
}

