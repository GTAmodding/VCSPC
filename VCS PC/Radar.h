#ifndef __RADAR
#define __RADAR

#define WIN32_LEAN_AND_MEAN

#define FUNC_CRadar__ChangeBlipBrightness					0x583C70

class CRadar
{
public:
	static void		ChangeBlipBrightness(int nBlipID, int nBrightness);
	static DWORD	GetRadarTraceColour(int colour, bool bDark, bool bFriend);
};

#endif