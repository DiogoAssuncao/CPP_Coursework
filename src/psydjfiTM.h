#pragma once
#include "TileManager.h"

class psydjfiTM : public TileManager {
public:
	psydjfiTM();
	virtual void virtDrawTileAt(BaseEngine* pEngine, DrawingSurface* pSurface, int iMapX, int iMapY, int iStartPositionScreenX, int iStartPositionScreenY) const override;
};

