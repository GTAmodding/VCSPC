#ifndef __RENDERER
#define __RENDERER
#include "General.h"

class CRenderer
{
public:
	static CEntity** ms_aVisibleEntityPtrs;
	static int& ms_nNoOfVisibleEntities;

	static void RenderRoads();
	static void RenderOneRoad(CEntity* pEntity);
};
#endif