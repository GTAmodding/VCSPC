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
#include "Font.h"
#include "UserDisplay.h"
#include "PostEffects.h"
#include "TimeCycle.h"
#include "Breakable.h"
#include "Hud.h"
#include "Antennas.h"
#include "VisibilityPlugins.h"
#include "Scene.h"
#include "debugmenu_public.h"

bool &CGame::bMissionPackGame = *(bool*)0xB72910;
int32 &CGame::currArea = *(int*)0xB72914;
int32 &CGame::currLevel = *(int*)0xB7290C;
char *CGame::aDatFile = (char*)0xB728EC;	// [32]

// something d3d related
WRAPPER void sub_70F9E0(void) { EAXJMP(0x70F9E0); }
WRAPPER void LoadingScreen(const char *str1, const char *str2, const char *unused) { EAXJMP(0x53DED0); }

void (*DebugMenuProcess)(void);
void (*DebugMenuRender)(void);
static void stub(void) { }

static Reversed Initialise_kill(0x53BC80, 0x53BCEF);
bool
CGame::Initialise(const char *fileName)
{
	char dlcname[256];

	if(DebugMenuLoad()){
		DebugMenuProcess = (void(*)(void))GetProcAddress(gDebugMenuAPI.module, "DebugMenuProcess");
		DebugMenuRender = (void(*)(void))GetProcAddress(gDebugMenuAPI.module, "DebugMenuRender");
	}else{
		DebugMenuProcess = stub;
		DebugMenuRender = stub;
	}

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
WRAPPER void CMessages__Init(void) { EAXJMP(0x69EE00); }
WRAPPER void CMessages__ClearAllMessagesDisplayedByGame(bool b) { EAXJMP(0x69EDC0); }
WRAPPER void CVehicleRecording__Init(void) { EAXJMP(0x459390); }
WRAPPER void CRestart__Initialise(void) { EAXJMP(0x460630); }
WRAPPER void CAnimManager__Initialise(void) { EAXJMP(0x5BF6B0); }
WRAPPER void CAnimManager__LoadAnimFiles(void) { EAXJMP(0x4D5620); }
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
	void UpdateStreaming(bool);
	void AllocatePathFindInfoMem(void) { }
	void PreparePathData(void) { }
};
WRAPPER void CPathFind::Init(void) { EAXJMP(0x44D080); }
WRAPPER void CPathFind::UpdateStreaming(bool) { EAXJMP(0x450A60); }
CPathFind &ThePaths = *(CPathFind*)0x96F050;

class InteriorManager_c
{
public:
	void Init(void);
	void Update(void);
};
WRAPPER void InteriorManager_c::Init(void) { EAXJMP(0x5C0500); }
WRAPPER void InteriorManager_c::Update(void) { EAXJMP(0x598F50); }
InteriorManager_c &g_interiorMan = *(InteriorManager_c*)0xBAF670;

class ProcObjectMan_c
{
public:
	void Init(void);
	void Update(void) { }
};
WRAPPER void ProcObjectMan_c::Init(void) { EAXJMP(0x5A3EA0); }
ProcObjectMan_c &g_procObjMan = *(ProcObjectMan_c*)0xBAF670;

class WaterCreatureManager_c
{
public:
	void Init(void);
	void Update(float t);
};
WRAPPER void WaterCreatureManager_c::Init(void) { EAXJMP(0x6E3F90); }
WRAPPER void WaterCreatureManager_c::Update(float t) { EAXJMP(0x6E4F10); }
WaterCreatureManager_c &g_waterCreatureMan = *(WaterCreatureManager_c*)0xC1DF30;

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
	CTheZones__Init();
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

static Reversed Init2_kill(0x5BA1A0, 0x5BA3FF);
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

class CLoadMonitor
{
public:
	void BeginFrame(void);
	void EndFrame(void);
};
WRAPPER void CLoadMonitor::BeginFrame(void) { EAXJMP(0x53D030); }
WRAPPER void CLoadMonitor::EndFrame(void) { EAXJMP(0x53D0B0); }
CLoadMonitor &g_LoadMonitor = *(CLoadMonitor*)0xB72978;

class CFireManager
{
public:
	void Update(void);
};
WRAPPER void CFireManager::Update(void) { EAXJMP(0x53AF00); }
CFireManager &gFireManager = *(CFireManager*)0xB71F80;

class CInterestingEvents
{
public:
	void ScanForNearbyEntities(void);
};
WRAPPER void CInterestingEvents::ScanForNearbyEntities(void) { EAXJMP(0x605A30); }
CInterestingEvents &g_InterestingEvents = *(CInterestingEvents*)0xC0B058;

