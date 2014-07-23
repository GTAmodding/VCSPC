#include "StdAfx.h"
#include "RealTimeShadowMgr.h"

#include "Shadows.h"
#include "Ped.h"
#include "World.h"
#include "Tasks.h"
#include "PedIntelligence.h"

WRAPPER RwCamera* CShadowCamera::Create(int nSize) { WRAPARG(nSize); EAXJMP(0x705B60); }
WRAPPER void CShadowCamera::Destroy() { EAXJMP(0x705400); }
WRAPPER void CShadowCamera::SetLight(RpLight* pLight) { WRAPARG(pLight); EAXJMP(0x705520); }
WRAPPER void CShadowCamera::MakeGradientRaster() { EAXJMP(0x705D20); }
WRAPPER RwCamera* CShadowCamera::SetCenter(RwV3d* pVector) { WRAPARG(pVector); EAXJMP(0x705590); }
WRAPPER RwCamera* CShadowCamera::Update(RpAtomic* pAtomic) { WRAPARG(pAtomic); EAXJMP(0x705C80); }
WRAPPER RwRaster* CShadowCamera::RasterResample(RwRaster* pRaster) { WRAPARG(pRaster); EAXJMP(0x706070); }
WRAPPER RwRaster* CShadowCamera::RasterBlur(RwRaster* pRaster, int nPasses) { WRAPARG(pRaster); WRAPARG(nPasses); EAXJMP(0x706170); }
WRAPPER RwRaster* CShadowCamera::RasterGradient(RwRaster* pRaster) { WRAPARG(pRaster); EAXJMP(0x706330); }
WRAPPER void CShadowCamera::InvertRaster() { EAXJMP(0x705660); }

WRAPPER void CRealTimeShadow::Destroy() { EAXJMP(0x705990); }
WRAPPER bool CRealTimeShadow::SetShadowedObject(CPhysical* pObject) { WRAPARG(pObject); EAXJMP(0x706520); }

CShadowCamera	CRealTimeShadowManager::m_BlurCamera_Player;
CShadowCamera	CRealTimeShadowManager::m_GradientCamera_Player;

static RpAtomic* ShadowCameraRenderCallback(RpAtomic* pAtomic, void* pData)
{
	UNREFERENCED_PARAMETER(pData);
	return AtomicDefaultRenderCallBack(pAtomic);
}

void CShadowCamera::ReInit()
{
	RwInt32 nRasterWidth = RwRasterGetWidth(RwCameraGetRaster(m_pCamera));
	RwInt32	nRasterHeight = RwRasterGetHeight(RwCameraGetRaster(m_pCamera));

	RwRasterDestroy(RwCameraGetRaster(m_pCamera));
	
	RwRaster*	pNewRaster = RwRasterCreate(nRasterWidth, nRasterHeight, 0, rwRASTERTYPECAMERATEXTURE);
	RwCameraSetRaster(m_pCamera, pNewRaster);
	RwTextureSetRaster(m_pTexture, pNewRaster);
}

