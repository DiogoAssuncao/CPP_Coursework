#include "header.h"
#include "playerObject.h"
#include "psydjfiTM.h"
#include "CollisionDetection.h"
#include "enemyObject.h"

#define MAX_ACC_X 300
#define MAX_ACC_Y 950
#define FRICTION_X 20
#define FRICTION_Y 20
#define SPRITE_WIDTH 75
#define SPRITE_HEIGHT 120
#define DEFAULT_SPRITE_X 2325

//Constructor
playerObject::playerObject(int centerStartX, int centerStartY, psydjfiEngine* engine, shared_ptr<psydjfiTM>tileManager, GameState* state) : DisplayableObject(engine, SPRITE_WIDTH, SPRITE_HEIGHT,true),
			  sprite(ImageManager::get()->getImagebyURL("./images/sprite.png", true, false)), spriteAura(ImageManager::get()->getImagebyURL("./images/spriteAura.png", true, false)),
			  spriteHurt(ImageManager::get()->getImagebyURL("./images/spriteHurt.png", true, false)), tm(tileManager), oEngine(engine), gState(state), item(nullptr) {
	m_iDrawWidth = SPRITE_WIDTH;
	m_iDrawHeight = SPRITE_HEIGHT;
	Xoffset = DEFAULT_SPRITE_X;
	XoffsetTemp = DEFAULT_SPRITE_X;
	m_iCurrentScreenX = centerStartX - m_iDrawWidth/2;
	m_iCurrentScreenY = centerStartY - m_iDrawHeight/2;
}

//Load Constructor
playerObject::playerObject(int centerStartX, int centerStartY, psydjfiEngine* engine, shared_ptr<psydjfiTM>tileManager, GameState* state, int linvTimer, int lhit, int lehit) :
		DisplayableObject(engine, SPRITE_WIDTH, SPRITE_HEIGHT, true),
		sprite(ImageManager::get()->getImagebyURL("./images/sprite.png", true, false)), spriteAura(ImageManager::get()->getImagebyURL("./images/spriteAura.png", true, false)),
		spriteHurt(ImageManager::get()->getImagebyURL("./images/spriteHurt.png", true, false)), tm(tileManager), oEngine(engine), gState(state), item(nullptr),
		invencibleTimer(linvTimer), hit(lhit), enemyHit(lehit){
	m_iDrawWidth = SPRITE_WIDTH;
	m_iDrawHeight = SPRITE_HEIGHT;
	Xoffset = DEFAULT_SPRITE_X;
	XoffsetTemp = DEFAULT_SPRITE_X;
	m_iCurrentScreenX = centerStartX ;
	m_iCurrentScreenY = centerStartY ;
}

//Check which sprite to use
void playerObject::virtDraw() {
	//Check Invencible
	if (invencibleTimer > 0) {
		invencible = true;
		invencibleTimer--;
	}
	else { invencible = false; }
	if (isVisible()) {
		if (invencibleTimer > 0 && (hit || enemyHit)) {
			if (currentSprite == 2) {
				sprite.renderImageWithMask(getEngine()->getForegroundSurface(), Xoffset, 0, m_iCurrentScreenX + m_iStartDrawPosX, m_iCurrentScreenY + m_iStartDrawPosY, m_iDrawWidth, m_iDrawHeight);
				currentSprite = 0;
			}
			else {
				spriteHurt.renderImageWithMask(getEngine()->getForegroundSurface(), Xoffset, 0, m_iCurrentScreenX + m_iStartDrawPosX, m_iCurrentScreenY + m_iStartDrawPosY, m_iDrawWidth, m_iDrawHeight, spriteAura.getPixelColour(0, 0));
				currentSprite = 2;
			}
		}
		else if (invencibleTimer > 200) {
			spriteAura.renderImageWithMask(getEngine()->getForegroundSurface(), Xoffset, 0, m_iCurrentScreenX + m_iStartDrawPosX, m_iCurrentScreenY + m_iStartDrawPosY, m_iDrawWidth, m_iDrawHeight,spriteAura.getPixelColour(0,0));
			currentSprite = 1;
		}
		else if (invencibleTimer > 0){
			if (currentSprite == 1) {
				sprite.renderImageWithMask(getEngine()->getForegroundSurface(), Xoffset, 0, m_iCurrentScreenX + m_iStartDrawPosX, m_iCurrentScreenY + m_iStartDrawPosY, m_iDrawWidth, m_iDrawHeight);
				currentSprite = 0;
			}
			else {
				spriteAura.renderImageWithMask(getEngine()->getForegroundSurface(), Xoffset, 0, m_iCurrentScreenX + m_iStartDrawPosX, m_iCurrentScreenY + m_iStartDrawPosY, m_iDrawWidth, m_iDrawHeight, spriteAura.getPixelColour(0, 0));
				currentSprite = 1;
			}

		}
		else {
			sprite.renderImageWithMask(getEngine()->getForegroundSurface(), Xoffset, 0, m_iCurrentScreenX + m_iStartDrawPosX, m_iCurrentScreenY + m_iStartDrawPosY, m_iDrawWidth, m_iDrawHeight);
			currentSprite = 0;
		}
	}
}

