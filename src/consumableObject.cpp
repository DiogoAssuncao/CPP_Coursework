#include "header.h"
#include "consumableObject.h"
#include "playerObject.h"

//Constructor
consumableObject::consumableObject(int centerStartX, int centerStartY, int distPerMove, int speed, int pathSize, bool stopped, psydjfiEngine* engine, std::string URL) : 
		DisplayableObject(engine, 50, 100, true),
	    image(ImageManager::get()->getImagebyURL(URL, true, false)),
		engine(engine),
		up(true),
		stopMov(stopped),
		stopTimer(speed),
		iSpeed(speed),
		distXmove(distPerMove),
		size(pathSize),
		empty(false),
		tileX(0),
		tileY(0), value(-1) {
	m_iDrawWidth = image.getWidth();
	m_iDrawHeight = image.getHeight();
	m_iCurrentScreenX = centerStartX - m_iDrawWidth / 2;
	m_iCurrentScreenY = centerStartY - m_iDrawHeight / 2;
	originY = m_iCurrentScreenY;
}

//Placeholder for empty consumable
consumableObject::consumableObject(psydjfiEngine* engine) : DisplayableObject(engine, 50, 100, true), tileX(-1), tileY(-1), empty(true),
	up(false), stopMov(true), stopTimer(0), iSpeed(0), distXmove(0), size(0), originY(0), engine(engine), value(0) { }

//draw player image -- no need for imageObject
void consumableObject::virtDraw() {
	if (isVisible()) {
		image.renderImageWithMask(getEngine()->getForegroundSurface(), 0, 0, m_iCurrentScreenX + m_iStartDrawPosX, m_iCurrentScreenY + m_iStartDrawPosY, m_iDrawWidth, m_iDrawHeight);
	}
}

//Get current X tile
int consumableObject::getTileX() { return tileX; }

//Get current Y tile
int consumableObject::getTileY() { return tileY; }

//Sets Tile X
void consumableObject::setTileX(int X) { tileX = X; }

//Sets Tile Y
void consumableObject::setTileY(int Y) { tileY = Y; }

//gets empty
bool consumableObject::getEmpty() { return empty; }

//default behaviour class -- does nothing
void consumableObject::behaviour(){}

//move up and down
void consumableObject::virtDoUpdate(int iCurrentTime) {
	if (!stopMov) {
		if (stopTimer == 0) {
			if (m_iCurrentScreenY == originY + size) {
				up = false;
			}
			else if (m_iCurrentScreenY == originY) {
				up = true;
			}
			if (up) {
				m_iCurrentScreenY += distXmove;
			}
			else {
				m_iCurrentScreenY -= distXmove;
			}
			stopTimer = iSpeed;
		}
		else {
			stopTimer--;
		}
	}
	redrawDisplay();
}

//toggle move and stop
void consumableObject::toggleMovement() {
	stopMov = !stopMov;
}

//Pixel perfect collision detection -- when in combination with player object collision detection
bool consumableObject::virtIsPositionWithinObject(int iX, int iY) {
	if (DisplayableObject::virtIsPositionWithinObject(iX, iY)) {
		int maskColour = image.getPixelColour(0, 0);
		return maskColour != image.getPixelColour(iX - m_iCurrentScreenX + m_iStartDrawPosX, iY - m_iCurrentScreenY + m_iStartDrawPosY);
	}
	return false;
}

//Get value
int consumableObject::getValue() { return value; }

//--------------------------------------------------------------------------------- redPill Class ---------------------------------------------------------------------------------------------------------------------------------------------------

//Red pill contructor
redPill::redPill(int tileX, int tileY, psydjfiEngine* engine, playerObject* player) :
	consumableObject(tileX*50 + 25, tileY*25 - 2*25, 2, 4, 20, false, engine, "./images/redPill.png"), playerO(player) {
	value = 1;
	setTileX(tileX);
	setTileY(tileY);
}

//red pill behaviour
//Set player incencivle for a limited time
void redPill::behaviour() {
	//invencible for 15s
	playerO->setInvencible();
}

//--------------------------------------------------------------------------------- bluePill Class ---------------------------------------------------------------------------------------------------------------------------------------------------

//blue pill contructor
bluePill::bluePill(int tileX, int tileY, psydjfiEngine* engine, playerObject* player) :
	consumableObject(tileX * 50 + 25, tileY * 25 - 2 * 25, 2, 4, 20, false, engine, "./images/bluePill.png"), playerO(player) {
	value = 2;
	setTileX(tileX);
	setTileY(tileY);
}

//Erases all traps in current tm
//And redraws background where needed
void bluePill::behaviour() {
	//erase all traps and enemies
	shared_ptr<psydjfiTM> tm = engine->getTileManager();
	SimpleImage* background = ((GameState*)(engine->getState()))->getBackground();
	int tileX = 0, tileY = 0;
	while (tileX < tm->getMapWidth()) {
		while (tileY < tm->getMapHeight()) {
			if (tm->getMapValue(tileX, tileY) == 4) {
				tm->setMapValue(tileX, tileY, 0);
				int x = tileX * 50;
				int y = tileY * 25;
				engine->lockBackgroundForDrawing();
				background->renderImage(engine->getBackgroundSurface(), x, y, x, y, 50, 25);
				engine->unlockBackgroundForDrawing();
			}
			tileY++;
		}
		tileX++;
		tileY = 0;
	}
	((GameState*)(engine->getState()))->resetTraps();
}

//--------------------------------------------------------------------------------- greenPill Class ---------------------------------------------------------------------------------------------------------------------------------------------------

//green pill contructor
greenPill::greenPill(int tileX, int tileY, psydjfiEngine* engine, playerObject* player) :
	consumableObject(tileX * 50 + 25, tileY * 25 - 2 * 25, 2, 4, 20, false, engine, "./images/greenPill.png"), playerO(player) {
	value = 3;
	setTileX(tileX);
	setTileY(tileY);
}

//add a life to game state
void greenPill::behaviour() {
	//1UP
	((GameState*)(engine->getState()))->addLife();
}