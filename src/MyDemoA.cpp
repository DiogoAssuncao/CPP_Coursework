#include "header.h"
#include "MyDemoA.h"

MyDemoA::MyDemoA() {

}

void MyDemoA::virtSetupBackgroundBuffer()
{
	fillBackground(0x000000);
	for (int iX = 0;iX <= getWindowWidth();iX++) {
		for (int iY = 0; iY <= getWindowHeight(); iY++) {
			switch (rand() % 600) {
			case 0: setBackgroundPixel(iX, iY, 0xff0000); break;
			case 1: setBackgroundPixel(iX, iY, 0x00ff00); break;
			case 2: setBackgroundPixel(iX, iY, 0x0000ff); break;
			case 3: setBackgroundPixel(iX, iY, 0xffff00); break;
			case 4: setBackgroundPixel(iX, iY, 0x00ffff); break;
			case 5: setBackgroundPixel(iX, iY, 0xff00ff); break;
			}
		}
	}
}


void MyDemoA::virtMouseDown(int iButton, int iX, int iY)
{
	printf("Mouse clicked at %d %d\n", iX, iY);
	
	if (iButton == SDL_BUTTON_LEFT) {
		lockBackgroundForDrawing();
		drawBackgroundRectangle(iX - 10, iY - 10, iX + 10, iY + 10,0xff0000);
		unlockBackgroundForDrawing();
		redrawDisplay();
	}
}
