#ifndef __RADAR
#define __RADAR

class CRadar
{
public:
	static void		ChangeBlipBrightness(int nBlipID, int nBrightness);
	static DWORD	GetRadarTraceColour(int colour, bool bDark, bool bFriend);
};

#endif