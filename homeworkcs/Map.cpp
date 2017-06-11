#include<GL/glut.h>
#include "Map.h"
#include<stdio.h>
#include<iostream>
#include<string>
using namespace std;
#include"game.h"
GLuint SkyboxTexture[6];
extern Box maprect[20];
struct ROLE MapEnemyArray[6];
GLuint enemyTexture;
GLuint ground;
int mapnum;
int enemynummax;
int enemyNum;
extern Game game;
GLuint wall;
GLuint enemyDisplayTexture;
void texture(GLuint textur)
{
	glBindTexture(GL_TEXTURE_2D, textur);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
}

//指定贴图
void texture0(GLuint textur)
{
	glBindTexture(GL_TEXTURE_2D, textur);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}
extern int iAttackLife;

void LoadMap() {
	FILE *fp;
	fp = fopen(MAP_PATH, "r");
	if (fp == NULL) { 
		cout << "File Map Error" << endl;
		return; 
	}
	int i = 0;
	while (fscanf(fp, "%f %f %f %f %f %f %f\n", &maprect[i].x, &maprect[i].y, &maprect[i].z, &maprect[i].length, &maprect[i].width, &maprect[i].height, &maprect[i].angle) != EOF)
	{
		printf("%f %f %f %f %f %f %f\n", maprect[i].x, maprect[i].y, maprect[i].z, maprect[i].length, maprect[i].width, maprect[i].height, maprect[i].angle);
		i++;
	//	printf("id:%s name:%s department:%s\n", record.id, record.name, record.department); /* 显示到屏幕*/
	}
	mapnum = i;
	fclose(fp);

	memset(MapEnemyArray, 0, sizeof(MapEnemyArray));
	enemynummax = 0;
	fp = fopen(ENEMY_PATH, "r");
	if (!fp)
	{
		cout << "File Enemy Error" << endl;
		return ;
	}
	//找到了某一关的地图数据
	//enemy数据
	i = 0;
	while (fscanf(fp, "%d %f %f %d %d %d %f \n",&MapEnemyArray[i].id,
		&MapEnemyArray[i].x,
		&MapEnemyArray[i].z,
		&MapEnemyArray[i].hp,
		&MapEnemyArray[i].iAngle,
		&MapEnemyArray[i].iMoveDir,
		&MapEnemyArray[i].iMoveSpeed) != EOF)
	{
		printf("%d %f %f %d %d %d %f\n", MapEnemyArray[i].id,
			MapEnemyArray[i].x,
			MapEnemyArray[i].z,
			MapEnemyArray[i].hp,
			MapEnemyArray[i].iAngle,
			MapEnemyArray[i].iMoveDir,
			MapEnemyArray[i].iMoveSpeed);
			MapEnemyArray[i].show = true;
			MapEnemyArray[i].iCurrentAni = ENEMY_RUN_ANI;

		i++;
	}
	enemynummax = i;
	enemyNum = i;

}
void checkRole()
{
	static int clock = 0;
	clock++;
	if (clock % 20 != 0) {
		return;
	}
	else clock = 0;
	int i, j;
	int iTurn;//是否反向运动
	struct animation	*pAni;

	for (i = 0; i<enemynummax; i++)
	{
		if (!MapEnemyArray[i].show)
			continue;

		//刷新帧
		pAni = &(game.m_anmobj->anim[0][MapEnemyArray[i].iCurrentAni]);
		switch (MapEnemyArray[i].iCurrentAni)
		{
		case 1:
			//run
			MapEnemyArray[i].iframe++;
			if (MapEnemyArray[i].iframe > pAni->end)
			{
				//循环播放
				MapEnemyArray[i].iframe = pAni->start;
			}
			break;

		default:
			if (MapEnemyArray[i].iframe < pAni->end)
			{
				MapEnemyArray[i].iframe++;
			}
			break;
		}
		
	}// end for

	for(int i = 0; i<enemynummax; i++)
	{
		if (!MapEnemyArray[i].show)
			continue;

		//如果是跑步动作, 刷新坐标
		if (ENEMY_RUN_ANI != MapEnemyArray[i].iCurrentAni)
			continue;

		//run
		switch (MapEnemyArray[i].iMoveDir)
		{
		case MOVE_X:
			MapEnemyArray[i].x += MapEnemyArray[i].iMoveSpeed;
			break;

		case MOVE_Z:
			MapEnemyArray[i].z += MapEnemyArray[i].iMoveSpeed;
			break;

		default:
			break;
		}

		iTurn = 0;//是否反向运动
					//碰撞检测
		if (!checkCamera(MapEnemyArray[i].x,
			MapEnemyArray[i].z,
			maprect))
		{
			iTurn = 1;
		}
		if (iTurn)
		{
			MapEnemyArray[i].iMoveSpeed = -MapEnemyArray[i].iMoveSpeed;//速度矢量取反
																		//角度取反（0 1 2 3表示四个方向 0 x轴正向）
			MapEnemyArray[i].iAngle = (MapEnemyArray[i].iAngle + 2) % 4;
		}
	}
}

