#include "StdAfx.h"
#include "gtamain.h"

#include "Coronas.h"
#include "TimeCycle.h"
#include "Weather.h"
#include "Clouds.h"
#include "Frontend.h"
#include "PostEffects.h"
#include "Renderer.h"
#include "VisibilityPlugins.h"
#include "Breakable.h"
#include "Camera.h"
#include "Shadows.h"
#include "Antennas.h"
#include "Rubbish.h"
#include "NeoCarpipe.h"
#include "WaterLevel.h"
#include "Font.h"
#include "Hud.h"
#include "Timer.h"
#include "Game.h"
#include "Audio.h"
#include "World.h"
#include "Script.h"
#include "RealTimeShadowMgr.h"
#include "misc.h"
#include "debugmenu_public.h"
#include "Text.h"
#include "Pad.h"

#define USESASKY

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

WRAPPER void LoadingScreen(const char *str1, const char *str2, const char *unused) { EAXJMP(0x53DED0); }

void (*DebugMenuProcess)(void);
void (*DebugMenuRender)(void);
static void stub(void) { }

bool hide2Dstuff = false;
bool bAirBreak = false;

short EnableAirBreak() { return bAirBreak = bAirBreak == false; }

void
DebugMenuInit(void)
{
	if(DebugMenuLoad()){
		DebugMenuProcess = (void(*)(void))GetProcAddress(gDebugMenuAPI.module, "DebugMenuProcess");
		DebugMenuRender = (void(*)(void))GetProcAddress(gDebugMenuAPI.module, "DebugMenuRender");
	}
	if(DebugMenuProcess == NULL || DebugMenuRender == NULL){
		DebugMenuProcess = stub;
		DebugMenuRender = stub;
	}

}

void
DefinedState(void)
{
	RwRenderStateSet(rwRENDERSTATETEXTUREADDRESS, (void*)rwTEXTUREADDRESSWRAP);
	RwRenderStateSet(rwRENDERSTATETEXTUREPERSPECTIVE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rwSHADEMODEGOURAUD);
	RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERLINEAR);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)FALSE);
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEBORDERCOLOR, (void*)RWRGBALONG(0, 0, 0, 255));
	RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)FALSE);
	RwRenderStateSet(rwRENDERSTATEFOGCOLOR, (void*)RWRGBALONG(
		CTimeCycle::GetSkyBottomRed(),
		CTimeCycle::GetSkyBottomGreen(),
		CTimeCycle::GetSkyBottomBlue(), 255));
	RwRenderStateSet(rwRENDERSTATEFOGTYPE, (void*)rwFOGTYPELINEAR);
	RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLNONE);
	RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTION, (void*)rwALPHATESTFUNCTIONGREATER);
	RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, (void*)2);
}

static Reversed SetLightsWithTimeOfDayColour_kill(0x7354E0, 0x73571F);
void
SetLightsWithTimeOfDayColour(RpWorld*)
{
	if(pAmbient){
		AmbientLightColourForFrame.red = CTimeCycle::GetAmbientRed() * CCoronas::LightsMult;
		AmbientLightColourForFrame.green = CTimeCycle::GetAmbientGreen() * CCoronas::LightsMult;
		AmbientLightColourForFrame.blue = CTimeCycle::GetAmbientBlue() * CCoronas::LightsMult;
		AmbientLightColourForFrame_PedsCarsAndObjects.red = CTimeCycle::GetAmbientRed_Obj() * CCoronas::LightsMult;
		AmbientLightColourForFrame_PedsCarsAndObjects.green = CTimeCycle::GetAmbientGreen_Obj() * CCoronas::LightsMult;
		AmbientLightColourForFrame_PedsCarsAndObjects.blue = CTimeCycle::GetAmbientBlue_Obj() * CCoronas::LightsMult;
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
		DirectionalLightColourForFrame.red = CTimeCycle::GetDirectionalRed() * CCoronas::LightsMult;
		DirectionalLightColourForFrame.green = CTimeCycle::GetDirectionalGreen() * CCoronas::LightsMult;
		DirectionalLightColourForFrame.blue = CTimeCycle::GetDirectionalBlue() * CCoronas::LightsMult;
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


WRAPPER void CSkidmarks__Render_orig(void) { EAXJMP(0x720640); }
void CSkidmarks__Render(void)
{
	int alphafunc;
	RwRenderStateGet(rwRENDERSTATEALPHATESTFUNCTION, &alphafunc);
	RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTION, (void*)rwALPHATESTFUNCTIONALWAYS);
	CSkidmarks__Render_orig();
	RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTION, (void*)alphafunc);
}

// int renderTransparent = 1;
// int renderTwoPasses = 0;

