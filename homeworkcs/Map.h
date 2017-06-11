#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include "stdafx.h"
#include<GL/glut.h>
#include "ImageLoader.h"
#define SKYFRONT 0  //Front ID
#define SKYBACK 1  //Back ID
#define SKYLEFT 2  //Left ID
#define SKYRIGHT 3  //Right ID
#define SKYUP 4  //Up ID
#define SKYDOWN 5  //Down ID
#define MAP_PATH "data\\map\\map.txt"
#define ENEMY_PATH "data\\map\\enemy.txt"
#define ENEMY_DEATH_ANI 17
#define ENEMY_RUN_ANI   1
//人物移动坐标轴
#define MOVE_X 0
#define MOVE_Y 1
#define MOVE_Z 2
void LoadTexture(GLuint *SkyboxTexture, char * path, int type);
void LoadTexture(char * path,GLuint *SkyboxTexture);

void Draw_Skybox(float x, float y, float z, float width, float height, float length, GLuint *SkyboxTexture);
void Draw_Box(float x, float y, float z, float width, float height, float length, GLuint *SkyboxTexture);
void drawMap();
typedef struct {
	float x,y,z,length,width,height,angle;
	GLuint tex;
}Box;
void LoadMap();
void checkRole();
bool checkCamera(float &x, float &y, Box * maprect);
struct ROLE
{
	int id;	//ID
			//坐标
	float x;
	float y;
	float z;

	int w;
	int h;

	int iframe;
	int iCurrentAni;

	//移动部分
	int iAngle;
	int iMoveDir;
	float iMoveSpeed;

	//人物属性
	int hp;//生命值
	int hpMax;

	int show;	//是否显示

	int r;//旋转角度
};
#endif ///__SKYBOX_H__