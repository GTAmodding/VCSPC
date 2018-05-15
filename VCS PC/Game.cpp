#include "StdAfx.h"
#include "Game.h"

#include "Camera.h"
#include "Script.h"
#include "ColAccel.h"
#include "NeoCarpipe.h"
#include "TxdStore.h"
#include "Streaming.h"
#include "FxSystem.h"
#include "Rubbish.h"
#include "Pad.h"
#include "RealTimeShadowMgr.h"
#include "World.h"
#include "WaterLevel.h"
#include "Coronas.h"
#include "Shadows.h"
#include "Darkel.h"
#include "Renderer.h"
#include "Radar.h"
#include "Gangs.h"
#include "Clock.h"
#include "Stats.h"
#include "Replay.h"
#include "Garages.h"

bool &CGame::bMissionPackGame = *(bool*)0xB72910;
int32 &CGame::currArea = *(int*)0xB72914;
int32 &CGame::currLevel = *(int*)0xB7290C;
char *CGame::aDatFile = (char*)0xB728EC;	// [32]

// something d3d related
WRAPPER void sub_70F9E0(void) { EAXJMP(0x70F9E0); }
WRAPPER void LoadingScreen(const char *str1, const char *str2, const char *unused) { EAXJMP(0x53DED0); }

bool
CGame::Initialise(const char *fileName)
{
	char dlcname[256];

	CGame::Init1(fileName);
	CColAccel::startCache();
	// DLC support
	CFileLoader::ParseLevelFile("DATA\\DEFAULT.DAT", dlcname);
	CFileLoader::ParseLevelFile(fileName, dlcname);
	CFileLoader::LoadLevels();
	CColAccel::endCache();
	CGame::Init2(fileName);
	sub_70F9E0();
	LoadingScreen("Loading the Game", "Start script", NULL);
	CTheScripts::StartTestScript();
	CTheScripts::Process();
	TheCamera.Process();
	CGame::Init3(fileName);
	return true;
}

int &gameTxdSlot = *(int*)0xB728E8;

WRAPPER void CMaths__InitMathsTables(void) { EAXJMP(0x59AC90); }
WRAPPER void CPlaceable__InitMatrixArray(void) { EAXJMP(0x54F3A0); }
WRAPPER void CIniFile__LoadIniFile(void) { EAXJMP(0x56D070); }
WRAPPER void dummy_730AC0(char b) { EAXJMP(0x730AC0); }
WRAPPER void CGameLogic__InitAtStartOfGame(void) { EAXJMP(0x441210); }
WRAPPER void CGangWars__InitAtStartOfGame(void) { EAXJMP(0x443920); }
WRAPPER void CConversations__Clear(void) { EAXJMP(0x43A7B0); }
WRAPPER void CPedToPlayerConversations__Clear(void) { EAXJMP(0x43AAE0); }
WRAPPER void CQuadTreeNode__InitPool(void) { EAXJMP(0x552C00); }
WRAPPER bool CPlantMgr__Initialise(void) { EAXJMP(0x5DD910); }
WRAPPER bool CCustomRoadsignMgr__Initialise(void) { EAXJMP(0x6FE120); }
WRAPPER void CReferences__Init(void) { EAXJMP(0x5719B0); }
WRAPPER void CDebug__DebugInitTextBuffer(void) { EAXJMP(0x532240); }
WRAPPER void CTagManager__Init(void) { EAXJMP(0x49CC50); }
WRAPPER void CWeather__Init(void) { EAXJMP(0x72A480); }
WRAPPER void CCover__Init(void) { EAXJMP(0x698710); }
WRAPPER void CCullZones__Init(void) { EAXJMP(0x72D6B0); }
WRAPPER void COcclusion__Init(void) { EAXJMP(0x71DCA0); }
WRAPPER void CCollision__Init(void) { EAXJMP(0x416260); }
WRAPPER void CBirds__Init(void) { EAXJMP(0x711EC0); }
WRAPPER void CEntryExitManager__Init(void) { EAXJMP(0x43F880); }
WRAPPER void CEntryExitManager__PostEntryExitsCreation(void) { EAXJMP(0x43F0A0); }
WRAPPER void CStuntJumpManager__Init(void) { EAXJMP(0x49CA50); }
WRAPPER void CSetPieces__Init(void) { EAXJMP(0x4994F0); }
WRAPPER void CTheZones__Init(void) { EAXJMP(0x572670); }
void CTheZones__PostZoneCreation(void) { }
WRAPPER void CUserDisplay__Init(void) { EAXJMP(0x571EE0); }
WRAPPER void CMessages__Init(void) { EAXJMP(0x69EE00); }
WRAPPER void CMessages__ClearAllMessagesDisplayedByGame(bool b) { EAXJMP(0x69EDC0); }
WRAPPER void CVehicleRecording__Init(void) { EAXJMP(0x459390); }
WRAPPER void CRestart__Initialise(void) { EAXJMP(0x460630); }
WRAPPER void CAnimManager__Initialise(void) { EAXJMP(0x5BF6B0); }
WRAPPER void CAnimManager__LoadAnimFiles(void) { EAXJMP(0x4D5620); }
WRAPPER void CCutsceneMgr__Initialise(void) { EAXJMP(0x4D5A20); }
WRAPPER void CCarCtrl__Init(void) { EAXJMP(0x4212E0); }
WRAPPER void CPickups__Init(void) { EAXJMP(0x454A70); }
WRAPPER void CTheCarGenerators__Init(void) { EAXJMP(0x6F3270); }
WRAPPER void CGarages__Init(void) { EAXJMP(0x447120); }
WRAPPER void CAudioZones__Init(void) { EAXJMP(0x5081A0); }
WRAPPER void CStreaming__InitImageList(void) { EAXJMP(0x4083C0); }
WRAPPER void CStreaming__ReadIniFile(void) { EAXJMP(0x5BCCD0); }
WRAPPER void CTaskSimpleFight__LoadMeleeData(void) { EAXJMP(0x5BEDC0); }
WRAPPER void CCheat__ResetCheats(void) { EAXJMP(0x438450); }

