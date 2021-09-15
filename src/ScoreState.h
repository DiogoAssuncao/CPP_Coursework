#pragma once
#include "psydjfiState.h"
#include "psydjfiEngine.h"

using namespace std;

#define BACKGROUND_SPEED 3
class score;

class ScoreState : public psydjfiState {
public:
	ScoreState::ScoreState(psydjfiEngine* pEngine);
	ScoreState::ScoreState(psydjfiEngine* pEngine, int pScore);
	void initializeState();
	void setupBackground();
	void initializeObjects();
	void mouseDown(int iButton, int iX, int iY);
	void drawStringsOnTop();
	void keyDown(int iKeyCode);
	void mouseMoved(int iX, int iY);
	void postDraw();
	void mainLoopDoBeforeUpdate();
	void copyBackgroundBuffer();
	bool checkForUpdateTime();
	int readScore();
	void saveScore();
	void switch2Read();
private:
	int nameIterator = 0;
	char newName[3] = {'ç', 'ç', 'ç'};
	bool cont = false;
	SimpleImage background;
	psydjfiEngine* engine;
	int timer = BACKGROUND_SPEED;
	vector<score> scores;
	int offset = 0;
	bool Read;
	bool Write;
	int wScore;
};

class score {
public:
	score::score(int k, string str);
	void setValue(int k);
	void setName(string str);
	int getValue();
	string getName();
	friend bool operator<(score& a, score& b);
	friend bool operator>(score& a, score& b);
	friend bool operator<=(score& a, score& b);
	friend bool operator>=(score& a, score& b);
private:
	int value;
	string name;
};