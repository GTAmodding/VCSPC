#include "StdAfx.h"
#include "Shadows.h"

#include "3DMarkers.h"
#include "Coronas.h"
#include "RealTimeShadowMgr.h"
#include "Vehicle.h"
#include "Object.h"
#include "Camera.h"
#include "Building.h"

eShadowQuality	CShadows::m_bShadowQuality;
float			CShadows::m_fShadowDistMult;

float&			MAX_DISTANCE_REALTIME_SHADOWS = *(float*)0x8D5240;
float&			MAX_DISTANCE_REALTIME_SHADOWS_SQR = *(float*)0xC4B6B0;

float			MAX_DISTANCE_PED_SHADOWS, MAX_DISTANCE_PED_SHADOWS_SQR;
float			MAX_DISTANCE_CAR_SHADOWS, MAX_DISTANCE_CAR_SHADOWS_SQR;

WRAPPER void CShadows::UpdateStaticShadows(void) { EAXJMP(0x707F40); }
WRAPPER void CShadows::RenderStaticShadows(void) { EAXJMP(0x708300); }
WRAPPER void CShadows::RenderStoredShadows(void) { EAXJMP(0x70A960); }

WRAPPER void CShadows::CalcPedShadowValues(float x, float y, float z, float *sfx, float *sfy, float *ssx, float *ssy, float *sdx, float *sdy)
{ WRAPARG(x); WRAPARG(y); WRAPARG(z); WRAPARG(sfx); WRAPARG(sfy); WRAPARG(ssx); WRAPARG(ssy); WRAPARG(sdx); WRAPARG(sdy); EAXJMP(0x7076C0); }

// SHADOW DRAW DISTANCES
// Ped
// Min - 12.5 Max - 45.0
// Vehicle
// Min - 15.0 Max - 60.0
// Object
// Min - 15.0 Max - 60.0

void CShadows::RenderIndicatorShadow(unsigned int nIndex, unsigned char, RwTexture*, CVector* pPos, float radiusX, float, float, float radiusY, short)
{
	C3dMarkers::PlaceMarkerSet(nIndex, 1, *pPos, radiusX > -radiusY ? radiusX : -radiusY, MARKER_SET_COLOR_R, MARKER_SET_COLOR_G, MARKER_SET_COLOR_B, MARKER_SET_COLOR_A, 2048, 0.2f);
}

void CShadows::InitialiseChangedSettings()
{
	static eShadowQuality	eOldQuality = SHADOW_QUALITY_UNDEFINED;

	if ( eOldQuality != m_bShadowQuality )
	{
		eOldQuality = m_bShadowQuality;
		g_realTimeShadowMan.ResetForChangedSettings();
	}

	// Recalculate distances
	MAX_DISTANCE_PED_SHADOWS = 12.5f + (m_fShadowDistMult*(45.0f-12.5f));
	MAX_DISTANCE_CAR_SHADOWS = 15.0f + (m_fShadowDistMult*(60.0f-15.0f));

	MAX_DISTANCE_PED_SHADOWS_SQR = MAX_DISTANCE_PED_SHADOWS*MAX_DISTANCE_PED_SHADOWS;
	MAX_DISTANCE_CAR_SHADOWS_SQR = MAX_DISTANCE_CAR_SHADOWS*MAX_DISTANCE_CAR_SHADOWS;
}

bool CShadows::StoreRealTimeShadowForVehicle(CVehicle* pVehicle)
{
	if ( m_bShadowQuality > SHADOW_QUALITY_LOW )
	{
		g_realTimeShadowMan.DoShadowThisFrame(pVehicle);

		return	pVehicle->GetClass() == VEHICLE_BIKE || 
				pVehicle->GetSubClass() == VEHICLE_HELI || pVehicle->GetSubClass() == VEHICLE_PLANE;
	}

	return m_bShadowQuality == SHADOW_QUALITY_OFF;
}

void CShadows::StoreRealTimeShadowForObject(CObject* pObject)
{
	if ( m_bShadowQuality > SHADOW_QUALITY_MEDIUM && ThisPropCanHaveShadow(pObject) )
		g_realTimeShadowMan.DoShadowThisFrame(pObject);
}

void CShadows::StoreRealTimeShadowForBuilding(CBuilding* pBuilding)
{
	if ( m_bShadowQuality > SHADOW_QUALITY_MEDIUM && ThisPropCanHaveShadow(pBuilding) )
	{
		float		fMaxDist = GetRealTimeShadowDistances(pBuilding) * 1.1f;
		if ( (pBuilding->GetCoords() - TheCamera.GetCoords()).MagnitudeSqr() < fMaxDist*fMaxDist )
		{
			g_realTimeShadowMan.DoShadowThisFrame(pBuilding);
		}
	}
}

bool CShadows::ThisPropCanHaveShadow(CEntity* pEntity)
{
	// Is flying component?
	if ( pEntity->m_nModelIndex >= 374 && pEntity->m_nModelIndex <= 379 )
		return true;

	// IDE flag enabled?
	CAtomicModelInfo*	pModelInfo = CModelInfo::ms_modelInfoPtrs[pEntity->m_nModelIndex]->AsAtomicModelInfoPtr();

	if ( pModelInfo )
		return pModelInfo->CastShadow();

	return false;
}

