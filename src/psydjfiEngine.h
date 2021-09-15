#pragma once
#include "BaseEngine.h"
#include "psydjfiTM.h"
#include "psydjfiState.h"

using namespace std;

class psydjfiEngine : public BaseEngine{
public:
	psydjfiEngine();
	void virtSetupBackgroundBuffer();
	int virtInitialiseObjects();
	void virtMouseDown(int iButton, int iX, int iY);
	void virtDrawStringsOnTop();
	void virtKeyDown(int iKeyCode);
	void setState(psydjfiState* currentState);
	psydjfiState* getState();
	void virtMouseMoved(int iX, int iY);
	void virtPostDraw();
	void virtMainLoopDoBeforeUpdate();
	void copyAllBackgroundBuffer();
	bool virtCheckForUpdateTime();
	void destroyOldObjects();
	shared_ptr<psydjfiTM> getTileManager();
	int storeObjectInArray(DisplayableObject* pObject);
	int storeObjectInArray(DisplayableObject* pObject, int position);
	bool removeDisplayableObject(DisplayableObject* pObject);
	void exit();
	int getSizeObjectArray();
	void playMusic(const char* url);
	void loopMusic();
	void stopMusic();
	void toggleMusic();
private:
	void resizeArray();
	int lastObject = -1, arraySize;
	psydjfiState* currentState;
	shared_ptr<psydjfiTM> tm;
	string currentSong = "";
	bool musicStopped = false;
	bool startedMusic = false;
};

