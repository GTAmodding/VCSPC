#include "StdAfx.h"
#include "Radar.h"
#include "Frontend.h"

#include "PlayerInfo.h"
#include "World.h"
#include "Hud.h"

float &CRadar::m_radarRange = *(float *)0xBA8314;
unsigned short *CRadar::MapLegendList = (unsigned short *)0xBA8318;
unsigned short &CRadar::MapLegendCounter = *(unsigned short *)0xBA86B8;
CRGBA *CRadar::ArrowBlipColour = (CRGBA *)0xBA86D4;
tRadarTrace *CRadar::ms_RadarTrace = (tRadarTrace *)0xBA86F0;
CVector2D &CRadar::vec2DRadarOrigin = *(CVector2D *)0xBAA248;
CSprite2d *CRadar::RadarBlipSprites = (CSprite2d *)0xBAA250;
CRect &CRadar::m_radarRect = *(CRect *)0x8D0920;
unsigned char &CRadar::airstrip_location = *(unsigned char *)0xBA8300;
int &CRadar::airstrip_blip = *(int *)0xBA8304;

WRAPPER void CRadar::Initialise(void) { EAXJMP(0x587FB0); }
WRAPPER void CRadar::ChangeBlipBrightness(int nBlipID, int nBrightness) { WRAPARG(nBlipID); WRAPARG(nBrightness); EAXJMP(0x583C70); }
WRAPPER void CRadar::DrawRadarSection(int nX, int nY) { WRAPARG(nX); WRAPARG(nY); EAXJMP(0x586110); }
WRAPPER void CRadar::InitFrontEndMap() { EAXJMP(0x585960); }
WRAPPER void CRadar::DrawLegend(int x, int y, int blipType) { WRAPARG(x); WRAPARG(y);  WRAPARG(blipType); EAXJMP(0x5828A0); }
WRAPPER void CRadar::LimitToMap(float* pX, float* pY) { WRAPARG(pX); WRAPARG(pY); EAXJMP(0x583350); }
WRAPPER void CRadar::AddBlipToLegendList(unsigned char arg0, int blipArrId) { WRAPARG(arg0); WRAPARG(blipArrId); EAXJMP(0x5859F0); }
WRAPPER void CRadar::DrawRadarSectionMap(int x, int y, CRect rect) { WRAPARG(x); WRAPARG(y); WRAPARG(rect); EAXJMP(0x586520); }
WRAPPER bool CRadar::DisplayThisBlip(int spriteId, char priority) { WRAPARG(spriteId); WRAPARG(priority); EAXJMP(0x583B40); }
WRAPPER void CRadar::TransformRealWorldPointToRadarSpace(CVector2D& out, CVector2D const& in) { WRAPARG(out); WRAPARG(in); EAXJMP(0x583530); }
WRAPPER float CRadar::LimitRadarPoint(CVector2D& point) { WRAPARG(point); EAXJMP(0x5832F0); }
WRAPPER void CRadar::DrawBlips() {EAXJMP(0x588050); }

WRAPPER void CRadar::ClearBlip(int blipIndex) { WRAPARG(blipIndex);  EAXJMP(0x587CE0); }
WRAPPER void CRadar::SetBlipSprite(int blipIndex, int spriteId) { WRAPARG(blipIndex); WRAPARG(spriteId); EAXJMP(0x583D70); }
WRAPPER int CRadar::SetCoordBlip(eBlipType type, float x, float y, float z, int a5, eBlipDisplay display) { WRAPARG(type); WRAPARG(x); WRAPARG(y); WRAPARG(z);  WRAPARG(a5); WRAPARG(display); EAXJMP(0x583D70); }

static const char* const RadarBlipSpriteFilenames[NUM_BLIP_SPRITES] = { "", "", "radar_centre", "arrow", 
																		"radar_north", "", "radar_gun", "radar_bomb",
																		"", "", "", "",
																		"", "", "", "",
																		"", "", "radar_gunshop", "",
																		"", "", "", "",
																		"", "", "", "", 
																		"", "", "", "",
																		"", "", "", "radar_save",
																		"", "", "", "",
																		"", "radar_waypoint", "", "radar_louise",
																		"radar_marty", "radar_cshop", "radar_phil", "radar_jerry",
																		"", "", "", "",
																		"", "", "", "",
																		"", "", "", "",
																		"", "", "", "radar_spray" };

