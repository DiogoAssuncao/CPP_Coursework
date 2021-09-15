#include "header.h"
#include "GameState.h"
#include "PauseState.h"
#include "playerObject.h"
#include "consumableObject.h"
#include "enemyObject.h"
#include "MenuState.h"
#include "ScoreState.h"
#include <sstream>

#define MAX_LIVES 10

#define MUSIC_PATH1 "./music/dirt-rhodes-by-kevin-macleod-from-filmmusic-io.wav"
#define MUSIC_PATH2 "./music/tech-live-by-kevin-macleod-from-filmmusic-io.wav"
#define MUSIC_PATH3 "./music/metalmania-by-kevin-macleod-from-filmmusic-io.wav"
#define BACKGROUND_PATH1 "./images/dojo.jpg"
#define BACKGROUND_PATH2 "./images/subway.png"

//Constructor
GameState::GameState(psydjfiEngine* currentEngine) : engine(currentEngine), init(true), tm(engine->getTileManager()), objX(-1), objY(-1) { 
	readMaps();
	if (rand() % 2 == 0) {
		background = ImageManager::loadImage(BACKGROUND_PATH1, false);
		currentMap = BACKGROUND_PATH1;
		if (int k = rand() % 2 == 0) {
			loadMap(maps[k]);
		}
		else {
			loadMap(maps[k]);
		}
	}
	else {
		background = ImageManager::loadImage(BACKGROUND_PATH2, false);
		currentMap = BACKGROUND_PATH2;
		if (int k = rand() % 2 == 0) {
			loadMap(maps[2 + k]);
		}
		else {
			loadMap(maps[2 + k]);
		}
	}
	background = background.resizeTo(1350, 750);
	tm->setTopLeftPositionOnScreen(0, 0);
}

//Load Constructor
GameState::GameState(psydjfiEngine* currentEngine, string url, int llives, int lscore):lives(llives),score(lscore), 
		engine(currentEngine), init(false), tm(engine->getTileManager()), objX(-1), objY(-1) {
	background = ImageManager::loadImage(url, false);
	currentMap = url;
	background = background.resizeTo(1350, 750);
}

//Setup Background
void GameState::setupBackground() {
	background.renderImage(engine->getBackgroundSurface(), 0, 0, 0, 0, engine->getWindowWidth(), engine->getWindowHeight());
	tm->drawAllTiles(engine, engine->getBackgroundSurface());
}

//Initialize Objects - Player
void GameState::initializeObjects() {
	engine->drawableObjectsChanged();
	engine->storeObjectInArray(new playerObject(engine->getWindowWidth() / 2, 65, engine, engine->getTileManager(),this));
	engine->storeObjectInArray(new consumableObject(engine));
	engine->setAllObjectsVisible(false);
	engine->getDisplayableObject(0)->setVisible(true);
	((playerObject*)(engine->getDisplayableObject(0)))->setConsumable(((consumableObject*)(engine->getDisplayableObject(1))));
}

//Used for debugging - No current use
void GameState::mouseDown(int iButton, int iX, int iY) {
	switch (iButton){
	//case SDL_BUTTON_LEFT: spawnConsumable(); break;
	//case SDL_BUTTON_RIGHT: spawnEnemy(); break;
	default:
		break;
	}
}

//Lives and Score
void GameState::drawStringsOnTop() {
	engine->lockForegroundForDrawing();
	SimpleImage icon = ImageManager::loadImage("./images/neo.png", true);
	icon = icon.resizeTo(35, 50);
	icon.renderImageWithMask(engine->getForegroundSurface(), 0, 0, 10, 2, 35, 50);
	engine->drawForegroundString(50, 0, int2char(lives).c_str(), 0x00de00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 80));
	string sc = int2char(score);
	engine->drawForegroundString(engine->getWindowWidth()-50*sc.size(), 0, sc.c_str(), 0x00de00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 80));
	engine->unlockForegroundForDrawing();
}

//Esc starts pause menu
void GameState::keyDown(int iKeyCode) {
	switch (iKeyCode) {
	case SDLK_ESCAPE:engine->setState(new PauseState(engine,this)); break; 
	default:
		break;
	}
}

