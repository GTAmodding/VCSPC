#include "StdAfx.h"
#include "Frontend.h"

#include "Game.h"
#include "Font.h"
#include "Timer.h"
#include "UpdateManager.h"
#include "Messages.h"
#include "Stats.h"
#include "VideoPlayer.h"
#include "Text.h"
#include "ControlsMgr.h"
#include "Audio.h"
#include "Pad.h"
#include "Vehicle.h"
#include "Camera.h"
#include "FxSystem.h"
#include "Shadows.h"
#include "PcSave.h"
#include "PostEffects.h"
#include "Rs.h"
#include "NeoCarpipe.h"
#include "gtamain.h"
#include "MenuSystem.h"
#include "Radar.h"
#include "Hud.h"

bool m_bResetMap;

CLoadingScreen		SplashScreen;
CSprite2d* const	LoadingSprites = (CSprite2d*)0xBAB35C;
int&				CurrentLoadingSprite = *(int*)0x8D093C;

int					CMenuManager::ms_nRubberSlider;
float				CMenuManager::m_fStatsScrollPos;
int					CMenuManager::m_nFocusedDLC = -1;
int					CMenuManager::m_nLastFocusedDLC = -1;
bool				CMenuManager::m_bLastDLCState[NUM_DLC_PACKS];
std::string			CMenuManager::m_strSerialCode[SERIAL_CODES_LENGTH];
bool				CMenuManager::m_bSerialFull;
const char*			CMenuManager::m_pDLCMessage;
signed int			CMenuManager::m_nDLCMessageTimer = 0;
signed char			CMenuManager::m_nSwitchToThisAfterMessage = -1;
short				CMenuManager::m_nNumMenuEntries;
unsigned char		CMenuManager::m_bPadPageShown;
float				CMenuManager::m_fScrollerOffset = 0.0f;
bool				CMenuManager::m_bAppliedVSync;
bool				CMenuManager::m_bVSync;

bool				CMenuManager::m_bSwitchToSkyMenu;
bool				CMenuManager::m_bEnableSkyMenu;
bool				CMenuManager::m_bMenuPagesHasBeenStored;
int					CMenuManager::m_nSelectedSkyMenuItem = 0;
bool				CMenuManager::m_bIsMenuSwitched = false;
float				CMenuManager::m_fSafeZone;
bool				CMenuManager::m_bControlSafeZone;
float				CMenuManager::SafeZoneHideStuffAlpha = 255.0f;

bool				CMenuManager::m_bBottomMenu;
bool				CMenuManager::m_bCanReturnToBottomMenu;
bool				CMenuManager::m_bIsMouseInPosition;
bool				CMenuManager::m_bIsPauseMenu;
CRGBA				CMenuManager::m_nBackgroundColor;
bool				CMenuManager::m_bPCFrontEnd;

CVector2D			CMenuManager::m_vecMenuColumnPosn[2];
CVector2D			CMenuManager::m_vecMenuColumnSize;

float				CMenuManager::m_fVCSMapZoom;
float				CMenuManager::m_fVCSMapBaseX;
float				CMenuManager::m_fVCSMapBaseY;

static int	nTimeToStopPadShake;

// Temp;
extern float&		ms_lodDistScale;

//short			CMenuManager::nColourMenuEntries;
#define MENUPAGES 53
MenuItem		CMenuManager::_aScreensNone[MENUPAGES];
MenuItem		CMenuManager::_aScreens[MENUPAGES];
MenuItem		CMenuManager::_MenuEntriesList[MENUPAGES];

