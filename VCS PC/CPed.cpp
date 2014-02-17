#include "StdAfx.h"

// Static variables
CPedData* CPedData::pPedData = new CPedData[140];

void CPed::GiveWeapon(int WeaponType, int WeaponAmmo)
{
	DWORD dwFunc = (DWORD)FUNC_CPed__GiveWeapon;
	_asm
	{
		push	1
		mov		eax, WeaponAmmo
		push	eax
		mov		eax, WeaponType
		push	eax
		mov		ecx, this
		call	dwFunc
	}
}

void CPed::GetBonePosition(CVector* result, int boneID, bool bUnk)
{
	DWORD dwFunc = (DWORD)FUNC_CPed__GetBonePosition;
	_asm
	{
		movzx	eax, bUnk
		push	eax
		mov		eax, boneID
		push	eax
		mov		eax, result
		push	eax
		mov		ecx, this
		call	dwFunc
	}
}

BYTE CPed::GetWeaponSkill()
{
	DWORD	dwFunc = (DWORD)FUNC_CPed_GetWeaponSkill;
	BYTE	bResult;
	_asm
	{
		mov		ecx, this
		call	dwFunc
		mov		bResult, al
	}
	return bResult;
}

long double CPed::GetCrosshairSize()
{
	CWeaponInfo* weapPtr;
	eWeaponType weaponID;
	BYTE unkData;
	long double crosshairSize;

	weaponID = weaponSlots[m_bActiveWeapon].dwType;
	unkData = GetWeaponSkill();
	weapPtr = CWeaponInfo::GetWeaponInfo(weaponID, unkData);
	crosshairSize = 0.5 / weapPtr->GetAccuracy();
	
	if ( !weapPtr->GetWeaponType() )
		crosshairSize = 0.0;

	return crosshairSize;
}

void CPed::Remap()
{
	CPedData*	pTempPedData = &CPedData::pPedData[CPools::GetPedPool()->GetIndexOnly(this)];
	CPedModelInfoVCS::SetPedColour(pTempPedData->m_color1, pTempPedData->m_color2, pTempPedData->m_color3, pTempPedData->m_color4);
	CPedModelInfoVCS::SetEditableMaterials(m_pRwObject);
}

CPed* CPedData::Constructor(CPed* pPed, WORD model)
{
	if ( model == 0xFFFF )
		model = pPed->ModelIndex();

	CPedData*	pTempData = &pPedData[CPools::GetPedPool()->GetIndexOnly(pPed)];
	CPedModelInfoVCS* pModelInfo = (CPedModelInfoVCS*)CModelInfo::ms_modelInfoPtrs[model];
	if ( pModelInfo )
		pModelInfo->GetRandomPedColour(pTempData->m_color1, pTempData->m_color2, pTempData->m_color3, pTempData->m_color4);
	return pPed;
}