RwCamera* CShadowCamera::Update(RpClump* pClump, CPed* pPed)
{
	RwRGBA	ClearColour = { 255, 255, 255, 0 };
	RwCameraClear(m_pCamera, &ClearColour, rwCAMERACLEARIMAGE|rwCAMERACLEARZ);

	if ( RwCameraBeginUpdate(m_pCamera ) )
	{
		RpGeometry*	pGeometry = RpAtomicGetGeometry(GetFirstAtomic(pClump));
		auto		geometryFlags = RpGeometryGetFlags(pGeometry);

		RpGeometrySetFlags(pGeometry, geometryFlags & ~(rpGEOMETRYTEXTURED|rpGEOMETRYPRELIT|
						rpGEOMETRYLIGHT|rpGEOMETRYMODULATEMATERIALCOLOR|rpGEOMETRYTEXTURED2));
		RpClumpForAllAtomics(pClump, ShadowCameraRenderCallback, nullptr);
		RpGeometrySetFlags(pGeometry, geometryFlags);

		if ( pPed )
		{
			if ( pPed->m_pWeaponObject )
			{
				RpHAnimHierarchy*	pAnimHierarchy = GetAnimHierarchyFromSkinClump(pClump);
				bool				bHasParachute = pPed->weaponSlots[pPed->m_bActiveWeapon].m_eWeaponType == WEAPONTYPE_PARACHUTE;

				RwFrame*			pFrame = RpClumpGetFrame(reinterpret_cast<RpClump*>(pPed->m_pWeaponObject));
				*RwFrameGetMatrix(pFrame) = RpHAnimHierarchyGetMatrixArray(pAnimHierarchy)[RpHAnimIDGetIndex(pAnimHierarchy, bHasParachute ? 3 : 24)];

				if ( bHasParachute )
				{
					const RwV3d		vecParachuteTranslation = { 0.1f, -0.15f, 0.0f };
					const RwV3d		vecParachuteRotation = { 0.0f, 1.0f, 0.0f };
					RwMatrixTranslate(RwFrameGetMatrix(pFrame), &vecParachuteTranslation, rwCOMBINEPRECONCAT);
					RwMatrixRotate(RwFrameGetMatrix(pFrame), &vecParachuteRotation, 90.0f, rwCOMBINEPRECONCAT);
				}

				RwFrameUpdateObjects(pFrame);
				AtomicDefaultRenderCallBack(GetFirstAtomic(reinterpret_cast<RpClump*>(pPed->m_pWeaponObject)));	

				// Dual weapons
				if ( CWeaponInfo::GetWeaponInfo(pPed->weaponSlots[pPed->m_bActiveWeapon].m_eWeaponType, pPed->GetWeaponSkill())->hexFlags >> 11 & 1 )
				{
					*RwFrameGetMatrix(pFrame) = RpHAnimHierarchyGetMatrixArray(pAnimHierarchy)[RpHAnimIDGetIndex(pAnimHierarchy, 34)];				

					const RwV3d		vecParachuteRotation = { 1.0f, 0.0f, 0.0f };
					const RwV3d		vecParachuteTranslation  = { 0.04f, -0.05f, 0.0f };
					RwMatrixRotate(RwFrameGetMatrix(pFrame), &vecParachuteRotation, 180.0f, rwCOMBINEPRECONCAT);
					RwMatrixTranslate(RwFrameGetMatrix(pFrame), &vecParachuteTranslation, rwCOMBINEPRECONCAT);

					RwFrameUpdateObjects(pFrame);
					AtomicDefaultRenderCallBack(GetFirstAtomic(reinterpret_cast<RpClump*>(pPed->m_pWeaponObject)));	
				}
			}

			// Render jetpack
			auto*	pJetPackTask = pPed->GetPedIntelligencePtr()->GetTaskJetPack();
			if ( pJetPackTask )
				pJetPackTask->RenderJetPack(pPed);
		}

		InvertRaster();
		RwCameraEndUpdate(m_pCamera);
	}
	return m_pCamera;
}

RwTexture* CRealTimeShadow::Update()
{
	if ( m_nRwObjectType == rpATOMIC )
		RwV3dTransformPoints(&m_BaseSphere.center, &m_BoundingSphere.center, 1, RwFrameGetMatrix(RpAtomicGetFrame(reinterpret_cast<RpAtomic*>(m_pEntity->m_pRwObject))));
	else if ( m_nRwObjectType == rpCLUMP )
		RwV3dTransformPoints(&m_BaseSphere.center, &m_BoundingSphere.center, 1, RwFrameGetMatrix(RpClumpGetFrame(reinterpret_cast<RpClump*>(m_pEntity->m_pRwObject))));

	m_Camera.SetCenter(&m_BaseSphere.center);
	if ( m_nRwObjectType == rpATOMIC )
		m_Camera.Update(reinterpret_cast<RpAtomic*>(m_pEntity->m_pRwObject));
	else if ( m_nRwObjectType == rpCLUMP )
		m_Camera.Update(reinterpret_cast<RpClump*>(m_pEntity->m_pRwObject), m_pEntity->nType == 3 ? static_cast<CPed*>(m_pEntity) : nullptr);

	RwRaster*	pRaster = RwCameraGetRaster(m_Camera.m_pCamera);

	if ( m_bDrawResample )
		pRaster = m_ResampledCamera.RasterResample(pRaster);

	if ( m_dwBlurPasses )
		pRaster = m_bUsePlayerHelperCams ? CRealTimeShadowManager::m_BlurCamera_Player.RasterBlur(pRaster, m_dwBlurPasses) : g_realTimeShadowMan.m_BlurCamera.RasterBlur(pRaster, m_dwBlurPasses);

	if ( m_bDrawGradient )
		pRaster = m_bUsePlayerHelperCams ? CRealTimeShadowManager::m_GradientCamera_Player.RasterGradient(pRaster) : g_realTimeShadowMan.m_GradientCamera.RasterGradient(pRaster);

	return m_bDrawResample ? m_ResampledCamera.m_pTexture : m_Camera.m_pTexture;
}