uint32 CRadar::GetRadarTraceColour(int colour, bool bDark, bool bFriend)
{
	switch ( colour )
	{
	case 0:
		{
			if ( bFriend )
				return 0x484dffff;
// what is this? arrow markers should have the above instead of this
//				return 0x00007FFF;
			if ( bDark )
				return 0xFF484DFF;	
			return 0x7F0000FF;
		}
	case 1:
		{
			if ( bDark )
				return 0x5FA06AFF;
			return 0x007F00FF;
		}
	case 2:
		{
			return 0x00007FFF;
		}
	case 3:
		{
			if ( bDark )
				return 0xE1E1E1FF;
			return 0x7F7F7FFF;
		}
	case 4:
		{
			if ( bDark )
				return 0xFFFF00FF;
			return 0x7F7F00FF;
		}
	case 5:
		{
return 0x00FF00FF;
			if ( bDark )
				return 0xFFA5C9FF;
			return 0x6E103CFF;
		}
	case 6:
		{
			return 0x005858FF;
		}
	case 7:
		{
			return 0x6E103CFF;
		}

	/*case 0:
		{
			if ( bDark )
				return 0x712B49FF;
			else
				return 0x7F0000FF;
		}
	case 1:
		{
			if ( bDark )
				return 0x5FA06AFF;
			else
				return 0x007F00FF;
		}
	case 2:
		{
			if ( bDark )
				return 0x80A7F3FF;
			else
				return 0x00007FFF;
		}
	case 3:
		{
			if ( bDark )
				return 0xE1E1E1FF;
			else
				return 0x7F7F7FFF;
		}
	case 4:
		{
			if ( bDark )
				return 0xFFFF00FF;
			else
				return 0x7F7F00FF;
		}
	case 5:
		{
			if ( bDark )
				return 0xFF00FFFF;
			else
				return 0x7F007FFF;
		}
	case 6:
		{
			if ( bDark )
				return 0x00FFFFFF;
			else
				return 0x007F7FFF;
		}*/
	default:
		return colour;
	}
}

void CRadar::LoadTextures()
{
	CPNGArchive		HudSPTA("models\\hud.spta");
	HudSPTA.SetDirectory("blips");

	for ( int i = 0; i < NUM_BLIP_SPRITES; ++i )
		RadarBlipSprites[i].SetTextureFromSPTA(HudSPTA, RadarBlipSpriteFilenames[i]);

	HudSPTA.CloseArchive();
}

static RwCamera*	pRadarCam;

void CRadar::Draw3DRadar(int nX, int nY)
{
	static bool			bCamCreated = false;

	if ( !bCamCreated )
	{
		pRadarCam = RwCameraCreate();
		RwCameraSetFrame(pRadarCam, RwFrameCreate());
		RwCameraSetRaster(pRadarCam, RwRasterCreate(512, 512, 0, rwRASTERTYPECAMERATEXTURE));
		RwCameraSetZRaster(pRadarCam, RwRasterCreate(512, 512, 0, rwRASTERTYPEZBUFFER));

		RwV2d			vecViewWindow = { tan(M_PI/3), tan(M_PI/3) };

		RwCameraSetViewWindow(pRadarCam, &vecViewWindow);
		RwCameraSetProjection(pRadarCam, rwPERSPECTIVE);
		bCamCreated = true;

		// helper matrix
		/*CMatrix			matTemp;
		matTemp.SetRotateYOnly(-M_PI);
		matTemp.SetTranslateOnly(0.0, 0.0, 1.0f);

		memcpy(RwFrameGetMatrix(RwCameraGetFrame(pRadarCam)), &matTemp.matrix, sizeof(RwMatrix));*/
	}

	//RwRGBA		color = { 255, 255, 255, 255 };

	/*RwCameraEndUpdate(Scene);
	RwCameraClear(pRadarCam, &color, rwCAMERACLEARIMAGE);
	RwCameraBeginUpdate(pRadarCam);*/

	//RwRenderStateSet(rwRENDERSTATEZTESTENABLE, false);
	//RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, false);

	DrawRadarSection(nX - 1, nY - 1);
	DrawRadarSection(nX, nY - 1);
	DrawRadarSection(nX + 1, nY - 1);
	DrawRadarSection(nX - 1, nY);
	DrawRadarSection(nX, nY);
	DrawRadarSection(nX + 1, nY);
	DrawRadarSection(nX - 1, nY + 1);
	DrawRadarSection(nX, nY + 1);
	DrawRadarSection(nX + 1, nY + 1);

	//RwCameraEndUpdate(pRadarCam);
	//RwCameraBeginUpdate(Scene);

	/*CSprite2d::SetVertices(CRect(_xleft(35.0f), _ydown(107.0f), _xleft(35.0f +  94.0f), _ydown(107.0f - (94.0f * 448.0f / 480.0f))),
				CRGBA(255, 255, 255, HUD_TRANSPARENCY), CRGBA(255, 255, 255, HUD_TRANSPARENCY), CRGBA(255, 255, 255, HUD_TRANSPARENCY), CRGBA(255, 255, 255, HUD_TRANSPARENCY),
				1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);*/

	//CSprite2d::SetVertices(CRect(_xleft(35.0f), _ydown(107.0f), _xleft(35.0f +  94.0f), _ydown(107.0f - (94.0f * 448.0f / 480.0f))), CRGBA(255, 255, 255, HUD_TRANSPARENCY), CRGBA(255, 255, 255, HUD_TRANSPARENCY), CRGBA(255, 255, 255, HUD_TRANSPARENCY), CRGBA(255, 255, 255, HUD_TRANSPARENCY));

	/*aSpriteVertices[0].rhw = 0.41f;
	aSpriteVertices[1].rhw = 0.41f;
	aSpriteVertices[2].rhw = 1.0f;
	aSpriteVertices[3].rhw = 1.0f;*/

	//RwRenderStateSet(rwRENDERSTATETEXTURERASTER, RwCameraGetRaster(pRadarCam));
	//RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, aSpriteVertices, 4);
}