WRAPPER bool CCustomCarPlateMgr__Initialise(void) { EAXJMP(0x6FD500); }
WRAPPER void CRoadBlocks__Init(void) { EAXJMP(0x461100); }
WRAPPER void CPopulation__Initialise(void) { EAXJMP(0x610E10); }
WRAPPER void CWeaponEffects__Init(void) { EAXJMP(0x742AB0); }
WRAPPER void CSkidmarks__Init(void) { EAXJMP(0x7204E0); }
WRAPPER void CGlass__Init(void) { EAXJMP(0x71A8D0); }
WRAPPER void CMovingThings__Init(void) { EAXJMP(0x717D00); }
WRAPPER void CClouds__Init(void) { EAXJMP(0x7138D0); }	// TODO: make a class
WRAPPER void CSpecialFX__Init(void) { EAXJMP(0x7268F0); }
WRAPPER void CRopes__Init(void) { EAXJMP(0x555DC0); }
WRAPPER void CWaterCannons__Init(void) { EAXJMP(0x728C80); }
WRAPPER void CPedGroups__Init(void) { EAXJMP(0x5FB8C0); }
WRAPPER void CInformFriendsEventQueue__Init(void) { EAXJMP(0x4B2990); }
WRAPPER void CInformGroupEventQueue__Init(void) { EAXJMP(0x4B2AD0); }
WRAPPER void CShopping__Init(void) { EAXJMP(0x49C290); }

bool &CCredits__bCreditsGoing = *(bool*)0xC6E97C;

class CPathFind
{
public:
	void Init(void);
	void AllocatePathFindInfoMem(void) { }
	void PreparePathData(void) { }
};
WRAPPER void CPathFind::Init(void) { EAXJMP(0x44D080); }
CPathFind &ThePaths = *(CPathFind*)0x96F050;

class InteriorManager_c
{
public:
	void Init(void);
};
WRAPPER void InteriorManager_c::Init(void) { EAXJMP(0x5C0500); }
InteriorManager_c &g_interiorMan = *(InteriorManager_c*)0xBAF670;

class ProcObjectMan_c
{
public:
	void Init(void);
};
WRAPPER void ProcObjectMan_c::Init(void) { EAXJMP(0x5A3EA0); }
ProcObjectMan_c &g_procObjMan = *(ProcObjectMan_c*)0xBAF670;

class WaterCreatureManager_c
{
public:
	void Init(void);
};
WRAPPER void WaterCreatureManager_c::Init(void) { EAXJMP(0x6E3F90); }
WaterCreatureManager_c &g_waterCreatureMan = *(WaterCreatureManager_c*)0xC1DF30;

bool
CGame::Init1(const char *fileName)
{
	//EAXJMP(0x5BF840);

	CMaths__InitMathsTables();
	strcpy(CGame::aDatFile, fileName);
	CPools::Initialise();
	CPlaceable__InitMatrixArray();
	// CIniFile__LoadIniFile();
	dummy_730AC0(0);
	CGame::currLevel = 0;
	CGame::currArea = 0;
	gameTxdSlot = CTxdStore::AddTxdSlot("generic");
	CTxdStore::Create(gameTxdSlot);
	CTxdStore::AddRef(gameTxdSlot);
	int particle = CTxdStore::AddTxdSlot("particle");
	CTxdStore::LoadTxd(particle, "MODELS\\PARTICLE.TXD");
	CTxdStore::AddRef(particle);
	CTxdStore::SetCurrentTxd(gameTxdSlot);
	CGameLogic__InitAtStartOfGame();
	CGangWars__InitAtStartOfGame();
	CConversations__Clear();
	CPedToPlayerConversations__Clear();
	CQuadTreeNode__InitPool();
	if(!CPlantMgr__Initialise() || !CCustomRoadsignMgr__Initialise())
		return false;
	CReferences__Init();
	CDebug__DebugInitTextBuffer();
	CTagManager__Init();
	CWeather__Init();
	CCover__Init();
	CCullZones__Init();
	COcclusion__Init();
	CCollision__Init();
	CBirds__Init();
	CEntryExitManager__Init();
	CStuntJumpManager__Init();
	CSetPieces__Init();
	CTheZones__Init();
	CUserDisplay__Init();
	CMessages__Init();
	CMessages__ClearAllMessagesDisplayedByGame(0);
	CVehicleRecording__Init();
	CRestart__Initialise();
	CWorld::Initialise();
	CAnimManager__Initialise();
	CCutsceneMgr__Initialise();
	CCarCtrl__Init();
	InitModelIndices();
	CModelInfo::Initialise();
	CPickups__Init();
	CTheCarGenerators__Init();
	CGarages__Init();
	CAudioZones__Init();
	CStreaming__InitImageList();
	// CStreaming__ReadIniFile();
	ThePaths.Init();
	ThePaths.AllocatePathFindInfoMem();
	CTaskSimpleFight__LoadMeleeData();
	CCheat__ResetCheats();
	g_fx.Init();	// Fx_c;

	CarPipeInit();

	return true;
}

