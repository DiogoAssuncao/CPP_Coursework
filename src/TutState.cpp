#include "header.h"
#include "TutState.h"
#include "playerObject.h"
#include "enemyObject.h"
#include "MenuState.h"

//returnToMenu is used when playing for the first time,
//Instead of going straight to the game if no scores are found the game will first run the turorial and then jump straight into the game returnToMenu = false
//when opening tutorial from menu it will then return to menu as sooon at it finishes returnToMenu = true
TutState::TutState(psydjfiEngine* currentEngine, bool returnToMenu): engine(currentEngine), zoom(new FilterPointsScaling(2,2)), translation(new FilterPointsTranslation(0, 0, zoom)), goToGame(!returnToMenu) {}

//Setup Background
void TutState::setupBackground(){
	morpheus = ImageManager::loadImage("./images/morpheus.png");
	pill1 = ImageManager::loadImage("./images/redPill.png", true);
	pill2 = ImageManager::loadImage("./images/bluePill.png", true);
	pill3 = ImageManager::loadImage("./images/greenPill.png", true);
	tile = ImageManager::loadImage("./images/brick2.png", true);
	trap = ImageManager::loadImage("./images/glitch.png", true);
	payphone = ImageManager::loadImage("./images/payphone.png", true);
	morpheus = morpheus.resizeTo(200, 255);
	engine->fillBackground(0x000000);
	engine->drawBackgroundThickLine(200,0,200,300,0xeeeeee,2);
	engine->drawBackgroundThickLine(1150, 0, 1150, 300, 0xeeeeee, 2);
	engine->drawBackgroundThickLine(0, 390, 200, 300, 0xeeeeee, 2);
	engine->drawBackgroundThickLine(1350, 390, 1150, 300, 0xeeeeee, 2);
	engine->drawBackgroundThickLine(200, 300, 1150, 300, 0xeeeeee, 2);
	engine->drawBackgroundRectangle(0, 400, 1350, 750, 0x0e0e0e);
	engine->drawBackgroundRectangle(0, 390, 1350, 395, 0x00aa00);
	engine->drawBackgroundRectangle(0, 400, 1350, 410, 0x00aa00);
	engine->drawBackgroundRectangle(0, 400, 10, 750, 0x00aa00);
	engine->drawBackgroundRectangle(1340, 400, 1350, 750, 0x00aa00);
	engine->drawBackgroundRectangle(0, 740, 1350, 750, 0x00aa00);
	morpheus.renderImageWithMask(engine->getBackgroundSurface(), 0, 0, 60, 450, morpheus.getWidth(), morpheus.getHeight());
	engine->drawBackgroundString(930, 710, "Press Enter to Continue...", 0x00aa00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 30));
}

//Initialize Objects -- Player and Enemy
void TutState::initializeObjects(){
	engine->drawableObjectsChanged();
	engine->storeObjectInArray(new playerObject(325, 88, engine, engine->getTileManager(), nullptr));
	engine->storeObjectInArray(new consumableObject(engine));
	engine->storeObjectInArray(new enemyObject(400, 678,engine));
	engine->setAllObjectsVisible(true);
	engine->getDisplayableObject(1)->setVisible(false);
	((playerObject*)(engine->getDisplayableObject(0)))->setConsumable(((consumableObject*)(engine->getDisplayableObject(1))));
}

//Does nothing
void TutState::mouseDown(int iButton, int iX, int iY){
}

//Draws images
void TutState::drawStringsOnTop(){
	engine->lockForegroundForDrawing();
	pill1.renderImageWithMask(engine->getForegroundSurface(), 0, 0, 185, 378, pill1.getWidth(), pill1.getHeight());
	pill2.renderImageWithMask(engine->getForegroundSurface(), 0, 0, 310, 378, pill2.getWidth(), pill2.getHeight());
	pill3.renderImageWithMask(engine->getForegroundSurface(), 0, 0, 435, 378, pill3.getWidth(), pill3.getHeight());
	tile.renderImageWithMask(engine->getForegroundSurface(), 0, 0, 250, 710, tile.getWidth(), tile.getHeight());
	tile.renderImageWithMask(engine->getForegroundSurface(), 0, 0, 150, 710, tile.getWidth(), tile.getHeight());
	tile.renderImageWithMask(engine->getForegroundSurface(), 0, 0, 200, 710, tile.getWidth(), tile.getHeight());
	trap.renderImageWithMask(engine->getForegroundSurface(), 0, 0, 200, 685, trap.getWidth(), trap.getHeight());
	payphone.renderImageWithMask(engine->getForegroundSurface(), 0, 0, 675, 29, payphone.getWidth(), payphone.getHeight());
	engine->unlockForegroundForDrawing();
	engine->redrawDisplay();
}

//Enter calls handleState
void TutState::keyDown(int iKeyCode){
	switch (iKeyCode){
	case SDLK_KP_ENTER: case SDLK_RETURN: handleState(); break;
	default:
		break;
	}
}

//Does Nothing
void TutState::mouseMoved(int iX, int iY){}

//BaseEngine Version
void TutState::postDraw() { engine->BaseEngine::virtPostDraw(); }

//BaseEngine Version
void TutState::mainLoopDoBeforeUpdate() { drawStringsOnTop();  engine->BaseEngine::virtMainLoopDoAfterUpdate(); }

//BaseEngine Version
void TutState::copyBackgroundBuffer() { engine->BaseEngine::copyAllBackgroundBuffer(); }

//Starts State
//Setup background
//Set drawpoint filter to the foreground surface - translation then zoom
//not required for background , as we want the same background throughout
//Initializes Objects
//Zooms in -- it will remain in zoomed in state until state ends
//Plays music
void TutState::initializeState(){
	engine->lockAndSetupBackground();
	engine->getForegroundSurface()->setDrawPointsFilter(translation);
	initializeObjects();
	zoom->compress();
	engine->playMusic("./music/rising-tide-faster-by-kevin-macleod-from-filmmusic-io.wav");
	handleState();
}

