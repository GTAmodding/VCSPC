#include "StdAfx.h"
#include "Shadows.h"

#include "3DMarkers.h"
#include "Coronas.h"
#include "RealTimeShadowMgr.h"
#include "Vehicle.h"
#include "Object.h"

eShadowQuality CShadows::m_bShadowQuality;

float&			MAX_DISTANCE_PED_SHADOWS = *(float*)0x8D5240;

void CShadows::RenderIndicatorShadow(unsigned int nIndex, unsigned char, RwTexture*, CVector* pPos, float radiusX, float, float, float radiusY, short)
{
	C3DMarkers::PlaceMarkerSet(nIndex, 1, *pPos, radiusX > -radiusY ? radiusX : -radiusY, MARKER_SET_COLOR_R, MARKER_SET_COLOR_G, MARKER_SET_COLOR_B, MARKER_SET_COLOR_A, 2048, 0.2f);
}

void CShadows::InitialiseChangedSettings()
{
	g_realTimeShadowMan.ResetForChangedSettings();
}

bool CShadows::StoreRealTimeShadowForVehicle(CVehicle* pVehicle)
{
	if ( m_bShadowQuality > SHADOW_QUALITY_LOW )
	{
		g_realTimeShadowMan.DoShadowThisFrame(pVehicle);

		return pVehicle->GetSubClass() == VEHICLE_HELI || pVehicle->GetSubClass() == VEHICLE_PLANE;
	}

	return false;
}

void CShadows::StoreRealTimeShadowForObject(CObject* pObject)
{
	if ( m_bShadowQuality > SHADOW_QUALITY_MEDIUM && ThisPropCanHaveShadow(pObject) )
		g_realTimeShadowMan.DoShadowThisFrame(pObject);

}

bool CShadows::ThisPropCanHaveShadow(CPhysical* pPhysical)
{
	return pPhysical->m_nModelIndex == MI_PARKBENCH || pPhysical->m_nModelIndex == MI_CANOPY_TEST || pPhysical->m_nModelIndex == MI_CHAIR_TEST
		|| pPhysical->m_nModelIndex == MI_PAPERMACHINE || pPhysical->m_nModelIndex == MI_HYDRANT
		// Flying components
		|| ( pPhysical->m_nModelIndex >= 374 && pPhysical->m_nModelIndex <= 379 );
}

static const float f215 = 2.15f;
static const float f115 = 1.15f;

//static float gA;
static float gB;

static CPhysical *gCurrentEntityStoresShadow;

void * __fastcall SetLightParameters(int shd, int edx0, float a, float b, bool set)
{
	//gA = a;
	gB = b;
	return ((void *(__thiscall *)(int, float, float, bool))0x705900)(shd, a, b, set);
}

void RotateLightFrame(RwFrame *frame, RwV3d *axis, float angle, int combine)
{
	((void (__cdecl *)(RwFrame *, RwV3d *, float, int))0x7F1010)(frame, axis, gB, combine);
}

void TranlateShdMatrix(RwMatrix *matrix, RwV3d *translation, int combineOp)
{
	translation->y = 0.5f;
	((void (__cdecl *)(RwMatrix *, RwV3d *, int))0x7F2450)(matrix, translation, combineOp);
}

void CastShadow(void *a1, float a2, float a3, float a4, float a5, void *a6, float a7, float a8, float a9, float a10, short a11, char a12, char a13, char a14, float a15, 
						 float a16, void **a17, CRealTimeShadow* a18, char *a19)
{
	gCurrentEntityStoresShadow = a18->GetOwner();;
	((void (__cdecl *)(void *, float, float, float, float, void *, float, float, float, float, short, char, char, char, float, float, void **, CRealTimeShadow *, char *))0x70A7E0)
		(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19);
}

void __fastcall SetupShadowBoundSphere(void *sphere, int edx0, float size, CVector  const& center, unsigned char material, unsigned char flags, unsigned char lighting)
{
	((void (__thiscall *)(void *, float, CVector  const&, unsigned char, unsigned char, unsigned char))0x40FD10)
		(sphere, 3.0f, center, material, flags, lighting);
}

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
	Memory::Patch<BYTE>(0x70F9B0, 0xA1);
	Memory::Patch<const void*>(0x70F9B1, &CShadows::m_bShadowQuality);
	Memory::Patch<DWORD>(0x70F9B5, 0x940FC085);
	Memory::Patch<WORD>(0x70F9B9, 0xC3C0);
}

static StaticPatcher	Patcher([](){ 

#ifdef NEW_SHADOWS_TEST2
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

						//Memory::Patch<const void*>(0x70A211, &f115);
						//Memory::Patch<const void*>(0x70A228, &f115);

						Memory::InjectHook(0x707E4F, SetLightParameters);
						Memory::InjectHook(0x70596A, RotateLightFrame);

						// Same as TranslateShdMatrix
						Memory::Patch<float>(0x70A19D, 0.5f);

						//Memory::InjectHook(0x70A1AC, TranlateShdMatrix);
						Memory::InjectHook(0x707E2B, CompareSunZ, PATCH_JUMP);

						//Memory::InjectHook(0x70AD0D, CastShadow);
						Memory::InjectHook(0x70A2C8, SetupShadowBoundSphere);

						// matrix rotate
						Memory::Nop(0x70A0C9, 5);

						// Improved in car shadows
						//Memory::Patch<WORD>(0x5E682C, 0x26EB);
						Memory::Patch<BYTE>(0x5E6813, 0xEB);
						//Memory::Patch<BYTE>(0x5E683B, 0xEB);
						Memory::InjectHook(0x70BDA4, StoreRTVehicleShadowHack);
						Memory::InjectHook(0x59FEDB, StoreRTObjectShadowHack, PATCH_JUMP);
						//Memory::InjectHook(0x70C753, StoreRTPoleShadowHack);
#endif
						//Memory::InjectHook(0x705590, &CShadowCamera::SetCenter, PATCH_JUMP);
						CShadows::Inject();

						});