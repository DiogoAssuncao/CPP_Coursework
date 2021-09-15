#pragma once
#include "header.h"
#include "ImageManager.h"
#include "DisplayableObject.h"
#include "psydjfiEngine.h"
#include "playerObject.h"
#include "psydjfiTM.h"
#include "CoordinateMapping.h"

class GameState;

class enemyObject : public DisplayableObject {
public:
	enemyObject(int Xpos , int Ypos , psydjfiEngine* engine);
	enemyObject(int Xpos, int Ypos, psydjfiEngine* pEngine, int movSpeed, int lifeSpan);
	enemyObject(int Xpos, int Ypos, psydjfiEngine* pEngine, int movSpeed, int lifeSpan, bool lclimbing, bool ldescending);
	~enemyObject();
	void setTransparencyColour(int colour);
	int getPixelColour(int x, int y);
	void virtDraw();
	void virtDoUpdate(int iCurrentTime);
	void handleMovement();
	int convertY(int Ypos);
	int calculateDistance(int X, int Y);
	void end();
	bool virtIsPositionWithinObject(int iX, int iY);
	int getX();
	int getY();
	int getLifetime();
	int getSpeed();
	int getDescending();
	int getClimbing();
	int getTarget();
private:
	int lifetime = 2500;
	int speed = 2;
	bool climbing = false;
	bool descending = false;
	int target = -1;
	int Ychange = 0;
	psydjfiEngine* engine;
	GameState* gState;
	shared_ptr<psydjfiTM> tm;
	playerObject* player;
	SimpleImage image;
	CoordinateMapping* mapping;
};

class mappingFlip : public CoordinateMapping {
public:
	mappingFlip(bool flipHorizontally, bool flipVertically, int masking = -1):horizontalFlip(flipHorizontally), verticalFlip(flipVertically), mask(masking){}
	bool mapCoordinates(double& x, double& y, const SimpleImage& image) override;
	bool getHorizontalFlip() { return horizontalFlip; }
	bool getVerticalFlip() { return verticalFlip; }
	void setHorizontalFlip(bool flipHorizontally) { horizontalFlip = flipHorizontally; }
	void setVerticalFlip(bool flipVertically) { verticalFlip = flipVertically; }
	void setMask(int masking) { mask = masking; }
private:
	bool horizontalFlip;
	bool verticalFlip;
	int mask;
};