#include "header.h"
#include "psydjfiTM.h"

/*
Tile Values
0 - Empty Tile
1 - Wood Tile 
2 - Brick Tile
3 - Payphone / Objective - Must alwyas be 5 vertically stacked tiles - only draws top one
4 - Glitch / Trap
5 - Consumable - Although not drawn, very important for placement and save/loading
*/
 
//Construcor
psydjfiTM::psydjfiTM():TileManager(25,50,27,30) {}

//Draws Tiles that require it
void psydjfiTM::virtDrawTileAt( BaseEngine* pEngine, DrawingSurface* pSurface, int iMapX, int iMapY, int iStartPositionScreenX, int iStartPositionScreenY) const {

	int iMapValue = getMapValue(iMapX, iMapY);
	if (iMapValue == 1) {
		SimpleImage tile1 = ImageManager::loadImage("./images/wood1.png", true);
		tile1.renderImage(pSurface, 0, 0, iStartPositionScreenX, iStartPositionScreenY, getTileWidth(), getTileHeight() );
	}
	if (iMapValue == 2) {
		SimpleImage tile2 = ImageManager::loadImage("./images/brick2.png", true);
		tile2.renderImage(pSurface, 0, 0, iStartPositionScreenX, iStartPositionScreenY, getTileWidth(), getTileHeight());
	}
	else if (iMapValue == 3 && getMapValue(iMapX, iMapY - 1) != 3) {
		SimpleImage tile3 = ImageManager::loadImage("./images/payphone.png", true);
		tile3.renderImageWithMask(pSurface, 0, 0, iStartPositionScreenX, iStartPositionScreenY+5, tile3.getWidth(), tile3.getHeight());
	}
	else if (iMapValue == 4 && getMapValue(iMapX, iMapY - 1) != 3) {
		SimpleImage tile4 = ImageManager::loadImage("./images/glitch.png", true);
		tile4.renderImageWithMask(pSurface, 0, 0, iStartPositionScreenX, iStartPositionScreenY + 5, getTileWidth(), getTileHeight());
	}
}
