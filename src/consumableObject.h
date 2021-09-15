#pragma once
#include "DisplayableObject.h"
#include "psydjfiEngine.h"

class playerObject;

class consumableObject : public DisplayableObject {
public:
	consumableObject::consumableObject(int centerStartX, int centerStartY, int distPerMove, int speed, int pathSize, bool stopped, psydjfiEngine* engine, std::string URL);
	consumableObject::consumableObject(psydjfiEngine* engine);
	void virtDraw();
	void virtDoUpdate(int iCurrentTime);
	void toggleMovement();
	bool virtIsPositionWithinObject(int iX, int iY);
	virtual void behaviour();
	int getTileX();
	int getTileY();
	bool getEmpty();
	void setTileX(int X);
	void setTileY(int Y);
	int getValue();
protected:
	psydjfiEngine* engine;
	int value;
private:
	SimpleImage image;
	bool up, stopMov;
	int originY, stopTimer, iSpeed, size, distXmove;
	int tileX, tileY;
	bool empty;
};
      
class redPill : public consumableObject {
public:
	redPill::redPill(int tileX, int tileY, psydjfiEngine* engine, playerObject* player);
	void behaviour();
private:
	playerObject* playerO;
};

class bluePill : public consumableObject {
public:
	bluePill::bluePill(int tileX, int tileY, psydjfiEngine* engine, playerObject* player);
	void behaviour();
private:
	playerObject* playerO;
};

class greenPill : public consumableObject {
public:
	greenPill::greenPill(int tileX, int tileY, psydjfiEngine* engine, playerObject* player);
	void behaviour();
private:
	playerObject* playerO;
};
