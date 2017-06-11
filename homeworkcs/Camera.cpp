#include"Camera.h"
#include"Map.h"
Box maprect[20];
extern int mapnum;

bool checkCamera(float &x, float &y, Box * maprect) {
	if (x >  Sky_width/2-20|| x <-Sky_width/2+20 || y >  Sky_height/2-20 || y < -Sky_height/2+20) {
		if (x > Sky_width/2 - 20) x = Sky_width/2 - 20;
		if (x < -Sky_width/2 + 20) x = -Sky_width/2 + 20;
		if (y > Sky_height/2 - 20) y = Sky_height/2 - 20;
		if (y < -Sky_height/2 + 20) y = -Sky_height/2 + 20;
		return false;
	}

	for (int i = 0; i < mapnum; i++) {
		if (x<maprect[i].x + maprect[i].width + 20 &&x>maprect[i].x - 20 &&y<maprect[i].z + maprect[i].length + 20 &&y>maprect[i].z - 20) {
				if((maprect[i].x + maprect[i].width + 20 - x < x - maprect[i].x +20? maprect[i].x + maprect[i].width - x+20 : x - maprect[i].x+20)<(maprect[i].z + maprect[i].length - y+20 < y - maprect[i].z +20? maprect[i].z + maprect[i].length - y+20 :  y - maprect[i].z+20))
				x = maprect[i].x + maprect[i].width + 20 - x < x - maprect[i].x+20 ? maprect[i].x + maprect[i].width+20: maprect[i].x-20;
				else
				y = maprect[i].z + maprect[i].length + 20 - y < y - maprect[i].z +20 ? maprect[i].z + maprect[i].length+20 : maprect[i].z-20;
				return false;
		}
		
	}
	return true;
}

Camera::Camera(float x,float y,float z,float ca, float ga){
	posx = x;
	posy = y;
	posz = z;
	cita = ca;
	gamma = ga;
	centerx = posx - r*sin(cita / 180.0f*PI);
	centerz = posz + r* cos(cita / 180.0f*PI);
	centery = posy += r*sin(gamma / 180.0f*PI);

}

void Camera::CameraUp(int step) {
	posy += step;
	centery += step;
}

void Camera::CameraLeft(int step) {
	posx += step*cos(cita / 180.0f*PI);
	posz += step*sin(cita / 180.0f*PI);
	checkCamera(posx, posz, maprect);
	centerx = posx - r* sin(cita / 180.0f*PI);
	centerz = posz + r*cos(cita / 180.0f*PI);

}

void Camera::CameraRight(int step) {
	posx -= step*cos(cita / 180.0f*PI);
	posz -= step*sin(cita / 180.0f*PI);
	checkCamera(posx, posz, maprect);
	centerx = posx - r*sin(cita / 180.0f*PI);
	centerz = posz + r* cos(cita / 180.0f*PI);
}

void Camera::CameraFoward(int step) {
	posx -= step*sin(cita / 180.0f*PI);
	posz += step*cos(cita / 180.0f*PI);
	checkCamera(posx, posz, maprect);
	centerx = posx - r* sin(cita / 180.0f*PI);
	centerz = posz + r* cos(cita / 180.0f*PI);

}

void Camera::CameraBackward(int step) {
	posx += step*sin(cita / 180.0f*PI);
	posz -= step*cos(cita / 180.0f*PI);
	checkCamera(posx, posz, maprect);
	centerx = posx - r*sin(cita / 180.0f*PI);
	centerz = posz + r* cos(cita / 180.0f*PI);

}

void Camera::MousePasive(int x, int y) {
    POINT mousePos;
	int middleX = GetSystemMetrics(SM_CXSCREEN) >> 1;
	int middleY = GetSystemMetrics(SM_CYSCREEN) >> 1;
	float angleY = 0.0f;       
	float angleZ = 0.0f;                     
	//static float gamma = 0.0f;
	GetCursorPos(&mousePos);
	ShowCursor(false);
	if ((mousePos.x == middleX) && (mousePos.y == middleY))
		return;
	SetCursorPos(middleX, middleY);
	angleY = (float)((middleX - mousePos.x)) / 10.0f;
	angleZ = (float)((middleY - mousePos.y)) / 10.0f;
	gamma += angleZ;
	cita -= angleY;
	if (gamma > 45) gamma = 45;
	else if (gamma < -45) gamma = -45;
	centerx = posx - r*sin(cita / 180.0f*PI);
	centerz = posz + r* cos(cita / 180.0f*PI);
	centery = posy + r*sin(gamma / 180.0f*PI);
}

void Camera::CameraRotateY(float angle) {
	cita += angle;
	if (cita > 360) cita -= 360;
	if (cita < -360) cita += 360;
	centerx = posx - r*sin(cita / 180.0f*PI);
	centerz = posz + r* cos(cita / 180.0f*PI);
}