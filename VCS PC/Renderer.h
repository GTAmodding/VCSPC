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

	static int ms_nNumOpaqueObjects;
	static CEntity *ms_aOpaqueRenderList[1000];

	static void Init(void);

	static void RenderOneRoad(CEntity* pEntity);
	static void RenderFadingInEntities(void);
	static void RenderFadingInUnderwaterEntities(void);
	static void RenderRoads(void);
	static void RenderOneNonRoad(CEntity *entity);
	static void RenderEverythingBarRoads(void);

	static void AddEntityToOpaqueList(CEntity *e);
	static void SortOutVisibleEntities(void);
	static void RenderEverythingBarRoadsOpaque(void);
	static void RenderAllBuildingsOpaque(void);
	static void RenderAllBuildingsTransparent(void);
};
#endif