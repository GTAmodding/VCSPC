#include "StdAfx.h"
#include "Shadows.h"

#include "3DMarkers.h"
#include "Coronas.h"

void CShadows::RenderIndicatorShadow(unsigned int nIndex, unsigned char, RwTexture*, CVector* pPos, float radiusX, float, float, float radiusY, short)
{
	C3DMarkers::PlaceMarkerSet(nIndex, 1, *pPos, radiusX > -radiusY ? radiusX : -radiusY, MARKER_SET_COLOR_R, MARKER_SET_COLOR_G, MARKER_SET_COLOR_B, MARKER_SET_COLOR_A, 2048, 0.2f);
}


static const float f215 = 2.15f;
static const float f115 = 1.15f;

float gA;
float gB;

CPhysical *gCurrentEntityStoresShadow;

struct CShadowCamera
{
	RwCamera*		m_pCamera;
	RwTexture*		m_pTexture;

	RwCamera*		SetCenter(RwV3d* vecCenter)
	{
		RwFrame*	pCamFrame = RwCameraGetFrame(m_pCamera);

		RwFrameGetMatrix(pCamFrame)->pos.x = RwCameraGetFarClipPlane(m_pCamera) * -0.5f * RwFrameGetMatrix(pCamFrame)->at.x + vecCenter->x;
		RwFrameGetMatrix(pCamFrame)->pos.y = RwCameraGetFarClipPlane(m_pCamera) * -0.5f * RwFrameGetMatrix(pCamFrame)->at.y + vecCenter->y;
		RwFrameGetMatrix(pCamFrame)->pos.z = RwCameraGetFarClipPlane(m_pCamera) * -0.5f * RwFrameGetMatrix(pCamFrame)->at.z + vecCenter->z;

		RwMatrixUpdate(RwFrameGetMatrix(pCamFrame));
		RwFrameUpdateObjects(pCamFrame);
		RwFrameOrthoNormalize(pCamFrame);

		CMatrix		HelperMatrix(RwFrameGetMatrix(pCamFrame));

		CVector		vecPos = *HelperMatrix.GetPos();
		CVector		vecRight = vecPos + *HelperMatrix.GetRight();
		CVector		vecUp = vecPos + *HelperMatrix.GetUp();
		CVector		vecAt = vecPos + *HelperMatrix.GetAt();

		// pos
		CCoronas::RegisterCorona(111111112, nullptr, 255, 255, 0, 255, vecPos, 0.25f, 1000.0f, gpCoronaTexture[1], 0, 0, 0, 0, 0.0f, false, 1.5f, false, 255, false, true);

		// right
		CCoronas::RegisterCorona(111111112 + 1, nullptr, 255, 0, 0, 255, vecRight, 0.25f, 1000.0f, gpCoronaTexture[1], 0, 0, 0, 0, 0.0f, false, 1.5f, false, 255, false, true);

		// up
		CCoronas::RegisterCorona(111111112 + 2, nullptr, 0, 255, 0, 255, vecUp, 0.25f, 1000.0f, gpCoronaTexture[1], 0, 0, 0, 0, 0.0f, false, 1.5f, false, 255, false, true);

		// at
		CCoronas::RegisterCorona(111111112 + 3, nullptr, 0, 0, 255, 255, vecAt, 0.25f, 1000.0f, gpCoronaTexture[1], 0, 0, 0, 0, 0.0f, false, 1.5f, false, 255, false, true);

		return m_pCamera;
	}
};

struct CRealTimeShadow
{
    CPhysical*		m_pEntity;
 char isExist, intensity;
 CShadowCamera image;
 char isBlurred;
 CShadowCamera blurredImage;
 int blurLevel;
 char createBlurTypeB;
 int objectType;
 RpLight *light;
 RwSphere boundingSphere, baseSphere;
};

void * __fastcall SetLightParameters(int shd, int edx0, float a, float b, bool set)
{
	gA = a;
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
	gCurrentEntityStoresShadow = a18->m_pEntity;
	((void (__cdecl *)(void *, float, float, float, float, void *, float, float, float, float, short, char, char, char, float, float, void **, CRealTimeShadow *, char *))0x70A7E0)
		(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19);
}

void __fastcall SetupShadowBoundSphere(void *sphere, int edx0, float size, CVector  const& center, unsigned char material, unsigned char flags, unsigned char lighting)
{
	((void (__thiscall *)(void *, float, CVector  const&, unsigned char, unsigned char, unsigned char))0x40FD10)
		(sphere, 2.0f, center, material, flags, lighting);
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

static StaticPatcher	Patcher([](){ 
						Memory::InjectHook(0x70CCB0, CShadows::RenderIndicatorShadow, PATCH_JUMP);

#ifdef NEW_SHADOWS_TEST2
						// Disable stencil
						Memory::Patch<BYTE>(0x53E159, 0xC3);

						// CRealTimeShadow for vehicle (temp)
						Memory::Patch<DWORD>(0x70BDA0, 0xC40350B9);
						Memory::Patch<DWORD>(0x70BDA4, 0x2474FF00);
						Memory::Patch<BYTE>(0x70BDA8, 0x04);
						Memory::InjectHook(0x70BDA9, 0x706BA0, PATCH_CALL);
						Memory::Patch<BYTE>(0x70BDAE, 0xC3);

						Memory::Patch<const void*>(0x707EF7, &f215);
						Memory::Patch<const void*>(0x707F05, &f215);
						Memory::Patch<const void*>(0x707F13, &f215);
						Memory::Patch<const void*>(0x707F21, &f215);

						//Memory::Patch<const void*>(0x70A211, &f115);
						//Memory::Patch<const void*>(0x70A228, &f115);

						Memory::InjectHook(0x707E4F, SetLightParameters);
						Memory::InjectHook(0x70596A, RotateLightFrame);

						// Same as TranlateShdMatrix
						Memory::Patch<float>(0x70A19D, 0.5f);

						//Memory::InjectHook(0x70A1AC, TranlateShdMatrix);
						//Memory::InjectHook(0x707E2B, CompareSunZ, PATCH_JUMP);

						//Memory::InjectHook(0x70AD0D, CastShadow);
						Memory::InjectHook(0x70A2C8, SetupShadowBoundSphere);

						// matrix rotate
						Memory::Nop(0x70A0C9, 5);
#endif
						//Memory::InjectHook(0x705590, &CShadowCamera::SetCenter, PATCH_JUMP);

						});