void CRealTimeShadow::Create(int nSize, int nSizeResampled, bool bResample, int nBlurPasses, bool bGradient, bool bUsePlayerCams)
{
	m_pLight = RpLightCreate(rpLIGHTDIRECTIONAL);

	if ( m_pLight )
	{
		const RwRGBAReal		LightColour = { 0.8f, 0.8f, 0.8f, 0.0f };
		RpLightSetColor(m_pLight, &LightColour);
		RpLightSetFrame(m_pLight, RwFrameCreate());

		if ( m_Camera.Create(nSize) )
		{
			m_bDrawResample = bResample;
			m_bDrawGradient = bGradient;
			m_dwBlurPasses = nBlurPasses;
			m_bUsePlayerHelperCams = bUsePlayerCams;
			if ( bResample )
			{
				if ( !m_ResampledCamera.Create(nSizeResampled) )
				{
					Destroy();
					return;
				}
			}
			m_Camera.SetLight(m_pLight);
		}
		else
		{
			Destroy();
			return;
		}
	}
}

// PRESETS
// Very Low: Player - static; peds - static
// Low: Player - 64x64, no resample, 2 blur passes; peds - static
// Medium: Player - 128x128, no resample, 3 blur passes; peds - player's Low
// High: Player - 512x512 resampled to 256x256, 4 blur passes; peds - player's Medium
// Very High: Player AND peds - 512x512 resampled to 256x256, 6 blur passes
static const tShadowQualitySettings			gShadowSettings[] = {
		// Low
		{ 6, 6, 2, true, false },
		// Medium
		{ 7, 7, 3, true, true },
		// High
		{ 9, 8, 4, true, true },
		// Very High
		{ 9, 8, 6, true, false }
	};

void CRealTimeShadowManager::Init()
{
	if ( !m_bInitialised )
	{
		for ( int i = 0; i < NUM_MAX_REALTIME_SHADOWS; i++ )
			m_pShadows[i] = new CRealTimeShadow;

		eShadowQuality		nShadowQuality = CShadows::GetShadowQuality();

		// Realtime shadows used?
		if ( nShadowQuality > SHADOW_QUALITY_LOWEST )
		{
			const tShadowQualitySettings*	pSets_Player;
			const tShadowQualitySettings*	pSets;

			switch ( nShadowQuality )
			{
			case SHADOW_QUALITY_LOW:
				pSets_Player = &gShadowSettings[0];
				pSets = nullptr;
				break;
			case SHADOW_QUALITY_MEDIUM:
				pSets_Player = &gShadowSettings[1];
				pSets = &gShadowSettings[0];
				break;
			case SHADOW_QUALITY_HIGH:
				pSets_Player = &gShadowSettings[2];
				pSets = &gShadowSettings[1];
				break;
			case SHADOW_QUALITY_HIGHEST:
				pSets_Player = &gShadowSettings[3];
				pSets = &gShadowSettings[3];
				break;
			default:
				pSets_Player = nullptr;
				pSets = nullptr;
				break;
			}

			for ( int i = 0; i < NUM_MAX_REALTIME_SHADOWS; i++ )
			{
				if ( !i )
					m_pShadows[0]->Create(pSets_Player->nQuality, pSets_Player->nQualityAfterResample, 
									pSets_Player->nQuality != pSets_Player->nQualityAfterResample,
									pSets_Player->nBlurPasses, pSets_Player->bUseGradient,
									pSets_Player->bDedicatedCamsForPlayer);
				else
				{
					if ( pSets )
						m_pShadows[i]->Create(pSets->nQuality, pSets->nQualityAfterResample, 
									pSets->nQuality != pSets->nQualityAfterResample,
									pSets->nBlurPasses, pSets->bUseGradient,
									false);
				}
			}

			if ( !pSets )
				pSets = pSets_Player;

			m_BlurCamera.Create(pSets->nQualityAfterResample);
			m_GradientCamera.Create(pSets->nQualityAfterResample);
			m_GradientCamera.MakeGradientRaster();

			if ( pSets_Player->bDedicatedCamsForPlayer )
			{
				m_BlurCamera_Player.Create(pSets_Player->nQualityAfterResample);
				m_GradientCamera_Player.Create(pSets_Player->nQualityAfterResample);
				m_GradientCamera_Player.MakeGradientRaster();
				m_bPlayerHelperCamsInUse = true;
			}
			else
				m_bPlayerHelperCamsInUse = false;
		}

		m_bInitialised = true;
	}
}