static Reversed RenderScene_kill(0x53DF40, 0x53E15F);
void
RenderScene(void)
{
	int underwater = CWeather::UnderWaterness > 0.0f;
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, 0);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, 0);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, 0);

	if(CMirrors::TypeOfMirror == 0){
		CMovingThings__Render_BeforeClouds();
		CClouds::Render();
#ifndef USESASKY
		CClouds::RenderHorizon();	// for old background
#endif
	}

	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)1);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)1);
	RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rwSHADEMODEGOURAUD);

	CRenderer::SortOutVisibleEntities();

	CRenderer::bRenderOpaque = true;
	CRenderer::bRenderTransparent = true;

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
		CWaterLevel::RenderWater();
		RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLBACK);
	}
	CRenderer::RenderFadingInEntities();
//return;

	if(!CMirrors::bRenderingReflection){
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

	if(CMirrors::TypeOfMirror == 0){
		CClouds::RenderBottomFromHeight();
		CWeather::RenderRainStreaks();
		CCoronas::RenderSunReflection();
	}

	if(underwater){
		RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLNONE);
		CWaterLevel::RenderWater();
		RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLBACK);
	}
	// stencilShadows2();	// not used in VCSPC

	CarPipe::RenderEnvTex();
}

void
RenderEnvScene(void)
{
	// TODO: maybe set up some render states here?
	if(CWeather::LightningFlash)
		CClouds::RenderBackground(255, 255, 255,
				255, 255, 255,
				255);
	else
		CClouds::RenderBackground(
				CTimeCycle::GetSkyTopRed(),
				CTimeCycle::GetSkyTopGreen(),
				CTimeCycle::GetSkyTopBlue(),
				CTimeCycle::GetSkyBottomRed(),
				CTimeCycle::GetSkyBottomGreen(),
				CTimeCycle::GetSkyBottomBlue(),
				255);
	CClouds::RenderHorizon();
	CRenderer::RenderAllBuildingsOpaque();
	CRenderer::RenderAllBuildingsTransparent();
}

void
RenderEnvSceneNeo(void)
{
	CRenderer::RenderAllBuildingsOpaque();
	CRenderer::RenderAllBuildingsTransparent();
}

static Reversed RenderEffects_kill(0x53E170, 0x53E22F);
void
RenderEffects(void)
{
	CSpecialFX__Render();	// has to be called before coronas because 3d markers want to draw them
	CBirds__Render();
	CSkidmarks__Render();
	CRopes__Render();
	CAntennas::Render();
	CRubbish::Render();
	CGlass__Render();
	CMovingThings__Render();
//	CVisibilityPlugins::RenderReallyDrawLastObjects();	// no "grasshouse" in VCSPC
	CCoronas::RenderBuffered();
	Fx_c__Render(TheCamera.m_pRwCamera, 0);
	CWaterCannons__Render();
	CWaterLevel::RenderWaterFog();
	CClouds::MovingFogRender();
	CClouds::VolumetricCloudsRender();
	if(CHeli::NumberOfSearchLights || CTheScripts::NumberOfScriptSearchLights){
		CHeli::Pre_SearchLightCone();
		CHeli::RenderAllHeliSearchLights();
		CTheScripts::RenderAllSearchLights();
		CHeli::Post_SearchLightCone();
	}
	CWeaponEffects__Render();
	// no CPlayerPed::DrawTriangleForMouseRecruitPed
	// CSpecialFX__Render was here
	CVehicleRecording__Render();
	CPointLights__RenderFogEffect();
	CRenderer::RenderFirstPersonVehicle();
	CPostEffects::Render();
}

// TODO: reverse this
WRAPPER void
Render2dStuff(void)
{
	EAXJMP(0x53E230);
}

void NewDebugMenuOptions() {
    // Air Break.
    CPed *player = FindPlayerPed(0);

    if (currKeyState->enter || currKeyState->extenter)
        bAirBreak = false;

    if (bAirBreak) {
        float Up = FindPlayerPed(0)->GetCoords().z + 1.0f;
        float Down = FindPlayerPed(0)->GetCoords().z - 1.0f;

        player->bDisableMovement = 1;
        TheCamera.m_bCamDirectlyBehind = true;

       // if (currKeyState->num8) // Forward

        if (currKeyState->num9) // Up
            player->Teleport(FindPlayerPed(0)->GetCoords().x, FindPlayerPed(0)->GetCoords().y, Up, 0);

        if (currKeyState->num3) // Down
            player->Teleport(FindPlayerPed(0)->GetCoords().x, FindPlayerPed(0)->GetCoords().y, Down, 0);
        

        if (currKeyState->num4) // Rotate left
            player->SetHeading(player->GetHeading() + 0.1f);
        else if (currKeyState->num6) // Rotate right
            player->SetHeading(player->GetHeading() - 0.1f);

        CHud::SetMessage("NUMPAD9 - Up, NUMPAD3 - Down, NUMPAD4 - Left, NUMPAD6 - Right, ENTER - Disable AirBreak");
    }
    else {
        player->bDisableMovement = 0;
    }
}

