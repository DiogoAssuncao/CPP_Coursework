#pragma once

//Interface for State Classes
class psydjfiState {
public: 
	virtual void initializeState() = 0;
	virtual void setupBackground() = 0;
	virtual void initializeObjects() = 0;
	virtual void mouseDown(int iButton, int iX, int iY) = 0;
	virtual void drawStringsOnTop() = 0;
	virtual void keyDown(int iKeyCode) = 0;
	virtual void mouseMoved(int iX, int iY) = 0;
	virtual void postDraw() = 0;
	virtual void mainLoopDoBeforeUpdate() = 0;
	virtual void copyBackgroundBuffer() = 0;
	virtual bool checkForUpdateTime() = 0;
};