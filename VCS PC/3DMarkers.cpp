#include "StdAfx.h"
#include "3DMarkers.h"

#include "Timer.h"
#include "ModelInfo.h"
#include "Streaming.h"
#include "TxdStore.h"
#include "Scene.h"
#include "Camera.h"
#include "Coronas.h"

// Static variables
float		C3dMarkers::m_PosZMult;
const float	C3dMarkers::m_MovingMultiplier = 0.25f;

RpClump	**C3dMarkers::m_pRpClumpArray = (RpClump**)0xC7C6DC;
C3dMarker *C3dMarkers::m_aMarkerArray = (C3dMarker*)0xC7DD58;
int &C3dMarkers::NumActiveMarkers = *(int*)0xC7C6D8;
float &C3dMarkers::m_angleDiamond = *(float*)0xC7C700;
bool &C3dMarkers::IgnoreRenderLimit = *(bool*)0xC7C704;

void
C3dMarkers::Init(void)
{
	int i;
	C3dMarker *marker;

	marker = m_aMarkerArray;
	for(i = 0; i < 32; i++){
		marker->m_lastPosition = CVector(0, 0, 0);
		marker->m_pAtomic = NULL;
		marker->m_nType = 0x101;
		marker->m_bIsUsed = false;
		marker->m_bIsUsed2 = false;
		marker->m_nIdentifier = 0;
		marker->m_color.red = 255;
		marker->m_color.green = 255;
		marker->m_color.blue = 255;
		marker->m_color.alpha = 255;
		marker->m_nPulsePeriod = 1024;
		marker->m_nRotateRate = 5;
		marker->m_nStartTime = 0;
		marker->m_fPulseFraction = 0.25;
		marker->m_fStdSize = 1.0;
		marker->m_fSize = 1.0;
		marker->m_fBrightness = 1.0;
		marker->m_fCameraRange = 0.0;
		marker->m_normal = CVector(0, 0, 1);
		marker->m_LastMapReadX = 30000;
		marker->m_roofHeight = 65535.0;
		marker++;
	}
	memset(m_pRpClumpArray, 0, sizeof(RpClump*[7]));
	NumActiveMarkers = 0;
	m_angleDiamond = 0;
	int slot = CTxdStore::FindTxdSlot("particle");
	CTxdStore::PushCurrentTxd();
	CTxdStore::SetCurrentTxd(slot);
	m_pRpClumpArray[1] = LoadMarker("cylinder");
	m_pRpClumpArray[4] = LoadMarker("hoop");
	m_pRpClumpArray[0] = LoadMarker("diamond_3");
	m_pRpClumpArray[6] = m_pRpClumpArray[0];
	m_pRpClumpArray[5] = LoadMarker("arrow");
	CTxdStore::PopCurrentTxd();
}

void
C3dMarkers::Render(void)
{
	int alphafunc;
	C3dMarker *marker;
	static RwRGBAReal markerAmbient = { 0.0, 0.0, 0.0, 0.0 };
	static RwRGBAReal markerDirectional = { 0.0, 0.0, 0.0, 0.0 };

	RwRenderStateGet(rwRENDERSTATEALPHATESTFUNCTION, &alphafunc);
	RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTION, (void*)rwALPHATESTFUNCTIONALWAYS);
	NumActiveMarkers = 0;
	ActivateDirectional();
	SetAmbientColours(&markerAmbient);
	SetDirectionalColours(&markerDirectional);
	User3dMarkersDraw();
	for(marker = m_aMarkerArray; marker < &m_aMarkerArray[32]; marker++){
		if(marker->m_bIsUsed2){
			if(marker->m_fCameraRange < 150.0f || IgnoreRenderLimit || marker->m_nType == MARKER_HOOP){
				//if(TheCamera.IsSphereVisible_(*marker->m_mat.GetPos(), 2.0, &TheCamera.m_matInverse.matrix)){
					marker->Render();
					if(marker->m_nType == MARKER_ARROW)
						CCoronas::RegisterCorona((uint32)marker, NULL,
							marker->m_color.red, marker->m_color.green, marker->m_color.blue, 0x84,
							*marker->m_mat.GetPos(), 1.2*marker->m_fSize, TheCamera.LODDistMultiplier * 50.0f,
							gpCoronaTexture[1], 0, 0, 0, 0, 0.0, false, 1.5f, false, 255, false, true);
				//}
			}
			NumActiveMarkers++;
			marker->m_bIsUsed = false;
			marker->m_bIsUsed2 = false;
		}else{
			if(marker->m_pAtomic){
				marker->m_nIdentifier = 0;
				marker->m_nStartTime = 0;
				marker->m_bIsUsed = false;
				marker->m_bIsUsed2 = false;
				marker->m_nType = 0x101;
				RwFrame *f = RpAtomicGetFrame(marker->m_pAtomic);
				RpAtomicDestroy(marker->m_pAtomic);
				RwFrameDestroy(f);
				marker->m_pAtomic = NULL;
			}
		}
	}
	DirectionArrowsDraw();
	RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTION, (void*)alphafunc);
}

