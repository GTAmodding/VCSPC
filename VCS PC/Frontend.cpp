#include "StdAfx.h"

long			CMenuManager::ms_nRubberSlider;
float			CMenuManager::m_fStatsScrollPos;
int				CMenuManager::m_nFocusedDLC = -1;
int				CMenuManager::m_nLastFocusedDLC = -1;
bool			CMenuManager::m_bLastDLCState[NUM_DLC_PACKS];
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


	{ "FEH_DIS", 33, 2,
		27, "FED_BRI", ACTION_CLICKORARROWS, 4, 0, -97, 2, 0, 0,
		33, "MAP_LEG", ACTION_CLICKORARROWS, 4, 0, 0, 2, 0, 0,
		34, "FED_RDR", ACTION_CLICKORARROWS, 4, 0, 0, 2, 0, 0,
		35, "FED_HUD", ACTION_CLICKORARROWS, 4, 0, 0, 2, 0, 0,
		25, "FED_SUB", ACTION_CLICKORARROWS, 4, 0, 0, 2, 0, 0,
		65, "FED_GLS", ACTION_CLICKORARROWS, 4, 0, 0, 2, 0, 0,
		5, "FED_ADV", ACTION_STANDARD, 27, 0, 96, 3, 0, 0,
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


	{ "FEH_TIT", 3, 4,
		1, "FEA_SUB", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		63, "FEA_MPM", ACTION_CLICKORARROWS, 26, 0, -34, 2, 0, 0,
		64, "FEA_AMS", ACTION_CLICKORARROWS, 26, 0, 0, 2, 0, 0,
		6, "FEA_SCN", ACTION_STANDARD, 26, 0, 0, 3, 0, 0,
		2, "FEDS_TB", ACTION_STANDARD, 3, 0, 136, 3, 0, 0 },


	{ "FEH_DIS", 4, 3,
		61, "FEM_LOD", ACTION_CLICKORARROWS, 27, 0, -97, 2, 0, 0,
		24, "FEM_FRM", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		26, "FED_WIS", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		42, "FED_FXQ", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		43, "FED_MIP", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		44, "FED_AAS", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		56, "FED_RES", ACTION_CLICKORARROWS, 27, 0, 0, 2, 0, 0,
		2, "FEDS_TB", ACTION_STANDARD, 4, 0, 136, 3, 0, 0 },

	// Language
	{ "FEH_LAN", 33, 3,
		37, "FEL_ENG", ACTION_STANDARD, 28, 0, -72, 3, 0, 0,
		//38, "FEL_SPA", ACTION_STANDARD, 28, 0, 0, 3, 0, 0,
		38, "FEL_POL", ACTION_STANDARD, 28, 0, 0, 3, 0, 0,
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
	{ "FEH_UPT", 33, 5,
		ACTION_UPDATER_BUTTON, "FEU_UPC", ACTION_UPDATER, 44, 0, 78, 3, 1, 0,
		2, "FEDS_TB", ACTION_STANDARD, 0, 0, 48, 3, 1, 0 },

	// Downloadable Content
	{ "FEH_DLC", 33, 4,
		1, "FEE_HEA", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		2, "FEDS_TB", ACTION_STANDARD, 0, 0, 48, 3, 1, 0,	// Hacky hacky
		ACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 60, -70, 1, 0, -1,
		ACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 0, 0, 3, 0, 0,
		ACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 0, 0, 3, 0, 0,
		ACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 0, 0, 3, 0, 0,
		ACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 0, 0, 3, 0, 0,
		ACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 0, 0, 3, 0, 0,
		ACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 0, 0, 3, 0, 0,
		ACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 0, 0, 3, 0, 0,
		ACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 0, 0, 3, 0, 0,
		ACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 0, 0, 3, 0, 0,
		ACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 0, 0, 3, 0, 0,
		ACTION_TOGGLE_DLC, "FEE_NXX", ACTION_DLC, 45, 0, 0, 3, 0, 0 },

	// Downloadable Content - no DLCs available
	{ "FEH_DLC", 33, 4,
		1, "FEE_NON", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEM_OK", ACTION_STANDARD, 33, 0, 16, 3, 0, 0 },

	// Downloadable Content - game restart required
	{ "FEH_DLC", 33, 4,
		1, "FEE_RES", ACTION_NONE, 0, 0, 0, 0, 0, 0,
		5, "FEM_OK", ACTION_STANDARD, 33, 0, 16, 3, 0, 0 },
};

WRAPPER void CMenuManager::ShowFullscreenMessage(const char* pMessage, bool bUnk1, bool bUnk2)
	{ WRAPARG(pMessage); WRAPARG(bUnk1); WRAPARG(bUnk2); EAXJMP(0x579330); }

void CMenuManager::DrawBackEnd()
{
	/*CRect	coords(0.0, RsGlobal.MaximumHeight, RsGlobal.MaximumWidth, 0.0);
	RwRGBA		rgba(255, 255, 255, 255);
	RwRGBA		BorderColour(0, 0, 0, 255);*/

	//textures[13]->DrawTexturedRect(&coords, &rgba);
	textures[13].Draw(CRect(-1.0f, RsGlobal.MaximumHeight, RsGlobal.MaximumWidth, -1.0f), CRGBA(255, 255, 255, 255));

	if ( bCurrentScreen == 44 )
	{
		textures[14].Draw(CRect(_x(245.0f), _y(85.0f), _x(25.0f), _y(30.0f)), CRGBA(255, 255, 255, 255));
		CUpdateManager::ReportUpdaterScreenSeen();	// Wrong place
	}

	CFont::SetTextBackground(0, 0);
	CFont::SetTextUseProportionalValues(false);
	CFont::SetFontStyle(FONT_PagerFont);
	CFont::SetTextAlignment(ALIGN_Right);
	CFont::SetTextWrapX(0.0);
	CFont::SetTextOutline(1);
	CFont::SetTextBorderRGBA(CRGBA(0, 0, 0, 255));
	CFont::SetTextLetterSize(_width(0.25), _height(0.4));
	CFont::SetTextColour(CRGBA(BaseColors[11]));
	CFont::PrintString(_x(2.5), _ydown(13.0), MOD_VERSION" BUILD "BUILDNUMBER_STR);

#ifdef DEBUG
	#ifdef MAKE_ZZCOOL_MOVIE_DEMO
		CFont::PrintString(_x(2.5), _ydown(20.5), "DEMONSTRATION BUILD");
	#else
		CFont::PrintString(_x(2.5), _ydown(20.5), "DEV BUILD");
	#endif
#else
	#ifdef COMPILE_RC
		CFont::PrintString(_x(2.5), _ydown(20.5), "RELEASE CANDIDATE "RELEASE_CANDIDATE);
	#else
		CFont::PrintString(_x(2.5), _ydown(20.5), VERSION_NAME_UPPERCASE);
	#endif
#endif

	if ( bCurrentScreen != 44 )
		CUpdateManager::Display();

#ifdef INCLUDE_PROMO_BANNER
	if ( bCurrentScreen == 33 || bCurrentScreen == 34 || bCurrentScreen == 41 )
	{
		int			nDaysTillDeadline = static_cast<int>(GetCurrentDate().GetSecondsLeft(CDate(11, 12, 2013, true))) / (60*60*24);
		if ( nDaysTillDeadline >= 0 )
		{
			textures[16].Draw(CRect(_x(135.0f), _y(122.5f), _x(15.0f), _y(2.5f)), CRGBA(255, 255, 255, 255));

			CFont::SetTextUseProportionalValues(true);
			CFont::SetTextLetterSize(_width(0.8f), _height(1.2f));
			CFont::SetFontStyle(FONT_RageItalic);
			//CFont::SetTextBorderRGBA(CRGBA(0, 0, 0, 255));
			//CFont::SetTextOutline(1);
			CFont::SetTextAlignment(ALIGN_Center);

			CFont::SetTextColour(CRGBA(MODDB_RED_R, MODDB_RED_G, MODDB_RED_B, 255));
			CFont::PrintString(_x(75.0f), _y(115.0f), gxt->GetText("FEP_MOD"));

			CFont::SetTextUseProportionalValues(false);
			CFont::SetFontStyle(FONT_PagerFont);
			CFont::SetTextLetterSize(_width(0.375f), _height(0.725f));
			if ( nDaysTillDeadline >= 3 )
				CFont::SetTextColour(CRGBA(255, 255, 255, 255));

			CMessages::InsertNumberInString(gxt->GetText(nDaysTillDeadline == 1 ? "FEP_DY2" : "FEP_DYZ"), nDaysTillDeadline, -1, -1, -1, -1, -1, gString);
			CFont::PrintString(_x(75.0f), _y(140.0f), gString);
		}
	}
#endif

	CFont::SetTextUseProportionalValues(true);
	CFont::SetTextOutline(0);
}

void CMenuManager::DrawRadioStationIcons()
{
	BYTE	bLoopCounter = 1;

	float	fPosition = (0.5*WidescreenSupport::GetScreenWidthMultiplier()) + 300.0;
	static	DWORD LastTimeIconsWereUpdated = 0;

#if defined COMPILE_BOUNCING_ICONS || defined COMPILE_SMOOTHBEATING_ICONS || defined COMPILE_BEATING_ICONS
	static BYTE			bLastRadioStation = -1;
	static double		fRadioStationBouncingAngle = 0.0;

	//if ( (CTimer::m_snTimeInMillisecondsPauseMode - LastTimeIconsWereUpdated) > 32 )
	//{
		// TODO: Scratch the multiplir when CTimer is rewritten and fTimeStepPauseMode exists
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
		textures[bLoopCounter].Draw(_x(fPosition), _y(290.0 - (20.0 * abs(sin(fRadioStationBouncingAngle)) * ( radioStation == bLoopCounter && fRadioStationBouncingAngle >= 0.0 ))), _width(60.0), _height(60.0), CRGBA(255, 255, 255, radioStation == bLoopCounter ? 255 : 30));
#elif defined COMPILE_SMOOTHBEATING_ICONS
		textures[bLoopCounter].Draw(_x(fPosition - (10.0 * abs(sin(fRadioStationBouncingAngle)) * ( radioStation == bLoopCounter && fRadioStationBouncingAngle >= 0.0 ))), _y(290.0 - (10.0 * abs(sin(fRadioStationBouncingAngle)) * ( radioStation == bLoopCounter && fRadioStationBouncingAngle >= 0.0 ))), _width(60.0 + (20.0 * abs(sin(fRadioStationBouncingAngle)) * ( radioStation == bLoopCounter && fRadioStationBouncingAngle >= 0.0 ))), _height(60.0 + (20.0 * abs(sin(fRadioStationBouncingAngle)) * ( radioStation == bLoopCounter && fRadioStationBouncingAngle >= 0.0 ))), CRGBA(255, 255, 255, radioStation == bLoopCounter ? 255 : 30));
#elif defined COMPILE_BEATING_ICONS
		textures[bLoopCounter].Draw(_x(fPosition + (10.0 * sin(fRadioStationBouncingAngle) * ( radioStation == bLoopCounter && fRadioStationBouncingAngle >= 0.0 && fRadioStationBouncingAngle < M_PI ))), _ymiddle(66.0 - (10.0 * sin(fRadioStationBouncingAngle) * ( radioStation == bLoopCounter && fRadioStationBouncingAngle >= 0.0 && fRadioStationBouncingAngle < M_PI ))), _width(60.0 + (20.0 * sin(fRadioStationBouncingAngle) * ( radioStation == bLoopCounter && fRadioStationBouncingAngle >= 0.0 && fRadioStationBouncingAngle < M_PI ))), _height(60.0 + (20.0 * sin(fRadioStationBouncingAngle) * ( radioStation == bLoopCounter && fRadioStationBouncingAngle >= 0.0 && fRadioStationBouncingAngle < M_PI ))), CRGBA(255, 255, 255, radioStation == bLoopCounter ? 255 : 30));
#else
		textures[bLoopCounter].Draw(_x(fPosition), _y(290.0), _width(60.0), _height(60.0), CRGBA(255, 255, 255, radioStation == bLoopCounter ? 255 : 30));
#endif
		++bLoopCounter;
		fPosition -= 60.0;
	}
	while ( bLoopCounter < 11 );
}

int CMenuManager::DrawSliders(float posX, float posY, float, float height, float distBetweenRects, float filledAmount, int iWidth)
{
	BYTE			loopCounter = NUM_SLIDERS * 2;
	BYTE			positionCounter = 0;
	BYTE			secondPositionCounter = 0;
	bool			bDrawHalfSlider = false;
	float			fullWidth = 0.0;
	float			width = iWidth;

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
				colour = CRGBA(MENU_ACTIVE_R, MENU_ACTIVE_G, MENU_ACTIVE_B, 0xFF);
			fullWidth = mousePosX;
		}

		if ( loopCounter % 2 )
		{
			CSprite2d::DrawRect(CRect(itemPosX - _width(1.25), height + posY + _height(1.25), itemPosX + width + _width(1.25), posY - _height(1.25)), CRGBA(0, 0, 0, 0xFF));
			if ( !bDrawHalfSlider )
				CSprite2d::DrawRect(CRect(itemPosX, height + posY, width + itemPosX, posY), colour);
			else
			{
				CSprite2d::DrawRect(CRect(itemPosX, height + posY, width + itemPosX, posY), CRGBA(MENU_INACTIVE_R, MENU_INACTIVE_G, MENU_INACTIVE_B, 0xFF));
				CSprite2d::DrawRect(CRect(itemPosX + width * 0.25, height + posY - height * 0.25, width + itemPosX - width * 0.25, posY + height * 0.25), CRGBA(MENU_ACTIVE_R, MENU_ACTIVE_G, MENU_ACTIVE_B, 0xFF));
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

		CFont::SetTextAlignment(ALIGN_Right);
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
	sPosY.fOut = _ymiddle(sPosY.nIn);
	return RsGlobal.MaximumWidth * ( bCurrentScreen == 9 || bCurrentScreen == 10 || bCurrentScreen == 16 ? 40.0 : 0.5 * WidescreenSupport::GetScreenWidthMultiplier() - MENU_TEXT_POSITION_RCOLUMN) * WidescreenSupport::GetScreenWidthDivider();
}

void CMenuManager::DrawOutroSplash()
{
	static DWORD	outroPageFrameCounter = 0;
	static int		outroTimer = 0;
	static WORD		outroPageAlpha = 0;
	static bool		bOutroSplashLoaded = false;

	if ( !bOutroSplashLoaded )
	{
		CSprite2d::ReadLoadingTextures(true, 0);
		bOutroSplashLoaded = true;
	}

	// TODO: Smooth when CTimer is rewritten
	if ( CTimer::m_snTimeInMillisecondsPauseMode - outroTimer > 10 )
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
	loadingTextures[0].Draw(CRect(0.0, RsGlobal.MaximumHeight, RsGlobal.MaximumWidth, 0.0), CRGBA(255, 255, 255, outroPageAlpha));
	if ( outroPageAlpha == 255 && outroPageFrameCounter == 90 )
		RsEventHandler(rsQUITAPP, NULL);

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
				m_fStatsScrollPos += (_height(100.0) / fStatsScrollSpeed);
			else
				m_fStatsScrollPos -= (_height(100.0) / fStatsScrollSpeed);
		}
		nStatsTimer = CTimer::m_snTimeInMillisecondsPauseMode;
	}

	long	nIndents;
	DWORD	dwLoopCounter = 0;
	DWORD	dwStatsToShow = CStats::ConstructStatLine(99999, nIndents);

	CFont::SetTextLetterSize(_width(0.3), _height(0.7));
	CFont::SetFontStyle(FONT_Eurostile);

	while ( dwLoopCounter < dwStatsToShow )
	{
		float		fStartingPos = _height(39.0) * dwLoopCounter + _height(75.0) - m_fStatsScrollPos;

		while ( fStartingPos < _height(50.0) )
			fStartingPos += _height((dwStatsToShow + 7) * 39.0);

		if ( fStartingPos > _height(75.0) && fStartingPos < _ydown(100.0) )
		{
			float	fTextAlpha = 255.0;
			if ( fStartingPos < _height(100.0) )
				fTextAlpha = 10.2 * (fStartingPos - _height(75.0));
			else
			{
				if ( fStartingPos > _ydown(125.0) )
					fTextAlpha = 10.2 * (_ydown(100.0) - fStartingPos);
			}

			if ( fTextAlpha > 255.0f )
				fTextAlpha = 255.0f;

			CStats::ConstructStatLine(dwLoopCounter, nIndents);

			CFont::SetTextBorderRGBA(CRGBA(0, 0, 0, fTextAlpha));
			CFont::SetTextOutline(1);
			CFont::SetTextAlignment(ALIGN_Left);
			CFont::SetTextColour(CRGBA(255, 255, 255, fTextAlpha));
			CFont::PrintString(_xleft(50.0f + (nIndents * 3.0f)), fStartingPos, gString);
			CFont::SetTextAlignment(ALIGN_Right);
			CFont::PrintString(_x(50.0f), fStartingPos, gUString);
		}
		++dwLoopCounter;
	}
	// TODO: Criminal Rating
}