void CRadar::TransformRadarPointToScreenSpace(CVector2D& vecOut, const CVector2D& vecIn)
{
	//RwV2d			vecViewWindow = { 3.0f, 3.0f };
	//CVector			vecTemp(vecIn.x * 1.0f, -vecIn.y;
	CMatrix			matTemp;
	matTemp.SetRotateXOnly(-M_PI + M_PI/6);
	matTemp.SetTranslateOnly(0.0, sin(M_PI/6), cos(M_PI/6));
	memcpy(RwFrameGetMatrix(RwCameraGetFrame(pRadarCam)), &matTemp.matrix, sizeof(RwMatrix));

	//RwCameraSetViewWindow(pRadarCam, &vecViewWindow);
	//RwCameraSetProjection(pRadarCam, rwPERSPECTIVE);

	CVector vecOutTemp = matTemp * CVector(vecIn.x, vecIn.y);


	//vecOut.x = ((vecOutTemp.x / vecOutTemp.z) + 1.0f) * 256.0f;
	//vecOut.y = ((vecOutTemp.y / vecOutTemp.z) + 1.0f) * 256.0f;
	//vecOut = vecIn;

	vecOut.x = (vecIn.x + 1.0f)/* * 256.0f*/;
	vecOut.y = (vecIn.y + 1.0f)/* * 256.0f*/;
}

void CRadar::Set3DVerts(int nVerts, float* pX, float* pY, const CRGBA& rgb)
{
	RwIm3DVertex			verts[8];

	for ( int i = 0; i < nVerts; i++ )
	{
		RwIm3DVertexSetPos(&verts[i], pX[i], pY[i], 0.0);
		RwIm3DVertexSetRGBA(&verts[i], rgb.r, rgb.g, rgb.b, rgb.a);
	}

	if ( !RwIm3DTransform(verts, nVerts, &CWorld::Players[CWorld::PlayerInFocus].GetPed()->GetMatrix()->matrix/*nullptr*/, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA) )
		assert(!"ERROR!");

}

void CRadar::Render3D(void*, void*, int nVerts)
{
	RwImVertexIndex			indices[8];

	for ( int i = 0; i < nVerts; i++ )
		indices[i] = i;

	RwIm3DRenderIndexedPrimitive(rwPRIMTYPETRILIST, indices, nVerts);
	RwIm3DEnd();
}

static void __declspec(naked)	BlipAlphaHook()
{
	_asm
	{
		movzx		edi, [esp+30h-24h]
		push		edi
		push		586F39h
		retn
	}
}


void __cdecl CRadar::DrawRotatingRadarSprite(CSprite2d *texture, float x, float y, float r_angle, unsigned int width, unsigned int height, CRGBA const& color) {
	if (FrontEndMenuManager.drawRadarOrMap) {
		x = (RsGlobal.MaximumWidth * 0.0015625) * x * 1.33334 / ScreenAspectRatio + _xmiddle(-274.0f);
		y = RsGlobal.MaximumHeight  * 0.002232143 * y;
		CRadar::LimitToMap(&x, &y);
	}

	CVector2D posn[4];

	posn[0].x = x - width / 1.2f;

	posn[0].y = y + height / 1.2f;

	posn[1].x = x + width / 1.2f;

	posn[1].y = y + height / 1.2f;

	posn[2].x = x - width / 1.2f;

	posn[2].y = y - height / 1.2f;

	posn[3].x = x + width / 1.2f;

	posn[3].y = y - height / 1.2f;

	RotateVertices(posn, 4, x, y, r_angle);

	texture->Draw(posn[2].x, posn[2].y,
		posn[3].x, posn[3].y,
		posn[0].x, posn[0].y,
		posn[1].x, posn[1].y,
		CRGBA(255, 255, 255, 255));
}

