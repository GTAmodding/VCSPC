#include "StdAfx.h"
#include "Renderer.h"
#include "VisibilityPlugins.h"
#include "Scene.h"
#include "ModelInfo.h"
#include "Vehicle.h"
#include "Ped.h"
#include "Camera.h"

CVector &CRenderer::ms_vecCameraPosition = *(CVector*)0xB76870;
CEntity **CRenderer::ms_aVisibleEntityPtrs = (CEntity**)0xB75898;
CEntity **CRenderer::ms_aVisibleLodPtrs = (CEntity**)0xB748F8;
int &CRenderer::ms_nNoOfVisibleEntities = *(int*)0xB76844;
int &CRenderer::ms_nNoOfVisibleLods = *(int*)0xB76840;

WRAPPER void CRenderer::RenderOneRoad(CEntity* pEntity) { WRAPARG(pEntity); EAXJMP(0x553230); }

void
CRenderer::RenderFadingInEntities(void)
{
	RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)1);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)1);
	RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)1);
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
			// SA does something when VisionFX is on, but we don't do that
			pEntity->Render();
	}
}

void
CRenderer::RenderOneNonRoad(CEntity *entity)
{
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
CRenderer::RenderEverythingBarRoads(void)
{
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
			float dist = (ms_vecCameraPosition - e->GetCoords()).Magnitude();
			if(CVisibilityPlugins::InsertEntityIntoSortedList(e, dist) == 0)
				RenderOneNonRoad(e);
		}
	}
	// SA does something with the camera's z values here but that has no effect on d3d
	for(int i = 0; i < ms_nNoOfVisibleLods; i++)
		RenderOneNonRoad(ms_aVisibleLodPtrs[i]);
}

#if 0
// FUCK THIS FIX, IT DOESN'T WORK AT ALL. WTF DANNY?
void CRenderer::RenderRoads()
{
	RwRenderStateSet(rwRENDERSTATEFOGENABLE, reinterpret_cast<void*>(TRUE));
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, reinterpret_cast<void*>(TRUE));

	DeActivateDirectional();
	SetAmbientColours(&AmbientLightColourForFrame);

	for (int i = 0; i < ms_nNoOfVisibleEntities; i++)
	{
		CEntity* pEntity = ms_aVisibleEntityPtrs[i];
		CBaseModelInfo* pModelInfo = CModelInfo::ms_modelInfoPtrs[pEntity->m_nModelIndex];

		if (pEntity->nType == 1 && pModelInfo->bWetRoadReflection) // Is building (TODO: Enums)
		{
			if (pEntity->bBackfaceCulled)
				RwRenderStateSet(rwRENDERSTATECULLMODE, reinterpret_cast<void*>(rwCULLMODECULLBACK));
			else
				RwRenderStateSet(rwRENDERSTATECULLMODE, reinterpret_cast<void*>(rwCULLMODECULLNONE));

			RenderOneRoad(pEntity);
		}
	}
}
#endif

static StaticPatcher	Patcher([](){
	Memory::InjectHook(0x553A10, CRenderer::RenderRoads, PATCH_JUMP);
	Memory::InjectHook(0x5531E0, CRenderer::RenderFadingInEntities, PATCH_JUMP);
	Memory::InjectHook(0x553AA0, CRenderer::RenderEverythingBarRoads, PATCH_JUMP);
//						Memory::InjectHook(0x553A10, CRenderer::RenderRoads, PATCH_JUMP);
});