WRAPPER RpClump *C3dMarkers::LoadMarker(const char *name) { WRAPARG(name); EAXJMP(0x722810); }

WRAPPER void C3dMarkers::User3dMarkersDraw(void) { EAXJMP(0x723240); }
WRAPPER void C3dMarkers::DirectionArrowsDraw(void) { EAXJMP(0x7211F0); }

WRAPPER C3dMarker* C3dMarkers::PlaceMarker(unsigned int nIndex, unsigned short markerID, CVector& vecPos, float fSize, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, unsigned short pulsePeriod, float pulseFraction, short rotateRate, float normalX, float normalY, float normalZ, bool checkZ)
	{	WRAPARG(nIndex); WRAPARG(markerID); WRAPARG(vecPos); WRAPARG(fSize); WRAPARG(red); WRAPARG(green); WRAPARG(blue); WRAPARG(alpha);
		WRAPARG(pulsePeriod); WRAPARG(pulseFraction); WRAPARG(rotateRate); WRAPARG(normalX); WRAPARG(normalY); WRAPARG(normalZ);
		WRAPARG(checkZ); EAXJMP(0x725120); }

void C3dMarkers::PlaceMarkerSet(unsigned int nIndex, unsigned short markerID, CVector& vecPos, float fSize, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, unsigned short pulsePeriod, float pulseFraction)
{
	PlaceMarker(nIndex, markerID, vecPos, fSize, red, green, blue, static_cast<unsigned char>(alpha * (1.0f/3.0f)), pulsePeriod, pulseFraction, 1, 0.0, 0.0, 0.0, false);
	PlaceMarker(nIndex, markerID, vecPos, fSize * 0.9f, red, green, blue, static_cast<unsigned char>(alpha * (1.0f/3.0f)), pulsePeriod, pulseFraction, -1, 0.0, 0.0, 0.0, false);
}

long double C3dMarker::CalculateRealSize()
{
	long double		fVariable = (((m_nPulsePeriod - 1)) & ( CTimer::m_snTimeInMilliseconds /*- m_nStartTime*/ ));
	return (2 * M_PI) * fVariable / static_cast<long double>(m_nPulsePeriod);
}

void
C3dMarker::Render(void)
{
	if(m_pAtomic == NULL)
		return;
	if(m_nType == 1)
		RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLNONE);
	else
		RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLBACK);
	RpMaterialSetColor(m_pMaterial, &m_color);
	m_mat.UpdateRW();
	CMatrix mat(m_mat.pMatrix);
	mat.Scale(m_fSize);
	mat.UpdateRW();
	RwFrameUpdateObjects(RpClumpGetFrame(m_pAtomic));
	SetBrightMarkerColours();
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);
	if(m_nType == 3)
		RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLNONE);
	RpAtomicRender(m_pAtomic);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);
	ReSetAmbientAndDirectionalColours();
}