void DrawLoadingText() {
    auto FadeValue = *(int*)0xC3EFAB;
    static float alpha;
    static bool check;
    CFont::SetBackground(0, 0);
    CFont::SetProportional(true);
    CFont::SetFontStyle(FONT_RageItalic);
    CFont::SetOrientation(ALIGN_Right);
    CFont::SetRightJustifyWrap(0.0);
    CFont::SetEdge(0);
    CFont::SetScale(_width(1.3f), _height(2.1f));

    if (!check) {
        if (alpha > 0.0) {
            alpha += CTimer::ms_fTimeScale * 0.01 * -1000.0;
            if (alpha <= 0.0) {
                alpha = 0.0;
                check = true;
            }
        }
        else if (alpha == 0.0) {
            if (alpha != 255.0)
                alpha += CTimer::ms_fTimeScale * 0.01 * 1000.0;
        }
    }
    else if (FadeValue < 10)
        alpha = 0;
    else {
        if (alpha < 255.0) {
            alpha += CTimer::ms_fTimeScale * 0.01 * 1000.0;
            if (alpha >= 255.0) {
                alpha = 255.0;
                check = false;
            }
        }
    }

    CFont::SetDropColor(CRGBA(0, 0, 0, static_cast<float>(alpha)));
    CFont::SetColor(CRGBA(MENU_PINK_R, MENU_PINK_G, MENU_PINK_B, static_cast<float>(alpha)));

    if (!TheCamera.GetFading() && !FrontEndMenuManager.m_bMenuActive) {
        if (FadeValue > 10)
            CFont::PrintString(_x(40.0), _y(11.0f), TheText.Get("LOADCOL"));
    }
}

void RenderFontBuffer(void)
{
	CFont::RenderFontBuffer();
}

void
Render2dStuffAfterFade(void)
{
	CHud::DrawAfterFade();
    // DrawLoadingText(); // Removed because of Ash_735 :(
	CMessages__Display(0);
	RenderFontBuffer();
	// CCredits__Render();	// already rendered in Idle
}

WRAPPER void DoFade(void) { EAXJMP(0x53E600); }

RwRGBA &gColourTop = *(RwRGBA*)0xB72CA0 ;


static Reversed DoRWStuffStartOfFrame_Horizon_kill(0x53D7A0, 0x53D82F);
bool
DoRWStuffStartOfFrame_Horizon(uint16 topr, uint16 topg, uint16 topb, uint16 botr, uint16 botg, uint16 botb, uint16 alpha)
{
	CDraw::CalculateAspectRatio();
	CameraSize(Scene.camera, NULL, tan(CDraw::ms_fFOV * M_PI / 360.0f), CDraw::ms_fAspectRatio);
	CVisibilityPlugins::SetRenderWareCamera(Scene.camera);
	RwCameraClear(Scene.camera, &gColourTop, rwCAMERACLEARZ | rwCAMERACLEARSTENCIL);	// rwCAMERACLEARSTENCIL not needed since we're no longer doing shadows
	if(!RsCameraBeginUpdate(Scene.camera))
		return false;
	DefinedState();
	TheCamera.m_viewMatrix.Update();

//	Doesn't look too good from far up
#ifndef USESASKY
	CClouds::RenderBackground(topr, topg, topb, botr, botg, botb, alpha);	// old gradient
#else
	// Make sure we get the lightning flash right
	CTimeCycle::m_CurrentColours_exe.skytopr = topr;
	CTimeCycle::m_CurrentColours_exe.skytopg = topg;
	CTimeCycle::m_CurrentColours_exe.skytopb = topb;
	CTimeCycle::m_CurrentColours_exe.skybotr = botr;
	CTimeCycle::m_CurrentColours_exe.skybotg = botg;
	CTimeCycle::m_CurrentColours_exe.skybotb = botb;
	// Is this even used?
	CTimeCycle::m_CurrentColours.skytopr = topr;
	CTimeCycle::m_CurrentColours.skytopg = topg;
	CTimeCycle::m_CurrentColours.skytopb = topb;
	CTimeCycle::m_CurrentColours.skybotr = botr;
	CTimeCycle::m_CurrentColours.skybotg = botg;
	CTimeCycle::m_CurrentColours.skybotb = botb;
	CClouds::RenderSkyPolys();
#endif

	return true;
}