MenuItem		CMenuManager::ms_pMenus[MENUPAGES] = {
	// Stats
	{ "FEH_STA", 42, 3, 0, 0,
		21, "FEDS_TB", ACTION_CLICKORARROWS, 0, 0, 69, 3, 1, 0 },

	// Game
	{ "FEH_LOA", 42, 1, 0, 0,
		10, "FES_NGA", ACTION_STANDARD, 6, 0, -74, 3, 0, 0,
		5, "FES_LOA", ACTION_STANDARD, 9, 0, 0, 3, 0, 0,
		5, "FES_DEL", ACTION_STANDARD, 10, 0, 0, 3, 0, 0,
		2, "FEDS_TB", ACTION_STANDARD, 0, 0, 0, 3, 0, 0 },

	// Brief
	{ "FEH_BRI", 42, 4, 0, 0,
		2, "FEDS_TB", ACTION_STANDARD, 42, 0, 45, 3, 1, 0 },

	// Audio Setup
	{ "FEH_AUD", 33, 1, 0, 0,
		28, "FEA_MUS", ACTION_CLICKORARROWS, 3, 0, -124, 2, 0, 0,
		29, "FEA_SFX", ACTION_CLICKORARROWS, 3, 0, 0, 2, 0, 0,
		31, "FEA_ART", ACTION_CLICKORARROWS, 3, 0, 0, 2, 0, 0,
		5, "FEA_TIT", ACTION_STANDARD, 26, 0, -29, 3, 0, 0,
		32, "FEA_RSS", ACTION_CLICKORARROWS, 3, 0, 36, 2, 0, 0,
		5, "FET_DEF", ACTION_STANDARD, 24, 0, 143, 3, 0, 0,
		2, "FEDS_TB", ACTION_STANDARD, 42, 0, 0, 3, 0, 0 },

	// Display Setup
	{ "FEH_DIS", 33, 2, 0, 0,
		27, "FED_BRI", ACTION_CLICKORARROWS, 4, 0, -102, 2, 0, 0,
		MENUACTION_SAFEZONE, "FED_SFZ", ACTION_CLICKORARROWS, 4, 0, 0, 2, 0, 0,
		33, "MAP_LEG", ACTION_CLICKORARROWS, 4, 0, 0, 2, 0, 0,
		34, "FED_RDR", ACTION_CLICKORARROWS, 4, 0, 0, 2, 0, 0,
		35, "FED_HUD", ACTION_CLICKORARROWS, 4, 0, 0, 2, 0, 0,
		25, "FED_SUB", ACTION_CLICKORARROWS, 4, 0, 0, 2, 0, 0,
		65, "FED_GLS", ACTION_CLICKORARROWS, 4, 0, 0, 2, 0, 0,
		MENUACTION_SKY_MENU, "FED_SKY", ACTION_CLICKORARROWS, 4, 0, 0, 2, 0, 0,
		5, "FET_DEF", ACTION_STANDARD, 23, 0, 141, 3, 0, 0,
		2, "FEDS_TB", ACTION_STANDARD, 33, 0, 0, 3, 0, 0 },


	{ "FEH_MAP", 42, 2, 0, 0,
		//2, "FEDS_TB", ACTION_STANDARD, 42, 57, 141, 1, 0, 
    },
    

	{ "FEH_NGA", 1, 0, 0, 0,
		1, "FESZ_QR", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		4, "FEM_NO", ACTION_STANDARD, 1, 0, -9, 3, 0, 0,
		14, "FEM_YES", ACTION_STANDARD, 6, 0, 16, 3, 0, 0 },


	{ "FEH_NGA", 1, 0, 0, 0,
		1, "FEN_NGS", ACTION_NONE, 0, 0, 0, 1, 0, 0,
		5, "FEN_NGX", ACTION_STANDARD, 6, 0, -74, 3, 0, 0,
		11, "FEN_MPX", ACTION_MISSIONPACK, 0, 0, 0, 3, 0, 0,
		11, "FEN_MPX", ACTION_MISSIONPACK, 0, 0, 0, 3, 0, 0,
		11, "FEN_MPX", ACTION_MISSIONPACK, 0, 0, 0, 3, 0, 0,
		11, "FEN_MPX", ACTION_MISSIONPACK, 0, 0, 0, 3, 0, 0,
		11, "FEN_MPX", ACTION_MISSIONPACK, 0, 0, 0, 3, 0, 0,
		11, "FEN_MPX", ACTION_MISSIONPACK, 0, 0, 0, 3, 0, 0,
		11, "FEN_MPX", ACTION_MISSIONPACK, 0, 0, 0, 3, 0, 0,
		11, "FEN_MPX", ACTION_MISSIONPACK, 0, 0, 0, 3, 0, 0,
		11, "FEN_MPX", ACTION_MISSIONPACK, 0, 0, 0, 3, 0, 0,
		11, "FEN_MPX", ACTION_MISSIONPACK, 0, 0, 0, 3, 0, 0,
		11, "FEN_MPX", ACTION_MISSIONPACK, 0, 0, 0, 3, 0, 0,
		11, "FEN_MPX", ACTION_MISSIONPACK, 0, 0, 0, 3, 0, 0 },


	{ "FES_LMI", 1, 0, 0, 0,
		1, "FESZ_QM", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		4, "FEM_NO", ACTION_STANDARD, 1, 0, -9, 3, 0, 0,
		12, "FEM_YES", ACTION_STANDARD, 0, 0, 16, 3, 0, 0 },


	{ "FET_LG", 1, 1, 0, 0,
		1, "FES_SEL", ACTION_NONE, 0, 0, 0, 1, 0, 0,
		13, "FEM_SL1", ACTION_SAVE_1, 0, 80, -84, 1, 0, -1,
		13, "FEM_SL2", ACTION_SAVE_2, 0, 80, -64, 1, 0, -1,
		13, "FEM_SL3", ACTION_SAVE_3, 0, 80, -44, 1, 0, -1,
		13, "FEM_SL4", ACTION_SAVE_4, 0, 80, -24, 1, 0, -1,
		13, "FEM_SL5", ACTION_SAVE_5, 0, 80, -4, 1, 0, -1,
		13, "FEM_SL6", ACTION_SAVE_6, 0, 80, 16, 1, 0, -1,
		13, "FEM_SL7", ACTION_SAVE_7, 0, 80, 36, 1, 0, -1,
		13, "FEM_SL8", ACTION_SAVE_8, 0, 80, 56, 1, 0, -1,
		13, "FEM_SL9", ACTION_SAVE_9, 0, 80, 76, 1, 0, -1,
		13, "FEM_SLA", ACTION_SAVE_10, 0, 80, 96, 1, 0, -1,
		13, "FEM_SLB", ACTION_SAVE_11, 0, 80, 116, 1, 0, -1,
		13, "FEM_SLC", ACTION_SAVE_12, 0, 80, 136, 1, 0, -1,
		2, "FEDS_TB", ACTION_STANDARD, 0, 0, 48, 3, 1, 0 },


	{ "FEH_DEL", 1, 2, 0, 0,
		1, "FES_SED", ACTION_NONE, 0, 0, 0, 1, 0, 0,
		13, "FEM_SL1", ACTION_SAVE_1, 0, 80, -84, 1, 0, -1,
		13, "FEM_SL2", ACTION_SAVE_2, 0, 80, -64, 1, 0, -1,
		13, "FEM_SL3", ACTION_SAVE_3, 0, 80, -44, 1, 0, -1,
		13, "FEM_SL4", ACTION_SAVE_4, 0, 80, -24, 1, 0, -1,
		13, "FEM_SL5", ACTION_SAVE_5, 0, 80, -4, 1, 0, -1,
		13, "FEM_SL6", ACTION_SAVE_6, 0, 80, 16, 1, 0, -1,
		13, "FEM_SL7", ACTION_SAVE_7, 0, 80, 36, 1, 0, -1,
		13, "FEM_SL8", ACTION_SAVE_8, 0, 80, 56, 1, 0, -1,
		13, "FEM_SL9", ACTION_SAVE_9, 0, 80, 76, 1, 0, -1,
		13, "FEM_SLA", ACTION_SAVE_10, 0, 80, 96, 1, 0, -1,
		13, "FEM_SLB", ACTION_SAVE_11, 0, 80, 116, 1, 0, -1,
		13, "FEM_SLC", ACTION_SAVE_12, 0, 80, 136, 1, 0, -1,
		2, "FEDS_TB", ACTION_STANDARD, 0, 0, 48, 3, 1, 0 },


	{ "FET_LG", 9, 0, 0, 0,
		1, "FESZ_QL", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		4, "FEM_NO", ACTION_STANDARD, 9, 0, -9, 3, 0, 0,
		3, "FEM_YES", ACTION_STANDARD, 13, 0, 16, 3, 0, 0 },


	{ "FEH_DEL", 10, 0, 0, 0,
		1, "FESZ_QD", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		4, "FEM_NO", ACTION_STANDARD, 10, 0, -9, 3, 0, 0,
		3, "FEM_YES", ACTION_STANDARD, 14, 0, 16, 3, 0, 0 },


	{ "FET_LG", 9, 0, 0, 0 },


	{ "FEH_DEL", 10, 0, 0, 0 },


	{ "FEH_DEL", 1, 0, 0, 0,
		1, "FES_DSC", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEM_OK", ACTION_STANDARD, 1, 0, 16, 3, 0, 0 },


	{ "FET_SG", 255, 0, 0, 0,
		1, "FES_SES", ACTION_NONE, 0, 0, 0, 1, 0, 0,
		18, "FEM_SL1", ACTION_SAVE_1, 0, 80, -84, 1, 0, -1,
		18, "FEM_SL2", ACTION_SAVE_2, 0, 80, -64, 1, 0, -1,
		18, "FEM_SL3", ACTION_SAVE_3, 0, 80, -44, 1, 0, -1,
		18, "FEM_SL4", ACTION_SAVE_4, 0, 80, -24, 1, 0, -1,
		18, "FEM_SL5", ACTION_SAVE_5, 0, 80, -4, 1, 0, -1,
		18, "FEM_SL6", ACTION_SAVE_6, 0, 80, 16, 1, 0, -1,
		18, "FEM_SL7", ACTION_SAVE_7, 0, 80, 36, 1, 0, -1,
		18, "FEM_SL8", ACTION_SAVE_8, 0, 80, 56, 1, 0, -1,
		18, "FEM_SL9", ACTION_SAVE_9, 0, 80, 76, 1, 0, -1,
		18, "FEM_SLA", ACTION_SAVE_10, 0, 80, 96, 1, 0, -1,
		18, "FEM_SLB", ACTION_SAVE_11, 0, 80, 116, 1, 0, -1,
		18, "FEM_SLC", ACTION_SAVE_12, 0, 80, 136, 1, 0, -1,
		15, "FESZ_CA", ACTION_STANDARD, 0, 0, 48, 3, 1, 0 },


	{ "FET_SG", 16, 0, 0, 0,
		1, "FESZ_QZ", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		4, "FEM_NO", ACTION_STANDARD, 16, 0, -9, 3, 0, 0,
		3, "FEM_YES", ACTION_STANDARD, 18, 0, 16, 3, 0, 0 },


	{ "FET_SG", 16, 0, 0, 0 },


	{ "FET_SG", 16, 0, 0, 0,
		1, "FES_SSC", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		54, "FEM_OK", ACTION_STANDARD, 0, 0, 16, 3, 0, 0 },


	{ "FET_SG", 42, 0, 0, 0,
		1, "", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEM_OK", ACTION_STANDARD, 0, 0, 46, 3, 0, 0 },


	{ "FET_LG", 42, 0, 0, 0,
		1, "", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEM_OK", ACTION_STANDARD, 0, 0, 46, 3, 0, 0 },


	{ "FET_SG", 1, 0, 0, 0,
		1, "FES_CHE", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEM_OK", ACTION_STANDARD, 16, 0, 16, 3, 0, 0 },

	// Restore defaults - Display Setup
	{ "FEH_DIS", 4, 6, 0, 0,
		1, "FED_RDP", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEM_NO", ACTION_STANDARD, 4, 0, -9, 3, 0, 0,
		57, "FEM_YES", ACTION_STANDARD, 4, 0, 16, 3, 0, 0},

	// Restore detaults - Audio Setup
	{ "FEH_AUD", 3, 5, 0, 0,
		1, "FED_RDP", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEM_NO", ACTION_STANDARD, 3, 0, -9, 3, 0, 0,
		57, "FEM_YES", ACTION_STANDARD, 3, 0, 16, 3, 0, 0 },

	// Restore defaults - Controller Setup
	{ "FET_CTL", 36, 3, 0, 0,
		1, "FED_RDP", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEM_NO", ACTION_STANDARD, 36, 0, -9, 3, 0, 0,
		57, "FEM_YES", ACTION_STANDARD, 36, 0, 16, 3, 0, 0 },

	// Custom Tracks Options
	{ "FEH_TIT", 3, 4, 0, 0,
		1, "FEA_SUB", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		63, "FEA_MPM", ACTION_CLICKORARROWS, 26, 0, -34, 2, 0, 0,
		64, "FEA_AMS", ACTION_CLICKORARROWS, 26, 0, 0, 2, 0, 0,
		6, "FEA_SCN", ACTION_STANDARD, 26, 0, 0, 3, 0, 0,
		2, "FEDS_TB", ACTION_STANDARD, 3, 0, 136, 3, 0, 0 },

	// Graphics Setup
	{ "FEH_GFX", 33, 3, 75, 50,
		56, "FED_RES", ACTION_CLICKORARROWS, 27, 0, -154, 2, 0, 0,
		26, "FED_WIS", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		MENUACTION_VSYNC, "FED_VSN", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		24, "FEM_FRM", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		MENUACTION_EFFECTS_QUALITY, "FED_EFF", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		MENUACTION_SHADOWS_QUALITY, "FED_SHA", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		MENUACTION_PIPE_QUALITY, "FED_RFQ", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		61, "FEM_LOD", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		MENUACTION_SHADOWS_DISTANCE, "FED_SHD", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		44, "FED_AAS", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		MENUACTION_TEXTURE_FILTERMODE, "FED_TXF", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		MENUACTION_TRAILS, "FED_TRA", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		MENUACTION_NEOVEH, "NEOVEH", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		5, "FET_DEF", ACTION_STANDARD, MENU_PAGE_RESTORE_GRAPHICS, 0, 0, 3, 0, 0,
		2, "FEDS_TB", ACTION_STANDARD, 33, 0, 0, 3, 0, 0 },

	// Language
	{ "FEH_LAN", 33, 4, 0, 0,
		37, "FEL_ENG", ACTION_STANDARD, 28, 0, -72, 3, 0, 0,
		//38, "FEL_GER", ACTION_STANDARD, 28, 0, 0, 3, 0, 0,
        //39, "FEL_SPA", ACTION_STANDARD, 28, 0, 0, 3, 0, 0,
        38, "FEL_ITA", ACTION_STANDARD, 28, 0, 0, 3, 0, 0,
		39, "FEL_BRA", ACTION_STANDARD, 28, 0, 0, 3, 0, 0,
		40, "FEL_POL", ACTION_STANDARD, 28, 0, 0, 3, 0, 0,

		2, "FEDS_TB", ACTION_STANDARD, 4, 0, 0, 3, 0, 0 },


	{ "FET_SG", 1, 0, 0, 0,
		1, "FED_LWR", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEC_OKK", ACTION_STANDARD, 16, 0, 0, 0, 0, 0 },


	{ "FET_SG", 16, 0, 0, 0,
		1, "FEC_SVU", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEC_OKK", ACTION_STANDARD, 16, 0, 0, 0, 0, 0 },


	{ "FET_LG", 16, 0, 0, 0,
		1, "FEC_SVU", ACTION_NONE, 0, 0, 0, 0, 0, 0 },


	{ "FET_LG", 1, 0, 0, 0,
		1, "FEC_LUN", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		2, "FEDS_TB", ACTION_STANDARD, 1, 0, 0, 0, 0, 0 },

	// Options
	{ "FET_OPT", 42, 5, 0, 0,
		5, "FEO_CON", ACTION_STANDARD, 36, 0, -72, 3, 0, 0,
		5, "FEO_AUD", ACTION_STANDARD, 3, 0, 0, 3, 0, 0,
		5, "FEO_DIS", ACTION_STANDARD, 4, 0, 0, 3, 0, 0,
		5, "FEO_GFX", ACTION_STANDARD, 27, 0, 0, 3, 0, 0,
		5, "FEO_LAN", ACTION_STANDARD, 28, 0, 0, 3, 0, 0,
		5, "FEO_DLC", ACTION_STANDARD, 45, 0, 0, 3, 0, 0,
		5, "FEO_UPT", ACTION_STANDARD, 44, 0, 0, 3, 0, 0,
		2, "FEDS_TB", ACTION_STANDARD, 0, 0, 0, 3, 0, 0 },


	{ "FEM_MM", 255, 0, 0, 0,
		5, "FEP_STG", ACTION_STANDARD, 1, 0, -54, 3, 0, 0,
		5, "FEP_OPT", ACTION_STANDARD, 33, 0, 0, 3, 0, 0,
		5, "FEP_QUI", ACTION_STANDARD, 35, 0, 0, 3, 0, 0 },


	{ "FET_QG", 42, 6, 0, 0,
		1, "FEQ_SRE", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		55, "FEM_NO", ACTION_STANDARD, 42, 0, -9, 3, 0, 0,
		53, "FEM_YES", ACTION_STANDARD, 42, 0, 16, 3, 0, 0 },


	// Controller Setup
	{ "FET_CTL", 33, 0, 0, 0,
		5, "FEC_RED", ACTION_STANDARD, 37, 0, -29, 3, 0, 0,
		5, "FEC_MOU", ACTION_STANDARD, 39, 0, 0, 3, 0, 0,
		5, "FEC_CTL", ACTION_STANDARD, MENU_PAGE_CONTROLLER_SETUP, 0, 0, 3, 0, 0,
		5, "FET_DEF", ACTION_STANDARD, 25, 0, 70, 3, 0, 0,
		2, "FEDS_TB", ACTION_STANDARD, 0, 0, 0, 3, 0, 0 },


	// Redefine Controls
	{ "FET_CTL", 36, 0, 0, 0,
		8, "FET_CFT", ACTION_STANDARD, 38, 0, -34, 3, 0, 0,
		9, "FET_CCR", ACTION_STANDARD, 38, 0, 0, 3, 0, 0,
		2, "FEDS_TB", ACTION_STANDARD, 36, 0, 46, 3, 0, 0 },


	{ "FET_CTL", 37, 0, 0, 0 },


	// Mouse Settings
	{ "FEM_MOU", 36, 1, 0, 0,
		62, "FEC_MSH", ACTION_CLICKORARROWS, 39, 0, -94, 2, 0, 0,
		46, "FEC_IVV", ACTION_CLICKORARROWS, 39, 0, 0, 2, 0, 0,
		59, "FET_MST", ACTION_CLICKORARROWS, 39, 0, 0, 2, 0, 0,
		60, "FET_MFL", ACTION_CLICKORARROWS, 39, 0, 0, 2, 0, 0,
		2, "FEDS_TB", ACTION_STANDARD, 0, 0, 66, 3, 0, 0 },


	// Controller Settings
	{ "FEH_CTL", 36, 2, 0, 0,
		MENUACTION_CTRL_TYPE, "FEC_CFG", ACTION_CLICKORARROWS, MENU_PAGE_CONTROLLER_SETUP, 0, -164, 2, 0, 0,
		MENUACTION_PAD_FRONTEND_PAGE, "FEC_CDP", ACTION_CLICKORARROWS, MENU_PAGE_CONTROLLER_SETUP, 0, 0, 2, 0, 0,
		//MENUACTION_INVERTLOOK, "FEC_ILU", ACTION_CLICKORARROWS, MENU_PAGE_CONTROLLER_SETUP, 0, 0, 2, 0, 0,
		MENUACTION_VIBRATION, "FEC_VIB", ACTION_CLICKORARROWS, MENU_PAGE_CONTROLLER_SETUP, 0, 0, 2, 0, 0,
		5, "FEC_ADO", ACTION_STANDARD, MENU_PAGE_ADDITIONAL_CONTROLLER, 0, 0, 3, 0, 0,
		2, "FEDS_TB", ACTION_STANDARD, 0, 0, 182, 3, 0, 0 },

	// Pause menu
	{ "FET_PAU", 255, 0, 0, 0,
		54, "FEP_RES", ACTION_STANDARD, 0, 0, -84, 3, 0, 0,
		5, "FEP_SGA", ACTION_STANDARD, 1, 0, 0, 3, 0, 0,
		5, "FEP_MAP", ACTION_STANDARD, 5, 0, 0, 3, 0, 0,
		5, "FEP_STA", ACTION_STANDARD, 0, 0, 0, 3, 0, 0,
		5, "FEP_BRI", ACTION_STANDARD, 2, 0, 0, 3, 0, 0,
		5, "FEP_OPT", ACTION_STANDARD, 33, 0, 0, 3, 0, 0,
		5, "FEP_QUI", ACTION_STANDARD, 35, 0, 0, 3, 0, 0 },


	// 2 menus serving a special purpose, cba moving
	{ "", 0, 0, 0, 0 },


	{ "", 0, 0, 0, 0 },


	// Game Updates
	{ "FEH_UPT", 33, 6, 0, 0,
		MENUACTION_UPDATER_BUTTON, "FEU_UPC", ACTION_UPDATER, 44, 0, 138, 3, 1, 0,
		MENUACTION_CHECKING_PERIOD, "FEU_CHP", ACTION_CLICKORARROWS, 44, 0, 0, 2, 1, 0,
		MENUACTION_AUTOINSTALL_UPDATES, "FEU_AUI", ACTION_CLICKORARROWS, 44, 0, 0, 2, 1, 0,
		2, "FEDS_TB", ACTION_STANDARD, 0, 0, 0, 3, 1, 0 },

	// Downloadable Content
	{ "FEH_DLC", 33, 5, 0, 0,
		1, "FEE_HEA", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		2, "FEDS_TB", ACTION_STANDARD, 0, 0, 48, 3, 1, 0,	// Hacky hacky
		MENUACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 60, -70, 1, 0, -1,
		MENUACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 0, 0, 1, 0, -1,
		MENUACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 0, 0, 1, 0, -1,
		MENUACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 0, 0, 1, 0, -1,
		MENUACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 0, 0, 1, 0, -1,
		MENUACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 0, 0, 1, 0, -1,
		MENUACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 0, 0, 1, 0, -1,
		MENUACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 0, 0, 1, 0, -1,
		MENUACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 0, 0, 1, 0, -1,
		MENUACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 0, 0, 1, 0, -1,
		MENUACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 0, 0, 1, 0, -1,
		MENUACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 0, 0, 1, 0, -1 },

	// Downloadable Content - no DLCs available
	{ "FEH_DLC", 33, 5, 0, 0,
		1, "FEE_NON", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEM_OK", ACTION_STANDARD, 33, 0, 16, 3, 0, 0 },

	// Downloadable Content - game restart required
	{ "FEH_DLC", 33, 5, 0, 0,
		1, "FEE_RES", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEM_OK", ACTION_STANDARD, 33, 0, 16, 3, 0, 0 },

	// Downloadable Content - DLC installation prompt
	{ "FEH_DLC", 45, 2, 0, 0,
		1, "FEE_IXX", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEM_NO", ACTION_STANDARD, 45, 0, -9, 3, 0, 0,
		57, "FEM_YES", ACTION_STANDARD, 44, 0, 16, 3, 0, 0 },

	// DLC activation
	{ "FEH_DLC", 45, 0, 0, 0,
		1, "FEE_KEY", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		MENUACTION_ACTIVATE_SERIAL, "FEE_ACT", ACTION_SERIAL, 0, 0, 78, 3, 1, 0,
		2, "FEDS_TB", ACTION_STANDARD, 0, 0, 48, 3, 1, 0 },

	// Restore defaults - Graphics Setup
	{ "FEH_GFX", 27, 9, 0, 0,
		1, "FED_RDP", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEM_NO", ACTION_STANDARD, 27, 0, -9, 3, 0, 0,
		57, "FEM_YES", ACTION_STANDARD, 27, 0, 16, 3, 0, 0 },

	// Additional Controller Options
	{ "FEH_CTL", MENU_PAGE_CONTROLLER_SETUP, 3, 0, 0,
		MENUACTION_INVERTLOOK, "FEC_ILU", ACTION_CLICKORARROWS, MENU_PAGE_ADDITIONAL_CONTROLLER, 0, -164, 2, 0, 0,
		MENUACTION_SOUTHPAW, "FEC_SOU", ACTION_CLICKORARROWS, MENU_PAGE_CONTROLLER_SETUP, 0, 0, 2, 0, 0,
		MENUACTION_BUTTONSTYLE, "FEC_STL",  ACTION_CLICKORARROWS, MENU_PAGE_ADDITIONAL_CONTROLLER, 0, 0, 2, 0, 0,
		2, "FEDS_TB", ACTION_STANDARD, 0, 0, 182, 3, 0, 0
	},

	// 52 VCS Map Menu
	{ "FEH_MAP", 42, 2, 0, 0,

	},
    
};

MenuItem		CMenuManager::m_SkyMenus[] = {
	// 0 Stats
	{ "FEH_STA", 0, 0, 0, 0 },

	// 1 Game
	{ "FEH_LOA", 1, 0, 0, 0,
		10, "FES_NGA", ACTION_STANDARD, 6, 0, -74, 3, 0, 0,
		5, "FES_LOA", ACTION_STANDARD, 9, 0, 0, 3, 0, 0,
		5, "FES_DEL", ACTION_STANDARD, 10, 0, 0, 3, 0, 0,
		5, "FEP_QUI",  ACTION_STANDARD, 35, 0, 0, 3, 0, 0 },

	// 2 Brief
	{ "FEH_BRI", 2, 0, 0, 0 },

	// 3 Audio Setup
	{ "FEH_AUD", 3, 0, 0, 0,
		28, "FEA_MUS", ACTION_CLICKORARROWS, 3, 0, -144, 2, 0, 0,
		29, "FEA_SFX", ACTION_CLICKORARROWS, 3, 0, 0, 2, 0, 0,
		31, "FEA_ART", ACTION_CLICKORARROWS, 3, 0, 0, 2, 0, 0,
		5, "FEA_TIT", ACTION_STANDARD, 26, 0, 0, 3, 0, 0,
		32, "FEA_RSS", ACTION_CLICKORARROWS, 3, 0, 0, 2, 0, 0 },

	// 4 Display Setup
	{ "FEH_DIS", 4, 0, 0, 0,
		27, "FED_BRI", ACTION_CLICKORARROWS, 4, 0, -144, 2, 0, 0,
		MENUACTION_SAFEZONE, "FED_SFZ", ACTION_CLICKORARROWS, 4, 0, 0, 2, 0, 0,
		33, "MAP_LEG", ACTION_CLICKORARROWS, 4, 0, 0, 2, 0, 0,
		34, "FED_RDR", ACTION_CLICKORARROWS, 4, 0, 0, 2, 0, 0,
		35, "FED_HUD", ACTION_CLICKORARROWS, 4, 0, 0, 2, 0, 0,
		25, "FED_SUB", ACTION_CLICKORARROWS, 4, 0, 0, 2, 0, 0,
		65, "FED_GLS", ACTION_CLICKORARROWS, 4, 0, 0, 2, 0, 0, 
		37, "FED_LAN", ACTION_CLICKORARROWS, 4, 0, 0, 2, 0, 0,
		MENUACTION_SKY_MENU, "FED_SKY", ACTION_CLICKORARROWS, 4, 0, 0, 2, 0, 0 },

	// 5
	{ "FEH_MAP", 5, 0, 0, 0,

    },

	// 6
	{ "FEH_NGA", 1, 0, 0, 0,
		1, "FESZ_QR", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		4, "FEM_NO", ACTION_STANDARD, 1, 0, -9, 3, 0, 0,
		14, "FEM_YES", ACTION_STANDARD, 6, 0, 16, 3, 0, 0 },

	// 7
	{ "FEH_NGA", 1, 0, 0, 0,
		1, "FEN_NGS", ACTION_NONE, 0, 0, 0, 1, 0, 0,
		5, "FEN_NGX", ACTION_STANDARD, 6, 0, -90, 3, 0, 0,
		11, "FEN_MPX", ACTION_MISSIONPACK, 0, 0, 0, 3, 0, 0,
		11, "FEN_MPX", ACTION_MISSIONPACK, 0, 0, 0, 3, 0, 0,
		11, "FEN_MPX", ACTION_MISSIONPACK, 0, 0, 0, 3, 0, 0,
		11, "FEN_MPX", ACTION_MISSIONPACK, 0, 0, 0, 3, 0, 0,
		11, "FEN_MPX", ACTION_MISSIONPACK, 0, 0, 0, 3, 0, 0,
		11, "FEN_MPX", ACTION_MISSIONPACK, 0, 0, 0, 3, 0, 0,
		11, "FEN_MPX", ACTION_MISSIONPACK, 0, 0, 0, 3, 0, 0,
		11, "FEN_MPX", ACTION_MISSIONPACK, 0, 0, 0, 3, 0, 0,
		11, "FEN_MPX", ACTION_MISSIONPACK, 0, 0, 0, 3, 0, 0,
		11, "FEN_MPX", ACTION_MISSIONPACK, 0, 0, 0, 3, 0, 0,
		11, "FEN_MPX", ACTION_MISSIONPACK, 0, 0, 0, 3, 0, 0,
		11, "FEN_MPX", ACTION_MISSIONPACK, 0, 0, 0, 3, 0, 0 },

	// 8
	{ "FES_LMI", 1, 0, 0, 0,
		1, "FESZ_QM", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		4, "FEM_NO", ACTION_STANDARD, 1, 0, -9, 3, 0, 0,
		12, "FEM_YES", ACTION_STANDARD, 0, 0, 16, 3, 0, 0 },

	// 9
	{ "FET_LG", 1, 1, 0, 0,
		1, "FES_SEL", ACTION_NONE, 0, 0, 0, 1, 0, 0,
		13, "FEM_SL1", ACTION_SAVE_1, 0, 80, -114, 1, 0, -1,
		13, "FEM_SL2", ACTION_SAVE_2, 0, 80, -94, 1, 0, -1,
		13, "FEM_SL3", ACTION_SAVE_3, 0, 80, -74, 1, 0, -1,
		13, "FEM_SL4", ACTION_SAVE_4, 0, 80, -54, 1, 0, -1,
		13, "FEM_SL5", ACTION_SAVE_5, 0, 80, -34, 1, 0, -1,
		13, "FEM_SL6", ACTION_SAVE_6, 0, 80, -14, 1, 0, -1,
		13, "FEM_SL7", ACTION_SAVE_7, 0, 80, 6, 1, 0, -1,
		13, "FEM_SL8", ACTION_SAVE_8, 0, 80, 26, 1, 0, -1,
		13, "FEM_SL9", ACTION_SAVE_9, 0, 80, 46, 1, 0, -1,
		13, "FEM_SLA", ACTION_SAVE_10, 0, 80, 66, 1, 0, -1,
		13, "FEM_SLB", ACTION_SAVE_11, 0, 80, 86, 1, 0, -1,
		13, "FEM_SLC", ACTION_SAVE_12, 0, 80, 106, 1, 0, -1 },

	// 10
	{ "FEH_DEL", 1, 2, 0, 0,
		1, "FES_SED", ACTION_NONE, 0, 0, 0, 1, 0, 0,
		13, "FEM_SL1", ACTION_SAVE_1, 0, 80, -114, 1, 0, -1,
		13, "FEM_SL2", ACTION_SAVE_2, 0, 80, -94, 1, 0, -1,
		13, "FEM_SL3", ACTION_SAVE_3, 0, 80, -74, 1, 0, -1,
		13, "FEM_SL4", ACTION_SAVE_4, 0, 80, -54, 1, 0, -1,
		13, "FEM_SL5", ACTION_SAVE_5, 0, 80, -34, 1, 0, -1,
		13, "FEM_SL6", ACTION_SAVE_6, 0, 80, -14, 1, 0, -1,
		13, "FEM_SL7", ACTION_SAVE_7, 0, 80, 6, 1, 0, -1,
		13, "FEM_SL8", ACTION_SAVE_8, 0, 80, 26, 1, 0, -1,
		13, "FEM_SL9", ACTION_SAVE_9, 0, 80, 46, 1, 0, -1,
		13, "FEM_SLA", ACTION_SAVE_10, 0, 80, 66, 1, 0, -1,
		13, "FEM_SLB", ACTION_SAVE_11, 0, 80, 86, 1, 0, -1,
		13, "FEM_SLC", ACTION_SAVE_12, 0, 80, 106, 1, 0, -1 },

	{ "FET_LG", 9, 0, 0, 0,
		1, "FESZ_QL", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		4, "FEM_NO", ACTION_STANDARD, 9, 0, -9, 3, 0, 0,
		3, "FEM_YES", ACTION_STANDARD, 13, 0, 16, 3, 0, 0 },

	{ "FEH_DEL", 10, 0, 0, 0,
		1, "FESZ_QD", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		4, "FEM_NO", ACTION_STANDARD, 10, 0, -9, 3, 0, 0,
		3, "FEM_YES", ACTION_STANDARD, 14, 0, 16, 3, 0, 0 },

	{ "FET_LG", 9, 0, 0, 0 },

	{ "FEH_DEL", 10, 0, 0, 0 },

	{ "FEH_DEL", 1, 0, 0, 0,
		1, "FES_DSC", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEM_OK", ACTION_STANDARD, 1, 0, 16, 3, 0, 0 },

	{ "FET_SG", 255, 0, 0, 0,
		1, "FES_SES", ACTION_NONE, 0, 0, 0, 1, 0, 0,
		18, "FEM_SL1", ACTION_SAVE_1, 0, 80, -114, 1, 0, -1,
		18, "FEM_SL2", ACTION_SAVE_2, 0, 80, -94, 1, 0, -1,
		18, "FEM_SL3", ACTION_SAVE_3, 0, 80, -74, 1, 0, -1,
		18, "FEM_SL4", ACTION_SAVE_4, 0, 80, -54, 1, 0, -1,
		18, "FEM_SL5", ACTION_SAVE_5, 0, 80, -34, 1, 0, -1,
		18, "FEM_SL6", ACTION_SAVE_6, 0, 80, -14, 1, 0, -1,
		18, "FEM_SL7", ACTION_SAVE_7, 0, 80, 6, 1, 0, -1,
		18, "FEM_SL8", ACTION_SAVE_8, 0, 80, 26, 1, 0, -1,
		18, "FEM_SL9", ACTION_SAVE_9, 0, 80, 46, 1, 0, -1,
		18, "FEM_SLA", ACTION_SAVE_10, 0, 80, 66, 1, 0, -1,
		18, "FEM_SLB", ACTION_SAVE_11, 0, 80, 86, 1, 0, -1,
		18, "FEM_SLC", ACTION_SAVE_12, 0, 80, 106, 1, 0, -1 },

	{ "FET_SG", 16, 0, 0, 0,
		1, "FESZ_QZ", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		4, "FEM_NO", ACTION_STANDARD, 16, 0, -9, 3, 0, 0,
		3, "FEM_YES", ACTION_STANDARD, 18, 0, 16, 3, 0, 0 },


	{ "FET_SG", 16, 0, 0, 0 },


	{ "FET_SG", 16, 0, 0, 0,
		1, "FES_SSC", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		54, "FEM_OK", ACTION_STANDARD, 0, 0, 16, 3, 0, 0 },


	{ "FET_SG", 42, 0, 0, 0,
		1, "", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEM_OK", ACTION_STANDARD, 0, 0, 46, 3, 0, 0 },


	{ "FET_LG", 42, 0, 0, 0,
		1, "", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEM_OK", ACTION_STANDARD, 0, 0, 46, 3, 0, 0 },

	// 22
	{ "FET_SG", 1, 0, 0, 0,
		1, "FES_CHE", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEM_OK", ACTION_STANDARD, 16, 0, 16, 3, 0, 0 },

	// 23 Restore defaults - Display Setup
	{ "", 23, 0, 0, 0 },

	// 24 Restore detaults - Audio Setup
	{ "", 24, 0, 0, 0 },

	// 25 Restore defaults - Controller Setup
	{ "", 25, 0, 0, 0 },

	// 26 Custom Tracks Options
	{ "FEH_TIT", 3, 3, 0, 0,
		1, "FEA_SUB", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		63, "FEA_MPM", ACTION_CLICKORARROWS, 26, 0, -70, 2, 0, 0,
		64, "FEA_AMS", ACTION_CLICKORARROWS, 26, 0, 0, 2, 0, 0,
		6, "FEA_SCN", ACTION_STANDARD, 26, 0, 0, 3, 0, 0 },

	// 27 Graphics Setup
	{ "FEH_GFX", 27, 0, 70, 110,
		56, "FED_RES", ACTION_CLICKORARROWS, 27, 0, -154, 2, 0, 0,
		26, "FED_WIS", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		MENUACTION_VSYNC, "FED_VSN", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		24, "FEM_FRM", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		MENUACTION_EFFECTS_QUALITY, "FED_EFF", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		MENUACTION_SHADOWS_QUALITY, "FED_SHA", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		MENUACTION_PIPE_QUALITY, "FED_RFQ", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		61, "FEM_LOD", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		MENUACTION_SHADOWS_DISTANCE, "FED_SHD", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		44, "FED_AAS", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		MENUACTION_TEXTURE_FILTERMODE, "FED_TXF", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		MENUACTION_TRAILS, "FED_TRA", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		MENUACTION_NEOVEH, "NEOVEH", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0},

	// 28 Language
	{ "FEH_LAN", 28, 4, 0, 0,
		37, "FEL_ENG", ACTION_STANDARD, 28, 0, -72, 3, 0, 0,
		//38, "FEL_GER", ACTION_STANDARD, 28, 0, 0, 3, 0, 0,
        //39, "FEL_SPA", ACTION_STANDARD, 28, 0, 0, 3, 0, 0,
        38, "FEL_ITA", ACTION_STANDARD, 28, 0, 0, 3, 0, 0,
		39, "FEL_BRA", ACTION_STANDARD, 28, 0, 0, 3, 0, 0,
		40, "FEL_POL", ACTION_STANDARD, 28, 0, 0, 3, 0, 0 },

	// 29
	{ "FET_SG", 1, 0, 0, 0,
		1, "FED_LWR", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEC_OKK", ACTION_STANDARD, 16, 0, 0, 0, 0, 0 },

	// 30
	{ "FET_SG", 16, 0, 0, 0,
		1, "FEC_SVU", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEC_OKK", ACTION_STANDARD, 16, 0, 0, 0, 0, 0 },

	// 31
	{ "FET_LG", 16, 0, 0, 0,
		1, "FEC_SVU", ACTION_NONE, 0, 0, 0, 0, 0, 0 },

	// 32
	{ "FET_LG", 1, 0, 0, 0,
		1, "FEC_LUN", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		2, "FEDS_TB", ACTION_STANDARD, 1, 0, 0, 0, 0, 0 },

	// 33 Options
	{ "FET_OPT", 42, 5, 0, 0,
		5, "FEO_CON", ACTION_STANDARD, 36, 0, -72, 3, 0, 0,
		5, "FEO_AUD", ACTION_STANDARD, 3, 0, 0, 3, 0, 0,
		5, "FEO_DIS", ACTION_STANDARD, 4, 0, 0, 3, 0, 0,
		5, "FEO_GFX", ACTION_STANDARD, 27, 0, 0, 3, 0, 0,
		5, "FEO_LAN", ACTION_STANDARD, 28, 0, 0, 3, 0, 0,
		5, "FEO_DLC", ACTION_STANDARD, 45, 0, 0, 3, 0, 0,
		5, "FEO_UPT", ACTION_STANDARD, 44, 0, 0, 3, 0, 0,
		2, "FEDS_TB", ACTION_STANDARD, 0, 0, 0, 3, 0, 0 },

	// 34
	{ "FEM_MM", 255, 0, 0, 0,
		5, "FEP_STG", ACTION_STANDARD, 1, 0, -54, 3, 0, 0,
		5, "FEP_OPT", ACTION_STANDARD, 33, 0, 0, 3, 0, 0,
		5, "FEP_QUI", ACTION_STANDARD, 35, 0, 0, 3, 0, 0 },

	// 35
	{ "FET_QG", 1, 6, 0, 0,
		1, "FEQ_SRE", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		55, "FEM_NO", ACTION_STANDARD, 42, 0, -9, 3, 0, 0,
		53, "FEM_YES", ACTION_STANDARD, 42, 0, 16, 3, 0, 0 },


	// 36 Controller Setup
	{ "FET_CTL", 37, 0, 0, 0,
		5, "FEC_RED", ACTION_STANDARD, 37, 0, -29, 3, 0, 0,
		5, "FEC_MOU", ACTION_STANDARD, 39, 0, 0, 3, 0, 0,
		5, "FEC_CTL", ACTION_STANDARD, MENU_PAGE_CONTROLLER_SETUP, 0, 0, 3, 0, 0,
		5, "FET_DEF", ACTION_STANDARD, 25, 0, 70, 3, 0, 0,
		2, "FEDS_TB", ACTION_STANDARD, 0, 0, 0, 3, 0, 0 },


	// 37 Redefine Controls
	{ "FET_CTL", 38, 0, 0, 0,
	8, "FET_CFT", ACTION_STANDARD, 38, 0, -34, 3, 0, 0,
	9, "FET_CCR", ACTION_STANDARD, 38, 0, 0, 3, 0, 0 },

	// 38
	{ "FET_CTL", 39, 0, 0, 0 },

	// 39 Mouse Settings
	{ "FEM_MOU", 40, 1, 0, 0,
		62, "FEC_MSH", ACTION_CLICKORARROWS, 39, 0, -94, 2, 0, 0,
		46, "FEC_IVV", ACTION_CLICKORARROWS, 39, 0, 0, 2, 0, 0,
		59, "FET_MST", ACTION_CLICKORARROWS, 39, 0, 0, 2, 0, 0,
		60, "FET_MFL", ACTION_CLICKORARROWS, 39, 0, 0, 2, 0, 0 },

	// 40 Controller Settings
	{ "FEH_CTL", 40, 0, 0, 0,
		MENUACTION_CTRL_TYPE, "FEC_CFG", ACTION_CLICKORARROWS, MENU_PAGE_CONTROLLER_SETUP, 0, -160, 2, 0, 0,
		MENUACTION_PAD_FRONTEND_PAGE, "FEC_CDP", ACTION_CLICKORARROWS, MENU_PAGE_CONTROLLER_SETUP, 0, 0, 2, 0, 0,
		//MENUACTION_INVERTLOOK, "FEC_ILU", ACTION_CLICKORARROWS, MENU_PAGE_CONTROLLER_SETUP, 0, 0, 2, 0, 0,
		MENUACTION_VIBRATION, "FEC_VIB", ACTION_CLICKORARROWS, MENU_PAGE_CONTROLLER_SETUP, 0, 0, 2, 0, 0,
		5, "FEC_ADO", ACTION_STANDARD, MENU_PAGE_ADDITIONAL_CONTROLLER, 0, 0, 3, 0, 0 },

	// 41 Pause menu
	{ "", 255, 0, 0, 0 },

	// 42 2 menus serving a special purpose, cba moving
	{ "", 0, 0, 0, 0 },

	// 43
	{ "", 0, 0, 0, 0 },

	// 44 Game Updates
	{ "FEH_UPT", 33, 6, 0, 0,
		MENUACTION_UPDATER_BUTTON, "FEU_UPC", ACTION_UPDATER, 44, 0, 138, 3, 1, 0,
		MENUACTION_CHECKING_PERIOD, "FEU_CHP", ACTION_CLICKORARROWS, 44, 0, 0, 2, 1, 0,
		MENUACTION_AUTOINSTALL_UPDATES, "FEU_AUI", ACTION_CLICKORARROWS, 44, 0, 0, 2, 1, 0,
		2, "FEDS_TB", ACTION_STANDARD, 0, 0, 0, 3, 1, 0 },

	// 45 Downloadable Content
	{ "FEH_DLC", 33, 5, 0, 0,
		1, "FEE_HEA", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		2, "FEDS_TB", ACTION_STANDARD, 0, 0, 48, 3, 1, 0,	// Hacky hacky
		MENUACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 60, -70, 1, 0, -1,
		MENUACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 0, 0, 1, 0, -1,
		MENUACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 0, 0, 1, 0, -1,
		MENUACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 0, 0, 1, 0, -1,
		MENUACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 0, 0, 1, 0, -1,
		MENUACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 0, 0, 1, 0, -1,
		MENUACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 0, 0, 1, 0, -1,
		MENUACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 0, 0, 1, 0, -1,
		MENUACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 0, 0, 1, 0, -1,
		MENUACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 0, 0, 1, 0, -1,
		MENUACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 0, 0, 1, 0, -1,
		MENUACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 0, 0, 1, 0, -1 },

	// 46 Downloadable Content - no DLCs available
	{ "FEH_DLC", 33, 5, 0, 0,
		1, "FEE_NON", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEM_OK", ACTION_STANDARD, 33, 0, 16, 3, 0, 0 },

	// 47 Downloadable Content - game restart required
	{ "FEH_DLC", 33, 5, 0, 0,
		1, "FEE_RES", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEM_OK", ACTION_STANDARD, 33, 0, 16, 3, 0, 0 },

	// 48 Downloadable Content - DLC installation prompt
	{ "FEH_DLC", 45, 2, 0, 0,
		1, "FEE_IXX", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEM_NO", ACTION_STANDARD, 45, 0, -9, 3, 0, 0,
		57, "FEM_YES", ACTION_STANDARD, 44, 0, 16, 3, 0, 0 },

	// 49 DLC activation
	{ "FEH_DLC", 45, 0, 0, 0,
		1, "FEE_KEY", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		MENUACTION_ACTIVATE_SERIAL, "FEE_ACT", ACTION_SERIAL, 0, 0, 78, 3, 1, 0,
		2, "FEDS_TB", ACTION_STANDARD, 0, 0, 48, 3, 1, 0 },

	// 50 Restore defaults - Graphics Setup
	{ "FEH_GFX", 27, 9, 0, 0,
		1, "FED_RDP", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEM_NO", ACTION_STANDARD, 27, 0, -9, 3, 0, 0,
		57, "FEM_YES", ACTION_STANDARD, 27, 0, 16, 3, 0, 0 },

	// 51 Additional Controller Options
	{ "FEH_CTL", MENU_PAGE_CONTROLLER_SETUP, 3, 0, 0,
		MENUACTION_INVERTLOOK, "FEC_ILU", ACTION_CLICKORARROWS, MENU_PAGE_ADDITIONAL_CONTROLLER, 0, -70, 2, 0, 0,
		MENUACTION_SOUTHPAW, "FEC_SOU", ACTION_CLICKORARROWS, MENU_PAGE_CONTROLLER_SETUP, 0, 0, 2, 0, 0,
		MENUACTION_BUTTONSTYLE, "FEC_STL",  ACTION_CLICKORARROWS, MENU_PAGE_ADDITIONAL_CONTROLLER, 0, 0, 2, 0, 0 },

	// 52 VCS Map Menu
	{ "FEH_MAP", 42, 2, 0, 0,

	},
};

// ENGLISH, ITALIAN, BRAZILIAN, POLISH
const char* legal_lang[NUM_LANGUAGES] = { "legal_EN", "legal_IT", "legal_BR", "legal_PL" };

WRAPPER void CMenuManager::Process(void) { EAXJMP(0x57B440); }
WRAPPER void CMenuManager::DrawFrontEnd(void) { EAXJMP(0x57C290); }
WRAPPER void CMenuManager::DrawWindow(const CRect& rect, const char* pKey, unsigned char nColour, CRGBA backColour, bool bUnused, bool bBackground) { WRAPARG(rect); WRAPARG(pKey); WRAPARG(nColour); WRAPARG(backColour); WRAPARG(bUnused); WRAPARG(bBackground); EAXJMP(0x573EE0); }
WRAPPER void CMenuManager::SmallMessageScreen(const char* pMessage) { WRAPARG(pMessage); EAXJMP(0x574010); }
WRAPPER void CMenuManager::InitialiseChangedLanguageSettings(bool bRemapButtons) { WRAPARG(bRemapButtons); EAXJMP(0x573260); }
WRAPPER void CMenuManager::PrintBrief() { EAXJMP(0x576320); }
WRAPPER void CMenuManager::DrawControllerScreenExtraText(int nUnk) { WRAPARG(nUnk); EAXJMP(0x57D8D0); }
WRAPPER bool CMenuManager::CheckHover(int, int, int, int) { EAXJMP(0x57C4F0); }
WRAPPER void CMenuManager::ProcessMissionPackNewGame() { EAXJMP(0x57D520); }
WRAPPER void CMenuManager::DoSettingsBeforeStartingAGame() { EAXJMP(0x573330); }
WRAPPER void CMenuManager::ScrollRadioStations(signed char nDirection) { WRAPARG(nDirection); EAXJMP(0x573A00); }

void CMenuManager::SaveSettings()
{
	CFileMgr::SetDirMyDocuments();
	
	ControlsManager.SaveToFile();
	if ( FILE* hFile = CFileMgr::OpenFile(SET_FILE_NAME, "wb") )
	{
		const DWORD		dwFileVersion = SET_FILE_VERSION;

		CFileMgr::Write(hFile, &dwFileVersion, sizeof(dwFileVersion));
		
		// Controls Setup
		CFileMgr::Write(hFile, &CCamera::m_fMouseAccelHorzntl, sizeof(CCamera::m_fMouseAccelHorzntl));
		CFileMgr::Write(hFile, &MousePointerStateHelper.m_bHorizontalInvert, sizeof(MousePointerStateHelper.m_bHorizontalInvert));
		CFileMgr::Write(hFile, &CVehicle::m_bEnableMouseSteering, sizeof(CVehicle::m_bEnableMouseSteering));
		CFileMgr::Write(hFile, &CVehicle::m_bEnableMouseFlying, sizeof(CVehicle::m_bEnableMouseFlying));
		//CFileMgr::Write(hFile, &m_nController, sizeof(m_nController));
		CFileMgr::Write(hFile, &invertPadX1, sizeof(invertPadX1));
		CFileMgr::Write(hFile, &invertPadY1, sizeof(invertPadY1));
		CFileMgr::Write(hFile, &invertPadX2, sizeof(invertPadX2));
		CFileMgr::Write(hFile, &invertPadY2, sizeof(invertPadY2));
		CFileMgr::Write(hFile, &swapPadAxis1, sizeof(swapPadAxis1));
		CFileMgr::Write(hFile, &swapPadAxis2, sizeof(swapPadAxis2));
		CFileMgr::Write(hFile, &CPad::SavedMode, sizeof(CPad::SavedMode));
		CFileMgr::Write(hFile, &m_bVibrationEnabled, sizeof(m_bVibrationEnabled));
		CFileMgr::Write(hFile, &CPad::bInvertLook4Pad, sizeof(CPad::bInvertLook4Pad));
		CFileMgr::Write(hFile, &CPad::bSouthpaw, sizeof(CPad::bSouthpaw));
		CFileMgr::Write(hFile, &CFont::bX360Buttons, sizeof(CFont::bX360Buttons));

		// Audio Setup
		CFileMgr::Write(hFile, &m_nSfxVolume, sizeof(m_nSfxVolume));
		CFileMgr::Write(hFile, &m_nRadioVolume, sizeof(m_nRadioVolume));
		CFileMgr::Write(hFile, &m_nRadioStation, sizeof(m_nRadioStation));
		CFileMgr::Write(hFile, &m_bRadioAutoSelect, sizeof(m_bRadioAutoSelect));
		CFileMgr::Write(hFile, &m_bTracksAutoScan, sizeof(m_bTracksAutoScan));
		CFileMgr::Write(hFile, &m_nRadioMode, sizeof(m_nRadioMode));

		// Display Setup
		CFileMgr::Write(hFile, &m_dwBrightness, sizeof(m_dwBrightness));
		CFileMgr::Write(hFile, &m_fSafeZone, sizeof(m_fSafeZone));
		CFileMgr::Write(hFile, &m_bEnableSkyMenu, sizeof(m_bEnableSkyMenu));
		CFileMgr::Write(hFile, &m_bShowSubtitles, sizeof(m_bShowSubtitles));
		CFileMgr::Write(hFile, &m_bHudOn, sizeof(m_bHudOn));
		CFileMgr::Write(hFile, &m_dwRadarMode, sizeof(m_dwRadarMode));
		CFileMgr::Write(hFile, &m_bSavePhotos, sizeof(m_bSavePhotos));
		CFileMgr::Write(hFile, &m_bMapLegend, sizeof(m_bMapLegend));

		// Other
		CFileMgr::Write(hFile, &field_AC, sizeof(field_AC));
		CFileMgr::Write(hFile, CText::GetLanguageAcronymByIndex(m_nLanguage), 2);

		// Graphics Setup
		//const FxQuality_e		nFxQuality = g_fx.GetFxQuality();
		const FxQuality_e		nEffectsQuality = Fx_c::GetEffectsQuality();
		const unsigned char		nFilterQuality = Fx_c::GetTextureFilteringQuality();
		const eShadowQuality	nShadowQuality = CShadows::GetShadowQuality();
		const float				fShadowDist = CShadows::GetShadowDistance();
		const RwInt32			nSubSystem = RwEngineGetCurrentSubSystem();
		const bool				bTrailsEnabled = CPostEffects::TrailsEnabled();
		const uint8				vehpipe = CarPipe::PipeSwitch;
		const eEnvMapSize		nReflectionQuality = CarPipe::envMapSize;

		//CFileMgr::Write(hFile, &m_bMipMapping, sizeof(m_bMipMapping));
		CFileMgr::Write(hFile, &m_dwAppliedAntiAliasingLevel, sizeof(m_dwAppliedAntiAliasingLevel));
		//CFileMgr::Write(hFile, &nFxQuality, sizeof(nFxQuality));
		CFileMgr::Write(hFile, &nEffectsQuality, sizeof(nEffectsQuality));
		CFileMgr::Write(hFile, &nFilterQuality, sizeof(nFilterQuality));
		CFileMgr::Write(hFile, &nShadowQuality, sizeof(nShadowQuality));
		CFileMgr::Write(hFile, &fShadowDist, sizeof(fShadowDist));
		CFileMgr::Write(hFile, &m_fDrawDistance, sizeof(m_fDrawDistance));
		CFileMgr::Write(hFile, &m_bAspectRatioMode, sizeof(m_bAspectRatioMode));
		CFileMgr::Write(hFile, &m_bFrameLimiterMode, sizeof(m_bFrameLimiterMode));
		CFileMgr::Write(hFile, &bTrailsEnabled, sizeof(bTrailsEnabled));
		CFileMgr::Write(hFile, &m_dwAppliedResolution, sizeof(m_dwAppliedResolution));
		CFileMgr::Write(hFile, &nSubSystem, sizeof(nSubSystem));
		CFileMgr::Write(hFile, &vehpipe, sizeof(vehpipe));
		CFileMgr::Write(hFile, &nReflectionQuality, sizeof(nReflectionQuality));

		CFileMgr::CloseFile(hFile);
	}
	CFileMgr::SetDir("");
}

void CMenuManager::LoadSettings()
{
	CFileMgr::SetDirMyDocuments();
	
	unsigned char	nPrevLanguage = m_nLanguage;

	ControlsManager.LoadFromFile();
	if ( FILE* hFile = CFileMgr::OpenFile(SET_FILE_NAME, "rb") )
	{
		DWORD		dwFileVersion;

		CFileMgr::Read(hFile, &dwFileVersion, sizeof(dwFileVersion));
		if ( dwFileVersion == SET_FILE_VERSION )
		{
			// Controls Setup
			CFileMgr::Read(hFile, &CCamera::m_fMouseAccelHorzntl, sizeof(CCamera::m_fMouseAccelHorzntl));
			CFileMgr::Read(hFile, &MousePointerStateHelper.m_bHorizontalInvert, sizeof(MousePointerStateHelper.m_bHorizontalInvert));
			CFileMgr::Read(hFile, &CVehicle::m_bEnableMouseSteering, sizeof(CVehicle::m_bEnableMouseSteering));
			CFileMgr::Read(hFile, &CVehicle::m_bEnableMouseFlying, sizeof(CVehicle::m_bEnableMouseFlying));
			//CFileMgr::Read(hFile, &m_nController, sizeof(m_nController));
			CFileMgr::Read(hFile, &invertPadX1, sizeof(invertPadX1));
			CFileMgr::Read(hFile, &invertPadY1, sizeof(invertPadY1));
			CFileMgr::Read(hFile, &invertPadX2, sizeof(invertPadX2));
			CFileMgr::Read(hFile, &invertPadY2, sizeof(invertPadY2));
			CFileMgr::Read(hFile, &swapPadAxis1, sizeof(swapPadAxis1));
			CFileMgr::Read(hFile, &swapPadAxis2, sizeof(swapPadAxis2));
			CFileMgr::Read(hFile, &CPad::SavedMode, sizeof(CPad::SavedMode));
			CFileMgr::Read(hFile, &m_bVibrationEnabled, sizeof(m_bVibrationEnabled));
			CFileMgr::Read(hFile, &CPad::bInvertLook4Pad, sizeof(CPad::bInvertLook4Pad));
			CFileMgr::Read(hFile, &CPad::bSouthpaw, sizeof(CPad::bSouthpaw));
			CFileMgr::Read(hFile, &CFont::bX360Buttons, sizeof(CFont::bX360Buttons));

			// Audio Setup
			CFileMgr::Read(hFile, &m_nSfxVolume, sizeof(m_nSfxVolume));
			CFileMgr::Read(hFile, &m_nRadioVolume, sizeof(m_nRadioVolume));
			CFileMgr::Read(hFile, &m_nRadioStation, sizeof(m_nRadioStation));
			CFileMgr::Read(hFile, &m_bRadioAutoSelect, sizeof(m_bRadioAutoSelect));
			CFileMgr::Read(hFile, &m_bTracksAutoScan, sizeof(m_bTracksAutoScan));
			CFileMgr::Read(hFile, &m_nRadioMode, sizeof(m_nRadioMode));

			// Display Setup
			CFileMgr::Read(hFile, &m_dwBrightness, sizeof(m_dwBrightness));
			CFileMgr::Read(hFile, &m_fSafeZone, sizeof(m_fSafeZone));
			CFileMgr::Read(hFile, &m_bEnableSkyMenu, sizeof(m_bEnableSkyMenu));
			CFileMgr::Read(hFile, &m_bShowSubtitles, sizeof(m_bShowSubtitles));
			CFileMgr::Read(hFile, &m_bHudOn, sizeof(m_bHudOn));
			CFileMgr::Read(hFile, &m_dwRadarMode, sizeof(m_dwRadarMode));
			CFileMgr::Read(hFile, &m_bSavePhotos, sizeof(m_bSavePhotos));
			CFileMgr::Read(hFile, &m_bMapLegend, sizeof(m_bMapLegend));

			// Other
			char		LangAcronym[2];

			CFileMgr::Read(hFile, &field_AC, sizeof(field_AC));
			CFileMgr::Read(hFile, LangAcronym, 2);

			// Graphics Setup
			FxQuality_e			/*nFxQuality, */nEffectsQuality;
			eShadowQuality		nShadowQuality;
			float				fShadowDist;
			unsigned char		nFilterQuality;
			bool				bTrailsEnabled;
			uint8				vehpipe;
			eEnvMapSize			nReflectionQuality;

			//CFileMgr::Read(hFile, &m_bMipMapping, sizeof(m_bMipMapping));
			CFileMgr::Read(hFile, &m_dwAntiAliasingLevel, sizeof(m_dwAppliedAntiAliasingLevel));
			//CFileMgr::Read(hFile, &nFxQuality, sizeof(nFxQuality));
			CFileMgr::Read(hFile, &nEffectsQuality, sizeof(nEffectsQuality));
			CFileMgr::Read(hFile, &nFilterQuality, sizeof(nFilterQuality));
			CFileMgr::Read(hFile, &nShadowQuality, sizeof(nShadowQuality));
			CFileMgr::Read(hFile, &fShadowDist, sizeof(fShadowDist));
			CFileMgr::Read(hFile, &m_fDrawDistance, sizeof(m_fDrawDistance));
			CFileMgr::Read(hFile, &m_bAspectRatioMode, sizeof(m_bAspectRatioMode));
			CFileMgr::Read(hFile, &m_bFrameLimiterMode, sizeof(m_bFrameLimiterMode));
			CFileMgr::Read(hFile, &bTrailsEnabled, sizeof(bTrailsEnabled));
			CFileMgr::Read(hFile, &m_dwResolution, sizeof(m_dwAppliedResolution));
			CFileMgr::Read(hFile, &field_DC, sizeof(field_DC));
			CFileMgr::Read(hFile, &vehpipe, sizeof(vehpipe));
			CFileMgr::Read(hFile, &nReflectionQuality, sizeof(nReflectionQuality));

			// Apply sets
			//CCamera::m_bUseMouse3rdPerson = m_nController == 0;
			ms_lodDistScale = m_fDrawDistance;
			// Fuck everything x2
			//((void(__thiscall*)(int,float,bool))0x747200)(0xC92134, m_dwBrightness * (1.0f/512.0f), true);
			m_dwAppliedAntiAliasingLevel = m_dwAntiAliasingLevel;
			m_bChangeVideoMode = true;
			m_nLanguage = CText::GetLanguageIndexByAcronym(LangAcronym);
			RsGlobal.frameLimit = RsGlobalFrameLimits[m_bFrameLimiterMode];

			//g_fx.SetFxQuality(nFxQuality);
			Fx_c::SetEffectsQuality(nEffectsQuality);
			Fx_c::SetTextureFilteringQuality(nFilterQuality);

			CPostEffects::SetTrailsState(bTrailsEnabled);
			CarPipe::PipeSwitch = vehpipe;
			CarPipe::SetReflectionQuality(nReflectionQuality);

			CShadows::SetShadowQuality(nShadowQuality);
			CShadows::SetShadowDistance(fShadowDist);
			CShadows::InitialiseChangedSettings();

			AudioEngine.SetMusicMasterVolume(m_nRadioVolume);
			AudioEngine.SetEffectsMasterVolume(m_nSfxVolume);
			AudioEngine.SetRadioAutoRetuneOnOff(m_bRadioAutoSelect);
			AudioEngine.RetuneRadio(m_nRadioStation);

			if ( m_nLanguage == nPrevLanguage )
				m_bLanguageChanged = false;
			else
			{
				m_bLanguageChanged = true;
				TheText.Load(false);
				m_bReinitLanguageSettings = true;
				InitialiseChangedLanguageSettings(false);

				// Just for the sake of it
				OutputDebugString("The previously saved language is now in use");
			}

			// Success
			CFileMgr::CloseFile(hFile);
			CFileMgr::SetDir("");
			return;
		}

		CFileMgr::CloseFile(hFile);
	}

	// Failure condition code
	SetDefaultPreferences(3);
	SetDefaultPreferences(4);
	SetDefaultPreferences(27);
	SetDefaultPreferences(36);

	field_DC = 0;
	m_nLanguage = nPrevLanguage;

	CFileMgr::SetDir("");
}

void SwitchToSkyMenuInstance() {
	static int m_dwTimeToDisplayMessage;
	static bool m_bTimingHasBeenGiven;
	static bool m_bDisplayMessageScreen;

	int m_dwTime = 2000;

	if (FrontEndMenuManager.m_bSwitchToSkyMenu) {
		if (!m_bTimingHasBeenGiven) {
			m_dwTimeToDisplayMessage = CTimer::m_snTimeInMillisecondsPauseMode + m_dwTime;
			m_bTimingHasBeenGiven = true;
		}

		if (CTimer::m_snTimeInMillisecondsPauseMode < m_dwTimeToDisplayMessage)
			m_bDisplayMessageScreen = true;
		else
			m_bDisplayMessageScreen = false;

		if (m_bDisplayMessageScreen) {
			CSprite2d::DrawRect(CRect(0, RsGlobal.MaximumHeight, RsGlobal.MaximumWidth, 0), CRGBA(0, 0, 0, 255));

			static float alpha;
			CFont::SetBackground(0, 0);
			CFont::SetProportional(true);
			CFont::SetFontStyle(FONT_RageItalic);
			CFont::SetOrientation(ALIGN_Right);
			CFont::SetRightJustifyWrap(0.0);
			CFont::SetEdge(0);
			CFont::SetScale(_width(1.25f), _height(2.1f));

			static bool reset;
			if (!reset) {
				if (alpha < 255.0f)
					alpha += CTimer::ms_fTimeScale * 0.01 * 1000.0;

				if (alpha >= 255.0f)
					reset = true;
			}
			else {
				if (alpha > 0.0f)
					alpha += CTimer::ms_fTimeScale * 0.01 * -1000.0;

				if (alpha <= 0.0f)
					reset = false;
			}

			if (alpha >= 255)
				alpha = 255;
			if (alpha <= 0)
				alpha = 0;

			CFont::SetDropColor(CRGBA(0, 0, 0, static_cast<float>(alpha)));
			CFont::SetColor(CRGBA(MENU_PINK_R, MENU_PINK_G, MENU_PINK_B, static_cast<float>(alpha)));

			CFont::PrintString(_x(40.0 + (UI_SAFEZONE * 1.8f)), _y(18.0f + (UI_SAFEZONE)), TheText.Get("LOADCOL"));
		}
		else {
			int i;
			for (i = 0; i < MENUPAGES; i++) {
				FrontEndMenuManager._aScreens[i] = FrontEndMenuManager._aScreensNone[i];
				FrontEndMenuManager._MenuEntriesList[i] = FrontEndMenuManager._aScreensNone[i];
			}

			if (i >= MENUPAGES) {
				FrontEndMenuManager.m_bMenuPagesHasBeenStored = false;

				FrontEndMenuManager.m_bSwitchToSkyMenu = FrontEndMenuManager.m_bSwitchToSkyMenu == false;
				FrontEndMenuManager.m_bEnableSkyMenu = FrontEndMenuManager.m_bEnableSkyMenu == false;
				m_bTimingHasBeenGiven = false;

				if (FrontEndMenuManager.m_bEnableSkyMenu)
					FrontEndMenuManager.m_dwSelectedMenuItem = 8;
				else
					FrontEndMenuManager.m_dwSelectedMenuItem = 7;

				FrontEndMenuManager.SaveSettings();
			}
		}
	}
}

static Reversed DrawStandardMenus_kill(0x5794A0, 0x57B43F);
void CMenuManager::DrawStandardMenus(bool bDrawMenu)
{
	static bool Once;

	if (!Once && FrontEndMenuManager.m_bEnableSkyMenu) {
		FrontEndMenuManager.m_bIsMenuSwitched = false;
		FrontEndMenuManager.m_nSelectedSkyMenuItem = 0;
		FrontEndMenuManager.m_bBottomMenu = true;

		Once = true;
	}

	// SkyMenu
	if (!FrontEndMenuManager.m_bSwitchToSkyMenu) {
		if (FrontEndMenuManager.m_bEnableSkyMenu) {
			DrawSkyMenu();

			FrontEndMenuManager.m_vecMenuColumnPosn[1] = CVector2D(18.0f, 0.0f);
			FrontEndMenuManager.m_vecMenuColumnSize = CVector2D(0.64f, 1.16f);
			m_nBackgroundColor = { 130, 130, 130, 130 };

			if (!FrontEndMenuManager.m_bMenuPagesHasBeenStored) {
				for (int i = 0; i < MENUPAGES; i++) {
					FrontEndMenuManager._aScreens[i] = m_SkyMenus[i];
					FrontEndMenuManager._MenuEntriesList[i] = m_SkyMenus[i];
				}
				FrontEndMenuManager.m_bMenuPagesHasBeenStored = true;
			}
		}
		else {
			FrontEndMenuManager.m_vecMenuColumnPosn[1] = CVector2D(MENU_TEXT_POSITION_RCOLUMN, 0.0f);
			FrontEndMenuManager.m_vecMenuColumnSize = CVector2D(0.7f, 1.0f);
			m_nBackgroundColor = { 255, 255, 255, 130 };

			if (!FrontEndMenuManager.m_bMenuPagesHasBeenStored) {
				for (int i = 0; i < MENUPAGES; i++) {
					FrontEndMenuManager._aScreens[i] = ms_pMenus[i];
					FrontEndMenuManager._MenuEntriesList[i] = ms_pMenus[i];
				}
				FrontEndMenuManager.m_bMenuPagesHasBeenStored = true;
			}
		}

		CFont::SetBackground(0, 0);
		CFont::SetProportional(true);
		CFont::SetOrientation(1);
		CFont::SetWrapx(RsGlobal.MaximumWidth - 10);
		CFont::SetRightJustifyWrap(10.0f);
		CFont::SetCentreSize(RsGlobal.MaximumWidth);

		// Scrollbar
		if (FrontEndMenuManager.m_bEnableSkyMenu) {
			if (FrontEndMenuManager._aScreens[m_bCurrentMenuPage].topMargin != 0 || FrontEndMenuManager._aScreens[m_bCurrentMenuPage].bottomMargin != 0)
			{
				m_apMouseTextures[0].Draw(CRect(_x(19.5f), _y(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].topMargin + 2.5f), _x(29.5f), _ydown(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].bottomMargin + 2.5f)), CRGBA(255, 255, 255));

				float fScrollerLength = _ydown(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].bottomMargin) - _y(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].topMargin) - _height(25.0f);
				float fScrollerPos = _height(2.5f) + (fScrollerLength * m_dwSelectedMenuItem / (m_nNumMenuEntries - 1));

				m_apMouseTextures[1].Draw(CRect(_x(19.5f - 5.0f), _y(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].topMargin) + fScrollerPos, _x(29.5f + 5.0f), _y(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].topMargin + 20.0f) + fScrollerPos), CRGBA(MENU_INACTIVE_R, MENU_INACTIVE_G, MENU_INACTIVE_B));
			}
		}
		else {
			if (FrontEndMenuManager._aScreens[m_bCurrentMenuPage].topMargin != 0 || FrontEndMenuManager._aScreens[m_bCurrentMenuPage].bottomMargin != 0)
			{
				CSprite2d::DrawRect(CRect(_xmiddle(-310.0f), _y(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].topMargin), _xmiddle(310.0f), _ydown(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].bottomMargin)), CRGBA(MENU_BOX_BLUE_R, MENU_BOX_BLUE_G, MENU_BOX_BLUE_B, MENU_BOX_BLUE_A));

				m_apMouseTextures[0].Draw(CRect(_xmiddle(297.5f), _y(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].topMargin + 2.5f), _xmiddle(307.5f), _ydown(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].bottomMargin + 2.5f)), CRGBA(255, 255, 255));

				float fScrollerLength = _ydown(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].bottomMargin) - _y(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].topMargin) - _height(25.0f);
				float fScrollerPos = _height(2.5f) + (fScrollerLength * m_dwSelectedMenuItem / (m_nNumMenuEntries - 1));

				m_apMouseTextures[1].Draw(CRect(_xmiddle(297.5f - 5.0f), _y(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].topMargin) + fScrollerPos, _xmiddle(307.5f + 5.0f), _y(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].topMargin + 20.0f) + fScrollerPos), CRGBA(MENU_INACTIVE_R, MENU_INACTIVE_G, MENU_INACTIVE_B));
			}
		}

		switch (m_bCurrentMenuPage)
		{
		case MENU_PAGE_STATS:
			PrintStats();
			break;
		case MENU_PAGE_BRIEF:
			PrintBrief();
			break;
		case MENU_PAGE_AUDIO_SETUP:
			if (bDrawMenu) {
				if (!FrontEndMenuManager.m_bEnableSkyMenu)
					DrawRadioStationIcons();
			}
			break;
		case MENU_PAGE_CONTROLLER_SETUP:
			PrintControllerSetupScreen();
			break;
		case MENU_PAGE_GAME_UPDATES:
			PrintUpdaterScreen();
			break;
		case MENU_PAGE_DLC:
			PrintDLCScreen();
			break;
			/*case MENU_PAGE_ACTIVATE_SERIAL:
				PrintActivationScreen();
				break;*/
		case MENU_PAGE_VCS_MAP:
			PrintVCSMap();
			break;
		default:
			ResetVCSMap();
			break;
		}

		// Header
		if (bDrawMenu)
		{
			if (!FrontEndMenuManager.m_bEnableSkyMenu) {
				if (FrontEndMenuManager._aScreens[m_bCurrentMenuPage].name[0])
				{
					//if ( m_bCurrentMenuPage != 5 || !m_bMapLoaded )
					//{
					CFont::SetOrientation(1);
					CFont::SetFontStyle(FONT_RageItalic);
					CFont::SetScale(_width(1.3f), _height(2.1f));
					CFont::SetDropShadowPosition(2);
					CFont::SetColor(CRGBA(MENU_PINK_R, MENU_PINK_G, MENU_PINK_B, SafeZoneHideStuffAlpha));
					CFont::SetDropColor(CRGBA(0, 0, 0, SafeZoneHideStuffAlpha));
					CFont::PrintString(_xleft(40.0f), _y(11.0f), TheText.Get(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].name));
					//}
				}
			}
		}

		// Action 1 text
		if (FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[0].action == 1)
		{
			CFont::SetWrapx(RsGlobal.MaximumWidth - 40);
			CFont::SetFontStyle(FONT_Eurostile);
			CFont::SetEdge(1);
			CFont::SetColor(CRGBA(MENU_INACTIVE_R, MENU_INACTIVE_G, MENU_INACTIVE_B));
			CFont::SetDropColor(CRGBA(0, 0, 0));

			const char*		pText;
			switch (m_bCurrentMenuPage)
			{
			case 6:
			{
				pText = TheText.Get(m_bMainMenuSwitch ? "FESZ_QQ" : FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[0].entry);
				break;
			}
			case 35:
			{
				pText = TheText.Get(m_bMainMenuSwitch ? "FEQ_SRW" : FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[0].entry);
				break;
			}
			case 17:
			{
				switch (SlotValidation[m_bSelectedSaveGame + 1])
				{
				case 0:
					pText = TheText.Get("FESZ_QO");
					break;
				case 2:
					pText = TheText.Get("FESZ_QC");
					break;
				default:
					pText = TheText.Get(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[0].entry);
					break;
				}
				break;
			}
			case 11:
			{
				pText = TheText.Get(m_bMainMenuSwitch ? "FES_LCG" : FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[0].entry);
				break;
			}
			default:
			{
				pText = TheText.Get(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[0].entry);
				break;
			}
			}

			if (FrontEndMenuManager.m_bEnableSkyMenu) {
				CFont::SetCentreSize(RsGlobal.MaximumWidth - 80);
				CFont::SetScaleLang(_width(0.55f), _height(1.3f));
				CFont::SetOrientation(ALIGN_Center);
				CFont::PrintString(_xmiddle(0.0f), _y(72.0f), pText);
			}
			else {
				CFont::SetScaleLang(_width(0.49f), _height(1.2f));
				CFont::SetOrientation(1);
				CFont::PrintString(_xleft(60.0f), _y(97.0f), pText);
			}

			CFont::SetWrapx(RsGlobal.MaximumWidth - 10);
		}

		// Special menu draws - post header and action 1 text
		switch (m_bCurrentMenuPage)
		{
		case 38:
			DrawControllerScreenExtraText(-8);
			break;
		case MENU_PAGE_ACTIVATE_SERIAL:
			PrintActivationScreen();
			break;
		}

		// Menu drawing loop
		for (int i = 0; i < NUM_ENTRIES_PER_MENU; i++)
		{
			CFont::SetFontStyle(FONT_Eurostile);
			CFont::SetEdge(1);
			if (FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].specialDescFlag >= ACTION_SAVE_1 && FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].specialDescFlag <= ACTION_SAVE_12)
			{
				// Save slot
				CFont::SetScale(_width(0.42f), _height(0.95f));
			}
			else
			{
				// Regular texts
				CFont::SetScale(_width(m_vecMenuColumnSize.x), _height(m_vecMenuColumnSize.y));
			}
			unsigned char		nAlign;

			switch (FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].align)
			{
			case 1:
				nAlign = ALIGN_Left;
				break;
			case 2:
				nAlign = ALIGN_Right;
				break;
			default:
				nAlign = ALIGN_Center;
				break;
			}

			CRGBA m_dwSelectedItemColor;

			if (i == m_dwSelectedMenuItem && m_bMapLoaded) {
				if (FrontEndMenuManager.m_bEnableSkyMenu) {
					if (!FrontEndMenuManager.m_bBottomMenu)
						m_dwSelectedItemColor = { MENU_ACTIVE_R, MENU_ACTIVE_G, MENU_ACTIVE_B, 255 };
					else
						m_dwSelectedItemColor = { MENU_INACTIVE_R, MENU_INACTIVE_G, MENU_INACTIVE_B, static_cast<BYTE>(SafeZoneHideStuffAlpha) };
				}
				else
					m_dwSelectedItemColor = { MENU_ACTIVE_R, MENU_ACTIVE_G, MENU_ACTIVE_B, 255 };
			}
			else
			{
				if (CUpdateManager::NewUpdatesPending() && ((m_bCurrentMenuPage == 33 && i == 6) || (m_bCurrentMenuPage == 34 && i == 1) || (m_bCurrentMenuPage == 41 && i == 5) || (m_bCurrentMenuPage == 44 && i == 0)))
					m_dwSelectedItemColor = { MENU_UPDATES_R, MENU_UPDATES_G, MENU_UPDATES_B, static_cast<BYTE>(SafeZoneHideStuffAlpha) };
				else
					m_dwSelectedItemColor = { MENU_INACTIVE_R, MENU_INACTIVE_G, MENU_INACTIVE_B, static_cast<BYTE>(SafeZoneHideStuffAlpha) };
			}

			CFont::SetColor(CRGBA(m_dwSelectedItemColor));

			CFont::SetDropColor(CRGBA(0, 0, 0));
			CFont::SetOrientation(nAlign);

			// Auto positioning
			if ((FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].posX == 0 && FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].posY == 0)
				|| FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].specialDescFlag == ACTION_MISSIONPACK)
			{
				if (i && (i != 1 || FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[0].action != 1))
				{
					FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].posX = FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i - 1].posX;
					if (FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].screenVertAlign == 1)
						FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].posY = FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i - 1].posY - (FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].specialDescFlag == ACTION_MISSIONPACK ? 20 : GetAutoSpacingHeight());
					else
						FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].posY = FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i - 1].posY + (FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].specialDescFlag == ACTION_MISSIONPACK ? 20 : GetAutoSpacingHeight());
				}
				else
				{
					FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].posX = 320;
					FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].posY = 130;
				}
			}

			if (FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].action != 1 && FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].entry[0])
			{
				const char*	pTextToShow = nullptr;
				const char*	pTextToShow_RightColumn = nullptr;
				char		cReservedSpace[64];

				if (FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].specialDescFlag >= ACTION_SAVE_1 && FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].specialDescFlag <= ACTION_SAVE_12)
				{
					FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].posX = 80;
					FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].screenHorAlign = -1;
					CFont::SetOrientation(ALIGN_Left);

					switch (SlotValidation[i])
					{
					case 0:
						pTextToShow = CGenericGameStorage::GetNameOfSavedGame(i - 1);
						pTextToShow_RightColumn = GetSavedGameDateAndTime(i - 1);
						break;
					case 2:
						pTextToShow = TheText.Get("FESZ_CS");
						break;
					}

					if (!pTextToShow || !pTextToShow[0])
					{
						sprintf(gString, "FEM_SL%X", i);
						CFont::SetOrientation(ALIGN_Center);
						FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].posX = 0;
						FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].screenHorAlign = 0;
						pTextToShow = TheText.Get(gString);
					}
				}
				else if (FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].specialDescFlag == ACTION_MISSIONPACK)
				{
					// TODO: MPACK name

				}
				/*else if ( FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].specialDescFlag == ACTION_JOYMOUSE )
				{
					pTextToShow = TheText.Get(m_nController ? "FEJ_TIT" : "FEC_MOU");
				}*/
				else if (FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].specialDescFlag == ACTION_UPDATER)
				{
					pTextToShow = CUpdateManager::GetGXTEntryForButton();
				}
				else if (FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].specialDescFlag == ACTION_DLC)
				{
					pTextToShow = ProcessDLCSlot(i);
				}
				else
				{
					if (FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].specialDescFlag == ACTION_SERIAL)
					{
						if (!m_bSerialFull)
							CFont::SetColor(CRGBA(MENU_LOCKED_R, MENU_LOCKED_G, MENU_LOCKED_B));
					}
					pTextToShow = TheText.Get(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].entry);
				}

				// Regular actions - right column
				switch (FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].action)
				{
				case 31:
					pTextToShow_RightColumn = TheText.Get(m_bRadioAutoSelect ? "FEM_ON" : "FEM_OFF");
					break;
				case 32:
					pTextToShow_RightColumn = AudioEngine.GetRadioStationName(m_nRadioStation);
					break;
				case 33:
					pTextToShow_RightColumn = TheText.Get(m_bMapLegend ? "FEM_ON" : "FEM_OFF");
					break;
				case 25:
					pTextToShow_RightColumn = TheText.Get(m_bShowSubtitles ? "FEM_ON" : "FEM_OFF");
					break;
				case 26:
					sprintf(cReservedSpace, "FEM_AS%d", m_bAspectRatioMode);
					pTextToShow_RightColumn = TheText.Get(cReservedSpace);
					break;
				case MENUACTION_CTRL_TYPE:
					switch (CPad::SavedMode)
					{
					case 0:
						pTextToShow_RightColumn = TheText.Get("FEC_CF1");
						break;
					case 1:
						pTextToShow_RightColumn = TheText.Get("FEC_CF2");
						break;
					case 3:
						pTextToShow_RightColumn = TheText.Get("FEC_CF3");
						break;
					}
					break;
				case MENUACTION_PAD_FRONTEND_PAGE:
					pTextToShow_RightColumn = TheText.Get(m_bPadPageShown == 1 ? "FEC_INC" : "FEC_ONF");
					break;
				case 34:
					switch (m_dwRadarMode)
					{
					case 0:
						pTextToShow_RightColumn = TheText.Get("FED_RDM");
						break;
					case 1:
						pTextToShow_RightColumn = TheText.Get("FED_RDB");
						break;
					case 2:
						pTextToShow_RightColumn = TheText.Get("FEM_OFF");
						break;
					}
					break;
				case 35:
					switch (m_bHudOn)
					{
					case 0:
						pTextToShow_RightColumn = TheText.Get("FEM_OFF");
						break;
					case 1:
						pTextToShow_RightColumn = TheText.Get("FEM_24H");
						break;
					case 2:
						pTextToShow_RightColumn = TheText.Get("FEM_12H");
						break;
					}
					break;
				case 65:
					pTextToShow_RightColumn = TheText.Get(m_bSavePhotos ? "FEM_ON" : "FEM_OFF");
					break;
				case 63:
					switch (m_nRadioMode)
					{
					case 0:
						pTextToShow_RightColumn = TheText.Get("FEA_PR1");
						break;
					case 1:
						pTextToShow_RightColumn = TheText.Get("FEA_PR2");
						break;
					case 2:
						pTextToShow_RightColumn = TheText.Get("FEA_PR3");
						break;
					};
					break;
				case MENUACTION_SHADOWS_QUALITY:
					switch (CShadows::GetShadowQuality())
					{
					case SHADOW_QUALITY_OFF:
						pTextToShow_RightColumn = TheText.Get("FEM_OFF");
						break;
					case SHADOW_QUALITY_LOWEST:
						pTextToShow_RightColumn = TheText.Get("FED_FXC");
						break;
					case SHADOW_QUALITY_LOW:
						pTextToShow_RightColumn = TheText.Get("FED_FXL");
						break;
					case SHADOW_QUALITY_MEDIUM:
						pTextToShow_RightColumn = TheText.Get("FED_FXM");
						break;
					case SHADOW_QUALITY_HIGH:
						pTextToShow_RightColumn = TheText.Get("FED_FXH");
						break;
					case SHADOW_QUALITY_HIGHEST:
						pTextToShow_RightColumn = TheText.Get("FED_FXV");
						break;
					};
					break;
				case MENUACTION_PIPE_QUALITY:
					switch (CarPipe::envMapSize) {
					case REFLECTION_QUALITY_LOW:
						pTextToShow_RightColumn = TheText.Get("FED_FXL");
						break;
					case REFLECTION_QUALITY_MEDIUM:
						pTextToShow_RightColumn = TheText.Get("FED_FXM");
						break;
					case REFLECTION_QUALITY_HIGH:
						pTextToShow_RightColumn = TheText.Get("FED_FXH");
						break;
					case REFLECTION_QUALITY_HIGHEST:
						pTextToShow_RightColumn = TheText.Get("FED_FXV");
						break;
					};
					break;
				case 24:
					if (m_bFrameLimiterMode == 0)
						pTextToShow_RightColumn = TheText.Get("FEM_OFF");
					else
					{
						CMessages::InsertNumberInString(TheText.Get("FEM_FPS"), RsGlobalFrameLimits[m_bFrameLimiterMode], -1, -1, -1, -1, -1, cReservedSpace);
						pTextToShow_RightColumn = cReservedSpace;
					}
					break;
					// case 43 - mipmapping has been nuked
				case 64:
					pTextToShow_RightColumn = TheText.Get(m_bTracksAutoScan ? "FEM_ON" : "FEM_OFF");
					break;
				case 46:
					pTextToShow_RightColumn = TheText.Get(!MousePointerStateHelper.m_bHorizontalInvert ? "FEM_ON" : "FEM_OFF");
					break;
				case 47:
					pTextToShow_RightColumn = TheText.Get(invertPadX1 ? "FEM_ON" : "FEM_OFF");
					break;
				case 48:
					pTextToShow_RightColumn = TheText.Get(invertPadY1 ? "FEM_ON" : "FEM_OFF");
					break;
				case 49:
					pTextToShow_RightColumn = TheText.Get(invertPadX2 ? "FEM_ON" : "FEM_OFF");
					break;
				case 50:
					pTextToShow_RightColumn = TheText.Get(invertPadY2 ? "FEM_ON" : "FEM_OFF");
					break;
				case 51:
					pTextToShow_RightColumn = TheText.Get(swapPadAxis1 ? "FEM_ON" : "FEM_OFF");
					break;
				case 52:
					pTextToShow_RightColumn = TheText.Get(swapPadAxis2 ? "FEM_ON" : "FEM_OFF");
					break;
				case 59:
					pTextToShow_RightColumn = TheText.Get(CVehicle::m_bEnableMouseSteering ? "FEM_ON" : "FEM_OFF");
					break;
				case 60:
					pTextToShow_RightColumn = TheText.Get(CVehicle::m_bEnableMouseFlying ? "FEM_ON" : "FEM_OFF");
					break;
				case 56:
				{
					char* pszResolution = GetDisplayModesList()[m_dwResolution];

					if (!pszResolution)
					{
						pszResolution = "";
					}

					strncpy(cReservedSpace, pszResolution, sizeof(cReservedSpace));
					pTextToShow_RightColumn = cReservedSpace;
					break;
				}
				case 37:
					if (FrontEndMenuManager.m_bEnableSkyMenu) {
						switch (FrontEndMenuManager.m_nLanguage) {
						case 0:
							pTextToShow_RightColumn = TheText.Get("FEL_ENG");
							break;
						case 1:
							pTextToShow_RightColumn = TheText.Get("FEL_ITA");
							break;
						case 2:
							pTextToShow_RightColumn = TheText.Get("FEL_BRA");
							break;
						case 3:
							pTextToShow_RightColumn = TheText.Get("FEL_POL");
							break;
						};
					}
					break;			
				case 44:
					if (m_dwAntiAliasingLevel <= 1)
						pTextToShow_RightColumn = TheText.Get("FEM_OFF");
					else
					{
						switch (m_dwAntiAliasingLevel)
						{
						case 2:
							pTextToShow_RightColumn = TheText.Get("FED_AA1");
							break;
						case 3:
							pTextToShow_RightColumn = TheText.Get("FED_AA2");
							break;
						case 4:
							pTextToShow_RightColumn = TheText.Get("FED_AA3");
							break;
						}
					}
					break;
				case MENUACTION_AUTOINSTALL_UPDATES:
					pTextToShow_RightColumn = TheText.Get(CUpdateManager::AutoInstallEnabled() ? "FEM_ON" : "FEM_OFF");
					break;
				case MENUACTION_CHECKING_PERIOD:
					sprintf(cReservedSpace, "FEU_CH%d", CUpdateManager::GetCheckingPeriod() + 1);
					pTextToShow_RightColumn = TheText.Get(cReservedSpace);
					break;
				case MENUACTION_TEXTURE_FILTERMODE:
					sprintf(cReservedSpace, "FED_TX%d", Fx_c::GetTextureFilteringQuality());
					pTextToShow_RightColumn = TheText.Get(cReservedSpace);
					break;
				case MENUACTION_EFFECTS_QUALITY:
					switch (Fx_c::GetEffectsQuality())
					{
					case FXQUALITY_LOW:
						pTextToShow_RightColumn = TheText.Get("FED_FXL");
						break;
					case FXQUALITY_MEDIUM:
						pTextToShow_RightColumn = TheText.Get("FED_FXM");
						break;
					case FXQUALITY_HIGH:
						pTextToShow_RightColumn = TheText.Get("FED_FXH");
						break;
					}
					break;
				case MENUACTION_SHADOWS_DISTANCE:
					if (CShadows::GetShadowQuality() == SHADOW_QUALITY_OFF)
						CFont::SetColor(CRGBA(MENU_LOCKED_R, MENU_LOCKED_G, MENU_LOCKED_B));
					break;
				case MENUACTION_TRAILS:
					pTextToShow_RightColumn = TheText.Get(CPostEffects::TrailsEnabled() ? "FEM_ON" : "FEM_OFF");
					break;
				case MENUACTION_NEOVEH:
					pTextToShow_RightColumn = CarPipe::PipeSwitch ? "VCS" : "NEO";
					break;
				case MENUACTION_VSYNC:
					pTextToShow_RightColumn = TheText.Get(m_bVSync ? "FEM_ON" : "FEM_OFF");
					break;
				case MENUACTION_VIBRATION:
					pTextToShow_RightColumn = TheText.Get(m_bVibrationEnabled ? "FEM_ON" : "FEM_OFF");
					break;
				case MENUACTION_INVERTLOOK:
					pTextToShow_RightColumn = TheText.Get(CPad::bInvertLook4Pad == false ? "FEM_ON" : "FEM_OFF");
					break;
				case MENUACTION_SOUTHPAW:
					pTextToShow_RightColumn = TheText.Get(CPad::bSouthpaw ? "FEM_ON" : "FEM_OFF");
					break;
				case MENUACTION_BUTTONSTYLE:
					switch (CFont::bX360Buttons)
					{
					case 1:
						pTextToShow_RightColumn = "XBOX";
						break;
					case 0:
						pTextToShow_RightColumn = "PLAYSTATION";
						break;
					}
					break;
				case MENUACTION_SKY_MENU:
					switch (FrontEndMenuManager.m_bEnableSkyMenu)
					{
					case 1:
						pTextToShow_RightColumn = "CONSOLE";
						break;
					case 0:
						pTextToShow_RightColumn = "PC";
						break;
					}
					break;
				}

				float	fPosX, fPosY;

				if (FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].screenHorAlign == -1)
					fPosX = _xleft(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].posX);
				else if (FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].screenHorAlign == 1)
					fPosX = _x(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].posX);
				else
					fPosX = _xmiddle(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].posX);


				if (FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].screenVertAlign == -1)
					fPosY = _y(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].posY);
				else if (FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].screenVertAlign == 1)
					fPosY = _ydown(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].posY);
				else
					fPosY = _ymiddle(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].posY);

				fPosY += m_fScrollerOffset;

				if (fPosY < _y(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].topMargin) || fPosY > _ydown(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].bottomMargin + 15.0f))
					continue;

				if (pTextToShow)
				{
					if (FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].specialDescFlag >= ACTION_SAVE_1 && FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].specialDescFlag <= ACTION_SAVE_12
						&& (SlotValidation[i] != 1 && SlotValidation[i] != 2))
					{
						char		cSlotNumberText[4];
						sprintf(cSlotNumberText, "%d", FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].specialDescFlag);

						CFont::PrintString(fPosX + _xleft(25.0f), fPosY, pTextToShow);

						CFont::SetOrientation(ALIGN_Right);
						CFont::PrintString(fPosX, fPosY, cSlotNumberText);
					}
					else
						CFont::PrintString(fPosX, fPosY, pTextToShow);
				}

				if (pTextToShow_RightColumn)
				{
					CFont::SetFontStyle(FONT_Eurostile);
					CFont::SetEdge(1);

					bool	bSaveMenu = m_bCurrentMenuPage == 9 || m_bCurrentMenuPage == 10 || m_bCurrentMenuPage == 16;

					if (bSaveMenu)
						CFont::SetOrientation(ALIGN_Right);
					else
						CFont::SetOrientation(ALIGN_Left);

					if (FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].specialDescFlag >= ACTION_SAVE_1 && FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].specialDescFlag <= ACTION_SAVE_12)
						CFont::SetScale(_width(0.35f), _height(0.95f));
					else if ((m_bCurrentMenuPage == 3 && i == 4) || (m_bCurrentMenuPage == 27 && i == 0))
						CFont::SetScale(_width(0.56f), _height(1.0f));
					else
						CFont::SetScale(_width(m_vecMenuColumnSize.x), _height(m_vecMenuColumnSize.y));

					CFont::PrintString(bSaveMenu ? _x(40.0f) : _xmiddle(m_vecMenuColumnPosn[1].x), fPosY, pTextToShow_RightColumn);
				}

				// Sliders
				switch (FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].action)
				{
				case 27:
				{
					// Brightness
					float	nMouseInput = DisplaySlider(_xmiddle(m_vecMenuColumnPosn[1].x), fPosY + _height(MENU_SLIDER_HEIGHT / 2 - 1.25f), _height(MENU_SLIDER_HEIGHT), _width(100.0f), m_dwBrightness * (1.0f / 384.0f), _width(MENU_SLIDER_WIDTH), false);

					if (i == m_dwSelectedMenuItem)
					{
						if (CheckHover(_xleft(95.0f), nMouseInput - _width(3.0f), fPosY + _height(MENU_SLIDER_HEIGHT / 2 - 1.25f), fPosY + _height(3 * MENU_SLIDER_HEIGHT / 2 + 1.25f)))
							m_nHoverOption = 7;
						else if (CheckHover(nMouseInput + _width(3.0f), _x(95.0f), fPosY + _height(MENU_SLIDER_HEIGHT / 2 - 1.25f), fPosY + _height(3 * MENU_SLIDER_HEIGHT / 2 + 1.25f)))
							m_nHoverOption = 6;
						else
							m_nHoverOption = 16;
					}
					break;
				}
				case 28:
				{
					// Radio volume
					float	nMouseInput = DisplaySlider(_xmiddle(m_vecMenuColumnPosn[1].x), fPosY + _height(MENU_SLIDER_HEIGHT / 2 - 1.25f), _height(MENU_SLIDER_HEIGHT), _width(100.0f), m_nRadioVolume * (1.0f / 64.0f), _width(MENU_SLIDER_WIDTH), false);

					if (i == m_dwSelectedMenuItem)
					{
						if (CheckHover(_xleft(95.0f), nMouseInput - _width(3.0f), fPosY + _height(MENU_SLIDER_HEIGHT / 2 - 1.25f), fPosY + _height(3 * MENU_SLIDER_HEIGHT / 2 + 1.25f)))
							m_nHoverOption = 11;
						else if (CheckHover(nMouseInput + _width(3.0f), _x(95.0f), fPosY + _height(MENU_SLIDER_HEIGHT / 2 - 1.25f), fPosY + _height(3 * MENU_SLIDER_HEIGHT / 2 + 1.25f)))
							m_nHoverOption = 10;
						else
							m_nHoverOption = 16;
					}
					break;
				}
				case 29:
				{
					// SFX volume
					float	nMouseInput = DisplaySlider(_xmiddle(m_vecMenuColumnPosn[1].x), fPosY + _height(MENU_SLIDER_HEIGHT / 2 - 1.25f), _height(MENU_SLIDER_HEIGHT), _width(100.0f), m_nSfxVolume * (1.0f / 64.0f), _width(MENU_SLIDER_WIDTH), false);

					if (i == m_dwSelectedMenuItem)
					{
						if (CheckHover(_xleft(95.0f), nMouseInput - _width(3.0f), fPosY + _height(MENU_SLIDER_HEIGHT / 2 - 1.25f), fPosY + _height(3 * MENU_SLIDER_HEIGHT / 2 + 1.25f)))
							m_nHoverOption = 13;
						else if (CheckHover(nMouseInput + _width(3.0f), _x(95.0f), fPosY + _height(MENU_SLIDER_HEIGHT / 2 - 1.25f), fPosY + _height(3 * MENU_SLIDER_HEIGHT / 2 + 1.25f)))
							m_nHoverOption = 12;
						else
							m_nHoverOption = 16;
					}
					break;
				}
				case 61:
				{
					// Draw Distance
					float	nMouseInput = DisplaySlider(_xmiddle(m_vecMenuColumnPosn[1].x), fPosY + _height(MENU_SLIDER_HEIGHT / 2 - 1.25f), _height(MENU_SLIDER_HEIGHT), _width(100.0f), (m_fDrawDistance - 0.925f) * (1.0f / 0.875f), _width(MENU_SLIDER_WIDTH), false);

					if (i == m_dwSelectedMenuItem)
					{
						if (CheckHover(_xleft(95.0f), nMouseInput - _width(3.0f), fPosY + _height(MENU_SLIDER_HEIGHT / 2 - 1.25f), fPosY + _height(3 * MENU_SLIDER_HEIGHT / 2 + 1.25f)))
							m_nHoverOption = 9;
						else if (CheckHover(nMouseInput + _width(3.0f), _x(95.0f), fPosY + _height(MENU_SLIDER_HEIGHT / 2 - 1.25f), fPosY + _height(3 * MENU_SLIDER_HEIGHT / 2 + 1.25f)))
							m_nHoverOption = 8;
						else
							m_nHoverOption = 16;
					}
					break;
				}
				case 62:
				{
					// Mouse Sensitivity
					float	nMouseInput = DisplaySlider(_xmiddle(m_vecMenuColumnPosn[1].x), fPosY + _height(MENU_SLIDER_HEIGHT / 2 - 1.25f), _height(MENU_SLIDER_HEIGHT), _width(100.0f), (CCamera::m_fMouseAccelHorzntl - 0.0003125f)* (1.0f / 0.0049f), _width(MENU_SLIDER_WIDTH), false);

					if (i == m_dwSelectedMenuItem)
					{
						if (CheckHover(_xleft(95.0f), nMouseInput - _width(3.0f), fPosY + _height(MENU_SLIDER_HEIGHT / 2 - 1.25f), fPosY + _height(3 * MENU_SLIDER_HEIGHT / 2 + 1.25f)))
							m_nHoverOption = 15;
						else if (CheckHover(nMouseInput + _width(3.0f), _x(95.0f), fPosY + _height(MENU_SLIDER_HEIGHT / 2 - 1.25f), fPosY + _height(3 * MENU_SLIDER_HEIGHT / 2 + 1.25f)))
							m_nHoverOption = 14;
						else
							m_nHoverOption = 16;
					}
					break;
				}
				case MENUACTION_SHADOWS_DISTANCE:
				{
					// Shadows Distance
					bool	bLockedSlider = CShadows::GetShadowQuality() == SHADOW_QUALITY_OFF;
					float	nMouseInput = DisplaySlider(_xmiddle(m_vecMenuColumnPosn[1].x), fPosY + _height(MENU_SLIDER_HEIGHT / 2 - 1.25f), _height(MENU_SLIDER_HEIGHT), _width(100.0f), CShadows::GetShadowDistance(), _width(MENU_SLIDER_WIDTH), bLockedSlider);

					if (!bLockedSlider)
					{
						/*if ( i == m_dwSelectedMenuItem )
						{
							if ( CheckHover(_xleft(95.0f), nMouseInput - _width(3.0f), _ymiddle(26.0f + MENU_SLIDER_HEIGHT/2 - 1.25f), _ymiddle(26.0f + 3*MENU_SLIDER_HEIGHT/2 + 1.25f)) )
								m_nHoverOption = 15;
							else if ( CheckHover(nMouseInput + _width(3.0f), _x(95.0f), _ymiddle(26.0f + MENU_SLIDER_HEIGHT/2 - 1.25f), _ymiddle(26.0f + 3*MENU_SLIDER_HEIGHT/2 + 1.25f)) )
								m_nHoverOption = 14;
							else
								m_nHoverOption = 16;
						}*/
					}
					break;
				}
				case MENUACTION_SAFEZONE:
					// Safe Zone
					float	nMouseInput = DisplaySlider(_xmiddle(m_vecMenuColumnPosn[1].x), fPosY + _height(MENU_SLIDER_HEIGHT / 2 - 1.25f), _height(MENU_SLIDER_HEIGHT), _width(100.0f), m_fSafeZone * (1.0f / 16.0f), _width(MENU_SLIDER_WIDTH), false);

					/*if (i == m_dwSelectedMenuItem)
					{
						if (CheckHover(_xleft(95.0f), nMouseInput - _width(3.0f), fPosY + _height(MENU_SLIDER_HEIGHT / 2 - 1.25f), fPosY + _height(3 * MENU_SLIDER_HEIGHT / 2 + 1.25f)))
							m_nHoverOption = 13;
						else if (CheckHover(nMouseInput + _width(3.0f), _x(95.0f), fPosY + _height(MENU_SLIDER_HEIGHT / 2 - 1.25f), fPosY + _height(3 * MENU_SLIDER_HEIGHT / 2 + 1.25f)))
							m_nHoverOption = 12;
						else
							m_nHoverOption = 16;
					}*/
					break;
				}
			}
		}

		// SafeZone rect
		if (m_bCurrentMenuPage == 4 && FrontEndMenuManager.m_dwSelectedMenuItem == 1) {

			if (SafeZoneHideStuffAlpha < 100)
				DrawSafeZoneScreen();
		}
		else
			m_bControlSafeZone = false;

		if (!m_bControlSafeZone) {
			if (SafeZoneHideStuffAlpha < 255.0f)
				SafeZoneHideStuffAlpha += CTimer::ms_fTimeScale * 0.01 * 1000.0;

			if (SafeZoneHideStuffAlpha >= 255.0f)
				SafeZoneHideStuffAlpha = 255.0f;
		}
		else {
			if (SafeZoneHideStuffAlpha > 50.0f)
				SafeZoneHideStuffAlpha += CTimer::ms_fTimeScale * 0.01 * -1000.0;

			if (SafeZoneHideStuffAlpha <= 50.0f)
				SafeZoneHideStuffAlpha = 50.0f;
		}

		// Helpers
		if (!strcmp(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[m_dwSelectedMenuItem].entry, "FED_RES"))
		{
			if (m_dwResolution == m_dwAppliedResolution)
			{
				if (m_nHelperTextIndex == 1)
					ResetHelperText();
			}
			else
				SetHelperText(1);
		}
		else
		{
			if (m_dwResolution != m_dwAppliedResolution)
			{
				m_dwResolution = m_dwAppliedResolution;
				SetHelperText(3);
			}
		}

		if (!strncmp(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[m_dwSelectedMenuItem].entry, "FED_AAS", 8))
		{
			if (m_dwAntiAliasingLevel == m_dwAppliedAntiAliasingLevel)
			{
				if (m_nHelperTextIndex == 1)
					ResetHelperText();
			}
			else
				SetHelperText(1);
		}
		else
		{
			if (m_dwAntiAliasingLevel != m_dwAppliedAntiAliasingLevel)
			{
				m_dwAntiAliasingLevel = m_dwAppliedAntiAliasingLevel;
				SetHelperText(3);
			}
		}

		if (!strncmp(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[m_dwSelectedMenuItem].entry, "FED_VSN", 8))
		{
			if (m_bVSync == m_bAppliedVSync)
			{
				if (m_nHelperTextIndex == 1)
					ResetHelperText();
			}
			else
				SetHelperText(1);
		}
		else
		{
			if (m_bVSync != m_bAppliedVSync)
			{
				m_bVSync = m_bAppliedVSync;
				SetHelperText(3);
			}
		}

		switch (m_bCurrentMenuPage)
		{
		case 0:
		case 3:
		case 4:
		case 5:
		case 26:
		case 27:
		case 36:
		case 39:
		case 40:
			DisplayHelperText(nullptr);
			break;
		}

	}
	SwitchToSkyMenuInstance();
}