void __declspec(naked) C3dMarkerSizeHack()
{
	_asm
	{
		push	eax		// Not keeping it causes marker to flicker
		mov		ecx, esi
		call	C3dMarker::CalculateRealSize
		fst		C3dMarkers::m_PosZMult
		pop		eax
		retn
	}
}

void __declspec(naked) EnexMarkersColorBreak()
{
	_asm
	{
		push	96h
		push	64h
		push	ebx
		//push	00h
		mov		eax, 440F43h
		jmp		eax
	}
}

static StaticPatcher	Patcher([](){
	using namespace Memory;

	InjectHook(0x7269FA, C3dMarkers::Init);
	InjectHook(0x7250B1, &C3dMarker::Render);
	InjectHook(0x726AE4, C3dMarkers::Render);
	*(uint8*)0x8D5D8B = 0xD1;	// cone marker alpha

	//Nop(0x585F9A, 5);	// C3dMarkers::PlaceMarkerCone in CRadar::Draw3dMarkers
	// some radar color
	Patch<BYTE>(0x585CCB, MARKER_SET_COLOR_B);
	Patch<BYTE>(0x585CCD, MARKER_SET_COLOR_G);
	Patch<BYTE>(0x585CCF, MARKER_SET_COLOR_R);

	// Spheres colours
	//dwFunc = 0x4810E0 + 0x2B;
//	patch(dwFunc, MARKER_SET_COLOR_A, 4);
//	dwFunc += 0x6;
	Patch<BYTE>(0x4810E0 + 0x2B, MARKER_SET_COLOR_B);
	//dwFunc += 0x2;
	Patch<BYTE>(0x4810E0 + 0x2B + 0x2, MARKER_SET_COLOR_G);
	//dwFunc += 0x2;
	Patch<BYTE>(0x4810E0 + 0x2B + 0x4, MARKER_SET_COLOR_R);

	/*Patch<BYTE>(0x70CD0B, 0xB4);
	Patch<BYTE>(0x70CD0D, 0x82);
	Patch<BYTE>(0x70CD0F, 0xED);
	Patch<BYTE>(0x70CD58, 0xB4);
	Patch<BYTE>(0x70CD5A, 0x82);
	Patch<BYTE>(0x70CD5C, 0xED);
	Patch<BYTE>(0x70CDAB, 0xB4);
	Patch<BYTE>(0x70CDAD, 0x82);
	Patch<BYTE>(0x70CDAF, 0xED);*/

	// Growing/shrinking 3DMarkers
	Patch<float>(0x440F26, 0.0f);
	InjectHook(0x72576B, &C3dMarkerSizeHack, PATCH_CALL);
	Nop(0x725770, 1);

	// New style of markers
	// What is this?
	InjectHook(0x725BA0, &C3dMarkers::PlaceMarkerSet, PATCH_JUMP);

	// Enex markers RGB
	InjectHook(0x440F38, EnexMarkersColorBreak, PATCH_JUMP);

	// arrow.dff as marker
	Patch<const float*>(0x725636, C3dMarkers::GetPosZMult());
	Patch<const float*>(0x7259A1, C3dMarkers::GetPosZMult());
	//Patch<const float*>(0x7232C7, C3dMarkers::GetPosZMult());
	Patch<const float*>(0x72564B, C3dMarkers::GetMovingMult());
	Patch<const float*>(0x7259A9, C3dMarkers::GetMovingMult());
	Nop(0x72563A, 6);
	Nop(0x72599F, 6);
	Nop(0x72502B, 6);
	Nop(0x725647, 2);
	Patch<uint8>(0x726DA6, 5);	// arrow (old cone) rotate rate
	Patch(0x7232C1, &C3dMarkers::m_pRpClumpArray[0]);	// marker 0 (user marker)

	// Arrow sizes
//	Patch<float>(0x585DA5, 2.5f);	// Vehicles
//	Patch<float>(0x585E4D, 1.5f);	// Peds
//	Patch<float>(0x585F8E, 1.0f);	// Objects

		});