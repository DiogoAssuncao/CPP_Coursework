#include "header.h"
#include "psydjfiEngine.h"
#include "ImageManager.h"
#include "consumableObject.h"
#include "MenuState.h"
#include "GameState.h"
#include "ScoreState.h"
#include "TutState.h"

#define INITIAL_OBJECT_ARRAY_SIZE 5

//Constructor
psydjfiEngine::psydjfiEngine():currentState(new	MenuState(this)), arraySize(INITIAL_OBJECT_ARRAY_SIZE)  {
	tm = make_shared<psydjfiTM>();
	currentSong = "./music/hiding-your-reality-by-kevin-macleod-from-filmmusic-io.wav";
}

//Calling State version
void psydjfiEngine::virtSetupBackgroundBuffer() {
	if (currentState) {
		currentState->setupBackground();
	}
}

//Calling State version
int psydjfiEngine::virtInitialiseObjects(){
	drawableObjectsChanged();
	destroyOldObjects();
	createObjectArray(arraySize);
	if (currentState) {
		currentState->initializeObjects();
	}
	return 0;
}

//Calling State version
void psydjfiEngine::virtMouseDown(int iButton, int iX, int iY) {
	if (currentState) {
		currentState->mouseDown(iButton, iX, iY); 
	}
}

//Calling State version
void psydjfiEngine::virtDrawStringsOnTop() {
	if (currentState) {
		currentState->drawStringsOnTop();
	}
}

//Calling State version
void psydjfiEngine::virtKeyDown(int iKeyCode) {
	if (currentState) {
		currentState->keyDown(iKeyCode);
	}
}

//Calling State version
void psydjfiEngine::virtMouseMoved(int iX, int iY) {
	if (currentState) {
		currentState->mouseMoved(iX,iY);
	}
}

//caller responsible for deleting old state
void psydjfiEngine::setState(psydjfiState* newState) {
	currentState = newState; 
	currentState->initializeState();
}

//Calling State version
void psydjfiEngine::virtPostDraw() {
	if (currentState) {
		currentState->postDraw();
	}
}

//Calling State version
void psydjfiEngine::virtMainLoopDoBeforeUpdate() {
	if (currentState) {
		currentState->mainLoopDoBeforeUpdate();
	}
	loopMusic();
}

//Calling State version
void psydjfiEngine::copyAllBackgroundBuffer() {
	if (currentState) {
		currentState->copyBackgroundBuffer();
	}
}

//Calling State version
bool psydjfiEngine::virtCheckForUpdateTime() {
	if (currentState && m_iExitWithCode == -1) {
		return currentState->checkForUpdateTime();
	}
	return false;
}

//Getter tm
shared_ptr<psydjfiTM> psydjfiEngine::getTileManager() {
	return tm;
}

//Exit handling
void psydjfiEngine::exit() {
	if (getForegroundSurface()->getDrawPointsFilter()) {
		delete getForegroundSurface()->getDrawPointsFilter();
	}
	currentState = nullptr;
	stopMusic();
	setExitWithCode(0);
}

//Keeps the index of last object, and resizes the array if need be
int psydjfiEngine::storeObjectInArray(DisplayableObject* pObject) {
	if (lastObject == arraySize - 1) {
		resizeArray();
	}
	lastObject++;
	BaseEngine::storeObjectInArray(lastObject, pObject);
	return lastObject;
}

//ONLY USE WHEN REPLACING OBJECTS THAT HAVE ALREADY BEEN CREATED
//Ignores the last object index
int psydjfiEngine::storeObjectInArray(DisplayableObject* pObject, int position) {
	if (lastObject == arraySize - 1) {
		resizeArray();
	}
	BaseEngine::storeObjectInArray(position, pObject);
	return lastObject;
}

//Updates last object index, keeps array size consistent
bool psydjfiEngine::removeDisplayableObject(DisplayableObject* pObject) {
	if (pObject == getDisplayableObject(lastObject)) {
		lastObject--;
	}
	bool temp = BaseEngine::removeDisplayableObject(pObject);
	m_vecDisplayableObjects.resize(arraySize);
	return temp;
}

//Resizes object array
//Uses doubling strategy
void psydjfiEngine::resizeArray() {
	m_vecDisplayableObjects.resize(arraySize + 10);
	arraySize += 10;
}

//Updates last object index, calls base class function
void psydjfiEngine::destroyOldObjects() {
	lastObject = -1;
	BaseEngine::destroyOldObjects(true);
}

//Getter for state
psydjfiState* psydjfiEngine::getState() {
	return currentState;
}

//Return size of object array
int psydjfiEngine::getSizeObjectArray() {
	return m_vecDisplayableObjects.size();
}

//---------------------------------------------------------------------------------------SOUND-------------------------------------------------------------------------------------------------------

// Audio buffer to be played
static Uint8* audioBuffer = 0; 
// Remaining length of the sample
static Uint32 audioLen = 0; 

//Callback function for sound
void callbackFunction(void* userdata, Uint8* stream, int len) {
	if (audioLen == 0) {
		//Finished
		return;
	}
	len = (len > audioLen ? audioLen : len);
	//Copy from buffer to be played to stream buffer
	SDL_memcpy(stream, audioBuffer, len);
	//Update variables
	audioBuffer += len;
	audioLen -= len;
}

//Play music file where url is the path
void psydjfiEngine::playMusic(const char * url) {
	
	if (musicStopped || (url == currentSong && audioBuffer != 0)) {
		return;
	}
	currentSong = url;
	//Start SDL
	SDL_Init(SDL_INIT_AUDIO);
	//Wav params
	static Uint32 wavLength;
	static SDL_AudioSpec wavSpec;
	static Uint8* wavBuffer;
	//Load wav File
	if (SDL_LoadWAV(url, &wavSpec, &wavBuffer, &wavLength) == NULL) {
		cout << "Error Openning File\n";
		return;
	}
	//Set callbackFunciton
	wavSpec.callback = callbackFunction;
	wavSpec.userdata = NULL;
	//Set starting buffer and buffer lenght
	audioBuffer = wavBuffer;
	audioLen = wavLength;
	if (!startedMusic) {
		if (SDL_OpenAudio(&wavSpec, NULL) < 0) {
			cout << "Error Openning Audio\n";
			return;
		}
		startedMusic = true;
	}
	
	
	//Starts
	SDL_PauseAudio(0);
}

//Whenever the music ends Play again
void psydjfiEngine::loopMusic() {
	if (audioLen <= 0) {
		audioBuffer = 0;
		playMusic(currentSong.c_str());
	}
}

//Stops Music
void psydjfiEngine::stopMusic() {
	audioBuffer = 0;
	audioLen = 0;
	startedMusic = false;
	SDL_CloseAudio();
}

//Toggles Between on and off
void psydjfiEngine::toggleMusic() {
	if (musicStopped) {
		audioBuffer = 0;
		musicStopped = false;
		playMusic(currentSong.c_str());
	}
	else {
		stopMusic();
		musicStopped = true;
	}
}