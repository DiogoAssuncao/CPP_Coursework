#include "header.h"
#include "SaveNLoad.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

#define PATH "./saves/saveFile.txt"

//--------------------------------------------------------------------------------------Save Class------------------------------------------------------------------------------------------------------------

//Constructor
Save::Save(psydjfiEngine* pEngine, GameState* sState) : engine(pEngine), gState(sState) {}

//Joins all save funtions and wrapps them in the string with G..G&P..P&E..E&....&&
void Save::saveAll() {
	stringstream finalStr;
	string state, player;
	enemyObject* enemy;
	state = saveState();
	player = savePlayer((playerObject*)engine->getDisplayableObject(0));
	finalStr << "G" << state << "G&P" << player << "P&";
	int k = 2;
	while (enemy = (enemyObject*)engine->getDisplayableObject(k)) {
		string enemyStr = saveEnemy(enemy);
		finalStr << "E" << enemyStr << "E&";
		k++;
	}
	finalStr << "&";
	writeToFile(finalStr.str());
}

//Saves the important gameState parameters like
//background map
//"bitmap" for tile manager
//lives
//score
string Save::saveState() {
	stringstream stateStr;
	shared_ptr<psydjfiTM> tm = engine->getTileManager();
	//background path
	stateStr << "b?" << gState->getMap() << "?";
	//tm bitmap
	stateStr << "m?";
	int X = 0, Y = 0;
	while (Y < tm->getMapHeight()) {
		while (X < tm->getMapWidth()) {
			stateStr << tm->getMapValue(X, Y);
			X++;
		}
		Y++;
		X = 0;
	}
	stateStr << "?";
	//lives
	stateStr << "l?" << gState->getLives() << "?";
	//Score
	stateStr << "s?" << gState->getScore() << "?";
	return stateStr.str();
}

//Save the important playerObject paramaters like
//X and Y coordinates
//invencible timer
//hit
//enemyHit
//Current consumable
//Current consumable's visibility
string Save::savePlayer(playerObject* player){
	stringstream playerStr;
	//Position 
	playerStr << "p?" << player->getX() << "/" << player->getY() << "?";
	//incencible timer 
	playerStr << "t?" << player->getITimer() << "?";
	//hit 
	playerStr << "h?" << player->getHit() << "?";
	//enemy hit 
	playerStr << "e?" << player->getEHit() << "?";
	//consumable type 
	playerStr << "c?" << player->getConsumable()->getValue() << "?";
	//consumable visible 
	playerStr << "v?" << player->getConsumable()->isVisible() << "?";
	return playerStr.str();
}

//Save important enemyObject parameters like
//X and Y position
//lifetime
//speed 
//Climbing
//Descending
//target - if there is one
string Save::saveEnemy(enemyObject* enemy) {
	stringstream enemyStr;
	//Position 
	enemyStr << "p?" << enemy->getX() << "/" << enemy->getY() << "?";
	//lifetime
	enemyStr << "l?" << enemy->getLifetime() << "?";
	//speed
	enemyStr << "s?" << enemy->getSpeed() << "?";
	//climbing
	enemyStr << "c?" << enemy->getClimbing() << "?";
	//descending
	enemyStr << "d?" << enemy->getDescending() << "?";
	//target
	if (enemy->getTarget() != -1) {
		enemyStr << "e?" << enemy->getTarget() << "?";
	}
	return enemyStr.str();
}

//write string to file
void Save::writeToFile(string final) {
	ofstream file(PATH);
	if (!file) {
		cout << "Failed to Open File" << endl;
		return;
	}
	else {
		file << final << "\n";
	}
	file.close();
}

//--------------------------------------------------------------------------------------Load Class------------------------------------------------------------------------------------------------------------

//Constructor
Load::Load(psydjfiEngine* pEngine) : engine(pEngine){}

