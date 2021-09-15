#include "header.h"
#include "PauseState.h"
#include "MenuState.h"
#include "playerObject.h"
#include "consumableObject.h"
#include "psydjfiTM.h"
#include "SaveNLoad.h"

//Constructor
PauseState::PauseState(psydjfiEngine* currentEngine, GameState* oldState) : engine(currentEngine), gameState(oldState) { }

//Set up background
void PauseState::setupBackground() {
}

//Initialize Objects - Nothing
void PauseState::initializeObjects() {
}

//HandleEnter if in possible bounds
void PauseState::mouseDown(int iButton, int iX, int iY) {
	if (iButton == SDL_BUTTON_LEFT) {
		if ((iX >= 143 && iX <= 340 && iY >= 198 && iY <= 227) ||
			(iX >= 143 && iX <= 490 && iY >= 258 && iY <= 288) ||
			(iX >= 143 && iX <= 507 && iY >= 318 && iY <= 348) ||
			(iX >= 143 && iX <= 255 && iY >= 378 && iY <= 410)) {
			handleEnter();
		}

	}
}

//Menu Strings
void PauseState::drawStringsOnTop() {
	engine->lockForegroundForDrawing();

	engine->drawForegroundRectangle(100, 0, 700, 1350, 0x003300);
	engine->drawForegroundRectangle(120, 0, 680, 1350, 0x0e0e0e);

	engine->drawForegroundString(145, 200, "Resume", 0x004400, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
	engine->drawForegroundString(145, 260, "Toggle Music", 0x004400, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
	engine->drawForegroundString(145, 320, "Save and Exit", 0x004400, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
	engine->drawForegroundString(145, 380, "Exit", 0x004400, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));


	switch (menuItem) {
	case 0: engine->drawForegroundString(143, 198, "Resume", 0x00dd00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50)); break;
	case 1: engine->drawForegroundString(143, 258, "Toggle Music", 0x00dd00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50)); break;
	case 2: engine->drawForegroundString(143, 318, "Save and Exit", 0x00dd00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50)); break;
	case 3: engine->drawForegroundString(143, 378, "Exit", 0x00dd00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50)); break;
	default:
		break;
	}

	engine->unlockForegroundForDrawing();
	engine->redrawDisplay();
}

//Navigating menu
void PauseState::keyDown(int iKeyCode) {
	switch (iKeyCode) {
	case SDLK_ESCAPE:engine->setState(gameState); delete this; break;
	case SDLK_DOWN: case SDLK_s: menuItem = (menuItem + 1) % 4; drawStringsOnTop(); break;
	case SDLK_UP: case SDLK_w: menuItem = (4 + (menuItem - 1)) % 4; drawStringsOnTop(); break;
	case SDLK_KP_ENTER: case SDLK_RETURN: handleEnter(); break;
	default: 
		break;
	}
}

//Handle button clicks and key presses
void PauseState::handleEnter() {
	switch (menuItem) {
	case 0: engine->setState(gameState); delete this; break;
	case 2: save();break;
	case 1: engine->toggleMusic(); break;
	case 3: exitToMenu(); break;
	default:
		break;
	}
}

//Draw strings on star
void PauseState::initializeState() {
	drawStringsOnTop();
}

//change menu items for mouse navigation
void PauseState::mouseMoved(int iX, int iY) {
	//printf("%d, %d\n", iX, iY);
	if (iX >= 143 && iX <= 340 && iY >= 198 && iY <= 227) {
		if (menuItem != 0) {
			menuItem = 0;
			drawStringsOnTop();
		}
	}
	else if (iX >= 143 && iX <= 490 && iY >= 258 && iY <= 288) {
		if (menuItem != 1) {
			menuItem = 1;
			drawStringsOnTop();
		}
	}
	else if (iX >= 143 && iX <= 507 && iY >= 318 && iY <= 348) {
		if (menuItem != 2) {
			menuItem = 2;
			drawStringsOnTop();
		}
	}
	else if (iX >= 143 && iX <= 255 && iY >= 378 && iY <= 410) {
		if (menuItem != 3) {
			menuItem = 3;
			drawStringsOnTop();
		}
	}
}

//BaseEngine version
void PauseState::postDraw() {
	engine->BaseEngine::virtPostDraw();
}

//BaseEngine version
void PauseState::mainLoopDoBeforeUpdate() {
	engine->BaseEngine::virtMainLoopDoBeforeUpdate();
}

//BaseEngine version
void PauseState::copyBackgroundBuffer() {
	engine->BaseEngine::copyAllBackgroundBuffer();
}

//DONT update objects
bool PauseState::checkForUpdateTime() {
	return false;
}

//Return to menu delete gameState
void PauseState::exitToMenu() {
	shared_ptr<psydjfiTM> tm = engine->getTileManager();
	for (int i = 0; i < tm->getMapWidth(); i++) {
		for (int ii = 0; ii < tm->getMapHeight(); ii++) {
			tm->setMapValue(i, ii, 0);
		}
	}
	engine->drawableObjectsChanged();
	engine->destroyOldObjects();
	engine->setState(new MenuState(engine));
	delete gameState;
	delete this;
}

//Save game state and return to menu
void PauseState::save() {
	Save* sv = new Save(engine, gameState);
	sv->saveAll();
	delete sv;
	exitToMenu();
}