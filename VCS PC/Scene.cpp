#include "StdAfx.h"
#include "Scene.h"

#include "Coronas.h"
#include "TimeCycle.h"
#include "Weather.h"
#include "Frontend.h"
#include "PostEffects.h"
#include "Renderer.h"
#include "VisibilityPlugins.h"
#include "Breakable.h"
#include "Camera.h"
#include "Shadows.h"
#include "NeoCarpipe.h"

GlobalScene &Scene = *(GlobalScene*)0xC17038;

RpLight *&pAmbient = *(RpLight**)0xC886E8;
RpLight *&pDirect = *(RpLight**)0xC886EC;
RpLight **pExtraDirectionals = (RpLight**)0xC886F0;	// [6]
int &NumExtraDirLightsInWorld = *(int*)0xC88708;
RwRGBAReal &DirectionalLightColourForFrame = *(RwRGBAReal*)0xC886B4;
RwRGBAReal &AmbientLightColourForFrame_PedsCarsAndObjects = *(RwRGBAReal*)0xC886C4;
RwRGBAReal &AmbientLightColourForFrame = *(RwRGBAReal*)0xC886D4;
RwRGBAReal &AmbientLightColour = *(RwRGBAReal*)0xC886A4;
RwRGBAReal &DirectionalLightColour = *(RwRGBAReal*)0xC88694;

void
SetLightsWithTimeOfDayColour(RpWorld*)
{
	if(pAmbient){
		AmbientLightColourForFrame.red = CTimeCycle::m_CurrentColours.ambr * CCoronas::LightsMult;
		AmbientLightColourForFrame.green = CTimeCycle::m_CurrentColours.ambg * CCoronas::LightsMult;
		AmbientLightColourForFrame.blue = CTimeCycle::m_CurrentColours.ambb * CCoronas::LightsMult;
		AmbientLightColourForFrame_PedsCarsAndObjects.red = CTimeCycle::m_CurrentColours.ambobjr * CCoronas::LightsMult;
		AmbientLightColourForFrame_PedsCarsAndObjects.green = CTimeCycle::m_CurrentColours.ambobjg * CCoronas::LightsMult;
		AmbientLightColourForFrame_PedsCarsAndObjects.blue = CTimeCycle::m_CurrentColours.ambobjb * CCoronas::LightsMult;
		// Game most likely uses PS2 color range here too. We should do this in a shader but for now just convert the intensity here
		AmbientLightColourForFrame_PedsCarsAndObjects.red *= 255.0/128.0;
		AmbientLightColourForFrame_PedsCarsAndObjects.green *= 255.0/128.0;
		AmbientLightColourForFrame_PedsCarsAndObjects.blue *= 255.0/128.0;
		if(AmbientLightColourForFrame_PedsCarsAndObjects.red > 1.0) AmbientLightColourForFrame_PedsCarsAndObjects.red = 1.0;
		if(AmbientLightColourForFrame_PedsCarsAndObjects.green > 1.0) AmbientLightColourForFrame_PedsCarsAndObjects.green = 1.0;
		if(AmbientLightColourForFrame_PedsCarsAndObjects.blue > 1.0) AmbientLightColourForFrame_PedsCarsAndObjects.blue = 1.0;
		if(CWeather::LightningFlash){
			AmbientLightColourForFrame.red = 1.0f;
			AmbientLightColourForFrame.green = 1.0f;
			AmbientLightColourForFrame.blue = 1.0f;
			AmbientLightColourForFrame_PedsCarsAndObjects.red = 1.0f;
			AmbientLightColourForFrame_PedsCarsAndObjects.green = 1.0f;
			AmbientLightColourForFrame_PedsCarsAndObjects.blue = 1.0f;
		}
		RpLightSetColor(pAmbient, &AmbientLightColourForFrame);
	}
	if(pDirect){
		DirectionalLightColourForFrame.red = CTimeCycle::m_CurrentColours.dirr * CCoronas::LightsMult;
		DirectionalLightColourForFrame.green = CTimeCycle::m_CurrentColours.dirg * CCoronas::LightsMult;
		DirectionalLightColourForFrame.blue = CTimeCycle::m_CurrentColours.dirb * CCoronas::LightsMult;
		RpLightSetColor(pDirect, &DirectionalLightColourForFrame);
		CVector sun = CTimeCycle::m_VectorToSun[CTimeCycle::m_CurrentStoredValue];
		CVector v0(0.0, 0.0, 1.0);
		CVector v1 = CrossProduct(v0, sun);
		v1.Normalise();
		CVector v2 = CrossProduct(v1, sun);
		RwMatrix m;
		RwMatrixSetIdentity(&m);
		m.right = *(RwV3d*)&v0;
		m.up = *(RwV3d*)&v1;
		v0 = -sun;
		m.at = *(RwV3d*)&v0;
		RwFrameTransform(RpLightGetFrame(pDirect), &m, rwCOMBINEREPLACE);
	}

	if(FrontEndMenuManager.m_dwBrightness > 256){
		float f = FrontEndMenuManager.m_dwBrightness/256.0 - 1.0f;
		float staticf = 3.0 * f * 0.6 + 1.0;
		float dynamicf = 2.0 * f * 0.6 + 1.0;

		AmbientLightColourForFrame.red *= staticf;
		if(AmbientLightColourForFrame.red > 1.0) AmbientLightColourForFrame.red = 1.0;
		AmbientLightColourForFrame.green *= staticf;
		if(AmbientLightColourForFrame.green > 1.0) AmbientLightColourForFrame.green = 1.0;
		AmbientLightColourForFrame.blue *= staticf;
		if(AmbientLightColourForFrame.blue > 1.0) AmbientLightColourForFrame.blue = 1.0;

		AmbientLightColourForFrame_PedsCarsAndObjects.red *= dynamicf;
		if(AmbientLightColourForFrame_PedsCarsAndObjects.red > 1.0) AmbientLightColourForFrame_PedsCarsAndObjects.red = 1.0;
		AmbientLightColourForFrame_PedsCarsAndObjects.green *= dynamicf;
		if(AmbientLightColourForFrame_PedsCarsAndObjects.green > 1.0) AmbientLightColourForFrame_PedsCarsAndObjects.green = 1.0;
		AmbientLightColourForFrame_PedsCarsAndObjects.blue *= dynamicf;
		if(AmbientLightColourForFrame_PedsCarsAndObjects.blue > 1.0) AmbientLightColourForFrame_PedsCarsAndObjects.blue = 1.0;

		// Vice City calculates new directional from ambient, but this must be a bug, even though VCS code appears to do the same o_O
		DirectionalLightColourForFrame.red = DirectionalLightColourForFrame.red * dynamicf;
		if(DirectionalLightColourForFrame.red > 1.0) DirectionalLightColourForFrame.red = 1.0;
		DirectionalLightColourForFrame.green = DirectionalLightColourForFrame.green * dynamicf;
		if(DirectionalLightColourForFrame.green > 1.0) DirectionalLightColourForFrame.green = 1.0;
		DirectionalLightColourForFrame.blue = DirectionalLightColourForFrame.blue * dynamicf;
		if(DirectionalLightColourForFrame.blue > 1.0) DirectionalLightColourForFrame.blue = 1.0;
	}
}


