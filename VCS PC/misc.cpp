#include "StdAfx.h"
#include "misc.h"

#include "Frontend.h"

// For all the stuff that's only wrapped. Some of those might deserve better treatment.
// Mostly needed for CGame::Initialise, CGame::Process and Idle


// Classes with actual instances

WRAPPER void CPathFind::Init(void) { EAXJMP(0x44D080); }
WRAPPER void CPathFind::UpdateStreaming(bool) { EAXJMP(0x450A60); }
CPathFind &ThePaths = *(CPathFind*)0x96F050;

WRAPPER void InteriorManager_c::Init(void) { EAXJMP(0x5C0500); }
WRAPPER void InteriorManager_c::Update(void) { EAXJMP(0x598F50); }
InteriorManager_c &g_interiorMan = *(InteriorManager_c*)0xBAF670;

WRAPPER void ProcObjectMan_c::Init(void) { EAXJMP(0x5A3EA0); }
ProcObjectMan_c &g_procObjMan = *(ProcObjectMan_c*)0xBB7CB0;

WRAPPER void WaterCreatureManager_c::Init(void) { EAXJMP(0x6E3F90); }
WRAPPER void WaterCreatureManager_c::Update(float t) { EAXJMP(0x6E4F10); }
WaterCreatureManager_c &g_waterCreatureMan = *(WaterCreatureManager_c*)0xC1DF30;

WRAPPER void CLoadMonitor::BeginFrame(void) { EAXJMP(0x53D030); }
WRAPPER void CLoadMonitor::EndFrame(void) { EAXJMP(0x53D0B0); }
CLoadMonitor &g_LoadMonitor = *(CLoadMonitor*)0xB72978;

WRAPPER void CFireManager::Update(void) { EAXJMP(0x53AF00); }
CFireManager &gFireManager = *(CFireManager*)0xB71F80;

WRAPPER void CInterestingEvents::ScanForNearbyEntities(void) { EAXJMP(0x605A30); }
CInterestingEvents &g_InterestingEvents = *(CInterestingEvents*)0xC0B058;


// static classes

WRAPPER void CTheZones::Init(void) { EAXJMP(0x572670); }
WRAPPER void CTheZones::Update(void) { EAXJMP(0x572D10); }


WRAPPER void CClouds::Init(void) { EAXJMP(0x7138D0); }
WRAPPER void CClouds::Update(void) { EAXJMP(0x712FF0); }
WRAPPER void CClouds::MovingFogRender(void) { EAXJMP(0x716C90); }
WRAPPER void CClouds::VolumetricCloudsRender(void) { EAXJMP(0x716380); }
WRAPPER void CClouds::Render(void) { EAXJMP(0x713950); }
WRAPPER void CClouds::RenderBottomFromHeight(void) { EAXJMP(0x7154B0); }

bool &CCredits::bCreditsGoing = *(bool*)0xC6E97C;
WRAPPER void CCredits::RenderCredits(void) { EAXJMP(0x5A87F0); }

void
CCredits::Render(void)
{
	if(bCreditsGoing)
		if(!FrontEndMenuManager.m_bMenuActive)
			RenderCredits();
}


int &CMirrors::TypeOfMirror = *(int*)0xC7C724;
bool &CMirrors::bRenderingReflection = *(bool*)0xC7C728;
WRAPPER void CMirrors::BeforeMainRender(void) { EAXJMP(0x727140); }
WRAPPER void CMirrors::RenderMirrorBuffer(void) { EAXJMP(0x726090); }



int &CWindModifiers__Number = *(int*)0xC81450;
int &CPointLights__NumLights = *(int*)0xC3F0D0;


// something d3d related
WRAPPER void sub_70F9E0(void) { EAXJMP(0x70F9E0); }

WRAPPER void CMaths__InitMathsTables(void) { EAXJMP(0x59AC90); }
WRAPPER void CPlaceable__InitMatrixArray(void) { EAXJMP(0x54F3A0); }
WRAPPER void CIniFile__LoadIniFile(void) { EAXJMP(0x56D070); }
WRAPPER void dummy_730AC0(char b) { EAXJMP(0x730AC0); }
WRAPPER void CGameLogic__InitAtStartOfGame(void) { EAXJMP(0x441210); }
WRAPPER void CGangWars__InitAtStartOfGame(void) { EAXJMP(0x443920); }
WRAPPER void CConversations__Clear(void) { EAXJMP(0x43A7B0); }
WRAPPER void CPedToPlayerConversations__Clear(void) { EAXJMP(0x43AAE0); }
WRAPPER void CQuadTreeNode__InitPool(void) { EAXJMP(0x552C00); }
// WRAPPER bool CPlantMgr__Initialise(void) { EAXJMP(0x5DD910); }
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
WRAPPER void CSpecialFX__Init(void) { EAXJMP(0x7268F0); }
WRAPPER void CRopes__Init(void) { EAXJMP(0x555DC0); }
WRAPPER void CWaterCannons__Init(void) { EAXJMP(0x728C80); }
WRAPPER void CPedGroups__Init(void) { EAXJMP(0x5FB8C0); }
WRAPPER void CInformFriendsEventQueue__Init(void) { EAXJMP(0x4B2990); }
WRAPPER void CInformGroupEventQueue__Init(void) { EAXJMP(0x4B2AD0); }
WRAPPER void CShopping__Init(void) { EAXJMP(0x49C290); }


WRAPPER void CCover__Update(void) { EAXJMP(0x6997E0); }
WRAPPER void CAudioZones__Update(bool, float, float, float) { EAXJMP(0x5083C0); }
void CAudioZones__Update(bool b, CVector v) { CAudioZones__Update(b, v.x, v.y, v.z); }
WRAPPER void CCheat__DoCheats(void) { EAXJMP(0x439AF0); }
WRAPPER void CSkidmarks__Update(void) { EAXJMP(0x7205C0); }
WRAPPER void CGlass__Update(void) { EAXJMP(0x71B0D0); }
WRAPPER void CCreepingFire__Update(void) { EAXJMP(0x539CE0); }
WRAPPER void CSetPieces__Update(void) { EAXJMP(0x49AA00); }
WRAPPER void CPopulation__Update(bool) { EAXJMP(0x616650); }
WRAPPER void CTheCarGenerators__Process(void) { EAXJMP(0x6F3F40); }
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


WRAPPER void CBirds__Render(void) { EAXJMP(0x712810); }
WRAPPER void CRopes__Render(void) { EAXJMP(0x556AE0); }
WRAPPER void CGlass__Render(void) { EAXJMP(0x71CE20); }
void CMovingThings__Render(void) {}
WRAPPER void CMovingThings__Render_BeforeClouds(void) { EAXJMP(0x7178F0); }
WRAPPER void Fx_c__Render(RwCamera *cam, int i) { WRAPARG(cam); WRAPARG(i); EAXJMP(0x49E650); }
WRAPPER void CWaterCannons__Render(void) { EAXJMP(0x729B30); }
WRAPPER void CWeaponEffects__Render(void) { EAXJMP(0x742CF0); }
WRAPPER void CSpecialFX__Render(void) { EAXJMP(0x726AD0); }
void CVehicleRecording__Render(void) {}
WRAPPER void CPointLights__RenderFogEffect(void) { EAXJMP(0x7002D0); }
WRAPPER void CMessages__Display(bool) { EAXJMP(0x69EFC0); }
