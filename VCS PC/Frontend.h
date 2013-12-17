#ifndef __CMENUMANAGER
#define __CMENUMANAGER

#define						MENU_TEXT_POSITION_LCOLUMN 320.0f	// Adjusted by the game
#define						MENU_TEXT_POSITION_RCOLUMN 70.0f
//#define						MENU_TEXT_POSITION_RCOLUMN 250.0f
#define						MENU_SLIDER_HEIGHT 10.0f
#define						MENU_SLIDER_WIDTH 9.0f

#define						MENU_SLIDER_POSY 135.0f

#define						NUM_MENUS 46				// Relevant for debugging only
#define						NUM_ENTRIES_PER_MENU 14

#define						NUM_SLIDERS 8
//#define					COMPILE_BOUNCING_ICONS
//#define					COMPILE_SMOOTHBEATING_ICONS
#define						COMPILE_BEATING_ICONS
#define						INCLUDE_PROMO_BANNER

#define MENU_INACTIVE_R			0x1E
#define MENU_INACTIVE_G			0xFF
#define MENU_INACTIVE_B			0xFF

#define MENU_ACTIVE_R			0xFF
#define MENU_ACTIVE_G			0xEC
#define MENU_ACTIVE_B			0x93

#define MENU_LOCKED_R			0x06
#define MENU_LOCKED_G			0x33
#define MENU_LOCKED_B			0x33

#define DEBUG_ORANGE_R			0xE7
#define DEBUG_ORANGE_G			0x8D
#define DEBUG_ORANGE_B			0x42

#define MENU_UPDATES_R			0x00
#define MENU_UPDATES_G			0xFF
#define MENU_UPDATES_B			0x7F

#define MENU_PINK_R				0xED
#define MENU_PINK_G				0x82
#define MENU_PINK_B				0xB4

#define MENU_INACTIVE_PINK_R	0x33
#define MENU_INACTIVE_PINK_G	0x1C
#define MENU_INACTIVE_PINK_B	0x27

#define MODDB_RED_R				0xF4
#define MODDB_RED_G				0x34
#define MODDB_RED_B				0x34

#define						MAX_AA				8
#define						SET_FILE_VERSION	7

// Macroes
#define MenuEntriesList (CMenuManager::ms_pMenus)

enum eMenuActions
{
	// TODO: Fill?
	ACTION_UPDATER_BUTTON		= 66,
	ACTION_TOGGLE_DLC,

	NUM_MENU_ACTIONS
};

enum eMenuSpecialFlags
{
	ACTION_NONE,
	ACTION_SAVE_1,
	ACTION_SAVE_2,
	ACTION_SAVE_3,
	ACTION_SAVE_4,
	ACTION_SAVE_5,
	ACTION_SAVE_6,
	ACTION_SAVE_7,
	ACTION_SAVE_8,
	ACTION_SAVE_9,
	ACTION_SAVE_10,
	ACTION_SAVE_11,
	ACTION_SAVE_12,
	ACTION_MISSIONPACK,
	ACTION_JOYMOUSE,
	ACTION_STANDARD,
	ACTION_CLICKORARROWS,
	ACTION_UPDATER,
	ACTION_DLC
};

// For right column hack
union MenuVar
{
	long	nIn;
	float	fOut;
};

struct MenuItem
{
	char		name[8];
	BYTE		prevMenu;
	BYTE		startingMenuEntry;
	struct MenuEntry
	{
		BYTE		action;
		char		entry[8];
		BYTE		specialDescFlag;
		BYTE		targetMenu;
		short		posX;
		short		posY;
		BYTE		align;
		signed char	screenVertAlign		: 4;
		signed char	screenHorAlign		: 4;
	}			entryList[NUM_ENTRIES_PER_MENU];
};

//static_assert(sizeof(MenuItem) == CMenuItem_ARRAYSIZE, "MenuItem has wrong size!");

class CMenuManager
{
	friend			void Main_Patches();
	friend			RwTexture* GetTheTexture();
	friend			class WidescreenSupport;
private:
	bool			bUnkScroll;
	float			fStatsScrollSpeed;
	BYTE			__pad1[25];
	BYTE			hudMode;
	BYTE			__pad2[34];
	bool			m_bSubtitlesEnabled;
	BYTE			__pad3[5];
	bool			m_bLegend;
	BYTE			m_bAspectRatioMode;
	BYTE			frameLimiterMode;
	BYTE			__pad4[2];
	BYTE			sfxVolume;
	BYTE			radioVolume;
	BYTE			radioEq;
	BYTE			radioStation;
	DWORD			currentMenuEntry;
	BYTE			__pad8[4];
	bool			bIsActive;
	BYTE			__pad5[31];
	DWORD			titleLanguage;
	DWORD			textLanguage;
	BYTE			language;
	BYTE			__pad6[114];
	CSprite2d		textures[25];
	BYTE			texturesLoaded;
	BYTE			bCurrentScreen;
	BYTE			bLastScreen;
	BYTE			__pad7[6681];

	// VCS PC class extension
	static int			m_nFocusedDLC, m_nLastFocusedDLC;
	static float		m_fStatsScrollPos;
	static long			ms_nRubberSlider;
	static bool			m_bLastDLCState[NUM_DLC_PACKS];

public:
	static MenuItem		ms_pMenus[];

public:
	BYTE			GetHudMode() 
						{ return hudMode; };

	BYTE			GetLanguage()
						{ return language; };

	bool			IsActive()
						{ return bIsActive; };

	void			SetLanguage(BYTE lang)
						{ language = lang; };

	void			SetTitleLanguage(DWORD lang)
						{ titleLanguage = lang; };

	void			SetTextLanguage(BYTE lang)
						{ textLanguage = lang; };

	void			ShowFullscreenMessage(const char* pMessage, bool bUnk1, bool bUnk2);

	void			DrawBackEnd();
	void			DrawRadioStationIcons();
	int				DrawSliders(float posX, float posY, float, float height, float distBetweenRects, float filledAmount, int width);
	void			DrawLeftColumn(MenuItem::MenuEntry& pPosition, const char* pText, const char* pRightText);
	//float			GetLeftColumnPos_Height(long posY);
	float			GetRightColumnPos(MenuVar& sPosY);
	void			DrawOutroSplash();
	void			PrintStats();
	void			PrintUpdaterScreen();
	void			PrintDLCScreen();
	void			ReadFrontendTextures();
	void			SwitchToNewScreen(signed char bScreen);

	const char*		ProcessDLCSlot(int nSlotID);
	float			GetTextYPos(const MenuItem::MenuEntry& pPosition);

	// Hacky workaround
	float			GetTextYPosNextItem(const MenuItem::MenuEntry& pPosition);

//	void		SaveStatsHTML();
};

void	LoadingScreen();
void	LoadSplashes(bool bIntroSplash, unsigned char nIntroSplashID);

static_assert(sizeof(CMenuManager) == CMenuManager_ARRAYSIZE, "CMenuManager has wrong size!");

#endif