/*
Intialize State Steps
Setup background
Start objects
spawn first objective
plays music
init - whether you need to initialize the state, used when switching to pause
*/
void GameState::initializeState() {
	if (init) {
		engine->lockAndSetupBackground();
		initializeObjects();
		spawnObjective();
		init = false;
		engine->playMusic(MUSIC_PATH1);
	}
}

//Loads the platform map string to the tile manager
// formats is NumberOfTiles_x_Value_n
// b for next line
// only fills in the 7,15,23 lines where the platforms will stay
void GameState::loadMap(string str) {
	int tileX = 0, tileY = 7, maxY = tm->getMapHeight(), maxX = tm->getMapWidth();
	int iterator = 0, num  = 0, value = 0 ;
	while (tileY < maxY) {
		while (true) {
			if (str[iterator] == 'b') { iterator++;  break; }
			else {
				while (true) {
					num += str[iterator] - '0';
					iterator++;
					if (str[iterator] == 'x') { iterator++; break; }
					else { num *= 10; }
				}
				while (true) {
					value = str[iterator] - '0';
					iterator++;
					if (str[iterator] == 'n') { iterator++; break; }
					else { value *= 10; }
				}
				while (num > 0) {
					if (tileX < maxX) {
						tm->setMapValue(tileX, tileY, value);
						if (value == 1 || value == 2) {
							tiles.push_back(tuple<int,int>(tileX,tileY));
						}
						if (value == 0) {
							spaces.push_back(tuple<int, int>(tileX, (tileY-3)/4));
						}
					}
					tileX++;
					num--;
				}
				num = 0;
			}
		}
		tileX = 0;
		tileY += 8;
	}
	//Adding the window bottom as floor tiles for spawning purposes
	tileX = 0;
	tileY = maxY;
	while (tileX < maxX) {
		tiles.push_back(tuple<int, int>(tileX, tileY));
		tileX++;
	}
}

//BaseEngine version
void GameState::mouseMoved(int iX, int iY) {
	engine->BaseEngine::virtMouseMoved(iX,iY);
}

//BaseEngine version
void GameState::postDraw() {
	engine->BaseEngine::virtPostDraw();
}

//Check for lose condition
//And draw foreground strings
void GameState::mainLoopDoBeforeUpdate() {
	engine->BaseEngine::virtMainLoopDoBeforeUpdate();
	drawStringsOnTop();
	if (lives <= 0) {
		shared_ptr<psydjfiTM> tm = engine->getTileManager();
		for (int i = 0; i < tm->getMapWidth(); i++) {
			for (int ii = 0; ii < tm->getMapHeight(); ii++) {
				tm->setMapValue(i, ii, 0);
			}
		}
		engine->drawableObjectsChanged();
		engine->destroyOldObjects();

		engine->drawableObjectsChanged();
		engine->setState(new ScoreState(engine,score));
		delete this;
	}
}

//BaseEngine version
void GameState::copyBackgroundBuffer() {
	engine->BaseEngine::copyAllBackgroundBuffer();
}

//BaseEngine version
bool GameState::checkForUpdateTime() {
	return engine->BaseEngine::virtCheckForUpdateTime();
}

