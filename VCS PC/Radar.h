#ifndef __RADAR
#define __RADAR

#define NUM_BLIP_SPRITES	64

class CRadar
{
private:
	static CSprite2d* const	RadarBlipSprites;		

public:
	static void				ChangeBlipBrightness(int nBlipID, int nBrightness);

	static DWORD			GetRadarTraceColour(int colour, bool bDark, bool bFriend);
	static void				LoadTextures();
};

#endif