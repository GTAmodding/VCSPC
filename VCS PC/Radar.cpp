#include "StdAfx.h"
#include "Radar.h"

#include "PlayerInfo.h"
#include "World.h"

CSprite2d* const	CRadar::RadarBlipSprites = (CSprite2d*)0xBAA250;

WRAPPER void CRadar::Initialise(void) { EAXJMP(0x587FB0); }
WRAPPER void CRadar::ChangeBlipBrightness(int nBlipID, int nBrightness) { WRAPARG(nBlipID); WRAPARG(nBrightness); EAXJMP(0x583C70); }
WRAPPER void CRadar::DrawRadarSection(int nX, int nY) { WRAPARG(nX); WRAPARG(nY); EAXJMP(0x586110); }

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

static StaticPatcher	Patcher([](){ 
						//Memory::InjectHook(0x586F34, BlipAlphaHook, PATCH_JUMP); 
									});