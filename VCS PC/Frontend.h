#ifndef __CMENUMANAGER
#define __CMENUMANAGER

#include "Sprite.h"
#include "DLCManager.h"

#define						MENU_TEXT_POSITION_LCOLUMN 320.0f	// Adjusted by the game
#define						MENU_TEXT_POSITION_RCOLUMN 70.0f
//#define						MENU_TEXT_POSITION_RCOLUMN 250.0f
#define						MENU_SLIDER_HEIGHT 10.0f
#define						MENU_SLIDER_WIDTH 9.0f

#define						MENU_SLIDER_POSY 135.0f

#define						NUM_MENUS 47				// Relevant for debugging only
#define						NUM_ENTRIES_PER_MENU 14

#define						NUM_SLIDERS 8
//#define					COMPILE_BOUNCING_ICONS
//#define					COMPILE_SMOOTHBEATING_ICONS
#define						COMPILE_BEATING_ICONS
//#define						INCLUDE_PROMO_BANNER

#define MENU_INACTIVE_R			0x1E
#define MENU_INACTIVE_G			0xFF
#define MENU_INACTIVE_B			0xFF

#define MENU_ACTIVE_R			0xFF
#define MENU_ACTIVE_G			0xEC
#define MENU_ACTIVE_B			0x93

#define MENU_LOCKED_R			(MENU_INACTIVE_R*2/10)
#define MENU_LOCKED_G			(MENU_INACTIVE_G*2/10)
#define MENU_LOCKED_B			(MENU_INACTIVE_B*2/10)

#define MENU_ACTIVE_LOCKED_R	(MENU_ACTIVE_R*2/10)
#define MENU_ACTIVE_LOCKED_G	(MENU_ACTIVE_G*2/10)
#define MENU_ACTIVE_LOCKED_B	(MENU_ACTIVE_B*2/10)

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

#define MENU_RED_R				0xFF
#define MENU_RED_G				0x48
#define MENU_RED_B				0x4D

#define						MAX_AA				8
#define						SET_FILE_VERSION	11

#ifdef DEVBUILD
#define						SET_FILE_NAME		"gta_vcsd.set"
#else
#define						SET_FILE_NAME		"gta_vcs.set"
#endif

// Macroes
#define MenuEntriesList (CMenuManager::ms_pMenus)
#define aScreens (CMenuManager::ms_pMenus)

enum eMenuPages
{
	// TODO: Fill?
	MENU_PAGE_STATS,
	MENU_PAGE_GAME,
	MENU_PAGE_BRIEF,
	MENU_PAGE_AUDIO_SETUP,
	MENU_PAGE_DISPLAY_SETUP,

	MENU_PAGE_GAME_UPDATES = 44,
	MENU_PAGE_DLC,
	MENU_PAGE_ACTIVATE_SERIAL = 49
};

enum eMenuActions
{
	// TODO: Fill?
	MENUACTION_SHADOWS_QUALITY		= 42,	// Formerly Visual FX Quality