static Reversed DisplayHelperText_kill(0x57E245, 0x57E4CF);
void CMenuManager::DisplayHelperText(const char* pText)
{
	CFont::SetProportional(1);
	CFont::SetDropColor(CRGBA(0, 0, 0, SafeZoneHideStuffAlpha));
	CFont::SetScale(_width(0.4f), _height(0.6f));
	CFont::SetFontStyle(FONT_Eurostile);
	CFont::SetEdge(0);
	CFont::SetOrientation(ALIGN_Right);

	if (!FrontEndMenuManager.m_bEnableSkyMenu) {
		if (pText != nullptr) {
			float offset = 0.0f;

			if (FrontEndMenuManager.m_bCurrentMenuPage == 5) {
				offset = 20.0f;
				CSprite2d::DrawRect(CRect(_x(340.0f), _ydown(26.0f), _x(38.0f), _ydown(88.0f)), CRGBA(MENU_BOX_BLUE_R, MENU_BOX_BLUE_G, MENU_BOX_BLUE_B, MENU_BOX_BLUE_A));
			}
			CFont::SetColor(CRGBA(0xFF, 0xFF, 0xFF, SafeZoneHideStuffAlpha));
			CFont::PrintStringFromBottom(_x(30.0f + offset), _ydown(10.0f + offset), TheText.Get(pText));
		}
		else
		{
			int		nTextAlpha = SafeZoneHideStuffAlpha;

			if (m_nHelperTextIndex != 0 && m_nHelperTextIndex != 1)
			{
				// Fade
				if (CTimer::m_snTimeInMillisecondsPauseMode - m_nLastTimeHelperUpdated > 10)
				{
					// TODO: Timestep?
					m_nLastTimeHelperUpdated = CTimer::m_snTimeInMillisecondsPauseMode;
					m_nHelperAlpha -= 2;

					if (m_nHelperAlpha < 1)
						ResetHelperText();

					nTextAlpha = Min(m_nHelperAlpha, 0xFF);
				}
			}

			CFont::SetColor(CRGBA(0xFF, 0xFF, 0xFF, nTextAlpha));

			const char*		pTextToDisplay;

			if (pXboxPad[0]->HasPadInHands())
			{
				// Pad helps
				switch (m_nHelperTextIndex)
				{
				case 5:
					pTextToDisplay = "FEA_SCS";
					break;
				case 4:
					pTextToDisplay = "FEA_SCF";
					break;
				case 2:
					pTextToDisplay = "FET_HRD";
					break;
				case 3:
					pTextToDisplay = "FET_RSO";
					break;
				case 6:
					pTextToDisplay = "FET_STS";
					break;
				case 1:
					pTextToDisplay = "FET_AP2";
					break;
				default:
					switch (FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[m_dwSelectedMenuItem].action)
					{
					case 6:
						pTextToDisplay = "FEH_SN2";
						break;
					case 5:
					case 7:
					case 8:
					case 9:
						pTextToDisplay = "FEH_JM2";
						break;
					case 2:
						pTextToDisplay = "FEH_BP2";
						break;
					default:
						pTextToDisplay = m_bCurrentMenuPage != 0 ? "FET_MI2" : "FEH_SS2";
						break;
					}
					break;
				}
			}
			else
			{
				// Keyboard & mouse helps
				switch (m_nHelperTextIndex)
				{
				case 5:
					pTextToDisplay = "FEA_SCS";
					break;
				case 4:
					pTextToDisplay = "FEA_SCF";
					break;
				case 2:
					pTextToDisplay = "FET_HRD";
					break;
				case 3:
					pTextToDisplay = "FET_RSO";
					break;
				case 6:
					pTextToDisplay = "FET_STS";
					break;
				case 1:
					pTextToDisplay = "FET_APP";
					break;
				default:
					switch (FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[m_dwSelectedMenuItem].action)
					{
					case 6:
						pTextToDisplay = "FEH_SNC";
						break;
					case 5:
					case 7:
					case 8:
					case 9:
						pTextToDisplay = "FEH_JMP";
						break;
					case 2:
						pTextToDisplay = "FEH_BPO";
						break;
					default:
						pTextToDisplay = m_bCurrentMenuPage != 0 ? "FET_MIG" : "FEH_SSA";
						break;
					};
					break;
				}
			}
			if (FrontEndMenuManager.m_bCurrentMenuPage != 5)
				CFont::PrintStringFromBottom(_x(30.0f), _ydown(m_bCurrentMenuPage != 5 ? 10.0f : 2.0f), TheText.Get(pTextToDisplay));
		}
	}
}

