#include "StdAfx.h"

void CRadar::ChangeBlipBrightness(int nBlipID, int nBrightness)
{
	DWORD dwFunc = (DWORD)FUNC_CRadar__ChangeBlipBrightness;
	_asm
	{
		push	nBrightness
		push	nBlipID
		call	dwFunc
		add		esp, 8
	}
}

DWORD CRadar::GetRadarTraceColour(int colour, bool bDark, bool bFriend)
{
	switch ( colour )
	{
	case 0:
		{
			if ( bFriend )
				return 0x00007FFF;
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