	MENUACTION_TOGGLE_LANGUAGE_6	= 66,
	MENUACTION_UPDATER_BUTTON,
	MENUACTION_TOGGLE_DLC,
	MENUACTION_ACTIVATE_SERIAL,
	MENUACTION_AUTOINSTALL_UPDATES,
	MENUACTION_CHECKING_PERIOD,
	MENUACTION_TEXTURE_FILTERMODE,
	MENUACTION_EFFECTS_QUALITY,
	MENUACTION_SHADOWS_DISTANCE,
	MENUACTION_TRAILS,

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
	ACTION_DLC,
	ACTION_SERIAL
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
public:
	bool				m_bStatScrollUp;
	float               m_fStatsScrollSpeed;
	__int8 field_8;
	__int8 field_9[23];
	bool				m_bVibrationEnabled;
	unsigned char		m_bHudOn;
	__int8 field_22[2];
	__int32             m_dwRadarMode;
	__int8 field_28[4];
	__int32 field_2C;
	__int8 field_30;
	__int8 field_31;
	bool				m_bBackIntoGame;
	bool                m_bActivateMenuNextFrame;
	bool                m_bMenuAccessWidescreen;
	__int8 field_35;
	__int8 field_36[2];
	__int32 field_38;
	__int32             m_dwBrightness;
	float               m_fDrawDistance;
	bool                m_bShowSubtitles;
	bool 				m_bMapShowLocations;
	bool				m_bMapShowContacts;
	bool				m_bMapShowMission;
	bool				m_bMapShowOther;
	bool				m_bMapShowGangArea;
	bool                m_bMapLegend;
	unsigned char		m_bAspectRatioMode;
	unsigned char       m_bFrameLimiterMode;
	bool                m_bRadioAutoSelect;
	__int8 field_4E;
	__int8              m_nSfxVolume;
	__int8              m_nRadioVolume;
	bool                m_bRadioEq;
	signed char			m_nRadioStation;
	__int8 field_53;
	unsigned int		m_dwSelectedMenuItem;
	__int8 field_58;
	__int8 drawRadarOrMap;
	__int8 field_5A;
	__int8 field_5B;
	bool                m_bMenuActive;
	__int8 doGameReload;
	__int8 field_5E;
	__int8 isSaveDone;
	bool                m_bLoadingData;
	__int8 field_61[3];
	float               m_fMapZoom;
	float               m_fMapBaseX;
	float               m_fMapBaseY;
	CVector2D           m_vMousePos;
	bool				m_bMapLoaded;
	__int8 field_79[3];
	__int32 titleLanguage;
	__int32 textLanguage;
	unsigned char       m_nLanguage;
	unsigned char		m_nPrevLanguage;
	__int8 field_86[2];
	__int32 field_88;
	bool                m_bLanguageChanged;
	__int8 field_8D[3];
	__int32 field_90;
	__int8 field_94[24];
	__int32 field_AC;
	__int8              m_nRadioMode;
	__int8 invertPadX1;
	__int8 invertPadY1;
	__int8 invertPadX2;
	__int8 invertPadY2;
	__int8 swapPadAxis1;
	__int8 swapPadAxis2;
	bool				m_bInVehicleControlsScreen;
	bool                m_bDrawMouse;
	__int8 field_B9[3];
	__int32             m_dwMousePosLeft;
	__int32             m_dwMousePosTop;
	bool                m_bMipMapping;
	bool                m_bTracksAutoScan;
	__int16 field_C6;
	__int32             m_dwAppliedAntiAliasingLevel;
	__int32             m_dwAntiAliasingLevel;
	__int8              m_nController;
	__int8 field_D1[3];
	__int32             m_dwAppliedResolution;
	__int32             m_dwResolution;
	__int32 field_DC;
	__int32 mousePosLeftA;
	__int32 mousePosTopA;
	bool                m_bSavePhotos;
	bool                m_bMainMenuSwitch;
	__int8              m_nPlayerNumber;
	bool				m_bReinitLanguageSettings;
	__int32 field_EC;
	void* field_F0;
	__int8 field_F4;
	__int8 field_F5[3];
	union{
		struct{
			CSprite2d m_apTextures[25];
		};
		struct{
			CSprite2d m_apRadioSprites[13];
			CSprite2d m_apBackgroundTextures[8];
			CSprite2d m_apAdditionalBackgroundTextures[2];
			CSprite2d m_apMouseTextures[2];
		};
	};
	bool                m_bTexturesLoaded;
	signed char	        m_bCurrentMenuPage;
	signed char			m_bLastMenuPage;
	unsigned char		m_bSelectedSaveGame;
	unsigned char		m_bSelectedMissionPack;
	__int8 field_161;
	char                m_mpackName[8];
	__int8 field_16A[6486];
	__int32 field_1AC0;
	__int32 field_1AC4;
	__int32 field_1AC8;
	__int32 field_1ACC;
	__int32 field_1AD0;
	__int32 field_1AD4;
	__int32 field_1AD8;
	__int16 field_1ADC;
	bool                m_bChangeVideoMode;
	__int8 field_1ADF;
	__int32 field_1AE0;
	__int32 field_1AE4;
	__int8 field_1AE8;
	__int8 field_1AE9;
	__int8 field_1AEA;
	__int8 field_1AEB;
	int			m_nHelperAlpha;
	__int8 field_1AF0;
	__int8 field_1AF1;
	__int8 field_1AF2;
	__int8 field_1AF3;
	__int32 field_1AF4;
	__int32 field_1AF8;
	__int32 field_1AFC;
	int			m_nHoverOption;
	__int32 field_1B04;
	__int8 field_1B08;
	__int8 field_1B09;
	__int8 field_1B0A;
	__int8 field_1B0B;
	__int32 field_1B0C;
	__int8 field_1B10;
	__int8 field_1B11;
	__int8 field_1B12;
	__int8 field_1B13;
	__int8 field_1B14;
	__int8 field_1B15;
	__int8 field_1B16;
	__int8 field_1B17;
	int			m_nHelperTextIndex;
	__int32 field_1B1C;
	__int8 field_1B20;
	__int8 field_1B21;
	__int16 field_1B22;
	__int32 field_1B24;
	__int8 field_1B28;
	__int8 field_1B29;
	__int16 field_1B2A;
	__int32 field_1B2C;
	__int32 field_1B30;
	__int16 field_1B34;
	__int16 field_1B36;
	__int32 field_1B38;
	__int8 field_1B3C;
	__int8 field_1B3D;
	__int8 field_1B3E;
	__int8 field_1B3F;
	__int32 field_1B40;
	__int8 field_1B44;
	__int8 field_1B45;
	__int16 field_1B46;
	__int32 field_1B48;
	__int32 field_1B4C;
	__int8              m_nBackgroundSprite;
	__int8 field_1B51;
	__int16 field_1B52;
	__int32 field_1B54;
	int		m_nLastTimeHelperUpdated;
	__int8 field_1B5C;
	__int8 field_1B5D;
	__int16 field_1B5E;
	__int32 field_1B60;
	__int32 field_1B64;
	__int32 field_1B68;
	__int32 field_1B6C;
	__int32 field_1B70;
	__int32 field_1B74;

