#ifndef __RENDERER
#define __RENDERER
#include "General.h"

class CRenderer
{
public:
	static CVector &ms_vecCameraPosition;
	static CEntity **ms_aVisibleEntityPtrs;
	static CEntity **ms_aVisibleLodPtrs;
	static int &ms_nNoOfVisibleEntities;
	static int &ms_nNoOfVisibleLods;

	static void RenderOneRoad(CEntity* pEntity);
	static void RenderFadingInEntities(void);
	static void RenderFadingInUnderwaterEntities(void);
	static void RenderRoads(void);
	static void RenderOneNonRoad(CEntity *entity);
	static void RenderEverythingBarRoads(void);
};
#endif