static Reversed ProcessMenuOptions_kill(0x576FE0, 0x5773CF);
void CMenuManager::ProcessMenuOptions(signed char nArrowsInput, bool* bReturn, bool bEnterInput)
{
	switch ( FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[m_dwSelectedMenuItem].action )
	{
	case 2:
		*bReturn = true;
		return;
	case 25:
		m_bShowSubtitles = m_bShowSubtitles == false;
		SaveSettings();
		return;
	case 33:
		m_bMapLegend = m_bMapLegend == false;
		SaveSettings();
		return;
	case 26:
		if ( nArrowsInput >= 0 )
		{
			if ( ++m_bAspectRatioMode > 5 )
				m_bAspectRatioMode = 0;
		}
		else
		{
			if ( m_bAspectRatioMode )
				--m_bAspectRatioMode;
			else
				m_bAspectRatioMode = 5;
		}
		WidescreenSupport::Recalculate(RsGlobal.MaximumWidth, RsGlobal.MaximumHeight, true);
		SaveSettings();
		return;
	case 35:
		if ( nArrowsInput >= 0 )
		{
			if ( ++m_bHudOn > 2 )
				m_bHudOn = 0;
		}
		else
		{
			if ( m_bHudOn != 0 )
				--m_bHudOn;
			else
				m_bHudOn = 2;
		}
		SaveSettings();
		return;
	case MENUACTION_CTRL_TYPE:
		if ( nArrowsInput >= 0 )
		{
			if (CPad::SavedMode == 0 )
				CPad::SavedMode = 1;
            else if (CPad::SavedMode == 1)
                CPad::SavedMode = 3;
            else if (CPad::SavedMode == 2)
                CPad::SavedMode = 3;
            else if (CPad::SavedMode == 3)
                CPad::SavedMode = 0;
        }
		else
		{
            if (CPad::SavedMode == 0)
                CPad::SavedMode = 3;
            else if (CPad::SavedMode == 1)
                CPad::SavedMode = 0;
            else if (CPad::SavedMode == 2)
                CPad::SavedMode = 1;
            else if (CPad::SavedMode == 3)
                CPad::SavedMode = 1;


		}
		if ( pXboxPad[0]->HasPadInHands() )
			CPad::GetPad(0)->Mode = CPad::SavedMode;

		OnModeChangePatches();
		SaveSettings();
		return;
	case MENUACTION_PAD_FRONTEND_PAGE:
		m_bPadPageShown = m_bPadPageShown == 0 ? 1 : 0;
		return;
	case 31:
		m_bRadioAutoSelect = m_bRadioAutoSelect == false;
		AudioEngine.SetRadioAutoRetuneOnOff(m_bRadioAutoSelect);
		SaveSettings();
		return;
	// case 30: - removed option
	case 21:
		if ( bEnterInput )
		{
			*bReturn = true;
			return;
		}

		if ( nArrowsInput >= 0 )
		{
			if ( m_bStatScrollUp )
			{
				if ( m_fStatsScrollSpeed == 0.0f )
					m_fStatsScrollSpeed = 150.0f;
				else if ( m_fStatsScrollSpeed == 150.0f )
					m_fStatsScrollSpeed = 30.0f;
			}
			else
			{
				if ( m_fStatsScrollSpeed == 0.0f )
					m_fStatsScrollSpeed = 150.0f;
				else
					m_fStatsScrollSpeed = 0.0f;

				m_bStatScrollUp = true;
			}
		}
		else
		{
			if ( m_bStatScrollUp )
			{
				if ( m_fStatsScrollSpeed != 0.0f )
					m_fStatsScrollSpeed = 0.0f;
				m_bStatScrollUp = false;
			}
			else
			{
				if ( m_fStatsScrollSpeed == 0.0f )
					m_fStatsScrollSpeed = 150.0f;
				else if ( m_fStatsScrollSpeed == 150.0f )
					m_fStatsScrollSpeed = 30.0f;
			}
		}

		return;
	case 32:
		ScrollRadioStations(nArrowsInput);
		return;
	//case 36: - PS2 only
	case 34:
		if ( nArrowsInput >= 0 )
		{
			if ( ++m_dwRadarMode > 2 )
				m_dwRadarMode = 0;
		}
		else
		{
			if ( m_dwRadarMode )
				--m_dwRadarMode;
			else
				m_dwRadarMode = 2;
		}
		SaveSettings();
		return;
	case 3:
	case 4:
	case 5:
	case 7:	// - Used to switch menus depending on current input device used
		SwitchToNewScreen(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[m_dwSelectedMenuItem].targetMenu);
		return;
	case 11:
		// TODO: Do
		return;
	case 10:
		ProcessMissionPackNewGame();
		return;
	case 13:
		if ( FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[m_dwSelectedMenuItem].specialDescFlag >= ACTION_SAVE_1 && FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[m_dwSelectedMenuItem].specialDescFlag <= ACTION_SAVE_12 )
		{
			m_bSelectedSaveGame = m_dwSelectedMenuItem-1;
			if ( m_bCurrentMenuPage == 9 )
			{
				if ( SlotValidation[m_dwSelectedMenuItem] != 1 && SlotValidation[m_dwSelectedMenuItem] != 2 )
					SwitchToNewScreen(11);
			}
			else if ( m_bCurrentMenuPage == 10 )
			{
				if ( SlotValidation[m_dwSelectedMenuItem] != 1  )
					SwitchToNewScreen(12);
			}
		}
		return;
	case 18:
		if ( FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[m_dwSelectedMenuItem].specialDescFlag >= ACTION_SAVE_1 && FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[m_dwSelectedMenuItem].specialDescFlag <= ACTION_SAVE_12 )
		{
			m_bSelectedSaveGame = m_dwSelectedMenuItem-1;
			SwitchToNewScreen(17);
		}
		return;
	case 12:
		CGame::bMissionPackGame = m_bSelectedMissionPack;
		DoSettingsBeforeStartingAGame();
		return;
	case 14:
		CGame::bMissionPackGame = 0;
		DoSettingsBeforeStartingAGame();
		m_bBackIntoGame = true;
		return;
	case 15:
		m_bBackIntoGame = true;
		return;

	case 46:
		MousePointerStateHelper.m_bHorizontalInvert = MousePointerStateHelper.m_bHorizontalInvert == false;
		SaveSettings();
		return;
	case 47:
		invertPadX1 = invertPadX1 == false;
		SaveSettings();
		return;
	case 48:
		invertPadY1 = invertPadY1 == false;
		SaveSettings();
		return;
	case 49:
		invertPadX2 = invertPadX2 == false;
		SaveSettings();
		return;
	case 50:
		invertPadY2 = invertPadY2 == false;
		SaveSettings();
		return;
	case 51:
		swapPadAxis1 = swapPadAxis1 == false;
		SaveSettings();
		return;
	case 52:
		swapPadAxis2 = swapPadAxis2 == false;
		SaveSettings();
		return;
	case 59:
		CVehicle::m_bEnableMouseSteering = CVehicle::m_bEnableMouseSteering == false;
		SaveSettings();
		return;
	case 60:
		CVehicle::m_bEnableMouseFlying = CVehicle::m_bEnableMouseFlying == false;
		SaveSettings();
		return;
	case 24:
		if ( nArrowsInput >= 0 )
		{
			if ( ++m_bFrameLimiterMode > 4 )
				m_bFrameLimiterMode = 0;
		}
		else
		{
			if ( m_bFrameLimiterMode )
				--m_bFrameLimiterMode;
			else
				m_bFrameLimiterMode = 4;
		}
		RsGlobal.frameLimit = RsGlobalFrameLimits[m_bFrameLimiterMode];
		SaveSettings();
		return;
	//case 43: - Removed option
	case 64:
		m_bTracksAutoScan = m_bTracksAutoScan == false;
		SaveSettings();
		return;
	case 65:
		m_bSavePhotos = m_bSavePhotos == false;
		SaveSettings();
		return;
	//case 58:	- Removed option
	case 56:
		if ( bEnterInput )
		{
			if ( m_dwResolution != m_dwAppliedResolution )
			{
				m_dwAppliedResolution = m_dwResolution;
				// CSettings__SetCurrentVideoMode
				((void(*)(int))0x745C70)(m_dwAppliedResolution);
				CentreMousePointer();

				m_fScrollerOffset = 0.0f;
				m_bDrawMouse = true;
				SaveSettings();
				CPostEffects::DoScreenModeDependentInitializations();
			}
		}
		else
		{
			char**				pVideoModes = GetDisplayModesList();
			int					nNumVideoModes = GetNumDisplayModes();

			if ( nArrowsInput > 0 )
			{
				int		nCurrentVidMode = m_dwResolution + 1;

				if ( nCurrentVidMode >= nNumVideoModes )
					nCurrentVidMode = 0;

				while ( !pVideoModes[nCurrentVidMode] )
				{
					++nCurrentVidMode;

					if ( nCurrentVidMode >= nNumVideoModes )
						nCurrentVidMode = 0;
				}

				m_dwResolution = nCurrentVidMode;
			}
			else
			{
				int		nCurrentVidMode = m_dwResolution - 1;

				if ( nCurrentVidMode < 0 )
					nCurrentVidMode = nNumVideoModes - 1;

				while ( !pVideoModes[nCurrentVidMode] )
				{
					--nCurrentVidMode;

					if ( nCurrentVidMode < 0 )
						nCurrentVidMode = nNumVideoModes - 1;
				}

				m_dwResolution = nCurrentVidMode;
			}
		}
		return;
	case 44:
		if ( bEnterInput )
		{
			if ( m_dwAntiAliasingLevel != m_dwAppliedAntiAliasingLevel )
			{
				m_dwAppliedAntiAliasingLevel = m_dwAntiAliasingLevel;

				RwD3D9ChangeMultiSamplingLevels(m_dwAppliedAntiAliasingLevel);
				// CSettings__SetCurrentVideoMode
				((void(*)(int))0x745C70)(m_dwAppliedResolution);
				SaveSettings();
			}
		}
		else
		{
			int		nNumMultiSamplingModes = RwD3D9EngineGetMaxMultiSamplingLevels();

			if ( nNumMultiSamplingModes > 4 )
				nNumMultiSamplingModes = 4;

			if ( nArrowsInput > 0 )
			{
				int		nCurrentMode = m_dwAntiAliasingLevel + 1;

				if ( nCurrentMode > nNumMultiSamplingModes )
					nCurrentMode = 1;

				m_dwAntiAliasingLevel = nCurrentMode;
			}
			else
			{
				int		nCurrentMode = m_dwAntiAliasingLevel - 1;

				if ( nCurrentMode <= 0 )
					nCurrentMode = nNumMultiSamplingModes;

				m_dwAntiAliasingLevel = nCurrentMode;
			}
		}
		return;
	case MENUACTION_SHADOWS_QUALITY:
		if ( nArrowsInput >= 0 )
		{
			switch ( CShadows::GetShadowQuality() )
			{
			case SHADOW_QUALITY_OFF:
				CShadows::SetShadowQuality(SHADOW_QUALITY_LOWEST);
				break;
			case SHADOW_QUALITY_LOWEST:
				CShadows::SetShadowQuality(SHADOW_QUALITY_LOW);
				break;
			case SHADOW_QUALITY_LOW:
				CShadows::SetShadowQuality(SHADOW_QUALITY_MEDIUM);
				break;
			case SHADOW_QUALITY_MEDIUM:
				CShadows::SetShadowQuality(SHADOW_QUALITY_HIGH);
				break;
			case SHADOW_QUALITY_HIGH:
				CShadows::SetShadowQuality(SHADOW_QUALITY_HIGHEST);
				break;
			case SHADOW_QUALITY_HIGHEST:
				CShadows::SetShadowQuality(SHADOW_QUALITY_OFF);
				break;
			}
		}
		else
		{
			switch ( CShadows::GetShadowQuality() )
			{
			case SHADOW_QUALITY_OFF:
				CShadows::SetShadowQuality(SHADOW_QUALITY_HIGHEST);
				break;
			case SHADOW_QUALITY_LOWEST:
				CShadows::SetShadowQuality(SHADOW_QUALITY_OFF);
				break;
			case SHADOW_QUALITY_LOW:
				CShadows::SetShadowQuality(SHADOW_QUALITY_LOWEST);
				break;
			case SHADOW_QUALITY_MEDIUM:
				CShadows::SetShadowQuality(SHADOW_QUALITY_LOW);
				break;
			case SHADOW_QUALITY_HIGH:
				CShadows::SetShadowQuality(SHADOW_QUALITY_MEDIUM);
				break;
			case SHADOW_QUALITY_HIGHEST:
				CShadows::SetShadowQuality(SHADOW_QUALITY_HIGH);
				break;
			}
		}
		CShadows::InitialiseChangedSettings();
		SaveSettings();
		return;
	case MENUACTION_PIPE_QUALITY:
		if (nArrowsInput >= 0) {
			switch (CarPipe::envMapSize) {
			case REFLECTION_QUALITY_LOW:
				CarPipe::envMapSize = REFLECTION_QUALITY_MEDIUM;
				break;
			case REFLECTION_QUALITY_MEDIUM:
				CarPipe::envMapSize = REFLECTION_QUALITY_HIGH;
				break;
			case REFLECTION_QUALITY_HIGH:
				CarPipe::envMapSize = REFLECTION_QUALITY_HIGHEST;
				break;
			case REFLECTION_QUALITY_HIGHEST:
				CarPipe::envMapSize = REFLECTION_QUALITY_LOW;
				break;
			};
		}
		else {
			switch (CarPipe::envMapSize) {
			case REFLECTION_QUALITY_LOW:
				CarPipe::envMapSize = REFLECTION_QUALITY_HIGHEST;
				break;
			case REFLECTION_QUALITY_MEDIUM:
				CarPipe::envMapSize = REFLECTION_QUALITY_LOW;
				break;
			case REFLECTION_QUALITY_HIGH:
				CarPipe::envMapSize = REFLECTION_QUALITY_MEDIUM;
				break;
			case REFLECTION_QUALITY_HIGHEST:
				CarPipe::envMapSize = REFLECTION_QUALITY_HIGH;
				break;
			};
		}
		CarPipe::CarPipeInit();
		SaveSettings();
		return;
	case 63:
		if ( nArrowsInput >= 0 )
		{
			if ( ++m_nRadioMode > 2 )
				m_nRadioMode = 0;
		}
		else
		{
			if ( m_nRadioMode )
				--m_nRadioMode;
			else
				m_nRadioMode = 2;
		}
		SaveSettings();
		return;
	case 54:
		m_bBackIntoGame = true;
		return;
	case 57:
		SetDefaultPreferences(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[2].targetMenu);

		if ( FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[2].targetMenu == 36 )
			CControllerConfigManager::ReinitControls();
		else if ( FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[2].targetMenu == 27 )
		{
			CPostEffects::Close();
			CPostEffects::Initialise();
		}

		SaveSettings();
		SwitchToNewScreen(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[2].targetMenu);
		SetHelperText(2);
		return;
	case 8:
		m_bInVehicleControlsScreen = false;
		SwitchToNewScreen(38);
		field_90 = 0;
		return;
	case 9:
		m_bInVehicleControlsScreen = true;
		SwitchToNewScreen(38);
		field_90 = 0;
		return;
	case 45:
		field_1B14 = 1;
		field_1B09 = 1;
		field_1B08 = 1;
		field_1B0C = m_dwSelectedMenuItem;
		field_F0 =  &field_36[2];
		return;
	case 53:
		SwitchToNewScreen(43);
		return;
	case 55:
		SwitchToNewScreen(-2);
		return;
	case 6:
		field_1AEB = 1;
		return;
	case 37:
		if (FrontEndMenuManager.m_bEnableSkyMenu) {
			if (nArrowsInput >= 0) {
				switch (FrontEndMenuManager.m_nLanguage) {
				case 0:
					FrontEndMenuManager.m_nLanguage = 1;
					FrontEndMenuManager.m_bReinitLanguageSettings = true;
					FrontEndMenuManager.InitialiseChangedLanguageSettings(false);
					break;
				case 1:
					FrontEndMenuManager.m_nLanguage = 2;
					FrontEndMenuManager.m_bReinitLanguageSettings = true;
					FrontEndMenuManager.InitialiseChangedLanguageSettings(false);
					break;
				case 2:
					FrontEndMenuManager.m_nLanguage = 3;
					FrontEndMenuManager.m_bReinitLanguageSettings = true;
					FrontEndMenuManager.InitialiseChangedLanguageSettings(false);
					break;
				case 3:
					FrontEndMenuManager.m_nLanguage = 0;
					FrontEndMenuManager.m_bReinitLanguageSettings = true;
					FrontEndMenuManager.InitialiseChangedLanguageSettings(false);
					break;
				};
			}
			else {
				switch (FrontEndMenuManager.m_nLanguage) {
				case 0:
					FrontEndMenuManager.m_nLanguage = 3;
					FrontEndMenuManager.m_bReinitLanguageSettings = true;
					FrontEndMenuManager.InitialiseChangedLanguageSettings(false);
					break;
				case 1:
					FrontEndMenuManager.m_nLanguage = 0;
					FrontEndMenuManager.m_bReinitLanguageSettings = true;
					FrontEndMenuManager.InitialiseChangedLanguageSettings(false);
					break;
				case 2:
					FrontEndMenuManager.m_nLanguage = 1;
					FrontEndMenuManager.m_bReinitLanguageSettings = true;
					FrontEndMenuManager.InitialiseChangedLanguageSettings(false);
					break;
				case 3:
					FrontEndMenuManager.m_nLanguage = 2;
					FrontEndMenuManager.m_bReinitLanguageSettings = true;
					FrontEndMenuManager.InitialiseChangedLanguageSettings(false);
					break;
				};
			}
			FrontEndMenuManager.SaveSettings();
			return;
		}
		else {
			if (m_nLanguage != 0)
			{
				m_nLanguage = 0;
				m_bReinitLanguageSettings = true;
				InitialiseChangedLanguageSettings(false);
				SaveSettings();
			}
			return;
		}
	case 38:
		if ( m_nLanguage != 1 )
		{
			m_nLanguage = 1;
			m_bReinitLanguageSettings = true;
			InitialiseChangedLanguageSettings(false);
			SaveSettings();
		}
		return;
	case 39:
		if ( m_nLanguage != 2 )
		{
			m_nLanguage = 2;
			m_bReinitLanguageSettings = true;
			InitialiseChangedLanguageSettings(false);
			SaveSettings();
		}
		return;
	case 40:
		if ( m_nLanguage != 3 )
		{
			m_nLanguage = 3;
			m_bReinitLanguageSettings = true;
			InitialiseChangedLanguageSettings(false);
			SaveSettings();
		}
		return;
	case 41:
		if ( m_nLanguage != 4 )
		{
			m_nLanguage = 4;
			m_bReinitLanguageSettings = true;
			InitialiseChangedLanguageSettings(false);
			SaveSettings();
		}
		return;
	case MENUACTION_TOGGLE_LANGUAGE_5:
		if ( m_nLanguage != 5 )
		{
			m_nLanguage = 5;
			m_bReinitLanguageSettings = true;
			InitialiseChangedLanguageSettings(false);
			SaveSettings();
		}
		return;

	case MENUACTION_UPDATER_BUTTON:
		CUpdateManager::HandleButtonClick();
		return;
	case MENUACTION_TOGGLE_DLC:
		CDLCManager::HandleButtonClick(m_dwSelectedMenuItem);
		return;
	case MENUACTION_ACTIVATE_SERIAL:
		if ( m_bSerialFull )
			CDLCManager::ActivateSerial(m_strSerialCode);
		return;
	case MENUACTION_AUTOINSTALL_UPDATES:
		CUpdateManager::SetAutoInstall(CUpdateManager::AutoInstallEnabled() == false);
		CUpdateManager::SaveSettings();
		return;
	case MENUACTION_CHECKING_PERIOD:
		if ( nArrowsInput >= 0 )
		{
			unsigned char		nPeriod = CUpdateManager::GetCheckingPeriod();
			if ( ++nPeriod > 3 )
				nPeriod = 0;

			CUpdateManager::SetCheckingPeriod(nPeriod);
		}
		else
		{
			unsigned char		nPeriod = CUpdateManager::GetCheckingPeriod();
			if ( nPeriod )
				--nPeriod;
			else
				nPeriod = 3;

			CUpdateManager::SetCheckingPeriod(nPeriod);
		}
		CUpdateManager::SaveSettings();
		return;
	case MENUACTION_TEXTURE_FILTERMODE:
		if ( nArrowsInput >= 0 )
		{
			unsigned char	nFilteringQuality = Fx_c::GetTextureFilteringQuality();
			if ( ++nFilteringQuality > Fx_c::GetMaxTextureFilteringQuality() )
				nFilteringQuality = 0;

			Fx_c::SetTextureFilteringQuality(nFilteringQuality);
		}
		else
		{
			unsigned char	nFilteringQuality = Fx_c::GetTextureFilteringQuality();
			if ( nFilteringQuality )
				--nFilteringQuality;
			else
				nFilteringQuality = Fx_c::GetMaxTextureFilteringQuality();

			Fx_c::SetTextureFilteringQuality(nFilteringQuality);
		}
		SaveSettings();
		return;
	case MENUACTION_EFFECTS_QUALITY:
		if ( nArrowsInput >= 0 )
		{
			switch ( Fx_c::GetEffectsQuality() )
			{
			case FXQUALITY_LOW:
				Fx_c::SetEffectsQuality(FXQUALITY_MEDIUM);
				break;
			case FXQUALITY_MEDIUM:
				Fx_c::SetEffectsQuality(FXQUALITY_HIGH);
				break;
			case FXQUALITY_HIGH:
				Fx_c::SetEffectsQuality(FXQUALITY_LOW);
				break;
			}
		}
		else
		{
			switch ( Fx_c::GetEffectsQuality() )
			{
			case FXQUALITY_LOW:
				Fx_c::SetEffectsQuality(FXQUALITY_HIGH);
				break;
			case FXQUALITY_MEDIUM:
				Fx_c::SetEffectsQuality(FXQUALITY_LOW);
				break;
			case FXQUALITY_HIGH:
				Fx_c::SetEffectsQuality(FXQUALITY_MEDIUM);
				break;
			}
		}
		SaveSettings();
		return;
	case MENUACTION_TRAILS:
		if ( CPostEffects::TrailsEnabled() )
		{
			CPostEffects::SetTrailsState(false);
		//	CPostEffects::Radiosity_Close();	// radiosity always initialised now
		}
		else
		{
			CPostEffects::SetTrailsState(true);
		//	CPostEffects::Radiosity_Init();		// radiosity always initialised now
		}
		SaveSettings();
		return;
	case MENUACTION_NEOVEH:
		CarPipe::PipeSwitch = !CarPipe::PipeSwitch;
		SaveSettings();
		return;
	case MENUACTION_VSYNC:
		if ( bEnterInput )
		{
			if ( m_bVSync != m_bAppliedVSync )
			{
				m_bAppliedVSync = m_bVSync;

				ToggleVSync(m_bAppliedVSync);
				SaveSettings();
			}
		}
		else
		{
			m_bVSync = m_bVSync == false;
		}

		return;
	case MENUACTION_VIBRATION:
		if ( !m_bVibrationEnabled )
		{
			m_bVibrationEnabled = true;
			nTimeToStopPadShake = CTimer::m_snTimeInMillisecondsPauseMode + 500;
			CPad::GetPad(0)->StartShake(350, 150, 0);
		}
		else
			m_bVibrationEnabled = false;

		SaveSettings();
		return;
	case MENUACTION_INVERTLOOK:
		CPad::bInvertLook4Pad = CPad::bInvertLook4Pad == false;
		SaveSettings();
		return;
	case MENUACTION_SOUTHPAW:
		CPad::bSouthpaw = CPad::bSouthpaw == false;
		SaveSettings();
		return;
	case MENUACTION_BUTTONSTYLE:
		CFont::bX360Buttons = CFont::bX360Buttons == false;
		CFont::ShutdownButtons();
		CFont::InitialiseButtons();
		SaveSettings();
		return;
	case MENUACTION_SKY_MENU:
		FrontEndMenuManager.m_bSwitchToSkyMenu = FrontEndMenuManager.m_bSwitchToSkyMenu == false;
		return;
	}
}

static Reversed CheckSliderMovement_kill(0x573440, 0x57367F);
void CMenuManager::CheckSliderMovement(signed char nDirection)
{
	switch ( FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[m_dwSelectedMenuItem].action )
	{
	case 27:
		{
			float	fNewBrightness = m_dwBrightness + (nDirection*(24.19f));

			if ( fNewBrightness > 384.0f)
				fNewBrightness = 384.0f;
			else if ( fNewBrightness < 0.0f )
				fNewBrightness = 0.0f;
			m_dwBrightness = fNewBrightness;

			//((void(__thiscall*)(int,float,bool))0x747200)(0xC92134, fNewBrightness * (1.0f/512.0f), false);
			SaveSettings();
			return;
		}
	case 28:
		{
			signed char		nNewVolume = m_nRadioVolume + (nDirection*4);
			
			if ( nNewVolume > 64 )
				nNewVolume = 64;
			else if ( nNewVolume < 0 )
				nNewVolume = 0;

			m_nRadioVolume = nNewVolume;
			AudioEngine.SetMusicMasterVolume(nNewVolume);
			SaveSettings();
			return;
		}
	case 29:
		{
			signed char		nNewVolume = m_nSfxVolume + (nDirection*4);
			
			if ( nNewVolume > 64 )
				nNewVolume = 64;
			else if ( nNewVolume < 0 )
				nNewVolume = 0;

			m_nSfxVolume = nNewVolume;
			AudioEngine.SetEffectsMasterVolume(nNewVolume);
			SaveSettings();
			return;
		}
	case 61:
		{
			float	fNewDrawDist = m_fDrawDistance + (nDirection*((1.8f-0.925f)/16.0f));

			if ( fNewDrawDist > 1.8f )
				fNewDrawDist = 1.8f;
			else if ( fNewDrawDist < 0.925f )
				fNewDrawDist = 0.925f;
			ms_lodDistScale = m_fDrawDistance = fNewDrawDist;

			CModelInfo::RecalcDrawDistances();
			SaveSettings();
			return;
		}
	case 62:
		{
			float	fNewSensitivity = CCamera::m_fMouseAccelHorzntl + (nDirection*((0.005f-0.0003125f)/16.0f));

			if ( fNewSensitivity > 0.005f )
				fNewSensitivity = 0.005f;
			else if ( fNewSensitivity < 0.0003125f )
				fNewSensitivity = 0.0003125f;
			CCamera::m_fMouseAccelHorzntl = fNewSensitivity;

			SaveSettings();
			return;
		}
	case MENUACTION_SHADOWS_DISTANCE:
		{
			if ( CShadows::GetShadowQuality() != SHADOW_QUALITY_OFF )
			{
				float	fNewDist = CShadows::GetShadowDistance() + (nDirection*(1.0f/16.0f));

				if ( fNewDist > 1.0f )
					fNewDist = 1.0f;
				else if ( fNewDist < 0.0f )
					fNewDist = 0.0f;
				CShadows::SetShadowDistance(fNewDist);
				CShadows::InitialiseChangedSettings();

				SaveSettings();
			}
			return;
		}
	case MENUACTION_SAFEZONE:
	{
		float	fNewSafeZone = m_fSafeZone + (nDirection);

		if (fNewSafeZone > 16.0f)
			fNewSafeZone = 16.0f;
		else if (fNewSafeZone < 0.0f)
			fNewSafeZone = 0.0f;

		m_fSafeZone = fNewSafeZone;

		m_bControlSafeZone = true;
		SaveSettings();
		return;
	}
	}
}

int CMenuManager::GetAutoSpacingHeight()
{
	switch ( m_bCurrentMenuPage )
	{
	case MENU_PAGE_CONTROLLER_SETUP:
		return 25;
	}
	return 30;
}

void CMenuManager::CentreMousePointer()
{
	POINT		PointerPos;
	PointerPos.x = RsGlobal.MaximumWidth / 2;
	PointerPos.y = RsGlobal.MaximumHeight / 2;

	ClientToScreen(RsGlobal.ps->window, &PointerPos);
	SetCursorPos(PointerPos.x, PointerPos.y);

	RsGlobal.ps->lastMousePos_X = RsGlobal.MaximumWidth / 2;
	RsGlobal.ps->lastMousePos_Y = RsGlobal.MaximumHeight / 2;
}

void CMenuManager::DrawBackEnd()
{
	if (!FrontEndMenuManager.m_bSwitchToSkyMenu) {
		// AA Menu Fix
		CSprite2d::DrawRect(CRect(-2.0f, RsGlobal.MaximumHeight, RsGlobal.MaximumWidth + 2.0f, -2.0f), CRGBA(0, 0, 0, 255));

		// Calculate proper dimensions
		// Displayed image is 16:9
		CVector2D				vecSplashScale = WidescreenSupport::GetFullscreenImageDimensions(16.0f / 9.0f, ScreenAspectRatio, false);

		CSprite2d::DrawRect(CRect(-1.0f, RsGlobal.MaximumHeight + 1.0f, RsGlobal.MaximumWidth + 1.0f, RsGlobal.MaximumHeight * 0.95f - 1.0f), CRGBA(7, 7, 7, 255));
		m_apBackgroundTextures[0].Draw(CRect(-1.0f - (vecSplashScale.x - RsGlobal.MaximumWidth), RsGlobal.MaximumHeight * 0.95f, RsGlobal.MaximumWidth + 1.0f, -1.0f - (vecSplashScale.y - RsGlobal.MaximumHeight)), CRGBA(m_nBackgroundColor));

		if (m_bCurrentMenuPage == 44)
			m_apBackgroundTextures[1].Draw(CRect(_x(245.0f), _y(80.0f), _x(25.0f), _y(25.0f)), CRGBA(255, 255, 255, 255));

		CFont::SetBackground(0, 0);
		CFont::SetProportional(false);
		CFont::SetFontStyle(FONT_PagerFont);
		CFont::SetRightJustifyWrap(0.0);
		CFont::SetEdge(1);
		CFont::SetDropColor(CRGBA(0, 0, 0, SafeZoneHideStuffAlpha));
		CFont::SetColor(CRGBA(BaseColors[11].r, BaseColors[11].g, BaseColors[11].b, SafeZoneHideStuffAlpha));

		if (m_bEnableSkyMenu) {
			CFont::SetOrientation(ALIGN_Right);
			CFont::SetScale(_width(0.22f), _height(0.4f));

			CFont::PrintString(_x(17.0f), _y(12.5f), MOD_VERSION " BUILD " BUILDNUMBER_STR);
			CFont::PrintString(_x(17.0f), _y(20.5f), "END 2018 BUILD");
		}
		else {
			if (FrontEndMenuManager.m_bCurrentMenuPage != 5) {
				CFont::SetOrientation(ALIGN_Left);
				CFont::SetScale(_width(0.25f), _height(0.4f));

				CFont::PrintString(_xleft(2.5f), _ydown(13.0f), MOD_VERSION " BUILD " BUILDNUMBER_STR);

#if defined DEVBUILD
#ifdef MAKE_ZZCOOL_MOVIE_DEMO
				CFont::PrintString(_xleft(2.5f), _ydown(20.5f), "DEMONSTRATION BUILD");
#else
				//CFont::PrintString(_xleft(2.5f), _ydown(20.5f), "DEV BUILD");
				CFont::PrintString(_xleft(2.5f), _ydown(20.5f), "END 2018 BUILD");
#endif
#elif defined COMPILE_RC
				CFont::PrintString(_xleft(2.5f), _ydown(20.5f), "RELEASE CANDIDATE "RELEASE_CANDIDATE);
#else
				CFont::PrintString(_xleft(2.5f), _ydown(20.5f), VERSION_NAME_UPPERCASE);
#endif
			}
			// UPDATER removed for now
			//	if ( m_bCurrentMenuPage != 44 )
			//		CUpdateManager::Display();

#ifdef INCLUDE_PROMO_BANNER
			if (bCurrentScreen == 33 || bCurrentScreen == 34 || bCurrentScreen == 41)
			{
				int			nDaysTillDeadline = static_cast<int>(GetCurrentDate().GetSecondsLeft(CDate(21, 12, 2013, true))) / (60 * 60 * 24);
				if (nDaysTillDeadline >= 0)
				{
					textures[16].Draw(CRect(_x(135.0f), _y(122.5f), _x(15.0f), _y(2.5f)), CRGBA(255, 255, 255, 255));

					CFont::SetProportional(true);
					CFont::SetScale(_width(0.8f), _height(1.2f));
					CFont::SetFontStyle(FONT_RageItalic);
					//CFont::SetDropColor(CRGBA(0, 0, 0, 255));
					//CFont::SetEdge(1);
					CFont::SetOrientation(ALIGN_Center);

					CFont::SetColor(CRGBA(MODDB_RED_R, MODDB_RED_G, MODDB_RED_B, 255));
					CFont::PrintString(_x(75.0f), _y(115.0f), TheText.Get("FEP_MOD"));

					CFont::SetProportional(false);
					CFont::SetFontStyle(FONT_PagerFont);
					CFont::SetScale(_width(0.375f), _height(0.725f));
					if (nDaysTillDeadline >= 3)
						CFont::SetColor(CRGBA(255, 255, 255, 255));

					CMessages::InsertNumberInString(TheText.Get(nDaysTillDeadline == 1 ? "FEP_DY2" : "FEP_DYZ"), nDaysTillDeadline, -1, -1, -1, -1, -1, gString);
					CFont::PrintString(_x(75.0f), _y(140.0f), gString);
				}
			}
#endif
		}
	}

	CFont::SetProportional(true);
	CFont::SetEdge(0);
}

void CMenuManager::DrawRadioStationIcons()
{
	BYTE	bLoopCounter = 1;

	float	fPosition = (0.5f*WidescreenSupport::GetScreenWidthMultiplier()) + 300.0f;
	static	int LastTimeIconsWereUpdated = 0;

	CSprite2d::DrawRect(CRect(_xmiddle(-305.0f), _y(287.5f), _xmiddle(305.0f), _y(352.5f)), CRGBA(MENU_BOX_BLUE_R, MENU_BOX_BLUE_G, MENU_BOX_BLUE_B, MENU_BOX_BLUE_A));

#if defined COMPILE_BOUNCING_ICONS || defined COMPILE_SMOOTHBEATING_ICONS || defined COMPILE_BEATING_ICONS
	static signed char	bLastRadioStation = -1;
	static float		fRadioStationBouncingAngle = 0.0;

	//if ( (CTimer::m_snTimeInMillisecondsPauseMode - LastTimeIconsWereUpdated) > 32 )
	//{
		// TODO: Scratch the multiplier when CTimer is rewritten and fTimeStepPauseMode exists
#ifdef COMPILE_BEATING_ICONS
		fRadioStationBouncingAngle += (( fRadioStationBouncingAngle >= 0.0 && fRadioStationBouncingAngle < M_PI ) ? 0.3 : 0.075) * (CTimer::m_snTimeInMillisecondsPauseMode - LastTimeIconsWereUpdated) * (1.0/32.0);
#else
		fRadioStationBouncingAngle += 0.1;
#endif
		LastTimeIconsWereUpdated = CTimer::m_snTimeInMillisecondsPauseMode;
	//}
	if ( fRadioStationBouncingAngle > M_PI * 2.0 )
		fRadioStationBouncingAngle -= M_PI * 2.0;
	if ( bLastRadioStation != m_nRadioStation || m_dwSelectedMenuItem != 4 )
	{
		fRadioStationBouncingAngle = -0.75 * M_PI;

		bLastRadioStation = m_nRadioStation;
	}
#endif

	do
	{
#if defined COMPILE_BOUNCING_ICONS
		m_apRadioSprites[bLoopCounter].Draw(_x(fPosition), _y(290.0f - (20.0f * abs(static_cast<float>(sin(fRadioStationBouncingAngle))) * ( radioStation == bLoopCounter && fRadioStationBouncingAngle >= 0.0 ))), _width(60.0), _height(60.0), CRGBA(255, 255, 255, radioStation == bLoopCounter ? 255 : 30));
#elif defined COMPILE_SMOOTHBEATING_ICONS
		m_apRadioSprites[bLoopCounter].Draw(_x(fPosition - (10.0f * abs(static_cast<float>(sin(fRadioStationBouncingAngle))) * ( radioStation == bLoopCounter && fRadioStationBouncingAngle >= 0.0 ))), _y(290.0 - (10.0 * abs(static_cast<float>(sin(fRadioStationBouncingAngle))) * ( radioStation == bLoopCounter && fRadioStationBouncingAngle >= 0.0 ))), _width(60.0 + (20.0 * abs(static_cast<float>(sin(fRadioStationBouncingAngle))) * ( radioStation == bLoopCounter && fRadioStationBouncingAngle >= 0.0 ))), _height(60.0 + (20.0 * abs(static_cast<float>(sin(fRadioStationBouncingAngle))) * ( radioStation == bLoopCounter && fRadioStationBouncingAngle >= 0.0 ))), CRGBA(255, 255, 255, radioStation == bLoopCounter ? 255 : 30));
#elif defined COMPILE_BEATING_ICONS
		m_apRadioSprites[bLoopCounter].Draw(_x(fPosition + (10.0f * static_cast<float>(sin(fRadioStationBouncingAngle)) * ( m_nRadioStation == bLoopCounter && fRadioStationBouncingAngle >= 0.0 && fRadioStationBouncingAngle < M_PI ))), _ymiddle(66.0f - (10.0f * static_cast<float>(sin(fRadioStationBouncingAngle)) * ( m_nRadioStation == bLoopCounter && fRadioStationBouncingAngle >= 0.0 && fRadioStationBouncingAngle < M_PI ))), _width(60.0f + (20.0f * static_cast<float>(sin(fRadioStationBouncingAngle)) * ( m_nRadioStation == bLoopCounter && fRadioStationBouncingAngle >= 0.0 && fRadioStationBouncingAngle < M_PI ))), _height(60.0f + (20.0f * static_cast<float>(sin(fRadioStationBouncingAngle)) * ( m_nRadioStation == bLoopCounter && fRadioStationBouncingAngle >= 0.0 && fRadioStationBouncingAngle < M_PI ))), CRGBA(255, 255, 255, m_nRadioStation == bLoopCounter ? 255 : 30));
#else
		m_apRadioSprites[bLoopCounter].Draw(_x(fPosition), _y(290.0f), _width(60.0f), _height(60.0f), CRGBA(255, 255, 255, radioStation == bLoopCounter ? 255 : 30));
#endif
		++bLoopCounter;
		fPosition -= 60.0f;
	}
	while ( bLoopCounter < 11 );
}

