#ifndef __RADAR
#define __RADAR

#include "General.h"
#include "Sprite.h"

#define NUM_BLIP_SPRITES	64

class CRadar
{
private:
	static CSprite2d* const	RadarBlipSprites;		

public:
	static void				ChangeBlipBrightness(int nBlipID, int nBrightness);
	static void				DrawRadarSection(int nX, int nY);

	static uint32			GetRadarTraceColour(int colour, bool bDark, bool bFriend);
	static void				LoadTextures();

	// 3D radar test
	static void				Draw3DRadar(int nX, int nY);
	static void				TransformRadarPointToScreenSpace(CVector2D& vecOut, const CVector2D& vecIn);
	static void				Set3DVerts(int nVerts, float* pX, float* pY, const CRGBA& rgb);
	static void				Render3D(void*, void*, int nVerts);
};

#endif