static Reversed DoRWStuffEndOfFrame_kill(0x53D840, 0x53D86F);
void
DoRWStuffEndOfFrame(void)
{
	// CDebug::DebugDisplayTextBuffer();
	// FlushObrsPrintfs();

	RwCameraEndUpdate(Scene.camera);
	RsCameraShowRaster(Scene.camera);
}

void
RenderMenus(void)
{
	if(FrontEndMenuManager.m_bMenuActive)
		FrontEndMenuManager.DrawFrontEnd();
}

void
RenderDebugShit(void)
{
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, FALSE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, FALSE);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEFOGENABLE, FALSE);
	RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLNONE);

	DebugMenuRender();
}

static Reversed Idle_kill(0x53E920, 0x53EC0F);
void
Idle(void *arg)
{
	// Why aren't we doing this?
//	static uint32 lastTime;
//	while(CTimer::GetCurrentTimeInMilliseconds() - lastTime < 14);
//	lastTime = CTimer::GetCurrentTimeInMilliseconds();

	CTimer::Update();
	CSprite2d::InitPerFrame();
	CFont::InitPerFrame();
	CPointLights__NumLights = 0;
	CGame::Process();
	AudioEngine.Service();
	SetLightsWithTimeOfDayColour(Scene.world);

	if(arg == NULL)
		return;

	if(!FrontEndMenuManager.m_bMenuActive && TheCamera.GetFadeStage() != 2){
		RwV2d pos;
		pos.x = RsGlobal.MaximumWidth / 2.0f;
		pos.y = RsGlobal.MaximumHeight / 2.0f;
		RsMouseSetPos(&pos);

		CRenderer::ConstructRenderList();
		CRenderer::PreRender();
		CWorld::ProcessPedsAfterPreRender();
		g_realTimeShadowMan.Update();
		CMirrors::BeforeMainRender();

		// Do this BEFORE DoRWStuffStartOfFrame
		RwCameraSetFarClipPlane(Scene.camera, CTimeCycle::GetFarClip());
		RwCameraSetFogDistance(Scene.camera, CTimeCycle::GetFogStart());

		if(CWeather::LightningFlash){
			// sets fog colour in DefinedState
			CTimeCycle::SetSkyBottomRed(255);
			CTimeCycle::SetSkyBottomGreen(255);
			CTimeCycle::SetSkyBottomBlue(255);
			if(!DoRWStuffStartOfFrame_Horizon(255, 255, 255, 255, 255, 255, 255))
				return;
		}else{
			if(!DoRWStuffStartOfFrame_Horizon(
							CTimeCycle::GetSkyTopRed(),
							CTimeCycle::GetSkyTopGreen(),
							CTimeCycle::GetSkyTopBlue(),
							CTimeCycle::GetSkyBottomRed(),
							CTimeCycle::GetSkyBottomGreen(),
							CTimeCycle::GetSkyBottomBlue(),
							255))
				return;
		}

		DefinedState();

		CMirrors::RenderMirrorBuffer();
		RenderScene();

		CVisibilityPlugins::RenderWeaponPedsForPC();
		CVisibilityPlugins::ResetWeaponPedsForPC();

		RenderEffects();

		// skipping camera motion blur, not used

		RenderDebugShit();

		if(!hide2Dstuff)
			Render2dStuff();


        // New debug menu options.
        NewDebugMenuOptions();

	}else{
		CDraw::CalculateAspectRatio();
		CameraSize(Scene.camera, NULL, tan(CDraw::ms_fFOV * M_PI / 360.0f), CDraw::ms_fAspectRatio);
		CVisibilityPlugins::SetRenderWareCamera(Scene.camera);
		RwCameraClear(Scene.camera, &gColourTop, rwCAMERACLEARZ);
		if(!RsCameraBeginUpdate(Scene.camera))
			return;
	}

	RenderMenus();

	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
	DoFade();

	Render2dStuffAfterFade();
	CCredits::Render();

	DoRWStuffEndOfFrame();
}

static StaticPatcher Patcher([](){
	Memory::InjectHook(0x53ECBD, Idle);
//	Memory::InjectHook(0x735D90, SetLightColoursForPedsCarsAndObjects, PATCH_JUMP);

	if(DebugMenuLoad()){
		DebugMenuAddVarBool8("Misc", "Hide 2D stuff", (int8*)&hide2Dstuff, NULL);
		DebugMenuAddCmd("Misc", "Air Break", []() { EnableAirBreak(); });
//		DebugMenuAddVarBool32("Rendering", "Render Transparent Entities", &renderTransparent, NULL);
//		DebugMenuAddVarBool32("Rendering", "Render Transparent objects in two passes", &renderTwoPasses, NULL);
	}
});
