#include "StdAfx.h"
#include "Ped.h"

#include "ModelInfo.h"
#include "Pools.h"
#include "PcSave.h"
#include "RealTimeShadowMgr.h"
#include "Rs.h"
#include "VisibilityPlugins.h"

// Wrappers
WRAPPER void CPed::GiveWeapon(int WeaponType, int WeaponAmmo, bool bFlag) { WRAPARG(WeaponType); WRAPARG(WeaponAmmo); WRAPARG(bFlag); EAXJMP(0x5E6080); }
WRAPPER void CPed::GetBonePosition(RwV3d& vecOut, unsigned int nBone, bool bFlag) { WRAPARG(vecOut); WRAPARG(nBone); WRAPARG(bFlag); EAXJMP(0x5E4280); }
WRAPPER unsigned char CPed::GetWeaponSkill() { EAXJMP(0x5E6580); }
WRAPPER void CPed::SetCharCreatedBy(unsigned char bBy) { WRAPARG(bBy); EAXJMP(0x5E47E0); }
WRAPPER void CPed::SetCurrentWeapon(int nSlot) { WRAPARG(nSlot); EAXJMP(0x5E61F0); }
WRAPPER void CPed::ResetGunFlashAlpha() { EAXJMP(0x5DF4E0); }
WRAPPER void CPed::FindWeaponLockOnTarget() { EAXJMP(0x60DC50); }

WRAPPER CPed* FindPlayerPed(int nIndex) { EAXJMP(0x56E210); }

short&			CPed::m_sGunFlashBlendStart = *(short*)0x8D1370;

CPedEx* CPed::GetEx()
{
	return CPools::GetPedPoolAux()->GetAtPointer(this);
}

long double CPed::GetCrosshairSize()
{
	CWeaponInfo* pWeapon = CWeaponInfo::GetWeaponInfo(weaponSlots[m_bActiveWeapon].m_eWeaponType, GetWeaponSkill());
	
	if ( !pWeapon->GetWeaponType() )
		return 0.0;

	return 0.5 / pWeapon->GetAccuracy();
}