void CShadows::SetRealTimeShadowDistances(CEntity* pEntity)
{
	switch ( pEntity->nType )
	{
	case 3:
		MAX_DISTANCE_REALTIME_SHADOWS = MAX_DISTANCE_PED_SHADOWS;
		MAX_DISTANCE_REALTIME_SHADOWS_SQR = MAX_DISTANCE_PED_SHADOWS_SQR;
		break;
	default:
		MAX_DISTANCE_REALTIME_SHADOWS = MAX_DISTANCE_CAR_SHADOWS;
		MAX_DISTANCE_REALTIME_SHADOWS_SQR = MAX_DISTANCE_CAR_SHADOWS_SQR;
		break;
	}
}

float CShadows::GetRealTimeShadowDistances(CEntity* pEntity)
{
	switch ( pEntity->nType )
	{
	case 3:
		return MAX_DISTANCE_PED_SHADOWS;
	default:
		return MAX_DISTANCE_CAR_SHADOWS;
	}
}

static const float f215 = 2.15f;
static const float f115 = 1.15f;

//static float gA;
static float gB;

static CEntity *gCurrentEntityStoresShadow;




/*void CastShadow(void *a1, float a2, float a3, float a4, float a5, void *a6, float a7, float a8, float a9, float a10, short a11, char a12, char a13, char a14, float a15, 
						 float a16, void **a17, CRealTimeShadow* a18, char *a19)
{
	gCurrentEntityStoresShadow = a18->GetOwner();;
	((void (__cdecl *)(void *, float, float, float, float, void *, float, float, float, float, short, char, char, char, float, float, void **, CRealTimeShadow *, char *))0x70A7E0)
		(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19);
}*/

/*void __fastcall SetupShadowBoundSphere(void *sphere, int edx0, float size, CVector  const& center, unsigned char material, unsigned char flags, unsigned char lighting)
{
	((void (__thiscall *)(void *, float, CVector  const&, unsigned char, unsigned char, unsigned char))0x40FD10)
	//	(sphere, 7.5f/*3.0f*///, center, material, flags, lighting);
//}

#define NightState (*(float *)0x8D12C0)

void __declspec(naked) CompareSunZ()
{
	static float	gNight, sunZ;
	__asm{
	mov     ecx, edi
	mov     eax, dword ptr [eax+8]
	mov     sunZ, eax
	}
	gNight = NightState * 0.5f + 0.6f;
	if(sunZ < gNight)
		sunZ = gNight;
	__asm{
	fld ds:sunZ
	mov eax, 0x707E30
	jmp eax
	}
}

static void __declspec(naked) ShowShadowsConditionHack()
{
	_asm
	{
		mov		eax, [CShadows::m_bShadowQuality]
		test	eax, eax	// Shadows off?
		jz		ShowShadowsConditionHack_ShadowsOff
		cmp		eax, SHADOW_QUALITY_LOWEST
		jz		ShowShadowsConditionHack_Static
		cmp		eax, SHADOW_QUALITY_LOW
		ja		ShowShadowsConditionHack_Dynamic
		mov     eax, [ebp+598h]
		test	eax, eax
		jnz		ShowShadowsConditionHack_Static

ShowShadowsConditionHack_Dynamic:
		push	5E678Fh
		retn

ShowShadowsConditionHack_Static:
		push	5E68B7h
		retn

ShowShadowsConditionHack_ShadowsOff:
		push	5E6908h
		retn
	}
}

static void __declspec(naked) StoreRTVehicleShadowHack()
{
	_asm
	{
		push	[esp+4Ch+4]
		call	CShadows::StoreRealTimeShadowForVehicle
		add		esp, 4
		retn
	}
}

static void __declspec(naked) StoreRTObjectShadowHack()
{
	_asm
	{
		push	esi
		call	CShadows::StoreRealTimeShadowForObject
		add		esp, 4
		pop		esi
		add		esp, 10h
		retn
	}
}

static void __declspec(naked) GetShadowHack()
{
	_asm
	{
		push	ecx
		call	CShadows::SetRealTimeShadowDistances
		add		esp, 4
		mov		ecx, ebp
		call	CEntity::GetRealTimeShadow
		mov		edi, eax
		push	707CB0h
		retn
	}
}

static void __declspec(naked) SelfShadowingFix()
{
	_asm
	{
		jnz		SelfShadowingFix_DontCast
		cmp		esi, [ebx]CRealTimeShadow.m_pEntity
		jz		SelfShadowingFix_DontCast
		push	70A841h
		retn

SelfShadowingFix_DontCast:
		push	70A93Fh
		retn
	}
}

/*static void __declspec(naked) StoreRTPoleShadowHack()
{
	_asm
	{
		push	[esp+18h+4]
		call	CShadows::StoreRealTimeShadowForVehicle
		add		esp, 4
		retn
	}
}*/

