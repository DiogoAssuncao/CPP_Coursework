#pragma once
#include "psydjfiState.h"
#include "psydjfiEngine.h"
#include "psydjfiTM.h"
#include "DrawingFilters.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

class GameState : public psydjfiState {
public:
	GameState(psydjfiEngine *currentEngine);
	GameState(psydjfiEngine* currentEngine,string url, int llives, int lscore);
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
	void loadMap(string str);
	void spawnObjective();
	void spawnTrap();
	void spawnConsumable();
	void spawnEnemy();
	void gameSpawner();
	void lose();
	void addLife();
	void addScore();
	void resetTraps();
	void readMaps();
	string getMap();
	int getLives();
	int getScore();
	vector<tuple<int, int>>* getSpaces();
	vector<tuple<int, int>> getSpacesCopy();
	SimpleImage* getBackground();
	string int2char(int x);
	void setNTraps(int x);
	vector<tuple<int, int>>* getTiles();
	void setObjX(int x);
	void setObjY(int y);

private:
	vector<string> maps;
	string currentMap;
	int nTraps = 0;
	int lives = 3, score = 0;
	vector<tuple<int,int>> tiles;
	vector<tuple<int, int>> spaces;
	bool init;
	int objX, objY;
	int counter = 20;
	SimpleImage background;
	psydjfiEngine *engine;
	shared_ptr<psydjfiTM> tm;
	int width = 1350, height = 1350;
};