#ifndef __HUD
#define __HUD

enum eFlash
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

//#define COMPILE_SLANTED_TEXT

class CHud
{
	friend void Main_Patches();

private:
	bool			bShouldFPSBeDisplayed :	1;
	bool			bLCSPS2Style : 1;
	bool			bShouldGridrefBeDisplayed : 1;
//	bool			bDrawDevLogos : 1;

	int				CarNameStage;
	char*			pCarNameToDisplay;
	char*			pLastCarName;
	char*			pCarName;
	int				CarNameShowingTime;
	int				CarNameFadingTime;

	int				m_ZoneState;
	int				m_ZoneFadeTimer;
	int				m_ZoneNameTimer;
	char*			m_pZoneName;
	char*			m_pLastZoneName;
	char*			m_ZoneToPrint;

	float			BigMessage2Alpha;
	float			BigMessage3Alpha;

	static int&		m_HelpMessageState;

	static char		m_PagerMessage[16];
	//float			BigMessage3PosY;

	static CSprite2d* const		Sprites;

public:
	/*CHud()
	{
		bShouldFPSBeDisplayed = false;
		bLCSPS2Style = false;
		bShouldGridrefBeDisplayed = false;
		bDrawDevLogos = false;
		CarNameStage = 0;
		pCarNameToDisplay = NULL;
		pLastCarName = NULL;
		pCarName = NULL;
		CarNameShowingTime = 0;
		CarNameFadingTime = 0;

		m_ZoneState = 0;
		m_ZoneFadeTimer = 0;
		m_ZoneNameTimer = 0;
		m_pZoneName = NULL;
		m_pLastZoneName = NULL;
		m_ZoneToPrint = NULL;
	};*/

	bool		ShouldDisplayFPS() 
					{ return bShouldFPSBeDisplayed; };
	bool		IsStyleLCS() 
					{ return bLCSPS2Style; }
	bool		ShouldDisplayGridref() 
					{ return bShouldGridrefBeDisplayed; };
	/*bool		ShouldLogosBeDrawn()
					{ return bDrawDevLogos; };*/
	void		SetFPSDisplay(bool a) 
					{ bShouldFPSBeDisplayed = a; };
	void		SetGridrefDisplay(bool a) 
					{ bShouldGridrefBeDisplayed = a; };
	void		SetStyleDisplay(bool a) 
					{ bLCSPS2Style = a; };
	/*void		SetDevLogosDisplay(bool a)
					{ bDrawDevLogos = a; };*/

	static void		DrawBarChart(float fX, float fY, WORD wWidth, WORD wHeight, float fPercentage, BYTE drawBlueLine, BYTE drawPercentage, BYTE drawBorder, CRGBA dwColor, CRGBA dwForeColor);
	static void		DrawWeaponIcon(CPed* pPed, int iX, int iY, float fAlpha);
	
	static void		GetRidOfAllCustomHUDMessages();
	static void		Initialise();
	static void		ReInitialise();
	static void		SetVehicleName(char* carName);
	static void		SetZoneName(char* zoneName, bool bIgnoreIntID);
	static void		SetPagerMessage(char* pMsg);
	static bool		HelpMessageShown();

	static void		DrawHUD();
	static void		DrawOnscreenTimer();
	static void		DrawPermanentTexts();
	static void		PrintHealthForPlayer(int playerID, float posX, float posY);
	static void		PrintArmourForPlayer(int playerID, float posX, float posY);
	static void		PrintBreathForPlayer(int playerID, float posX, float posY);
	static void		DrawWeaponAmmo(CPed* ped, float fX, float fY);
	static void		DrawRadioName(void* object, const char* radioName);
	static void		DrawVehicleName();
	static void		DrawAreaText();
	static void		DrawDevLogos();
	static void		DrawBigMessage(bool bHideOnFade);
	static void		DrawBigMessage1();
	static void		DrawBigMessage2();
	static void		DrawBigMessage3();
	static void		DrawBarChartWithRoundBorder(float fX, float fY, WORD wWidth, WORD wHeight, float fPercentage, BYTE drawBlueLine, BYTE drawPercentage, BYTE drawBorder, CRGBA dwColor, CRGBA dwForeColor);
	static void		DrawSquareBar(float fX, float fY, WORD wWidth, WORD wHeight, float fPercentage, BYTE drawBlueLine, BYTE drawShadow, BYTE drawBorder, CRGBA dwColour, CRGBA dwForeColor);
	static float	GetScreenCoordsForPlayerItem(BYTE plrID, float position, BYTE offset);
};

extern CHud		hud;

#endif