void CShadows::Inject()
{
	Memory::InjectHook(0x70CCB0, RenderIndicatorShadow, PATCH_JUMP);

	Memory::InjectHook(0x5E6759, ShowShadowsConditionHack, PATCH_JUMP);
	
	// No stencil shadows
	Memory::Patch<BYTE>(0x53E159, 0xC3);
	Memory::Nop(0x53C1AB, 5);
	
	// Off vehicle shadows
	//Memory::Patch<BYTE>(0x70F9B0, 0xA1);
	//Memory::Patch<const void*>(0x70F9B1, &CShadows::m_bShadowQuality);
	//Memory::Patch<DWORD>(0x70F9B5, 0x940FC085);
	//Memory::Patch<WORD>(0x70F9B9, 0xC3C0);
	Memory::InjectHook(0x70C753, DontRenderShadowsForPoles);

	// Adjustable shadows draw distance
	Memory::Patch<const void*>(0x707BF3, &MAX_DISTANCE_PED_SHADOWS);
	Memory::Patch<const void*>(0x5E67E0, &MAX_DISTANCE_PED_SHADOWS_SQR);
	Memory::Patch<const void*>(0x707B9E, &MAX_DISTANCE_PED_SHADOWS_SQR);

	Memory::Patch<const void*>(0x70BEA7, &MAX_DISTANCE_CAR_SHADOWS_SQR);
	Memory::Patch<const void*>(0x70BEB6, &MAX_DISTANCE_CAR_SHADOWS);
}

/*void MatrixTranslate(RwMatrix* matrix, CRealTimeShadow* pShadow)
{
	const RwV3d		translation = { 0.5f, 0.5f, 0.0f };
	RwMatrixTranslate(matrix, &translation, rwCOMBINEPOSTCONCAT);
}*/

static StaticPatcher	Patcher([](){ 

						// CRealTimeShadow for vehicle (temp)
						/*Memory::Patch<DWORD>(0x70BDA0, 0xC40350B9);
						Memory::Patch<DWORD>(0x70BDA4, 0x2474FF00);
						Memory::Patch<BYTE>(0x70BDA8, 0x04);
						Memory::InjectHook(0x70BDA9, 0x706BA0, PATCH_CALL);
						Memory::Patch<BYTE>(0x70BDAE, 0xC3);*/

						Memory::Patch<const void*>(0x707EF7, &f215);
						Memory::Patch<const void*>(0x707F05, &f215);
						Memory::Patch<const void*>(0x707F13, &f215);
						Memory::Patch<const void*>(0x707F21, &f215);
						
						Memory::Patch<const void*>(0x70A211, &f115);
						Memory::Patch<const void*>(0x70A228, &f115);

						//Memory::InjectHook(0x707E4F, SetLightParameters);
						//Memory::InjectHook(0x70596A, RotateLightFrame);
						Memory::Patch<DWORD>(0x705960, 0x3C2474FF);
						Memory::Nop(0x70595F, 1);

						// Same as TranslateShdMatrix
						Memory::Patch<float>(0x70A19D, 0.5f);
						// Same as SetupShadowBoundSphere
						Memory::Patch<float>(0x70A2AA, 23.5f);
						Memory::Patch<float>(0x707D59, 25.0f);

						//Memory::InjectHook(0x70A1AC, TranlateShdMatrix);
						Memory::InjectHook(0x707E2B, CompareSunZ, PATCH_JUMP);

						//Memory::InjectHook(0x70AD0D, CastShadow);
						//Memory::InjectHook(0x70A2C8, SetupShadowBoundSphere);
						/*Memory::Patch<DWORD>(0x70A182, 0x3824B4FF);
						Memory::Patch<DWORD>(0x70A186, 0x8D000002);
						Memory::Patch<DWORD>(0x70A18C, 0xF8);
						Memory::Patch<WORD>(0x70A191, 0x19EB);
						Memory::Patch<DWORD>(0x70A1B7, 0x224);
						Memory::Patch<BYTE>(0x70A1BD, 0x1C);
						Memory::InjectHook(0x70A1AC, MatrixTranslate);*/

						// matrix rotate
						Memory::Nop(0x70A0C9, 5);

						// Sun pos fix
						Memory::Nop(0x707E29, 2);
						Memory::Nop(0x707E3E, 2);

						// Improved in car shadows
						//Memory::Patch<WORD>(0x5E682C, 0x26EB);
						Memory::Patch<BYTE>(0x5E6813, 0xEB);
						//Memory::Patch<BYTE>(0x5E683B, 0xEB);
						Memory::InjectHook(0x70BDA4, StoreRTVehicleShadowHack);
						Memory::InjectHook(0x59FEDB, StoreRTObjectShadowHack, PATCH_JUMP);
						Memory::InjectHook(0x707CAA, GetShadowHack, PATCH_JUMP);
						Memory::InjectHook(0x70A83B, SelfShadowingFix, PATCH_JUMP);
						//Memory::InjectHook(0x70C753, StoreRTPoleShadowHack);

						//Memory::InjectHook(0x705590, &CShadowCamera::SetCenter, PATCH_JUMP);
						CShadows::Inject();

						});