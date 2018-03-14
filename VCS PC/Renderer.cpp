#include "StdAfx.h"
#include "Renderer.h"
#include "Scene.h"
#include "ModelInfo.h"

CEntity** CRenderer::ms_aVisibleEntityPtrs = (CEntity**)0xB75898;
int& CRenderer::ms_nNoOfVisibleEntities = *(int*)0xB76844;

WRAPPER void CRenderer::RenderOneRoad(CEntity* pEntity) { WRAPARG(pEntity); EAXJMP(0x553230); }

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

static StaticPatcher	Patcher([](){
						Memory::InjectHook(0x553A10, CRenderer::RenderRoads);
									});