#include "StdAfx.h"
#include "Ped.h"

#include "ModelInfo.h"
#include "Pools.h"
#include "PcSave.h"

// Wrappers
WRAPPER void CPed::GiveWeapon(int WeaponType, int WeaponAmmo, bool bFlag) { WRAPARG(WeaponType); WRAPARG(WeaponAmmo); WRAPARG(bFlag); EAXJMP(0x5E6080); }
WRAPPER void CPed::GetBonePosition(RwV3d& vecOut, unsigned int nBone, bool bFlag) { WRAPARG(vecOut); WRAPARG(nBone); WRAPARG(bFlag); EAXJMP(0x5E4280); }
WRAPPER unsigned char CPed::GetWeaponSkill() { EAXJMP(0x5E6580); }
WRAPPER void CPed::SetCharCreatedBy(unsigned char bBy) { WRAPARG(bBy); EAXJMP(0x5E47E0); }
WRAPPER void CPed::SetCurrentWeapon(int nSlot) { WRAPARG(nSlot); EAXJMP(0x5E61F0); }
WRAPPER void CPed::ResetGunFlashAlpha() { EAXJMP(0x5DF4E0); }

long double CPed::GetCrosshairSize()
{
	CWeaponInfo* pWeapon = CWeaponInfo::GetWeaponInfo(weaponSlots[m_bActiveWeapon].m_eWeaponType, GetWeaponSkill());
	
	if ( !pWeapon->GetWeaponType() )
		return 0.0;

	return 0.5 / pWeapon->GetAccuracy();
}

void CPed::Remap()
{
	CPedData*	pTempPedData = CPools::GetPedPoolAux()->GetAtPointer(this);//&CPedData::pPedData[CPools::GetPedPool()->GetIndex(this)];
	CPedModelInfoVCS::SetPedColour(pTempPedData->m_color1, pTempPedData->m_color2, pTempPedData->m_color3, pTempPedData->m_color4);
	CPedModelInfoVCS::SetEditableMaterials(reinterpret_cast<RpClump*>(m_pRwObject));
}

bool CPed::Save()
{
	CPedSaveStructure::Construct(this);
	return true;
}

bool CPed::Load()
{
	CPedSaveStructure::Extract(this);
	return true;
}


CPed* CPedData::Initialise(CPed* pPed, short model)
{
	if ( model == -1 )
		model = pPed->GetModelIndex();

	CPedData*	pTempData = CPools::GetPedPoolAux()->GetAtPointer(pPed);//&pPedData[CPools::GetPedPool()->GetIndex(pPed)];
	CPedModelInfoVCS* pModelInfo = (CPedModelInfoVCS*)CModelInfo::ms_modelInfoPtrs[model];
	if ( pModelInfo )
		pModelInfo->GetRandomPedColour(pTempData->m_color1, pTempData->m_color2, pTempData->m_color3, pTempData->m_color4);
	return pPed;
}