void CMenuManager::PrintUpdaterScreen()
{
	CFont::SetTextLetterSize(_width(0.8f), _height(1.2f));
	CFont::SetFontStyle(FONT_RageItalic);
	CFont::SetTextBorderRGBA(CRGBA(0, 0, 0, 255));
	CFont::SetTextOutline(1);
	CFont::SetTextAlignment(ALIGN_Center);

	CFont::SetTextColour(CRGBA(MENU_INACTIVE_R, MENU_INACTIVE_G, MENU_INACTIVE_B, 255));
	CFont::PrintString(_x(137.5), _y(15.0), gxt->GetText("FEU_POW"));

	CFont::SetFontStyle(FONT_Eurostile);
	CFont::SetTextLetterSize(_width(0.3), _height(0.7));
	CFont::SetTextColour(CRGBA(255, 255, 255, 255));
	CFont::SetTextAlignment(ALIGN_Left);

	float		fStartingPos = _height(100.0);

	for ( int i = 0; i < NUM_MESSAGES_PER_UPT_SCREEN; ++i )
	{
		if ( const char* pLine = CUpdateManager::FetchMessage(i) )
		{
			if ( pLine[0] )
			{
				CFont::PrintString(_xleft(50.0), fStartingPos, pLine);
				fStartingPos += _height(39.0);
			}
			else
				break;
		}
		else
			break;
	}

	float		dDownloadPercentage = CUpdateManager::GetDownloadProgress();

	CSprite2d::DrawRect(CRect(_xmiddle(-269.0), _ydown(89.0), _xmiddle(271.0), _ydown(109.0)), CRGBA(0, 0, 0, 0xFF));

	if ( dDownloadPercentage != 0.0 )
		CSprite2d::DrawRect(CRect(_xmiddle(-270.0), _ydown(90.0), _xmiddle(-270.0 + (5.4 * dDownloadPercentage)), _ydown(110.0)), CRGBA(MENU_PINK_R, MENU_PINK_G, MENU_PINK_B, 0xFF));

	if ( dDownloadPercentage != 100.0 )
		CSprite2d::DrawRect(CRect(_xmiddle(-270.0 + (5.4 * dDownloadPercentage)), _ydown(90.0), _xmiddle(270.0), _ydown(110.0)), CRGBA(MENU_INACTIVE_PINK_R, MENU_INACTIVE_PINK_G, MENU_INACTIVE_PINK_B, 0xFF));

	CFont::SetTextLetterSize(_width(0.35), _height(0.7));
	CFont::SetFontStyle(FONT_PagerFont);
	CFont::SetTextAlignment(ALIGN_Center);
	if ( CUpdateManager::IsDownloading() )
	{
		CMessages::InsertNumberInString(gxt->GetText("FEU_PRC"), static_cast<signed long>(dDownloadPercentage), -1, -1, -1, -1, -1, gString);
		CFont::PrintString(RsGlobal.MaximumWidth / 2, _ydown(105.0), gString);
	}

}