//handles key presses and makes the call to handle movement and collisions
void playerObject::virtDoUpdate(int iCurrentTime){
	//Collision Check
	handleCollisions();
	//Key Presses
	if (getEngine()->isKeyPressed(SDLK_UP) || getEngine()->isKeyPressed(SDLK_w) || getEngine()->isKeyPressed(SDLK_SPACE)) {
		if (solidCollisionDown && jump) {
			accelarationY = - MAX_ACC_Y;
		}
	}
	if (getEngine()->isKeyPressed(SDLK_LEFT) || getEngine()->isKeyPressed(SDLK_a)) {
		if (accelarationX > -MAX_ACC_X) {
			accelarationX -= 100;
		}
		faceRight = false;
	}
	if (getEngine()->isKeyPressed(SDLK_RIGHT) || getEngine()->isKeyPressed(SDLK_d)) {
		if (accelarationX < MAX_ACC_X) {
			accelarationX += 100;
		}
		faceRight = true;
	}
	if (getEngine()->isKeyPressed(SDLK_DOWN) || getEngine()->isKeyPressed(SDLK_s)) {
		if (onObject) {
			gState->gameSpawner();
		}
	}
	//Movement
	handleMovement();
}

//handles collisons
//Pixel perfect collisions for walls and tiles, enemies and consumables
//trap collision made weaker to allow for more player mistakes
void playerObject::handleCollisions() {
	bool borderDown = false, borderUp = false, borderLeft = false, borderRight = false;
	bool tileCollisionDown = false, tileCollisionUp = false, tileCollisionLeft = false, tileCollisionRight = false;
	int tileX = 0, tileY = 0;
	int Xpos = Xoffset, Ypos = 0, Xscreen = 0, Yscreen = 0;
	int maskColour = sprite.getPixelColour(0,Ypos);
	bool tempHit = false;
	solidCollisionDown = false;
	solidCollisionUp = false;
	solidCollisionLeft = false;
	solidCollisionRight = false;
	//Check every border Pixel (Mask - Character) for collisions
	while (Ypos < 120) {
		while (Xpos < Xoffset + 75) {
			if (sprite.getPixelColour(Xpos, Ypos) != maskColour) {
				Xscreen = m_iCurrentScreenX + Xpos - Xoffset;
				Yscreen = m_iCurrentScreenY + Ypos;
				//Checks the four connected Pixels
				//Right
				if (sprite.getPixelColour(Xpos+1, Ypos) == maskColour) {
					//Walls
					if(!solidCollisionRight){
						borderRight = Xscreen + 1 >= getEngine()->getWindowWidth();
						if (tm->isValidTilePosition(Xscreen + 1, Yscreen)) {
							tileX = tm->getMapXForScreenX(Xscreen + 1);
							tileY = tm->getMapYForScreenY(Yscreen);
							tileCollisionRight = (tm->getMapValue(tileX, tileY) == 1 || tm->getMapValue(tileX, tileY) == 2);
						}
						solidCollisionRight = borderRight || tileCollisionRight;
					}
					//Consumable
					if (item->isVisible()) {
						if (item->virtIsPositionWithinObject(Xscreen, Yscreen - 1)) {
							hitConsumable();
						}
					}
					//Enemy
					int k = 2;
					enemyObject* temp;
					while (k < oEngine->getSizeObjectArray() ) {
						if (temp = (enemyObject*)oEngine->getDisplayableObject(k)) {
							if (temp->virtIsPositionWithinObject(Xscreen, Yscreen - 1) && temp->isVisible()) {
								if (!invencible) {
									tempHit = true;
								}
								temp->end();
							}
						}
						k++;
					}
				}
				//Left
				if (sprite.getPixelColour(Xpos - 1, Ypos) == maskColour) {
					//Walls
					if (!solidCollisionLeft) {
						borderLeft = Xscreen - 1 <= 0;
						if (tm->isValidTilePosition(Xscreen - 1, Yscreen)) {
							tileX = tm->getMapXForScreenX(Xscreen - 1);
							tileY = tm->getMapYForScreenY(Yscreen);
							tileCollisionLeft = (tm->getMapValue(tileX, tileY) == 1 || tm->getMapValue(tileX, tileY) == 2);
						}
						solidCollisionLeft = borderLeft || tileCollisionLeft;
					}
					//Consumable
					if (item->isVisible()) {
						if (item->virtIsPositionWithinObject(Xscreen, Yscreen - 1)) {
							hitConsumable();
						}
					}
					//Enemy
					int k = 2;
					enemyObject* temp;
					while (temp = (enemyObject*)oEngine->getDisplayableObject(k)) {
						if (temp->virtIsPositionWithinObject(Xscreen, Yscreen - 1) && temp->isVisible()) {
							if (!invencible) {
								tempHit = true;
							}
							temp->end();
						}
						k++;
					}
				}
				//Down
				if ((sprite.getPixelColour(Xpos, Ypos + 1) == maskColour || Ypos == 119)) {
					//Walls
					if (!solidCollisionDown) {
						borderDown = Yscreen + 1 >= getEngine()->getWindowHeight();
						if (tm->isValidTilePosition(Xscreen, Yscreen + 1)) {
							tileX = tm->getMapXForScreenX(Xscreen);
							tileY = tm->getMapYForScreenY(Yscreen + 1);
							tileCollisionDown = (tm->getMapValue(tileX, tileY) == 1 || tm->getMapValue(tileX, tileY) == 2);
						}
						solidCollisionDown = borderDown || tileCollisionDown;
					}
					//Consumable
					if (item->isVisible()) {
						if (item->virtIsPositionWithinObject(Xscreen, Yscreen - 1)) {
							hitConsumable();
						}
					}
					//Enemy
					int k = 2;
					enemyObject* temp;
					while (temp = (enemyObject*)oEngine->getDisplayableObject(k)) {
						if (temp->virtIsPositionWithinObject(Xscreen, Yscreen - 1) && temp->isVisible()) {
							if (!invencible) {
								tempHit = true;
							}
							temp->end();
						}
						k++;
					}
				}
				//Up
				if (sprite.getPixelColour(Xpos, Ypos - 1) == maskColour) {
					//Walls
					if (!solidCollisionUp) {
						borderUp = Yscreen - 1 <= 0;
						if (tm->isValidTilePosition(Xscreen, Yscreen - 1)) {
							tileX = tm->getMapXForScreenX(Xscreen);
							tileY = tm->getMapYForScreenY(Yscreen - 1);
							tileCollisionUp = (tm->getMapValue(tileX, tileY) == 1 || tm->getMapValue(tileX, tileY) == 2);
						}
						solidCollisionUp = borderUp || tileCollisionUp;
					}
					//Consumable
					if (item->isVisible()) {
						if (item->virtIsPositionWithinObject(Xscreen, Yscreen - 1)) {
							hitConsumable();
						}
					}
					//Enemy
					int k = 2;
					enemyObject* temp;
					while (temp = (enemyObject*)oEngine->getDisplayableObject(k)) {
						if (temp->virtIsPositionWithinObject(Xscreen, Yscreen - 1) && temp->isVisible()) {
							if (!invencible) {
								tempHit = true;
							}
							temp->end();
						}
						k++;
					}
					
				}
			}
			Xpos++;
		}
		Xpos = Xoffset;
		Ypos++;
	}
	//Enemy hit check
	if (!invencible) {
		if (tempHit) {
			enemyHit = true;
			gState->lose();
			invencibleTimer = 100;
			invencible = true;
		}
		else {
			enemyHit = false;
		}
	}
	
	//Wether the player is close enough to activate the objective
	Xpos = getXCentre();
	Ypos = getYCentre();
	tileX = tm->getMapXForScreenX(Xpos);
	tileY = tm->getMapYForScreenY(Ypos);
	if (tm->getMapValue(tileX, tileY) == 3) {
		onObject = true;
	} else { onObject = false; }
	//Trap Collision - Does not use pixel perfect collision because it would make the game near impossible to play in a difficulty perspective
	//				   Simpler collision allows for more player mistakes decreasing the overall difficulty	
	if (accelarationY == 0 && !invencible) {
		int Xpos2 = m_iCurrentScreenX + getDrawWidth() - 27;
		int tileX2 = tileX = tm->getMapXForScreenX(Xpos2);
		Xpos = m_iCurrentScreenX + 20;
		Ypos = m_iCurrentScreenY + getDrawHeight() - 10;
		tileX = tm->getMapXForScreenX(Xpos);
		tileY = tm->getMapYForScreenY(Ypos);
		if (tm->getMapValue(tileX, tileY) == 4 || tm->getMapValue(tileX2, tileY) == 4) {
			if (((Xpos > tileX * 50 + 2 || Xpos < tileX * 50 + 48) && (Ypos > tileY * 25 + 5 || Ypos < tileY * 25 + 25)) ||
				((Xpos2 > tileX2 * 50 + 2 || Xpos2 < tileX2 * 50 + 48) && (Ypos > tileY * 25 + 5 || Ypos < tileY * 25 + 25))) {
				hit = true;
				gState->lose();
				invencibleTimer = 100;
				invencible = true;
			}
			
			
		} else {
				hit = false;
			}
	}
	//Small wait before you can jump again after you land
	if (solidCollisionDown) {
		if (jumpTimer < 40) {
			jump = false;
		}
		else { jump = true; }
		jumpTimer++;
	}
	else {
		jumpTimer = 0;
	}
}

