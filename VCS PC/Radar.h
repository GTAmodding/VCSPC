#ifndef __RADAR
#define __RADAR

#define NUM_BLIP_SPRITES	64

class CRadar
{
private:
	static CSprite2d* const	RadarBlipSprites;		

public:
	static void				ChangeBlipBrightness(int nBlipID, int nBrightness);
	static void				DrawRadarSection(int nX, int nY);

	static DWORD			GetRadarTraceColour(int colour, bool bDark, bool bFriend);
	static void				LoadTextures();

	// 3D radar test
	static void				Draw3DRadar(int nX, int nY);
	static void				TransformRadarPointToScreenSpace(CVector2D& vecOut, const CVector2D& vecIn);
};

#endif