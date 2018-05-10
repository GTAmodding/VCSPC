#include "StdAfx.h"
#include "Renderer.h"
#include "VisibilityPlugins.h"
#include "Scene.h"
#include "ModelInfo.h"
#include "Vehicle.h"
#include "Ped.h"
#include "Camera.h"

CVector &CRenderer::ms_vecCameraPosition = *(CVector*)0xB76870;
CEntity **CRenderer::ms_aVisibleEntityPtrs = (CEntity**)0xB75898;	// [1000]
CEntity **CRenderer::ms_aVisibleLodPtrs = (CEntity**)0xB748F8;		// [1000]
int &CRenderer::ms_nNoOfVisibleEntities = *(int*)0xB76844;
int &CRenderer::ms_nNoOfVisibleLods = *(int*)0xB76840;

int CRenderer::ms_nNumOpaqueObjects;
CEntity *CRenderer::ms_aOpaqueRenderList[1000];

void
CRenderer::RenderOneRoad(CEntity* pEntity)
{
	// SA does something when VisionFX is on, but we don't do that
	pEntity->Render();
}

void
CRenderer::RenderOneNonRoad(CEntity *entity)
{
	// peds in vehicle - rendered by vehicle, not here
	if(entity->nType == ENTITY_TYPE_PED && ((CPed*)entity)->pedState == 50)
		return;

	entity->SetupLighting();

	if(entity->nType == ENTITY_TYPE_VEHICLE){
		CVehicle *v = (CVehicle*)entity;
		CVisibilityPlugins::SetupVehicleVariables((RpClump*)v->m_pRwObject);
		CVisibilityPlugins::InitAlphaAtomicList();
		v->RenderDriverAndPassengers();
		v->SetupRender();
	}else if(!entity->bBackfaceCulled)
		RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLNONE);

	// no vision FX, just render
	entity->Render();

	if(entity->nType == ENTITY_TYPE_VEHICLE){
		CVehicle *v = (CVehicle*)entity;
		v->bImBeingRendered = 1;
		CVisibilityPlugins::RenderAlphaAtomics();
		v->bImBeingRendered = 0;
		v->ResetAfterRender();
	}else if(!entity->bBackfaceCulled)
		RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLBACK);

	entity->RemoveLighting();
}

void
CRenderer::RenderFadingInEntities(void)
{
	RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLBACK);
	DeActivateDirectional();
	SetAmbientColours();
	CVisibilityPlugins::RenderFadingEntities();
}

void
CRenderer::RenderFadingInUnderwaterEntities(void)
{
	DeActivateDirectional();
	SetAmbientColours();
	CVisibilityPlugins::RenderFadingUnderwaterEntities();
}

void
CRenderer::RenderRoads(void)
{
	RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLBACK);

	DeActivateDirectional();
	SetAmbientColours();

	for(int i = 0; i < ms_nNoOfVisibleEntities; i++){
		CEntity *pEntity = ms_aVisibleEntityPtrs[i];
		CAtomicModelInfo *pModelInfo = (CAtomicModelInfo*)CModelInfo::GetModelInfo(pEntity->m_nModelIndex);
		if(pEntity->nType == ENTITY_TYPE_BUILDING && pModelInfo->GetWetRoadReflection())
			RenderOneRoad(pEntity);
	}
}

void
CRenderer::RenderEverythingBarRoads(void)
{
	assert(0 && "don't use this");

	RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLBACK);
	if(CGame::currArea == 0)
		RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, (void*)140);
	for(int i = 0; i < ms_nNoOfVisibleEntities; i++){
		CEntity *e = ms_aVisibleEntityPtrs[i];
		int type = e->nType;
		if(type == ENTITY_TYPE_BUILDING && ((CAtomicModelInfo*)CModelInfo::GetModelInfo(e->m_nModelIndex))->GetWetRoadReflection())
			;	// do nothing, it's a road
		else if(type == ENTITY_TYPE_VEHICLE){
			CVehicle *veh = (CVehicle*)e;
			if(veh->GetClass() == VEHICLE_BOAT){
				int cammode = TheCamera.Cams[TheCamera.ActiveCam].Mode;
				if(cammode == MODE_WHEELCAM ||
				   cammode == MODE_1STPERSON && TheCamera.GetLookDirection() != 3 && TheCamera.GetLookDirection() != 0 ||
				   CVisibilityPlugins::GetClumpAlpha((RpClump*)e->m_pRwObject) != 0xFF)
					goto sorted;
			}else if(!veh->bTouchingWater)
				goto sorted;
			float dist = (ms_vecCameraPosition - e->GetCoords()).Magnitude();
			if(CVisibilityPlugins::InsertEntityIntoUnderwaterList(e, dist) == 0)
				RenderOneNonRoad(e);
		}else if(type != ENTITY_TYPE_PED || CVisibilityPlugins::GetClumpAlpha((RpClump*)e->m_pRwObject) == 0xFF)
			RenderOneNonRoad(e);
		else{
sorted:
			// this is for fading peds and all vehicles that don't touch the water
			float dist = (ms_vecCameraPosition - e->GetCoords()).Magnitude();
			if(CVisibilityPlugins::InsertEntityIntoSortedList(e, dist) == 0)
				RenderOneNonRoad(e);
		}
	}
	// SA does something with the camera's z values here but that has no effect on d3d
	for(int i = 0; i < ms_nNoOfVisibleLods; i++)
		RenderOneNonRoad(ms_aVisibleLodPtrs[i]);
}