float CMenuManager::DisplaySlider(float posX, float posY, float height, float distBetweenRects, float filledAmount, float width, bool bLocked)
{
	BYTE			loopCounter = NUM_SLIDERS * 2;
	BYTE			positionCounter = 0;
	BYTE			secondPositionCounter = 0;
	bool			bDrawHalfSlider = false;
	float			fullWidth = 0.0;

	do
	{
		float	itemPosX = distBetweenRects * positionCounter / NUM_SLIDERS + posX;
		float	mousePosX = distBetweenRects/2 * secondPositionCounter / (NUM_SLIDERS * 2) + posX;
		CRGBA	colour;

		if ( static_cast<float>(positionCounter) / NUM_SLIDERS + ( 1 / (NUM_SLIDERS * 2) ) >= filledAmount )
			colour = bLocked ? CRGBA(MENU_LOCKED_R, MENU_LOCKED_G, MENU_LOCKED_B) : CRGBA(MENU_INACTIVE_R, MENU_INACTIVE_G, MENU_INACTIVE_B);
		else
		{
			if ( static_cast<float>(secondPositionCounter) / (NUM_SLIDERS * 2) + ( 1 / (NUM_SLIDERS * 4)) >= filledAmount )
				bDrawHalfSlider = true;
			else
				colour = bLocked ? CRGBA(MENU_ACTIVE_LOCKED_R, MENU_ACTIVE_LOCKED_G, MENU_ACTIVE_LOCKED_B) : CRGBA(MENU_ACTIVE_R, MENU_ACTIVE_G, MENU_ACTIVE_B);

			fullWidth = mousePosX;
		}

		if ( loopCounter % 2 )
		{
			CSprite2d::DrawRect(CRect(itemPosX - _width(1.25f), height + posY + _height(1.25f), itemPosX + width + _width(1.25f), posY - _height(1.25f)), CRGBA(0, 0, 0, 255));
			if ( !bDrawHalfSlider )
				CSprite2d::DrawRect(CRect(itemPosX, height + posY, width + itemPosX, posY), colour);
			else
			{
				CSprite2d::DrawRect(CRect(itemPosX, height + posY, width + itemPosX, posY), CRGBA(MENU_INACTIVE_R, MENU_INACTIVE_G, MENU_INACTIVE_B, 255));
				CSprite2d::DrawRect(CRect(itemPosX + width * 0.25f, height + posY - height * 0.25f, width + itemPosX - width * 0.25f, posY + height * 0.25f), CRGBA(MENU_ACTIVE_R, MENU_ACTIVE_G, MENU_ACTIVE_B, 255));
				bDrawHalfSlider = false;
			}
			++positionCounter;
		}
		++secondPositionCounter;
		--loopCounter;
	}
	while ( loopCounter );

	return fullWidth;
}

float CMenuManager::GetRightColumnPos(MenuVar& sPosY)
{
	/*float	fTemp = RsGlobal.MaximumWidth * ( bCurrentScreen == 9 || bCurrentScreen == 10 || bCurrentScreen == 16 ? 40.0 : m_vecMenuColumnPosn[1].x) / 853.0;
	return fTemp;*/
	if ( m_bCurrentMenuPage != 44 )
	{
		sPosY.fOut = _ymiddle(sPosY.nIn);
		return RsGlobal.MaximumWidth * ( m_bCurrentMenuPage == 9 || m_bCurrentMenuPage == 10 || m_bCurrentMenuPage == 16 ? 40.0f : 0.5f * WidescreenSupport::GetScreenWidthMultiplier() - m_vecMenuColumnPosn[1].x) * WidescreenSupport::GetScreenWidthDivider();
	}

	sPosY.fOut = _y(sPosY.nIn);
	return RsGlobal.MaximumWidth * ( 0.5f * WidescreenSupport::GetScreenWidthMultiplier() - m_vecMenuColumnPosn[1].x) * WidescreenSupport::GetScreenWidthDivider();
}

void CMenuManager::DrawOutroSplash()
{
	static DWORD	outroPageFrameCounter = 0;
	static int		outroTimer = 0;
	static WORD		outroPageAlpha = 0;
	static bool		bOutroSplashLoaded = false;
	static CRect	rectSpriteDimensions;

	if (bNoOutro)
	{
		RsEventHandler(rsQUITAPP, nullptr);
		exit(0);
	}
	else
	{
		if (!bOutroSplashLoaded)
		{
			CLoadingScreen::LoadSplashes(true, 0);

			CVector2D				vecSplashScale = WidescreenSupport::GetFullscreenImageDimensions(512.0f / 400.0f, ScreenAspectRatio, true);

			rectSpriteDimensions.x1 = 0.5f * (RsGlobal.MaximumWidth - vecSplashScale.x) - SplashScreen.vec_mOutroPosn[0].x;
			rectSpriteDimensions.y1 = 0.5f * (RsGlobal.MaximumHeight + vecSplashScale.y) + SplashScreen.vec_mOutroPosn[0].y;
			rectSpriteDimensions.x2 = 0.5f * (RsGlobal.MaximumWidth + vecSplashScale.x) + SplashScreen.vec_mOutroPosn[0].x;
			rectSpriteDimensions.y2 = 0.5f * (RsGlobal.MaximumHeight - vecSplashScale.y) - SplashScreen.vec_mOutroPosn[0].y;

			bOutroSplashLoaded = true;
		}

		// TODO: Smooth when CTimer is rewritten
		if (CTimer::m_snTimeInMillisecondsPauseMode - outroTimer > 15)
		{
			if (outroPageAlpha != 255)
			{
				outroPageAlpha += 25;
				if (outroPageAlpha > 255)
					outroPageAlpha = 255;
			}
			else
				++outroPageFrameCounter;
			outroTimer = CTimer::m_snTimeInMillisecondsPauseMode;
		}

		CSprite2d::DrawRect(CRect(-5.0f, RsGlobal.MaximumHeight + 5.0f, RsGlobal.MaximumWidth + 5.0f, -5.0f), CRGBA(0, 0, 0, static_cast<BYTE>(outroPageAlpha)));
		SplashScreen.m_nSplashes[OUTRO].Draw(rectSpriteDimensions, CRGBA(255, 255, 255, static_cast<BYTE>(outroPageAlpha)));
		if (outroPageAlpha == 255 && outroPageFrameCounter == 90)
			RsEventHandler(rsQUITAPP, nullptr);
	}
}

void CMenuManager::PrintStats()
{
	//static float	fStatsScrollPos = -120.0;
	static int		nStatsTimer = 0;
	static DWORD	dwLastSelectedMenuEntry = 0;


	if ( CTimer::m_snTimeInMillisecondsPauseMode - nStatsTimer > 40 )
	{
		/*if ( CTimer::m_snTimeInMillisecondsPauseMode - nStatsTimer > 10000 )
			fStatsScrollPos = -120.0;*/

		if ( m_fStatsScrollSpeed > 0.0 )
		{
			if ( m_bStatScrollUp )
				m_fStatsScrollPos += (_height(100.0f) / m_fStatsScrollSpeed);
			else
				m_fStatsScrollPos -= (_height(100.0f) / m_fStatsScrollSpeed);
		}
		nStatsTimer = CTimer::m_snTimeInMillisecondsPauseMode;
	}

	int		nIndents;
	DWORD	dwLoopCounter = 0;
	DWORD	dwStatsToShow = CStats::ConstructStatLine(99999, nIndents);

	CFont::SetFontStyle(FONT_Eurostile);
	CFont::SetEdge(1);

	// Criminal Rating
	CFont::SetScale(_width(0.425f), _height(0.85f));
	CFont::SetDropColor(CRGBA(0, 0, 0, 255));
	CFont::SetOrientation(ALIGN_Left);
	CFont::SetColor(CRGBA(MENU_INACTIVE_R, MENU_INACTIVE_G, MENU_INACTIVE_B, 255));
	CFont::PrintString(_xleft(50.0f), _y(85.0f), TheText.Get("CRIMRA"));
	CFont::SetOrientation(ALIGN_Right);
	CFont::PrintString(_x(50.0f), _y(85.0f), CStats::FindCriminalRatingNumber());

	CFont::SetScale(_width(0.3f), _height(0.7f));

	while ( dwLoopCounter < dwStatsToShow )
	{
		float		fStartingPos = _height(39.0f) * dwLoopCounter + _height(110.0f) - m_fStatsScrollPos;

		while ( fStartingPos < _height(85.0f) )
			fStartingPos += _height((dwStatsToShow + 7) * 39.0f);

		if ( fStartingPos > _height(110.0f) && fStartingPos < _ydown(100.0f) )
		{
			float	fTextAlpha;
			if ( fStartingPos < _height(135.0f) )
				fTextAlpha = 10.2f * (fStartingPos - _height(110.0f));
			else
			{
				if ( fStartingPos > _ydown(125.0f) )
					fTextAlpha = 10.2f * (_ydown(100.0f) - fStartingPos);
				else
					fTextAlpha = 255.0f;
			}

			if ( fTextAlpha > 255.0f )
				fTextAlpha = 255.0f;

			CStats::ConstructStatLine(dwLoopCounter, nIndents);

			CFont::SetDropColor(CRGBA(0, 0, 0, static_cast<BYTE>(fTextAlpha)));
			CFont::SetOrientation(ALIGN_Left);
			CFont::SetColor(CRGBA(255, 255, 255, static_cast<BYTE>(fTextAlpha)));
			CFont::PrintString(_xleft(50.0f + (nIndents * 3.0f)), fStartingPos, gString);
			CFont::SetOrientation(ALIGN_Right);
			CFont::PrintString(_x(50.0f), fStartingPos, gUString);
		}
		++dwLoopCounter;
	}
}

void CMenuManager::PrintControllerMapping(ControllerField* pMappings, size_t nCount, const float fReduction)
{
	const float	fButtonWidth = 24.0f * fReduction;
	const float fButtonHeight = fButtonWidth * (448.0f/480.0f);
	const float fTextWidth = 0.3f * fReduction;
	const float fTextHeight = 0.65f * fReduction;

	CFont::SetColor(CRGBA(255, 255, 255));
	CFont::SetEdge(0);
	CFont::SetFontStyle(FONT_Eurostile);
	CFont::SetScale(_width(fTextWidth), _height(fTextHeight));

	for ( size_t i = 0; i < nCount; i++ )
	{
		if ( pMappings[i].posX == 0.0 && pMappings[i].posY == 0.0 )
		{
			// Auto-spacing
			pMappings[i].posX = pMappings[i-1].posX;
			if ( pMappings[i-1].buttonID == 0 && pMappings[i].buttonID == 0 )
				pMappings[i].posY = pMappings[i-1].posY + (fButtonHeight * 0.5f);
			else if ( pMappings[i-1].buttonID == 0 || pMappings[i].buttonID == 0 )
				pMappings[i].posY = pMappings[i-1].posY + (fButtonHeight * (2.0f/3.0f));
			else
				pMappings[i].posY = pMappings[i-1].posY + fButtonHeight;
		}

		if ( pMappings[i].buttonID == 0 )
		{
			if ( pMappings[i].alignment == ALIGN_Left )
			{
				m_apMouseTextures[0].Draw(	_xmiddle(pMappings[i].posX + 100.0f), _ymiddle(pMappings[i].posY + 5.0f), 
								_xmiddle(pMappings[i].posX + 100.0), _ymiddle(pMappings[i].posY - 5.0f), 
								_xmiddle(pMappings[i].posX - 10.0f - fButtonWidth), _ymiddle(pMappings[i].posY + 5.0f),
								_xmiddle(pMappings[i].posX - 10.0f - fButtonWidth), _ymiddle(pMappings[i].posY - 5.0f),
								CRGBA(255, 255, 255) );
			}
			else
			{
				m_apMouseTextures[0].Draw(	_xmiddle(pMappings[i].posX - 100.0f), _ymiddle(pMappings[i].posY + 5.0f), 
								_xmiddle(pMappings[i].posX - 100.0f), _ymiddle(pMappings[i].posY - 5.0f), 
								_xmiddle(pMappings[i].posX + 10.0f + fButtonWidth), _ymiddle(pMappings[i].posY + 5.0f),
								_xmiddle(pMappings[i].posX + 10.0f + fButtonWidth), _ymiddle(pMappings[i].posY - 5.0f),
								CRGBA(255, 255, 255) );
			}
		}
		else
		{
			CFont::SetOrientation(pMappings[i].alignment);
			if ( pMappings[i].alignment == ALIGN_Left )
			{
				CFont::PrintString( _xmiddle(pMappings[i].posX), _ymiddle(pMappings[i].posY - (8.0f*fTextHeight)), TheText.Get( GetActionNameForFrontend(pMappings[i].buttonID) ) );
				CFont::PS2Sprite[pMappings[i].buttonID].Draw(CRect( _xmiddle(pMappings[i].posX - 10.0f - fButtonWidth), _ymiddle(pMappings[i].posY + (0.5f*fButtonHeight)), _xmiddle(pMappings[i].posX - 10.0f), _ymiddle(pMappings[i].posY - (0.5f*fButtonHeight))), CRGBA(255, 255, 255) );	
			}
			else
			{
				CFont::PrintString( _xmiddle(pMappings[i].posX), _ymiddle(pMappings[i].posY - (8.0f*fTextHeight)), TheText.Get( GetActionNameForFrontend(pMappings[i].buttonID) ) );
				CFont::PS2Sprite[pMappings[i].buttonID].Draw(CRect( _xmiddle(pMappings[i].posX + 10.0f), _ymiddle(pMappings[i].posY + (0.5f*fButtonHeight)), _xmiddle(pMappings[i].posX + 10.0f + fButtonWidth), _ymiddle(pMappings[i].posY - (0.5f*fButtonHeight))), CRGBA(255, 255, 255) );	
			}
		}
	}
}

void CMenuManager::PrintControllerSetupScreen()
{
	// Debug rectangle
	//CSprite2d::DrawRect(CRect(_xmiddle(-150.0f), _ymiddle(170.0f), _xmiddle(150.0f), _ymiddle(20.0f)), CRGBA(255, 0, 0));

	// Controller
	CRGBA				BlendColour;
	if (pXboxPad[0]->IsPadConnected())
	{
		int					nColourCycle = CTimer::m_snTimeInMillisecondsPauseMode % 24000;
		const CRGBA			PinkColour(MENU_PINK_R, MENU_PINK_G, MENU_PINK_B);
		const CRGBA			BlueColour(MENU_INACTIVE_R, MENU_INACTIVE_G, MENU_INACTIVE_B);
		const CRGBA			GreenColour(MENU_UPDATES_R, MENU_UPDATES_G, MENU_UPDATES_B);
		const CRGBA			YellowColour(MENU_ACTIVE_R, MENU_ACTIVE_G, MENU_ACTIVE_B);

		if (nColourCycle < 4000)
			BlendColour = BlueColour;
		else if (nColourCycle < 6000)
			BlendColour = BlendSqr(BlueColour, GreenColour, (nColourCycle - 4000.0f) / 2000.0f);
		else if (nColourCycle < 10000)
			BlendColour = GreenColour;
		else if (nColourCycle < 12000)
			BlendColour = BlendSqr(GreenColour, YellowColour, (nColourCycle - 10000.0f) / 2000.0f);
		else if (nColourCycle < 16000)
			BlendColour = YellowColour;
		else if (nColourCycle < 18000)
			BlendColour = BlendSqr(YellowColour, PinkColour, (nColourCycle - 16000.0f) / 2000.0f);
		else if (nColourCycle < 22000)
			BlendColour = PinkColour;
		else
			BlendColour = BlendSqr(PinkColour, BlueColour, (nColourCycle - 22000.0f) / 2000.0f);

		if (FrontEndMenuManager.m_bEnableSkyMenu)
			m_apBackgroundTextures[4].Draw(CRect(_xmiddle(-175.0f), _ymiddle(120.0f), _xmiddle(175.0f), _ymiddle(-55.0f)), BlendColour);
		else
			m_apBackgroundTextures[4].Draw(CRect(_xmiddle(-175.0f), _ymiddle(140.0f), _xmiddle(175.0f), _ymiddle(-35.0f)), BlendColour);
	}
	else
		BlendColour = CRGBA(255, 255, 255);

	if (FrontEndMenuManager.m_bEnableSkyMenu)
		m_apBackgroundTextures[3].Draw(CRect(_xmiddle(-175.0f), _ymiddle(120.0f), _xmiddle(175.0f), _ymiddle(-55.0f)), BlendSqr(BlendColour, CRGBA(255, 255, 255), 0.5f));
	else
		m_apBackgroundTextures[3].Draw(CRect(_xmiddle(-175.0f), _ymiddle(140.0f), _xmiddle(175.0f), _ymiddle(-35.0f)), BlendSqr(BlendColour, CRGBA(255, 255, 255), 0.5f));

	//m_apBackgroundTextures[5].Draw(CRect(_xmiddle(-175.0f), _ymiddle(227.5f), _xmiddle(175.0f), _ymiddle(-122.5f)), CRGBA(255, 255, 255));

	//CLines::ImmediateLine2D( _xmiddle(-200.0f), _ymiddle(-30.0f), _xmiddle(-85.0f), _ymiddle(5.0f), 255, 0, 0, 255, 255, 0, 0, 255 );
	//CLines::ImmediateLine2D( _xmiddle(-200.0f), _ymiddle(10.0f), _xmiddle(-85.0f), _ymiddle(5.0f), 255, 0, 0, 255, 255, 0, 0, 255 );

	// Part of SkyMenu
	float fReduction = 1.0f;
	float fPosY = -60;
	if (FrontEndMenuManager.m_bEnableSkyMenu) {
		fReduction = 0.740f;
		fPosY = -54;
	}

	// Texts
	ControllerField Page_PS3[] = {
		{ 0, -200.0, fPosY, ALIGN_Right },
		{ BUTTON_L2, 0, 0, ALIGN_Right },
		{ BUTTON_L1, 0, 0, ALIGN_Right },
		{ 0, 0, 0, ALIGN_Right },
		{ 0, 0, 0, ALIGN_Right },
		{ BUTTON_UP, 0, 0, ALIGN_Right },
		{ BUTTON_LEFT, 0, 0, ALIGN_Right },
		{ BUTTON_RIGHT, 0, 0, ALIGN_Right },
		{ BUTTON_DOWN, 0, 0, ALIGN_Right },
		{ 0, 0, 0, ALIGN_Right },
		{ 0, 0, 0, ALIGN_Right },
		{ BUTTON_THUMBL, 0, 0, ALIGN_Right },
		{ BUTTON_L3, 0, 0, ALIGN_Right },
		{ 0, 0, 0, ALIGN_Right },
		{ BUTTON_SELECT, 0, 0, ALIGN_Right },
	};

	ControllerField Page_X360[] = {
		{ 0, -200.0, fPosY, ALIGN_Right },
		{ BUTTON_L2, 0, 0, ALIGN_Right },
		{ BUTTON_L1, 0, 0, ALIGN_Right },
		{ 0, 0, 0, ALIGN_Right },
		{ 0, 0, 0, ALIGN_Right },
		{ BUTTON_THUMBL, 0, 0, ALIGN_Right },
		{ BUTTON_L3, 0, 0, ALIGN_Right },
		{ 0, 0, 0, ALIGN_Right },
		{ 0, 0, 0, ALIGN_Right },
		{ BUTTON_UP, 0, 0, ALIGN_Right },
		{ BUTTON_LEFT, 0, 0, ALIGN_Right },
		{ BUTTON_RIGHT, 0, 0, ALIGN_Right },
		{ BUTTON_DOWN, 0, 0, ALIGN_Right },
		{ 0, 0, 0, ALIGN_Right },
		{ BUTTON_SELECT, 0, 0, ALIGN_Right },
	}; 

	ControllerField Page_Shared[] = {
		{ 0, 200.0f, fPosY, ALIGN_Left },
		{ BUTTON_R2, 0, 0, ALIGN_Left },
		{ BUTTON_R1, 0, 0, ALIGN_Left },
		{ 0, 0, 0, ALIGN_Left },
		{ 0, 0, 0, ALIGN_Left },
		{ BUTTON_TRIANGLE, 0, 0, ALIGN_Left },
		{ BUTTON_SQUARE, 0, 0, ALIGN_Left },
		{ BUTTON_CIRCLE, 0, 0, ALIGN_Left },
		{ BUTTON_CROSS, 0, 0, ALIGN_Left },
		{ 0, 0, 0, ALIGN_Left },
		{ 0, 0, 0, ALIGN_Left },
		{ BUTTON_THUMBR, 0, 0, ALIGN_Left },
		{ BUTTON_R3, 0, 0, ALIGN_Left },
		{ 0, 0, 0, ALIGN_Left },
		{ BUTTON_START, 0, 0, ALIGN_Left },
	};

	if (CFont::bX360Buttons)
		PrintControllerMapping(Page_X360, _countof(Page_X360), fReduction);
	else
		PrintControllerMapping(Page_PS3, _countof(Page_PS3), fReduction);

	PrintControllerMapping(Page_Shared, _countof(Page_Shared), fReduction);

	//float		fEntryPosX = -190.0f;
	//float		fEntryPosY = -30.0f;

	/*m_apMouseTextures[0].Draw(	_xmiddle(fEntryPosX - 100.0f), _ymiddle(fEntryPosY + 5.0f), 
								_xmiddle(fEntryPosX - 100.0f), _ymiddle(fEntryPosY - 5.0f), 
								_xmiddle(fEntryPosX + 10.0f + fButtonWidth), _ymiddle(fEntryPosY + 5.0f),
								_xmiddle(fEntryPosX + 10.0f + fButtonWidth), _ymiddle(fEntryPosY - 5.0f),
								CRGBA(255, 255, 255) );*/

	//fEntryPosY += 10.0f;
	

	/*CFont::SetScale(_width(0.9f), _height(1.5f));
	CFont::SetOrientation(ALIGN_Center);
	CFont::PrintString( _xmiddle(-175.0f), _ymiddle(-40.0f), "~m~" );
	CFont::PrintString( _xmiddle(-175.0f), _ymiddle(-15.0f), "~K~" );*/
	/*CFont::PrintString(_xmiddle(-155.0f), _ymiddle(30.0f), "DUMMY");
	CFont::PrintString(_xmiddle(-155.0f), _ymiddle(45.0f), "DUMMIER");
	CFont::PrintString(_xmiddle(-155.0f), _ymiddle(75.0f), "COOKIES R GUD");
	CFont::PrintString(_xmiddle(-155.0f), _ymiddle(130.0f), "Movement");
	CFont::PrintString(_xmiddle(-155.0f), _ymiddle(140.0f), "Horn");
	CFont::PrintString(_xmiddle(-155.0f), _ymiddle(160.0f), "Camera Modes");*/

	//CFont::SetOrientation(ALIGN_Left);
	/*CFont::PrintString(_xmiddle(155.0f), _ymiddle(30.0f), "DUMMY");
	CFont::PrintString(_xmiddle(155.0f), _ymiddle(45.0f), "DUMMIEST");
	CFont::PrintString(_xmiddle(155.0f), _ymiddle(65.0f), "This would be Square");
	CFont::PrintString(_xmiddle(155.0f), _ymiddle(80.0f), "DUMMYY");*/

	//CFont::SetOrientation(ALIGN_Center);
	/*CFont::PrintString(RsGlobal.MaximumWidth / 2, _ymiddle(30.0f), "DUMMY");*/
}

const char* CMenuManager::GetActionNameForFrontend( int button )
{
	return m_bPadPageShown == 0 ? GetActionNameForFrontend_OnFoot( button ) : GetActionNameForFrontend_InCar( button );
}

const char* CMenuManager::GetActionNameForFrontend_OnFoot( int button )
{
	uint32_t padMode = CPad::SavedMode;
	switch ( button )
	{
	case BUTTON_UP:
		switch ( padMode )
		{
		case CONTROLS_STANDARD_1:
		case CONTROLS_STANDARD_2:
			return "FEC_MOV";
		case CONTROLS_MODERN:
			return "FEC_NA";
		
		}
		break;
	case BUTTON_DOWN:
		switch ( padMode )
		{
		case CONTROLS_STANDARD_1:
		case CONTROLS_STANDARD_2:
			return "FEC_MOV";
		case CONTROLS_MODERN:
			return "FEC_NA";

		}
		break;
	case BUTTON_LEFT:
		switch ( padMode )
		{
		case CONTROLS_STANDARD_1:
		case CONTROLS_STANDARD_2:
			return "FEC_MOV";
		case CONTROLS_MODERN:
			return "FEC_CWL";

		}
		break;
	case BUTTON_RIGHT:
		switch ( padMode )
		{
		case CONTROLS_STANDARD_1:
		case CONTROLS_STANDARD_2:
			return "FEC_MOV";
		case CONTROLS_MODERN:
			return "FEC_CWR";

		}
		break;
	case BUTTON_CROSS:
		return "FEC_RUN";
	case BUTTON_CIRCLE:
		switch ( padMode )
		{
		case CONTROLS_STANDARD_1:
		case CONTROLS_STANDARD_2:
			return "FEC_FWE";
		case CONTROLS_MODERN:
			return "FEC_REL";

		}
		break;
	case BUTTON_SQUARE:
		return "FEC_JMP";
	case BUTTON_TRIANGLE:
		return "FEC_ENV";
	case BUTTON_L1:
		switch ( padMode )
		{
		case CONTROLS_STANDARD_1:
		case CONTROLS_STANDARD_2:
			return "FEC_CEN";
		case CONTROLS_MODERN:
			return "FEC_PKU";

		}
		break;
	case BUTTON_L2:
		switch ( padMode )
		{
		case CONTROLS_STANDARD_1:
		case CONTROLS_STANDARD_2:
			return "FEC_CWL";
		case CONTROLS_MODERN:
			return "FEC_TAR";

		}
		break;
	case BUTTON_L3:
		return CPad::bSouthpaw ? "FEC_LBA" : "FEC_CRO";
	case BUTTON_R1:
		switch ( padMode )
		{
		case CONTROLS_STANDARD_1:
		case CONTROLS_STANDARD_2:
			return "FEC_TAR";
		case CONTROLS_MODERN:
			return "FEC_CEN";

		}
		break;
	case BUTTON_R2:
		switch ( padMode )
		{
		case CONTROLS_STANDARD_1:
		case CONTROLS_STANDARD_2:
			return "FEC_CWR";
		case CONTROLS_MODERN:
			return "FEC_FWE";

		}
		break;
	case BUTTON_R3:
		return CPad::bSouthpaw ? "FEC_CRO" : "FEC_LBA";
	case BUTTON_START:
		return "FEC_PAS";
	case BUTTON_SELECT:
		return "FEC_CMR";
	case BUTTON_THUMBL:
		return CPad::bSouthpaw ? "FEC_LOK" : "FEC_MOV";
	case BUTTON_THUMBR:
		return CPad::bSouthpaw ? "FEC_MOV" : "FEC_LOK";
	}
	return "";
}

const char* CMenuManager::GetActionNameForFrontend_InCar( int button )
{
	uint32_t padMode = CPad::SavedMode;
	switch ( button )
	{
	case BUTTON_UP:
		switch ( padMode )
		{
		case CONTROLS_STANDARD_1:
		case CONTROLS_STANDARD_2:
			return "FEC_VES";
		case CONTROLS_MODERN:
			return "FEC_NA";

		}
		break;
	case BUTTON_DOWN:
		switch ( padMode )
		{
		case CONTROLS_STANDARD_1:
		case CONTROLS_STANDARD_2:
			return "FEC_VES";
		case CONTROLS_MODERN:
			return "FEC_NA";

		}
		break;
	case BUTTON_LEFT:
		switch ( padMode )
		{
		case CONTROLS_STANDARD_1:
		case CONTROLS_STANDARD_2:
			return "FEC_VES";
		case CONTROLS_MODERN:
			return "FEC_RSP";

		}
		break;
	case BUTTON_RIGHT:
		switch ( padMode )
		{
		case CONTROLS_STANDARD_1:
		case CONTROLS_STANDARD_2:
			return "FEC_VES";
		case CONTROLS_MODERN:
			return "FEC_RSC";

		}
		break;
	case BUTTON_CROSS:
		switch ( padMode )
		{
		case CONTROLS_STANDARD_1:
		case CONTROLS_STANDARD_2:
			return "FEC_ACC";
		case CONTROLS_MODERN:
			return "FEC_HND";

		}
		break;
	case BUTTON_CIRCLE:
		return "FEC_FWE";
	case BUTTON_SQUARE:
		switch ( padMode )
		{
		case CONTROLS_STANDARD_1:
		case CONTROLS_STANDARD_2:
			return "FEC_BRA";
		case CONTROLS_MODERN:
			return "FEC_NA";

		}
		break;
	case BUTTON_TRIANGLE:
		return "FEC_EXV";
	case BUTTON_L1:
		switch ( padMode )
		{
		case CONTROLS_STANDARD_1:
		case CONTROLS_STANDARD_2:
			return "FEC_NA";
		case CONTROLS_MODERN:
			return "FEC_LOL";

		}
		break;
	case BUTTON_L2:
		switch ( padMode )
		{
		case CONTROLS_STANDARD_1:
		case CONTROLS_STANDARD_2:
			return "FEC_LOL";
		case CONTROLS_MODERN:
			return "FEC_BRA";

		}
		break;
	case BUTTON_L3:
		return CPad::bSouthpaw ? "FEC_SUB" : "FEC_HOR";
	case BUTTON_R1:
		switch ( padMode )
		{
		case CONTROLS_STANDARD_1:
		case CONTROLS_STANDARD_2:
			return "FEC_HND";
		case CONTROLS_MODERN:
			return "FEC_LOR";

		}
		break;
	case BUTTON_R2:
		switch ( padMode )
		{
		case CONTROLS_STANDARD_1:
		case CONTROLS_STANDARD_2:
			return "FEC_LOR";
		case CONTROLS_MODERN:
			return "FEC_ACC";

		}
		break;
	case BUTTON_R3:
		return CPad::bSouthpaw ? "FEC_HOR" : "FEC_SUB";
	case BUTTON_START:
		return "FEC_PAS";
	case BUTTON_SELECT:
		return "FEC_CMR";
	case BUTTON_THUMBL:
		return CPad::bSouthpaw ? "FEC_LOK" : "FEC_VES";
	case BUTTON_THUMBR:
		return CPad::bSouthpaw ? "FEC_VES" : "FEC_LOK";
	}
	return "";
}

void CMenuManager::PrintUpdaterScreen()
{
	CFont::SetScale(_width(0.8f), _height(1.2f));
	CFont::SetFontStyle(FONT_RageItalic);
	CFont::SetDropColor(CRGBA(0, 0, 0, 255));
	CFont::SetEdge(1);
	CFont::SetOrientation(ALIGN_Center);

	CFont::SetColor(CRGBA(MENU_INACTIVE_R, MENU_INACTIVE_G, MENU_INACTIVE_B, 255));
	CFont::PrintString(_x(137.5f), _y(11.0f), TheText.Get("FEU_POW"));

	CSprite2d::DrawRect(CRect(_xmiddle(-300.0f), _y(77.5f), _xmiddle(300.0f), _ydown(142.5f)), CRGBA(MENU_BOX_BLUE_R, MENU_BOX_BLUE_G, MENU_BOX_BLUE_B, MENU_BOX_BLUE_A));

	CFont::SetFontStyle(FONT_Eurostile);
	CFont::SetScale(_width(0.3f), _height(0.7f));
	CFont::SetColor(CRGBA(255, 255, 255, 255));
	CFont::SetOrientation(ALIGN_Left);

	float		fStartingPos = _height(82.5f);

	for ( int i = 0; i < NUM_MESSAGES_PER_UPT_SCREEN; ++i )
	{
		if ( const char* pLine = CUpdateManager::FetchMessage(i) )
		{
			if ( pLine[0] )
			{
				CFont::PrintString(_xmiddle(-285.0f), fStartingPos, pLine);
				fStartingPos += _height(17.5f);
			}
			else
				break;
		}
		else
			break;
	}

	float		dDownloadPercentage = CUpdateManager::GetDownloadProgress();

	CSprite2d::DrawRect(CRect(_xmiddle(-284.0f), _ydown(149.0f), _xmiddle(286.0f), _ydown(169.0f)), CRGBA(0, 0, 0, 255));

	CSprite2d::DrawRect(CRect(_xmiddle(-285.0f), _ydown(150.0f), _xmiddle(-285.0f + ((2.0f*2.85f) * dDownloadPercentage)), _ydown(170.0f)), CRGBA(MENU_PINK_R, MENU_PINK_G, MENU_PINK_B, 255));
	CSprite2d::DrawRect(CRect(_xmiddle(-285.0f + ((2.0f*2.85f) * dDownloadPercentage)), _ydown(150.0f), _xmiddle(285.0f), _ydown(170.0f)), CRGBA(MENU_INACTIVE_PINK_R, MENU_INACTIVE_PINK_G, MENU_INACTIVE_PINK_B, 255));

	CFont::SetScale(_width(0.35f), _height(0.7f));
	CFont::SetFontStyle(FONT_PagerFont);
	CFont::SetOrientation(ALIGN_Center);
	if ( CUpdateManager::IsDownloading() )
	{
		CMessages::InsertNumberInString(TheText.Get("FEU_PRC"), static_cast<int>(dDownloadPercentage), -1, -1, -1, -1, -1, gString);
		CFont::PrintString(static_cast<float>(RsGlobal.MaximumWidth / 2), _ydown(165.0f), gString);
	}

}

void CMenuManager::PrintDLCScreen()
{
	if ( m_dwSelectedMenuItem > 1 )
	{
		m_nFocusedDLC = CDLCManager::GetActiveDLCWithIndex(m_dwSelectedMenuItem-2);

		bool		bThisDLCIsEnabled = CDLCManager::GetDLC(static_cast<eExpansionPack>(m_nFocusedDLC))->IsActive();
		bool		bThisDLCIsInstalled = CDLCManager::GetDLC(static_cast<eExpansionPack>(m_nFocusedDLC))->IsInstalled();
		if ( m_nLastFocusedDLC != m_nFocusedDLC )
		{
			m_nLastFocusedDLC = m_nFocusedDLC;

			// Load a video
			char			cVideoPath[64];
			const CRect		videoFrame(_x(250.0f), _ymiddle(140.0f), _x(30.0f), _ymiddle(-65.0f));
			sprintf(cVideoPath, "movies\\dlc%d.bik", m_nFocusedDLC);

			CVideoPlayer::Release();
			CVideoPlayer::Create(cVideoPath, &videoFrame, false, !bThisDLCIsInstalled);
		}

		char		cGXTName[8];
		sprintf(cGXTName, "FEE_D%02d", m_nFocusedDLC);

		CSprite2d::DrawRect(CRect(_x(251.5f), _ymiddle(195.5f), _x(26.5f), _ymiddle(-66.5f)), CRGBA(0, 0, 0, 255));
		CSprite2d::DrawRect(CRect(_x(252.5f), _ymiddle(194.5f), _x(27.5f), _ymiddle(-67.5f)), CRGBA(MENU_INACTIVE_PINK_R, MENU_INACTIVE_PINK_G, MENU_INACTIVE_PINK_B, 255));
		CVideoPlayer::PlayNextFrame();

		CFont::SetWrapx(_x(30.0f));
		CFont::SetColor(CRGBA(255, 255, 255, 255));
		CFont::SetDropShadowPosition(1);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetScale(_width(0.35f), _height(0.7f));
		CFont::SetFontStyle(FONT_PagerFont);
		CFont::SetJustify(true);
		CFont::PrintString(_x(249.0f), _ymiddle(142.5f), TheText.Get(cGXTName));

		if ( !bThisDLCIsInstalled )
			CFont::SetColor(CRGBA(MENU_RED_R, MENU_RED_G, MENU_RED_B, 255));
		else
		{
			if ( bThisDLCIsEnabled )
				CFont::SetColor(CRGBA(MENU_UPDATES_R, MENU_UPDATES_G, MENU_UPDATES_B, 255));
		}
		CFont::SetScale(_width(0.6f), _height(1.1f));
		CFont::SetJustify(false);
		CFont::SetOrientation(ALIGN_Center);
		CFont::PrintString(_x(140.0f), _ymiddle(-87.5f), TheText.Get(bThisDLCIsInstalled ? (bThisDLCIsEnabled ? "FEE_ON" : "FEE_OFF") : "FEE_ABS"));
	}
	else
	{
		CVideoPlayer::Release();
		m_nFocusedDLC = m_nLastFocusedDLC = -1;
	}
}