void
SetLightColoursForPedsCarsAndObjects(float f)
{
	DirectionalLightColour.red = DirectionalLightColourForFrame.red * f;
	DirectionalLightColour.green = DirectionalLightColourForFrame.green * f;
	DirectionalLightColour.blue = DirectionalLightColourForFrame.blue * f;
	AmbientLightColour.red = CTimeCycle::m_BrightnessAddedToAmbientRed + AmbientLightColourForFrame_PedsCarsAndObjects.red * f;
	AmbientLightColour.green = CTimeCycle::m_BrightnessAddedToAmbientGreen + AmbientLightColourForFrame_PedsCarsAndObjects.green * f;
	AmbientLightColour.blue = CTimeCycle::m_BrightnessAddedToAmbientBlue + AmbientLightColourForFrame_PedsCarsAndObjects.blue * f;
	RpLightSetColor(pAmbient, &AmbientLightColour);
	RpLightSetColor(pDirect, &DirectionalLightColour);
}

void DeActivateDirectional()
{
	pDirect->object.object.flags = 0;
}

void
SetBrightMarkerColours(void)
{
	DirectionalLightColour.red = (1.0f - DirectionalLightColourForFrame.red) * 0.4f + DirectionalLightColourForFrame.red;
	DirectionalLightColour.green = (1.0f - DirectionalLightColourForFrame.green) * 0.4f + DirectionalLightColourForFrame.green;
	DirectionalLightColour.blue = (1.0f - DirectionalLightColourForFrame.blue) * 0.4f + DirectionalLightColourForFrame.blue;
	AmbientLightColour.red = 0.6;
	AmbientLightColour.green = 0.6;
	AmbientLightColour.blue = 0.6;
	RpLightSetColor(pAmbient, &AmbientLightColour);
	RpLightSetColor(pDirect, &DirectionalLightColour);
}

