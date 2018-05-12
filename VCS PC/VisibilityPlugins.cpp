#include "StdAfx.h"
#include "VisibilityPlugins.h"

#include "Ped.h"
#include "Scene.h"

// Atomic and Frame have 4 bytes each
struct VisibilityClumpData
{
	bool (*isVisible)(RpClump *clump);
	int32 alpha;
};

int32 &CVisibilityPlugins::ms_atomicPluginOffset = *(int32*)0x8D608C;
int32 &CVisibilityPlugins::ms_framePluginOffset = *(int32*)0x8D6090;
int32 &CVisibilityPlugins::ms_clumpPluginOffset = *(int32*)0x8D6094;

CLinkList<CVisibilityPlugins::AlphaObjectInfo>&		CVisibilityPlugins::m_alphaList = *(CLinkList<CVisibilityPlugins::AlphaObjectInfo>*)0xC88070;
CLinkList<CVisibilityPlugins::AlphaObjectInfo>&		CVisibilityPlugins::m_alphaBoatAtomicList = *(CLinkList<CVisibilityPlugins::AlphaObjectInfo>*)0xC880C8;
CLinkList<CVisibilityPlugins::AlphaObjectInfo>&		CVisibilityPlugins::m_alphaEntityList = *(CLinkList<CVisibilityPlugins::AlphaObjectInfo>*)0xC88120;
CLinkList<CVisibilityPlugins::AlphaObjectInfo>&		CVisibilityPlugins::m_alphaUnderwaterEntityList = *(CLinkList<CVisibilityPlugins::AlphaObjectInfo>*)0xC88178;
//CLinkList<CVisibilityPlugins::AlphaObjectInfo>&		CVisibilityPlugins::m_alphaReallyDrawLastList = *(CLinkList<CVisibilityPlugins::AlphaObjectInfo>*)0xC881D0;
CLinkList<CPed*>&									CVisibilityPlugins::ms_weaponPedsForPC = *(CLinkList<CPed*>*)0xC88224;

void
CVisibilityPlugins::InitAlphaAtomicList(void)
{
	m_alphaList.Clear();
}

bool
CVisibilityPlugins::InsertEntityIntoSortedList(CEntity *ent, float sort)
{
	// SA adds "grasshouse" models to the really last list here, unnecessary for VCSPC

	AlphaObjectInfo i;
	i.fCompareValue = sort;
	i.pObject = ent;
	i.callback = (VisibilityCallback)RenderEntity;
	if(ent->bUnderwater)
		return m_alphaUnderwaterEntityList.InsertSorted(i) != NULL;
	else
		return m_alphaEntityList.InsertSorted(i) != NULL;
}

bool
CVisibilityPlugins::InsertEntityIntoUnderwaterList(CEntity *ent, float sort)
{
	AlphaObjectInfo i;
	i.fCompareValue = sort;
	i.pObject = ent;
	i.callback = (VisibilityCallback)RenderEntity;
	return m_alphaUnderwaterEntityList.InsertSorted(i) != NULL;
}

WRAPPER void
CVisibilityPlugins::RenderEntity(CEntity *e, float dist)
{
	EAXJMP(0x732B40);
}

void CVisibilityPlugins::RenderAlphaAtomics()
{
	// TODO: What is this for? do we really want this?
	// actually not even used since it's only used with vehicles and the pipeline uses its own alpha ref with the dual pass
//	RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, 0);

	RenderOrderedList(m_alphaList);
}

void CVisibilityPlugins::RenderWeaponPedsForPC()
{
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, reinterpret_cast<void*>(TRUE));
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, FALSE);

	for ( auto it = ms_weaponPedsForPC.m_lnListHead.m_pNext; it != &ms_weaponPedsForPC.m_lnListTail; it = it->m_pNext )
	{
		it->V()->SetupLighting();
		it->V()->RenderWeapon(true, false);
		it->V()->RemoveLighting();
	}
}

void
CVisibilityPlugins::RenderBoatAlphaAtomics(void)
{
	RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLNONE);
	RenderOrderedList(m_alphaBoatAtomicList);
	RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLBACK);
}

void
CVisibilityPlugins::RenderFadingEntities(void)
{
	RenderOrderedList(m_alphaEntityList);
	RenderBoatAlphaAtomics();
}

void
CVisibilityPlugins::RenderFadingUnderwaterEntities(void)
{
	RenderOrderedList(m_alphaUnderwaterEntityList);
}

void
CVisibilityPlugins::RenderAllFadingBuildings(void)
{
	RenderOrderedListBuildings(m_alphaUnderwaterEntityList);
	RenderOrderedListBuildings(m_alphaEntityList);
}

// This is just used to render The Truth's weed "grasshouse"es
// unused in VCSPC
/*
void
CVisibilityPlugins::RenderReallyDrawLastObjects(void)
{
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLNONE);
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
	SetAmbientColours();
	DeActivateDirectional();

	RenderOrderedList(m_alphaReallyDrawLastList);

	RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)FALSE);
}
*/

WRAPPER void
CVisibilityPlugins::SetupVehicleVariables(RpClump *clump)
{
	EAXJMP(0x733160);
}

WRAPPER uint16 CVisibilityPlugins::GetAtomicId(RpAtomic *atomic) { WRAPARG(atomic); EAXJMP(0x732370); }

int32
CVisibilityPlugins::GetClumpAlpha(RpClump *clump)
{
	return RWPLUGINOFFSET(VisibilityClumpData, clump, ms_clumpPluginOffset)->alpha;
}