//Does not Update objects
bool TutState::checkForUpdateTime() { return false; }

//Handles conversation with key presses
//Uses translation to scrool around to the several objects and images
void TutState::handleState() {
	SimpleImage neo;
	engine->lockBackgroundForDrawing();
	engine->drawBackgroundRectangle(300, 450, 1320, 700, 0x0e0e0e);
	switch (state) {
	case 0: engine->drawBackgroundString(320, 500, "Welcome to the Matrix!", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50)); break;
	case 1: engine->drawBackgroundString(320, 500, "You can move with WASD or ARROW keys", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50)); 
			engine->drawBackgroundString(320, 550, "You can also jump with SPACE", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			break;
	case 2: translation->changeOffset(-180, 0);
			engine->drawBackgroundString(320, 500, "These Are your objectives", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			break;
	case 3: engine->drawBackgroundString(320, 500, "You must get as many of these", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50)); 
			engine->drawBackgroundString(320, 550, "As you can!", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			break;
	case 4: engine->drawBackgroundString(320, 500, "You can press S or DOWN ARROW", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			engine->drawBackgroundString(320, 550, "To interact with them", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			break;
	case 5:	engine->drawBackgroundString(320, 500, "The Score will keep track", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			engine->drawBackgroundString(320, 550, "of how many you have gotten", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			break;
	case 6: engine->drawBackgroundString(320, 500, "You can find it in the", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			engine->drawBackgroundString(320, 550, "Top right of your screen", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			engine->drawBackgroundString(1000, 5, "0000", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			break;
	case 7: translation->changeOffset(180, -620);
			engine->drawBackgroundString(320, 500, "The are glicthes in the matrix!", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			break;
	case 8: engine->drawBackgroundString(320, 500, "The First one will appear", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			engine->drawBackgroundString(320, 550, "Randomly in the platforms ", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			engine->drawBackgroundString(320, 600, "you can stand on ", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			break;
	case 9: engine->drawBackgroundString(320, 500, "The Second one will chase you", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			engine->drawBackgroundString(320, 550, "Their speed is related ", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			engine->drawBackgroundString(320, 600, "to their life span  ", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			break;
	case 10:engine->drawBackgroundString(320, 500, "The slower they are", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			engine->drawBackgroundString(320, 550, "the longer they live", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			break;
	case 11:engine->drawBackgroundString(320, 500, "You must not let them touch you", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			engine->drawBackgroundString(320, 550, "So steer clear!", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			break;
	case 12:engine->drawBackgroundString(320, 500, "If they do touch you", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			engine->drawBackgroundString(320, 550, "You will lose a life", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			break;
	case 13:engine->drawBackgroundString(320, 500, "If you lose all your lives", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			engine->drawBackgroundString(320, 550, "it is game over", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			break;
	case 14:engine->drawBackgroundString(320, 500, "You can keep track of lives", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			engine->drawBackgroundString(320, 550, "On the top left of your screen", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			neo = ImageManager::loadImage("./images/neo.png", false);
			neo = neo.resizeTo(25, 35);
			neo.renderImageWithMask(engine->getBackgroundSurface(), 0, 0, 215, 5, 25, 35);
			engine->drawBackgroundString(250, 5, "3", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			break;
	case 15:translation->changeOffset(0, +320);
			engine->drawBackgroundString(320, 500, "These Will help you!", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			engine->drawBackgroundString(320, 550, "You just need to touch them", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			break;
	case 16:engine->drawBackgroundString(320, 500, "They will appear everytime", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			engine->drawBackgroundString(320, 550, "you get 10 objectives", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			break;
	case 17:engine->drawBackgroundString(320, 500, "The Red one will give you", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			engine->drawBackgroundString(320, 550, "A protective aura that makes you", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			engine->drawBackgroundString(320, 600, "Invencible for a limited time", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			break;
	case 18:engine->drawBackgroundString(320, 500, "The Green one will give you", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			engine->drawBackgroundString(320, 550, "an extra life", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			break;
	case 19:engine->drawBackgroundString(320, 500, "The blue one will ", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			engine->drawBackgroundString(320, 550, "Clear the plataforms", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			engine->drawBackgroundString(320, 600, "of all glitches!", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			break;
	case 20:engine->drawBackgroundString(320, 500, "Use them wisely", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			engine->drawBackgroundString(320, 550, "Only one can appear at a time", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			engine->drawBackgroundString(320, 600, "So don't keep them for long", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			break;
	case 21:translation->changeOffset(0, +300);
			engine->drawBackgroundString(320, 500, "The rest is up to you", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			break;
	case 22:engine->drawBackgroundString(320, 500, "Are you the one?", 0x00ee00, engine->getFont("./resources/SquareGalaxy-ad2K.ttf", 50));
			break;
	case 23:
		engine->unlockBackgroundForDrawing(); finish(); return;
	default:
		break;
	}
	engine->unlockBackgroundForDrawing();
	engine->redrawDisplay();
	state++;
}

//Clears up and deletes itself
//Sets the filter to within surface bounds to replace zoom and translation
//destroys objects
//deletes zoom and translation
//If Go to game is true goes straight into a game State else returns to menu
void TutState::finish() {
	engine->getForegroundSurface()->setDrawPointsFilter(new FilterPointsWithinSurfaceBounds());
	engine->drawableObjectsChanged();
	engine->destroyOldObjects();
	delete zoom;
	delete translation;
	if (goToGame) {
		engine->setState(new GameState(engine));
	}
	else {
		engine->setState(new MenuState(engine));
	}
	delete this;
}