void __cdecl CRadar::DrawRadarCentre(CSprite2d* sprite, float x, float y, float angle, unsigned int width, unsigned int height, CRGBA color) {
	DrawRotatingRadarSprite(sprite, x, y, angle, _width(7.0f), _width(8.0f), color);
}

void CRadar::DrawRadarSprites(BYTE iconID, float x, float y, unsigned __int8 alpha) {
	float w = _width(8.0f);
	float h = _height(8.0f);

	if (FrontEndMenuManager.drawRadarOrMap) {
		x = (RsGlobal.MaximumWidth * 0.0015625) * x * 1.33334 / ScreenAspectRatio + _xmiddle(-274.0f);
		y = RsGlobal.MaximumHeight  * 0.002232143 * y;
		CRadar::LimitToMap(&x, &y);
	}

	if (CRadar::DisplayThisBlip(iconID, -99)) {
		CRadar::RadarBlipSprites[iconID].Draw(CRect(x - (w), y + (h), x + (w), y - (h)), CRGBA(255, 255, 255, alpha));
		CRadar::AddBlipToLegendList(0, iconID);
	}
}


void CRadar::ShowRadarTraceWithHeight(float x, float y, unsigned int size, unsigned __int8 r, unsigned __int8 g, unsigned __int8 b, unsigned __int8 a, unsigned __int8 type_or_height) {
    if (FrontEndMenuManager.m_bMenuActive) {
        float w = _width(8.0f);
        float h = _height(8.0f);

        if (FrontEndMenuManager.drawRadarOrMap) {
            x = (RsGlobal.MaximumWidth * 0.0015625) * x * 1.33334 / ScreenAspectRatio + _xmiddle(-274.0f);
            y = RsGlobal.MaximumHeight  * 0.002232143 * y;
            CRadar::LimitToMap(&x, &y);
        }
        /* // TODO: object level.
        if (type_or_height == 0) // higher

        else if (type_or_height == 1) // lower

        else if (type_or_height == 2) // level
        */
    }
    else
        ((void(__cdecl*)(float, float, unsigned int, unsigned __int8, unsigned __int8, unsigned __int8, unsigned __int8, unsigned __int8))0x584070)(x, y, size, r, g, b, a, type_or_height);
}

void __fastcall CRadar::DrawRadarCircle(CSprite2d *sprite, int, CRect *rect, CRGBA *color) {
	float x = _xleft(RADAR_POS_X + 32.6f);
	float y = _ydown(RADAR_POS_Y + 120.0f);
	float w = _width(103.0f);
	float h = _height(93.0f);
	CHud::Sprites[HUD_Radardisc].Draw(x, y, w, h, CRGBA(0, 0, 0, HUD_TRANSPARENCY_BACK));
}

void __cdecl CRadar::TransformRadarPointToScreenSpaceVCS(CVector2D *out, CVector2D *in) {
	__asm push edx
	float x = _xleft(RADAR_POS_X + 84.0f);
	float y = _ydown(RADAR_POS_Y + 74.0f);
	float w = _width(90.0f);
	float h = _height(82.0f);
	if (FrontEndMenuManager.drawRadarOrMap) {
		out->x = FrontEndMenuManager.m_fMapZoom * in->x + FrontEndMenuManager.m_fMapBaseX;
		out->y = FrontEndMenuManager.m_fMapBaseY - FrontEndMenuManager.m_fMapZoom * in->y;
	}
	else {
		out->x = (x) + in->x * (w / 2);
		out->y = (y) - in->y * (h / 2);

	}
	__asm pop edx
}

static StaticPatcher	Patcher([](){ 
				//Memory::InjectHook(0x586F34, BlipAlphaHook, PATCH_JUMP); 
                Memory::InjectHook(0x586F3C, CRadar::DrawRadarSprites);
                Memory::InjectHook(0x587BD9, CRadar::DrawRadarSprites);
                Memory::InjectHook(0x588188, CRadar::DrawRadarSprites);

                Memory::InjectHook(0x586FE0, CRadar::ShowRadarTraceWithHeight);
                Memory::InjectHook(0x58736B, CRadar::ShowRadarTraceWithHeight);
                Memory::InjectHook(0x58766E, CRadar::ShowRadarTraceWithHeight);
                Memory::InjectHook(0x587B7C, CRadar::ShowRadarTraceWithHeight);

				//Memory::InjectHook(0x588722, CRadar::DrawRadarCentre);
				Memory::InjectHook(0x583480, CRadar::TransformRadarPointToScreenSpaceVCS, PATCH_JUMP);
                Memory::InjectHook(0x58AA25, CRadar::DrawRadarCircle);
				Memory::Nop(0x58A818, 16);
				Memory::Nop(0x58A8C2, 16);
				Memory::Nop(0x58A96C, 16);
			});