void
ReSetAmbientAndDirectionalColours(void)
{
	RpLightSetColor(pAmbient, &AmbientLightColourForFrame);
	RpLightSetColor(pDirect, &DirectionalLightColourForFrame);
}

void
SetAmbientColours(void)
{
	RpLightSetColor(pAmbient, &AmbientLightColourForFrame);
}
void
SetAmbientColours(RwRGBAReal *color)
{
	RpLightSetColor(pAmbient, color);
}

void
SetDirectionalColours(void)
{
	RpLightSetColor(pDirect, &DirectionalLightColourForFrame);
}

void
SetDirectionalColours(RwRGBAReal *color)
{
	RpLightSetColor(pDirect, color);
}

void
ActivateDirectional(void)
{
	RpLightSetFlags(pDirect, rpLIGHTLIGHTATOMICS);
}

WRAPPER void CBirds__Render(void) { EAXJMP(0x712810); }
WRAPPER void CRopes__Render(void) { EAXJMP(0x556AE0); }
WRAPPER void CGlass__Render(void) { EAXJMP(0x71CE20); }
void CMovingThings__Render(void) {}
WRAPPER void CMovingThings__Render_BeforeClouds(void) { EAXJMP(0x7178F0); }
WRAPPER void Fx_c__Render(RwCamera *cam, int i) { WRAPARG(cam); WRAPARG(i); EAXJMP(0x49E650); }
WRAPPER void CWaterCannons__Render(void) { EAXJMP(0x729B30); }
WRAPPER void CWaterLevel__RenderWaterFog(void) { EAXJMP(0x6E7760); }
WRAPPER void CClouds__MovingFogRender(void) { EAXJMP(0x716C90); }
WRAPPER void CClouds__VolumetricCloudsRender(void) { EAXJMP(0x716380); }
WRAPPER void CClouds__Render(void) { EAXJMP(0x713950); }
WRAPPER void CClouds__RenderBottomFromHeight(void) { EAXJMP(0x7154B0); }
int &CHeli__NumberOfSearchLights = *(int*)0xC1C96C;
short &CTheScripts__NumberOfScriptSearchLights = *(short*)0xA90830;
WRAPPER void CHeli__Pre_SearchLightCone(void) { EAXJMP(0x6C4650); }
WRAPPER void CHeli__RenderAllHeliSearchLights(void) { EAXJMP(0x6C7C50); }
WRAPPER void CTheScripts__RenderAllSearchLights(void) { EAXJMP(0x493E30); }
WRAPPER void CHeli__Post_SearchLightCone(void) { EAXJMP(0x6C46E0); }
WRAPPER void CWeaponEffects__Render(void) { EAXJMP(0x742CF0); }
WRAPPER void CSpecialFX__Render(void) { EAXJMP(0x726AD0); }
void CVehicleRecording__Render(void) {}
WRAPPER void CPointLights__RenderFogEffect(void) { EAXJMP(0x7002D0); }
WRAPPER void CRenderer__RenderFirstPersonVehicle(void) { EAXJMP(0x553D00); }
WRAPPER void CWaterLevel__RenderWater(void) { EAXJMP(0x6EF650); }


int &CMirrors__TypeOfMirror = *(int*)0xC7C724;
bool &CMirrors__bRenderingReflection = *(bool*)0xC7C728;

WRAPPER void CSkidmarks__Render_orig(void) { EAXJMP(0x720640); }
void CSkidmarks__Render(void)
{
	int alphafunc;
	RwRenderStateGet(rwRENDERSTATEALPHATESTFUNCTION, &alphafunc);
	RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTION, (void*)rwALPHATESTFUNCTIONALWAYS);
	CSkidmarks__Render_orig();
	RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTION, (void*)alphafunc);
}

