#ifndef __MISC
#define __MISC

class CPathFind
{
public:
	void Init(void);
	void UpdateStreaming(bool);
	void AllocatePathFindInfoMem(void) { }
	void PreparePathData(void) { }
};
extern CPathFind &ThePaths;

class InteriorManager_c
{
public:
	void Init(void);
	void Update(void);
};
extern InteriorManager_c &g_interiorMan;

class ProcObjectMan_c
{
public:
	void Init(void);
	void Update(void) { }
};
extern ProcObjectMan_c &g_procObjMan;

class WaterCreatureManager_c
{
public:
	void Init(void);
	void Update(float t);
};
extern WaterCreatureManager_c &g_waterCreatureMan;

class CLoadMonitor
{
public:
	void BeginFrame(void);
	void EndFrame(void);
};
extern CLoadMonitor &g_LoadMonitor;

class CFireManager
{
public:
	void Update(void);
};
extern CFireManager &gFireManager;

class CInterestingEvents
{
public:
	void ScanForNearbyEntities(void);
};
extern CInterestingEvents &g_InterestingEvents;


class CClouds
{
public:
	static void Init(void);
	static void Update(void);
	static void MovingFogRender(void);
	static void VolumetricCloudsRender(void);
	static void Render(void);
	static void RenderBottomFromHeight(void);
};

class CTheZones
{
public:
	static void Init(void);
	static void Update(void);
	static void PostZoneCreation(void) { }
};

class CCredits
{
public:
	static bool &bCreditsGoing;
	static void RenderCredits(void);
	static void Render(void);
};

class CMirrors
{
public:
	static int &TypeOfMirror;
	static bool &bRenderingReflection;
	static void BeforeMainRender(void);
	static void RenderMirrorBuffer(void);
};

extern int &CWindModifiers__Number;
extern int &CPointLights__NumLights;



void sub_70F9E0(void);

void CMaths__InitMathsTables(void);
void CPlaceable__InitMatrixArray(void);
// void CIniFile__LoadIniFile(void);
void dummy_730AC0(char b);
void CGameLogic__InitAtStartOfGame(void);
void CGangWars__InitAtStartOfGame(void);
void CConversations__Clear(void);
void CPedToPlayerConversations__Clear(void);
void CQuadTreeNode__InitPool(void);
// bool CPlantMgr__Initialise(void);
bool CCustomRoadsignMgr__Initialise(void);
void CReferences__Init(void);
void CDebug__DebugInitTextBuffer(void);
void CTagManager__Init(void);
void CCover__Init(void);
void CCullZones__Init(void);
void COcclusion__Init(void);
void CCollision__Init(void);
void CBirds__Init(void);
void CEntryExitManager__Init(void);
void CEntryExitManager__PostEntryExitsCreation(void);
void CStuntJumpManager__Init(void);
void CSetPieces__Init(void);
void CMessages__Init(void);
void CMessages__ClearAllMessagesDisplayedByGame(bool b);
void CVehicleRecording__Init(void);
void CRestart__Initialise(void);
void CAnimManager__Initialise(void);
void CAnimManager__LoadAnimFiles(void);
void CCarCtrl__Init(void);
void CPickups__Init(void);
void CTheCarGenerators__Init(void);
void CGarages__Init(void);
void CAudioZones__Init(void);
// void CStreaming__ReadIniFile(void);
void CTaskSimpleFight__LoadMeleeData(void);
void CCheat__ResetCheats(void);

bool CCustomCarPlateMgr__Initialise(void);
void CRoadBlocks__Init(void);
void CPopulation__Initialise(void);
void CWeaponEffects__Init(void);
void CSkidmarks__Init(void);
void CGlass__Init(void);
void CMovingThings__Init(void);
void CSpecialFX__Init(void);
void CRopes__Init(void);
void CWaterCannons__Init(void);
void CPedGroups__Init(void);
void CInformFriendsEventQueue__Init(void);
void CInformGroupEventQueue__Init(void);
void CShopping__Init(void);



void CCover__Update(void);
void CAudioZones__Update(bool, float, float, float);
void CAudioZones__Update(bool b, CVector v);
void CCheat__DoCheats(void);
void CSkidmarks__Update(void);
void CGlass__Update(void);
void CCreepingFire__Update(void);
void CSetPieces__Update(void);
void CPopulation__Update(bool);
void CTheCarGenerators__Process(void);
void CMovingThings__Update(void);
void CWaterCannons__Update(void);
void CPickups__Update(void);
void CCarCtrl__PruneVehiclesOfInterest(void);
void CEntryExitManager__Update(void);
void CStuntJumpManager__Update(void);
void CBirds__Update(void);
void CSpecialFX__Update(void);
void CRopes__Update(void);
void CPopCycle__Update(void);
void CCullZones__Update(void);
void CGameLogic__Update(void);
void CGangWars__Update(void);
void CConversations__Update(void);
void CPedToPlayerConversations__Update(void);
void CCarCtrl__GenerateRandomCars(void);
void CCarCtrl__RemoveDistantCars(void);
void CCarCtrl__RemoveCarsIfThePoolGetsFull(void);
void CRoadBlocks__GenerateRoadBlocks(void);

void CMessages__Display(bool);

void CBirds__Render(void);
void CRopes__Render(void);
void CGlass__Render(void);
void CMovingThings__Render(void);
void CMovingThings__Render_BeforeClouds(void);
void Fx_c__Render(RwCamera *cam, int i);
void CWaterCannons__Render(void);
void CWeaponEffects__Render(void);
void CSpecialFX__Render(void);
void CVehicleRecording__Render(void);
void CPointLights__RenderFogEffect(void);


#endif __MISC
