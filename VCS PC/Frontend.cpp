#include "StdAfx.h"
#include "Frontend.h"

#include "Font.h"
#include "Timer.h"
#include "UpdateManager.h"
#include "Messages.h"
#include "Stats.h"
#include "VideoPlayer.h"
#include "Text.h"
#include "ControlsMgr.h"
#include "Audio.h"

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

//short			CMenuManager::nColourMenuEntries;
MenuItem		CMenuManager::ms_pMenus[] = {
	// Stats
	{ "FEH_STA", 42, 3,
		21, "FEDS_TB", ACTION_CLICKORARROWS, 0, 0, 69, 3, 1, 0 },

	// Game
	{ "FEH_LOA", 42, 1,
		10, "FES_NGA", ACTION_STANDARD, 6, 0, -74, 3, 0, 0,
		5, "FES_LOA", ACTION_STANDARD, 9, 0, 0, 3, 0, 0,
		5, "FES_DEL", ACTION_STANDARD, 10, 0, 0, 3, 0, 0,
		2, "FEDS_TB", ACTION_STANDARD, 0, 0, 0, 3, 0, 0 },

	// Brief
	{ "FEH_BRI", 42, 4,
		2, "FEDS_TB", ACTION_STANDARD, 42, 0, 45, 3, 1, 0 },


	{ "FEH_AUD", 33, 1,
		28, "FEA_MUS", ACTION_CLICKORARROWS, 3, 0, -124, 2, 0, 0,
		29, "FEA_SFX", ACTION_CLICKORARROWS, 3, 0, 0, 2, 0, 0,
		31, "FEA_ART", ACTION_CLICKORARROWS, 3, 0, 0, 2, 0, 0,
		5, "FEA_TIT", ACTION_STANDARD, 26, 0, -29, 3, 0, 0,
		32, "FEA_RSS", ACTION_CLICKORARROWS, 3, 0, 36, 2, 0, 0,
		5, "FET_DEF", ACTION_STANDARD, 24, 0, 143, 3, 0, 0,
		2, "FEDS_TB", ACTION_STANDARD, 42, 0, 0, 3, 0, 0 },

	// Display Setup
	{ "FEH_DIS", 33, 2,
		27, "FED_BRI", ACTION_CLICKORARROWS, 4, 0, -97, 2, 0, 0,
		33, "MAP_LEG", ACTION_CLICKORARROWS, 4, 0, 0, 2, 0, 0,
		34, "FED_RDR", ACTION_CLICKORARROWS, 4, 0, 0, 2, 0, 0,
		35, "FED_HUD", ACTION_CLICKORARROWS, 4, 0, 0, 2, 0, 0,
		25, "FED_SUB", ACTION_CLICKORARROWS, 4, 0, 0, 2, 0, 0,
		65, "FED_GLS", ACTION_CLICKORARROWS, 4, 0, 0, 2, 0, 0,
		5, "FET_DEF", ACTION_STANDARD, 23, 0, 141, 3, 0, 0,
		2, "FEDS_TB", ACTION_STANDARD, 33, 0, 0, 3, 0, 0 },


	{ "FEH_MAP", 42, 2,
		2, "FEDS_TB", ACTION_STANDARD, 42, 57, 186, 1, 0, -1 },


	{ "FEH_NGA", 1, 0,
		1, "FESZ_QR", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		4, "FEM_NO", ACTION_STANDARD, 1, 0, -9, 3, 0, 0,
		14, "FEM_YES", ACTION_STANDARD, 6, 0, 16, 3, 0, 0 },


	{ "FEH_NGA", 1, 0,
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


	{ "FES_LMI", 1, 0,
		1, "FESZ_QM", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		4, "FEM_NO", ACTION_STANDARD, 1, 0, -9, 3, 0, 0,
		12, "FEM_YES", ACTION_STANDARD, 0, 0, 16, 3, 0, 0 },


	{ "FET_LG", 1, 1,
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


	{ "FEH_DEL", 1, 2,
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


	{ "FET_LG", 9, 0,
		1, "FESZ_QL", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		4, "FEM_NO", ACTION_STANDARD, 9, 0, -9, 3, 0, 0,
		3, "FEM_YES", ACTION_STANDARD, 13, 0, 16, 3, 0, 0 },


	{ "FEH_DEL", 10, 0,
		1, "FESZ_QD", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		4, "FEM_NO", ACTION_STANDARD, 10, 0, -9, 3, 0, 0,
		3, "FEM_YES", ACTION_STANDARD, 14, 0, 16, 3, 0, 0 },


	{ "FET_LG", 9, 0 },


	{ "FEH_DEL", 10, 0 },


	{ "FEH_DEL", 1, 0,
		1, "FES_DSC", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEM_OK", ACTION_STANDARD, 1, 0, 16, 3, 0, 0 },


	{ "FET_SG", 255, 0,
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


	{ "FET_SG", 16, 0,
		1, "FESZ_QZ", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		4, "FEM_NO", ACTION_STANDARD, 16, 0, -9, 3, 0, 0,
		3, "FEM_YES", ACTION_STANDARD, 18, 0, 16, 3, 0, 0 },


	{ "FET_SG", 16, 0 },


	{ "FET_SG", 16, 0,
		1, "FES_SSC", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		54, "FEM_OK", ACTION_STANDARD, 0, 0, 16, 3, 0, 0 },


	{ "FET_SG", 42, 0,
		1, "", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEM_OK", ACTION_STANDARD, 0, 0, 46, 3, 0, 0 },


	{ "FET_LG", 42, 0,
		1, "", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEM_OK", ACTION_STANDARD, 0, 0, 46, 3, 0, 0 },


	{ "FET_SG", 1, 0,
		1, "FES_CHE", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEM_OK", ACTION_STANDARD, 16, 0, 16, 3, 0, 0 },


	{ "FEH_DIS", 4, 7,
		1, "FED_RDP", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEM_NO", ACTION_STANDARD, 4, 0, -9, 3, 0, 0,
		57, "FEM_YES", ACTION_STANDARD, 4, 0, 16, 3, 0, 0},


	{ "FEH_AUD", 3, 6,
		1, "FED_RDP", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEM_NO", ACTION_STANDARD, 3, 0, -9, 3, 0, 0,
		57, "FEM_YES", ACTION_STANDARD, 3, 0, 16, 3, 0, 0 },


	{ "FET_CTL", 36, 3,
		1, "FED_RDP", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEM_NO", ACTION_STANDARD, 36, 0, -9, 3, 0, 0,
		57, "FEM_YES", ACTION_STANDARD, 36, 0, 16, 3, 0, 0 },

	// Custom Tracks Options
	{ "FEH_TIT", 3, 4,
		1, "FEA_SUB", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		63, "FEA_MPM", ACTION_CLICKORARROWS, 26, 0, -34, 2, 0, 0,
		64, "FEA_AMS", ACTION_CLICKORARROWS, 26, 0, 0, 2, 0, 0,
		6, "FEA_SCN", ACTION_STANDARD, 26, 0, 0, 3, 0, 0,
		2, "FEDS_TB", ACTION_STANDARD, 3, 0, 136, 3, 0, 0 },

	// Graphics Setup
	{ "FEH_GFX", 33, 3,
		61, "FEM_LOD", ACTION_CLICKORARROWS, 27, 0, -97, 2, 0, 0,
		24, "FEM_FRM", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		26, "FED_WIS", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		42, "FED_FXQ", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		43, "FED_MIP", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		44, "FED_AAS", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		56, "FED_RES", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		2, "FEDS_TB", ACTION_STANDARD, 33, 0, 136, 3, 0, 0 },

	// Language
	{ "FEH_LAN", 33, 4,
		37, "FEL_ENG", ACTION_STANDARD, 28, 0, -72, 3, 0, 0,
		38, "FEL_GER", ACTION_STANDARD, 28, 0, 0, 3, 0, 0,
		39, "FEL_BRA", ACTION_STANDARD, 28, 0, 0, 3, 0, 0,
		//38, "FEL_SPA", ACTION_STANDARD, 28, 0, 0, 3, 0, 0,
		40, "FEL_POL", ACTION_STANDARD, 28, 0, 0, 3, 0, 0,
		41, "FEL_HUN", ACTION_STANDARD, 28, 0, 0, 3, 0, 0,
		MENUACTION_TOGGLE_LANGUAGE_6, "FEL_RON", ACTION_STANDARD, 28, 0, 0, 3, 0, 0,
		2, "FEDS_TB", ACTION_STANDARD, 4, 0, 0, 3, 0, 0 },


	{ "FET_SG", 1, 0,
		1, "FED_LWR", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEC_OKK", ACTION_STANDARD, 16, 0, 0, 0, 0, 0 },


	{ "FET_SG", 16, 0,
		1, "FEC_SVU", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEC_OKK", ACTION_STANDARD, 16, 0, 0, 0, 0, 0 },


	{ "FET_LG", 16, 0,
		1, "FEC_SVU", ACTION_NONE, 0, 0, 0, 0, 0, 0 },


	{ "FET_LG", 1, 0,
		1, "FEC_LUN", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		2, "FEDS_TB", ACTION_STANDARD, 1, 0, 0, 0, 0, 0 },

	// Options
	{ "FET_OPT", 42, 5,
		5, "FEO_CON", ACTION_STANDARD, 36, 0, -72, 3, 0, 0,
		5, "FEO_AUD", ACTION_STANDARD, 3, 0, 0, 3, 0, 0,
		5, "FEO_DIS", ACTION_STANDARD, 4, 0, 0, 3, 0, 0,
		5, "FEO_GFX", ACTION_STANDARD, 27, 0, 0, 3, 0, 0,
		5, "FEO_LAN", ACTION_STANDARD, 28, 0, 0, 3, 0, 0,
		5, "FEO_DLC", ACTION_STANDARD, 45, 0, 0, 3, 0, 0,
		5, "FEO_UPT", ACTION_STANDARD, 44, 0, 0, 3, 0, 0,
		2, "FEDS_TB", ACTION_STANDARD, 0, 0, 0, 3, 0, 0 },


	{ "FEM_MM", 255, 0,
		5, "FEP_STG", ACTION_STANDARD, 1, 0, -54, 3, 0, 0,
		5, "FEP_OPT", ACTION_STANDARD, 33, 0, 0, 3, 0, 0,
		5, "FEP_QUI", ACTION_STANDARD, 35, 0, 0, 3, 0, 0 },


	{ "FET_QG", 42, 6,
		1, "FEQ_SRE", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		55, "FEM_NO", ACTION_STANDARD, 42, 0, -9, 3, 0, 0,
		53, "FEM_YES", ACTION_STANDARD, 42, 0, 16, 3, 0, 0 },


	{ "FET_CTL", 33, 0,
		58, "FEC_CFG", ACTION_CLICKORARROWS, 36, 0, -74, 2, 0, 0,
		5, "FEC_RED", ACTION_STANDARD, 37, 0, -29, 3, 0, 0,
		7, "FEC_CFG", ACTION_JOYMOUSE, 36, 0, 0, 3, 0, 0,
		5, "FET_DEF", ACTION_STANDARD, 25, 0, 51, 3, 0, 0,
		2, "FEDS_TB", ACTION_STANDARD, 0, 0, 0, 3, 0, 0 },


	{ "FET_CTL", 36, 1,
		8, "FET_CFT", ACTION_STANDARD, 38, 0, -34, 3, 0, 0,
		9, "FET_CCR", ACTION_STANDARD, 38, 0, 0, 3, 0, 0,
		2, "FEDS_TB", ACTION_STANDARD, 36, 0, 46, 3, 0, 0 },


	{ "FET_CTL", 37, 0 },


	{ "FEM_MOU", 36, 2,
		62, "FEC_MSH", ACTION_CLICKORARROWS, 39, 0, -94, 2, 0, 0,
		46, "FEC_IVV", ACTION_CLICKORARROWS, 39, 0, 0, 2, 0, 0,
		59, "FET_MST", ACTION_CLICKORARROWS, 39, 0, 0, 2, 0, 0,
		60, "FET_MFL", ACTION_CLICKORARROWS, 39, 0, 0, 2, 0, 0,
		2, "FEDS_TB", ACTION_STANDARD, 0, 0, 66, 3, 0, 0 },


	{ "FEM_TIT", 36, 2,
		47, "FEJ_INX", ACTION_CLICKORARROWS, 40, 0, -94, 2, 0, 0,
		48, "FEJ_INY", ACTION_CLICKORARROWS, 40, 0, 0, 2, 0, 0,
		51, "FEJ_INA", ACTION_CLICKORARROWS, 40, 0, 0, 2, 0, 0,
		49, "FEJ_RNX", ACTION_CLICKORARROWS, 40, 0, 21, 2, 0, 0,
		50, "FEJ_RNY", ACTION_CLICKORARROWS, 40, 0, 0, 2, 0, 0,
		52, "FEJ_RNA", ACTION_CLICKORARROWS, 40, 0, 0, 2, 0, 0,
		2, "FEDS_TB", ACTION_STANDARD, 0, 0, 146, 3, 0, 0 },


	{ "FET_PAU", 255, 0,
		54, "FEP_RES", ACTION_STANDARD, 0, 0, -84, 3, 0, 0,
		5, "FEP_SGA", ACTION_STANDARD, 1, 0, 0, 3, 0, 0,
		5, "FEP_MAP", ACTION_STANDARD, 5, 0, 0, 3, 0, 0,
		5, "FEP_STA", ACTION_STANDARD, 0, 0, 0, 3, 0, 0,
		5, "FEP_BRI", ACTION_STANDARD, 2, 0, 0, 3, 0, 0,
		5, "FEP_OPT", ACTION_STANDARD, 33, 0, 0, 3, 0, 0,
		5, "FEP_QUI", ACTION_STANDARD, 35, 0, 0, 3, 0, 0 },


	// 2 menus serving a special purpose, cba moving
	{ "", 0, 0 },


	{ "", 0, 0 },


	// Game Updates
	{ "FEH_UPT", 33, 6,
		MENUACTION_CHECKING_PERIOD, "FEU_CHP", ACTION_CLICKORARROWS, 44, 0, 90, 2, -1, 0,
		MENUACTION_AUTOINSTALL_UPDATES, "FEU_AUI", ACTION_CLICKORARROWS, 44, 0, 0, 2, -1, 0,
		MENUACTION_UPDATER_BUTTON, "FEU_UPC", ACTION_UPDATER, 44, 0, 78, 3, 1, 0,
		2, "FEDS_TB", ACTION_STANDARD, 0, 0, 48, 3, 1, 0 },

	// Downloadable Content
	{ "FEH_DLC", 33, 5,
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
	{ "FEH_DLC", 33, 5,
		1, "FEE_NON", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEM_OK", ACTION_STANDARD, 33, 0, 16, 3, 0, 0 },

	// Downloadable Content - game restart required
	{ "FEH_DLC", 33, 5,
		1, "FEE_RES", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEM_OK", ACTION_STANDARD, 33, 0, 16, 3, 0, 0 },

	// Downloadable Content - DLC installation prompt
	{ "FEH_DLC", 45, 2,
		1, "FEE_IXX", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEM_NO", ACTION_STANDARD, 45, 0, -9, 3, 0, 0,
		57, "FEM_YES", ACTION_STANDARD, 44, 0, 16, 3, 0, 0 },

	// DLC activation
	{ "FEH_DLC", 45, 0,
		1, "FEE_KEY", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		MENUACTION_ACTIVATE_SERIAL, "FEE_ACT", ACTION_SERIAL, 0, 0, 78, 3, 1, 0,
		2, "FEDS_TB", ACTION_STANDARD, 0, 0, 48, 3, 1, 0 },
};

static inline const char* GetTitlePCByLanguage()
{
	static const char* const	cTitlePCNames[] = { "title_pc_EN", "title_pc_EN", /*"title_pc_ES",*/ "title_pc_PL", "title_pc_EN", "title_pc_EN" };
	return cTitlePCNames[FrontEndMenuManager.GetLanguage()];
}

WRAPPER void CMenuManager::SmallMessageScreen(const char* pMessage) { WRAPARG(pMessage); EAXJMP(0x574010); }
WRAPPER void CMenuManager::SwitchToNewScreen(signed char bScreen) { WRAPARG(bScreen); EAXJMP(0x573680); }

void CMenuManager::DrawBackEnd()
{
	// Calculate proper dimensions
	// Displayed image is 16:9
	CVector2D				vecSplashScale = WidescreenSupport::GetFullscreenImageDimensions(16.0f/9.0f, ScreenAspectRatio, false);
	
	CSprite2d::DrawRect(CRect(-5.0f, RsGlobal.MaximumHeight + 5.0f, RsGlobal.MaximumWidth + 5.0f, RsGlobal.MaximumHeight * 0.95f - 5.0f), CRGBA(7, 7, 7, 255));
	textures[13].Draw(CRect(-2.5f - (vecSplashScale.x-RsGlobal.MaximumWidth), RsGlobal.MaximumHeight * 0.95f, RsGlobal.MaximumWidth + 2.5f, -2.5f - (vecSplashScale.y-RsGlobal.MaximumHeight)), CRGBA(255, 255, 255, 255));

	if ( bCurrentScreen == 44 )
	{
		textures[14].Draw(CRect(_x(245.0f), _y(85.0f), _x(25.0f), _y(30.0f)), CRGBA(255, 255, 255, 255));
		CUpdateManager::ReportUpdaterScreenSeen();	// Wrong place
	}

#ifdef FANCY_FRONTEND_CONTROLLERS_TEST
	int					nColourCycle = CTimer::m_snTimeInMillisecondsPauseMode % 15000;
	const CRGBA			PinkColour(MENU_PINK_R, MENU_PINK_G, MENU_PINK_B);
	const CRGBA			CyanColour(MENU_INACTIVE_R, MENU_INACTIVE_G, MENU_INACTIVE_B);
	CRGBA				BlendColour;

	if ( nColourCycle < 5000 )
		BlendColour = PinkColour;
	else if ( nColourCycle < 7500 )
		BlendColour = Blend(PinkColour, 7500-nColourCycle, CyanColour, nColourCycle-5000);
	else if ( nColourCycle < 12500 )
		BlendColour = CyanColour;
	else
		BlendColour = Blend(PinkColour, nColourCycle-12500, CyanColour, 15000-nColourCycle);

	textures[17].Draw(CRect(_x(425.0f), _y(230.0f), _x(25.0f), _y(30.0f)), BlendColour);
	textures[16].Draw(CRect(_x(425.0f), _y(230.0f), _x(25.0f), _y(30.0f)), CRGBA(255, 255, 255, 255));
#endif

	CFont::SetBackground(0, 0);
	CFont::SetProportional(false);
	CFont::SetFontStyle(FONT_PagerFont);
	CFont::SetOrientation(ALIGN_Right);
	CFont::SetRightJustifyWrap(0.0);
	CFont::SetEdge(1);
	CFont::SetDropColor(CRGBA(0, 0, 0, 255));
	CFont::SetScale(_width(0.25f), _height(0.4f));
	CFont::SetColor(BaseColors[11]);
	CFont::PrintString(_x(2.5f), _ydown(13.0f), MOD_VERSION" BUILD "BUILDNUMBER_STR);

#ifdef DEVBUILD
	#ifdef MAKE_ZZCOOL_MOVIE_DEMO
		CFont::PrintString(_x(2.5f), _ydown(20.5f), "DEMONSTRATION BUILD");
	#else
		CFont::PrintString(_x(2.5f), _ydown(20.5f), "DEV BUILD");
	#endif
#else
	#ifdef COMPILE_RC
		CFont::PrintString(_x(2.5f), _ydown(20.5f), "RELEASE CANDIDATE "RELEASE_CANDIDATE);
	#else
		CFont::PrintString(_x(2.5f), _ydown(20.5f), VERSION_NAME_UPPERCASE);
	#endif
#endif

	if ( bCurrentScreen != 44 )
		CUpdateManager::Display();

#ifdef INCLUDE_PROMO_BANNER
	if ( bCurrentScreen == 33 || bCurrentScreen == 34 || bCurrentScreen == 41 )
	{
		int			nDaysTillDeadline = static_cast<int>(GetCurrentDate().GetSecondsLeft(CDate(21, 12, 2013, true))) / (60*60*24);
		if ( nDaysTillDeadline >= 0 )
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
			if ( nDaysTillDeadline >= 3 )
				CFont::SetColor(CRGBA(255, 255, 255, 255));

			CMessages::InsertNumberInString(TheText.Get(nDaysTillDeadline == 1 ? "FEP_DY2" : "FEP_DYZ"), nDaysTillDeadline, -1, -1, -1, -1, -1, gString);
			CFont::PrintString(_x(75.0f), _y(140.0f), gString);
		}
	}
#endif

	CFont::SetProportional(true);
	CFont::SetEdge(0);
}

void CMenuManager::DrawRadioStationIcons()
{
	BYTE	bLoopCounter = 1;

	float	fPosition = (0.5f*WidescreenSupport::GetScreenWidthMultiplier()) + 300.0f;
	static	DWORD LastTimeIconsWereUpdated = 0;

#if defined COMPILE_BOUNCING_ICONS || defined COMPILE_SMOOTHBEATING_ICONS || defined COMPILE_BEATING_ICONS
	static signed char	bLastRadioStation = -1;
	static double		fRadioStationBouncingAngle = 0.0;

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
	if ( bLastRadioStation != radioStation || currentMenuEntry != 4 )
	{
		fRadioStationBouncingAngle = -0.75 * M_PI;

		bLastRadioStation = radioStation;
	}
#endif

	do
	{
#if defined COMPILE_BOUNCING_ICONS
		textures[bLoopCounter].Draw(_x(fPosition), _y(290.0f - (20.0f * abs(static_cast<float>(sin(fRadioStationBouncingAngle))) * ( radioStation == bLoopCounter && fRadioStationBouncingAngle >= 0.0 ))), _width(60.0), _height(60.0), CRGBA(255, 255, 255, radioStation == bLoopCounter ? 255 : 30));
#elif defined COMPILE_SMOOTHBEATING_ICONS
		textures[bLoopCounter].Draw(_x(fPosition - (10.0f * abs(static_cast<float>(sin(fRadioStationBouncingAngle))) * ( radioStation == bLoopCounter && fRadioStationBouncingAngle >= 0.0 ))), _y(290.0 - (10.0 * abs(static_cast<float>(sin(fRadioStationBouncingAngle))) * ( radioStation == bLoopCounter && fRadioStationBouncingAngle >= 0.0 ))), _width(60.0 + (20.0 * abs(static_cast<float>(sin(fRadioStationBouncingAngle))) * ( radioStation == bLoopCounter && fRadioStationBouncingAngle >= 0.0 ))), _height(60.0 + (20.0 * abs(static_cast<float>(sin(fRadioStationBouncingAngle))) * ( radioStation == bLoopCounter && fRadioStationBouncingAngle >= 0.0 ))), CRGBA(255, 255, 255, radioStation == bLoopCounter ? 255 : 30));
#elif defined COMPILE_BEATING_ICONS
		textures[bLoopCounter].Draw(_x(fPosition + (10.0f * static_cast<float>(sin(fRadioStationBouncingAngle)) * ( radioStation == bLoopCounter && fRadioStationBouncingAngle >= 0.0 && fRadioStationBouncingAngle < M_PI ))), _ymiddle(66.0f - (10.0f * static_cast<float>(sin(fRadioStationBouncingAngle)) * ( radioStation == bLoopCounter && fRadioStationBouncingAngle >= 0.0 && fRadioStationBouncingAngle < M_PI ))), _width(60.0f + (20.0f * static_cast<float>(sin(fRadioStationBouncingAngle)) * ( radioStation == bLoopCounter && fRadioStationBouncingAngle >= 0.0 && fRadioStationBouncingAngle < M_PI ))), _height(60.0f + (20.0f * static_cast<float>(sin(fRadioStationBouncingAngle)) * ( radioStation == bLoopCounter && fRadioStationBouncingAngle >= 0.0 && fRadioStationBouncingAngle < M_PI ))), CRGBA(255, 255, 255, radioStation == bLoopCounter ? 255 : 30));
#else
		textures[bLoopCounter].Draw(_x(fPosition), _y(290.0f), _width(60.0f), _height(60.0f), CRGBA(255, 255, 255, radioStation == bLoopCounter ? 255 : 30));
#endif
		++bLoopCounter;
		fPosition -= 60.0f;
	}
	while ( bLoopCounter < 11 );
}

int CMenuManager::DisplaySlider(float posX, float posY, float, float height, float distBetweenRects, float filledAmount, int iWidth)
{
	BYTE			loopCounter = NUM_SLIDERS * 2;
	BYTE			positionCounter = 0;
	BYTE			secondPositionCounter = 0;
	bool			bDrawHalfSlider = false;
	float			fullWidth = 0.0;
	float			width = static_cast<float>(iWidth);

	do
	{
		float	itemPosX = distBetweenRects * positionCounter / NUM_SLIDERS + posX;
		float	mousePosX = distBetweenRects * secondPositionCounter / (NUM_SLIDERS * 2) + posX;
		CRGBA	colour;

		if ( static_cast<float>(positionCounter) / NUM_SLIDERS + ( 1 / (NUM_SLIDERS * 2) ) >= filledAmount )
			colour = CRGBA(MENU_INACTIVE_R, MENU_INACTIVE_G, MENU_INACTIVE_B, 0xFF);
		else
		{
			if ( static_cast<float>(secondPositionCounter) / (NUM_SLIDERS * 2) + ( 1 / (NUM_SLIDERS * 4)) >= filledAmount )
				bDrawHalfSlider = true;
			else
				colour = CRGBA(MENU_ACTIVE_R, MENU_ACTIVE_G, MENU_ACTIVE_B, 255);
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

	return static_cast<int>(fullWidth);
}

void CMenuManager::DrawLeftColumn(MenuItem::MenuEntry& pPosition, const char* pText, const char* pRightText)
{
	unsigned char	nEntrySpecialFlag = pPosition.specialDescFlag;
	float			fPosX, fPosY;

	if ( pPosition.screenHorAlign == -1 )
		fPosX = _xleft(pPosition.posX);
	else
	{
		if ( pPosition.screenHorAlign == 1 )
			fPosX = _x(pPosition.posX);
		else
			fPosX = _xmiddle(pPosition.posX);
	}

	fPosY = GetTextYPos(pPosition);

	if ( nEntrySpecialFlag >= ACTION_SAVE_1 && nEntrySpecialFlag <= ACTION_SAVE_12 && pRightText )
	{
		char		cSlotNumberText[4];
		sprintf(cSlotNumberText, "%d", nEntrySpecialFlag);

		CFont::PrintString(fPosX + _xleft(25.0f), fPosY, pText);

		CFont::SetOrientation(ALIGN_Right);
		CFont::PrintString(fPosX, fPosY, cSlotNumberText);
	}
	else
		CFont::PrintString(fPosX, fPosY, pText);
}

/*float CMenuManager::GetLeftColumnPos(MenuItem::MenuEntry& pPosition)
{
	if ( pPosition.screenHorAlign == -1 )
		return _xleft(pPosition.posX);

	if ( pPosition.screenHorAlign == 1 )
		return _x(pPosition.posX);

	return _xmiddle(pPosition.posX);
}

float CMenuManager::GetLeftColumnPos_Height(long posY)
{
	return _ymiddle(posY);
}*/

float CMenuManager::GetRightColumnPos(MenuVar& sPosY)
{
	/*float	fTemp = RsGlobal.MaximumWidth * ( bCurrentScreen == 9 || bCurrentScreen == 10 || bCurrentScreen == 16 ? 40.0 : MENU_TEXT_POSITION_RCOLUMN) / 853.0;
	return fTemp;*/
	if ( bCurrentScreen != 44 )
	{
		sPosY.fOut = _ymiddle(sPosY.nIn);
		return RsGlobal.MaximumWidth * ( bCurrentScreen == 9 || bCurrentScreen == 10 || bCurrentScreen == 16 ? 40.0f : 0.5f * WidescreenSupport::GetScreenWidthMultiplier() - MENU_TEXT_POSITION_RCOLUMN) * WidescreenSupport::GetScreenWidthDivider();
	}

	sPosY.fOut = _y(sPosY.nIn);
	return RsGlobal.MaximumWidth * ( 0.5f * WidescreenSupport::GetScreenWidthMultiplier() - MENU_TEXT_POSITION_RCOLUMN) * WidescreenSupport::GetScreenWidthDivider();
}

void CMenuManager::DrawOutroSplash()
{
	static DWORD	outroPageFrameCounter = 0;
	static int		outroTimer = 0;
	static WORD		outroPageAlpha = 0;
	static bool		bOutroSplashLoaded = false;
	static CRect	rectSpriteDimensions;

	if ( !bOutroSplashLoaded )
	{
		LoadSplashes(true, 0);

		CVector2D				vecSplashScale = WidescreenSupport::GetFullscreenImageDimensions(512.0f/400.0f, ScreenAspectRatio, true);

		rectSpriteDimensions.x1 = 0.5f * (RsGlobal.MaximumWidth - vecSplashScale.x);
		rectSpriteDimensions.y1 = 0.5f * (RsGlobal.MaximumHeight + vecSplashScale.y);
		rectSpriteDimensions.x2 = 0.5f * (RsGlobal.MaximumWidth + vecSplashScale.x);
		rectSpriteDimensions.y2 = 0.5f * (RsGlobal.MaximumHeight - vecSplashScale.y);

		bOutroSplashLoaded = true;
	}

	// TODO: Smooth when CTimer is rewritten
	if ( CTimer::m_snTimeInMillisecondsPauseMode - outroTimer > 15 )
	{
		if ( outroPageAlpha != 255 )
		{
			outroPageAlpha += 25;
			if ( outroPageAlpha > 255 )
				outroPageAlpha = 255;
		}
		else
			++outroPageFrameCounter;
		outroTimer = CTimer::m_snTimeInMillisecondsPauseMode;
	}

	CSprite2d::DrawRect(CRect(-5.0f, RsGlobal.MaximumHeight + 5.0f, RsGlobal.MaximumWidth + 5.0f, -5.0f), CRGBA(0, 0, 0, static_cast<BYTE>(outroPageAlpha)));
	LoadingSprites[0].Draw(rectSpriteDimensions, CRGBA(255, 255, 255, static_cast<BYTE>(outroPageAlpha)));
	if ( outroPageAlpha == 255 && outroPageFrameCounter == 90 )
		RsEventHandler(rsQUITAPP, nullptr);

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

		if ( fStatsScrollSpeed > 0.0 )
		{
			if ( bUnkScroll )
				m_fStatsScrollPos += (_height(100.0f) / fStatsScrollSpeed);
			else
				m_fStatsScrollPos -= (_height(100.0f) / fStatsScrollSpeed);
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

void CMenuManager::PrintUpdaterScreen()
{
	CFont::SetScale(_width(0.8f), _height(1.2f));
	CFont::SetFontStyle(FONT_RageItalic);
	CFont::SetDropColor(CRGBA(0, 0, 0, 255));
	CFont::SetEdge(1);
	CFont::SetOrientation(ALIGN_Center);

	CFont::SetColor(CRGBA(MENU_INACTIVE_R, MENU_INACTIVE_G, MENU_INACTIVE_B, 255));
	CFont::PrintString(_x(137.5f), _y(15.0f), TheText.Get("FEU_POW"));

	CFont::SetFontStyle(FONT_Eurostile);
	CFont::SetScale(_width(0.3f), _height(0.7f));
	CFont::SetColor(CRGBA(255, 255, 255, 255));
	CFont::SetOrientation(ALIGN_Left);

	float		fStartingPos = _height(155.0f);

	for ( int i = 0; i < NUM_MESSAGES_PER_UPT_SCREEN; ++i )
	{
		if ( const char* pLine = CUpdateManager::FetchMessage(i) )
		{
			if ( pLine[0] )
			{
				CFont::PrintString(_xleft(50.0f), fStartingPos, pLine);
				fStartingPos += _height(17.5f);
			}
			else
				break;
		}
		else
			break;
	}

	float		dDownloadPercentage = CUpdateManager::GetDownloadProgress();

	CSprite2d::DrawRect(CRect(_xmiddle(-269.0f), _ydown(89.0f), _xmiddle(271.0f), _ydown(109.0f)), CRGBA(0, 0, 0, 255));

	if ( dDownloadPercentage != 0.0f )
		CSprite2d::DrawRect(CRect(_xmiddle(-270.0f), _ydown(90.0f), _xmiddle(-270.0f + (5.4f * dDownloadPercentage)), _ydown(110.0f)), CRGBA(MENU_PINK_R, MENU_PINK_G, MENU_PINK_B, 255));

	if ( dDownloadPercentage != 100.0f )
		CSprite2d::DrawRect(CRect(_xmiddle(-270.0f + (5.4f * dDownloadPercentage)), _ydown(90.0f), _xmiddle(270.0f), _ydown(110.0f)), CRGBA(MENU_INACTIVE_PINK_R, MENU_INACTIVE_PINK_G, MENU_INACTIVE_PINK_B, 255));

	CFont::SetScale(_width(0.35f), _height(0.7f));
	CFont::SetFontStyle(FONT_PagerFont);
	CFont::SetOrientation(ALIGN_Center);
	if ( CUpdateManager::IsDownloading() )
	{
		CMessages::InsertNumberInString(TheText.Get("FEU_PRC"), static_cast<int>(dDownloadPercentage), -1, -1, -1, -1, -1, gString);
		CFont::PrintString(static_cast<float>(RsGlobal.MaximumWidth / 2), _ydown(105.0f), gString);
	}

}

void CMenuManager::PrintDLCScreen()
{
	if ( currentMenuEntry > 1 )
	{
		m_nFocusedDLC = CDLCManager::GetActiveDLCWithIndex(currentMenuEntry-2);

		bool		bThisDLCIsEnabled = CDLCManager::GetDLC(static_cast<eExpansionPack>(m_nFocusedDLC))->IsActive();
		bool		bThisDLCIsInstalled = CDLCManager::GetDLC(static_cast<eExpansionPack>(m_nFocusedDLC))->IsInstalled();
		if ( m_nLastFocusedDLC != m_nFocusedDLC )
		{
			m_nLastFocusedDLC = m_nFocusedDLC;

			// Load a video
			char			cVideoPath[64];
			const CRect		videoFrame(_x(250.0f), _ymiddle(140.0f), _x(30.0f), _ymiddle(-65.0f));
			_snprintf(cVideoPath, sizeof(cVideoPath), "movies\\dlc%d.bik", m_nFocusedDLC);

			CVideoPlayer::Release();
			CVideoPlayer::Create(cVideoPath, &videoFrame, false, !bThisDLCIsInstalled);
		}

		char		cGXTName[8];
		_snprintf(cGXTName, sizeof(cGXTName), "FEE_D%02d", m_nFocusedDLC);

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

void CMenuManager::PrintActivationScreen()
{
	if ( m_nDLCMessageTimer > CTimer::m_snTimeInMillisecondsPauseMode )
		SmallMessageScreen(m_pDLCMessage);
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
			SmallMessageScreen("LOADCOL");
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
#ifdef FANCY_FRONTEND_CONTROLLERS_TEST
									"ps3front", "ps3back",
#endif
											};

	static const char* const	frontendpcTexNames[] = {
									"mouse",
									"crosshair" };

	CPNGArchive			FrontendArchive("models\\frontend.spta");
	CPNGArchive			DLCArchive(CFileLoader::GetFrontendPath());

	// fronten1
	FrontendArchive.SetDirectory("fronten1");

	for ( int i = 0; i < sizeof(frontend1TexNames)/sizeof(const char*); ++i )
	{
		textures[i+1].SetTextureFromSPTA(FrontendArchive, frontend1TexNames[i]);
		textures[i+1].SetAddressing(rwTEXTUREADDRESSCLAMP);
	}

	// fronten2
	FrontendArchive.SetDirectory("fronten2");
	DLCArchive.SetDirectory("fronten2");

	for ( int i = 0; i < sizeof(frontend2TexNames)/sizeof(const char*); ++i )
	{
		if ( !textures[i+13].SetTextureFromSPTA(DLCArchive, frontend2TexNames[i]) )
			textures[i+13].SetTextureFromSPTA(FrontendArchive, frontend2TexNames[i]);
		textures[i+13].SetAddressing(rwTEXTUREADDRESSCLAMP);
	}

	// fronten_pc
	FrontendArchive.SetDirectory("fronten_pc");

	for ( int i = 0; i < sizeof(frontendpcTexNames)/sizeof(const char*); ++i )
	{
		textures[i+23].SetTextureFromSPTA(FrontendArchive, frontendpcTexNames[i]);
		textures[i+23].SetAddressing(rwTEXTUREADDRESSCLAMP);
	}

	FrontendArchive.CloseArchive();
	DLCArchive.CloseArchive();
}

void CMenuManager::SwitchToNewScreenVCS(signed char bScreen)
{
	if ( !bScreen )
		m_fStatsScrollPos = -120.0f;
	else if ( bScreen == 45 )
	{
		if ( CDLCManager::AnyDLCsAvailable() )
		{
			currentMenuEntry = 2;

			for ( int i = 0; i < NUM_DLC_PACKS; i++ )
				m_bLastDLCState[i] = CDLCManager::GetDLC(i)->IsActive();
		}
		else
		{
			bCurrentScreen = 46;
			currentMenuEntry = 1;
		}
	}
	else if ( bScreen == 46 )
		currentMenuEntry = 1;

	// Unload BINK and check DLC state
	if ( bLastScreen == 45 )
	{
		m_nLastFocusedDLC = -1;
		CVideoPlayer::Release();

		for ( int i = 0; i < NUM_DLC_PACKS; i++ )
		{
			if ( m_bLastDLCState[i] != CDLCManager::GetDLC(i)->IsActive() )
			{
				bCurrentScreen = 47;
				currentMenuEntry = 1;
				break;
			}
		}
	}

	// Focus on DLC entry when exiting from any DLC-oriented screen
	if ( bLastScreen == 46 || bLastScreen == 47 )
		currentMenuEntry = 5;

	// Automatically check for updates and mark focused DLC for installation
	if ( bScreen == 44 && bLastScreen == 48 )
	{
		CUpdateManager::SetDLCStatus(CDLCManager::GetDLC(m_nFocusedDLC)->GetName(), true);
		CUpdateManager::ForceUpdate();
	}

	// Correct the first entry
	if ( bScreen == 48 )
		_snprintf(MenuEntriesList[48].entryList[0].entry, sizeof(MenuEntriesList->entryList->entry), "FEE_I%02d", m_nFocusedDLC);

	// Clear serial code buffer
	if ( bLastScreen == 49 )
		ClearSerialsBuffer();

	if ( bScreen == 49 )
	{
		// Check for key's presence in the clipboard
		LookIntoClipboardForSerial();
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
	if ( CDLCManager::IsContactingWebsite() )
		return;

	if ( m_nDLCMessageTimer > CTimer::m_snTimeInMillisecondsPauseMode )
		return;
	else if ( m_nSwitchToThisAfterMessage != -1 )
	{
		SwitchToNewScreen(m_nSwitchToThisAfterMessage);
		m_nSwitchToThisAfterMessage = -1;
	}

	// Call SA UserInput
	((void(__thiscall*)(CMenuManager*))0x57FD70)(this);
}

void CMenuManager::MessageScreen(const char* pMessage, bool bFullscreen, bool bWithinFrame)
{
	if ( !bWithinFrame )
	{
		if ( !RsCameraBeginUpdate(Scene) )
			return;
	}

	CSprite2d::InitPerFrame();
	CFont::InitPerFrame();
	DefinedState2d();

	if ( bFullscreen )
	{
		RwRGBA		colour = { 0, 0, 0, 255 };
		RwCameraClear(Scene, &colour, rwCAMERACLEARIMAGE);
	}

	SmallMessageScreen(pMessage);
	CFont::RenderFontBuffer();

	if ( !bWithinFrame )
		DoRWStuffEndOfFrame();
}

void CMenuManager::AdditionalOptionInputVCS(unsigned char* pUp, unsigned char* pDown)
{
	// Call SA AdditionalOptionInput
	((void(__thiscall*)(CMenuManager*,unsigned char*,unsigned char*))0x5773D0)(this, pUp, pDown);

	if ( bCurrentScreen == 45 || bCurrentScreen == 46 )
	{
		if ( ControlsManager.GetIsKeyboardKeyJustDown(rsEND) )
		{
			AudioEngine.ReportFrontendAudioEvent(1, 0.0f, 1.0f);
			SwitchToNewScreen(49);  
		}
	}

	if ( bCurrentScreen == 49 )
	{
		// Listen for Ctrl + V
		if ( (ControlsManager.GetIsKeyboardKeyDown(rsLCTRL) || ControlsManager.GetIsKeyboardKeyDown(rsRCTRL))
			 && ControlsManager.GetIsKeyboardKeyJustDown(static_cast<RsKeyCodes>(0x56)) )
		{
			ClearSerialsBuffer();
			LookIntoClipboardForSerial();
		}
	}
}

void CMenuManager::TypingKeyboardInput(wchar_t wKey)
{
	// DLC activation screen input
	if ( bCurrentScreen == 49 )
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

const char* CMenuManager::ProcessDLCSlot(int nSlotID)
{
	int				nDLC = CDLCManager::GetActiveDLCWithIndex(nSlotID-2);
	static char		lastDLCName[8];

	if ( nDLC == -1 )
	{
		// No DLC on this slot, skip
		ms_pMenus[bCurrentScreen].entryList[nSlotID].action = 20;
		ms_pMenus[bCurrentScreen].entryList[nSlotID].posY = -32728;
		/*if ( nSlotID == 2 )
		{
			static bool		bChangedHelperText = false;
			if ( !bChangedHelperText )
			{
				bChangedHelperText = true;
				strncpy(ms_pMenus[bCurrentScreen].entryList[0].entry, "FEE_NON", 8);
			}
		}*/
		return nullptr;
	}

	ms_pMenus[bCurrentScreen].entryList[nSlotID].action = MENUACTION_TOGGLE_DLC;
	_snprintf(lastDLCName, sizeof(lastDLCName), "FEE_N%02d", nDLC);
	return lastDLCName;
}

float CMenuManager::GetTextYPos(const MenuItem::MenuEntry& pPosition)
{
	if ( pPosition.screenVertAlign == -1 )
		return _y(pPosition.posY);

	if ( pPosition.screenVertAlign == 1 )
		return _ydown(pPosition.posY);

	return _ymiddle(pPosition.posY);
}

float CMenuManager::GetTextYPosNextItem(const MenuItem::MenuEntry& pPosition)
{
	if ( pPosition.screenVertAlign == -1 )
		return _y(pPosition.posY + 26);

	if ( pPosition.screenVertAlign == 1 )
		return _ydown(pPosition.posY - 26);

	return _ymiddle(pPosition.posY + 26);
}

// TODO: CLoadingScreen
static unsigned char		bDrawingStyle;

void LoadSplashes(bool bIntroSplash, unsigned char nIntroSplashID)
{
	unsigned char		bTempIndexes[NUM_LOADING_SPLASHES], bFinalIndexes[NUM_LOADING_SPLASHES];
	LARGE_INTEGER		lPerformanceCount;

//	memset(bTempIndexes, 0, sizeof(bTempIndexes));
	for ( unsigned char i = 0; i < NUM_LOADING_SPLASHES; ++i )
		bTempIndexes[i] = i;

	CPNGArchive		LoadscsArchive("models\\txd\\loadscs.spta");
	LoadscsArchive.SetDirectory(nullptr);

	QueryPerformanceCounter(&lPerformanceCount);
	srand(lPerformanceCount.LowPart);

	for ( int i = 0, j = NUM_LOADING_SPLASHES-2; i < NUM_LOADING_SPLASHES; ++i, --j )
	{
		int		nRandomNumber;

		if ( i )
			nRandomNumber = static_cast<int>(rand() * (1.0f/(RAND_MAX+1.0f)) * (j+1));
		else
			nRandomNumber = 0;

		bFinalIndexes[i] = bTempIndexes[nRandomNumber];
		if ( nRandomNumber < j )
			memcpy(&bTempIndexes[nRandomNumber], &bTempIndexes[nRandomNumber+1], j - nRandomNumber);
	}

	for ( int i = 0; i < 7; ++i )
	{
		char		SplashName[20];
		bDrawingStyle = bIntroSplash != 0;
		if ( bIntroSplash )
		{
			if ( nIntroSplashID == 1 )
				strncpy(SplashName, "intro", sizeof(SplashName));
			else
				strncpy(SplashName, "outro", sizeof(SplashName));
		}
		else
		{
			if ( i )
				_snprintf(SplashName, sizeof(SplashName), "loadsc%d", bFinalIndexes[i]);
			else
				strncpy(SplashName, GetTitlePCByLanguage(), sizeof(SplashName));
		}
		LoadingSprites[i].SetTextureFromSPTA(LoadscsArchive, SplashName);
	}
	LoadscsArchive.CloseArchive();
}

void LoadingScreen()
{
	CSprite2d::InitPerFrame();
	RwRenderStateSet(rwRENDERSTATETEXTUREADDRESS, reinterpret_cast<void*>(rwTEXTUREADDRESSCLAMP));

	// TODO: Name these variables
	if ( *(bool*)0xBAB31C )
	{
		if ( bDrawingStyle == 1 )
		{
			// Logo (small)
			LoadingSprites[CurrentLoadingSprite].Draw(CRect(_xmiddle(-175.0f), _ymiddle(175.0f), _xmiddle(175.0f), _ymiddle(-175.0f)), CRGBA(255, 255, 255, 255));
		}
		else
		{
			if ( !CurrentLoadingSprite )
			{
				// title_pc
				CVector2D				vecSplashScale = WidescreenSupport::GetFullscreenImageDimensions(640.0f/448.0f, WidescreenSupport::SetAspectRatio(), true);

				LoadingSprites[CurrentLoadingSprite].Draw(CRect(0.5f * (RsGlobal.MaximumWidth - vecSplashScale.x), 0.5f * (RsGlobal.MaximumHeight + vecSplashScale.y), 0.5f * (RsGlobal.MaximumWidth + vecSplashScale.x), 0.5f * (RsGlobal.MaximumHeight - vecSplashScale.y)), CRGBA(255, 255, 255, 255));
			}
			else
			{
				// Regular
				LoadingSprites[CurrentLoadingSprite].Draw(CRect(-5.0f, RsGlobal.MaximumHeight + 5.0f, RsGlobal.MaximumWidth + 5.0f, -5.0f), CRGBA(255, 255, 255, 255));
			}
		}

		if ( *(bool*)0xBAB31E || *(bool*)0xBAB31F )
			CSprite2d::DrawRect(CRect(-5.0f, RsGlobal.MaximumHeight + 5.0f, RsGlobal.MaximumWidth + 5.0f, -5.0f), CRGBA(0, 0, 0, *(bool*)0xBAB31E ? 255 - *(unsigned char*)0xBAB320 : *(unsigned char*)0xBAB320));
		else
			LoadingSprites[CurrentLoadingSprite-1].Draw(CRect(-5.0f, RsGlobal.MaximumHeight + 5.0f, RsGlobal.MaximumWidth + 5.0f, -5.0f), CRGBA(255, 255, 255, 255 - *(unsigned char*)0xBAB320));
	}
	else
	{
		if ( !*(bool*)0xBAB33D )
			LoadingSprites[CurrentLoadingSprite].Draw(CRect(-5.0f, RsGlobal.MaximumHeight + 5.0f, RsGlobal.MaximumWidth + 5.0f, -5.0f), CRGBA(255, 255, 255, 255));
	}
}

static void __declspec(naked) AutoInstallUpdates_RightColumm()
{
	_asm
	{
		mov		cl, [CUpdateManager::bAutoInstallUpdates]
		test	cl, cl
		mov		ecx, [TheText]
		jz		AutoInstallUpdates_RightColumm_Off
		push	579FA6h
		retn

AutoInstallUpdates_RightColumm_Off:
		push	579F65h
		retn
	}
}

static void __declspec(naked) CheckEvery_RightColumn()
{
	static const char		aFem_Ch1[] = "FEU_CH1";
	static const char		aFem_Ch2[] = "FEU_CH2";
	static const char		aFem_Ch3[] = "FEU_CH3";
	static const char		aFem_Ch4[] = "FEU_CH4";
	_asm
	{
		mov		ecx, [TheText]
		mov		al, [CUpdateManager::bCheckingPeriod]
		test	al, al
		jz		CheckEvery_RightColumn_Ch1
		dec		al
		jz		CheckEvery_RightColumn_Ch2
		dec		al
		jz		CheckEvery_RightColumn_Ch3
		push	offset aFem_Ch4
		jmp		CheckEvery_RightColumn_Return

CheckEvery_RightColumn_Ch1:
		push	offset aFem_Ch1
		jmp		CheckEvery_RightColumn_Return

CheckEvery_RightColumn_Ch2:
		push	offset aFem_Ch2
		jmp		CheckEvery_RightColumn_Return

CheckEvery_RightColumn_Ch3:
		push	offset aFem_Ch3

CheckEvery_RightColumn_Return:
		push	57A161h
		retn
	}
}

static void __declspec(naked) UserInputArrowSoundMenus()
{
	_asm
	{
		jz		UserInputArrowSoundMenus_AllowMenu
		cmp     al, 39
		jz		UserInputArrowSoundMenus_AllowMenu
		cmp		al, 44
		jnz		UserInputArrowSoundMenus_False

UserInputArrowSoundMenus_AllowMenu:
		push	5805C0h
		retn

UserInputArrowSoundMenus_False:
		push	580600h
		retn
	}
}


static StaticPatcher	Patcher([](){
				static const BYTE			StandardMenusRightColumn_Table[] = {
								0x00, 0x01, 0x02, 0x1B, 0x1B, 0x1B, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
								0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x0A, 0x0B, 0x0C, 0x1B, 0x0D, 0x0E, 0x0F, 0x10,
								0x11, 0x12, 0x13, 0x1B, 0x1B, 0x1B, 0x14, 0x1B, 0x15, 0x16, 0x17, 0x1B, 0x1B,
								0x18, 0x19, 0x1A, 0x1B, 0x1B, 0x1B, 0x1B, 0x1C, 0x1D };

				static const void* const	StandardMenusRightColumn_Addresses[] = {
								(void*)0x579EF0, (void*)0x579DDE, (void*)0x579DE6,
								(void*)0x579DB9, (void*)0x579DB1, (void*)0x579DC1,
								(void*)0x579DD6, (void*)0x579E37, (void*)0x579E67,
								(void*)0x579DEE, (void*)0x579EAE, (void*)0x579EF8,
								(void*)0x57A0D3, (void*)0x579F57, (void*)0x579F6F,
								(void*)0x579F77, (void*)0x579F7F, (void*)0x579F87,
								(void*)0x579F8F, (void*)0x579F97, (void*)0x57A05A,
								(void*)0x57A141, (void*)0x579FB0, (void*)0x57A005,
								(void*)0x579E7A, (void*)0x579F4F, (void*)0x579E6F,
								(void*)0x57A168, AutoInstallUpdates_RightColumm, CheckEvery_RightColumn };

				Memory::Patch<BYTE>(0x579D9C, sizeof(StandardMenusRightColumn_Table)-1);
				Memory::Patch<const void*>(0x579DA6, StandardMenusRightColumn_Table);
				Memory::Patch<const void*>(0x579DAD, StandardMenusRightColumn_Addresses);

				static const BYTE			UserInputArrowSoundMenus_Table[] = {
								0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01,
								0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,
								0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
								0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
								0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01,
								0x00 };

				Memory::Patch<BYTE>(0x5805DC, sizeof(UserInputArrowSoundMenus_Table)-1);
				Memory::Patch<const void*>(0x5805E2, UserInputArrowSoundMenus_Table);

				Memory::InjectHook(0x5805BA, UserInputArrowSoundMenus, PATCH_JUMP);
			});