void CMenuManager::ResetVCSMap() {
	m_fVCSMapBaseX = 0.0;
	m_fVCSMapBaseY = 0.0;
	m_fVCSMapZoom = 0.0;
}

void CMenuManager::PrintVCSMap() {
	float MOUSEX = FrontEndMenuManager.m_dwMousePosLeft;
	float MOUSEY = FrontEndMenuManager.m_dwMousePosTop;

	if (CPad::GetPad(0)->NewMouseControllerState.wheelUp || currKeyState->pgup) {	// Zoom in.
		if (m_fVCSMapZoom < 1600.0)
			m_fVCSMapZoom += CTimer::ms_fTimeScale * 0.1 * 1000.0;

		// Limit
		if (m_fVCSMapZoom > 1600.0)
			m_fVCSMapZoom = 1600.0;
	}
	else if (CPad::GetPad(0)->NewMouseControllerState.wheelDown || currKeyState->pgdn) { // Zoom out
		if (m_fVCSMapZoom > 0.0)
			m_fVCSMapZoom += CTimer::ms_fTimeScale * 0.1 * -1000.0;

		if (m_fVCSMapBaseX > 0.0)
			m_fVCSMapBaseX += CTimer::ms_fTimeScale * 0.1 * -1000.0;

		if (m_fVCSMapBaseY > 0.0)
			m_fVCSMapBaseY += CTimer::ms_fTimeScale * 0.1 * -1000.0;

		// Limit
		if (m_fVCSMapZoom < 0.0)
			m_fVCSMapZoom = 0.0;
	}

	CPad* pad = CPad::GetPad(0);
	int KEY_UP = pad->NewState.DPADUP || currKeyState->up || pad->NewState.LEFTSTICKY < 0;
	int KEY_DOWN = pad->NewState.DPADDOWN || currKeyState->down || pad->NewState.LEFTSTICKY > 0;

	int KEY_LEFT = pad->NewState.DPADLEFT || currKeyState->left || pad->NewState.LEFTSTICKX < 0;
	int KEY_RIGHT = pad->NewState.DPADRIGHT || currKeyState->right || pad->NewState.LEFTSTICKX > 0;

	// Movements
	if (CPad::GetPad(0)->NewMouseControllerState.lmb) {
		if (CPad::NewMouseControllerState.X)
			m_fVCSMapBaseX += CTimer::ms_fTimeScale * 0.0008 * CPad::NewMouseControllerState.X * 1000.0;

		if (CPad::NewMouseControllerState.Y)
			m_fVCSMapBaseY += CTimer::ms_fTimeScale * 0.0008 * CPad::NewMouseControllerState.Y * 1000.0;
	}
	else {
			m_fVCSMapBaseX += CTimer::ms_fTimeScale * 0.03 * KEY_RIGHT * -1000.0;
			m_fVCSMapBaseX += CTimer::ms_fTimeScale * 0.03 * KEY_LEFT * 1000.0;

			m_fVCSMapBaseY += CTimer::ms_fTimeScale * 0.03 * KEY_UP * 1000.0;
			m_fVCSMapBaseY += CTimer::ms_fTimeScale * 0.03 * KEY_DOWN * -1000.0;
	}

	// Limits
	if (m_fVCSMapBaseX > m_fVCSMapZoom)
		m_fVCSMapBaseX = m_fVCSMapZoom;
	if (m_fVCSMapBaseX < -m_fVCSMapZoom)
		m_fVCSMapBaseX = -m_fVCSMapZoom;

	if (m_fVCSMapBaseY > m_fVCSMapZoom)
		m_fVCSMapBaseY = m_fVCSMapZoom;
	if (m_fVCSMapBaseY < -m_fVCSMapZoom)
		m_fVCSMapBaseY = -m_fVCSMapZoom;

	// Draw
	CVector2D vec = WidescreenSupport::GetFullscreenImageDimensions(1024.0f / 1024.0f, CDraw::ms_fAspectRatio, true);
	CRect rect;
	rect.x1 = (0.5f * (RsGlobal.MaximumWidth - vec.x)) + ((FrontEndMenuManager.m_fVCSMapBaseX - FrontEndMenuManager.m_fVCSMapZoom));
	rect.y1 = (0.5f * (RsGlobal.MaximumHeight + vec.y)) + ((FrontEndMenuManager.m_fVCSMapBaseY + FrontEndMenuManager.m_fVCSMapZoom));
	rect.x2 = (0.5f * (RsGlobal.MaximumWidth + vec.x)) + ((FrontEndMenuManager.m_fVCSMapBaseX + FrontEndMenuManager.m_fVCSMapZoom));
	rect.y2 = (0.5f * (RsGlobal.MaximumHeight - vec.y)) + ((FrontEndMenuManager.m_fVCSMapBaseY - FrontEndMenuManager.m_fVCSMapZoom));

	FrontEndMenuManager.m_apBackgroundTextures[2].Draw(CRect(rect), CRGBA(255, 255, 255, 255));

	// Blips :(
	//CMenuManager::DrawYouAreHereSprite(vec);

	// Cursor
	bool m_bDrawCursor = true;
	if (m_bDrawCursor) {
		m_bDrawMouse = false;
		CRect rect;
		rect.x1 = NULL;
		rect.y1 = MOUSEY;
		rect.x2 = _x(0.0f);
		rect.y2 = rect.y1 + _height(3.0f);
		CSprite2d::DrawRect(CRect(rect), CRGBA(MENU_PINK_R, MENU_PINK_G, MENU_PINK_B, 200)); // Hor
		rect.x1 = MOUSEX + _height(3.5f);
		rect.y1 = NULL;
		rect.x2 = rect.x1 - _height(3.5f);
		rect.y2 = _ydown(0.0f);
		CSprite2d::DrawRect(CRect(rect), CRGBA(MENU_PINK_R, MENU_PINK_G, MENU_PINK_B, 200)); // Vert
	}
}

void CMenuManager::PrintActivationScreen()
{
	if ( m_nDLCMessageTimer > CTimer::m_snTimeInMillisecondsPauseMode )
	{
		CFont::RenderFontBuffer();
		SmallMessageScreen(m_pDLCMessage);
	}
	else
	{
		if ( !CDLCManager::IsContactingWebsite() )
		{
			float		fInitialPos = -(SERIAL_CODES_LENGTH*115.0f - 40.0f)/2.0f;

			CFont::SetProportional(false);
			CFont::SetColor(CRGBA(255, 255, 255, 255));
			CFont::SetDropShadowPosition(1);
			CFont::SetDropColor(CRGBA(0, 0, 0, 255));
			CFont::SetScale(_width(0.65f), _height(1.0f));
			CFont::SetFontStyle(FONT_PagerFont);
			CFont::SetOrientation(ALIGN_Center);

			for ( int i = 0; i < SERIAL_CODES_LENGTH; i++, fInitialPos += 115.0f )
			{
				CSprite2d::DrawRect(CRect(_xmiddle(fInitialPos), _ymiddle(15.0f), _xmiddle(fInitialPos + 75.0f), _ymiddle(-15.0f)), CRGBA(0, 0, 0, 255));
				CSprite2d::DrawRect(CRect(_xmiddle(fInitialPos+1.0f), _ymiddle(14.0f), _xmiddle(fInitialPos + 74.0f), _ymiddle(-14.0f)), CRGBA(MENU_INACTIVE_PINK_R, MENU_INACTIVE_PINK_G, MENU_INACTIVE_PINK_B, 255));
	
				if ( i != SERIAL_CODES_LENGTH-1 )
					CFont::PrintString(_xmiddle(fInitialPos + 95.0f), _ymiddle(-9.0f), "-");

				CFont::PrintString(_xmiddle(fInitialPos + 37.5f), _ymiddle(-9.0f), m_strSerialCode[i].c_str());
			}

			CFont::SetProportional(true);
		}
		else
		{
			CFont::RenderFontBuffer();
			SmallMessageScreen("FEE_W8");
		}
	}
}

void CMenuManager::ReadFrontendTextures()
{
	static const char* const	frontend1TexNames[] = {
									"radio_flash",
									"radio_vrock",
									"radio_paradise",
									"radio_VCPR",
									"radio_VCFL",
									"radio_wave103",
									"radio_fresh",
									"radio_espantoso",
									"radio_emotion",
									"radio_TPLAYER" };

	static const char* const	frontend2TexNames[] = {
									"background",
									"modbase",
									"map",
#ifdef INCLUDE_PROMO_BANNER
									"banner",
#endif
											};

	static const char* const	frontendpcTexNames[] = {
									"scrollback",
									"scrollfront",
									"mousepointers_texturesheet",
									 };

	CPNGArchive			FrontendArchive("models\\frontend.spta");
	CPNGArchive			DLCArchive(CFileLoader::GetFrontendPath());

	// fronten1
	FrontendArchive.SetDirectory("fronten1");

	for ( int i = 0; i < sizeof(frontend1TexNames)/sizeof(const char*); ++i )
	{
		m_apRadioSprites[i+1].SetTextureFromSPTA(FrontendArchive, frontend1TexNames[i]);
	}

	// fronten2
	FrontendArchive.SetDirectory("fronten2");
	DLCArchive.SetDirectory("fronten2");

	for ( int i = 0; i < sizeof(frontend2TexNames)/sizeof(const char*); ++i )
	{
		if ( !m_apBackgroundTextures[i].SetTextureFromSPTA(DLCArchive, frontend2TexNames[i]) )
			m_apBackgroundTextures[i].SetTextureFromSPTA(FrontendArchive, frontend2TexNames[i]);
	}

	// fronten_pc
	FrontendArchive.SetDirectory("fronten_pc");

	for ( int i = 0; i < sizeof(frontendpcTexNames)/sizeof(const char*); ++i )
	{
		m_apMouseTextures[i].SetTextureFromSPTA(FrontendArchive, frontendpcTexNames[i]);
	}

	FrontendArchive.CloseArchive();
	DLCArchive.CloseArchive();
}

void CMenuManager::LoadControllerSprites()
{
	CPNGArchive			FrontendArchive("models\\frontend.spta");
	FrontendArchive.SetDirectory("fronten2");

	if ( CFont::bX360Buttons )
	{
		m_apBackgroundTextures[3].SetTextureFromSPTA(FrontendArchive, "360front");
		m_apBackgroundTextures[4].SetTextureFromSPTA(FrontendArchive, "360back");
		m_apBackgroundTextures[5].SetTextureFromSPTA(FrontendArchive, "360overlay");
	}
	else
	{
		m_apBackgroundTextures[3].SetTextureFromSPTA(FrontendArchive, "ps3front");
		m_apBackgroundTextures[4].SetTextureFromSPTA(FrontendArchive, "ps3back");	
		m_apBackgroundTextures[5].SetTextureFromSPTA(FrontendArchive, "ps3overlay");	
	}

	FrontendArchive.CloseArchive();
}

void CMenuManager::UnloadControllerSprites()
{
	m_apBackgroundTextures[3].Delete();
	m_apBackgroundTextures[4].Delete();
}

void CMenuManager::SwitchToNewScreen(signed char bScreen)
{
	m_fScrollerOffset = 0.0f;

	((void(__thiscall*)(CMenuManager*, signed char))0x573680)(this, bScreen);

	if (bScreen == 0)
		m_fStatsScrollPos = -120.0f;
	else if (bScreen == MENU_PAGE_CONTROLLER_SETUP || m_bLastMenuPage == MENU_PAGE_ADDITIONAL_CONTROLLER)
		LoadControllerSprites();
	else if (bScreen == 44)
		CUpdateManager::ReportUpdaterScreenSeen();
	else if (bScreen == 45)
	{
		if (CDLCManager::AnyDLCsAvailable())
		{
			m_dwSelectedMenuItem = 2;

			for (int i = 0; i < NUM_DLC_PACKS; i++)
				m_bLastDLCState[i] = CDLCManager::GetDLC(i)->IsActive();
		}
		else
		{
			m_bCurrentMenuPage = 46;
			m_dwSelectedMenuItem = 1;
		}
	}
	else if (bScreen == 46)
		m_dwSelectedMenuItem = 1;

	// Unload BINK and check DLC state
	if (m_bLastMenuPage == 45)
	{
		m_nLastFocusedDLC = -1;
		CVideoPlayer::Release();

		// Display a message only if one does not want to install a DLC
		if (bScreen != 48)
		{
			for (int i = 0; i < NUM_DLC_PACKS; i++)
			{
				if (m_bLastDLCState[i] != CDLCManager::GetDLC(i)->IsActive())
				{
					m_bCurrentMenuPage = 47;
					m_dwSelectedMenuItem = 1;
					break;
				}
			}
		}
	}

	if (m_bLastMenuPage == MENU_PAGE_CONTROLLER_SETUP && m_bCurrentMenuPage != MENU_PAGE_CONTROLLER_SETUP)
		UnloadControllerSprites();

	// Focus on DLC entry when exiting from any DLC-oriented screen
	if (m_bLastMenuPage == 46 || m_bLastMenuPage == 47)
		m_dwSelectedMenuItem = 5;

	// Automatically check for updates and mark focused DLC for installation
	if (bScreen == 44 && m_bLastMenuPage == 48)
	{
		CUpdateManager::SetDLCStatus(CDLCManager::GetDLC(m_nFocusedDLC)->GetName(), true);
		CUpdateManager::ForceUpdate();
	}

	// Correct the first entry
	if (bScreen == 48)
		_snprintf(FrontEndMenuManager._aScreens[48].entryList[0].entry, sizeof(FrontEndMenuManager._aScreens->entryList->entry), "FEE_I%02d", m_nFocusedDLC);

	// Clear serial code buffer
	if (m_bLastMenuPage == 49)
		ClearSerialsBuffer();

	if (bScreen == 49)
	{
		// Check for key's presence in the clipboard
		LookIntoClipboardForSerial();
	}

	// Fix stuff for SkyMenu.
	if (FrontEndMenuManager.m_bEnableSkyMenu) {
		// Fix first entry after returning from quit menu page.
		if (m_bLastMenuPage == 35)
			m_dwSelectedMenuItem = 3;

		// Fix radio playing.
		if (FrontEndMenuManager.m_bCurrentMenuPage == 3 && !FrontEndMenuManager.m_bBottomMenu)
			((void(__stdcall*)(int, char))0x507DC0)(FrontEndMenuManager.m_nRadioStation, 0);
		else
			((void(__stdcall*)(int, char))0x506F70)(0, 0);

		// Set map
		if (m_bLastMenuPage != 5) {
			FrontEndMenuManager.m_fMapBaseX = 320.0f;
			FrontEndMenuManager.m_fMapBaseY = 170.0f;
			FrontEndMenuManager.m_fMapZoom = 310.0f;
		}
	}
}

void CMenuManager::RegisterDLCMessage(const char* pMessage)
{
	m_pDLCMessage = pMessage;
	m_nDLCMessageTimer = CTimer::m_snTimeInMillisecondsPauseMode + 5000;
}

void CMenuManager::LookIntoClipboardForSerial()
{
	if ( OpenClipboard(nullptr) )
	{
		if ( HANDLE hText = GetClipboardData(CF_TEXT) )
		{
			const char*	pText = static_cast<char*>(GlobalLock(hText));
			bool		bAllValid = true;

			// Iterate through this text
			while ( *pText == ' ' )
				++pText;

			// 1st field
			for ( int i = 0; i < SERIAL_CODES_LENGTH; i++ )
			{
				for ( int j = 0; j < SERIAL_CODES_ONE_WINDOW; j++ )
				{
					if ( ValidSerialCharacter(*pText) )
					{
						m_strSerialCode[i] += *pText++;
					}
					else
					{
						bAllValid = false;
						break;
					}	
				}

				if ( !bAllValid )
					break;

				for ( auto it = m_strSerialCode[i].begin(); it != m_strSerialCode[i].end(); it++ )
					*it = static_cast<char>(toupper(*it));

				if ( i != SERIAL_CODES_LENGTH-1 )
				{
					// Filter out spaces and look for -
					while ( *pText == ' ' )
						++pText;

					if ( *pText++ != '-' )
					{
						bAllValid = false;
						break;
					}

					while ( *pText == ' ' )
						++pText;
				}
			}

			GlobalUnlock(hText);

			if ( !bAllValid )
				ClearSerialsBuffer();
			else
				m_bSerialFull = true;
		}

		CloseClipboard();
	}

}

void CMenuManager::UserInputVCS()
{
	if (!FrontEndMenuManager.m_bSwitchToSkyMenu) {
		if (nTimeToStopPadShake != 0)
		{
			if (CTimer::m_snTimeInMillisecondsPauseMode > nTimeToStopPadShake)
			{
				CPad::GetPad(0)->StopShaking(0);
				nTimeToStopPadShake = 0;
			}
		}

		if (CDLCManager::IsContactingWebsite())
			return;

		if (m_nDLCMessageTimer > CTimer::m_snTimeInMillisecondsPauseMode)
			return;
		else if (m_nSwitchToThisAfterMessage != -1)
		{
			SwitchToNewScreen(m_nSwitchToThisAfterMessage);
			m_nSwitchToThisAfterMessage = -1;
		}

		// Call SA UserInput
		//((void(__thiscall*)(CMenuManager*))0x57FD70)(this);

		m_nNumMenuEntries = 0;
		for (int i = 0; i < NUM_ENTRIES_PER_MENU; i++)
		{
			if (FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[i].action == 0)
				break;
			m_nNumMenuEntries++;
		}

		float fBonusMargin = m_dwSelectedMenuItem != 0 && m_dwSelectedMenuItem != m_nNumMenuEntries - 1 ? _height(0.0f /*30.0f*/) : 0.0f;

		// Shuffle the entries so m_nScrollerPos entry is visible
		while (GetTextYPos(m_dwSelectedMenuItem) + m_fScrollerOffset < _y(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].topMargin) + fBonusMargin)
			m_fScrollerOffset += _height(10.0f);

		while (GetTextYPos(m_dwSelectedMenuItem) + m_fScrollerOffset > _ydown(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].bottomMargin + 15.0f) - fBonusMargin)
			m_fScrollerOffset -= _height(10.0f);

		// SkyMenu
		if (FrontEndMenuManager.m_bEnableSkyMenu) {
			CPad *pad = CPad::GetPad(0);

			int KEY_UP = pad->NewState.DPADUP != 0 && pad->OldState.DPADUP == 0 || currKeyState->up != 0 && prevKeyState->up == 0 || pad->NewState.LEFTSTICKY < 0 && !pad->OldState.LEFTSTICKY;
			int KEY_DOWN = pad->NewState.DPADDOWN != 0 && pad->OldState.DPADDOWN == 0 || currKeyState->down != 0 && prevKeyState->down == 0 || pad->NewState.LEFTSTICKY > 0 && !pad->OldState.LEFTSTICKY;

			int KEY_LEFT = pad->NewState.DPADLEFT != 0 && pad->OldState.DPADLEFT == 0 || currKeyState->left != 0 && prevKeyState->left == 0 || pad->NewState.LEFTSTICKX < 0 && !pad->OldState.LEFTSTICKX;
			int KEY_RIGHT = pad->NewState.DPADRIGHT != 0 && pad->OldState.DPADRIGHT == 0 || currKeyState->right != 0 && prevKeyState->right == 0 || pad->NewState.LEFTSTICKX > 0 && !pad->OldState.LEFTSTICKX;

			int LMB = pad->NewMouseControllerState.lmb == 0 && pad->OldMouseControllerState.lmb != 0;
			int KEY_ENTER = pad->NewState.CROSS != 0 && pad->OldState.CROSS == 0 || currKeyState->extenter != 0 && prevKeyState->extenter == 0 || currKeyState->enter != 0 && prevKeyState->enter == 0;
			int KEY_ESC = pad->NewState.START != 0 && pad->OldState.START == 0 || pad->NewState.TRIANGLE != 0 && pad->OldState.TRIANGLE == 0 || currKeyState->esc != 0 && prevKeyState->esc == 0;

			if (!FrontEndMenuManager.m_bBottomMenu || FrontEndMenuManager.m_bPCFrontEnd) {
				((void(__thiscall*)(CMenuManager*))0x57FD70)(this);
			}
			else {
				if (KEY_LEFT || KEY_RIGHT || KEY_UP || KEY_DOWN || KEY_ESC || KEY_ENTER)
					FrontEndMenuManager.m_bDrawMouse = false;
				else if (CPad::GetPad(0)->NewMouseControllerState.X || CPad::GetPad(0)->NewMouseControllerState.Y)
					FrontEndMenuManager.m_bDrawMouse = true;
				else
					FrontEndMenuManager.m_bDrawMouse = FrontEndMenuManager.m_bDrawMouse;

				// Update Mouse Position
				FrontEndMenuManager.field_1AF8 = FrontEndMenuManager.m_dwMousePosLeft;
				FrontEndMenuManager.field_1AFC = FrontEndMenuManager.m_dwMousePosTop;

				FrontEndMenuManager.m_dwMousePosLeft = FrontEndMenuManager.mousePosLeftA;
				FrontEndMenuManager.m_dwMousePosTop = FrontEndMenuManager.mousePosTopA;

				if (FrontEndMenuManager.mousePosLeftA < 0)
					FrontEndMenuManager.m_dwMousePosLeft = 0;
				if (FrontEndMenuManager.m_dwMousePosLeft > RsGlobal.MaximumWidth)
					FrontEndMenuManager.m_dwMousePosLeft = RsGlobal.MaximumWidth;
				if (FrontEndMenuManager.mousePosTopA < 0)
					FrontEndMenuManager.m_dwMousePosTop = 0;
				if (FrontEndMenuManager.m_dwMousePosTop > RsGlobal.MaximumHeight)
					FrontEndMenuManager.m_dwMousePosTop = RsGlobal.MaximumHeight;
			}

			// Set command keys
			if (FrontEndMenuManager.m_bBottomMenu) {
				//FrontEndMenuManager.m_dwSelectedMenuItem = -1;

				if (KEY_ESC) {
					FrontEndMenuManager.field_1B34 = CPad::GetPad(0)->DisablePlayerControls;
					CPad::GetPad(0)->DisablePlayerControls = 1;
					if (FrontEndMenuManager.m_bIsPauseMenu) {
						SwitchToNewScreen(54);
						CPad::GetPad(0)->DisablePlayerControls = FrontEndMenuManager.field_1B34;

						FrontEndMenuManager.m_bBackIntoGame = true;
					}
				}
				if (KEY_ENTER) {
					AudioEngine.ReportFrontendAudioEvent(1, 0.0f, 1.0f);

					FrontEndMenuManager.m_bBottomMenu = false;
				}
				else if (KEY_LEFT) {
					FrontEndMenuManager.m_bIsMenuSwitched = false;
					--FrontEndMenuManager.m_nSelectedSkyMenuItem;
					AudioEngine.ReportFrontendAudioEvent(1, 0.0f, 1.0f);
				}
				else if (KEY_RIGHT) {
					FrontEndMenuManager.m_bIsMenuSwitched = false;
					++FrontEndMenuManager.m_nSelectedSkyMenuItem;
					AudioEngine.ReportFrontendAudioEvent(1, 0.0f, 1.0f);
				}
				else if (KEY_DOWN || KEY_UP) {
					if (FrontEndMenuManager.m_bIsPauseMenu) {
						FrontEndMenuManager.m_bIsMenuSwitched = false;
						if (FrontEndMenuManager.m_nSelectedSkyMenuItem == 0)
							FrontEndMenuManager.m_nSelectedSkyMenuItem = 5;
						else if (FrontEndMenuManager.m_nSelectedSkyMenuItem == 1)
							FrontEndMenuManager.m_nSelectedSkyMenuItem = 6;
						else if (FrontEndMenuManager.m_nSelectedSkyMenuItem == 2)
							FrontEndMenuManager.m_nSelectedSkyMenuItem = 7;
						else if (FrontEndMenuManager.m_nSelectedSkyMenuItem == 3)
							FrontEndMenuManager.m_nSelectedSkyMenuItem = 7;
						else if (FrontEndMenuManager.m_nSelectedSkyMenuItem == 4)
							FrontEndMenuManager.m_nSelectedSkyMenuItem = 7;
						else if (FrontEndMenuManager.m_nSelectedSkyMenuItem == 5)
							FrontEndMenuManager.m_nSelectedSkyMenuItem = 0;
						else if (FrontEndMenuManager.m_nSelectedSkyMenuItem == 6)
							FrontEndMenuManager.m_nSelectedSkyMenuItem = 1;
						else if (FrontEndMenuManager.m_nSelectedSkyMenuItem == 7)
							FrontEndMenuManager.m_nSelectedSkyMenuItem = 3;
						AudioEngine.ReportFrontendAudioEvent(1, 0.0f, 1.0f);
					}
					else {
						FrontEndMenuManager.m_bIsMenuSwitched = false;
						if (FrontEndMenuManager.m_nSelectedSkyMenuItem == 0)
							FrontEndMenuManager.m_nSelectedSkyMenuItem = 3;
						else if (FrontEndMenuManager.m_nSelectedSkyMenuItem == 1)
							FrontEndMenuManager.m_nSelectedSkyMenuItem = 4;
						else if (FrontEndMenuManager.m_nSelectedSkyMenuItem == 2)
							FrontEndMenuManager.m_nSelectedSkyMenuItem = 4;
						else if (FrontEndMenuManager.m_nSelectedSkyMenuItem == 3)
							FrontEndMenuManager.m_nSelectedSkyMenuItem = 0;
						else if (FrontEndMenuManager.m_nSelectedSkyMenuItem == 4)
							FrontEndMenuManager.m_nSelectedSkyMenuItem = 1;
						AudioEngine.ReportFrontendAudioEvent(1, 0.0f, 1.0f);
					}
				}
			}
			else {
				//if (FrontEndMenuManager.m_dwSelectedMenuItem == -1)
					//FrontEndMenuManager.m_dwSelectedMenuItem = 0;

				if (KEY_ESC) {
					int m_dwLastPages[8] = { 5, 2 , 1, 0, 40, 3, 4, 27 };
					for (int i = 0; i < 8; i++) {
						if (FrontEndMenuManager.m_bLastMenuPage == m_dwLastPages[i])
							FrontEndMenuManager.m_bBottomMenu = true;
					}
				}
			}
		}
		else
			((void(__thiscall*)(CMenuManager*))0x57FD70)(this);
	}
}

void CMenuManager::MessageScreen(const char* pMessage, bool bFullscreen, bool bWithinFrame)
{
	if ( !bWithinFrame )
	{
		if ( !RsCameraBeginUpdate(Scene.camera) )
			return;
	}

	CSprite2d::InitPerFrame();
	CFont::InitPerFrame();
	DefinedState2d();

	if ( bFullscreen )
	{
		RwRGBA		colour = { 0, 0, 0, 255 };
		RwCameraClear(Scene.camera, &colour, rwCAMERACLEARIMAGE);
	}

	SmallMessageScreen(pMessage);
	CFont::RenderFontBuffer();

	if ( !bWithinFrame )
		DoRWStuffEndOfFrame();
}

static bool bCenteredCursor;

void CenterMousePosnForMapScreen() {
    RwV2d posn;
    posn.x = (RsGlobal.MaximumWidth / 2) * 1.14f / ScreenAspectRatio;
    posn.y = RsGlobal.MaximumHeight / 2;
    ClientToScreen(RsGlobal.ps->window, (LPPOINT)&posn);
    SetCursorPos((RsGlobal.MaximumWidth / 2) * 1.14f / ScreenAspectRatio, RsGlobal.MaximumHeight / 2);
    RsGlobal.ps->lastMousePos_X = (RsGlobal.MaximumWidth / 2) * 1.14f / ScreenAspectRatio;
    RsGlobal.ps->lastMousePos_Y = RsGlobal.MaximumHeight / 2;

    FrontEndMenuManager.m_vMousePos.x = (RsGlobal.MaximumWidth / 2) * 1.14f / ScreenAspectRatio;
    FrontEndMenuManager.m_vMousePos.y = (RsGlobal.MaximumHeight / 2);
}

void CMenuManager::AdditionalOptionInputVCS(unsigned char* pUp, unsigned char* pDown)
{
    // Call SA AdditionalOptionInput
	if (!CPad::GetPad(0)->NewMouseControllerState.lmb)
		((void(__thiscall*)(CMenuManager*, unsigned char*, unsigned char*))0x5773D0)(this, pUp, pDown);

    FrontEndMenuManager.field_30 = -1;

    if (m_bCurrentMenuPage == 45 || m_bCurrentMenuPage == 46)
    {
        if (ControlsManager.GetIsKeyboardKeyJustDown(rsEND))
        {
            AudioEngine.ReportFrontendAudioEvent(1, 0.0f, 1.0f);
            SwitchToNewScreen(49);
        }
    }

    if (m_bCurrentMenuPage == 49)
    {
        // Listen for Ctrl + V
        if ((ControlsManager.GetIsKeyboardKeyDown(rsLCTRL) || ControlsManager.GetIsKeyboardKeyDown(rsRCTRL))
            && ControlsManager.GetIsKeyboardKeyJustDown(static_cast<RsKeyCodes>(0x56)))
        {
            ClearSerialsBuffer();
            LookIntoClipboardForSerial();
        }
    }

	// Map Menu
	float fMapCenterX = 320.0f;
	float fMapCenterY = 170.0f;
	float fMapZoom = 310.0f;
	float fMapMaxZoom = 648.0f;
	float fSlideSpeed = 10.0f;
	float MOUSEX = FrontEndMenuManager.m_vMousePos.x;
	float MOUSEY = FrontEndMenuManager.m_vMousePos.y;

	// Set Zoom / Movement... may a new set needed.
	if (FrontEndMenuManager.m_bCurrentMenuPage == 5) {
		if (!bCenteredCursor) {
			CenterMousePosnForMapScreen();
			bCenteredCursor = true;
		}
		//FrontEndMenuManager.m_bDrawMouse = false;

		if (FrontEndMenuManager.m_fMapZoom > fMapZoom + 20.0f) {}
		else {
			FrontEndMenuManager.m_fMapBaseX = (fMapCenterX);
			FrontEndMenuManager.m_fMapBaseY = (fMapCenterY);
		}

		// Legend key.
	   /* if (CPad::GetPad(0)->NewState.CROSS && !CPad::GetPad(0)->OldState.CROSS || (currKeyState->standardKeys[76] && !prevKeyState->standardKeys[76]))
			FrontEndMenuManager.m_bMapLegend = FrontEndMenuManager.m_bMapLegend == 0;*/

		if (FrontEndMenuManager.m_dwMousePosTop < _ydown(23.0f)) {

			/* // Zoom
			 if (CPad::GetPad(0)->NewMouseControllerState.wheelUp || currKeyState->pgup || CPad::GetPad(0)->NewState.RIGHTSHOULDER2) {
				 if (FrontEndMenuManager.m_fMapZoom < fMapMaxZoom)
					 FrontEndMenuManager.m_fMapZoom += fSlideSpeed;
				 else if (FrontEndMenuManager.m_fMapZoom >= fMapMaxZoom)
					 FrontEndMenuManager.m_fMapZoom = fMapMaxZoom;

			 }
			 else if (CPad::GetPad(0)->NewMouseControllerState.wheelDown || currKeyState->pgdn || CPad::GetPad(0)->NewState.LEFTSHOULDER2) {
				 if (FrontEndMenuManager.m_fMapZoom > fMapZoom) {
					 FrontEndMenuManager.m_fMapZoom -= fSlideSpeed;
				 }
				 else if (FrontEndMenuManager.m_fMapZoom <= fMapZoom) {
					 FrontEndMenuManager.m_fMapZoom = fMapZoom;
				 }

				 // Restore posn.
				 if (FrontEndMenuManager.m_fMapBaseX != fMapCenterX) {
					 if (FrontEndMenuManager.m_fMapBaseX > fMapCenterX)
						 FrontEndMenuManager.m_fMapBaseX -= fSlideSpeed;
					 else if (FrontEndMenuManager.m_fMapBaseX < fMapCenterX)
						 FrontEndMenuManager.m_fMapBaseX += fSlideSpeed;
				 }

				 if (FrontEndMenuManager.m_fMapBaseY != fMapCenterY) {
					 if (FrontEndMenuManager.m_fMapBaseY > fMapCenterY)
						 FrontEndMenuManager.m_fMapBaseY -= fSlideSpeed;
					 else if (FrontEndMenuManager.m_fMapBaseY < fMapCenterY)
						 FrontEndMenuManager.m_fMapBaseY += fSlideSpeed;
				 }
			 }*/

			 // Position mouse.
			if (CPad::GetPad(0)->NewState.LEFTSTICKX || CPad::GetPad(0)->NewState.LEFTSTICKY || CPad::GetPad(0)->NewState.DPADRIGHT || CPad::GetPad(0)->NewState.DPADLEFT || CPad::GetPad(0)->NewState.DPADUP || CPad::GetPad(0)->NewState.DPADDOWN || currKeyState->left || currKeyState->right || currKeyState->up || currKeyState->down) {
				CenterMousePosnForMapScreen();
			}

			if (CPad::GetPad(0)->NewMouseControllerState.lmb) {
				if (FrontEndMenuManager.m_fMapZoom > fMapZoom + 20.0f) {
					if (CPad::GetPad(0)->NewMouseControllerState.X > 0) {
						if (FrontEndMenuManager.m_fMapBaseX < 640.0f) // Right
							FrontEndMenuManager.m_fMapBaseX += CPad::GetPad(0)->NewMouseControllerState.X * 0.4f;
					}
					else if (CPad::GetPad(0)->NewMouseControllerState.X < 0) {
						if (FrontEndMenuManager.m_fMapBaseX > 0.0f) // Left
							FrontEndMenuManager.m_fMapBaseX += CPad::GetPad(0)->NewMouseControllerState.X * 0.4f;
					}

					if (CPad::GetPad(0)->NewMouseControllerState.Y > 0) {
						if (FrontEndMenuManager.m_fMapBaseY < 640.0f) // Bottom
							FrontEndMenuManager.m_fMapBaseY += CPad::GetPad(0)->NewMouseControllerState.Y * 0.4f;
					}
					else if (CPad::GetPad(0)->NewMouseControllerState.Y < 0) {
						if (FrontEndMenuManager.m_fMapBaseY > -128.0f) // Top
							FrontEndMenuManager.m_fMapBaseY += CPad::GetPad(0)->NewMouseControllerState.Y * 0.4f;
					}
				}
			}
			else { // Position arrows
				if (FrontEndMenuManager.m_fMapZoom > fMapZoom + 20.0f) {
					// ANALOG
					if (CPad::GetPad(0)->NewState.LEFTSTICKX > 0) {
						if (FrontEndMenuManager.m_fMapBaseX > 0.0f) // Left
							FrontEndMenuManager.m_fMapBaseX -= CPad::GetPad(0)->NewState.LEFTSTICKX * 0.06f;
					}
					else if (CPad::GetPad(0)->NewState.LEFTSTICKX < 0) {
						if (FrontEndMenuManager.m_fMapBaseX < 640.0f) // Right
							FrontEndMenuManager.m_fMapBaseX -= CPad::GetPad(0)->NewState.LEFTSTICKX * 0.06f;
					}
					if (CPad::GetPad(0)->NewState.LEFTSTICKY > 0) {
						if (FrontEndMenuManager.m_fMapBaseY > -128.0f) // Top
							FrontEndMenuManager.m_fMapBaseY -= CPad::GetPad(0)->NewState.LEFTSTICKY * 0.06f;
					}
					else if (CPad::GetPad(0)->NewState.LEFTSTICKY < 0) {
						if (FrontEndMenuManager.m_fMapBaseY < 640.0f) // Bottom
							FrontEndMenuManager.m_fMapBaseY -= CPad::GetPad(0)->NewState.LEFTSTICKY * 0.06f;
					}
					// DPAD
					if (CPad::GetPad(0)->NewState.DPADRIGHT) {
						if (FrontEndMenuManager.m_fMapBaseX > 0.0f) // Left
							FrontEndMenuManager.m_fMapBaseX -= CPad::GetPad(0)->NewState.DPADRIGHT * 0.03f;
					}
					else if (CPad::GetPad(0)->NewState.DPADLEFT) {
						if (FrontEndMenuManager.m_fMapBaseX < 640.0f) // Right
							FrontEndMenuManager.m_fMapBaseX += CPad::GetPad(0)->NewState.DPADLEFT * 0.03f;
					}
					if (CPad::GetPad(0)->NewState.DPADDOWN) {
						if (FrontEndMenuManager.m_fMapBaseY > -128.0f) // Top
							FrontEndMenuManager.m_fMapBaseY -= CPad::GetPad(0)->NewState.DPADDOWN * 0.03f;
					}
					else if (CPad::GetPad(0)->NewState.DPADUP) {
						if (FrontEndMenuManager.m_fMapBaseY < 640.0f) // Bottom
							FrontEndMenuManager.m_fMapBaseY += CPad::GetPad(0)->NewState.DPADUP * 0.03f;
					}
					// WASD
					if (currKeyState->right) {
						if (FrontEndMenuManager.m_fMapBaseX > 0.0f) // Left
							FrontEndMenuManager.m_fMapBaseX -= currKeyState->right * 0.03f;
					}
					else if (currKeyState->left) {
						if (FrontEndMenuManager.m_fMapBaseX < 640.0f) // Right
							FrontEndMenuManager.m_fMapBaseX += currKeyState->left * 0.03f;
					}
					if (currKeyState->down) {
						if (FrontEndMenuManager.m_fMapBaseY > -128.0f) // Top
							FrontEndMenuManager.m_fMapBaseY -= currKeyState->down * 0.03f;
					}
					else if (currKeyState->up) {
						if (FrontEndMenuManager.m_fMapBaseY < 640.0f) // Bottom
							FrontEndMenuManager.m_fMapBaseY += currKeyState->up * 0.03f;
					}
				}
				else {
					FrontEndMenuManager.m_fMapBaseX = (fMapCenterX);
					FrontEndMenuManager.m_fMapBaseY = (fMapCenterY);
				}
			}
		}
	}
	else {
		bCenteredCursor = false;
		// FrontEndMenuManager.m_bDrawMouse = FrontEndMenuManager.m_bDrawMouse;

		FrontEndMenuManager.m_fMapZoom = (fMapZoom);
		FrontEndMenuManager.m_fMapBaseX = (fMapCenterX);
		FrontEndMenuManager.m_fMapBaseY = (fMapCenterY);
	}

	// Limits
	if (FrontEndMenuManager.m_fMapZoom < fMapZoom)
		FrontEndMenuManager.m_fMapZoom = fMapZoom;
}