//Deletes and redraws previous objective and creates a new object in an avaible tile
//Objective is 5 tiles high only the top tile draws the image
void GameState::spawnObjective() {
	int iterator;
	int X, Y;
	if (objX != -1) {
		tm->setMapValue(objX, objY - 1, 0);
		tm->setMapValue(objX, objY - 2, 0);
		tm->setMapValue(objX, objY - 3, 0);
		tm->setMapValue(objX, objY - 4, 0);
		tm->setMapValue(objX, objY - 5, 0);
		int x = objX * 50;
		int y = (objY - 5) * 25;
		engine->lockBackgroundForDrawing();
		background.renderImage(engine->getBackgroundSurface(), x, y, x, y, 50, 125);
		engine->unlockBackgroundForDrawing();
	}
    iterator = rand() % (tiles.size());
	while ((get<0>(tiles[iterator]) == objX) || (get<1>(tiles[iterator]) == objY) || tm->getMapValue(get<0>(tiles[iterator]), get<1>(tiles[iterator]) - 1) != 0) {
		iterator = rand() % (tiles.size()); 
	}
	X = get<0>(tiles[iterator]);
	Y = get<1>(tiles[iterator]);
	tm->setMapValue(X, Y - 1, 3);
	tm->setMapValue(X, Y - 2, 3);
	tm->setMapValue(X, Y - 3, 3);
	tm->setMapValue(X, Y - 4, 3);
	tm->setAndRedrawMapValueAt(X, Y - 5, 3, engine, engine->getBackgroundSurface());
	objX = X;
	objY = Y;

}

//Spawns a new trap
//Cant spawn right next to another trpa
//cant spawn in a jump tile (tile required to get to another level)
//cant spawn directly underneath
void GameState::spawnTrap() {
	int iterator;
	int X, Y, pX, pY;
	int counter = 0;
	iterator = rand() % (tiles.size());
	pX = tm->getMapXForScreenX(engine->getDisplayableObject(0)->getXCentre());
	pY = tm->getMapYForScreenY(engine->getDisplayableObject(0)->getYCentre() + 50);
	X = get<0>(tiles[iterator]);
	Y = get<1>(tiles[iterator]);
	while (tm->getMapValue(X,Y-1) != 0 || 
		   tm->getMapValue(X + 1, Y - 1) != 0 || tm->getMapValue(X - 1, Y - 1) != 0 || 
		   Y == tm->getMapHeight() || 
		   (tm->getMapValue(X, Y - 8) == 0 && ((tm->getMapValue(X + 1, Y - 8) == 1 || tm->getMapValue(X + 1, Y - 8) == 2) || (tm->getMapValue(X - 1, Y - 8) == 1 || tm->getMapValue(X - 1, Y - 8) == 2))) ||
		   tm->getMapValue(X + 1, Y) == 0 || tm->getMapValue(X - 1, Y) == 0 ||
		   (X == pX && Y - 1 == pY) ) {
		counter++;
		iterator = rand() % (tiles.size());
		X = get<0>(tiles[iterator]);
		Y = get<1>(tiles[iterator]);
		if (counter == tiles.size()) { return; }
	}
	int value = tm->getMapValue(X, Y);
	tm->setAndRedrawMapValueAt(X, Y - 1, 4, engine, engine->getBackgroundSurface());
	tm->setAndRedrawMapValueAt(X, Y, value, engine, engine->getBackgroundSurface());
	nTraps++;
}

//Deletes previous consumable and creates a new one wherever there is not a trap or an objective
void GameState::spawnConsumable() {
	playerObject* player = (playerObject*)(engine->getDisplayableObject(0));
	consumableObject* item = player->getConsumable(); 
	engine->drawableObjectsChanged();
	int iterator;
	int X = item->getTileX(), Y = item->getTileY();
	if (X != -1) {
		tm->setMapValue(X, Y - 1, 0);
		tm->setMapValue(X, Y - 2, 0);
		tm->setMapValue(X, Y - 3, 0);
	}
	delete item;
	iterator = rand() % (tiles.size());
	X = get<0>(tiles[iterator]);
	Y = get<1>(tiles[iterator]);
	while (tm->getMapValue(X, Y - 1) != 0) {
		iterator = rand() % (tiles.size());
		X = get<0>(tiles[iterator]);
		Y = get<1>(tiles[iterator]);
	}
	tm->setMapValue(X, Y - 1, 5);
	tm->setMapValue(X, Y - 2, 5);
	tm->setMapValue(X, Y - 3, 5);
	int a = rand() % 10;
	switch (a)
	{
	case 0: case 1: case 5: case 6: case 7: engine->storeObjectInArray(new redPill(X, Y, engine, player), 1); break;
	case 2: case 3: case 4: engine->storeObjectInArray(new greenPill(X, Y, engine, player), 1); break;
	case 9: case 8: engine->storeObjectInArray(new bluePill(X, Y, engine, player), 1); break;
	default:
		break;
	}
	((playerObject*)(engine->getDisplayableObject(0)))->setConsumable(((consumableObject*)(engine->getDisplayableObject(1))));
	item = player->getConsumable();
	item->setTileX(X);
	item->setTileY(Y);
}