void CRealTimeShadowManager::Exit()
{
	if ( m_bInitialised )
	{
		for ( int i = 0; i < NUM_MAX_REALTIME_SHADOWS; i++ )
			delete m_pShadows[i];

		m_BlurCamera.Destroy();
		m_GradientCamera.Destroy();

		if ( m_bPlayerHelperCamsInUse )
		{
			m_BlurCamera_Player.Destroy();
			m_GradientCamera_Player.Destroy();
			m_bPlayerHelperCamsInUse = false;
		}
		m_bInitialised = false;
	}
}

void CRealTimeShadowManager::DoShadowThisFrame(CPhysical* pEntity)
{
	bool			bRenderAtLowDetails;
	eShadowQuality	nShadowQuality = CShadows::GetShadowQuality();

	if ( pEntity->nType == 3 )
		bRenderAtLowDetails = static_cast<CPed*>(pEntity)->pedType == 0;
	else
		bRenderAtLowDetails = false;

	if ( nShadowQuality > SHADOW_QUALITY_LOW || (nShadowQuality == SHADOW_QUALITY_LOW && bRenderAtLowDetails) )
	{
		CRealTimeShadow*	pEntityShadow = pEntity->GetRealTimeShadow();
		if ( pEntityShadow )
			pEntityShadow->SetRenderedThisFrame();
		else
			GetRealTimeShadow(pEntity);
	}
}

void CRealTimeShadowManager::GetRealTimeShadow(CPhysical* pEntity)
{
	bool	bIsPlayer;
	bool	bRender = true;

	if ( pEntity->nType == 3 )
		bIsPlayer = static_cast<CPed*>(pEntity)->pedType == 0;
	else
		bIsPlayer = false;

	if ( pEntity->nType != 3 || !bIsPlayer )
	{
		CPed*	pPlayerPed = CWorld::Players[CWorld::PlayerInFocus].GetPed();

		if ( pPlayerPed->pedFlags.bInVehicle )
		{
			CVehicle*	pVehicle = pPlayerPed->pVehicle;
			if ( pVehicle )
				bRender = pVehicle->GetLinearVelocity().MagnitudeSqr() <= 0.3f * 0.3f;
		}
	}

	if ( m_bInitialised && bRender )
	{
		CRealTimeShadow*	pOutShadow;
		if ( bIsPlayer )
			pOutShadow = m_pShadows[0];
		else
		{
			pOutShadow = nullptr;
			for ( int i = 1; i < NUM_MAX_REALTIME_SHADOWS; i++ )
			{
				if ( !m_pShadows[i]->GetOwner() )
				{
					pOutShadow = m_pShadows[i];
					break;
				}
			}
		}

		if ( pOutShadow )
		{
			pOutShadow->SetShadowedObject(pEntity);
			pEntity->SetRealTimeShadow(pOutShadow);
			pOutShadow->SetRenderedThisFrame();
			pOutShadow->ResetIntensity();
		}
	}
}

void CRealTimeShadowManager::ReInit()
{
	if ( CShadows::GetShadowQuality() > SHADOW_QUALITY_LOWEST )
	{
		for ( int i = 0; i < NUM_MAX_REALTIME_SHADOWS; i++ )
			m_pShadows[i]->ReInit();

		m_BlurCamera.ReInit();
		m_GradientCamera.ReInit();
		m_GradientCamera.MakeGradientRaster();

		if ( m_bPlayerHelperCamsInUse )
		{
			m_BlurCamera_Player.ReInit();
			m_GradientCamera_Player.ReInit();
			m_GradientCamera_Player.MakeGradientRaster();
		}
	}
}

void CRealTimeShadowManager::ReturnRealTimeShadow(CRealTimeShadow* pShadow)
{
	if ( m_bInitialised && pShadow->GetOwner() )
	{
		pShadow->GetOwner()->SetRealTimeShadow(nullptr);
		pShadow->ClearOwner();
	}
}


static StaticPatcher	Patcher([](){ 
						Memory::InjectHook(0x7067C0, &CRealTimeShadowManager::Init, PATCH_JUMP);
						Memory::InjectHook(0x706A60, &CRealTimeShadowManager::Exit, PATCH_JUMP);
						Memory::InjectHook(0x706BA0, &CRealTimeShadowManager::DoShadowThisFrame, PATCH_JUMP);
						Memory::InjectHook(0x706AC2, &CRealTimeShadowManager::ReInit);
						Memory::InjectHook(0x706B29, &CRealTimeShadow::Update);
									});