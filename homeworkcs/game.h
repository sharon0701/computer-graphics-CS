#pragma once
#include"Camera.h"
#include"anmobj.h"
#define GridWidth 50

class Game {
public:
	Camera camera;
	int iMapEnemyCursor;
	anmobj* m_anmobj;
	int iHp;
	int gridarray[GridWidth*2*3*2];
	Game();

};