void CMenuManager::TypingKeyboardInput(wchar_t wKey)
{
	// DLC activation screen input
	if ( m_bCurrentMenuPage == 49 )
	{
		// Add the char if it's valid
		if ( ValidSerialCharacter(wKey) )
		{
			int i = 0;
			while ( m_strSerialCode[i].length() >= SERIAL_CODES_ONE_WINDOW )
			{
				if ( ++i == SERIAL_CODES_LENGTH )
					return;		// Serial's full
			}
			m_strSerialCode[i] += static_cast<char>(wKey);

			// Filled an entire field?
			if ( m_strSerialCode[i].length() >= SERIAL_CODES_ONE_WINDOW )
			{
				// Uppercase it
				for ( auto it = m_strSerialCode[i].begin(); it != m_strSerialCode[i].end(); it++ )
					*it = static_cast<char>(toupper(*it));

				if ( i == SERIAL_CODES_LENGTH-1 )
					m_bSerialFull = true;
			}
		}
		else if ( wKey == '\x8' ) // Backspace
		{
			int i = SERIAL_CODES_LENGTH;

			m_bSerialFull = false;
			while ( m_strSerialCode[--i].empty() )
			{
				if ( !i )
					return;		// Serial's empty
			}

			m_strSerialCode[i].pop_back();
		}
	}
}

void CMenuManager::SetDefaultPreferences(signed char bScreen)
{
	switch ( bScreen )
	{
	case 3:
		// Audio Settings
		m_nSfxVolume = 64;
		m_nRadioVolume = 64;
		m_bRadioEq = false;
		m_bRadioAutoSelect = true;
		m_bTracksAutoScan = false;
		m_nRadioMode = 0;

		AudioEngine.SetMusicMasterVolume(m_nRadioVolume);
		AudioEngine.SetEffectsMasterVolume(m_nSfxVolume);
		AudioEngine.SetRadioAutoRetuneOnOff(m_bRadioAutoSelect);
		return;

	case 4:
		// Display Settings
		// m_dwBrightness = 256;
		m_dwBrightness = 384;
		// Fuck everything
		//((void(__thiscall*)(int,float,bool))0x747200)(0xC92134, 96.0f/512.0f, true);

		m_fSafeZone = 8.0f;
		m_bHudOn = true;
		m_bSavePhotos = false;
		m_bMapLegend = true;
		m_dwRadarMode = 0;
		m_bShowSubtitles = false;
		m_bMapShowLocations = true;
		m_bMapShowContacts = true;
		m_bMapShowMission = true;
		m_bMapShowOther = true;
		m_bMapShowGangArea = true;
		m_bEnableSkyMenu = false;
		return;

	case 27:
		// Graphics Setup
		m_bFrameLimiterMode = 2;
		m_bAspectRatioMode = 0;
		//m_bMipMapping = true;
		m_dwAppliedAntiAliasingLevel = m_dwAntiAliasingLevel = 1;
		m_dwResolution = m_dwAppliedResolution;
		m_bVSync = m_bAppliedVSync = true;
		ms_lodDistScale = m_fDrawDistance = 1.328125;
		//g_fx.SetFxQuality(FXQUALITY_HIGH);
		Fx_c::SetEffectsQuality(FXQUALITY_HIGH);
		Fx_c::SetTextureFilteringQuality(3);	// Anisotropic 4x
		CShadows::SetShadowQuality(SHADOW_QUALITY_HIGHEST);
		CShadows::SetShadowDistance(8.0f/16.0f);
		CShadows::InitialiseChangedSettings();
		CPostEffects::SetTrailsState(true);
		CarPipe::PipeSwitch = PIPE_VCS;
		CarPipe::envMapSize = REFLECTION_QUALITY_MEDIUM;

		// Reinit widescreen and framelimit stuff
		WidescreenSupport::Recalculate(RsGlobal.MaximumWidth, RsGlobal.MaximumHeight, true);
		RsGlobal.frameLimit = RsGlobalFrameLimits[m_bFrameLimiterMode];
		return;

	case 36:
		// Controls Setup
		//m_nController = 0;
		MousePointerStateHelper.m_bHorizontalInvert = true;
		CVehicle::m_bEnableMouseSteering = false;
		CVehicle::m_bEnableMouseFlying = true;
		CCamera::m_bUseMouse3rdPerson = true;
		CCamera::m_fMouseAccelHorzntl = 0.0025f;
		CPad::SavedMode = 0;
		m_bVibrationEnabled = true;
		CPad::bInvertLook4Pad = false;
		CPad::bSouthpaw = false;
		CFont::bX360Buttons = false;

		invertPadX1 = false;
		invertPadY1 = false;
		invertPadX2 = false;
		invertPadY2 = false;
		swapPadAxis1 = false;
		swapPadAxis2 = false;
		return;
	}

}

const char* CMenuManager::ProcessDLCSlot(int nSlotID)
{
	int				nDLC = CDLCManager::GetActiveDLCWithIndex(nSlotID-2);
	static char		lastDLCName[8];

	if ( nDLC == -1 )
	{
		// No DLC on this slot, skip
		FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[nSlotID].action = 20;
		FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[nSlotID].posY = -32728;
		/*if ( nSlotID == 2 )
		{
			static bool		bChangedHelperText = false;
			if ( !bChangedHelperText )
			{
				bChangedHelperText = true;
				strncpy(FrontEndMenuManager._FrontEndMenuManager._aScreens[bCurrentScreen].entryList[0].entry, "FEE_NON", 8);
			}
		}*/
		return nullptr;
	}

	FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[nSlotID].action = MENUACTION_TOGGLE_DLC;
	_snprintf(lastDLCName, sizeof(lastDLCName), "FEE_N%02d", nDLC);
	return TheText.Get(lastDLCName);
}

float CMenuManager::GetTextYPos(short nSlotID)
{
	if ( FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[nSlotID].screenVertAlign == -1 )
		return _y(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[nSlotID].posY);

	if ( FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[nSlotID].screenVertAlign == 1 )
		return _ydown(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[nSlotID].posY);

	return _ymiddle(FrontEndMenuManager._aScreens[m_bCurrentMenuPage].entryList[nSlotID].posY);
}

float CMenuManager::_GetTextYPos(const MenuItem::MenuEntry& pPosition)
{
	if (pPosition.screenVertAlign == -1)
		return _y(pPosition.posY) + m_fScrollerOffset;

	if (pPosition.screenVertAlign == 1)
		return _ydown(pPosition.posY) + m_fScrollerOffset;

	return _ymiddle(pPosition.posY) + m_fScrollerOffset;
}

float CMenuManager::GetTextYPosNextItem(const MenuItem::MenuEntry& pPosition)
{
	if (pPosition.screenVertAlign == -1)
		return _y(pPosition.posY + 26) + m_fScrollerOffset;

	if (pPosition.screenVertAlign == 1)
		return _ydown(pPosition.posY - 26) + m_fScrollerOffset;

	return _ymiddle(pPosition.posY + 26) + m_fScrollerOffset;
}

bool CMenuManager::NeedsToRefreshHelps()
{
	bool	bLangChanged = false, bPadModeChanged = false;

	if ( m_nLanguage != m_nPrevLanguage )
	{
		m_nPrevLanguage = m_nLanguage;
		bLangChanged = true;
	}

	static bool		bPadWasInHands = false;
	bool			bDisplayPadNow = pXboxPad[0]->HasPadInHands();
	if ( bDisplayPadNow != bPadWasInHands )
	{
		bPadWasInHands = bDisplayPadNow;
		bPadModeChanged = true;
	}
	return bPadModeChanged || bLangChanged;
}

static void __stdcall DisplayHelperText_Wrap1(const char*)
{
	FrontEndMenuManager.DisplayHelperText(pXboxPad[0]->HasPadInHands() ? "FET_CI2" : "FET_CIG");
}

void CMenuManager::PrintMap(int x, int y, CRect rect) {
	CRect coords;
	float v113 = FrontEndMenuManager.m_fMapZoom * 0.16666667;
	coords.x1 = ((RsGlobal.MaximumWidth * (FrontEndMenuManager.m_fMapBaseX - (FrontEndMenuManager.m_fMapZoom * 0.68f)) * 0.0015625)) * 1.33334 / ScreenAspectRatio + _xmiddle(-274.0f);
	coords.y1 = ((RsGlobal.MaximumHeight * (FrontEndMenuManager.m_fMapBaseY + (FrontEndMenuManager.m_fMapZoom* 0.68f)) * 0.002232143));
	coords.x2 = ((RsGlobal.MaximumWidth * (FrontEndMenuManager.m_fMapBaseX + (FrontEndMenuManager.m_fMapZoom* 0.68f)) * 0.0015625)) * 1.33334 / ScreenAspectRatio + _xmiddle(-274.0f);
	coords.y2 = ((RsGlobal.MaximumHeight * (FrontEndMenuManager.m_fMapBaseY - (FrontEndMenuManager.m_fMapZoom* 0.68f)) * 0.002232143));

	FrontEndMenuManager.m_apBackgroundTextures[2].Draw(CRect(coords), CRGBA(255, 255, 255, 255));
}

void CMenuManager::PrintMapExtra() {
    float MOUSEX = FrontEndMenuManager.m_dwMousePosLeft;
    float MOUSEY = FrontEndMenuManager.m_dwMousePosTop;
    static bool bShowCursor;
    if (CPad::GetPad(0)->NewMouseControllerState.X || CPad::GetPad(0)->NewMouseControllerState.Y)
        bShowCursor = false;
	else if (CPad::GetPad(0)->NewState.LEFTSTICKX || CPad::GetPad(0)->NewState.LEFTSTICKY || CPad::GetPad(0)->NewState.DPADRIGHT || CPad::GetPad(0)->NewState.DPADLEFT || CPad::GetPad(0)->NewState.DPADUP || CPad::GetPad(0)->NewState.DPADDOWN || currKeyState->left || currKeyState->right || currKeyState->up || currKeyState->down)
        bShowCursor = true;

    // Cursor
	if (FrontEndMenuManager.m_bEnableSkyMenu ? MOUSEY > RsGlobal.MaximumHeight * 0.83f : MOUSEY > _ydown(23.0f)) {}
    else {
        if (!FrontEndMenuManager.m_bEnableSkyMenu && bShowCursor || FrontEndMenuManager.m_bEnableSkyMenu && !FrontEndMenuManager.m_bBottomMenu) {
            CRect rect;
            rect.x1 = NULL;
            rect.x2 = RsGlobal.MaximumWidth;
            rect.y1 = MOUSEY - _height(0.0f);
            rect.y2 = rect.y1 + _height(3.0f);
            CSprite2d::DrawRect(CRect(rect), FrontEndMenuManager.m_bEnableSkyMenu ? CRGBA(MENU_PINK_R, MENU_PINK_G, MENU_PINK_B, 200) : CRGBA(170, 120, 20, 200)); // Hor
            rect.x1 = MOUSEX + _xmiddle(-274.0f) + _height(3.5f);
            rect.x2 = rect.x1 - _height(3.5f);
            rect.y1 = NULL;
            rect.y2 = _ydown(23.0f);
			CSprite2d::DrawRect(CRect(rect.x1, rect.y1, rect.x2, rect.y2), FrontEndMenuManager.m_bEnableSkyMenu ? CRGBA(MENU_PINK_R, MENU_PINK_G, MENU_PINK_B, 200) : CRGBA(170, 120, 20, 200)); // Vert
        }
    }

	if (FrontEndMenuManager.m_bEnableSkyMenu)
		// Bottom Bar
		CSprite2d::DrawRect(CRect(-1.0f, RsGlobal.MaximumHeight + 1.0f, RsGlobal.MaximumWidth + 1.0f, RsGlobal.MaximumHeight * 0.83f - 1.0f), CRGBA(7, 7, 7, 255));
	else
		CSprite2d::DrawRect(CRect(-1.0f, RsGlobal.MaximumHeight + 1.0f, RsGlobal.MaximumWidth + 1.0f, RsGlobal.MaximumHeight * 0.95f - 1.0f), CRGBA(7, 7, 7, 255));
}

void RotateVertices(CVector2D *rect, unsigned int numVerts, float x, float y, float angle) {
    float xold, yold;
    float _cos = cosf(angle);
    float _sin = sinf(angle);
    for (unsigned int i = 0; i < numVerts; i++) {
        xold = rect[i].x;
        yold = rect[i].y;
        rect[i].x = x + (xold - x) * _cos + (yold - y) * _sin;
        rect[i].y = y - (xold - x) * _sin + (yold - y) * _cos;
    }
}

void CMenuManager::DrawSkyLegend() {
	CVector2D vecScale = WidescreenSupport::GetFullscreenImageDimensions(16.0f / 9.0f, CDraw::ms_fAspectRatio, true);
	const char *str;
	float fX = 349.5f * ((CDraw::ms_fAspectRatio < 16.0f / 10.0f) ? (1.0f / CDraw::ms_fAspectRatio) : 1.0f);
	float fY = 26.5f;

	if (CRadar::MapLegendCounter) {
		CFont::SetBackground(0, 0);
		CFont::SetProportional(1);
		CFont::SetWrapx(RsGlobal.MaximumWidth - 12.5f);
		CFont::SetEdge(0);
		CFont::SetDropShadowPosition(1);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetOrientation(ALIGN_Left);
		CFont::SetFontStyle(FONT_Eurostile);
		CFont::SetScale(_width(0.58f), _height(1.10f));
		CFont::SetColor(CRGBA(MENU_ACTIVE_R, MENU_ACTIVE_G, MENU_ACTIVE_B, 255));

		float fSpacing[11 * 2] = { 
			0, 0,
			1, 1, 
			2, 2, 
			3, 3, 
			4, 4, 
			5, 5, 
			6, 6, 
			7, 7, 
			8, 8, 
			9, 9,
			10, 10 };

		CVector2D m_nvecPositions[11 * 2] = {
			CVector2D(fX * 0, fY * 0), CVector2D(fX * 1, fY * 0),
			CVector2D(fX * 0, fY * 1), CVector2D(fX * 1, fY * 1),
			CVector2D(fX * 0, fY * 2), CVector2D(fX * 1, fY * 2),
			CVector2D(fX * 0, fY * 3), CVector2D(fX * 1, fY * 3),
			CVector2D(fX * 0, fY * 4), CVector2D(fX * 1, fY * 4),
			CVector2D(fX * 0, fY * 5), CVector2D(fX * 1, fY * 5),
			CVector2D(fX * 0, fY * 6), CVector2D(fX * 1, fY * 6),
			CVector2D(fX * 0, fY * 7), CVector2D(fX * 1, fY * 7),
			CVector2D(fX * 0, fY * 8), CVector2D(fX * 1, fY * 8),
			CVector2D(fX * 0, fY * 9), CVector2D(fX * 1, fY * 9),
			CVector2D(fX * 0, fY * 10), CVector2D(fX * 1, fY * 10),
		};

		// Background
		CSprite2d::DrawRect(CRect(_xleft(20.5f) + (0.5f * (RsGlobal.MaximumWidth - vecScale.x)), _ymiddle(-90.0f) + _height(30.0f * fSpacing[static_cast<BYTE>(CRadar::MapLegendCounter) - 1]), _x(20.5f) - (0.5f * (RsGlobal.MaximumWidth - vecScale.x)), _ymiddle(-150.0f)), CRGBA(0, 0, 0, 205));

		for (int i = 0; i < CRadar::MapLegendCounter; i++) {
			switch (CRadar::MapLegendList[i]) {
			case -5:
				str = TheText.Get("LG_56");
				break;
			case -4:
				str = TheText.Get("LG_55");
				break;
			case -3:
				str = TheText.Get("LG_54");
				break;
			case -2:
				str = TheText.Get("LG_50");
				break;
			case -1:
				str = TheText.Get("LG_49");
				break;
			case 3:
				str = TheText.Get("LG_01");
				break;
			case 5:
				str = TheText.Get("LG_02");
				break;
			case 6:
				str = TheText.Get("LG_03");
				break;
			case 7:
				str = TheText.Get("LG_04");
				break;
			case 8:
				str = TheText.Get("LG_05");
				break;
			case 9:
				str = TheText.Get("LG_06");
				break;
			case 0xA:
				str = TheText.Get("LG_07");
				break;
			case 0xB:
				str = TheText.Get("LG_66");
				break;
			case 0xC:
				str = TheText.Get("LG_09");
				break;
			case 0xD:
				str = TheText.Get("LG_10");
				break;
			case 0xE:
				str = TheText.Get("LG_11");
				break;
			case 0xF:
				str = TheText.Get("LG_12");
				break;
			case 0x10:
				str = TheText.Get("LG_13");
				break;
			case 0x11:
				str = TheText.Get("LG_67");
				break;
			case 0x12:
				str = TheText.Get("LG_15");
				break;
			case 0x13:
				str = TheText.Get("LG_16");
				break;
			case 0x14:
				str = TheText.Get("LG_17");
				break;
			case 0x15:
				str = TheText.Get("LG_18");
				break;
			case 0x16:
				str = TheText.Get("LG_19");
				break;
			case 0x17:
				str = TheText.Get("LG_20");
				break;
			case 0x18:
				str = TheText.Get("LG_21");
				break;
			case 0x19:
				str = TheText.Get("LG_22");
				break;
			case 0x1A:
				str = TheText.Get("LG_23");
				break;
			case 0x1B:
				str = TheText.Get("LG_24");
				break;
			case 0x1C:
				str = TheText.Get("LG_25");
				break;
			case 0x1D:
				str = TheText.Get("LG_26");
				break;
			case 0x1E:
				str = TheText.Get("LG_27");
				break;
			case 0x1F:
				str = TheText.Get("LG_28");
				break;
			case 0x20:
				str = TheText.Get("LG_29");
				break;
			case 0x21:
				str = TheText.Get("LG_30");
				break;
			case 0x22:
				str = TheText.Get("LG_31");
				break;
			case 0x23:
				str = TheText.Get("LG_32");
				break;
			case 0x24:
				str = TheText.Get("LG_33");
				break;
			case 0x25:
				str = TheText.Get("LG_63");
				break;
			case 0x26:
				str = TheText.Get("LG_35");
				break;
			case 0x27:
				str = TheText.Get("LG_36");
				break;
			case 0x28:
				str = TheText.Get("LG_37");
				break;
			case 0x29:
				str = TheText.Get("LG_64");
				break;
			case 0x2A:
				str = TheText.Get("LG_39");
				break;
			case 0x2B:
				str = TheText.Get("LG_40");
				break;
			case 0x2C:
				str = TheText.Get("LG_41");
				break;
			case 0x2D:
				str = TheText.Get("LG_42");
				break;
			case 0x2E:
				str = TheText.Get("LG_43");
				break;
			case 0x2F:
				str = TheText.Get("LG_44");
				break;
			case 0x30:
				str = TheText.Get("LG_45");
				break;
			case 0x31:
				str = TheText.Get("LG_46");
				break;
			case 0x32:
				str = TheText.Get("LG_47");
				break;
			case 0x33:
				str = TheText.Get("LG_48");
				break;
			case 0x34:
				str = TheText.Get("LG_51");
				break;
			case 0x35:
				str = TheText.Get("LG_52");
				break;
			case 0x36:
				str = TheText.Get("LG_53");
				break;
			case 0x37:
				str = TheText.Get("LG_57");
				break;
			case 0x39:
				str = TheText.Get("LG_65");
				break;
			case 0x3A:
				str = TheText.Get("LG_58");
				break;
			case 0x3B:
				str = TheText.Get("LG_59");
				break;
			case 0x3C:
				str = TheText.Get("LG_60");
				break;
			case 0x3D:
				str = TheText.Get("LG_61");
				break;
			case 0x3E:
				str = TheText.Get("LG_62");
				break;
			case 0x3F:
				str = TheText.Get("LG_34");
				break;
			default:
				return;
			}

			CFont::PrintString(_xleft(105.0f + m_nvecPositions[i].x) + (0.5f * (RsGlobal.MaximumWidth - vecScale.x)), _ymiddle(-128.5f + m_nvecPositions[i].y), str);
			CRadar::RadarBlipSprites[CRadar::MapLegendList[i]].Draw(CRect(_xleft(78.0f + m_nvecPositions[i].x) + (0.5f * (RsGlobal.MaximumWidth - vecScale.x)), _ymiddle(-128.5f + m_nvecPositions[i].y) + _height(18.5f), _xleft(78.0f + m_nvecPositions[i].x) + (0.5f * (RsGlobal.MaximumWidth - vecScale.x)) + _width(19.5f), _ymiddle(-128.5f + m_nvecPositions[i].y)), CRGBA(255, 255, 255, 255));
		}
	}
}

void CMenuManager::DrawYouAreHereSprite(CSprite2d* sprite, float x, float y, float angle, unsigned int width, unsigned int height, CRGBA color) {
    CRadar::DrawRotatingRadarSprite(sprite, x, y, angle, width, height, color);
}

void __stdcall CMenuManager::DrawLegendWindow(CRect *coords, char *titleKey, char fadeState, CRGBA color, int a5, char bDrawBox) {
	if (!FrontEndMenuManager.m_bEnableSkyMenu)
		CSprite2d::DrawRect(CRect(coords->x1 + _xmiddle(-320.0f), coords->y1, coords->x2 + _xmiddle(-320.0f), coords->y2), CRGBA(30, 30, 30, 245));
}

void __cdecl CMenuManager::DrawLegend2DPolygon(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, CRGBA *color) {
	if (!FrontEndMenuManager.m_bEnableSkyMenu)
		CSprite2d::Draw2DPolygon(x1 + _xmiddle(-320.0f), y1, x2 + _xmiddle(-320.0f), y2, x3 + _xmiddle(-320.0f), y3, x4 + _xmiddle(-320.0f), y4, *color);
}

void __cdecl CMenuManager::DrawLegendText(float x, float y, char *str) {
	CFont::SetColor(CRGBA(MENU_INACTIVE_R, MENU_INACTIVE_G, MENU_INACTIVE_B, 255));

	if (!FrontEndMenuManager.m_bEnableSkyMenu)
		CFont::PrintString(x + _xmiddle(-320.0f), y, str);
}

void CMenuManager::DrawLegend2DRect(CRect const& rect, CRGBA const& rgbaColor) {
	if (!FrontEndMenuManager.m_bEnableSkyMenu)
		CSprite2d::DrawRect(CRect(rect.x1 + _xmiddle(-320.0f), rect.y1, rect.x2 + _xmiddle(-320.0f), rect.y2), rgbaColor);
}

void __fastcall CMenuManager::DrawLegend2DSprite(CSprite2d *sprite, int, CRect *rect, CRGBA *color) {
	if (!FrontEndMenuManager.m_bEnableSkyMenu)
		sprite->Draw(CRect(rect->x1 + _xmiddle(-320.0f), rect->y1, rect->x2 + _xmiddle(-320.0f), rect->y2), *color);
}

void CMenuManager::PrintMapZones(float x, float y, char *text) {
	if (FrontEndMenuManager.m_bEnableSkyMenu) {
		CFont::SetScaleLang(_width(0.90f), _height(1.45f));
		CFont::PrintStringFromBottom(_xleft(18.0f), _ydown(86.0f), text);
	}
	else
		CFont::PrintString(_xleft(40.0f), _ydown(20.0f), text);
}

void DrawMouse() {
	CFont::RenderFontBuffer();

	RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERNAFILTERMODE);

	if (FrontEndMenuManager.m_bEnableSkyMenu && FrontEndMenuManager.m_bCurrentMenuPage == 5) {
		if (FrontEndMenuManager.m_bBottomMenu)
			FrontEndMenuManager.m_apMouseTextures[2].Draw(CRect(FrontEndMenuManager.m_dwMousePosLeft, FrontEndMenuManager.m_dwMousePosTop + _height(24.0f), FrontEndMenuManager.m_dwMousePosLeft + _width(26.0f), FrontEndMenuManager.m_dwMousePosTop), CRGBA(255, 255, 255, 255), 0.054f, 0.10f, 0.30f, 0.10f, 0.054f, 1.00f, 0.30f, 1.00f);
	}
	else {
		if (FrontEndMenuManager.m_bCurrentMenuPage == 5) {
			if (CPad::GetPad(0)->NewMouseControllerState.lmb)
				FrontEndMenuManager.m_apMouseTextures[2].Draw(CRect(FrontEndMenuManager.m_dwMousePosLeft + _xmiddle(-274.0f), FrontEndMenuManager.m_dwMousePosTop + _height(24.0f), FrontEndMenuManager.m_dwMousePosLeft + _xmiddle(-274.0f) + _width(26.0f), FrontEndMenuManager.m_dwMousePosTop), CRGBA(255, 255, 255, 255), 0.69f, 0.01f, 1.00f, 0.01f, 0.69f, 1.00f, 1.00f, 1.00f);
			else
				FrontEndMenuManager.m_apMouseTextures[2].Draw(CRect(FrontEndMenuManager.m_dwMousePosLeft + _xmiddle(-274.0f), FrontEndMenuManager.m_dwMousePosTop + _height(24.0f), FrontEndMenuManager.m_dwMousePosLeft + _xmiddle(-274.0f) + _width(26.0f), FrontEndMenuManager.m_dwMousePosTop), CRGBA(255, 255, 255, 255), 0.35f, 0.01f, 0.65f, 0.01f, 0.35f, 1.00f, 0.65f, 1.00f);
		}
		else
			FrontEndMenuManager.m_apMouseTextures[2].Draw(CRect(FrontEndMenuManager.m_dwMousePosLeft, FrontEndMenuManager.m_dwMousePosTop + _height(24.0f), FrontEndMenuManager.m_dwMousePosLeft + _width(26.0f), FrontEndMenuManager.m_dwMousePosTop), CRGBA(255, 255, 255, 255), 0.054f, 0.10f, 0.30f, 0.10f, 0.054f, 1.00f, 0.30f, 1.00f);
	}
}

void __fastcall CheckForMenuClosing(int _this, int) {
	((char(__thiscall*)(int))0x576B70)(_this);

	if (!FrontEndMenuManager.m_bMenuActive) {
		FrontEndMenuManager.m_bIsMenuSwitched = false;
		FrontEndMenuManager.m_nSelectedSkyMenuItem = 0;
		FrontEndMenuManager.m_bBottomMenu = true;
	}

	// May find a better check!?!
	if (FindPlayerPed(-1)) {
		FrontEndMenuManager.m_bIsPauseMenu = true;
	}
}

void __cdecl SetCoordBlipTest(eBlipType type, float x, float y, float z, int a5, eBlipDisplay display) {
	CRadar::SetCoordBlip(BLIP_COORD, x, y, 0.0, 0, (eBlipDisplay)2);
}

void CMenuManager::Inject()
{	
	//Memory::InjectHook(0x5775D2, &SetCoordBlipTest, PATCH_CALL);

    Memory::InjectHook(0x57BA58, &DrawStandardMenus);
    Memory::InjectHook(0x57B66F, &ProcessMenuOptions);
    Memory::InjectHook(0x57B702, &ProcessMenuOptions);

    Memory::InjectHook(0x57B70A, &CheckSliderMovement);
    Memory::InjectHook(0x580215, &CheckSliderMovement);
    Memory::InjectHook(0x57E240, &DisplayHelperText, PATCH_JUMP);

    // Hook helpers
    Memory::InjectHook(0x57ED7F, DisplayHelperText_Wrap1);
    Memory::InjectHook(0x57ED94, DisplayHelperText_Wrap1);

    // Mouse Pointers
    Memory::InjectHook(0x57BF70, DrawMouse);
    Memory::Patch<BYTE>(0x57C099 + 1, 0);
    Memory::Patch<BYTE>(0x57C0FF + 1, 0);
    Memory::Patch<BYTE>(0x57C18F + 1, 0);
    Memory::Patch<BYTE>(0x57C1F5 + 1, 0);

	// SkyMenu
	Memory::InjectHook(0x57B477, CheckForMenuClosing, PATCH_JUMP);
}

//
// CLoadingScreen
//
uint8	CLoadingScreen::bDrawingStyle;
bool	&CLoadingScreen::m_bFading = *(bool*)0xBAB31C;
bool	&CLoadingScreen::m_bFadeInNextSplashFromBlack = *(bool*)0xBAB31E;
bool	&CLoadingScreen::m_bFadeOutCurrSplashToBlack = *(bool*)0xBAB31F;
bool	&CLoadingScreen::m_bReadyToDelete = *(bool*)0xBAB33D;

void CLoadingScreen::Init() {
	if (FILE* hFile = CFileMgr::OpenFile("DATA\\SPLASH.DAT", "r")) {
		enum {
			SPLASH_NOTHING,
			SPLASH_INTRO,
			SPLASH_LEGAL,
			SPLASH_LOADSCS,
			SPLASH_OUTRO
		} curFileSection = SPLASH_NOTHING;

		while (const char* pLine = CFileLoader::LoadLine(hFile)) {
			if (pLine[0] && pLine[0] != '#') {
				switch (curFileSection) {
				case SPLASH_NOTHING:
					if (SECTION5(pLine, 'i', 'n', 't', 'r', 'o'))
						curFileSection = SPLASH_INTRO;
					else if (SECTION5(pLine, 'l', 'e', 'g', 'a', 'l'))
						curFileSection = SPLASH_LEGAL;
					else if (SECTION7(pLine, 'l', 'o', 'a', 'd', 's', 'c', 's'))
						curFileSection = SPLASH_LOADSCS;
					else if (SECTION5(pLine, 'o', 'u', 't', 'r', 'o'))
						curFileSection = SPLASH_OUTRO;
					break;
				case SPLASH_INTRO:
					if (SECTION3(pLine, 'e', 'n', 'd'))
						curFileSection = SPLASH_NOTHING;
					else {
						int nIntroIndex;
						CVector2D vec_temp_IntroPosn;
						CVector2D vec_temp_IntroScale;

						sscanf(pLine, "%d %f %f %f %f", &nIntroIndex, &vec_temp_IntroPosn.x, &vec_temp_IntroPosn.y, &vec_temp_IntroScale.x, &vec_temp_IntroScale.y);

						SplashScreen.vec_mIntroPosn[nIntroIndex] = vec_temp_IntroPosn;
						SplashScreen.vec_mIntroScale[nIntroIndex] = vec_temp_IntroScale;
					}
					break;
				case SPLASH_LEGAL:
					if (SECTION3(pLine, 'e', 'n', 'd'))
						curFileSection = SPLASH_NOTHING;
					else {
						int nLegalIndex;
						CVector2D vec_temp_LegalPosn;
						CVector2D vec_temp_LegalScale;

						sscanf(pLine, "%d %f %f %f %f", &nLegalIndex, &vec_temp_LegalPosn.x, &vec_temp_LegalPosn.y, &vec_temp_LegalScale.x, &vec_temp_LegalScale.y);

						SplashScreen.vec_mLegalPosn[nLegalIndex] = vec_temp_LegalPosn;
						SplashScreen.vec_mLegalScale[nLegalIndex] = vec_temp_LegalScale;
					}
					break;
				case SPLASH_LOADSCS:
					if (SECTION3(pLine, 'e', 'n', 'd'))
						curFileSection = SPLASH_NOTHING;
					else {
						int nLoadScreensIndex;
						CVector2D vec_temp_LoadPosn;
						CVector2D vec_temp_LoadScale;

						sscanf(pLine, "%d %f %f %f %f", &nLoadScreensIndex, &vec_temp_LoadPosn.x, &vec_temp_LoadPosn.y, &vec_temp_LoadScale.x, &vec_temp_LoadScale.y);

						SplashScreen.vec_mLoadPosn[nLoadScreensIndex] = vec_temp_LoadPosn;
						SplashScreen.vec_mLoadScale[nLoadScreensIndex] = vec_temp_LoadScale;
					}
					break;
				case SPLASH_OUTRO:
					if (SECTION3(pLine, 'e', 'n', 'd'))
						curFileSection = SPLASH_NOTHING;
					else {
						int nOutroIndex;
						CVector2D vec_temp_OutroPosn;
						CVector2D vec_temp_OutroScale;

						sscanf(pLine, "%d %f %f %f %f", &nOutroIndex, &vec_temp_OutroPosn.x, &vec_temp_OutroPosn.y, &vec_temp_OutroScale.x, &vec_temp_OutroScale.y);

						SplashScreen.vec_mOutroPosn[nOutroIndex] = vec_temp_OutroPosn;
						SplashScreen.vec_mOutroScale[nOutroIndex] = vec_temp_OutroScale;
					}
					break;
				};
			}
		}

		CFileMgr::CloseFile(hFile);
	}
}

void CLoadingScreen::LoadSplashes(bool bIntroSplash, unsigned char nIntroSplashID) {
	CLoadingScreen::Init();
	bDrawingStyle = bIntroSplash != 0;

	// LEGAL
	CPNGArchive		LegalSPTA("splash\\legal.spta");
	LegalSPTA.SetDirectory(nullptr);

	SplashScreen.m_nSplashes[LEGAL_LANG].SetTextureFromSPTA(LegalSPTA, legal_lang[FrontEndMenuManager.m_nLanguage]);
	SplashScreen.m_nSplashes[LEGAL_TITLE].SetTextureFromSPTA(LegalSPTA, "legal_title");

	LegalSPTA.CloseArchive();

	// OUTRO
	CPNGArchive		OutroSPTA("splash\\outro.spta");
	OutroSPTA.SetDirectory(nullptr);

	SplashScreen.m_nSplashes[OUTRO].SetTextureFromSPTA(OutroSPTA, "outro");

	OutroSPTA.CloseArchive();

	// LOGOS
	CPNGArchive		LogosSPTA("splash\\logos.spta");
	LogosSPTA.SetDirectory(nullptr);

	SplashScreen.m_nSplashes[VCS_LOGO].SetTextureFromSPTA(LogosSPTA, "vcs_logo");
	SplashScreen.m_nSplashes[VCSPCE_LOGO].SetTextureFromSPTA(LogosSPTA, "vcspce_logo");

	LogosSPTA.CloseArchive();

	// LOADSC0
	CPNGArchive		LoadSC0SPTA("splash\\loadsc0.spta");
	LoadSC0SPTA.SetDirectory(nullptr);

	SplashScreen.m_nSplashes[LOADSC0_bk].SetTextureFromSPTA(LoadSC0SPTA, "LOADSC0_bk");
	SplashScreen.m_nSplashes[LOADSC0_fr].SetTextureFromSPTA(LoadSC0SPTA, "LOADSC0_fr");

	LoadSC0SPTA.CloseArchive();

	// LOADSC1
	CPNGArchive		LoadSC1SPTA("splash\\loadsc1.spta");
	LoadSC1SPTA.SetDirectory(nullptr);

	SplashScreen.m_nSplashes[LOADSC1_bk].SetTextureFromSPTA(LoadSC1SPTA, "LOADSC1_bk");
	SplashScreen.m_nSplashes[LOADSC1_fr].SetTextureFromSPTA(LoadSC1SPTA, "LOADSC1_fr");

	LoadSC1SPTA.CloseArchive();

	// LOADSC2
	CPNGArchive		LoadSC2SPTA("splash\\loadsc2.spta");
	LoadSC2SPTA.SetDirectory(nullptr);

	SplashScreen.m_nSplashes[LOADSC2_bk].SetTextureFromSPTA(LoadSC2SPTA, "LOADSC2_bk");
	SplashScreen.m_nSplashes[LOADSC2_fr].SetTextureFromSPTA(LoadSC2SPTA, "LOADSC2_fr");

	LoadSC2SPTA.CloseArchive();

	// LOADSC3
	CPNGArchive		LoadSC3SPTA("splash\\loadsc3.spta");
	LoadSC3SPTA.SetDirectory(nullptr);

	SplashScreen.m_nSplashes[LOADSC3_bk].SetTextureFromSPTA(LoadSC3SPTA, "LOADSC3_bk");
	SplashScreen.m_nSplashes[LOADSC3_fr].SetTextureFromSPTA(LoadSC3SPTA, "LOADSC3_fr");

	LoadSC3SPTA.CloseArchive();

	// LOADSC4
	CPNGArchive		LoadSC4SPTA("splash\\loadsc4.spta");
	LoadSC4SPTA.SetDirectory(nullptr);

	SplashScreen.m_nSplashes[LOADSC4_bk].SetTextureFromSPTA(LoadSC4SPTA, "LOADSC4_bk");
	SplashScreen.m_nSplashes[LOADSC4_fr].SetTextureFromSPTA(LoadSC4SPTA, "LOADSC4_fr");

	LoadSC4SPTA.CloseArchive();

	// LOADSC5
	CPNGArchive		LoadSC5SPTA("splash\\loadsc5.spta");
	LoadSC5SPTA.SetDirectory(nullptr);

	SplashScreen.m_nSplashes[LOADSC5_bk].SetTextureFromSPTA(LoadSC5SPTA, "LOADSC5_bk");
	SplashScreen.m_nSplashes[LOADSC5_fr].SetTextureFromSPTA(LoadSC5SPTA, "LOADSC5_fr");

	LoadSC5SPTA.CloseArchive();
}

