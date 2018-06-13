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
#include "CutsceneManager.h"
#include "Frontend.h"
#include "Weather.h"
#include "Clouds.h"
#include "Font.h"
#include "UserDisplay.h"
#include "PostEffects.h"
#include "TimeCycle.h"
#include "Breakable.h"
#include "Hud.h"
#include "Antennas.h"
#include "VisibilityPlugins.h"
#include "Cheat.h"
#include "gtamain.h"
#include "misc.h"

bool &CGame::bMissionPackGame = *(bool*)0xB72910;
int32 &CGame::currArea = *(int*)0xB72914;
int32 &CGame::currLevel = *(int*)0xB7290C;
char *CGame::aDatFile = (char*)0xB728EC;	// [32]

static Reversed Initialise_kill(0x53BC80, 0x53BCEF);
bool
CGame::Initialise(const char *fileName)
{
	char dlcname[256];

	DebugMenuInit();

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

static Reversed Init1_kill(0x5BF840, 0x5BF9CF);
bool
CGame::Init1(const char *fileName)
{
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
	CTxdStore::LoadTxd(particle, CFileLoader::GetParticlesPath());
	CTxdStore::AddRef(particle);
	CTxdStore::SetCurrentTxd(gameTxdSlot);
	CGameLogic__InitAtStartOfGame();
	CGangWars__InitAtStartOfGame();
	CConversations__Clear();
	CPedToPlayerConversations__Clear();
	CQuadTreeNode__InitPool();
	// if(!CPlantMgr__Initialise())	// not used in VCSPC
	// 	return false;
	if(!CCustomRoadsignMgr__Initialise())
		return false;
	CReferences__Init();
	CDebug__DebugInitTextBuffer();
	CTagManager__Init();
	CWeather::Init();
	CCover__Init();
	CCullZones__Init();
	COcclusion__Init();
	CCollision__Init();
	CBirds__Init();
	CEntryExitManager__Init();
	CStuntJumpManager__Init();
	CSetPieces__Init();
	CTheZones::Init();
	CUserDisplay::Init();
	CMessages__Init();
	CMessages__ClearAllMessagesDisplayedByGame(0);
	CVehicleRecording__Init();
	CRestart__Initialise();
	CWorld::Initialise();
	CAnimManager__Initialise();
	CCutsceneMgr::Initialise();
	CCarCtrl__Init();
	InitModelIndices();
	CModelInfo::Initialise();
	CPickups__Init();
	CTheCarGenerators__Init();
	CGarages__Init();
	CAudioZones__Init();
	CStreaming::InitImageList();
	// CStreaming__ReadIniFile();
	ThePaths.Init();
	ThePaths.AllocatePathFindInfoMem();
	CTaskSimpleFight__LoadMeleeData();
	CCheat::ResetCheats();
	g_fx.Init();	// Fx_c;

	CarPipeInit();

	return true;
}

static Reversed Init2_kill(0x5BA1A0, 0x5BA3FF);
bool
CGame::Init2(const char *fileName)
{
	LoadingScreen("Loading the Game", "Add Particles", 0);
	CTheZones::PostZoneCreation();
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

	CClouds::Init();
	CSpecialFX__Init();
	LoadingScreen(0, 0, 0);
	CRopes__Init();
	CWaterCannons__Init();
	// CBridge::Init();
	CPedGroups__Init();
	CInformFriendsEventQueue__Init();
	CInformGroupEventQueue__Init();
	LoadingScreen(0, 0, 0);
	CCredits::bCreditsGoing = false;
	CReplay::Init();
	//CShopping__Init();	// TODO: probably remove this
	LoadingScreen(0, 0, 0);
	return true;
}

static Reversed Init3_kill(0x5BA400, 0x5BA48F);
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

//static Reversed tmp_kill(0x439AF0, 0x439B1D);

static Reversed Process_kill(0x53BEE0, 0x53C23F);
void
CGame::Process(void)
{
	CPad::UpdatePads();
	g_LoadMonitor.BeginFrame();

	uint32 timer = CTimer::GetCurrentTimeInMilliseconds();
	CStreaming::Update();
	timer = CTimer::GetCurrentTimeInMilliseconds() - timer;
	CCutsceneMgr::Update();
	DebugMenuProcess();
	if(!CCutsceneMgr::ms_cutsceneProcessing && !CTimer::m_CodePause)
		FrontEndMenuManager.Process();
	CTheZones::Update();
	CCover__Update();
	CAudioZones__Update(false, TheCamera.GetCoords());
	CWindModifiers__Number = 0;

	if(!CTimer::m_UserPause && !CTimer::m_CodePause){
		// CSprite2d::SetRecipNearClip();	// no-op
		CSprite2d::InitPerFrame();
		CFont::InitPerFrame();
		CCheat::DoCheats();
		CClock::Update();
		CWeather::Update();
		CTheScripts::Process();
		// CCollision::Update();	// no-op
		ThePaths.UpdateStreaming(false);
		// CTrain::UpdateTrains();	// no-op
		CHeli::UpdateHelis();
		CDarkel::Update();
		CSkidmarks__Update();

		CAntennas::Update();

		CGlass__Update();
		CWanted::UpdateEachFrame();
		CCreepingFire__Update();
		CSetPieces__Update();
		gFireManager.Update();

		if(timer < 4){
			timer = CTimer::GetCurrentTimeInMilliseconds();
			CPopulation__Update(true);
			timer = CTimer::GetCurrentTimeInMilliseconds() - timer;
		}else
			CPopulation__Update(false);

		CWeapon::UpdateWeapons();
		if(!CCutsceneMgr::ms_running)
			CTheCarGenerators__Process();
		// if(CReplay::Mode != 1)
		// 	CCranes::UpdateCranes();	// no-op
		CClouds::Update();
		CMovingThings__Update();
		CWaterCannons__Update();
		CUserDisplay::Process();
		CReplay::Update();
		CWorld::Process();
		g_LoadMonitor.EndFrame();

		if(!CTimer::bSkipProcessThisFrame){
			CPickups__Update();
			CCarCtrl__PruneVehiclesOfInterest();
			CGarages::Update();
			CEntryExitManager__Update();
			CStuntJumpManager__Update();
			CBirds__Update();

			CRubbish::Update();

			CSpecialFX__Update();
			CRopes__Update();
		}

		CPostEffects::Update();
		CTimeCycle::Update();
		CPopCycle__Update();

		
		if(*((uint8*)&g_InterestingEvents + 0x12c) & 1 )
			g_InterestingEvents.ScanForNearbyEntities();

		if(CReplay::ShouldStandardCameraBeProcessed())
			TheCamera.Process();
		else
			TheCamera.ProcessFade();

		CCullZones__Update();

		if(CReplay::Mode != 1){
			CGameLogic__Update();
			CGangWars__Update();
		}

		CConversations__Update();
		CPedToPlayerConversations__Update();
		// CBridge::Update();	// no-op

		CCoronas::DoSunAndMoon();
		CCoronas::Update();
		CShadows::UpdatePermanentShadows();

		// not used in VCSPC
		// CPlantMgr::Update(TheCamera.GetCoords());
		// CCustomBuildingRenderer::Update();

		// stencil shadows also not used
		// [skipped]

		if(CReplay::Mode != 1){
			if(timer < 4)
				CCarCtrl__GenerateRandomCars();
			CRoadBlocks__GenerateRoadBlocks();
			CCarCtrl__RemoveDistantCars();
			CCarCtrl__RemoveCarsIfThePoolGetsFull();
		}

		float t = CTimer::ms_fTimeStep / 50.0f;
		Fx_c::Update(TheCamera.m_pRwCamera, t);
		g_breakMan.Update(CTimer::ms_fTimeStep);
		g_interiorMan.Update();
		g_procObjMan.Update();
		g_waterCreatureMan.Update(t);

		CEmpireManager::Process();
	}
    PatchStats();

	CWaterLevel::PreRenderWater();
}

static StaticPatcher Patcher([](){
	Memory::InjectHook(0x53E58E, CGame::Initialise);
});
