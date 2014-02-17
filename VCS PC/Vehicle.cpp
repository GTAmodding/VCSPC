#include "StdAfx.h"

static RwObject* GetCurrentAtomicObjectCB(RwObject* pObject, void* data)
{
	if ( RpAtomicGetFlags(pObject) & rpATOMICRENDER )
		*static_cast<RwObject**>(data) = pObject;
	return pObject;
}

static RpMaterial* SetCompAlphaCB(RpMaterial* pMaterial, void* data)
{
	pMaterial->color.alpha = reinterpret_cast<RwUInt8>(data);
	return pMaterial;
}

void CVehicle::SetComponentAtomicAlpha(RpAtomic* pAtomic, int nAlpha)
{
	RpGeometry*	pGeometry = RpAtomicGetGeometry(pAtomic);
	pGeometry->flags |= rpGEOMETRYMODULATEMATERIALCOLOR;

	RpGeometryForAllMaterials(pGeometry, SetCompAlphaCB, reinterpret_cast<void*>(nAlpha));
}

void CHeli::ProcessRotorsAlpha()
{
	double		dRotorsSpeed, dMovingRotorSpeed;

	if ( m_fRotorSpeed > 0.0 )
		dRotorsSpeed = min(1.7 * (1.0/0.22) * m_fRotorSpeed, 1.5);
	else
		dRotorsSpeed = 0.0;

	dMovingRotorSpeed = dRotorsSpeed - 0.4;
	if ( dMovingRotorSpeed < 0.0 )
		dMovingRotorSpeed = 0.0;

	int			nStaticRotorAlpha = min((1.5-dRotorsSpeed) * 255.0, 255);
	int			nMovingRotorAlpha = min(dMovingRotorSpeed * 150.0, 150);

	if ( m_pCarNode[11] )
	{
		RpAtomic*	pOutAtomic = nullptr;
		RwFrameForAllObjects(m_pCarNode[11], GetCurrentAtomicObjectCB, &pOutAtomic);
		if ( pOutAtomic )
			SetComponentAtomicAlpha(pOutAtomic, nStaticRotorAlpha);
	}

	if ( m_pCarNode[13] )
	{
		RpAtomic*	pOutAtomic = nullptr;
		RwFrameForAllObjects(m_pCarNode[13], GetCurrentAtomicObjectCB, &pOutAtomic);
		if ( pOutAtomic )
			SetComponentAtomicAlpha(pOutAtomic, nStaticRotorAlpha);
	}

	if ( m_pCarNode[12] )
	{
		RpAtomic*	pOutAtomic = nullptr;
		RwFrameForAllObjects(m_pCarNode[12], GetCurrentAtomicObjectCB, &pOutAtomic);
		if ( pOutAtomic )
			SetComponentAtomicAlpha(pOutAtomic, nMovingRotorAlpha);
	}

	if ( m_pCarNode[14] )
	{
		RpAtomic*	pOutAtomic = nullptr;
		RwFrameForAllObjects(m_pCarNode[14], GetCurrentAtomicObjectCB, &pOutAtomic);
		if ( pOutAtomic )
			SetComponentAtomicAlpha(pOutAtomic, nMovingRotorAlpha);
	}
}