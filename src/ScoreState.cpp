#include "header.h"
#include "ScoreState.h"
#include "MenuState.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>  

#define PATH "./saves/scores.txt"

//Constructor - Start in reading mode
ScoreState::ScoreState(psydjfiEngine* pEngine) : engine(pEngine), Read(true), Write(false), wScore(-1),cont(true) {}

//Contructor - Start in saving/writing mode
ScoreState::ScoreState(psydjfiEngine* pEngine, int pScore) : engine(pEngine), Read(false), Write(true), wScore(pScore) {}

//Initialize State - Setup background
void ScoreState::initializeState() {
	engine->lockAndSetupBackground();
}

//Setup background
void ScoreState::setupBackground() {
	//Background
	background = ImageManager::loadImage("./images/digitalrain.jpg", false);
	background = background.resizeTo(engine->getWindowWidth(), engine->getWindowHeight());
	background.renderImage(engine->getBackgroundSurface(), 0, 0, 0, 0, engine->getWindowWidth(), engine->getWindowHeight());

	//Menu Area
	engine->drawBackgroundRectangle(330, 0, 1020, 750, 0x003300);
	engine->drawBackgroundRectangle(350, 0, 1000, 750, 0x0e0e0e);
	readScore();
	drawStringsOnTop();
}

//Animating background
void ScoreState::mainLoopDoBeforeUpdate() {
	if (timer == 0) {
		offset = (engine->getWindowHeight() + (offset - 1)) % engine->getWindowHeight();
		engine->redrawDisplay();
		timer = BACKGROUND_SPEED;
	}
	else { timer--; }

}

//Animating background
void ScoreState::copyBackgroundBuffer() {
	engine->getForegroundSurface()->copyRectangleFrom(engine->getBackgroundSurface(), 0, 0, engine->getWindowWidth(), engine->getWindowHeight(), 0, offset);
	engine->getForegroundSurface()->copyRectangleFrom(engine->getBackgroundSurface(), 0, engine->getWindowHeight() - offset, engine->getWindowWidth(), engine->getWindowHeight(), 0, offset - engine->getWindowHeight());
}

//Draw Strings
void ScoreState::drawStringsOnTop() {
	engine->lockForegroundForDrawing();
	if (Read) {
		engine->drawForegroundString(425, 50, "HighScores", 0x00de00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 80));
		for (int i = 0; i < 10; i++) {
			if (i < scores.size()) {
				stringstream temp;
				temp << i+1 << ". " << scores[i].getName() << " - " << scores[i].getValue();
				if (i == 0) {
					engine->drawForegroundString(370, 130, temp.str().c_str(), 0x00cc00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 100));
				}
				else if (i == 1) {
					engine->drawForegroundString(425, 200, temp.str().c_str(), 0x00bb00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 80));
				}
				else if (i == 2) {
					engine->drawForegroundString(445, 260, temp.str().c_str(), 0x00aa00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 75));
				}
				else {
					engine->drawForegroundString(480, 320 + 50 * (i - 3), temp.str().c_str(), 0x006600, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 60));
				}
			}
			else {
				stringstream temp;
				temp << i + 1 << ". ";
				engine->drawForegroundString(480, 170 + 50 * i, temp.str().c_str(), 0x006600, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 60));
			}
		}
		engine->drawForegroundString(400, 700, "Press Enter to Continue", 0x004400, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 40));
	}
	if (Write) {
		engine->drawForegroundString(470, 50, "Game", 0x00bb00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 150));
		engine->drawForegroundString(550, 150, "Over", 0x00bb00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 100));
		stringstream temp;
		temp << wScore;
		engine->drawForegroundString(600, 250, "Score", 0x006600, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
		int x;
		if (wScore >= 1000) {
			x = 515;
		}
		else if (wScore >= 100) {
			x = 560;
		}
		else if (wScore >= 10) {
			x = 600;
		}
		else {
			x = 630;
		}
		engine->drawForegroundString(x, 300, temp.str().c_str(), 0x00dd00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 120));
		if (scores.size()< 10 || wScore > scores[9].getValue()) {
			engine->drawForegroundString(560, 400, "Input Name", 0x006600, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 40));
			if (newName[0] != 'ç') {
				stringstream temp;
				temp << newName[0];
				engine->drawForegroundString(525, 450, temp.str().c_str(), 0x00aa00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 150));
			}
			else {
				engine->drawForegroundRectangle(540, 550, 625, 555, 0x00dd00);
			}
			if (newName[1] != 'ç') {
				stringstream temp;
				temp << newName[1];
				engine->drawForegroundString(625, 450, temp.str().c_str(), 0x00aa00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 150));
			}
			else {
				engine->drawForegroundRectangle(640, 550, 725, 555, 0x00dd00);
			}
			if (newName[2] != 'ç') {
				stringstream temp;
				temp << newName[2];
				engine->drawForegroundString(725, 450, temp.str().c_str(), 0x00aa00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 150));
				engine->drawForegroundString(400, 600,"Press Enter to Continue", 0x004400, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 40));
				cont = true;
			}
			else {
				engine->drawForegroundRectangle(740, 550, 825, 555, 0x00dd00);
			}
		}
		else {
			engine->drawForegroundString(400, 600, "Press Enter to Continue", 0x004400, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 40));
			cont = true;
		}
		
	}

	engine->unlockForegroundForDrawing();
	engine->redrawDisplay();
}

