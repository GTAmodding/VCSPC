#include "StdAfx.h"

// Static variables
CPedData* CPedData::pPedData = new CPedData[140];

// Wrappers
WRAPPER void CPed::GiveWeapon(int WeaponType, int WeaponAmmo, bool bFlag) { WRAPARG(WeaponType); WRAPARG(WeaponAmmo); WRAPARG(bFlag); EAXJMP(0x5E6080); }
WRAPPER void CPed::GetBonePosition(RwV3d& vecOut, unsigned int nBone, bool bFlag) { WRAPARG(vecOut); WRAPARG(nBone); WRAPARG(bFlag); EAXJMP(0x5E4280); }
WRAPPER unsigned char CPed::GetWeaponSkill() { EAXJMP(0x5E6580); }

long double CPed::GetCrosshairSize()
{
	CWeaponInfo* pWeapon = CWeaponInfo::GetWeaponInfo(weaponSlots[m_bActiveWeapon].dwType, GetWeaponSkill());
	
	if ( !pWeapon->GetWeaponType() )
		return 0.0;

	return 0.5 / pWeapon->GetAccuracy();
}

void CPed::Remap()
{
	CPedData*	pTempPedData = &CPedData::pPedData[CPools::GetPedPool()->GetIndex(this)];
	CPedModelInfoVCS::SetPedColour(pTempPedData->m_color1, pTempPedData->m_color2, pTempPedData->m_color3, pTempPedData->m_color4);
	CPedModelInfoVCS::SetEditableMaterials(m_pRwObject);
}

CPed* CPedData::Constructor(CPed* pPed, WORD model)
{
	if ( model == 0xFFFF )
		model = pPed->ModelIndex();

	CPedData*	pTempData = &pPedData[CPools::GetPedPool()->GetIndex(pPed)];
	CPedModelInfoVCS* pModelInfo = (CPedModelInfoVCS*)CModelInfo::ms_modelInfoPtrs[model];
	if ( pModelInfo )
		pModelInfo->GetRandomPedColour(pTempData->m_color1, pTempData->m_color2, pTempData->m_color3, pTempData->m_color4);
	return pPed;
}