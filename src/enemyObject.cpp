#include "header.h"
#include "enemyObject.h"
#include "GameState.h"

using namespace std;

//Constructor with default movement speed and lifespan
enemyObject::enemyObject(int Xpos, int Ypos, psydjfiEngine* pEngine) : DisplayableObject(pEngine, 100, 75, true), engine(pEngine),image(ImageManager::loadImage("./images/enemy.png",false)),
		gState((GameState*)(engine->getState())), player((playerObject*)(engine->getDisplayableObject(0))), tm(engine->getTileManager()){
	m_iDrawWidth = image.getWidth();
	m_iDrawHeight = image.getHeight();
	m_iCurrentScreenX = Xpos;
	m_iCurrentScreenY = Ypos;
	int i = rand() % 4;
	switch (i){
	case 0: mapping = new mappingFlip(false, false, image.getPixelColour(0, 0)); break;
	case 1: mapping = new mappingFlip(true, false, image.getPixelColour(0, 0)); break;
	case 2: mapping = new mappingFlip(false, true, image.getPixelColour(0, 0)); break;
	case 3: mapping = new mappingFlip(true, true, image.getPixelColour(0, 0)); break;
	default:
		mapping = new mappingFlip(false, false, image.getPixelColour(0, 0));  break;
	}
}

//Constructor with custom movement speed and lifespan
enemyObject::enemyObject(int Xpos, int Ypos, psydjfiEngine* pEngine, int movSpeed, int lifeSpan) : DisplayableObject(pEngine, 100, 75, true), engine(pEngine), image(ImageManager::loadImage("./images/enemy.png", false)),
		gState((GameState*)(engine->getState())), player((playerObject*)(engine->getDisplayableObject(0))), tm(engine->getTileManager()),speed(movSpeed),lifetime(lifeSpan) {
	m_iDrawWidth = image.getWidth();
	m_iDrawHeight = image.getHeight();
	m_iCurrentScreenX = Xpos;
	m_iCurrentScreenY = Ypos;
	int i = rand() % 4;
	switch (i) {
	case 0: mapping = new mappingFlip(false, false, image.getPixelColour(0, 0)); break;
	case 1: mapping = new mappingFlip(true, false, image.getPixelColour(0, 0)); break;
	case 2: mapping = new mappingFlip(false, true, image.getPixelColour(0, 0)); break;
	case 3: mapping = new mappingFlip(true, true, image.getPixelColour(0, 0)); break;
	default:
		mapping = new mappingFlip(false, false, image.getPixelColour(0, 0));  break;
	}
}

//Load Constructor
enemyObject::enemyObject(int Xpos, int Ypos, psydjfiEngine* pEngine, int movSpeed, int lifeSpan, bool lclimbing, bool ldescending) :
		DisplayableObject(pEngine, 100, 75, true), engine(pEngine), image(ImageManager::loadImage("./images/enemy.png", false)),descending(ldescending),
		gState((GameState*)(engine->getState())), player((playerObject*)(engine->getDisplayableObject(0))), tm(engine->getTileManager()), speed(movSpeed), lifetime(lifeSpan), climbing(lclimbing) {
	m_iDrawWidth = image.getWidth();
	m_iDrawHeight = image.getHeight();
	m_iCurrentScreenX = Xpos;
	m_iCurrentScreenY = Ypos;
	int i = rand() % 4;
	switch (i) {
	case 0: mapping = new mappingFlip(false, false, image.getPixelColour(0, 0)); break;
	case 1: mapping = new mappingFlip(true, false, image.getPixelColour(0, 0)); break;
	case 2: mapping = new mappingFlip(false, true, image.getPixelColour(0, 0)); break;
	case 3: mapping = new mappingFlip(true, true, image.getPixelColour(0, 0)); break;
	default:
		mapping = new mappingFlip(false, false, image.getPixelColour(0, 0));  break;
	}
}

//Deconstructor -- Delete mapping object
enemyObject::~enemyObject() {
	delete mapping;
}

//Draws image 
void enemyObject::virtDraw() {
	image.renderImageApplyingMapping(engine,getEngine()->getForegroundSurface(), m_iCurrentScreenX + m_iStartDrawPosX, m_iCurrentScreenY + m_iStartDrawPosY, m_iDrawWidth, m_iDrawHeight,mapping);
	image.setTransparencyColour(image.getPixelColour(0,0));
}

//Sets image transparency color
void enemyObject::setTransparencyColour(int colour) { image.setTransparencyColour(colour); }

//gets image pixel coulour
int enemyObject::getPixelColour(int x, int y) { return image.getPixelColour(x, y); }

//Count down lifespan when reaches 0 end this object
void enemyObject::virtDoUpdate(int iCurrentTime) {
	if (isVisible()) {
		lifetime--;
		if (lifetime == 0) {
			end();
			return;
		}
		handleMovement();
	}
}