//Spawns an enemy in either top or bottom level (whichever is furthest from player)
//Has a small chance it spawns a faster enemy that takes less time to disapear
void GameState::spawnEnemy() {
	int k = rand() % 2;
	int X = 0, Y = 0;
	if (engine->getDisplayableObject(0)->getYCentre() > engine->getWindowHeight() / 2) {
		switch (k) {
		case 0: X = 50; Y = 50; break;
		case 1: X = engine->getWindowWidth() - 100; Y = 50; break;
		default:
			break;
		}
	}
	else {
		switch (k) {
		case 0: X = engine->getWindowWidth() - 100; Y = engine->getWindowHeight() - 100; break;
		case 1: X = 50; Y = engine->getWindowHeight() - 100; break;
		default:
			break;
		}
	}
	k = rand() % 5;
	int i;
	enemyObject* enemy;
	if (k == 0) {
		enemy = new enemyObject(X, Y, engine, 5 , 500);
	}
	else {
		enemy = new enemyObject(X, Y, engine);
	}
	enemy->setTransparencyColour(enemy->getPixelColour(0, 0));
	i = engine->storeObjectInArray(enemy);
	engine->getDisplayableObject(i)->setVisible(true);
}

//handles the game loop, spawning enemies, traps, objectives, consumables and changes music 
void GameState::gameSpawner() {
	spawnObjective();
	addScore();
	//trap Spawn
	if (nTraps == 0) {
		spawnTrap();
	}
	else {
		int i = rand() % nTraps;
		if (i == 0) {
			spawnTrap();
		}
	}
	//Consumable Spawn
	if (score % 100 == 0) {
		spawnConsumable();
	}
	//Spawn Enemy
	if (score % 80 == 0) {
		spawnEnemy();
	}
	if (score % 250 == 0) {
		int i = rand() % 2;
		if (i == 0) {
			spawnEnemy();
		}
	}
	if (score == 150) {
		engine->playMusic(MUSIC_PATH2);
	}
	if (score == 300) {
		engine->playMusic(MUSIC_PATH3);
	}
}

//lose a life
void GameState::lose() {
	lives--;
}

//add a life up to max lives
void GameState::addLife() {
	if (lives <= MAX_LIVES) {
		lives++;
	}
}

//getter from background image
SimpleImage* GameState::getBackground() {
	return &background;
}

//integer to character converter using streamstring
string GameState::int2char(int x) {
	stringstream str;
	str << x;
	return str.str();
}

//Increment score by 10
void GameState::addScore() {
	score += 10;
}

//Set number of traps to 0
void GameState::resetTraps(){
	nTraps = 0;
}

//Read and save maps from maps folder
void GameState::readMaps() {
	ifstream file;
	string temp;
	file.open("./saves/maps.txt");
	if (!file) {
		cout << "Failed to Open File" << endl;
		return ;
	}
	else {
		while (getline(file, temp)) {
			maps.push_back(temp);
		}
	}
	file.close();
}

//Getter for spaces pointer
vector<tuple<int, int>>* GameState::getSpaces() {
	return &spaces;
}

//Getter for spaces copy
vector<tuple<int, int>> GameState::getSpacesCopy() {
	return spaces;
}

//Get path for current map
string GameState::getMap() { return currentMap;}

//Get Lives
int GameState::getLives() { return lives; }

//Get Score
int GameState::getScore() { return score; }

//Set number of traps
void GameState::setNTraps(int x) { nTraps = x; }

//get tiles pointer
vector<tuple<int, int>>* GameState::getTiles() { return &tiles; }

//set objX
void GameState::setObjX(int x) { objX = x; }

//set objY
void GameState::setObjY(int y) { objY = y; }