void CPed::Remap()
{
	CPedEx*		pTempPedData = GetEx();
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

void CPed::RenderForShadow(RpClump* pClump, bool bRenderWeapon)
{
	RpClumpForAllAtomics(pClump, ShadowCameraRenderCB, nullptr);

	if ( bRenderWeapon )
	{
		RenderWeapon(false, true);

		// Render jetpack
		auto*	pJetPackTask = pPedIntelligence->GetTaskJetPack();
		if ( pJetPackTask )
			pJetPackTask->RenderJetPack(this);
	}
}

void CPed::RenderWeapon(bool bMuzzleFlash, bool bForShadow)
{
	if ( m_pWeaponObject )
	{
#ifndef NDEBUG
		D3DPERF_BeginEvent(D3DCOLOR_ARGB(0xFF, 0xFF, 0, 0), L"CPed::RenderWeapon");
#endif

		RpHAnimHierarchy*	pAnimHierarchy = GetAnimHierarchyFromSkinClump(reinterpret_cast<RpClump*>(m_pRwObject));
		bool				bHasParachute = weaponSlots[m_bActiveWeapon].m_eWeaponType == WEAPONTYPE_PARACHUTE;

		RwFrame*			pFrame = RpClumpGetFrame(reinterpret_cast<RpClump*>(m_pWeaponObject));
		*RwFrameGetMatrix(pFrame) = RpHAnimHierarchyGetMatrixArray(pAnimHierarchy)[RpHAnimIDGetIndex(pAnimHierarchy, bHasParachute ? 3 : 24)];

		if ( bHasParachute )
		{
			const RwV3d		vecParachuteTranslation = { 0.1f, -0.15f, 0.0f };
			const RwV3d		vecParachuteRotation = { 0.0f, 1.0f, 0.0f };
			RwMatrixTranslate(RwFrameGetMatrix(pFrame), &vecParachuteTranslation, rwCOMBINEPRECONCAT);
			RwMatrixRotate(RwFrameGetMatrix(pFrame), &vecParachuteRotation, 90.0f, rwCOMBINEPRECONCAT);
		}

		RwFrameUpdateObjects(pFrame);
		if ( bForShadow )
			RpClumpForAllAtomics(reinterpret_cast<RpClump*>(m_pWeaponObject), ShadowCameraRenderCB, nullptr);
		else if ( !bMuzzleFlash )
			RpClumpRender(reinterpret_cast<RpClump*>(m_pWeaponObject));
		else if ( m_pMuzzleFlashFrame )
		{
			RpAtomic*	pWeapon = reinterpret_cast<RpAtomic*>(GetFirstObject(m_pMuzzleFlashFrame));
			SetGunFlashAlpha(false);
			if ( RpAtomicGetFlags(pWeapon) & rpATOMICRENDER  )
				RpAtomicRender(pWeapon);
		}

		// Dual weapons
		if ( CWeaponInfo::GetWeaponInfo(weaponSlots[m_bActiveWeapon].m_eWeaponType, GetWeaponSkill())->hexFlags >> 11 & 1 )
		{
			*RwFrameGetMatrix(pFrame) = RpHAnimHierarchyGetMatrixArray(pAnimHierarchy)[RpHAnimIDGetIndex(pAnimHierarchy, 34)];				

			const RwV3d		vecParachuteRotation = { 1.0f, 0.0f, 0.0f };
			const RwV3d		vecParachuteTranslation  = { 0.04f, -0.05f, 0.0f };
			RwMatrixRotate(RwFrameGetMatrix(pFrame), &vecParachuteRotation, 180.0f, rwCOMBINEPRECONCAT);
			RwMatrixTranslate(RwFrameGetMatrix(pFrame), &vecParachuteTranslation, rwCOMBINEPRECONCAT);

			RwFrameUpdateObjects(pFrame);
			if ( bForShadow )
				RpClumpForAllAtomics(reinterpret_cast<RpClump*>(m_pWeaponObject), ShadowCameraRenderCB, nullptr);
			else if ( !bMuzzleFlash )
				RpClumpRender(reinterpret_cast<RpClump*>(m_pWeaponObject));
			else if ( m_pMuzzleFlashFrame )
			{
				RpAtomic*	pWeapon = reinterpret_cast<RpAtomic*>(GetFirstObject(m_pMuzzleFlashFrame));
				SetGunFlashAlpha(true);
				if ( RpAtomicGetFlags(pWeapon) & rpATOMICRENDER  )
					RpAtomicRender(pWeapon);
			}
		}
		if ( bMuzzleFlash )
			ResetGunFlashAlpha();

#ifndef NDEBUG
		D3DPERF_EndEvent();
#endif
	}
}

void CPed::SetGunFlashAlpha(bool bSecondWeapon)
{
	short	nBlend = bSecondWeapon ? m_nGunFlashBlend_Secondary : m_nGunFlashBlend;

	if ( m_pMuzzleFlashFrame != nullptr )
	{
		RpAtomic*	pWeapon = reinterpret_cast<RpAtomic*>(GetFirstObject(m_pMuzzleFlashFrame));
		if ( pWeapon != nullptr )
		{
			if ( nBlend > 0 )
			{
				CVehicle::SetComponentAtomicAlpha(pWeapon, Min(350 * nBlend / m_sGunFlashBlendStart, 255));
				RpAtomicSetFlags(pWeapon, rpATOMICRENDER);		
			}
			else
				RpAtomicSetFlags(pWeapon, 0);
		}

		if ( bSecondWeapon )
		{
			if ( m_nGunFlashBlend_Secondary == 0 )
				m_nGunFlashBlend_Secondary = -1;
		}
		else
		{
			if ( m_nGunFlashBlend == 0 )
				m_nGunFlashBlend = -1;
		}
	}
}


CPed* CPedEx::Initialise(CPed* pPed, short model)
{
	if ( model == -1 )
		model = pPed->GetModelIndex();

	CPedEx*	pTempData = pPed->GetEx();
	CPedModelInfoVCS* pModelInfo = (CPedModelInfoVCS*)CModelInfo::ms_modelInfoPtrs[model];
	if ( pModelInfo )
		pModelInfo->GetRandomPedColour(pTempData->m_color1, pTempData->m_color2, pTempData->m_color3, pTempData->m_color4);
	return pPed;
}

void RenderWeapon(CPed* pPed)
{
	pPed->RenderWeapon(false, false);
	CVisibilityPlugins::ms_weaponPedsForPC.Insert(pPed);
}

static StaticPatcher	Patcher([](){ 
						using namespace Memory;

						// Fixed muzzleflash not showing from last bullet
						Nop(0x61ECE4, 2);

						// Weapons rendering
						InjectHook(0x5E7859, RenderWeapon);
						InjectHook(0x732F30, &CVisibilityPlugins::RenderWeaponPedsForPC, PATCH_JUMP);
									});