#pragma once
#include <windows.h>
#include<gl/glut.h>

#include<math.h>
#ifndef PI
#define PI 3.1415926
#endif // !PI
class Camera{
public:
	float posz;
	float posx;
	float posy;
	float centerx;
	float centery;
	float centerz;
	float cita = 0;
	float gamma;
	float r = 100;

	Camera::Camera(
		float x,
		float y,
		float z,
		float ca,
		float ga);
	void CameraUp(int step);
	void CameraLeft(int step);
	void CameraRight(int step);
	void CameraFoward(int step);
	void CameraBackward(int step);
	void MousePasive(int x, int y);
	void CameraRotateY(float angle);
	void setCamerLook() {
		gluLookAt(posx, posy, posz, centerx, centery, centerz, 0, 1, 0);
	}
	
};