//handles movements and calls update sprite
//Movement is handled with accelarition and friction
//accelariting one way will create oposite friction 
//Moves accelaration/100 pixels at a time
void playerObject::handleMovement() {
	//Stop accelaration if in collision
	if (solidCollisionLeft && accelarationX < 0) {
		accelarationX = 0;
	}
	if (solidCollisionRight && accelarationX > 0) {
		accelarationX = 0;
	}
	if (solidCollisionUp && accelarationY < 0) {
		accelarationY = 0;
	}
	//apply friction
	if (accelarationX > 0) {
		accelarationX -= FRICTION_X;
	}
	else if (accelarationX < 0) {
		accelarationX += FRICTION_X;
	}
	if (!solidCollisionDown) {
		accelarationY += FRICTION_Y;
	}
	else if(accelarationY > 0){
		accelarationY = 0;
	}
	//Move Calculation
 	int moveX = accelarationX / 100;
	int moveY = accelarationY / 100;
	//Update the Sprite (On a timer so animation seems fluid)
	if (updateTimer == 2) {
		updateSprite();
		updateTimer = 0; 
	}
	else { updateTimer++; }
	//Movement (1 by 1 for collision testing at every step)
	//Right Movement
	while (moveX > 0 && !solidCollisionRight) {
		m_iCurrentScreenX += 1;
		moveX--;
		handleCollisions();
	} while (solidCollisionRight) {
		m_iCurrentScreenX -= 1;
		handleCollisions();
	} // So the object does not get stuck in walls
	//Left Movement
	while (moveX < 0 && !solidCollisionLeft) {
		m_iCurrentScreenX -= 1;
		moveX++;
		handleCollisions();
	} while (solidCollisionLeft) {
		m_iCurrentScreenX += 1;
		handleCollisions();
	} // So the object does not get stuck in walls
	//Down Movement
	while (moveY > 0 && !solidCollisionDown) {
		m_iCurrentScreenY += 1;
		moveY--;
		handleCollisions();
	}
	//Up movement
	while (moveY < 0 && !solidCollisionUp) {
		m_iCurrentScreenY -= 1;
		moveY++;
		handleCollisions();
	}
}