//Takes the loaded value and starts everything up
void Load::startLoadedGame() {
	loadAll();
	GameState* newGame = new GameState(engine,bgPath,lives,score);
	shared_ptr<psydjfiTM> tm = engine->getTileManager();
	int X=0, Y=0, iterator = 0;
	vector<tuple <int, int>> *spaces = newGame->getSpaces(), *tiles = newGame->getTiles();
	int count = 0, count2 = 0;;
	while (Y < tm->getMapHeight()) {
		while (X < tm->getMapWidth()) {
			int value = bitmap[iterator] - 48;
			//Number of Traps
			if (value == 4) { numTraps++; }
			//Objective Position
			if (value == 3) {
				count++;
				if (count == 5) {
					objX = X;
					objY = Y + 1;
				}
			}
			//Consumable Position
			if (value == 5 ) {
				count2++;
				if (count2 == 3) {
					consX = X;
					consY = Y + 1;
				}
			}
			//tiles
			if (value == 1 || value == 2) {
				tiles->push_back(tuple<int, int>(X, Y));
			}
			//spaces
			if (Y == 7 || Y == 15 || Y == 23) {
				if (value == 0) {
					spaces->push_back(tuple<int, int>(X, (Y - 3) / 4));
				}
			}
			tm->setMapValue(X, Y, value);
			X++;
			iterator++;
		}
		Y++;
		X = 0;
	}
	newGame->setNTraps(numTraps);
	newGame->setObjX(objX);
	newGame->setObjY(objY);
	engine->setState(newGame);
	//Initalize Objects
	engine->drawableObjectsChanged();
	engine->storeObjectInArray(new playerObject(playerX,playerY , engine, tm, newGame, invTimer, hit, eHit));
	switch (consType){
		case 0: engine->storeObjectInArray(new consumableObject(engine)); break;
		case 1: engine->storeObjectInArray(new redPill(consX,consY,engine,(playerObject*)engine->getDisplayableObject(0))); break;
		case 2: engine->storeObjectInArray(new bluePill(consX, consY, engine, (playerObject*)engine->getDisplayableObject(0))); break;
		case 3: engine->storeObjectInArray(new greenPill(consX, consY, engine, (playerObject*)engine->getDisplayableObject(0))); break;
	default:
		break;
	}
	engine->setAllObjectsVisible(false);
	engine->getDisplayableObject(0)->setVisible(true);
	if (consVis == -1) {
		engine->getDisplayableObject(1)->setVisible(false);
	}
	else {
		engine->getDisplayableObject(1)->setVisible(consVis);
	}
	((playerObject*)(engine->getDisplayableObject(0)))->setConsumable(((consumableObject*)(engine->getDisplayableObject(1))));
	if (enemyX.size() > 0) {
		int iterator = 0;
		while (iterator < enemyX.size()) {
			engine->storeObjectInArray(new enemyObject(enemyX[iterator], enemyY[iterator], engine, speed[iterator], lifetime[iterator], climbing[iterator], descending[iterator]));
			iterator++;
		}
	}
	
	engine->lockAndSetupBackground();
	if (score < 150) {
		engine->playMusic("./music/dirt-rhodes-by-kevin-macleod-from-filmmusic-io.wav");
	}
	else if (score < 300){
		engine->playMusic("./music/tech-live-by-kevin-macleod-from-filmmusic-io.wav");
	}
	else {
		engine->playMusic("./music/metalmania-by-kevin-macleod-from-filmmusic-io.wav");
	}
	ofstream file(PATH);
	file << "\n";
	delete this;
}

