#include "header.h"
#include "MenuState.h"
#include "GameState.h"
#include "ScoreState.h"
#include "SaveNLoad.h"
#include "TutState.h"

#define MUSIC_PATH "./music/hiding-your-reality-by-kevin-macleod-from-filmmusic-io.wav"

//Constructor
MenuState::MenuState(psydjfiEngine* currentEngine) : engine(currentEngine) { 
	loadPossible = checkLoad();
	firstTime = !checkScore();
}

//Setup background
void MenuState::setupBackground() {
	//Background
	background = ImageManager::loadImage("./images/digitalrain.jpg", false);
	background = background.resizeTo(engine->getWindowWidth(), engine->getWindowHeight());
	background.renderImage(engine->getBackgroundSurface(), 0, 0, 0, 0, engine->getWindowWidth(), engine->getWindowHeight());

	//Menu Area
	engine->drawBackgroundRectangle(100, 0, 700, 750, 0x003300);
	engine->drawBackgroundRectangle(120, 0, 680, 750, 0x0e0e0e);

	drawStringsOnTop();
}

//Initialize Objects - Nothing
void MenuState::initializeObjects() {

}

//use handleEnter() when within the right bounds
void MenuState::mouseDown(int iButton, int iX, int iY) {
	if (iButton == SDL_BUTTON_LEFT) {
		if ((iX >= 143 && iX <= 274 && iY >= 198 && iY <= 227) ||
			(iX >= 143 && iX <= 374 && iY >= 248 && iY <= 288) ||
			(iX >= 143 && iX <= 455 && iY >= 308 && iY <= 348) ||
			((iX >= 143 && iX <= 272 && iY >= 368 && iY <= 408) && loadPossible) ||
			(iX >= 143 && iX <= 490 && iY >= 428 && iY <= 468) ||
			(iX >= 143 && iX <= 255 && iY >= 488 && iY <= 528)) {
			handleEnter();
		}
		
	}
}

//String rendering
void MenuState::drawStringsOnTop() {
	engine->lockForegroundForDrawing();

	engine->drawForegroundString(185, 40, "The", 0x00de00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 80));
	engine->drawForegroundString(135, 90, "Rabbit Hole", 0x00de00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 80));
	engine->drawForegroundString(145, 190, "Play", 0x004400, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
	engine->drawForegroundString(145, 250, "Tutorial", 0x004400, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
	engine->drawForegroundString(145, 310, "High Scores", 0x004400, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
	if (loadPossible){
		engine->drawForegroundString(145, 370, "Load", 0x004400, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
	} else {
		engine->drawForegroundString(145, 370, "Load", 0x002200, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
	}
	engine->drawForegroundString(145, 430, "Toggle Music", 0x004400, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
	engine->drawForegroundString(145, 490, "Exit", 0x004400, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));

	engine->drawForegroundString(1200, 710, "2020 PSYDJFI", 0xcccccc, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 20));
	engine->drawForegroundString(1075, 730, "MUSIC BY Kevin Macleod", 0xcccccc, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 20));

	
	
	switch (menuItem) {
		case 0: engine->drawForegroundString(143, 188, "Play", 0x00dd00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50)); break;
		case 1: engine->drawForegroundString(143, 248, "Tutorial", 0x00dd00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50)); break;
		case 2: engine->drawForegroundString(143, 308, "High Scores", 0x00dd00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50)); break;
		case 3: engine->drawForegroundString(143, 368, "Load", 0x00dd00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50)); break;
		case 4: engine->drawForegroundString(143, 428, "Toggle Music", 0x00dd00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50)); break;
		case 5: engine->drawForegroundString(143, 488, "Exit", 0x00dd00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50)); break;
	default:
		break;
	}
	if (menuItem == 0) {
		engine->drawForegroundString(145, 550, "You take the red pill", 0xff0000, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 30));
		engine->drawForegroundString(145, 580, "you stay in Wonderland", 0xff0000, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 30));
		engine->drawForegroundString(145, 610, "And I show you ", 0xff0000, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 30));
		engine->drawForegroundString(145, 640, "How deep the rabbit hole goes", 0xff0000, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 30));
	}
	else if (menuItem == 5){
		engine->drawForegroundString(145, 530, "You take the blue pill", 0x0000ff, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 30));
		engine->drawForegroundString(145, 560, "The story ends", 0x0000ff, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 30));
		engine->drawForegroundString(145, 590, "You wake up in your bed", 0x0000ff, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 30));
		engine->drawForegroundString(145, 620, "And believe", 0x0000ff, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 30));
		engine->drawForegroundString(145, 650, "whatever you want to believe", 0x0000ff, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 30));
	}
	
	engine->unlockForegroundForDrawing();
	engine->redrawDisplay();
}

