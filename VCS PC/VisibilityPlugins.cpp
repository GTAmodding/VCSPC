#include "StdAfx.h"
#include "VisibilityPlugins.h"

#include "Ped.h"

CLinkList<CVisibilityPlugins::AlphaObjectInfo>&		CVisibilityPlugins::m_alphaList = *(CLinkList<CVisibilityPlugins::AlphaObjectInfo>*)0xC88070;
CLinkList<CPed*>&									CVisibilityPlugins::ms_weaponPedsForPC = *(CLinkList<CPed*>*)0xC88224;

void CVisibilityPlugins::RenderAlphaAtomics()
{
	RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, 0);
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

WRAPPER uint16 CVisibilityPlugins::GetAtomicId(RpAtomic *atomic) { WRAPARG(atomic); EAXJMP(0x732370); }