void CMenuManager::PrintDLCScreen()
{
	if ( currentMenuEntry > 1 )
	{
		m_nFocusedDLC = CDLCManager::GetActiveDLCWithIndex(currentMenuEntry-2);
		if ( m_nLastFocusedDLC != m_nFocusedDLC )
		{
			m_nLastFocusedDLC = m_nFocusedDLC;

			// Load a video
			char			cVideoPath[64];
			const CRect		videoFrame(_x(250.0f), _ymiddle(140.0f), _x(30.0f), _ymiddle(-65.0f));
			_snprintf(cVideoPath, sizeof(cVideoPath), "movies\\dlc%d.bik", m_nFocusedDLC);

			CVideoPlayer::Release();
			CVideoPlayer::Create(cVideoPath, &videoFrame, false);
		}

		char		cGXTName[8];
		bool		bThisDLCIsEnabled = CDLCManager::GetDLC(static_cast<eExpansionPack>(m_nFocusedDLC))->IsActive();
		_snprintf(cGXTName, sizeof(cGXTName), "FEE_D%02d", m_nFocusedDLC);

		CSprite2d::DrawRect(CRect(_x(251.5f), _ymiddle(195.5f), _x(26.5f), _ymiddle(-66.5f)), CRGBA(0, 0, 0, 255));
		CSprite2d::DrawRect(CRect(_x(252.5f), _ymiddle(194.5f), _x(27.5f), _ymiddle(-67.5f)), CRGBA(MENU_INACTIVE_PINK_R, MENU_INACTIVE_PINK_G, MENU_INACTIVE_PINK_B, 255));
		CVideoPlayer::PlayNextFrame();

		CFont::SetWrapx(_x(30.0f));
		CFont::SetTextColour(CRGBA(255, 255, 255, 255));
		CFont::SetTextShadow(1);
		CFont::SetTextBorderRGBA(CRGBA(0, 0, 0, 255));
		CFont::SetTextLetterSize(_width(0.35f), _height(0.7f));
		CFont::SetFontStyle(FONT_PagerFont);
		CFont::SetTextJustify(true);
		CFont::PrintString(_x(249.0f), _ymiddle(142.5f), gxt->GetText(cGXTName));

		if ( bThisDLCIsEnabled )
			CFont::SetTextColour(CRGBA(MENU_UPDATES_R, MENU_UPDATES_G, MENU_UPDATES_B, 255));
		CFont::SetTextLetterSize(_width(0.6f), _height(1.1f));
		CFont::SetTextJustify(false);
		CFont::SetTextAlignment(ALIGN_Center);
		CFont::PrintString(_x(140.0f), _ymiddle(-87.5f), gxt->GetText(bThisDLCIsEnabled ? "FEE_ON" : "FEE_OFF"));
	}
	else
	{
		CVideoPlayer::Release();
		m_nFocusedDLC = m_nLastFocusedDLC = -1;
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
									"banner" };

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

void CMenuManager::SwitchToNewScreen(signed char bScreen)
{
	if ( !bScreen )
		m_fStatsScrollPos = -120.0f;
	else
	{
		if ( bScreen == 45 )
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
		else
		{
			if ( bScreen == 46 )
				currentMenuEntry = 1;
		}
	}

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
		currentMenuEntry = 4;
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

	ms_pMenus[bCurrentScreen].entryList[nSlotID].action = ACTION_TOGGLE_DLC;
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

/*void CMenuManager::ToggleTrails(bool bEnable)
{
	static bool		bGotColormodAddresses = false;
	static DWORD	colormodAddresses[5];
	if ( !bGotColormodAddresses )
	{
		colormodAddresses[0] = *((DWORD*)0x7F86C5);
		colormodAddresses[1] = *((DWORD*)0x7F9711);
		colormodAddresses[2] = *((DWORD*)0x7F9789);
		colormodAddresses[3] = *((DWORD*)0x7F9A17);
		colormodAddresses[4] = *((DWORD*)0x704D1F);
		bGotColormodAddresses = true;
	}

	if ( bEnable )
	{
		patch(0x7F86C4, 0xE8, 1);
		patch(0x7F86C5, colormodAddresses[0], 4);
		nop(0x7F86C9, 4);

		patch(0x7F9710, 0xE8, 1);
		patch(0x7F9711, colormodAddresses[1], 4);
		nop(0x7F9715, 6);

		patch(0x7F9788, 0xE8, 1);
		patch(0x7F9789, colormodAddresses[2], 4);
		nop(0x7F978D, 6);

		patch(0x7F9A16, 0xE8, 1);
		patch(0x7F9A17, colormodAddresses[3], 4);
		nop(0x7F9A1B, 6);

		patch(0x704D1F, colormodAddresses[4], 4);
		nop(0x704D0B, 9);
	}
	else
	{
		patch(0x7F86C4, 0x68, 1);
		patch(0x7F86C5, 0x00C9C040, 4);
		patch(0x7F86C9, 0x4051FF50, 4);

		patch(0x7F9710, 0x68, 1);
		patch(0x7F9711, 0x00C9C040, 4);
		patch(0x7F9715, 0xFF108B50, 4);
		patch(0x7F9719, 0x4052, 2);

		patch(0x7F9788, 0x68, 1);
		patch(0x7F9789, 0x00C9C040, 4);
		patch(0x7F978D, 0xFF108B50, 4);
		patch(0x7F9791, 0x4052, 2);

		patch(0x7F9A16, 0x68, 1);
		patch(0x7F9A17, 0x00C9C040, 4);
		patch(0x7F9A1B, 0xFF108B50, 4);
		patch(0x7F9A1F, 0x4052, 2);

		patch(0x704D1F, 0xFFFFE92D, 4);
		patch(0x704D0B, 0x8D518CA0, 4);
		patch(0x704D0F, 0x74C08400, 4);
		patch(0x704D13, 0x12, 1);
	}
}*/


/*void CMenuManager::SaveStatsHTML()
{
	wchar_t		dateBuf[12];
	SYSTEMTIME	SystemTime;

	CStreaming::ChangeDirectoryToUserDir();
	GetLocalTime(&SystemTime);
	swprintf(dateBuf, L"%d/%d/%d", SystemTime.wDay, SystemTime.wMonth, SystemTime.wYear % 100);

	char*	MissionName;
	if ( latestMissionName[0] )
		MissionName = latestMissionName;
	else
		MissionName = "ITBEG";

	if ( FILE*	hFile = fopen("stats.html", "w") )
	{
		BYTE	UTF8Header[] = { 0xEF, 0xBB, 0xBF };
		fwrite(UTF8Header, 3, 1, hFile);

		char	multiByteBuf[512];

		fprintf(hFile, "<title>Grand Theft Auto Vice City Stories Stats</title><meta http-equiv=\"content-type\" content=\"text/html;charset=utf-8\" />\n");
		fprintf(hFile, "<body bgcolor=\"#000000\" leftmargin=\"10\" topmargin=\"10\" marginwidth=\"10\" marginheight=\"10\">\n");
		fprintf(hFile, "<table width=\"560\" align=\"center\" border=\"0\" cellpadding=\"5\" cellspacing=\"0\">\n");
		fprintf(hFile, "<tr align=\"center\" valign=\"top\"> \n");
		fprintf(hFile, "<td height=\"59\" colspan=\"2\" bgcolor=\"#000000\"><div align=\"center\"><font color=\"#FFFFFF\" size=\"5\" face=\"Arial, \n");
		fprintf(hFile, "Helvetica, sans-serif\">-------------------------------------------------------------------</font><font \n");
		fprintf(hFile, "size=\"5\" face=\"Arial, Helvetica, sans-serif\"><br>\n");
		fprintf(hFile, "<strong><font color=\"#FFFFFF\">GRAND THEFT AUTO VICE CITY STORIES ");
		wcstombs(multiByteBuf, L"Śtątystykię!", 512);
//		fputs(, hFile);
		fprintf(hFile, "%s</font></strong><br /><font\n", multiByteBuf wcsupr(CStats::SACharsToASCII(gxt->GetText("FEH_STA"), 0)));
/*		fprintf(hFile, "color=\"#FFFFFF\">-------------------------------------------------------------------</font></font></div></td> </tr>\n");
		fprintf(hFile, "<tr align=\"center\" valign=\"top\" bgcolor=\"#000000\">     <td height=\"22\" colspan=\"2\">&nbsp;</td>  </tr>\n");
		fprintf(hFile, "<tr align=\"center\" valign=\"top\" bgcolor=\"#000000\"> \n");
//		fwprintf(hFile, L"<td height=\"40\" colspan=\"2\"> <p><font color=\"#F0000C\" size=\"2\" face=\"Arial, Helvetica, sans-serif\"><strong><font color=\"#F0000C\" size=\"1\">%s: \n", CStats::SACharsToASCII(gxt->GetText("FES_DAT"), 0));
//		fwprintf(hFile, L"%s</font><br>        %s: </strong>", dateBuf, CStats::SACharsToASCII(gxt->GetText("FES_CMI"), 0));


		fclose(hFile);

	}
}*/