	// VCS PC class extension
	static int			m_nFocusedDLC, m_nLastFocusedDLC;
	static float		m_fStatsScrollPos;
	static int			ms_nRubberSlider;
	static bool			m_bLastDLCState[NUM_DLC_PACKS];

	static std::string	m_strSerialCode[SERIAL_CODES_LENGTH];
	static bool			m_bSerialFull;
	static const char*	m_pDLCMessage;
	static signed int	m_nDLCMessageTimer;
	static signed char	m_nSwitchToThisAfterMessage;

public:
	static MenuItem		ms_pMenus[];

private:
	void			ProcessMissionPackNewGame();
	void			DoSettingsBeforeStartingAGame();
	void			ScrollRadioStations(signed char nDirection);

	void			PrintStats();
	void			PrintUpdaterScreen();
	void			PrintDLCScreen();
	void			PrintActivationScreen();
	void			DrawStandardMenus(bool bDrawMenu);
	void			DrawRadioStationIcons();
	float			DisplaySlider(float posX, float posY, float height, float distBetweenRects, float filledAmount, float width, bool bLocked);

	void			CentreMousePointer();
	void			ProcessMenuOptions(signed char nArrowsInput, bool* bReturn, bool bEnterInput);
	void			CheckSliderMovement(signed char nDirection);

public:
	inline BYTE		GetHudMode()
						{ return m_bHudOn; };

	inline BYTE		GetLanguage()
						{ return m_nLanguage; };

	inline bool		IsActive()
						{ return m_bMenuActive; };

	inline void		SetLanguage(BYTE lang)
						{ m_nLanguage = lang; };

	inline void		SetTitleLanguage(DWORD lang)
						{ titleLanguage = lang; };

	inline void		SetKeyboardLayout(BYTE lang)
						{ textLanguage = lang; };

	static inline void	ClearSerialsBuffer()
	{	m_bSerialFull = false;
		for ( int i = 0; i < SERIAL_CODES_LENGTH; ++i )
			m_strSerialCode[i].clear();
	}

	static inline bool	ValidSerialCharacter(wchar_t wKey)
	{ return (wKey >= '0' && wKey <= '9') || (wKey >= 'A' && wKey <= 'Z') || (wKey >= 'a' && wKey <= 'z'); }

	static inline void	SwitchToScreenAfterMessage(signed char nScreen)
	{ m_nSwitchToThisAfterMessage = nScreen; }
	
	void			ResetHelperText()
	{ m_nHelperTextIndex = 0; m_nHelperAlpha = 300; }
	void			SetHelperText(int nText)
	{ m_nHelperTextIndex = nText; m_nHelperAlpha = 300; }

	static void		RegisterDLCMessage(const char* pMessage);
	static void		LookIntoClipboardForSerial();

	void			DrawWindow(const CRect& rect, const char* pKey, unsigned char nColour, CRGBA backColour, bool bUnused=false, bool bBackground=false);
	void			SmallMessageScreen(const char* pMessage);
	void			SwitchToNewScreen(signed char bScreen);
	void			InitialiseChangedLanguageSettings(bool bRemapButtons);
	void			PrintBrief();
	void			DrawContollerScreenExtraText(int nUnk);
	void			DisplayHelperText(const char* pText);
	bool			CheckHover(int, int, int, int);

	void			SetDefaultPreferences(signed char bScreen);
	void			DrawBackEnd();
	void			DrawLeftColumn(MenuItem::MenuEntry& pPosition, const char* pText, const char* pRightText);
	//float			GetLeftColumnPos_Height(long posY);
	float			GetRightColumnPos(MenuVar& sPosY);
	void			DrawOutroSplash();
	void			ReadFrontendTextures();
	void			SwitchToNewScreenVCS(signed char bScreen);
	void			AdditionalOptionInputVCS(unsigned char* pUp, unsigned char* pDown);
	void			UserInputVCS();
	void			MessageScreen(const char* pMessage, bool bFullscreen, bool bWithinFrame);
	void			SaveSettings();
	void			LoadSettings();

	bool			NeedsToRefreshHelps();
	void			TypingKeyboardInput(wchar_t wKey);
	const char*		ProcessDLCSlot(int nSlotID);
	float			GetTextYPos(const MenuItem::MenuEntry& pPosition);

	// Hacky workaround
	float			GetTextYPosNextItem(const MenuItem::MenuEntry& pPosition);

	static void		Inject();

//	void		SaveStatsHTML();
};

void	LoadingScreen();
void	LoadSplashes(bool bIntroSplash, unsigned char nIntroSplashID);

extern CMenuManager&		FrontEndMenuManager;

static_assert(sizeof(CMenuManager) == 0x1B78, "Wrong size: CMenuManager");

#endif