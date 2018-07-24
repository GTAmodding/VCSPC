#ifndef __HUD
#define __HUD

#include "General.h"
#include "Ped.h"
#include "Sprite.h"

enum
{
	FLASH_Armourbar = 3,
	FLASH_Healthbar = 4,
	FLASH_Staminabar = 10
};

enum eHudTextures
{
	HUD_Fist,
	HUD_SiteM16,
	HUD_Siterocket,
	HUD_Radardisc,
	HUD_BarInside,
	HUD_BarOutline,
	HUD_Pager,
    HUD_SiteSniper,
    HUD_SiteLaser,
    HUD_LaserDot,
    HUD_Triangle,
    HUD_ViewFinder,
	HUD_NUMBERS,
	NUM_HUD_SPRITES
};


#define	BAR_ENERGY_LOSS_FLASH_DURATION	2000
#define HUD_TRANSPARENCY_BACK			215
#define HUD_TRANSPARENCY_FRONT			(HUD_TRANSPARENCY_BACK+8)
#define RADAR_RANGE                     100.0f

#define HUD_POS_X						-3.0f
#define HUD_POS_Y						-7.0f

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

	static char			*m_pHelpMessage;
	static char			*m_pLastHelpMessage;
	static int			&m_nHelpMessageState;
	static int			&m_nHelpMessageTimer;
	static int			&m_nHelpMessageFadeTimer;
	static char			*m_pHelpMessageToPrint;
	static float		&m_fTextBoxNumLines;
	static char			&m_bHelpMessagePermanent;
	static float		&m_fHelpMessageTime;
	static bool			&m_bHelpMessageQuick;
	static char			*MessageIDString;
	static char			(*m_BigMessage)[128];
	static float		&m_fHelpMessageBoxWidth;

	static char		m_PagerMessage[16];
	//float			BigMessage3PosY;
	static CVector2D m_vecFontScale;

public:
	static CSprite2d Sprites[NUM_HUD_SPRITES];

public:
	static inline void	SetFPSDisplay(bool a) 
					{ bShouldFPSBeDisplayed = a; };;

	static void		DrawBarChart(float fX, float fY, WORD wWidth, WORD wHeight, float fPercentage, BYTE drawBlueLine, BYTE drawPercentage, BYTE drawBorder, CRGBA dwColor, CRGBA dwForeColor);
	static void		DrawWeaponIcon(int playerID, float x, float y, float width, float height);
	static void		DrawWeaponAmmo(int playerID, float x, float y, bool bPS2Behaviour, float offsetCenter, float width, float height);

	static void		GetRidOfAllCustomHUDMessages();
	static void		Initialise();
	static void		ReInitialise();
	static void		Shutdown();
	static void		SetVehicleName(char* carName);
	static void		SetZoneName(char* zoneName, bool bIgnoreIntID);
	static void		SetPagerMessage(char* pMsg);
	static bool		HelpMessageShown();

	static void		SetFontScale(float w, float h);
	static void		PrintString(float x, float y, std::string string, CRGBA color);

	static void		DrawAfterFade();
	static void		DrawHUD();
	static void		DrawPager();
	static void		DrawWanted();
    static void     DrawCrosshairs();
	static void __stdcall	DrawWindowRect(CRect * coords, char * titleKey, char fadeState, CRGBA rgba, int a5, char bDrawBox);
	static void		DrawHelpText();
    static void     DrawTarget(float a1, float a2, int a3, int a4, int a5, int a6, float a7, unsigned __int8 a8, unsigned __int8 a9, unsigned __int8 a10, __int16 a11, float a12, char a13);
	static void		DrawOnscreenTimer();
	static void		DrawPermanentTexts();
	static void		DrawProgressBarWithSprites(float x, float y, float width, float height, float progress, CRGBA color);
	static void		PrintHealthBar(int playerID, float x, float y, float width, float height);
	static void		PrintArmourBar(int playerID, float x, float y, float width, float height);
	static void		PrintStaminaBar(int playerID, float x, float y, float width, float height);
	static void		DrawRadioName(void* object, const char* radioName);
	static void		DrawVehicleName();
	static void		DrawAreaName();
	static void		DrawDevLogos();
	static void		DrawBigMessage(bool bHideOnFade);
	static void		DrawBigMessage1();
	static void		DrawBigMessage2();
	static void		DrawBigMessage3();
    static void	    DrawSubtitles(float x, float y, char* str);
	static void		DrawBarChartWithRoundBorder(float fX, float fY, WORD wWidth, WORD wHeight, float fPercentage, BYTE drawBlueLine, BYTE drawPercentage, BYTE drawBorder, CRGBA dwColor, CRGBA dwForeColor);
	static void		SetHelpMessage(const char *text, bool b1, bool b2, bool b3);
    static void     SetMessage(char* pMsg);
	static float	GetYPosBasedOnHealth(unsigned char plrID, float position, signed char offset);
};

#endif