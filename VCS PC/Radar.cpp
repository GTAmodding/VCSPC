#include "StdAfx.h"

CSprite2d* const	CRadar::RadarBlipSprites = (CSprite2d*)0xBAA250;

WRAPPER void CRadar::ChangeBlipBrightness(int nBlipID, int nBrightness) { WRAPARG(nBlipID); WRAPARG(nBrightness); EAXJMP(0x583C70); }

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

void CRadar::LoadTextures()
{
	CPNGArchive		HudSPTA("models\\hud.spta");
	HudSPTA.SetDirectory("blips");

	for ( int i = 0; i < NUM_BLIP_SPRITES; ++i )
		RadarBlipSprites[i].SetTextureFromSPTA(HudSPTA, RadarBlipSpriteFilenames[i]);

	HudSPTA.CloseArchive();
}