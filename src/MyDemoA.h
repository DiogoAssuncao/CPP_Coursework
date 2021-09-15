#pragma once
#include "BaseEngine.h"

class MyDemoA : public BaseEngine {
public:
	MyDemoA();
	void virtSetupBackgroundBuffer() override;
	void virtMouseDown(int iButton, int iX, int iY);
};

