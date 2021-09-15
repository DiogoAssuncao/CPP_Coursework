#pragma once
#include "psydjfiState.h"
#include "psydjfiEngine.h"

#define BACKGROUND_SPEED 3

class MenuState : public psydjfiState {
public:
	MenuState(psydjfiEngine* currentEngine);
	void setupBackground();
	void initializeObjects();
	void mouseDown(int iButton, int iX, int iY);
	void drawStringsOnTop();
	void keyDown(int iKeyCode);
	void handleEnter();
	void mouseMoved(int iX, int iY);
	void postDraw();
	void mainLoopDoBeforeUpdate();
	void copyBackgroundBuffer();
	void initializeState();
	bool checkForUpdateTime();
	void load();
	bool checkLoad();
	bool checkScore();
private:
	psydjfiEngine* engine;
	SimpleImage morpheus, background;
	int menuItem = 0;
	int timer = BACKGROUND_SPEED;
	int offset = 0;
	bool loadPossible;
	bool firstTime = 0;
};

