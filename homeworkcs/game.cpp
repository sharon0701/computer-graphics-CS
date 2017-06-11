#include"game.h"

Game::Game():camera(100,25,0,180,0) {
	iHp = 100;
	for (int i = 0; i < GridWidth*2; i++) {
		gridarray[i * 3] = -GridWidth/2 + i/2;
		gridarray[i * 3 + 1] = 0;
		if(i&1)
		gridarray[i * 3 + 2] = GridWidth / 2;
		else gridarray[i * 3 + 2] =- GridWidth / 2;
	}
	for (int i = 0; i < GridWidth * 2; i++) {
		gridarray[GridWidth * 2 *3+i * 3+2] = -GridWidth / 2 + i / 2;
		gridarray[GridWidth * 2 *3+i * 3 + 1] = 0;
		if (i & 1)
			gridarray[GridWidth * 2 *3+i * 3 + 0] = GridWidth / 2;
		else gridarray[GridWidth * 2*3 +i * 3 + 0] =- GridWidth / 2;
	}
}