#pragma once
#include "psydjfiState.h"
#include "psydjfiEngine.h"
#include "psydjfiTM.h"
#include "GameState.h"

class PauseState : public psydjfiState {
public:
	PauseState(psydjfiEngine* currentEngine, GameState* oldState);
	void setupBackground();
	void initializeObjects();
	void mouseDown(int iButton, int iX, int iY);
	void drawStringsOnTop();
	void keyDown(int iKeyCode);
	void mouseMoved(int iX, int iY);
	void postDraw();
	void mainLoopDoBeforeUpdate();
	void copyBackgroundBuffer();
	void initializeState();
	bool checkForUpdateTime();
	void handleEnter();
	void exitToMenu();
	void save();
private:
	GameState* gameState;
	psydjfiEngine* engine;
	int menuItem = 0;
};