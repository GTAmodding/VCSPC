#include "StdAfx.h"
#include "Vehicle.h"

#include "Antennas.h"
#include "Timer.h"

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

#include "Font.h"

static float		fCurrentY;

static RwFrame* DisplayFrameInfo(RwFrame* frame, void* pData)
{
	char			Temp[256];

	RwMatrix*		pMat = RwFrameGetMatrix(frame);
	int				nFlags = rwMatrixGetFlags(pMat);
	sprintf(Temp, "%g %g %g %X", pMat->pos.x, pMat->pos.y, pMat->pos.z, nFlags);
	//rwMatrixSetFlags(pMat, 0);

	CFont::SetProportional(true);
	CFont::SetEdge(1);
	CFont::SetFontStyle(FONT_Eurostile);
	CFont::SetScale(_width(0.3f), _height(0.5f));
	CFont::SetOrientation(ALIGN_Left);
	CFont::SetColor(CRGBA(255, 255, 255, 255));
	CFont::SetDropColor(CRGBA(0, 0, 0, 255));
	CFont::PrintString(_xleft(10.0f), _y(fCurrentY), Temp);

	fCurrentY += 10.0f;

	RwFrameForAllChildren(frame, DisplayFrameInfo, pData);
	return frame;
}

void CAutomobile::DebugWheelDisplay()
{
	fCurrentY = 10.0f;

	DisplayFrameInfo(m_pCarNode[2], nullptr);
	DisplayFrameInfo(m_pCarNode[5], nullptr);
	DisplayFrameInfo(m_pCarNode[4], nullptr);
	DisplayFrameInfo(m_pCarNode[7], nullptr);
}

void CHeli::Render()
{
	double		dRotorsSpeed, dMovingRotorSpeed;

	m_nTimeTillWeNeedThisCar = CTimer::m_snTimeInMilliseconds + 3000;

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

	CEntity::Render();
}

CColModel* CAutomobile::RenderAntennas()
{
	if ( m_nModelIndex == VT_RCBANDIT )
	{
		CMatrix*	pCoords = GetMatrix();
		if ( pCoords )
			CAntennas::RegisterOne(reinterpret_cast<unsigned int>(this), *pCoords->GetAt(), *pCoords * CVector(0.13f, -0.55f, 0.5f), 1.0f, 0.9f);
	}
	if ( m_nModelIndex == VT_WILLARD )
	{
		CMatrix*	pCoords = GetMatrix();
		if ( pCoords )
			CAntennas::RegisterOne(reinterpret_cast<unsigned int>(this), *pCoords->GetAt(), *pCoords * CVector(-1.0f, 0.84f, 0.375f), 1.1f, 0.93f);
	}
	/*else if ( m_nModelIndex == VT_STINGER )
	{
		CMatrix*	pCoords = GetMatrix();
		if ( pCoords )
			CAntennas::RegisterOne(reinterpret_cast<unsigned int>(this), *pCoords->GetAt(), *pCoords * CVector(-0.8f, -1.75f, 0.2f), 0.5f, 0.9f);
	}*/
	/*else if ( m_nModelIndex == VT_POLICEM )
	{
		CMatrix*	pCoords = GetMatrix();
		if ( pCoords )
		{
			CAntennas::RegisterOne(reinterpret_cast<unsigned int>(this), *pCoords->GetAt(), *pCoords * CVector(0.9f, -2.25f, 0.22f), 1.25f, 0.95f);
			CAntennas::RegisterOne(reinterpret_cast<unsigned int>(this) + 1, *pCoords->GetAt(), *pCoords * CVector(-0.9f, -2.25f, 0.22f), 1.25f, 0.95f);
			CAntennas::RegisterOne(reinterpret_cast<unsigned int>(this) + 2, (*pCoords->GetAt() - (*pCoords->GetUp() * 0.5f)).Normalize(), *pCoords * CVector(0.0f, -1.1f, 0.68f), 0.75f, 0.9f);
		}
	}*/

	return ((CColModel*(__thiscall*)(CEntity*))0x535300)(this);
}