WRAPPER void CTheZones__Update(void) { EAXJMP(0x572D10); }
WRAPPER void CCover__Update(void) { EAXJMP(0x6997E0); }
WRAPPER void CAudioZones__Update(bool, float, float, float) { EAXJMP(0x5083C0); }
void CAudioZones__Update(bool b, CVector v) { CAudioZones__Update(b, v.x, v.y, v.z); }
WRAPPER void CCheat__DoCheats(void) { EAXJMP(0x439AF0); }
WRAPPER void CSkidmarks__Update(void) { EAXJMP(0x439AF0); }
WRAPPER void CGlass__Update(void) { EAXJMP(0x7205C0); }
WRAPPER void CCreepingFire__Update(void) { EAXJMP(0x539CE0); }
WRAPPER void CSetPieces__Update(void) { EAXJMP(0x49AA00); }
WRAPPER void CPopulation__Update(bool) { EAXJMP(0x616650); }
WRAPPER void CTheCarGenerators__Process(void) { EAXJMP(0x6F3F40); }
WRAPPER void CClouds__Update(void) { EAXJMP(0x712FF0); }
WRAPPER void CMovingThings__Update(void) { EAXJMP(0x7185B0); }
WRAPPER void CWaterCannons__Update(void) { EAXJMP(0x72A3C0); }
WRAPPER void CPickups__Update(void) { EAXJMP(0x458DE0); }
WRAPPER void CCarCtrl__PruneVehiclesOfInterest(void) { EAXJMP(0x423F10); }
WRAPPER void CEntryExitManager__Update(void) { EAXJMP(0x440D10); }
WRAPPER void CStuntJumpManager__Update(void) { EAXJMP(0x49C490); }
WRAPPER void CBirds__Update(void) { EAXJMP(0x712330); }
WRAPPER void CSpecialFX__Update(void) { EAXJMP(0x726AA0); }
WRAPPER void CRopes__Update(void) { EAXJMP(0x558D70); }
WRAPPER void CPopCycle__Update(void) { EAXJMP(0x610BF0); }
WRAPPER void CCullZones__Update(void) { EAXJMP(0x72DEC0); }
WRAPPER void CGameLogic__Update(void) { EAXJMP(0x442AD0); }
WRAPPER void CGangWars__Update(void) { EAXJMP(0x446610); }
WRAPPER void CConversations__Update(void) { EAXJMP(0x43C590); }
WRAPPER void CPedToPlayerConversations__Update(void) { EAXJMP(0x43B0F0); }
WRAPPER void CCarCtrl__GenerateRandomCars(void) { EAXJMP(0x4341C0); }
WRAPPER void CCarCtrl__RemoveDistantCars(void) { EAXJMP(0x42CD10); }
WRAPPER void CCarCtrl__RemoveCarsIfThePoolGetsFull(void) { EAXJMP(0x4322B0); }
WRAPPER void CRoadBlocks__GenerateRoadBlocks(void) { EAXJMP(0x4629E0); }

int &CWindModifiers__Number = *(int*)0xC81450;