void
RenderScene(void)
{
	int underwater = CWeather::UnderWaterness > 0.0f;
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, 0);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, 0);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, 0);

	if(CMirrors__TypeOfMirror == 0){
		CMovingThings__Render_BeforeClouds();
		CClouds__Render();
	}

	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)1);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)1);
	RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rwSHADEMODEGOURAUD);

	CRenderer::SortOutVisibleEntities();

	// CCarFXRenderer::PreRenderUpdate();	// not used in VCSPC
	CRenderer::RenderRoads();
	CCoronas::RenderReflections();

	// We add objects to the sorted lists in SortOutVisibleEntities() now
	// CRenderer::RenderEverythingBarRoads();
	// rendering done by...
	CRenderer::RenderEverythingBarRoadsOpaque();

	g_breakMan.Render(false);
	CRenderer::RenderFadingInUnderwaterEntities();
	if(!underwater){
		RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLNONE);
		CWaterLevel__RenderWater();
		RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLBACK);
	}
	CRenderer::RenderFadingInEntities();

	if(!CMirrors__bRenderingReflection){
		float nearclip = RwCameraGetNearClipPlane(Scene.camera);
		float z = TheCamera.Cams[TheCamera.ActiveCam].Front.z;
		if(z < 0.0)
			z = -z;
		else
			z = 0.0f;
		// whatever this does exactly....
		float zbias = ((2.0/0x1000000 * 0.25 - 2.0/0x1000000) * z + 2.0/0x1000000) * (RwCameraGetFarClipPlane(Scene.camera) - nearclip);
		RwCameraEndUpdate(Scene.camera);
		RwCameraSetNearClipPlane(Scene.camera, nearclip + zbias);
		RwCameraBeginUpdate(Scene.camera);
		CShadows::UpdateStaticShadows();
		CShadows::RenderStaticShadows();
		CShadows::RenderStoredShadows();
		RwCameraEndUpdate(Scene.camera);
		RwCameraSetNearClipPlane(Scene.camera, nearclip);
		RwCameraBeginUpdate(Scene.camera);
	}

	g_breakMan.Render(true);
	// CPlantMgr::Render();	// not used in VCSPC
	RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLNONE);

	if(CMirrors__TypeOfMirror == 0){
		CClouds__RenderBottomFromHeight();
		CWeather::RenderRainStreaks();
		CCoronas::RenderSunReflection();
	}

	if(underwater){
		RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLNONE);
		CWaterLevel__RenderWater();
		RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLBACK);
	}
	// stencilShadows2();	// not used in VCSPC

	CarPipe::RenderEnvTex();
}

void
RenderReflectionScene(void)
{
	CRenderer::RenderAllBuildingsOpaque();
	CRenderer::RenderAllBuildingsTransparent();
}

void
RenderEffects(void)
{
	CSpecialFX__Render();	// has to be called before coronas because 3d markers want to draw them
	CBirds__Render();
	CSkidmarks__Render();
	CRopes__Render();
	CGlass__Render();
	CMovingThings__Render();
//	CVisibilityPlugins::RenderReallyDrawLastObjects();	// no "grasshouse" in VCSPC
	CCoronas::RenderBuffered();
	Fx_c__Render(TheCamera.m_pRwCamera, 0);
	CWaterCannons__Render();
	CWaterLevel__RenderWaterFog();
	CClouds__MovingFogRender();
	CClouds__VolumetricCloudsRender();
	if(CHeli__NumberOfSearchLights || CTheScripts__NumberOfScriptSearchLights){
		CHeli__Pre_SearchLightCone();
		CHeli__RenderAllHeliSearchLights();
		CTheScripts__RenderAllSearchLights();
		CHeli__Post_SearchLightCone();
	}
	CWeaponEffects__Render();
	// CSpecialFX__Render was here
	CVehicleRecording__Render();
	CPointLights__RenderFogEffect();
	CRenderer__RenderFirstPersonVehicle();
	CPostEffects::Render();
}

void __declspec(naked) LightningSky()
{
	_asm {
		push	eax
		push	eax
		push	eax
		push	eax
		push	eax
		mov	CTimeCycle::m_CurrentColours.skytopr, ax
		mov	CTimeCycle::m_CurrentColours.skytopg, ax
		mov	CTimeCycle::m_CurrentColours.skytopb, ax
		// this is what the game actually uses:
		mov	ds:0xb7c4c4, ax
		mov	ds:0xb7c4c6, ax
		mov	ds:0xb7c4c8, ax
//		this doesn't work:
//		mov	CTimeCycle::m_CurrentColours_exe.skytopr, ax
//		mov	CTimeCycle::m_CurrentColours_exe.skytopg, ax
//		mov	CTimeCycle::m_CurrentColours_exe.skytopb, ax
		push	0x53EA2A
		retn
	}
}

static StaticPatcher	Patcher([](){
//					Memory::Nop(0x53EA1E, 2);	// always lightning sky, for testing purposes

					Memory::InjectHook(0x53EA25, LightningSky, PATCH_JUMP);

					Memory::InjectHook(0x53E997, SetLightsWithTimeOfDayColour);
//					Memory::InjectHook(0x735D90, SetLightColoursForPedsCarsAndObjects, PATCH_JUMP);
					Memory::InjectHook(0x53EAD3, RenderEffects);
					Memory::InjectHook(0x53E170, RenderEffects, PATCH_JUMP);
					Memory::InjectHook(0x53DF40, RenderScene, PATCH_JUMP);
				});
