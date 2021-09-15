#pragma once
#include "psydjfiEngine.h"
#include "GameState.h"
#include "playerObject.h"
#include "enemyObject.h"

using namespace std;

class Save {
public:
	Save::Save(psydjfiEngine* pEngine,GameState* sState);
	void saveAll();
	string saveState();
	string savePlayer(playerObject* player);
	string saveEnemy(enemyObject* enemy);
	void writeToFile(string final);
private:
	psydjfiEngine* engine;
	GameState* gState;
};

class Load {
public:
	Load::Load(psydjfiEngine* pEngine);
	void startLoadedGame();
	void loadAll();
	string readFile();
private:
	psydjfiEngine* engine;
	//GameState
	string bgPath;
	string bitmap;
	int lives = -1;
	int score = -1;
	int numTraps = 0;
	int objX = -1;
	int objY = -1;
	//Player
	int playerX = -1;
	int playerY = -1;
	int invTimer = -1;
	int hit = -1;
	int eHit = -1;
	int consType = -1;
	int consVis = -1;
	int consX = -1;
	int consY = -1;
	//Enemy
	vector<int> enemyX;
	vector<int> enemyY;
	vector<int> speed;
	vector<int> lifetime;
	vector<int> climbing;
	vector<int> descending;
	vector<int> target;
};