//finds shorter path towards player and moves
//Algorithm consists in if player is on the same level move towards it
// else find the space in the plataform where you fit where the sum of the distance from that space to you plus the distance from that space to the player is shorter and move towards the next level using that
void enemyObject::handleMovement() {
	int currentX = m_iCurrentScreenX;
	int currentY = convertY(tm->getMapYForScreenY(m_iCurrentScreenY));
	int playerX = player->getXCentre();
	int playerY = convertY(tm->getMapYForScreenY(player->getYCentre()));
	if (climbing || descending) {
		int temp = tm->getMapYForScreenY(m_iCurrentScreenY);
		if (climbing && (temp == 9 || temp == 17 || temp == 1)) {
			climbing = false;
			return;
		}
		if (descending && (temp == 10 || temp == 18 || temp == 26)) {
			descending = false;
			return;
		}
		if (climbing) {
			m_iCurrentScreenY -= speed;
		}
		else {
			m_iCurrentScreenY += speed;
		}
		return;
	}
	if (currentY == playerY) {
		target = -1;
		if (currentX > playerX - 50) {
			m_iCurrentScreenX -= speed;
		}
		else {
			m_iCurrentScreenX += speed;
		}
	}
	else {
		if (target == -1) {
			int tempDistance = -1;
			int iY;
			if (currentY > playerY) {
				iY = currentY - 1;
			}
			else {
				iY = currentY + 1;
			}
			vector<tuple<int, int>> spaces = gState->getSpacesCopy();
			int iterator = 0;
			while ( iterator + 1 < spaces.size() ) {
				if (get<1>(spaces[iterator]) == iY && get<0>(spaces[iterator])+1 == get<0>(spaces[iterator+1])) {
					int temp = calculateDistance(get<0>(spaces[iterator]), get<1>(spaces[iterator]));
					if (tempDistance == -1 || tempDistance > temp) {
						tempDistance = temp;
						target = get<0>(spaces[iterator]) * 50;
					}
				}
				iterator++;
			}
		}
		int moved = 0;
		while (currentX > target && moved < speed) {
			m_iCurrentScreenX -= 1;
			moved++;
		}
		while (currentX < target && moved < speed) {
			m_iCurrentScreenX += 1;
			moved++;
		}
		if (currentX == target) {
			if (currentY > playerY) {
				climbing = true;
				target = -1;
				m_iCurrentScreenY -= speed;
			}
			else {
				descending = true;
				target = -1;
				m_iCurrentScreenY += speed;
			}
		}
		
	}
}

//Enemy algorithm to find shortest path uses a slightly diferent Y leved map where each 7 empty tiles in a heigh level are reduced to a single level
//this converts from the normal tile manager tiles to enemy path finding tiles
int enemyObject::convertY(int Ypos) {
	if (Ypos < 7) {
		return 0;
	}
	else if (Ypos == 7) {
		return 1;
	}
	else if (Ypos < 15) {
		return 2;
	}
	else if (Ypos == 15) {
		return 3;
	}
	else if (Ypos < 23) {
		return 4;
	}
	else if (Ypos == 23) {
		return 5;
	}
	else {
		return 6;
	}
}

//Used to calculate the distance of a point to the player plus the point to this
int enemyObject::calculateDistance(int X, int Y) {
	int currentX = tm->getMapXForScreenX(m_iCurrentScreenX);
	int currentY = convertY(m_iCurrentScreenY);
	int playerX = tm->getMapXForScreenX(player->getXCentre());
	int playerY = convertY(player->getYCentre());
	return abs(currentY - Y) + abs(playerY - Y) + abs(currentX - X) + abs(playerX - X);
}

//Clean and delete this
void enemyObject::end() {
	engine->removeDisplayableObject(this);
	delete this;
}

//Pixel perfect collision when used with player collision
bool enemyObject::virtIsPositionWithinObject(int iX, int iY) {
	if (DisplayableObject::virtIsPositionWithinObject(iX, iY)) {
		int maskColour = image.getPixelColour(0, 0);
		return maskColour != image.getPixelColour(iX - m_iCurrentScreenX + m_iStartDrawPosX, iY - m_iCurrentScreenY + m_iStartDrawPosY);
	}
	return false;
}

//get current X postion
int enemyObject::getX() { return m_iCurrentScreenX; }

//get current Y position
int enemyObject::getY() { return m_iCurrentScreenY; }

//get lifetime
int enemyObject::getLifetime() { return lifetime; }

//get speed
int enemyObject::getSpeed() { return speed; }

//get descending -- going down = true
int enemyObject::getDescending() { return descending; }

//get climbing -- going up = true
int enemyObject::getClimbing() { return climbing; }

//get current target
int enemyObject::getTarget() { return target; }

//--------------------------------------------------------------------------------------mappingFlip-------------------------------------------------------------------------------------------------

//mapping that flips the image vertically, horizontally, both or neither, also can apply masking if you set it
bool mappingFlip::mapCoordinates(double& x, double& y, const SimpleImage& image) {
	if (x < 0) return false;
	if (y < 0) return false;
	if (x >= (image.getWidth() - 0.5)) return false;
	if (y >= (image.getHeight() - 0.5)) return false;

	if (horizontalFlip) {
		//flip horizontally
		x = image.getWidth() - x;
	}
	if (verticalFlip) {
		//flip vertically
		y = image.getHeight() - y;
	}

	if (x < 0) return false;
	if (y < 0) return false;
	if (x >= (image.getWidth() - 0.5)) return false;
	if (y >= (image.getHeight() - 0.5)) return false;
	//apply masking if set
	if (mask != -1 && image.getPixelColour((int)x, (int)y) == mask) return false;

	return true;
}