//Loads all value according to the format in the correct vairables
void Load::loadAll() {
	string loadStr = readFile();
	int iterator = 0;
	while (loadStr[iterator]) {
		//GameState
		if (loadStr[iterator] == 'G') {
			iterator++;
			while (loadStr[iterator] != 'G') {
				//background
				if (loadStr[iterator] == 'b') {
					iterator++;
					iterator++;
					stringstream temp;
					while (loadStr[iterator] != '?'){
						temp << loadStr[iterator];
						iterator++;
					}
					bgPath = temp.str();

				}
				//bitmap
				else if (loadStr[iterator] == 'm') {
					iterator++;
					iterator++;
					stringstream temp;
					while (loadStr[iterator] != '?') {
						temp << loadStr[iterator];
						iterator++;
					}
					bitmap = temp.str();
				}
				//lives
				else if (loadStr[iterator] == 'l') {
					iterator++;
					iterator++;
					stringstream temp;
					while (loadStr[iterator] != '?') {
						temp << loadStr[iterator];
						iterator++;
					}
					lives = stoi(temp.str());
				}
				//score
				else if (loadStr[iterator] == 's') {
					iterator++;
					iterator++;
					stringstream temp;
					while (loadStr[iterator] != '?') {
						temp << loadStr[iterator];
						iterator++;
					}
					score = stoi(temp.str());
				}
				iterator++;
			}
		}
		//Player
		else if (loadStr[iterator] == 'P') {
			iterator++;
			while (loadStr[iterator] != 'P') {
				//Position
				if (loadStr[iterator] == 'p') {
					iterator++;
					iterator++;
					stringstream temp;
					while (loadStr[iterator] != '/') {
						temp << loadStr[iterator];
						iterator++;
					}
					playerX = stoi(temp.str());
					iterator++;
					stringstream temp2;
					while (loadStr[iterator] != '?') {
						temp2 << loadStr[iterator];
						iterator++;
					}
					playerY = stoi(temp2.str());
				}
				//Invencible Timer
				else if (loadStr[iterator] == 't') {
					iterator++;
					iterator++;
					stringstream temp;
					while (loadStr[iterator] != '?') {
						temp << loadStr[iterator];
						iterator++;
					}
					invTimer = stoi(temp.str());
				}
				//Hit
				else if (loadStr[iterator] == 'h') {
					iterator++;
					iterator++;
					stringstream temp;
					while (loadStr[iterator] != '?') {
						temp << loadStr[iterator];
						iterator++;
					}
					hit = stoi(temp.str());
				}
				//Enemy hit
				else if (loadStr[iterator] == 'e') {
					iterator++;
					iterator++;
					stringstream temp;
					while (loadStr[iterator] != '?') {
						temp << loadStr[iterator];
						iterator++;
					}
					eHit = stoi(temp.str());
				}
				//Consumable Type
				else if (loadStr[iterator] == 'c') {
					iterator++;
					iterator++;
					stringstream temp;
					while (loadStr[iterator] != '?') {
						temp << loadStr[iterator];
						iterator++;
					}
					consType = stoi(temp.str());
				}
				//Consumable Visible
				else if (loadStr[iterator] == 'v') {
					iterator++;
					iterator++;
					stringstream temp;
					while (loadStr[iterator] != '?') {
						temp << loadStr[iterator];
						iterator++;
					}
					consVis = stoi(temp.str());
				}
				iterator++;
			}
		}
		//Enemy
		else if (loadStr[iterator] == 'E') {
			bool targetset = false;
			iterator++;
			while (loadStr[iterator] != 'E') {
				//Position
				if (loadStr[iterator] == 'p') {
					iterator++;
					iterator++;
					stringstream temp;
					while (loadStr[iterator] != '/') {
						temp << loadStr[iterator];
						iterator++;
					}
					enemyX.push_back(stoi(temp.str()));
					iterator++;
					stringstream temp2;
					while (loadStr[iterator] != '?') {
						temp2 << loadStr[iterator];
						iterator++;
					}
					enemyY.push_back(stoi(temp2.str()));
				}
				//Lifetime
				else if (loadStr[iterator] == 'l') {
					iterator++;
					iterator++;
					stringstream temp;
					while (loadStr[iterator] != '?') {
						temp << loadStr[iterator];
						iterator++;
					}
					lifetime.push_back(stoi(temp.str()));
				}
				//Speed
				else if (loadStr[iterator] == 's') {
					iterator++;
					iterator++;
					stringstream temp;
					while (loadStr[iterator] != '?') {
						temp << loadStr[iterator];
						iterator++;
					}
					speed.push_back(stoi(temp.str()));
				}
				//Climbing
				else if (loadStr[iterator] == 'c') {
					iterator++;
					iterator++;
					stringstream temp;
					while (loadStr[iterator] != '?') {
						temp << loadStr[iterator];
						iterator++;
					}
					climbing.push_back(stoi(temp.str()));
				}
				//Descending
				else if (loadStr[iterator] == 'd') {
					iterator++;
					iterator++;
					stringstream temp;
					while (loadStr[iterator] != '?') {
						temp << loadStr[iterator];
						iterator++;
					}
					descending.push_back(stoi(temp.str()));
				}
				//Target
				else if (loadStr[iterator] == 'd') {
					iterator++;
					iterator++;
					stringstream temp;
					while (loadStr[iterator] != '?') {
						temp << loadStr[iterator];
						iterator++;
					}
					target.push_back(stoi(temp.str()));
					targetset = true;
				}
				iterator++;
			}
			if (!targetset) {
				target.push_back(-1);
			}
		}
		iterator++;
	}
}

//Reads the string from the file
string Load::readFile() {
	string temp;
	ifstream file(PATH);
	if (!file) {
		cout << "Failed to Open File" << endl;
		return nullptr;
	}
	else {
		getline(file, temp);
	}
	file.close();
	return temp;
}