void CLoadingScreen::RenderSplash() {
	CSprite2d::InitPerFrame();
	RwRenderStateSet(rwRENDERSTATETEXTUREADDRESS, reinterpret_cast<void*>(rwTEXTUREADDRESSCLAMP));
	CVector2D				vecSplashScale = WidescreenSupport::GetFullscreenImageDimensions(640.0f / 448.0f, WidescreenSupport::SetAspectRatio(), true);

	if (m_bFading) {
		// Back screen.
		CSprite2d::DrawRect(CRect(0, RsGlobal.MaximumHeight, RsGlobal.MaximumWidth, 0), CRGBA(0, 0, 0, 255));

		if (bDrawingStyle == 1) { // Intro small logo
			SplashScreen.m_nSplashes[VCSPCE_LOGO].Draw(CRect(_xmiddle(SplashScreen.vec_mIntroPosn[0].x), _ymiddle(SplashScreen.vec_mIntroPosn[0].y) + _height(SplashScreen.vec_mIntroScale[0].y), _xmiddle(SplashScreen.vec_mIntroPosn[0].x) + _width(SplashScreen.vec_mIntroScale[0].x), _ymiddle(SplashScreen.vec_mIntroPosn[0].y)), CRGBA(255, 255, 255, 255));
		}
		else {
			if (!CurrentLoadingSprite) { // Legal Title
				SplashScreen.m_nSplashes[LEGAL_TITLE].Draw(CRect(_xmiddle(SplashScreen.vec_mLegalPosn[0].x - SplashScreen.vec_mLegalScale[0].x / 2), _ymiddle(SplashScreen.vec_mLegalPosn[0].y) + _height(SplashScreen.vec_mLegalScale[0].y), _xmiddle(SplashScreen.vec_mLegalPosn[0].x - SplashScreen.vec_mLegalScale[0].x / 2) + _width(SplashScreen.vec_mLegalScale[0].x), _ymiddle(SplashScreen.vec_mLegalPosn[0].y)), CRGBA(255, 255, 255, 255));
				SplashScreen.m_nSplashes[VCS_LOGO].Draw(CRect(_xmiddle(SplashScreen.vec_mLegalPosn[1].x - SplashScreen.vec_mLegalScale[1].x / 2), _ymiddle(SplashScreen.vec_mLegalPosn[1].y) + _height(SplashScreen.vec_mLegalScale[1].y), _xmiddle(SplashScreen.vec_mLegalPosn[1].x - SplashScreen.vec_mLegalScale[1].x / 2) + _width(SplashScreen.vec_mLegalScale[1].x), _ymiddle(SplashScreen.vec_mLegalPosn[1].y)), CRGBA(255, 255, 255, 255));
				if (SplashScreen.m_nSplashes[LEGAL_LANG].m_pTexture != nullptr)
					SplashScreen.m_nSplashes[LEGAL_LANG].Draw(CRect(_xmiddle(SplashScreen.vec_mLegalPosn[2].x - SplashScreen.vec_mLegalScale[2].x / 2), _ymiddle(SplashScreen.vec_mLegalPosn[2].y) + _height(SplashScreen.vec_mLegalScale[2].y), _xmiddle(SplashScreen.vec_mLegalPosn[2].x - SplashScreen.vec_mLegalScale[2].x / 2) + _width(SplashScreen.vec_mLegalScale[2].x), _ymiddle(SplashScreen.vec_mLegalPosn[2].y)), CRGBA(255, 255, 255, 255));
			}
			else // Loading splashes
				CLoadingScreen::RenderNewLoadingScreens(CurrentLoadingSprite - 1);
		}

		// Fading
		if (m_bFadeInNextSplashFromBlack || m_bFadeOutCurrSplashToBlack)
			CSprite2d::DrawRect(CRect(-5.0f, RsGlobal.MaximumHeight + 5.0f, RsGlobal.MaximumWidth + 5.0f, -5.0f), CRGBA(0, 0, 0, *(bool*)0xBAB31E ? 255 - *(unsigned char*)0xBAB320 : *(unsigned char*)0xBAB320));
	}
}

void DrawLoadingBar(float x, float y, float width, float height, float progress, bool border, bool shadow) {
	// Shadow
	if (shadow)
		CSprite2d::DrawRect(CRect((x + _coords(10.0f)), (y + _coords(10.0f)), (x + _coords(10.0f) + width + _coords(2.0f)), (y + _coords(10.0f) + height + _coords(4.0f))),
			CRGBA(0, 0, 0, 205));
	// Border
	if (border)
		CSprite2d::DrawRect(CRect((x - _coords(5.0f)), (y - _coords(5.0f)), (x - _coords(5.0f) + width + _coords(9.0f)), (y - _coords(5.0f) + height + _coords(9.0f))),
			CRGBA(0, 0, 0, 255));
	// progress value is 0.0 - 100.0
	if (progress >= 100.0f)
		progress = 100.0f;
	else {
		// Back
		CSprite2d::DrawRect(CRect((x), (y), (x + width), (y + height)),
			CRGBA(255, 255, 255, 255));
	}
	if (progress > 0.0f) {
		CSprite2d::DrawRect(CRect((x), (y), (x + width * (progress / 100.f)),
			(y + height)), CRGBA(237, 130, 180, 255));
	}
}

void CLoadingScreen::RenderNewLoadingScreens(char ScreenId) {
	CVector2D vecSplashScale = WidescreenSupport::GetFullscreenImageDimensions(21.0f / 9.0f, ScreenAspectRatio, false);
	auto  gfLoadingPercentage = *(float*)0xBAB330;

	// Percentage...
	if (gfLoadingPercentage >= 1.0) {
		if (SplashScreen.fLoadingPercentage <= 100.0f)
			SplashScreen.fLoadingPercentage = SplashScreen.fLoadingPercentage + CTimer::ms_fTimeScale * 1.0f;
		else
			SplashScreen.fLoadingPercentage = 100.0f;

		if (SplashScreen.fLoadingPercentage >= 100.0f)
			SplashScreen.fLoadingPercentage = 100.0f;
	}

	// Start Drawings.
	// Back screen.
	CSprite2d::DrawRect(CRect(0, RsGlobal.MaximumHeight, RsGlobal.MaximumWidth, 0), CRGBA(0, 0, 0, 255));

	// Start loading tune.
	((char(__thiscall*)(CAudioEngine*, int, float, float))0x4DD4A0)(&AudioEngine + 180, 0x32, 0.0, 1.0);

	if (CurrentLoadingSprite > 6)
		CurrentLoadingSprite = 2;

	if (ScreenId == 0) {
		SplashScreen.m_nSplashes[LOADSC0_bk].Draw(CRect(-1.0f - ((vecSplashScale.x / 2) - RsGlobal.MaximumWidth / 2), (RsGlobal.MaximumHeight / 2) + vecSplashScale.y / 2, ((vecSplashScale.x / 2) + RsGlobal.MaximumWidth / 2), (RsGlobal.MaximumHeight / 2) - vecSplashScale.y / 2), CRGBA(255, 255, 255, 255));
		SplashScreen.m_nSplashes[LOADSC0_fr].Draw(CRect(_xmiddle(SplashScreen.vec_mLoadPosn[0].x), _ymiddle(SplashScreen.vec_mLoadPosn[0].y) + _height(SplashScreen.vec_mLoadScale[0].y), _xmiddle(SplashScreen.vec_mLoadPosn[0].x) + _width(SplashScreen.vec_mLoadScale[0].x), _ymiddle(SplashScreen.vec_mLoadPosn[0].y)), CRGBA(255, 255, 255, 255));
	}
	else if (ScreenId == 1) {
		SplashScreen.m_nSplashes[LOADSC1_bk].Draw(CRect(-1.0f - ((vecSplashScale.x / 2) - RsGlobal.MaximumWidth / 2), (RsGlobal.MaximumHeight / 2) + vecSplashScale.y / 2, ((vecSplashScale.x / 2) + RsGlobal.MaximumWidth / 2), (RsGlobal.MaximumHeight / 2) - vecSplashScale.y / 2), CRGBA(255, 255, 255, 255));
		SplashScreen.m_nSplashes[LOADSC1_fr].Draw(CRect(_xmiddle(SplashScreen.vec_mLoadPosn[1].x), _ymiddle(SplashScreen.vec_mLoadPosn[1].y) + _height(SplashScreen.vec_mLoadScale[1].y), _xmiddle(SplashScreen.vec_mLoadPosn[1].x) + _width(SplashScreen.vec_mLoadScale[1].x), _ymiddle(SplashScreen.vec_mLoadPosn[1].y)), CRGBA(255, 255, 255, 255));
	}
	else if (ScreenId == 2) {
		SplashScreen.m_nSplashes[LOADSC2_bk].Draw(CRect(-1.0f - ((vecSplashScale.x / 2) - RsGlobal.MaximumWidth / 2), (RsGlobal.MaximumHeight / 2) + vecSplashScale.y / 2, ((vecSplashScale.x / 2) + RsGlobal.MaximumWidth / 2), (RsGlobal.MaximumHeight / 2) - vecSplashScale.y / 2), CRGBA(255, 255, 255, 255));
		SplashScreen.m_nSplashes[LOADSC2_fr].Draw(CRect(_xmiddle(SplashScreen.vec_mLoadPosn[2].x), _ymiddle(SplashScreen.vec_mLoadPosn[2].y) + _height(SplashScreen.vec_mLoadScale[2].y), _xmiddle(SplashScreen.vec_mLoadPosn[2].x) + _width(SplashScreen.vec_mLoadScale[2].x), _ymiddle(SplashScreen.vec_mLoadPosn[2].y)), CRGBA(255, 255, 255, 255));
	}
	else if (ScreenId == 3) {
		SplashScreen.m_nSplashes[LOADSC3_bk].Draw(CRect(-1.0f - ((vecSplashScale.x / 2) - RsGlobal.MaximumWidth / 2), (RsGlobal.MaximumHeight / 2) + vecSplashScale.y / 2, ((vecSplashScale.x / 2) + RsGlobal.MaximumWidth / 2), (RsGlobal.MaximumHeight / 2) - vecSplashScale.y / 2), CRGBA(255, 255, 255, 255));
		SplashScreen.m_nSplashes[LOADSC3_fr].Draw(CRect(_x(SplashScreen.vec_mLoadPosn[3].x), _ymiddle(SplashScreen.vec_mLoadPosn[3].y) + _height(SplashScreen.vec_mLoadScale[3].y), _x(SplashScreen.vec_mLoadPosn[3].x) + _width(SplashScreen.vec_mLoadScale[3].x), _ymiddle(SplashScreen.vec_mLoadPosn[3].y)), CRGBA(255, 255, 255, 255));
	}
	else if (ScreenId == 4) {
		SplashScreen.m_nSplashes[LOADSC4_bk].Draw(CRect(-1.0f - ((vecSplashScale.x / 2) - RsGlobal.MaximumWidth / 2), (RsGlobal.MaximumHeight / 2) + vecSplashScale.y / 2, ((vecSplashScale.x / 2) + RsGlobal.MaximumWidth / 2), (RsGlobal.MaximumHeight / 2) - vecSplashScale.y / 2), CRGBA(255, 255, 255, 255));
		SplashScreen.m_nSplashes[LOADSC4_fr].Draw(CRect(_xmiddle(SplashScreen.vec_mLoadPosn[4].x), _ymiddle(SplashScreen.vec_mLoadPosn[4].y) + _height(SplashScreen.vec_mLoadScale[4].y), _xmiddle(SplashScreen.vec_mLoadPosn[4].x) + _width(SplashScreen.vec_mLoadScale[4].x), _ymiddle(SplashScreen.vec_mLoadPosn[4].y)), CRGBA(255, 255, 255, 255));
	}
	else if (ScreenId == 5) {
		SplashScreen.m_nSplashes[LOADSC5_bk].Draw(CRect(-1.0f - ((vecSplashScale.x / 2) - RsGlobal.MaximumWidth / 2), (RsGlobal.MaximumHeight / 2) + vecSplashScale.y / 2, ((vecSplashScale.x / 2) + RsGlobal.MaximumWidth / 2), (RsGlobal.MaximumHeight / 2) - vecSplashScale.y / 2), CRGBA(255, 255, 255, 255));
		SplashScreen.m_nSplashes[LOADSC5_fr].Draw(CRect(_xmiddle(SplashScreen.vec_mLoadPosn[5].x), _ymiddle(SplashScreen.vec_mLoadPosn[5].y) + _height(SplashScreen.vec_mLoadScale[5].y), _xmiddle(SplashScreen.vec_mLoadPosn[5].x) + _width(SplashScreen.vec_mLoadScale[5].x), _ymiddle(SplashScreen.vec_mLoadPosn[5].y)), CRGBA(255, 255, 255, 255));
	}

	// Loading Bar
	DrawLoadingBar(_xleft(41.0f), _ydown(43.0f), _width(154.0f), _height(13.0f), SplashScreen.fLoadingPercentage, 1, 1);

	// Logo
	SplashScreen.m_nSplashes[VCS_LOGO].Draw(CRect(_xleft(28.0f), _ydown(196.0f) + _height(159.0f), _xleft(28.0f) + _width(181.0f), _ydown(196.0f)), CRGBA(255, 255, 255, 255));

	// Maybe find a better solution for this...
	if (SplashScreen.fLoadingPercentage >= 100.0f)
		Memory::Patch<BYTE>(0x590DAF, 0x75);
	else
		Memory::Patch<BYTE>(0x590DAF, 0x74);
}

void CMenuManager::SkyMenuHelpText(char nID) {
	float x = _x(98.0f);
	float fPosX[3] = { 66.0f, 36.0f, 6.0f };
	char *pText[5] = { "SKY_MOV", "SKY_BAK", "SKY_ENT", "SKY_LEG", "SKY_MAR", };

	CFont::SetOrientation(ALIGN_Right);
	CFont::SetFontStyle(FONT_Eurostile);
	CFont::SetScaleLang(_width(0.4f), _height(0.8f));
	CFont::SetDropShadowPosition(2);
	CFont::SetColor(CRGBA(205, 205, 205, SafeZoneHideStuffAlpha));
	CFont::SetDropColor(CRGBA(0, 0, 0, SafeZoneHideStuffAlpha));

	// First column
	for (int i = 0; i < 4; i++)
		CFont::PS2Sprite[i + 1].Draw(CRect(_xleft(16.0f * (i + 1)) + (x), (_ydown(56.0f)) + _height(16.0f * 440.f / 480.f), _xleft(16.0f * (i + 1)) + (x)+_width(16.0f), (_ydown(56.0f))), CRGBA(255, 255, 255, SafeZoneHideStuffAlpha));
	CFont::PrintString(_xleft(14.0f) + (x), _ydown(56.0f), TheText.Get(pText[0]));

	// Second column
	switch (nID) {
	case 2:
		CFont::PS2Sprite[8].Draw(CRect(_xleft(fPosX[0]) + (x), (_ydown(26.0f)) + _height(16.0f * 440.f / 480.f), _xleft(fPosX[0]) + (x)+_width(16.0f), (_ydown(26.0f))), CRGBA(255, 255, 255, SafeZoneHideStuffAlpha));
		CFont::PrintString(_xleft(fPosX[0] - 2.0f) + (x), _ydown(26.0f), TheText.Get(pText[1]));

		CFont::PS2Sprite[5].Draw(CRect(_xleft(fPosX[1]) + (x) - CFont::GetStringWidth((char*)TheText.Get(pText[1]), 1, 0), (_ydown(26.0f)) + _height(16.0f * 440.f / 480.f), _xleft(fPosX[1]) + (x) - CFont::GetStringWidth((char*)TheText.Get(pText[1]), 1, 0) + _width(16.0f), (_ydown(26.0f))), CRGBA(255, 255, 255, SafeZoneHideStuffAlpha));
		CFont::PrintString(_xleft(fPosX[1] - 2.0f) + (x) - CFont::GetStringWidth((char*)TheText.Get(pText[1]), 1, 0), _ydown(26.0f), TheText.Get(pText[3]));

		CFont::PS2Sprite[7].Draw(CRect(_xleft(fPosX[2]) + (x) - CFont::GetStringWidth((char*)TheText.Get(pText[1]), 1, 0) - CFont::GetStringWidth((char*)TheText.Get(pText[3]), 1, 0), (_ydown(26.0f)) + _height(16.0f * 440.f / 480.f), _xleft(fPosX[2]) + (x) - CFont::GetStringWidth((char*)TheText.Get(pText[1]), 1, 0) - CFont::GetStringWidth((char*)TheText.Get(pText[3]), 1, 0) + _width(16.0f), (_ydown(26.0f))), CRGBA(255, 255, 255, SafeZoneHideStuffAlpha));
		CFont::PrintString(_xleft(fPosX[2] - 2.0f) + (x) - CFont::GetStringWidth((char*)TheText.Get(pText[1]), 1, 0) - CFont::GetStringWidth((char*)TheText.Get(pText[3]), 1, 0), _ydown(26.0f), TheText.Get(pText[4]));
		break;
	case 1:
		CFont::PS2Sprite[5].Draw(CRect(_xleft(fPosX[0]) + (x), (_ydown(26.0f)) + _height(16.0f * 440.f / 480.f), _xleft(fPosX[0]) + (x)+_width(16.0f), (_ydown(26.0f))), CRGBA(255, 255, 255, SafeZoneHideStuffAlpha));
		CFont::PrintString(_xleft(fPosX[0] - 2.0f) + (x), _ydown(26.0f), TheText.Get(pText[2]));
		break;
	default:
		CFont::PS2Sprite[8].Draw(CRect(_xleft(fPosX[0]) + (x), (_ydown(26.0f)) + _height(16.0f * 440.f / 480.f), _xleft(fPosX[0]) + (x)+_width(16.0f), (_ydown(26.0f))), CRGBA(255, 255, 255, SafeZoneHideStuffAlpha));
		CFont::PrintString(_xleft(fPosX[0] - 2.0f) + (x), _ydown(26.0f), TheText.Get(pText[1]));

		CFont::PS2Sprite[5].Draw(CRect(_xleft(fPosX[1]) + (x)-CFont::GetStringWidth((char*)TheText.Get(pText[1]), 1, 0), (_ydown(26.0f)) + _height(16.0f * 440.f / 480.f), _xleft(fPosX[1]) + (x)-CFont::GetStringWidth((char*)TheText.Get(pText[1]), 1, 0) + _width(16.0f), (_ydown(26.0f))), CRGBA(255, 255, 255, SafeZoneHideStuffAlpha));
		CFont::PrintString(_xleft(fPosX[1] - 2.0f) + (x)-CFont::GetStringWidth((char*)TheText.Get(pText[1]), 1, 0), _ydown(26.0f), TheText.Get(pText[2]));
		break;
	};
}

void CMenuManager::DrawSkyMenu() {
	RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, reinterpret_cast<void*>(2));
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, reinterpret_cast<void*>(0));
	RwRenderStateSet(rwRENDERSTATEFOGENABLE, reinterpret_cast<void*>(0));
	RwRenderStateSet(rwRENDERSTATESRCBLEND, reinterpret_cast<void*>(5));
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, reinterpret_cast<void*>(6));
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, reinterpret_cast<void*>(1));
	RwRenderStateSet(rwRENDERSTATETEXTUREADDRESS, reinterpret_cast<void*>(3));
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, reinterpret_cast<void*>(0));
	RwRenderStateSet(rwRENDERSTATESHADEMODE, reinterpret_cast<void*>(1));

	float MOUSEX = FrontEndMenuManager.m_dwMousePosLeft;
	float MOUSEY = FrontEndMenuManager.m_dwMousePosTop;
	int LMB = CPad::GetPad(0)->NewMouseControllerState.lmb == 0 && CPad::GetPad(0)->OldMouseControllerState.lmb != 0;

	// Bottom Bar
	CSprite2d::DrawRect(CRect(-1.0f, RsGlobal.MaximumHeight + 1.0f, RsGlobal.MaximumWidth + 1.0f, RsGlobal.MaximumHeight * 0.83f - 1.0f), CRGBA(7, 7, 7, 255));

	if (FrontEndMenuManager.m_bBottomMenu && MOUSEY < RsGlobal.MaximumHeight * 0.83f - 1.0f) {
		if (LMB) {
			//FrontEndMenuManager.m_dwSelectedMenuItem = 0;
			FrontEndMenuManager.m_bIsMenuSwitched = true;
			FrontEndMenuManager.m_bBottomMenu = false;

			AudioEngine.ReportFrontendAudioEvent(1, 0.0f, 1.0f);
		}
	}

	if (FrontEndMenuManager.m_bCurrentMenuPage >= 16 &&
		FrontEndMenuManager.m_bCurrentMenuPage <= 22 ||
		FrontEndMenuManager.m_bCurrentMenuPage >= 29 &&
		FrontEndMenuManager.m_bCurrentMenuPage <= 32) {
		FrontEndMenuManager.m_bBottomMenu = false;
		FrontEndMenuManager.m_bPCFrontEnd = true;
	}
	else {
		FrontEndMenuManager.m_bPCFrontEnd = false;

		// Set pages.
		if (!FrontEndMenuManager.m_bIsMenuSwitched) {
			if (FrontEndMenuManager.m_bIsPauseMenu) {
				char m_dwPauseMenuPages[8] = { 5, 2, 1, 0, 40, 3, 4, 27 };

				for (int i = 0; i < 8; i++) {
					if (FrontEndMenuManager.m_nSelectedSkyMenuItem == i) {
						FrontEndMenuManager.m_dwSelectedMenuItem = -20;
						FrontEndMenuManager.SwitchToNewScreen(m_dwPauseMenuPages[i]);
						FrontEndMenuManager.m_bIsMenuSwitched = true;
					}
				}
			}
			else {
				char m_dwMainMenuPages[5] = { 1, 40, 3, 4, 27 };

				for (int i = 0; i < 5; i++) {
					if (FrontEndMenuManager.m_nSelectedSkyMenuItem == i) {
						FrontEndMenuManager.m_dwSelectedMenuItem = -20;
						FrontEndMenuManager.SwitchToNewScreen(m_dwMainMenuPages[i]);
						FrontEndMenuManager.m_bIsMenuSwitched = true;
					}
				}
			}
		}

		// Limits
		if (FrontEndMenuManager.m_bIsPauseMenu) {
			if (FrontEndMenuManager.m_nSelectedSkyMenuItem > 7)
				FrontEndMenuManager.m_nSelectedSkyMenuItem = 0;
			if (FrontEndMenuManager.m_nSelectedSkyMenuItem < 0)
				FrontEndMenuManager.m_nSelectedSkyMenuItem = 7;
		}
		else {
			if (FrontEndMenuManager.m_nSelectedSkyMenuItem > 4)
				FrontEndMenuManager.m_nSelectedSkyMenuItem = 0;
			if (FrontEndMenuManager.m_nSelectedSkyMenuItem < 0)
				FrontEndMenuManager.m_nSelectedSkyMenuItem = 4;
		}

		// Set Text
		CFont::SetBackground(0, 0);
		CFont::SetProportional(1);
		CFont::SetWrapx(RsGlobal.MaximumWidth * 1.5f);
		CFont::SetDropColor(CRGBA(0, 0, 0, SafeZoneHideStuffAlpha));
		CFont::SetScaleLang(_width(0.90f * ((CDraw::ms_fAspectRatio < 16.0f / 10.0f) ? (1.0f / CDraw::ms_fAspectRatio) : 1.0f)), _height(1.45f));
		CFont::SetOrientation(ALIGN_Left);
		CFont::SetFontStyle(FONT_Pricedown);

		int m_dwMenuSlots;
		char *MainMenuText[5] = { "FEH_LOA", "SKY_CTL", "FEH_AUD", "FEH_DIS", "FEH_GFX" };
		char *PauseMenuText[8] = { "FEH_MAP", "FEH_BRI", "FEH_LOA", "FEH_STA", "SKY_CTL", "FEH_AUD", "FEH_DIS", "FEH_GFX" };
		float a, b, c, d, e, f, g, h;

		if (FrontEndMenuManager.m_bIsPauseMenu) {
			a = CFont::GetStringWidth((char*)TheText.Get(PauseMenuText[0]), 1, 0);
			b = CFont::GetStringWidth((char*)TheText.Get(PauseMenuText[1]), 1, 0);
			c = CFont::GetStringWidth((char*)TheText.Get(PauseMenuText[2]), 1, 0);
			d = CFont::GetStringWidth((char*)TheText.Get(PauseMenuText[3]), 1, 0);
			e = CFont::GetStringWidth((char*)TheText.Get(PauseMenuText[4]), 1, 0);
			f = CFont::GetStringWidth((char*)TheText.Get(PauseMenuText[5]), 1, 0);
			g = CFont::GetStringWidth((char*)TheText.Get(PauseMenuText[6]), 1, 0);
			h = CFont::GetStringWidth((char*)TheText.Get(PauseMenuText[7]), 1, 0);

			m_dwMenuSlots = 8;
		}
		else {
			a = CFont::GetStringWidth((char*)TheText.Get(MainMenuText[0]), 1, 0);
			b = CFont::GetStringWidth((char*)TheText.Get(MainMenuText[1]), 1, 0);
			c = CFont::GetStringWidth((char*)TheText.Get(MainMenuText[2]), 1, 0);
			d = CFont::GetStringWidth((char*)TheText.Get(MainMenuText[3]), 1, 0);
			e = CFont::GetStringWidth((char*)TheText.Get(MainMenuText[4]), 1, 0);
			f = 0;
			g = 0;
			h = 0;

			m_dwMenuSlots = 5;
		}

		CVector2D vecBottomText[2] = { CVector2D(_xleft(18.0f), _ydown(68.0f)), CVector2D(_xleft(18.0f), _ydown(38.0f)) };

		float fWidthStart[8] = { 0.0f, a, a + b, FrontEndMenuManager.m_bIsPauseMenu ? a + b + c : 0.0f, FrontEndMenuManager.m_bIsPauseMenu ? a + b + c + d : d, 0.0f, f, f + g };
		float fWidthEnd[8] = {
			a,
			a + b,
			a + b + c,
			FrontEndMenuManager.m_bIsPauseMenu ? a + b + c + d : d,
			FrontEndMenuManager.m_bIsPauseMenu ? a + b + c + d + e : d + e,
			f,
			f + g,
			f + g + h };
		float fSpacing[8] = { 16.0f * 0, 16.0f * 1, 16.0f * 2, FrontEndMenuManager.m_bIsPauseMenu ? 16.0f * 3 : 0, FrontEndMenuManager.m_bIsPauseMenu ? 16.0f * 4 : 16.0f * 1, 16.0f * 0, 16.0f * 1, 16.0f * 2 };

		// Print strings
		CRGBA color;
		int i = 0;
		int j = 0;

		for (i = 0; i < m_dwMenuSlots; i++) {
			// Set selected menu item
			if (i > m_dwMenuSlots / 2)
				j = 1;
			if (FrontEndMenuManager.m_bDrawMouse) {
				if (MOUSEX > vecBottomText[j].x + _xleft(fSpacing[i]) + fWidthStart[i] && MOUSEX < vecBottomText[j].x + _xleft(fSpacing[i]) + fWidthEnd[i] && MOUSEY > vecBottomText[j].y && MOUSEY < vecBottomText[j].y + _y(24.0f)) {
					FrontEndMenuManager.m_bIsMouseInPosition = true;
					FrontEndMenuManager.m_bBottomMenu = true;
					//FrontEndMenuManager.m_dwSelectedMenuItem = -1;
				}
				else {
					FrontEndMenuManager.m_bIsMouseInPosition = false;
				}
			}
			else
				FrontEndMenuManager.m_bIsMouseInPosition = false;

			if (FrontEndMenuManager.m_nSelectedSkyMenuItem == i && FrontEndMenuManager.m_bBottomMenu) {
				color = { MENU_PINK_R, MENU_PINK_G, MENU_PINK_B };
			}
			else {
				if (FrontEndMenuManager.m_bIsMouseInPosition) {
					color = { MENU_ACTIVE_R, MENU_ACTIVE_G, MENU_ACTIVE_B };

					if (LMB) {
						AudioEngine.ReportFrontendAudioEvent(1, 0.0f, 1.0f);

						FrontEndMenuManager.m_bIsMenuSwitched = false;
						FrontEndMenuManager.m_nSelectedSkyMenuItem = i;
					}
				}
				else
					color = { MENU_INACTIVE_R, MENU_INACTIVE_G, MENU_INACTIVE_B };
			}

			CFont::SetColor(CRGBA(color.r, color.g, color.b, SafeZoneHideStuffAlpha));

			if (FrontEndMenuManager.m_bIsPauseMenu)
				CFont::PrintString((vecBottomText[j].x + _xleft(fSpacing[i]) + fWidthStart[i]), (vecBottomText[j].y), TheText.Get(PauseMenuText[i]));
			else
				CFont::PrintString((vecBottomText[j].x + _xleft(fSpacing[i]) + fWidthStart[i]), (vecBottomText[j].y), TheText.Get(MainMenuText[i]));
		}

		// Radio Icons
		if (FrontEndMenuManager.m_bCurrentMenuPage == 3 && !FrontEndMenuManager.m_bBottomMenu)
			((void(__stdcall*)(int, char))0x507DC0)(FrontEndMenuManager.m_nRadioStation, 0);
		else
			((void(__stdcall*)(int, char))0x506F70)(0, 0);

		if (FrontEndMenuManager.m_bCurrentMenuPage == MENU_PAGE_AUDIO_SETUP) {
			CRGBA color;
			float fPosition = (0.5f*WidescreenSupport::GetScreenWidthMultiplier()) + 210.0f;

			for (int i = 1; i < 11; i++) {
				if (i == FrontEndMenuManager.m_nRadioStation && !FrontEndMenuManager.m_bBottomMenu)
					color = { 255, 225, 135, 255 };
				else
					color = { 255, 255, 255, 100 };

				if (i < 6)
					FrontEndMenuManager.m_apRadioSprites[i].Draw(_x(fPosition - 90.0f * (i - 1)), _ymiddle(8.0f), _width(60.0f), _height(60.0f), CRGBA(color));
				else
					FrontEndMenuManager.m_apRadioSprites[i].Draw(_x(fPosition - 90.0f * (i - 6)), _ymiddle(70.0f), _width(60.0f), _height(60.0f), CRGBA(color));
			}
		}
	}

	// Legend
	if (FrontEndMenuManager.m_bCurrentMenuPage == 5) {
		if (FrontEndMenuManager.m_bMapLegend)
			FrontEndMenuManager.DrawSkyLegend();
	}

	// Header
	CFont::SetOrientation(1);
	CFont::SetFontStyle(FONT_RageItalic);
	CFont::SetScale(_width(1.8f), _height(3.2f));
	CFont::SetDropShadowPosition(2);
	CFont::SetColor(CRGBA(MENU_PINK_R, MENU_PINK_G, MENU_PINK_B, SafeZoneHideStuffAlpha));
	CFont::SetDropColor(CRGBA(0, 0, 0, SafeZoneHideStuffAlpha));
	CFont::PrintString(_xleft(18.0f), _y(4.0f), TheText.Get(FrontEndMenuManager._aScreens[FrontEndMenuManager.m_bCurrentMenuPage].name));

	// Help text.
	switch (FrontEndMenuManager.m_bCurrentMenuPage) {
	case 5:
		if (FrontEndMenuManager.m_bBottomMenu)
			SkyMenuHelpText(0);
		else
			SkyMenuHelpText(2);
		return;
	default:
		if (!FrontEndMenuManager.m_bIsPauseMenu && FrontEndMenuManager.m_bBottomMenu)
			SkyMenuHelpText(1);
		else
			SkyMenuHelpText(0);
		return;
	};
}

void CMenuManager::DrawSafeZoneScreen() {
	float fLayoutPosn[8] = { 
		_x(108.5f + UI_SAFEZONE * 2.2f), _y(27.0f + UI_SAFEZONE),
		_x(108.5f + UI_SAFEZONE * 2.2f), _ydown(31.5f + UI_SAFEZONE),
		_xleft(47.0f + UI_SAFEZONE * 2.2f), _y(27.0f + UI_SAFEZONE),
		_xleft(47.0f + UI_SAFEZONE * 2.2f),  _ydown(31.5f + UI_SAFEZONE)
	};

	float fLayoutScale[2] = { _width(114.0f * 0.5f), _height(8.0f * 0.5f) };

	CRGBA m_dwColor = { 255, 255, 255, 255 };

	// Top Right
	CSprite2d::DrawRect(CRect(fLayoutPosn[0], fLayoutPosn[1], fLayoutPosn[0] + fLayoutScale[0], fLayoutPosn[1] + fLayoutScale[1]), CRGBA(m_dwColor));
	CSprite2d::DrawRect(CRect(fLayoutPosn[0] + fLayoutScale[0], fLayoutPosn[1], fLayoutPosn[0] + fLayoutScale[0] + fLayoutScale[1], fLayoutPosn[1] + fLayoutScale[0]), CRGBA(m_dwColor));

	// Bottom Right
	CSprite2d::DrawRect(CRect(fLayoutPosn[2], fLayoutPosn[3], fLayoutPosn[2] + fLayoutScale[0], fLayoutPosn[3] + fLayoutScale[1]), CRGBA(m_dwColor));
	CSprite2d::DrawRect(CRect(fLayoutPosn[2] + fLayoutScale[0], fLayoutPosn[3] - fLayoutScale[0], fLayoutPosn[2] + fLayoutScale[0] + fLayoutScale[1], fLayoutPosn[3] + fLayoutScale[1]), CRGBA(m_dwColor));

	// Top Left
	CSprite2d::DrawRect(CRect(fLayoutPosn[4], fLayoutPosn[5], fLayoutPosn[4] + fLayoutScale[0] + fLayoutScale[1], fLayoutPosn[5] + fLayoutScale[1]), CRGBA(m_dwColor));
	CSprite2d::DrawRect(CRect(fLayoutPosn[4], fLayoutPosn[5], fLayoutPosn[4] + fLayoutScale[1], fLayoutPosn[5] + fLayoutScale[0]), CRGBA(m_dwColor));

	// Bottom Left
	CSprite2d::DrawRect(CRect(fLayoutPosn[6], fLayoutPosn[7], fLayoutPosn[6] + fLayoutScale[0] + fLayoutScale[1], fLayoutPosn[7] + fLayoutScale[1]), CRGBA(m_dwColor));
	CSprite2d::DrawRect(CRect(fLayoutPosn[6], fLayoutPosn[7] - fLayoutScale[0], fLayoutPosn[6] + fLayoutScale[1], fLayoutPosn[7]), CRGBA(m_dwColor));
}

static void __declspec(naked) UserInputArrowSoundMenus()
{
	_asm
	{
		jz		UserInputArrowSoundMenus_AllowMenu
		cmp     al, 39
		jz		UserInputArrowSoundMenus_AllowMenu
		cmp		al, MENU_PAGE_CONTROLLER_SETUP
		jz		UserInputArrowSoundMenus_AllowMenu
		cmp		al, 44
		jz		UserInputArrowSoundMenus_AllowMenu
		cmp		al, MENU_PAGE_ADDITIONAL_CONTROLLER
		jnz		UserInputArrowSoundMenus_False

UserInputArrowSoundMenus_AllowMenu:
		push	5805C0h
		retn

UserInputArrowSoundMenus_False:
		push	580600h
		retn
	}
}


static Reversed ProcessPCMenuOptions_kill(0x57CD50, 0x57D51F);


static StaticPatcher	Patcher([](){
				static const BYTE			UserInputArrowSoundMenus_Table[] = {
								0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01,
								0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,
								0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
								0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
								0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01,
								0x00 };

				Memory::Patch<BYTE>(0x5805DC, sizeof(UserInputArrowSoundMenus_Table)-1);
				Memory::Patch<const void*>(0x5805E2, UserInputArrowSoundMenus_Table);

				Memory::InjectHook(0x573AE0, &CMenuManager::SetDefaultPreferences, PATCH_JUMP);
				Memory::InjectHook(0x5805BA, &UserInputArrowSoundMenus, PATCH_JUMP);

				Memory::InjectHook(0x57C660, &CMenuManager::SaveSettings, PATCH_JUMP);
				Memory::InjectHook(0x747545, &CMenuManager::LoadSettings);
				Memory::InjectHook(0x47B8B2, &CMenuManager::NeedsToRefreshHelps);

				CMenuManager::Inject();
			});

