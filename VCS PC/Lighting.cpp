#include "StdAfx.h"
#include "Lighting.h"

#include "Coronas.h"
#include "TimeCycle.h"
#include "Weather.h"
#include "Frontend.h"

RpLight *&pAmbient = *(RpLight**)0xC886E8;
RpLight *&pDirect = *(RpLight**)0xC886EC;
RpLight **pExtraDirectionals = (RpLight**)0xC886F0;	// [6]
int &NumExtraDirLightsInWorld = *(int*)0xC88708;
RwRGBAReal &DirectionalLightColourForFrame = *(RwRGBAReal*)0xC886B4;
RwRGBAReal &AmbientLightColourForFrame_PedsCarsAndObjects = *(RwRGBAReal*)0xC886C4;
RwRGBAReal &AmbientLightColourForFrame = *(RwRGBAReal*)0xC886D4;

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

RwRGBAReal &ambientColor = *(RwRGBAReal*)0xC886A4;
RwRGBAReal &directColor = *(RwRGBAReal*)0xC88694;

//void
//SetLightColoursForPedsCarsAndObjects(float f)
//{
//	directColor.red = DirectionalLightColourForFrame.red * f;
//	directColor.green = DirectionalLightColourForFrame.green * f;
//	directColor.blue = DirectionalLightColourForFrame.blue * f;
//	ambientColor.red = CTimeCycle::m_BrightnessAddedToAmbientRed + AmbientLightColourForFrame_PedsCarsAndObjects.red * f;
//	ambientColor.green = CTimeCycle::m_BrightnessAddedToAmbientGreen + AmbientLightColourForFrame_PedsCarsAndObjects.green * f;
//	ambientColor.blue = CTimeCycle::m_BrightnessAddedToAmbientBlue + AmbientLightColourForFrame_PedsCarsAndObjects.blue * f;
//	RpLightSetColor(pAmbient, &ambientColor);
//	RpLightSetColor(pDirect, &directColor);
//}

static StaticPatcher	Patcher([](){
					Memory::InjectHook(0x53E997, SetLightsWithTimeOfDayColour);
//					Memory::InjectHook(0x735D90, SetLightColoursForPedsCarsAndObjects, PATCH_JUMP);
				});