//Key Presses
void MenuState::keyDown(int iKeyCode) {
	switch (iKeyCode) {
	case SDLK_ESCAPE:engine->exit(); delete this; break;				 
	case SDLK_DOWN: case SDLK_s : 
		menuItem = (menuItem + 1) % 6; 
		if (menuItem == 3 && !loadPossible) {
			menuItem = (menuItem + 1) % 6;
		}
		drawStringsOnTop(); 
		break;		  
	case SDLK_UP: case SDLK_w: 
		menuItem = (6 + (menuItem - 1)) % 6; 
		if (menuItem == 3 && !loadPossible) {
			menuItem = (6 + (menuItem - 1)) % 6;
		}
		drawStringsOnTop(); 
		break;
	case SDLK_KP_ENTER: case SDLK_RETURN: handleEnter(); break;
	default:
		break;
	}
}

//Enters the current menu option
void MenuState::handleEnter() {
	switch (menuItem) {
	case 0: if (!firstTime) {
				engine->setState(new GameState(engine));
			}
			else {
				engine->setState(new TutState(engine,false));
			}
			delete this;
			break;
	case 1: engine->setState(new TutState(engine,true)); delete this; break;
	case 2: engine->setState(new ScoreState(engine)); delete this; break;
	case 3: load(); break;
	case 4: engine->toggleMusic(); break;
	case 5: engine->stopMusic(); engine->exit(); delete this; break;
	default:
		break;
	}
}

//Mouse Menu handling
void MenuState::mouseMoved(int iX, int iY) {
	//printf("%d , %d \n", iX, iY);
	if (iX >= 143 && iX <= 274 && iY >= 188 && iY <= 228) {
		if (menuItem != 0) {
			menuItem = 0;
			drawStringsOnTop();
		}
	} else if (iX >= 143 && iX <= 374 && iY >= 248 && iY <= 288) {
		if (menuItem != 1) {
			menuItem = 1;
			drawStringsOnTop();
		}
	} else if (iX >= 143 && iX <= 455 && iY >= 308 && iY <= 348) {
		if (menuItem != 2) {
			menuItem = 2;
			drawStringsOnTop();
		}
	} else if (iX >= 143 && iX <= 272 && iY >= 368 && iY <= 408) {
		if (menuItem != 3 && loadPossible ) {
			menuItem = 3;
			drawStringsOnTop();
		}
	} else if (iX >= 143 && iX <= 490 && iY >= 428 && iY <= 468) {
		if (menuItem != 4) {
			menuItem = 4;
			drawStringsOnTop();
		}
	} else if (iX >= 143 && iX <= 255 && iY >= 488 && iY <= 528) {
		if (menuItem != 5) {
			menuItem = 5;
			drawStringsOnTop();
		}
	}
}

//BaseEngine call
void MenuState::postDraw() {
	engine->BaseEngine::virtPostDraw();
}

//Used to scroll background
void MenuState::mainLoopDoBeforeUpdate() {
	if (timer == 0) {
		offset = (engine->getWindowHeight() + (offset - 1)) % engine->getWindowHeight();
		engine->redrawDisplay();
		timer = BACKGROUND_SPEED;
	}
	else { timer--; }
}

//Used for background animation
void MenuState::copyBackgroundBuffer() {
	engine->getForegroundSurface()->copyRectangleFrom(engine->getBackgroundSurface(), 0, 0, engine->getWindowWidth(), engine->getWindowHeight(), 0, offset);
	engine->getForegroundSurface()->copyRectangleFrom(engine->getBackgroundSurface(), 0, engine->getWindowHeight() - offset, engine->getWindowWidth(), engine->getWindowHeight(), 0, offset - engine->getWindowHeight());
}

//Sets up background and starts music
void MenuState::initializeState() {
	engine->lockAndSetupBackground();
	engine->playMusic(MUSIC_PATH);
}

//BaseEngine version
bool MenuState::checkForUpdateTime() {
	return engine->BaseEngine::virtCheckForUpdateTime();
}

//Load the saved game
void MenuState::load() {
	Load* ld = new Load(engine);
	ld->startLoadedGame();
	delete this;
}

//Check if the save file is empty, to see whether you can or not load
bool MenuState::checkLoad() {
	std::unique_ptr<Load> ld(new Load(engine));
	string temp = ld->readFile();
	return (temp.size()  > 0);
}

//Check if there are any saved scores, if start tutorial on pressing play
bool MenuState::checkScore() {
	string temp;
	ifstream file("./saves/scores.txt");
	if (!file) {
		cout << "Failed to Open File" << endl;
		return nullptr;
	}
	else {
		getline(file, temp);
	}
	file.close();
	return temp.size() > 0;
}