//Read and Save Scores from file
int ScoreState::readScore() {
	scores.clear();
	ifstream file;
	string temp, name ,scoreStr;
	int scoreInt;
	file.open(PATH);
	if (!file) {
		cout << "Failed to Open File" << endl;
		return 1;
	}
	else {
		while (getline(file,temp)) {
			name = temp[0];
			name += temp[1];
			name += temp[2];
			scoreStr = temp[3];
			int k = 4;
			while (temp[k]) {
				scoreStr += temp[k];
				k++;
			}
			scores.push_back( score(stoi(scoreStr), name));
		}
	}
	file.close();
	sort(scores.begin() , scores.end());
	return 0;
}

//User Input handling
void ScoreState::keyDown(int iKeyCode) {
	if (Write) {
		if (iKeyCode == 8 && nameIterator > 0) {
			nameIterator--;
			newName[nameIterator] = 'ç';
			if (cont && (scores.size() < 10 || wScore > scores[9].getValue())) {
				cont = false;
			}
		}
		if ((iKeyCode > 96 && iKeyCode < 123) && nameIterator < 3) {
			newName[nameIterator] = iKeyCode;
			nameIterator++;
		}
		if ((iKeyCode == SDLK_KP_ENTER || iKeyCode == SDLK_RETURN) && cont == true) {
			if (scores.size() < 10 || wScore > scores[9].getValue()) {
				saveScore();
			}
			switch2Read();
		}
	}
	else if (Read) {
		if ((iKeyCode == SDLK_KP_ENTER || iKeyCode == SDLK_RETURN) && cont == true) {
			engine->setState(new MenuState(engine));
			delete this;
		}
	}
}

//Switch to read mode
void ScoreState::switch2Read() {
	cont = true;
	Read = true;
	Write = false;
	readScore();
}

//Save score to file
void ScoreState::saveScore() {
	ofstream file;
	file.open(PATH, std::ios_base::app);
	if (!file) {
		cout << "Failed to Open File" << endl;
		return ;
	}
	else {
		file << newName[0] << newName[1] << newName[2] << wScore << "\n";
	}
	file.close();
	
}

//Base Engine Version
void ScoreState::initializeObjects() { engine->BaseEngine::virtInitialiseObjects(); }

//Base Engine Version
void ScoreState::mouseDown(int iButton, int iX, int iY) { engine->BaseEngine::virtMouseDown(iButton, iX, iY); }

//Base Engine Version
void ScoreState::mouseMoved(int iX, int iY) { engine->BaseEngine::virtMouseMoved(iX, iY); }

//Base Engine Version
void ScoreState::postDraw() { engine->BaseEngine::virtPostDraw(); }

//Base Engine Version
bool ScoreState::checkForUpdateTime() { return engine->BaseEngine::virtCheckForUpdateTime(); }

//------------------------------------------------------------------------------------------Score Class---------------------------------------------------------------------------------------------------------------

//constructor
score::score(int k, string str) : value(k), name (str){}

//set value
void score::setValue(int k) { value = k; }

//set name
void score::setName(string str) { name = str; }

//get value
int score::getValue() { return value; }
//get nale
string score::getName() { return name; }

//Overloading Operations so the sort default function returns from bigger to smaller

//Overloading <
bool operator<(score &a, score &b){
	return (a.getValue() > b.getValue());
}

//Overloading >
bool operator>(score &a, score &b) {
	return (a.getValue() < b.getValue());
}

//Overloading <=
bool operator<=(score &a, score &b) {
	return !(a > b);
}

//Overloading >=
bool operator>=(score &a, score &b) {
	return !(a < b);
}