bool
CGame::Init2(const char *fileName)
{
	LoadingScreen("Loading the Game", "Add Particles", 0);
	CTheZones__PostZoneCreation();
	CEntryExitManager__PostEntryExitsCreation();
	LoadingScreen("Loading the Game", "Setup paths", 0);
	ThePaths.PreparePathData();
	for(int i = 0; i < 2; i++)
		CWorld::Players[i].Clear();
	// CPlayerInfo::LoadPlayerSkin(CWorld::Players);	// no skins
	// TestModelIndices();	// no-op anyway
	LoadingScreen("Loading the Game", "Setup water", 0);
	// CClothes::Init();	// no clothes in VCS
	CWaterLevel::WaterLevelInitialise();
	CDraw::SetFOV(120.0f);
	CDraw::ms_fLODDistance = 500.0f;
	if(!CCustomCarPlateMgr__Initialise())
		return false;

	LoadingScreen("Loading the Game", "Setup streaming", 0);
	CStreaming::LoadInitialVehicles();
	CStreaming::LoadInitialPeds();
	CStreaming::LoadAllRequestedModels(0);
	LoadingScreen("Loading the Game", "Load animations", 0);
	CAnimManager__LoadAnimFiles();
	LoadingScreen("Loading the Game", "Setup streaming", 0);
	CStreaming::LoadInitialWeapons();
	CStreaming::LoadAllRequestedModels(0);
	LoadingScreen("Loading the Game", "Ped Init", 0);
	CPed::Initialise();
	CRenderer::Init();
	LoadingScreen("Loading the Game", "Setup game variables", 0);
	CRadar::Initialise();
	CRadar::LoadTextures();
	LoadingScreen(0, 0, 0);
	CWeapon::InitialiseWeapons();
	CRoadBlocks__Init();
	LoadingScreen(0, 0, 0);
	CPopulation__Initialise();
	LoadingScreen(0, 0, 0);
	CWorld::PlayerInFocus = 0;
	CCoronas::Init();
	CShadows::Init();
	LoadingScreen(0, 0, 0);
	CWeaponEffects__Init();
	CSkidmarks__Init();
	CGlass__Init();
	CGarages::Init_AfterRestart();
	LoadingScreen("Loading the Game", "Load scripts", 0);
	CTheScripts::Init();
	CGangs::Initialise();
	LoadingScreen("Loading the Game", "Setup game variables", 0);
	CClock::Initialise(1000);
	CHeli::InitHelis();
	// CCranes::InitCranes();
	LoadingScreen(0, 0, 0);
	CMovingThings__Init();
	CDarkel::Init();
	LoadingScreen(0, 0, 0);
	// CGridRef::Init();
	CStats::Init();

	CRubbish::Init();

	CClouds__Init();
	CSpecialFX__Init();
	LoadingScreen(0, 0, 0);
	CRopes__Init();
	CWaterCannons__Init();
	// CBridge::Init();
	CPedGroups__Init();
	CInformFriendsEventQueue__Init();
	CInformGroupEventQueue__Init();
	LoadingScreen(0, 0, 0);
	CCredits__bCreditsGoing = 0;
	CReplay::Init();
	CShopping__Init();	// TODO: probably remove this
	LoadingScreen(0, 0, 0);
	return true;
}

bool
CGame::Init3(const char *fileName)
{
	LoadingScreen("Loading the Game", "Load scene", NULL);
	CPad::GetPad(0)->Clear(true, true);
	CPad::GetPad(1)->Clear(true, true);
	dummy_730AC0(1);
	LoadingScreen("Loading the Game", "Procedural Interiors", NULL);
	g_interiorMan.Init();
	g_procObjMan.Init();
	g_waterCreatureMan.Init();
	g_realTimeShadowMan.Init();
	return true;
}

static StaticPatcher Patcher([](){
	Memory::InjectHook(0x53E58E, CGame::Initialise);
});