//update sprite according to accelaration
void playerObject::updateSprite() {
	//Find correct sprite
	if (accelarationX == 0 && accelarationY == 0) {
		//Not moving 
		XoffsetTemp = DEFAULT_SPRITE_X;
	}
	else if (accelarationY > 0) {
		//Falling - Timer added to avoid collison errors due to the difference in sprite
		if (timer == 0) {
			XoffsetTemp = 2475;
			timer = 7;
		}
		else { timer--; }
	}
	else if (accelarationY < 0) {
		//Jumping
		XoffsetTemp = 2400;
	}
	else if (XoffsetTemp >= 2250 ) {
		//reset for movement start
		XoffsetTemp = 0;
	}
	else {
		//movement sprites
		XoffsetTemp += 75;
	}
	//Direction ajusting
	if (!faceRight) {
		//going Left
		Xoffset = 5025 - XoffsetTemp;
	}
	else {
		//going Right
		Xoffset = XoffsetTemp;
	}
}

//gets the current consumable
consumableObject* playerObject::getConsumable() {
	return item;
}

//Sets consumable
void playerObject::setConsumable(consumableObject* newItem) {
	item = newItem;
}

//increases invencible timer
void playerObject::setInvencible() {
	invencibleTimer = 1000;
}

//function for when consumable is hit
//calls the behaviour function of consumableObject
void playerObject::hitConsumable() {
	item->setVisible(false);
	item->behaviour();
}

//Get X position
int playerObject::getX() { return m_iCurrentScreenX; }

//Gets Y position
int playerObject::getY() { return m_iCurrentScreenY; }

//Gets invencible timer
int playerObject::getITimer() { return invencibleTimer; }

//Gets hit
int playerObject::getHit() { return hit; }

//gets enemy hit
int playerObject::getEHit() { return enemyHit; }