void
CRenderer::AddEntityToOpaqueList(CEntity *e)
{
	ms_aOpaqueRenderList[ms_nNumOpaqueObjects++] = e;
}

// Put transparent peds and all vehicles into the correct sorted lists, all other objects into the opaque list
void
CRenderer::SortOutVisibleEntities(void)
{
	ms_nNumOpaqueObjects = 0;
	for(int i = 0; i < ms_nNoOfVisibleEntities; i++){
		CEntity *e = ms_aVisibleEntityPtrs[i];
		int type = e->nType;
		if(type == ENTITY_TYPE_PED && CVisibilityPlugins::GetClumpAlpha((RpClump*)e->m_pRwObject) != 0xFF){
sorted:
			float dist = (ms_vecCameraPosition - e->GetCoords()).Magnitude();
			if(CVisibilityPlugins::InsertEntityIntoSortedList(e, dist) == 0)
				AddEntityToOpaqueList(e);
		}else if(type == ENTITY_TYPE_VEHICLE){
			CVehicle *veh = (CVehicle*)e;
			if(veh->GetClass() == VEHICLE_BOAT){
				int cammode = TheCamera.Cams[TheCamera.ActiveCam].Mode;
				if(cammode == MODE_WHEELCAM ||
				   cammode == MODE_1STPERSON && TheCamera.GetLookDirection() != 3 && TheCamera.GetLookDirection() != 0 ||
				   CVisibilityPlugins::GetClumpAlpha((RpClump*)e->m_pRwObject) != 0xFF)
					goto sorted;
			}else if(!veh->bTouchingWater)
				goto sorted;
			float dist = (ms_vecCameraPosition - e->GetCoords()).Magnitude();
			if(CVisibilityPlugins::InsertEntityIntoUnderwaterList(e, dist) == 0)
				AddEntityToOpaqueList(e);
		}else
			AddEntityToOpaqueList(e);
	}
}

void
CRenderer::RenderEverythingBarRoadsOpaque(void)
{
	RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLBACK);
	if(CGame::currArea == 0)
		RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, (void*)140);

	for(int i = 0; i < ms_nNumOpaqueObjects; i++){
		CEntity *e = ms_aOpaqueRenderList[i];
		if(e->nType != ENTITY_TYPE_BUILDING || !((CAtomicModelInfo*)CModelInfo::GetModelInfo(e->m_nModelIndex))->GetWetRoadReflection())
			RenderOneNonRoad(e);
	}
	for(int i = 0; i < ms_nNoOfVisibleLods; i++)
		RenderOneNonRoad(ms_aVisibleLodPtrs[i]);
}

void
CRenderer::RenderAllBuildingsOpaque(void)
{
	RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLBACK);
	if(CGame::currArea == 0)
		RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, (void*)140);
	DeActivateDirectional();
	SetAmbientColours();

	for(int i = 0; i < ms_nNumOpaqueObjects; i++)
		if(ms_aOpaqueRenderList[i]->nType == ENTITY_TYPE_BUILDING)
			RenderOneNonRoad(ms_aOpaqueRenderList[i]);
	for(int i = 0; i < ms_nNoOfVisibleLods; i++)
		RenderOneNonRoad(ms_aVisibleLodPtrs[i]);
}

void
CRenderer::RenderAllBuildingsTransparent(void)
{
	RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLBACK);
	CVisibilityPlugins::RenderAllFadingBuildings();
}

static StaticPatcher	Patcher([](){
	// make sure the game barfs in case the original functions are called
	Memory::Patch<uint8>(0x553A10, 0xCC);	// disable RenderRoads
	Memory::Patch<uint8>(0x553AA0, 0xCC);	// disable RenderEverythingBarRoads
	Memory::Patch<uint8>(0x553220, 0xCC);	// disable RenderFadingInUnderwaterEntities
	Memory::Patch<uint8>(0x5531E0, 0xCC);	// disable RenderFadingInEntities

//	Memory::InjectHook(0x553A10, CRenderer::RenderRoads, PATCH_JUMP);
//	Memory::InjectHook(0x5531E0, CRenderer::RenderFadingInEntities, PATCH_JUMP);
//	Memory::InjectHook(0x553AA0, CRenderer::RenderEverythingBarRoads, PATCH_JUMP);
//						Memory::InjectHook(0x553A10, CRenderer::RenderRoads, PATCH_JUMP);
});