static Reversed Process_kill(0x53BEE0, 0x53C23F);
void
CGame::Process(void)
{
	CPad::UpdatePads();
	g_LoadMonitor.BeginFrame();

	int timer = CTimer::GetCurrentTimeInMilliseconds();
	CStreaming::Update();
	timer = CTimer::GetCurrentTimeInMilliseconds() - timer;
	CCutsceneMgr::Update();
	DebugMenuProcess();
	if(!CCutsceneMgr::ms_cutsceneProcessing && !CTimer::m_CodePause)
		FrontEndMenuManager.Process();
	CTheZones__Update();
	CCover__Update();
	CAudioZones__Update(false, TheCamera.GetCoords());
	CWindModifiers__Number = 0;

	if(!CTimer::m_UserPause && !CTimer::m_CodePause){
		// CSprite2d::SetRecipNearClip();	// no-op
		CSprite2d::InitPerFrame();
		CFont::InitPerFrame();
		CCheat__DoCheats();
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
		CClouds__Update();
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

			CRubbish::Render();

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

	CWaterLevel::PreRenderWater();
}


WRAPPER void CMirrors__BeforeMainRender(void) { EAXJMP(0x727140); }
WRAPPER void CMirrors__RenderMirrorBuffer(void) { EAXJMP(0x726090); }
WRAPPER void CMessages__Display(bool) { EAXJMP(0x69EFC0); }
WRAPPER void CCredits__RenderCredits(void) { EAXJMP(0x5A87F0); }

WRAPPER void DoFade(void) { EAXJMP(0x53E600); }
void RenderFontBuffer(void) { CFont::RenderFontBuffer(); }

int &CPointLights__NumLights = *(int*)0xC3F0D0;

RwRGBA &gColourTop = *(RwRGBA*)0xB72CA0 ;

void
CCredits__Render(void)
{
	if(CCredits__bCreditsGoing)
		if(!FrontEndMenuManager.m_bMenuActive)
			CCredits__RenderCredits();
}

void
Render2dStuffAfterFade(void)
{
	CHud::DrawAfterFade();
	CMessages__Display(0);
	RenderFontBuffer();
	// CCredits__Render();	// already rendered in Idle
}

/*
void
DoRWStuffEndOfFrame(void)
{
	// CDebug::DebugDisplayTextBuffer();
	// FlushObrsPrintfs();

	RwCameraEndUpdate(Scene.camera);
	RsCameraShowRaster(Scene.camera);
}
*/

static Reversed Idle_kill(0x53E920, 0x53EC0F);
void
Idle(void *arg)
{
	static int lastTime;

	while(CTimer::GetCurrentTimeInMilliseconds() - lastTime < 14);

	CTimer::Update();
	CSprite2d::InitPerFrame();
	CFont::InitPerFrame();
	CPointLights__NumLights = 0;
	CGame::Process();
	AudioEngine.Service();
	SetLightsWithTimeOfDayColour(Scene.world);

	if(arg == NULL)
		return;

	if(!FrontEndMenuManager.m_bMenuActive && TheCamera.GetFadeStage() != 2){
		RwV2d pos;
		pos.x = RsGlobal.MaximumWidth / 2.0f;
		pos.y = RsGlobal.MaximumHeight / 2.0f;
		RsMouseSetPos(&pos);

		CRenderer::ConstructRenderList();
		CRenderer::PreRender();
		CWorld::ProcessPedsAfterPreRender();
		g_realTimeShadowMan.Update();
		CMirrors__BeforeMainRender();

		// TODO: reverse this to the old VC code, right now the arguments do nothing!
		if(CWeather::LightningFlash){
			// TODO: clean this up once everything is under our control
			CTimeCycle::m_CurrentColours.skybotr = 255;
			CTimeCycle::m_CurrentColours.skybotg = 255;
			CTimeCycle::m_CurrentColours.skybotb = 255;
			CTimeCycle::m_CurrentColours.skytopr = 255;
			CTimeCycle::m_CurrentColours.skytopg = 255;
			CTimeCycle::m_CurrentColours.skytopb = 255;
			CTimeCycle::m_CurrentColours_exe.skybotr = 255;
			CTimeCycle::m_CurrentColours_exe.skybotg = 255;
			CTimeCycle::m_CurrentColours_exe.skybotb = 255;
			CTimeCycle::m_CurrentColours_exe.skytopr = 255;
			CTimeCycle::m_CurrentColours_exe.skytopg = 255;
			CTimeCycle::m_CurrentColours_exe.skytopb = 255;
			if(!DoRWStuffStartOfFrame_Horizon(255, 255, 255, 255, 255, 255, 255))
				return;
		}else{
			if(!DoRWStuffStartOfFrame_Horizon(
							CTimeCycle::m_CurrentColours.skytopr,
							CTimeCycle::m_CurrentColours.skytopg,
							CTimeCycle::m_CurrentColours.skytopb,
							CTimeCycle::m_CurrentColours.skybotr,
							CTimeCycle::m_CurrentColours.skybotg,
							CTimeCycle::m_CurrentColours.skybotb,
							255))
				return;
		}

		DefinedState();
		RwCameraSetFarClipPlane(Scene.camera, CTimeCycle::m_CurrentColours.farclp);
		RwCameraSetFogDistance(Scene.camera, CTimeCycle::m_CurrentColours.fogst);

		CMirrors__RenderMirrorBuffer();
		RenderScene();

		CVisibilityPlugins::RenderWeaponPedsForPC();
		CVisibilityPlugins::ResetWeaponPedsForPC();

		RenderEffects();

		// skipping camera motion blur, not used

		Render2dStuff();

		DebugMenuRender();
	}else{
		CDraw::CalculateAspectRatio();
		CameraSize(Scene.camera, NULL, tan(CDraw::ms_fFOV * M_PI / 360.0f), CDraw::ms_fAspectRatio);
		CVisibilityPlugins::SetRenderWareCamera(Scene.camera);
		RwCameraClear(Scene.camera, &gColourTop, rwCAMERACLEARZ);
		if(!RsCameraBeginUpdate(Scene.camera))
			return;
	}

	if(FrontEndMenuManager.m_bMenuActive)
		FrontEndMenuManager.DrawFrontEnd();

	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
	DoFade();

	Render2dStuffAfterFade();
	CCredits__Render();

	DoRWStuffEndOfFrame();
}

#include <vector>

static StaticPatcher Patcher([](){
	Memory::InjectHook(0x53E58E, CGame::Initialise);
	// Memory::InjectHook(0x53E981, CGame::Process);
	Memory::InjectHook(0x53ECBD, Idle);
});