void drawMap() {
	for (int i = 0; i < mapnum; i++) {
		Draw_Box(maprect[i].x, maprect[i].y, maprect[i].z, maprect[i].length, maprect[i].width, maprect[i].height,& wall);
	}
}
void  LoadTexture(GLuint *SkyboxTexture, char * path, int type) {
	//string str(path);
	IplImage * image = cvLoadImage(path);
	if (image == NULL) {
		cout << "loadfailed!" << endl;
		return;
	}
	IplImage * image2 = cvCreateImage(cvSize(image->width, image->height), 8, 3);
	cvConvertImage(image, image2, CV_CVTIMG_SWAP_RB);
	cvFlip(image2, NULL, -1);
	cvFlip(image2, NULL, 1);
	glGenTextures(1, &SkyboxTexture[type]);
	glBindTexture(GL_TEXTURE_2D, SkyboxTexture[type]); //bind the texture to it's array
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height,
		0, GL_RGB, GL_UNSIGNED_BYTE, image2->imageData);
	cvReleaseImage(&image);
	cvReleaseImage(&image2);
}
void  LoadTexture(char * path,GLuint *SkyboxTexture) {
	IplImage * image = cvLoadImage(path);
	if (image == NULL) {
		cout << "loadfailed!" << endl;
		return;
	}
	IplImage * image2 = cvCreateImage(cvSize(image->width, image->height), 8, 3);
	cvConvertImage(image, image2, CV_CVTIMG_SWAP_RB);
	cvFlip(image2, NULL, -1);
	cvFlip(image2, NULL, 1);
	glGenTextures(1, SkyboxTexture);
	glBindTexture(GL_TEXTURE_2D,*SkyboxTexture); //bind the texture to it's array
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height,
		0, GL_RGB, GL_UNSIGNED_BYTE, image2->imageData);
	cvReleaseImage(&image);
	cvReleaseImage(&image2);
}
void Draw_Skybox(float x, float y, float z, float width, float height, float length,GLuint *SkyboxTexture)
{
	glDisable(GL_LIGHTING);
	// Center the Skybox around the given x,y,z position
	x = x - width / 2;
	//y = y - height / 2;
	z = z - length / 2;
	// Draw Front side
	glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYFRONT]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glEnd();

	// Draw Back side
	glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYBACK]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glEnd();

	// Draw Left side
	glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYLEFT]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glEnd();

	// Draw Right side
	glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYRIGHT]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glEnd();

	// Draw Up side
	glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYUP]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glEnd();

	// Draw Down side
	glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYDOWN]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z);
	glEnd();
	glEnable(GL_LIGHTING);
}

void Draw_Box(float x, float y, float z, float length, float width, float height, GLuint *SkyboxTexture)
{
	float ftex = 5.0f;
	texture0(*SkyboxTexture);
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(ftex, 0.0f); glVertex3f(x, y, z + length);
	glNormal3f(0, 0, 1);
	glTexCoord2f(ftex, ftex); glVertex3f(x, y + height, z + length);
	glNormal3f(0, 0, 1);
	glTexCoord2f(0.0f, ftex); glVertex3f(x + width, y + height, z + length);
	glNormal3f(0, 0, 1);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);

	glNormal3f(0, 0, -1);
	glTexCoord2f(ftex, 0.0f); glVertex3f(x + width, y, z);
	glNormal3f(0, 0, -1);
	glTexCoord2f(ftex, ftex); glVertex3f(x + width, y + height, z);
	glNormal3f(0, 0, -1);
	glTexCoord2f(0.0f, ftex); glVertex3f(x, y + height, z);
	glNormal3f(0, 0, -1);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);

	glNormal3f(-1, 0, 0);
	glTexCoord2f(0.0f, ftex); glVertex3f(x, y + height, z + length);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(ftex, ftex); glVertex3f(x, y + height, z);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(ftex, 0.0f); glVertex3f(x, y, z);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(ftex, 0.0f); glVertex3f(x, y, z+length);

	glNormal3f(1, 0, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glNormal3f(1, 0, 0);
	glTexCoord2f(ftex, 0.0f); glVertex3f(x + width, y, z + length);
	glNormal3f(1, 0, 0);
	glTexCoord2f(ftex, ftex); glVertex3f(x + width, y + height, z + length);
	glNormal3f(1, 0, 0);
	glTexCoord2f(0.0f, ftex); glVertex3f(x + width, y + height, z);

	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z);
	glNormal3f(0, 1, 0);
	glTexCoord2f(ftex, 0.0f); glVertex3f(x + width, y + height, z + length);
	glNormal3f(0, 1, 0);
	glTexCoord2f(ftex, ftex); glVertex3f(x, y + height, z + length);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0f, ftex); glVertex3f(x, y + height, z);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glNormal3f(0, -1, 0);
	glTexCoord2f(ftex, 0.0f); glVertex3f(x, y, z + length);
	glNormal3f(0, -1, 0);
	glTexCoord2f(ftex, ftex); glVertex3f(x + width, y, z + length);
	glNormal3f(0, -1, 0);
	glTexCoord2f(0.0f, ftex); glVertex3f(x + width, y, z);
	glEnd();

}
