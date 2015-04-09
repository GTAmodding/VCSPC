#ifndef __HUD
#define __HUD

#include "General.h"
#include "Ped.h"
#include "Sprite.h"

enum
{
	FLASH_Armourbar = 3,
	FLASH_Healthbar = 4,
	FLASH_Breathbar = 10
};

enum eHudTextures
{
	HUD_Fist,
	HUD_SiteM16,
	HUD_Siterocket,
	HUD_Radardisc,
	HUD_BarOutline,
	HUD_Pager,
	NUM_HUD_SPRITES
};


#define	BAR_ENERGY_LOSS_FLASH_DURATION	2000
#define HUD_TRANSPARENCY				217

#define HUD_POS_X						148.5f
#define HUD_POS_Y						38.0f

// For 2p HUD
#define HUD_POS_Y_2P_OFFSET				100.0f

//#define COMPILE_SLANTED_TEXT
//#define DRAW_OVERSCAN_720P

class CHud
{
	friend void Main_Patches();

private:
	static bool			bShouldFPSBeDisplayed;
	static bool			bLCSPS2Style;

	static int			CarNameStage;
	static const char*	pCarNameToDisplay;
	static const char*	pLastCarName;
	static const char*	pCarName;
	static int			CarNameShowingTime;
	static int			CarNameFadingTime;

	static int			m_ZoneState;
	static int			m_ZoneFadeTimer;
	static int			m_ZoneNameTimer;
	static const char*	m_pZoneName;
	static const char*	m_pLastZoneName;
	static const char*	m_ZoneToPrint;

	static float		BigMessage2Alpha;
	static float		BigMessage3Alpha;

	static int&			m_HelpMessageState;
	static short&		m_ItemToFlash;

	static char		m_PagerMessage[16];
	//float			BigMessage3PosY;

	static CSprite2d* const		Sprites;

private:
	static void		PrintHealthForPlayer(int playerID, float posX, float posY);
	static void		PrintArmourForPlayer(int playerID, float posX, float posY);
	static void		PrintBreathForPlayer(int playerID, float posX, float posY);
	static void		DrawWeaponAmmo(CPed* ped, float fX, float fY);

public:
	static inline void	SetFPSDisplay(bool a) 
					{ bShouldFPSBeDisplayed = a; };;

	static void		DrawBarChart(float fX, float fY, WORD wWidth, WORD wHeight, float fPercentage, BYTE drawBlueLine, BYTE drawPercentage, BYTE drawBorder, CRGBA dwColor, CRGBA dwForeColor);
	static void		DrawWeaponIcon(CPed* pPed, int iX, int iY, float fAlpha=255.0f);
	
	static void		GetRidOfAllCustomHUDMessages();
	static void		Initialise();
	static void		ReInitialise();
	static void		Shutdown();
	static void		SetVehicleName(char* carName);
	static void		SetZoneName(char* zoneName, bool bIgnoreIntID);
	static void		SetPagerMessage(char* pMsg);
	static bool		HelpMessageShown();

	static void		DrawHUD();
	static void		DrawWanted();
	static void		DrawOnscreenTimer();
	static void		DrawPermanentTexts();
	static void		DrawRadioName(void* object, const char* radioName);
	static void		DrawVehicleName();
	static void		DrawAreaName();
	static void		DrawDevLogos();
	static void		DrawBigMessage(bool bHideOnFade);
	static void		DrawBigMessage1();
	static void		DrawBigMessage2();
	static void		DrawBigMessage3();
	static void		DrawBarChartWithRoundBorder(float fX, float fY, WORD wWidth, WORD wHeight, float fPercentage, BYTE drawBlueLine, BYTE drawPercentage, BYTE drawBorder, CRGBA dwColor, CRGBA dwForeColor);
	static void		DrawSquareBar(float fX, float fY, WORD wWidth, WORD wHeight, float fPercentage, BYTE drawBlueLine, BYTE drawShadow, BYTE drawBorder, CRGBA dwColour, CRGBA dwForeColor);
	static float	GetYPosBasedOnHealth(unsigned char plrID, float position, signed char offset);
};

#endif