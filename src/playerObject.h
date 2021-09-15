#pragma once
#include "header.h"
#include "ImageManager.h"
#include "DisplayableObject.h"
#include "psydjfiEngine.h"
#include "GameState.h"
#include "consumableObject.h"

class psydjfiTM;
class psydjfiEngine;
class playerObject : public DisplayableObject {
public:
	playerObject(int centerStartX, int centerStartY, psydjfiEngine* engine, shared_ptr<psydjfiTM>tileManager, GameState* state);
	playerObject(int centerStartX, int centerStartY, psydjfiEngine* engine, shared_ptr<psydjfiTM>tileManager, GameState* state,int linvTimer, int lhit, int lehit);
	void virtDraw();
	void virtDoUpdate(int iCurrentTime);
	void handleCollisions();
	void handleMovement();
	void updateSprite();
	void setConsumable(consumableObject* newItem);
	void setInvencible();
	void hitConsumable();
	consumableObject* getConsumable();
	int getX();
	int getY();
	int getITimer();
	int getHit();
	int getEHit();
private:
	shared_ptr<psydjfiTM> tm;
	psydjfiEngine* oEngine;
	GameState* gState;
	SimpleImage sprite, spriteAura, spriteHurt;
	consumableObject* item;
	//collisions
	bool invencible = false;
	int invencibleTimer = 0;
	bool hit = false;
	bool enemyHit = false;
	bool onObject = false;
	bool solidCollisionUp = false;
	bool solidCollisionDown = false;
	bool solidCollisionLeft = false;
	bool solidCollisionRight = false;
	//movement
	int accelarationX = 0;
	int accelarationY = 0;
	int jumpTimer = 0;
	bool jump = false;
	//Sprite
	int currentSprite = 0;
	int Xoffset, XoffsetTemp;
	int updateTimer = 0, timer = 7;
	bool faceRight = true;

};

