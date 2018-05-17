#include "StdAfx.h"
#include "Clouds.h"

#include "Camera.h"
#include "Sprite.h"
#include "TimeCycle.h"

WRAPPER void CClouds::Init(void) { EAXJMP(0x7138D0); }
WRAPPER void CClouds::Update(void) { EAXJMP(0x712FF0); }
WRAPPER void CClouds::MovingFogRender(void) { EAXJMP(0x716C90); }
WRAPPER void CClouds::VolumetricCloudsRender(void) { EAXJMP(0x716380); }
WRAPPER void CClouds::Render(void) { EAXJMP(0x713950); }
WRAPPER void CClouds::RenderBottomFromHeight(void) { EAXJMP(0x7154B0); }
WRAPPER void CClouds::RenderSkyPolys(void) { EAXJMP(0x714650); }

float CClouds::ms_cameraRoll;
float CClouds::ms_HorizonTilt;
float CClouds::ms_horizonZ;
CRGBA CClouds::ms_colourBottom;
CRGBA CClouds::ms_colourTop;
CRGBA CClouds::ms_colourBkGrd;

bool
UseDarkBackground(void)
{
	return TheCamera.GetMatrix()->GetUp()->z < -0.9f;
}

void
CClouds::RenderBackground(uint16 topr, uint16 topg, uint16 topb, uint16 botr, uint16 botg, uint16 botb, uint8 alpha)
{
	int width = RsGlobal.MaximumWidth;
	int height = RsGlobal.MaximumHeight;

	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, FALSE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, FALSE);

	CVector right = CrossProduct(*TheCamera.GetMatrix()->GetAt(), *TheCamera.GetMatrix()->GetUp());
	right.Normalise();
	float c = sqrt(right.x*right.x + right.y*right.y);
	if(c > 1.0f) c = 1.0f;
	ms_cameraRoll = right.z < 0.0f ? -acos(c) : acos(c);
	ms_HorizonTilt = tan(ms_cameraRoll) * height/2;

	// clear background, this is done somewhere else in VCS but whatever
	CRGBA clearcol(32, 32, 32, 255);
	CSprite2d::DrawRect(CRect(0, 0, width, height), clearcol, clearcol, clearcol, clearcol);

	if(UseDarkBackground()){
		ms_colourTop.r = 50;
		ms_colourTop.g = 50;
		ms_colourTop.b = 50;
		ms_colourTop.a = 255;
		ms_colourBottom = ms_colourTop;
		CSprite2d::DrawRect(CRect(0, 0, width, height), ms_colourBottom, ms_colourBottom, ms_colourTop, ms_colourTop);
	}else{
		ms_horizonZ = CSprite::CalcHorizonCoors();

		// the gradient
		ms_colourTop.r = topr;
		ms_colourTop.g = topg;
		ms_colourTop.b = topb;
		ms_colourTop.a = alpha;
		ms_colourBottom.r = botr;
		ms_colourBottom.g = botg;
		ms_colourBottom.b = botb;
		ms_colourBottom.a = alpha;
		float botleft = ms_horizonZ + ms_HorizonTilt;
		float botright = ms_horizonZ - ms_HorizonTilt;
		float topleft = botleft - height/2;
		float topright = botright - height/2;
		CSprite2d::DrawAnyRect(
			0.0f, topleft,
			width, topright,
			0.0f, botleft,
			width, botright,
			ms_colourTop, ms_colourTop, ms_colourBottom, ms_colourBottom);

		float midr = (topr + 2 * botr) / 3;
		float midg = (topg + 2 * botg) / 3;
		float midb = (topb + 2 * botb) / 3;

		// the small strip at the bottom of the gradient
		ms_colourTop.r = midr;
		ms_colourTop.g = midg;
		ms_colourTop.b = midb;
		ms_colourTop.a = alpha;
		topleft = ms_horizonZ + ms_HorizonTilt;
		topright = ms_horizonZ - ms_HorizonTilt;
		botleft = topleft + 4.0f;
		botright = topright + 4.0f;
		CSprite2d::DrawAnyRect(
			0.0f, topleft,
			width, topright,
			0.0f, botleft,
			width, botright,
			ms_colourTop, ms_colourTop, ms_colourTop, ms_colourTop);

		// pure top
		if(ms_horizonZ + ms_HorizonTilt > 0.0f || ms_horizonZ - ms_HorizonTilt >= 0.0f){
			// top visible
			ms_colourTop.r = topr;
			ms_colourTop.g = topg;
			ms_colourTop.b = topb;
			ms_colourTop.a = alpha;

			if(ms_horizonZ - abs(ms_HorizonTilt) - height/2 < height){
				// gradient and top visible
				botleft = ms_horizonZ + ms_HorizonTilt - height/2;
				botright = ms_horizonZ - ms_HorizonTilt - height/2;
				// these are calculated too complicated in VC
				topleft = 0.0;
				topright = 0.0;
			}else{
				// only top visible
				topleft = 0.0f;
				topright = 0.0f;
				botleft = height;
				botright = height;
			}
			CSprite2d::DrawAnyRect(
				0.0f, topleft,
				width, topright,
				0.0f, botleft,
				width, botright,
				ms_colourTop, ms_colourTop, ms_colourTop, ms_colourTop);
		}
		ms_colourTop.r = midr;
		ms_colourTop.g = midg;
		ms_colourTop.b = midb;
		ms_colourBottom.r = midr;
		ms_colourBottom.g = midg;
		ms_colourBottom.b = midb;
	}

	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);
}

void
CClouds::RenderHorizon(void)
{
	if(UseDarkBackground())
		return;

	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, FALSE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, FALSE);

	// ms_colourBottom and ms_colourTop are set to the mid/fog colour here
	int width = RsGlobal.MaximumWidth;
	int height = RsGlobal.MaximumHeight;

	// VCS has half the colors, probably because of PS2 colour space conversion
	// so always convert in these cases

	// small strip gradient
	ms_colourBottom.r /= 2;
	ms_colourBottom.g /= 2;
	ms_colourBottom.b /= 2;
	ms_colourBottom.a = 230;
	ms_colourTop.r /= 2;
	ms_colourTop.g /= 2;
	ms_colourTop.b /= 2;
	ms_colourTop.a = 80;
	float topleft = ms_horizonZ + ms_HorizonTilt;
	float topright = ms_horizonZ - ms_HorizonTilt;
	float botleft = topleft + 4.0f;
	float botright = topright + 4.0f;
	CSprite2d::DrawAnyRect(
		0.0f, topleft,
		width, topright,
		0.0f, botleft,
		width, botright,
		ms_colourTop, ms_colourTop, ms_colourBottom, ms_colourBottom);

	// gradient from bottom to background
	ms_colourBkGrd.r = 100/2;
	ms_colourBkGrd.g = 100/2;
	ms_colourBkGrd.b = 100/2;
	ms_colourBkGrd.a = 0;
	topleft = botleft;
	topright = botright;
	botleft = topleft + height/448.0f*36.0f;
	botright = topright + height/448.0f*36.0f;
	CSprite2d::DrawAnyRect(
		0.0f, topleft,
		width, topright,
		0.0f, botleft,
		width, botright,
		ms_colourBottom, ms_colourBottom, ms_colourBkGrd, ms_colourBkGrd);

	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);
}
