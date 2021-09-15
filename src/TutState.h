#pragma once
#include "psydjfiState.h"
#include "psydjfiEngine.h"
#include "psydjfiTM.h"
#include "DrawingFilters.h" 

class TutState : public psydjfiState {
public:
	TutState::TutState(psydjfiEngine* currentEngine, bool returnToMenu);
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
	void handleState();
	void finish();
private:
	bool goToGame;
	int state = 0;
	SimpleImage morpheus, pill1,pill2,pill3,tile,trap,payphone;
	psydjfiEngine* engine;
	FilterPointsScaling* zoom;
	FilterPointsTranslation* translation;
};