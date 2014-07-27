#include "StdAfx.h"

#include "handlingIDs.h"
#include "AnimMgr.h"
#include "Ped.h"
#include "Vehicle.h"
#include "Pools.h"
#include "CConfiscatedWeapons.h"
#include "Streaming.h"
#include "Sprite.h"
#include "General.h"
#include "Building.h"
#include "Script.h"
#include "Camera.h"
#include "Text.h"
#include "Hud.h"
#include "Frontend.h"
#include "Audio.h"
#include "CutsceneManager.h"
#include "UserDisplay.h"
#include "3DMarkers.h"
#include "Font.h"
#include "FileMgr.h"
#include "Stats.h"
#include "WidescreenSupport.h"
#include "UpdateManager.h"
#include "DLCManager.h"
#include "PcSave.h"
#include "Radar.h"
#include "Pad.h"
#include "Garages.h"
#include "BankLoader.h"
#include "Coronas.h"
#include "Darkel.h"
#include "Messages.h"
#include "TxdStore.h"
#include "VideoPlayer.h"
#include "World.h"
#include "EntryExitMgr.h"
#include "Antennas.h"
#include "ControlsMgr.h"
#include "VisibilityPlugins.h"
#include "Object.h"
#include "VCSPC_SDK_Internal.h"

// Regular functions
//LONG CALLBACK	ExecHandle(EXCEPTION_POINTERS* ep);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam,LPARAM lParam);
void			CollectPCInfo();
void			DetermineGameVer();
void			DefineVariables();
//void			CheckStructures();
void			InjectDelayedPatches();
void			Main_Patches();
void			PatchMenus();
void			AnimationStylesPatching();
void			UserFiles();
void			HighspeedCamShake(float shake);
void			ViceSquadCheckInjectA(int townID);
int				ViceSquadCheckInjectB();
void			InjectArrowMarker();
void			OpaqueRadarHack2(RwPrimitiveType primType, RwIm2DVertex* vertices, RwInt32 numVertices);
float			__stdcall HelperPosXHack(int);
void			ReadLoadingSplashes(bool bIntroSplash, int nIntroSplashID);
void			InitialiseLanguage();
void			LoadGameFailedMessage(unsigned char bMessageIndex);
void			MessageLoop();
void			CdStreamClearNames();
void			ParseCommandlineFile();
char*			ParseCommandlineArgument(char* pArg);
BOOL			IsAlreadyRunning();
void			TXDLoadOverride(signed int nIndex, const char* pName);
void			SaveLanguageHack(FILE* stream, const char* ptr, size_t len);
void			LoadLanguageHack(FILE* stream, void* buf, size_t len);
CVehicle*		__fastcall VehiclePoolGetAt(CVehiclePool* pThis, int unused, int nIdentifier);
CPed*			__fastcall PedPoolGetAt(CPedPool* pThis, int unused, int nIdentifier);
/*#if DEBUG
void			LoadDevLogos();
void			ReleaseDevLogos();
#endif*/


// ASM functions
void			ViceSquadCheckInjectC();
void			EnexMarkersColorBreak();
void			drawRampageTextTimeBreak();
void			drawRampageTextKillsBreak();
void			GarageCapacityA();
void			GarageCapacityB();
void			FlamingStunt();
void			DoubleFlamingStunt();
void			TripleFlamingStunt();
void			QuadrupleFlamingStunt();
void			func_0050();
void			func_0166();
void			func_01C6();
void			func_0352();
void			func_03DF();
void			func_052E();
void			func_0602();
void			func_069C();
void			func_06BA();
void			func_0821();
void			func_09CD();
void			ZebraCabNitro();
void			ZebraCabCPlayerStuff();
void			func_610310();
void			func_6B4800();
void			func_6D1AA0();
void			GoodCitizenBonusFlag();
void			InCarKilledCounterBreak();
void			Language_CASE_English();
void			Language_CASE_German();
void			Language_CASE_Spanish();
void			Language_CASE_Brazilian();
void			Language_CASE_Polish();
void			Language_CASE_Hungarian();
void			Language_CASE_Romanian();
void			MissionLanguage_CASE_English();
void			MissionLanguage_CASE_German();
void			MissionLanguage_CASE_Brazilian();
void			MissionLanguage_CASE_Spanish();
void			MissionLanguage_CASE_Polish();
void			MissionLanguage_CASE_Hungarian();
void			MissionLanguage_CASE_Romanian();
void			CGameLogic__Update_Busted();
void			CGameLogic__Update_Wasted();
void			AssignViceSquadToVehicle();
void			Clock_SwitchInject();
void			Clock_StringInject();
void			RadioNameInject();
void			RunningScriptPointerParamWrapper();
void			ScriptCheckpointsColours_Normal();
void			ScriptCheckpointsColours_Others();
void			ScriptCheckpointsColours_Finish();
void			BigMessage1Inject();
void			BigMessage2Inject();
void			BigMessage3Inject();
void			BigMessage6Inject();
void			ExtraWeaponAnimationsInject();
void			WeaponSounds_Shotgun();
void			WeaponSounds_Stubby();
void			WeaponSounds_M16();
void			ATVPreRenderHack();
void			MultipleTitlePCInject();
void			InjectPedMapping();
void			PedDataConstructorInject_Civilian();
void			PedDataConstructorInject_Cop();
void			LightsOnFix();
void			NodeCrashFix();
void			NodeCrashFix2();
//void			HUDInitialiseBreak();
void			TimestampSignatureHack();
void			LoadGameFailedMessage_Inject();
void			SaveFallback_InjectOnLoad();
void			SaveFallback_InjectOnSave();
void			SaveFallback_FallbackPreBeta3Names();
void			SaveFallback_WrapAroundFirstBlock();
void			MenuEntriesAlignHack();
void			MenuEntriesPositionHack_Inject();
void			MenuEntriesLeftColumnHack();
void			MenuEntriesLeftColumnHack2();
void			MenuEntriesPlaceSave();
void			MenuEntriesPlaceSave2();
void			MenuEntryColourHack();
void			SetToNewMenuHack();
void			MenuToggleHack();
void			StatsMenuActionHack();
void			WidescreenSupportRecalculateHack();
void			WidescreenSupportRecalculateHack2();
void			WidescreenSupportRecalculateHack3();
void			WidescreenFOVHack();
void			WidescreenFOVHack2();
void			WidescreenBordersHack();
void			AimpointCalc();
void			WidescreenTextPositionHack();
void			WidescreenSkyWidthHack();
void			Widescreen_SwitchInject();
void			Widescreen_StringInject();
//void			Widescreen_TextDrawsFix();
void			Widescreen_TextDrawsFix2();
void			DriveByKillFix();
void			LookLeftRightHack();
void			DrawDistanceRecalc();
//void			ZeroScriptsCounter();
void			LoadFunctionBreak();
void			RightShockKeyHack();
void			C3DMarkerSizeHack();
void			NoRadioCommercialsHack();
void			RadioResetWrapper();
void			SaveRadios();
void			LoadRadios();
void			VideoPlayerCreate1();
void			VideoPlayerCreate2();
void			VideoPlayerPlayNextFrame();
void			VideoPlayerProc();
void			VideoPlayerRelease();
void			MaxosFrameLimitHack();
void			FrameLimit_SwitchInject();
void			FrameLimit_StringInject();
void			FrameLimit_SetFPS();
void			FrameLimit_SetFPS2();
void			CameraInitHack();
void			OpaqueRadarHack();
/*void			LoadFontsHack();
void			ReleaseFontsHack();*/
void			SetCutsceneModelHack();
void			ErrorSoundCentre();
void			BuySoundCentre();
void			UpdaterProcessHack();
void			UpdaterMenuDrawHack();
void			UpdaterTextSwap();
void			UpdaterMenuAction();
void			IMGEncryptionFindOut();
void			IMGEncryptionDo();
void			CommandlineEventHack();
void			ReadCommandlineFile();
void			LoadGameWithDLCHack();
void			DLCMenuAction();
void			ActivateSerialAction();
void			AutoInstallUpdatesAction();
void			CheckEveryAction();
void			VehAudioHook();
//void			RotorsHook();
void			Language6Action();
#ifdef INCLUDE_MULTIFONTFILES
void			MultipleFontsDAT_Inject();
void			MultipleFontsTXD_Inject();
void			MultipleFontsFiles_Validator();
#endif

#ifdef ADD_UNFINISHED_WINDOWED_MODE
#pragma message ("BETA: Including UNFINISHED windowed mode...")
void			InjectWindowedMode();
#endif

#ifdef INCLUDE_AUDIO_DEBUG
#pragma message ("DEBUG: Including audio debug stuff...")
BYTE				bShouldEcho = 0;
#endif

// Objects & variables
CCRC32				HashHelper;
CConfiscatedWeapons	weaponsToReturn;
DWORD				stackPtr;
HANDLE				gStreamFiles[NUM_STREAMS];
char				gStreamNames[NUM_STREAMS][64];
bool				InAmazingScreenshotMode;
#ifdef INCLUDE_MULTIFONTFILES
bool				bCanReloadFonts = false;
eFontFileIDs		bLastFontsID;
#endif

char				gString[552];
char				gUString[552];

#if DEVBUILD
CSprite2d			DevLogos[1];
#endif

void*						_ProcessEnexMarkers_JumpBack;
//void*						CGameLogic__Update_Busted_JumpBack;
//void*						CGameLogic__Update_Wasted_JumpBack;
void*						CMenu__DrawRadioStationIconsIconBreak_ElseJump;
void*						CMenu__DrawRadioStationIconsIconBreak_JumpBack;
void*						AssignCopToVehicleSwitch_JumpBack;
void*						ViceSquadCheckInjectC_JumpBack;
void*						func_0602_ElseJump;
void*						func_0602_JumpBack;
void*						TaxiCabNitro_JumpBack;
void*						TaxiCabNitro_ElseJump;
void*						ZebraCabCPlayerStuff_JumpBack;
void*						ZebraCabCPlayerStuff_ElseJump;
void*						func_610310_JumpBack;
void*						func_610310_ElseJump;
void*						func_6B4800_JumpBack;
void*						func_6B4800_ElseJump;
void*						func_6D1AA0_JumpBack;
void*						func_6D1AA0_ElseJump;
void*						Clock_SwitchInject_JumpBack;
void*						Clock_StringInject_JumpBack;
void*						Clock_StringInject_JumpToOFF;
void*						ScriptCheckpointsColours_JumpBack;
void*						BigMessage1Inject_JumpBack;
void*						ExtraWeaponAnimationsInject_JumpBack;
void*						ExtraWeaponAnimationsInject_NoArmed;
void*						WeaponSounds_JumpBack;
//void*						MultipleTitlePCInject_JumpBack;
void*						DeleteSecondSplash_JumpBack;
void*						RestoreSecondSplash_JumpBack;
void*						InjectPedMapping_JumpBack;
void*						NodeCrashFix_JumpBack;
void*						NodeCrashFix2_JumpBack;
void*						TimestampSignatureHack_JumpBack;
void*						TimestampSignatureHack_CheckFailed;
void*						LoadGameFailedMessage_JumpBack;
void*						SaveFallback_InjectOnLoad_JumpBack;
void*						SaveFallback_InjectOnSave_ReturnTrue;
void*						SaveFallback_InjectOnSave_ReturnFalse;
void*						SaveFallback_FallbackPreBeta3Names_JumpBack;
void*						MenuEntriesAlignHack_JumpBack;
void*						MenuEntriesPositionHack_JumpBack;
void*						MenuEntriesLeftColumnHack2_JumpBack;
void*						WidescreenFOVHack_JumpBack;
void*						Widescreen_SwitchInject_JumpBack;
void*						Widescreen_TextDrawsFix2_JumpBack;
void*						DriveByKillFix_ReturnTrue;
void*						DriveByKillFix_ReturnFalse;
void*						ZeroScriptsCounter_JumpBack;
void*						func_01C6_JumpBack;
void*						RightShockKeyHack_JumpBack;
void*						NoRadioCommercialsHack_JumpBack;
void*						VideoPlayerCreate1_JumpBack;
void*						VideoPlayerPlayNextFrame_JumpBack;
void*						VideoPlayerProc_JumpBack;
void*						VideoPlayerRelease_JumpBack;
void*						FrameLimit_SwitchInject_JumpBack;
void*						FrameLimit_StringInject_JumpBack;
/*void*						LoadFontsHack_JumpBack;
void*						ReleaseFontsHack_JumpBack;*/
void*						SetCutsceneModelHack_JumpBack;
#ifdef INCLUDE_MULTIFONTFILES
void*						MultipleFontsDAT_JumpBack;
void*						MultipleFontsTXD_JumpBack;
#endif

SCRIPT_VAR*					scriptParams;
SCRIPT_VAR*					scriptLocals;
void*						scmBlock;
DWORD*						CPopulation__allRandomPedThisType;
DWORD*						PlayerEnexEntryStage;
DWORD*						activeInterior;
DWORD*						memoryAvailable;
DWORD*						memoryUsed;
eFlash*						wFlashingComponentID;
BYTE*						bWants_To_Draw_Hud;
BYTE*						radarGrey;
float*						WidthAspectRatio;
float*						HeightAspectRatio;
float&						ScreenAspectRatio = *(float*)0xC3EFA4;
float*						fFOV;
bool*						bHideStyledTextWhileFading;
char*						latestMissionName;
char*						StyledText_1;
char*						StyledText_2;
char*						StyledText_3;
char*						StyledText_4;
char*						StyledText_5;
char*						StyledText_6;
char*						StyledText_7;
char*						PriorityText;
char*						Garage_MessageIDString;
float*						currentFPS;
RsGlobalType&				RsGlobal = *(RsGlobalType*)0xC17040;
CCamera&					TheCamera = *(CCamera*)0xB6F028;
CText&						TheText = *(CText*)0xC1B340;
CRGBA*						BaseColors;
CMenuManager&				FrontEndMenuManager = *(CMenuManager*)0xBA6748;
CAudioEngine&				AudioEngine = *(CAudioEngine*)0xB6BC90;
RpClump**					arrowClump;
std::pair<void*,int>* const	materialRestoreData = (std::pair<void*,int>*)0xB4DBE8;
CBlurStage*					blurStages;
//CGridref*					gridref;
CGarages*					garages;
CAnimationStyleDescriptor*	animationStyles;
CKeyState*					activeKeyState;
CKeyState*					currKeyState;
CKeyState*					prevKeyState;
//CSprite2d*					texFonts;
CRunningScript**			pActiveScripts;
CRunningScript*				ScriptsArray;
RwIm2DVertex* const			aSpriteVertices = (RwIm2DVertex*)0xC80468;
RwTexture** const			gpCoronaTexture = (RwTexture**)0xC3E000;
RwCamera*&					Scene = *(RwCamera**)0xC1703C;
DWORD*						gameState;
void**						rwengine = (void**)0xC97B24;

CControllerConfigManager&	ControlsManager = *(CControllerConfigManager*)0xB70198;

void						(*replacedTXDLoadFunc)();
void						(*replacedTXDReleaseFunc)();
void						(*TheScriptsInitFunc)();

const float					LoadingBarHeight = 27.5f;
const float					LoadingBarWidth = 155.0f;
const float					LoadingBarPosX = 40.0f;
const float					LoadingBarPosY = 55.0f;
const float					fMapZonePosX = 7.5f;
//const float					fMenuSliderPosY2 = MENU_SLIDER_POSY / 448.0;
//const float					fMenuSliderPosY3 = (MENU_SLIDER_POSY - 30.0) / 448.0;
//const float					fMenuSliderPosY4 = (MENU_SLIDER_POSY - MENU_SLIDER_WIDTH / 2.0) / 448.0;
const float					fMenuSliderWidth = MENU_SLIDER_WIDTH;
const float					fRadarPosX = 35.0f;
//const float					fRadarWidth = 76.0 * 480.0 / 448.0;
const float					fRadarHeight = 94.0f * 448.0f / 480.0f;
const float					fRadarPosY = 107.0f;
//const float					fMenuSliderHeight2 = MENU_SLIDER_HEIGHT / 448.0;
const float					fCTSliderRight = 370.0f;
const float					fRhinoHitStrength = 1000.0f;
const float					fRefZVal = 1.0f;
const float					fBrightnessStep = 1.0f / 192.0f;
const float					fBrightnessStep2 = 12.0f;
const float					fBrightnessMax = 192.0f;
//const float					fBriefTextHeight = 0.7/448.0;
const float					fNewDrawDistance = MAX_DRAW_DISTANCE;
const float					fSkyMultFix = 3.5f;
const float					fRadarTileDimensions = 2000.0f;
const float					fMinusRadarTileDimensions = -2000.0f;
const float					fRadarTileDimensions2 = 7.0f;
const float					fSubtitlesWidth = 0.45f;
const float					fSubtitlesHeight = 0.9f;
const float					fTextBoxPosY = 20.0f;
const float* const			pRefFal = &fRefZVal;

static const float			fWeaponIconWidth = 75.0f;
static const float			fWeaponIconHeight = 72.0f;
/*static const float			fWLStarPosX	= HUD_POS_X - 115.25f;
static const float			fWLStarPosY = HUD_POS_Y + 53.0f;
static const float			fWLStarHeight = 1.5f;
static const float			fWLStarWidth = 0.95f;
static const float			fWLStarDistance = 20.0f;
static const float			fWLStarAlpha = HUD_TRANSPARENCY;*/

static const char			aEnglish_gxt[] = "ENGLISH.GXT";
static const char			aGerman_gxt[] = "GERMAN.GXT";
static const char			aBrazilian_gxt[] = "BRAZILIAN.GXT";
static const char			aSpanish_gxt[] = "SPANISH.GXT";
static const char			aPolish_gxt[] = "POLISH.GXT";
static const char			aHungarian_gxt[] = "HUNGARIAN.GXT";
static const char			aRomanian_gxt[] = "ROMANIAN.GXT";
static const char			aFem_24H[] = "FEM_24H";
static const char			aFem_12H[] = "FEM_12H";
static const char			aFem_Fps[] = "FEM_FPS";
static const char			aFem_Asd[] = "FEM_AS%d";
#ifdef ADD_UNFINISHED_WINDOWED_MODE
const char					aWindowed[] = "WINDOWED";
#endif

const BYTE					audioGearboxSoundTable[] = {
									0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
									1, 1, 1, 1, 0, 1, 1, 1, 1, 1,
									1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
									1, 1, 1, 1, 1, 1, 1, 0, 1, 1,
									1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
									1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
									1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
									1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
									1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
									1, 0, 1, 1, 1, 1, 1, 1, 1, 1,
									1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
									1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
									1, 1, 1, 0, 0, 1, 0, 1, 1, 1,
									1, 1, 1, 1, 1, 0, 1, 1, 1, 1,
									1, 0 };

const BYTE					CAutomobile__PreRenderCoronasTable[] = {
									0, 4, 4, 4, 4, 4,
									4, 4, 4, 0, 4, 4, 4, 1, 4, 4,
									4, 4, 4, 4, 0, 4, 4, 4, 4, 4,
									4, 4, 4, 4, 4, 2, 4, 4, 4, 4,
									4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
									4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
									4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
									4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
									4, 4, 4, 4, 4, 4, 4, 3, 4, 4,
									4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
									4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
									4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
									4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
									4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
									4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
									4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
									4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
									4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
									4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
									4, 4, 4, 0, 0, 0, 0, 4, 4, 4,
									4, 2 };

const BYTE					PCMenuActionsTable[] = {
									0, 1, 2, 3, 33, 33, 33, 33,
									33, 33, 33, 33, 33, 33, 33, 33,
									33, 33, 4, 33, 33, 33, 33, 33,
									33, 33, 33, 33, 5, 33, 33, 6,
									7, 8, 9, 10, 11, 12, 13, 14, 15,
									16, 17, 18, 19, 20, 21, 22, 23,
									24, 25, 26, 27, 28, 29, 33, 33,
									30, 31, 32, 34, 35, 36, 37, 38, 39 };

const void*	const			PCMenuActionsAddresses[] = {
									(void*)0x57D397, (void*)0x57D2FA, (void*)0x57D322,
									(void*)0x57D32B, (void*)0x57CEC7, (void*)0x57D3A5,
									(void*)0x57D418, (void*)0x57D3CD, (void*)0x57D3DD,
									(void*)0x57D3EF, (void*)0x57D401, (void*)0x57D185,
									(void*)0x57CEE1, (void*)0x57D0C6, (void*)0x57D34C,
									(void*)0x57CD8F, (void*)0x57CDAD, (void*)0x57CDCD,
									(void*)0x57CDED, (void*)0x57CE0D, (void*)0x57CE2D,
									(void*)0x57CE4D, (void*)0x57D375, (void*)0x57D25D,
									(void*)0x57D386, (void*)0x57CF9B, (void*)0x57D268,
									(void*)0x57CF5B, (void*)0x57CE6D, (void*)0x57CE9A,
									(void*)0x57D21F, (void*)0x57CF1B, (void*)0x57CF3B,
									(void*)0x57D447, Language6Action, UpdaterMenuAction,
									DLCMenuAction, ActivateSerialAction, AutoInstallUpdatesAction,
									CheckEveryAction };

const int					iRadioTracks[NUM_RADIOSTATIONS][31] = {
				{ AA_OFFSET+1, AA_OFFSET+4, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922 },
				{ CH_OFFSET+1, CH_OFFSET+4, CH_OFFSET+7, CH_OFFSET+10, CH_OFFSET+13, CH_OFFSET+16, CH_OFFSET+19, CH_OFFSET+22, CH_OFFSET+25, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922 },
				{ CO_OFFSET+1, CO_OFFSET+4, CO_OFFSET+7, CO_OFFSET+10, CO_OFFSET+13, CO_OFFSET+16, CO_OFFSET+19, CO_OFFSET+22, CO_OFFSET+25, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922 },
				{ CR_OFFSET+1, CR_OFFSET+4, CR_OFFSET+7, CR_OFFSET+10, CR_OFFSET+13, CR_OFFSET+16, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922 },
				{ DS_OFFSET+1, DS_OFFSET+4, DS_OFFSET+7, DS_OFFSET+10, DS_OFFSET+13, DS_OFFSET+16, DS_OFFSET+19, DS_OFFSET+22, DS_OFFSET+25, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922 },
				{ HC_OFFSET+1, HC_OFFSET+4, HC_OFFSET+7, HC_OFFSET+10, HC_OFFSET+13, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922 },
				{ MH_OFFSET+1, MH_OFFSET+4, MH_OFFSET+7, MH_OFFSET+10, MH_OFFSET+13, MH_OFFSET+16, MH_OFFSET+19, MH_OFFSET+22, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922 },
				{ MR_OFFSET+1, MR_OFFSET+4, MR_OFFSET+7, MR_OFFSET+10, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922 },
				{ NJ_OFFSET+1, NJ_OFFSET+4, NJ_OFFSET+7, NJ_OFFSET+10, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922 },
				{ RE_OFFSET+1, RE_OFFSET+4, RE_OFFSET+7, RE_OFFSET+10, RE_OFFSET+13, RE_OFFSET+16, RE_OFFSET+19, RE_OFFSET+22, RE_OFFSET+25, RE_OFFSET+28, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922 },
			};

const int					iRadioIntros[NUM_RADIOSTATIONS][31] = {
				{ AA_OFFSET+0, AA_OFFSET+3, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922 },
				{ CH_OFFSET+0, CH_OFFSET+3, CH_OFFSET+6, CH_OFFSET+9, CH_OFFSET+12, CH_OFFSET+15, CH_OFFSET+18, CH_OFFSET+21, CH_OFFSET+24, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922 },
				{ CO_OFFSET+0, CO_OFFSET+3, CO_OFFSET+6, CO_OFFSET+9, CO_OFFSET+12, CO_OFFSET+15, CO_OFFSET+18, CO_OFFSET+21, CO_OFFSET+24, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922 },
				{ CR_OFFSET+0, CR_OFFSET+3, CR_OFFSET+6, CR_OFFSET+9, CR_OFFSET+12, CR_OFFSET+15, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922 },
				{ DS_OFFSET+0, DS_OFFSET+3, DS_OFFSET+6, DS_OFFSET+9, DS_OFFSET+12, DS_OFFSET+15, DS_OFFSET+18, DS_OFFSET+21, DS_OFFSET+24, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922 },
				{ HC_OFFSET+0, HC_OFFSET+3, HC_OFFSET+6, HC_OFFSET+9, HC_OFFSET+12, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922 },
				{ MH_OFFSET+0, MH_OFFSET+3, MH_OFFSET+6, MH_OFFSET+9, MH_OFFSET+12, MH_OFFSET+15, MH_OFFSET+18, MH_OFFSET+21, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922 },
				{ MR_OFFSET+0, MR_OFFSET+3, MR_OFFSET+6, MR_OFFSET+9, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922 },
				{ NJ_OFFSET+0, NJ_OFFSET+3, NJ_OFFSET+6, NJ_OFFSET+9, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922 },
				{ RE_OFFSET+0, RE_OFFSET+3, RE_OFFSET+6, RE_OFFSET+9, RE_OFFSET+12, RE_OFFSET+15, RE_OFFSET+18, RE_OFFSET+21, RE_OFFSET+24, RE_OFFSET+27, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922 },
			};

const int					iRadioOutros[NUM_RADIOSTATIONS][31] = {
				{ AA_OFFSET+2, AA_OFFSET+5, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922 },
				{ CH_OFFSET+2, CH_OFFSET+5, CH_OFFSET+8, CH_OFFSET+11, CH_OFFSET+14, CH_OFFSET+17, CH_OFFSET+20, CH_OFFSET+23, CH_OFFSET+26, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922 },
				{ CO_OFFSET+2, CO_OFFSET+5, CO_OFFSET+8, CO_OFFSET+11, CO_OFFSET+14, CO_OFFSET+17, CO_OFFSET+20, CO_OFFSET+23, CO_OFFSET+26, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922 },
				{ CR_OFFSET+2, CR_OFFSET+5, CR_OFFSET+8, CR_OFFSET+11, CR_OFFSET+14, CR_OFFSET+17, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922 },
				{ DS_OFFSET+2, DS_OFFSET+5, DS_OFFSET+8, DS_OFFSET+11, DS_OFFSET+14, DS_OFFSET+17, DS_OFFSET+20, DS_OFFSET+23, DS_OFFSET+26, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922 },
				{ HC_OFFSET+2, HC_OFFSET+5, HC_OFFSET+8, HC_OFFSET+11, HC_OFFSET+14, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922 },
				{ MH_OFFSET+2, MH_OFFSET+5, MH_OFFSET+8, MH_OFFSET+11, MH_OFFSET+14, MH_OFFSET+17, MH_OFFSET+20, MH_OFFSET+23, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922 },
				{ MR_OFFSET+2, MR_OFFSET+5, MR_OFFSET+8, MR_OFFSET+11, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922 },
				{ NJ_OFFSET+2, NJ_OFFSET+5, NJ_OFFSET+8, NJ_OFFSET+11, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922 },
				{ RE_OFFSET+2, RE_OFFSET+5, RE_OFFSET+8, RE_OFFSET+11, RE_OFFSET+14, RE_OFFSET+17, RE_OFFSET+20, RE_OFFSET+23, RE_OFFSET+26, RE_OFFSET+29, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922, 1922 },
			};

const int					iAdvertsRange[2] = { ADVERTS_OFFSET, ADVERTS_OFFSET+ADVERTS_STREAMS-1 };

const short					iBeatsStreams[] = { BEATS_OFFSET+2, BEATS_OFFSET+3, BEATS_OFFSET+4, BEATS_OFFSET, BEATS_OFFSET+1 };

const CutsceneData			cutsceneStreams[] = { { "JERA1", CUTSCENE_OFFSET+CUT_JERA1 }, { "JERA2", CUTSCENE_OFFSET+CUT_JERA2 }, { "JERA3", CUTSCENE_OFFSET+CUT_JERA3 },
												{ "LOUA1", CUTSCENE_OFFSET+CUT_LOUA1 }, { "LOUA2", CUTSCENE_OFFSET+CUT_LOUA2 }, { "LOUA3", CUTSCENE_OFFSET+CUT_LOUA3 },
												{ "MARA1", CUTSCENE_OFFSET+CUT_MARA1 }, { "MARA2", CUTSCENE_OFFSET+CUT_MARA2 }, { "MARA5", CUTSCENE_OFFSET+CUT_MARA5 },
												{ "PHILA1", CUTSCENE_OFFSET+CUT_PHILA1 },  { "PHILA2", CUTSCENE_OFFSET+CUT_PHILA2 },  { "PHILA3", CUTSCENE_OFFSET+CUT_PHILA3 },  { "PHILA4", CUTSCENE_OFFSET+CUT_PHILA4 } };

const void*	const			_CText__load_Jumptable[] = { Language_CASE_English, (void*)0x6A01F1,/*Language_CASE_Spanish,*/ Language_CASE_Brazilian, Language_CASE_Polish, Language_CASE_Hungarian, Language_CASE_Romanian };
const void*	const			_CText__loadMission_Jumptable[] = { MissionLanguage_CASE_English, (void*)0x69FD14, /*MissionLanguage_CASE_Spanish,*/ MissionLanguage_CASE_Brazilian, MissionLanguage_CASE_Polish, MissionLanguage_CASE_Hungarian, MissionLanguage_CASE_Romanian };

/*const void*					HJ_Stats_Jumptable[] =  { (void*)0x55AC7E, FlamingStunt, (void*)0x55AC97,
													(void*)0x55ACB0, DoubleFlamingStunt, (void*)0x55ACC9,
													(void*)0x55ACE2, TripleFlamingStunt, (void*)0x55ACFB,
													(void*)0x55AD14, QuadrupleFlamingStunt, (void*)0x55AD2D };*/

const void*	const			ScriptCheckpointsColours_Jumptable[] = { ScriptCheckpointsColours_Normal, ScriptCheckpointsColours_Finish, ScriptCheckpointsColours_Others };
const BYTE					ScriptCheckpointsColours_IndirectTable[] = { 0, 1, 0, 2, 2, 2, 2, 2, 2 };


BOOL CALLBACK CECheck(HWND hwnd, LPARAM lParam) {
	static unsigned char		cDebuggerNames[][14] = {
		{ 'C'^0x56, 'h'^0x56, 'e'^0x56, 'a'^0x56, 't'^0x56, ' '^0x56, 'E'^0x56, 'n'^0x56, 'g'^0x56, 'i'^0x56, 'n'^0x56, 'e'^0x56, '\0'^0x56, '\0'^0xAC },
		{ 'O'^0x56, 'l'^0x56, 'l'^0x56, 'y'^0x56, 'D'^0x56, 'b'^0x56, 'g'^0x56, '\0'^0x56, '\0'^0xFF, '\0'^0x52, '\0'^0x12, '\0', '\0'^0x05, '\0'^0xCC },
		{ 'I'^0x56, 'D'^0x56, 'A'^0x56, '\0'^0x56, '\0'^0x12, '\0'^0x55, '\0'^0xAD, '\0'^0x99, '\0'^0x76, '\0'^0x43, '\0'^0xDB, '\0'^0xA0, '\0'^0x87, '\0'^0x1F } };
    static bool		bDecrypted = false;

	UNREFERENCED_PARAMETER(lParam);

	if ( !bDecrypted )
	{
		for ( BYTE i = 0; i < 3; ++i )
		{
			for ( BYTE j = 0; j < 14;  ++j )
				cDebuggerNames[i][j] ^= 0x56;
		}
		bDecrypted = true;
	}

	char			cBuffer[16];

    GetWindowText(hwnd, cBuffer, sizeof(cBuffer));
    if(strstr(cBuffer, reinterpret_cast<char*>(cDebuggerNames[0])/*"Cheat Engine"*/) || strstr(cBuffer, reinterpret_cast<char*>(cDebuggerNames[1])/*"OllyDbg"*/) || strstr(cBuffer, reinterpret_cast<char*>(cDebuggerNames[2])/*"IDA"*/) )
		ExitProcess(0);

    return TRUE;
}

#include "SpeechRecognition.h"

void OnGameTermination()
{
	CUpdateManager::InstallIfNeeded();

	CUpdateManager::Terminate();
	CDLCManager::Terminate();
	//CPNGArchive::Shutdown();
	//SpeechDeinit();
	LogToFile("Logging ended\n");

	// WORKAROUND
	ExitProcess(0);
}

DWORD WINAPI ProcessEmergencyKey(LPVOID lpParam)
{
#ifdef DEVBUILD
	bool			bKeyState = false, bFPSState = true;
	static float	fCurrentFOV = 70.0f;
#endif

	UNREFERENCED_PARAMETER(lpParam);

	LogToFile("Emergency key thread created");

	while ( !(GetKeyState(VK_PAUSE) & 0x8000) )
	{
		Sleep(250);

#ifdef MULTITHREADING_TEST
		static bool		bInitThread  = false;

		if ( !bInitThread )
		{
			if ( GetKeyState(VK_F2) & 0x8000 )
			{
				CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)0x53E9AC, nullptr, 0, nullptr);
				bInitThread = true;
			}
		}
#endif

#ifdef DEVBUILD
		if ( GetKeyState(VK_F9) & 0x8000 )
		{
			if ( !bKeyState )
			{
				bKeyState = true;
				bFPSState = bFPSState == false;
				CHud::SetFPSDisplay(bFPSState);
			}
		}
		else
		{
			if ( bKeyState )
				bKeyState = false;
		}

		if ( GetKeyState(VK_OEM_4) & 0x8000 )
		{
			fCurrentFOV -= 1.0f;
			if ( fCurrentFOV < 15.0f )
				fCurrentFOV = 15.0f;

			Memory::Patch<const void*>(0x522F3A, &fCurrentFOV);
			Memory::Patch<const void*>(0x522F5D, &fCurrentFOV);
			Memory::Patch<float>(0x522F7A, fCurrentFOV);
		}

		if ( GetKeyState(VK_OEM_6) & 0x8000 )
		{
			fCurrentFOV += 1.0f;
			if ( fCurrentFOV > 170.0f )
				fCurrentFOV = 170.0f;

			Memory::Patch<const void*>(0x522F3A, &fCurrentFOV);
			Memory::Patch<const void*>(0x522F5D, &fCurrentFOV);
			Memory::Patch<float>(0x522F7A, fCurrentFOV);
		}
#endif

#if !defined DEVBUILD && !defined COMPILE_RC
		if ( IsDebuggerPresent() )
			break;

		EnumWindows(CECheck, NULL);
#endif
	}

	LogToFile("Process has been killed on user request");
	OnGameTermination();
	return TRUE;
}

extern "C" __declspec(dllexport) BOOL OnGameLaunch()
{
	LogToFile("Launching GTA: Vice City Stories PC Edition "MOD_VERSION" \""VERSION_NAME"\" build "BUILDNUMBER_STR"...");
#ifdef DEVBUILD
	LogToFile("This is a closed dev build!");
#endif
#ifdef COMPILE_RC
	LogToFile("This is a Release Candidate "RELEASE_CANDIDATE" build");
#endif
	LogToFile("Logging started");
#ifdef COLLECT_PC_INFO
	CollectPCInfo();
#endif
	DetermineGameVer();
	DefineVariables();
	Main_Patches();
	PatchMenus();
	AnimationStylesPatching();
	UserFiles();

	// Automatic patches
	StaticPatcher::Apply();

	CreateThread(NULL, 0, ProcessEmergencyKey, 0, 0, 0);

	// DLC initialisation
	CDLCManager::Initialise();
	return TRUE;
}

#ifdef COLLECT_PC_INFO
typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

__forceinline void CollectPCInfo()
{
	LogToFile("Collecting PC specs...");
	OSVERSIONINFOEX VersionInformation;

	VersionInformation.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((LPOSVERSIONINFOA)&VersionInformation);

	if ( FILE* LogFile = fopen("pcinfo.log", "w") )
	{
		bool	bOSFailed = false;
		SYSTEMTIME systemTime;
		GetLocalTime(&systemTime);
		fprintf(LogFile, "[%02d/%02d/%d %02d:%02d:%02d] GTA: Vice City Stories PC Edition "MOD_VERSION" build "BUILDNUMBER_STR" collected system information:\n", systemTime.wDay, systemTime.wMonth, systemTime.wYear, systemTime.wHour, systemTime.wMinute, systemTime.wSecond);

		switch ( VersionInformation.dwMajorVersion )
		{
		case 5:
			fprintf(LogFile, "Operating system: Windows %s",
								( VersionInformation.dwMinorVersion == 2 )
									? ( GetSystemMetrics(SM_SERVERR2) != 0 )
										? "Server 2003 R2" : "Server 2003"
									: ( VersionInformation.dwMinorVersion == 1 )
										? "XP" : "2000");
			break;
		case 6:
			fprintf(LogFile, "Operating system: Windows %s",
								( VersionInformation.dwMinorVersion != 0 )
									? ( VersionInformation.wProductType == VER_NT_WORKSTATION )
										? "7" : "Server 2008 R2"
									: ( VersionInformation.wProductType == VER_NT_WORKSTATION )
										? "Vista" : "Server 2008");
			break;

		default:
			fprintf(LogFile, "Operating system: Couldn't identify OS version, possibly older than Windows 2000\n");
			bOSFailed = true;
		}

		if ( !bOSFailed )
			fprintf(LogFile, " Build %d\nService Pack: %s\n", VersionInformation.dwBuildNumber, VersionInformation.szCSDVersion);

		SYSTEM_INFO SystemInfo;
		GetSystemInfo(&SystemInfo);

		switch ( SystemInfo.wProcessorArchitecture )
		{
		case PROCESSOR_ARCHITECTURE_AMD64:
			fputs("CPU type: x64\n", LogFile);
			break;

		case PROCESSOR_ARCHITECTURE_IA64:
			fputs("CPU type: Intel Itanium-based\n", LogFile);
			break;

		case PROCESSOR_ARCHITECTURE_INTEL:
			fputs("CPU type: x86\n", LogFile);
			break;

		case PROCESSOR_ARCHITECTURE_UNKNOWN:
			fputs("CPU type: Unknown\n", LogFile);
			break;
		}

		BOOL bIsWow64 = false;

		LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(("kernel32")),"IsWow64Process");
		if ( fnIsWow64Process != NULL )
			fnIsWow64Process(GetCurrentProcess(),&bIsWow64);

		if ( bIsWow64 )
			fputs("CPU architecture: 64-bit", LogFile);
		else
			fputs("CPU architecture: 32-bit", LogFile);

		fclose(LogFile);
	}
}

#endif


__forceinline void DetermineGameVer()
{
	/*LogToFile("Checking game version...");
	BYTE VersionID = *(BYTE*)0x400088;

	switch ( VersionID )
	{
	case 0x8A:
		{
			GameVersion = GAMEVER_10EU_noCD;
			LogToFile("Detected game version: 1.0 EU no-CD");
			break;
		}
	case 0xCA:
		{
			GameVersion = GAMEVER_10US_noCD;
			LogToFile("Detected game version: 1.0 US no-CD");
			break;
		}
	case 0xD0:
		{
			GameVersion = GAMEVER_11EU_noCD;
			LogToFile("Detected game version: 1.01 EU no-CD");
			break;
		}
	case 0xF9:
		{
			GameVersion = GAMEVER_10EU;
			LogToFile("Detected game version: 1.0 EU");
			break;
		}
	default:
		{
			GameVersion = GAMEVER_UNKNOWN;
			LogToFile("Warning: Unknown game version!");
			break;
		}
	}
	if ( GameVersion != GAMEVER_10US_noCD && MessageBox(0, "WARNING: We've detected that you are trying to use an incompatible EXE. You can still try to play the game but we do not accept error reports or feedback from this version. We recommend you try to use one of these EXE's instead:\n\n\t- 1.0 US HOODLUM (14 383 616 bytes)\n\t- 1.0 US compact (5 189 632 bytes)\n\nDo you want to launch the game anyway?", "GTA: Vice City Stories", MB_ICONERROR | MB_YESNO | MB_DEFBUTTON2 ) == IDNO )
		ExitProcess(0);*/

	// Colormod check
	LogToFile("Color Mod loaded: %s", GetModuleHandle("colormod") != 0 ? "YES" : "NO");
}

__forceinline void DefineVariables()
{
	LogToFile("Assigning variables...");

	_ProcessEnexMarkers_JumpBack = (void*)0x440F43;
	//CGameLogic__Update_Busted_JumpBack = (void*)0x442D34;
	//CGameLogic__Update_Wasted_JumpBack = (void*)0x442D0B;
	CMenu__DrawRadioStationIconsIconBreak_ElseJump = (void*)0x5747E8;
	CMenu__DrawRadioStationIconsIconBreak_JumpBack = (void*)0x57471D;
	AssignCopToVehicleSwitch_JumpBack = (void*)0x4613E9;
	ViceSquadCheckInjectC_JumpBack = (void*)0x421988;
	func_0602_JumpBack = (void*)0x4912DC;
	func_0602_ElseJump = (void*)0x4912E1;
	TaxiCabNitro_JumpBack = (void*)0x6B19B2;
	TaxiCabNitro_ElseJump = (void*)0x6B19B7;
	ZebraCabCPlayerStuff_JumpBack = (void*)0x56F93F;
	ZebraCabCPlayerStuff_ElseJump = (void*)0x56F996;
	func_610310_JumpBack = (void*)0x61038B;
	func_610310_ElseJump = (void*)0x610393;
	func_6B4800_JumpBack = (void*)0x6B4AF3;
	func_6B4800_ElseJump = (void*)0x6B4B95;
	func_6D1AA0_JumpBack = (void*)0x6D1ACC;
	func_6D1AA0_ElseJump = (void*)0x6D1ACE;
	Clock_SwitchInject_JumpBack = (void*)0x57708B;
	Clock_StringInject_JumpBack = (void*)0x57A161;
	Clock_StringInject_JumpToOFF = (void*)0x579F65;
	ScriptCheckpointsColours_JumpBack = (void*)0x493651;
	BigMessage1Inject_JumpBack = (void*)0x58C8B7;
	ExtraWeaponAnimationsInject_JumpBack = (void*)0x609A3B;
	ExtraWeaponAnimationsInject_NoArmed = (void*)0x6099E4;
	WeaponSounds_JumpBack = (void*)0x505132;
//	MultipleTitlePCInject_JumpBack = (void*)0x5901C2;
	DeleteSecondSplash_JumpBack = (void*)0x748AD4;
	RestoreSecondSplash_JumpBack = (void*)0x748DA3;
	InjectPedMapping_JumpBack = (void*)0x5E768D;
	NodeCrashFix_JumpBack = (void*)0x552CD6;
	NodeCrashFix2_JumpBack = (void*)0x4076F4;
	TimestampSignatureHack_JumpBack = (void*)0x5D19D3;
	TimestampSignatureHack_CheckFailed = (void*)0x5D1A2F;
	SaveFallback_InjectOnLoad_JumpBack = (void*)0x5D1893;
	SaveFallback_InjectOnSave_ReturnTrue = (void*)0x5D14A3;
	SaveFallback_InjectOnSave_ReturnFalse = (void*)0x5D15AB;
	SaveFallback_FallbackPreBeta3Names_JumpBack = (void*)0x6191EE;
	LoadGameFailedMessage_JumpBack = (void*)0x53C74B;
	MenuEntriesAlignHack_JumpBack = (void*)0x57A21F;
	MenuEntriesLeftColumnHack2_JumpBack = (void*)0x57A1BA;
	MenuEntriesPositionHack_JumpBack = (void*)0x57A3F1;
	WidescreenFOVHack_JumpBack = (void*)0x72FD5A;
	Widescreen_SwitchInject_JumpBack = (void*)0x577078;
	Widescreen_TextDrawsFix2_JumpBack = (void*)0x58C229;
	DriveByKillFix_ReturnTrue = (void*)0x43DE05;
	DriveByKillFix_ReturnFalse = (void*)0x43DE61;
	ZeroScriptsCounter_JumpBack = (void*)0x468D55;
	func_01C6_JumpBack = (void*)0x47D7EC;
	RightShockKeyHack_JumpBack = (void*)0x52FA42;
	NoRadioCommercialsHack_JumpBack = (void*)0x4EA675;
	VideoPlayerCreate1_JumpBack = (void*)0x748B08;
	VideoPlayerPlayNextFrame_JumpBack = (void*)0x748DA3;
	VideoPlayerProc_JumpBack = (void*)0x74817E;
	VideoPlayerRelease_JumpBack = (void*)0x748C21;
	FrameLimit_SwitchInject_JumpBack = (void*)0x57CECF;
	FrameLimit_StringInject_JumpBack = (void*)0x57A168;
	/*LoadFontsHack_JumpBack = (void*)0x5BA6E5;
	ReleaseFontsHack_JumpBack = (void*)0x7189C6;*/
	SetCutsceneModelHack_JumpBack = (void*)0x5B10DE;
#ifdef INCLUDE_MULTIFONTFILES
	MultipleFontsDAT_JumpBack = (void*)0x7187DB;
	MultipleFontsTXD_JumpBack = (void*)0x5BA6A3;
#endif

	scriptParams = (SCRIPT_VAR*)0xA43C78;
	scriptLocals = (SCRIPT_VAR*)0xA48960;
	scmBlock = (void*)0xA49960;
	CPopulation__allRandomPedThisType = (DWORD*)0x8D2534;
	PlayerEnexEntryStage = (DWORD*)0x96A7CC;
	activeInterior = (DWORD*)0xB72914;
	memoryAvailable = (DWORD*)0x8A5A80;
	memoryUsed = (DWORD*)0x8E4CB4;
	wFlashingComponentID = (eFlash*)0xBAB1DC;
	bWants_To_Draw_Hud = (BYTE*)0xA444A0;
	radarGrey = (BYTE*)0xA444A4;
	WidthAspectRatio = (float*)0x859520;
	HeightAspectRatio = (float*)0x859524;
	fFOV = (float*)0x8D5038;
	bHideStyledTextWhileFading = (bool*)0xA44489;
	latestMissionName = (char*)0xB78A00;
	StyledText_1 = (char*)0xBAACC0;
	StyledText_2 = (char*)0xBAAD40;
	StyledText_3 = (char*)0xBAADC0;
	StyledText_4 = (char*)0xBAAE40;
	StyledText_5 = (char*)0xBAAEC0;
	StyledText_6 = (char*)0xBAAF40;
	StyledText_7 = (char*)0xBAAFC0;
	PriorityText = (char*)0xBAB040;
	Garage_MessageIDString = (char*)0x96C014;
	currentFPS = (float*)0xB7CB50;
	BaseColors = (CRGBA*)0xBAB22C;
	arrowClump = (RpClump**)0xC7C6F0;
	blurStages = (CBlurStage*)0x8D5190;
	//gridref = (CGridref*)0xC72FB0;
	garages = (CGarages*)0x96C00C;
	animationStyles = (CAnimationStyleDescriptor*)0x8AA5A8;
	activeKeyState = (CKeyState*)0xB72CB0;
	currKeyState = (CKeyState*)0xB73190;
	prevKeyState = (CKeyState*)0xB72F20;
//	texFonts = (CSprite2d*)0xC71AD0;
	pActiveScripts = (CRunningScript**)0xA8B42C;
	ScriptsArray = (CRunningScript*)0xA8B430;
	gameState = (DWORD*)0xC8D4C0;

	replacedTXDLoadFunc = (void(*)())*((int*)0x5BD766);
	*(int*)&replacedTXDLoadFunc += (int)0x5BD765 + 5;

	replacedTXDReleaseFunc = (void(*)())*((int*)0x53BBCC);
	*(int*)&replacedTXDReleaseFunc += (int)0x53BBCB + 5;

	/*return;
		}
	default:
		{
			LogToFile("This game version is not supported yet");
			LogToFile("TODO: Add support");
		}
	}*/
}

/*__forceinline void CheckStructures()
{
	LogToFile("Checking class sizes...");

	assert(sizeof(CMenuItem) == CMenuItem_ARRAYSIZE);
	assert(sizeof(CEntryData) == CEntryData_ARRAYSIZE);
	assert(sizeof(CPed) == CPed_ARRAYSIZE);
	assert(sizeof(CWeaponSlot) == CWeaponSlot_ARRAYSIZE);
	assert(sizeof(CRunningScript) == CRunningScript_ARRAYSIZE);
	assert(sizeof(CPlayerInfo) == CPlayer_ARRAYSIZE);
	assert(sizeof(CWeaponInfo) == CWeaponInfo_ARRAYSIZE);
	assert(sizeof(CDarkel) == CDarkel_ARRAYSIZE);
	assert(sizeof(RwDevice) == RwDevice_ARRAYSIZE);
	assert(sizeof(RwEngineInstance) == RwEngineInstance_ARRAYSIZE);
	assert(sizeof(CWanted) == CWanted_ARRAYSIZE);
	assert(sizeof(CPhysical) == CPhysical_ARRAYSIZE);
	assert(sizeof(CEntity) == CEntity_ARRAYSIZE);
	assert(sizeof(CPlaceable) == CPlaceable_ARRAYSIZE);
	assert(sizeof(LoadedObjectInfo) == LoadedObjectInfo_ARRAYSIZE);
	assert(sizeof(CClock) == CClock_ARRAYSIZE);
	assert(sizeof(CMenuManager) == CMenuManager_ARRAYSIZE);
	assert(sizeof(CVehicle) == CVehicle_ARRAYSIZE);
	assert(sizeof(CPlayerPedData) == CPlayerData_ARRAYSIZE);
	assert(sizeof(CGridref) == CGridref_ARRAYSIZE);
	assert(sizeof(COnscreenTimer) == COnscreenTimer_ARRAYSIZE);
	/*if ( sizeof(CMenuItem) != CMenuItem_ARRAYSIZE )
		LogToFile("Warning: MenuItem class has wrong size - 0x%X", sizeof(CMenuItem));

	if ( sizeof(CEntryData) != CEntryData_ARRAYSIZE )
		LogToFile("Warning: EntryData class has wrong size - 0x%X", sizeof(CEntryData));

	if ( sizeof(CPed) != CPed_ARRAYSIZE )
		LogToFile("Warning: Ped class has wrong size - 0x%X", sizeof(CPed));

	if ( sizeof(CWeaponSlot) != CWeaponSlot_ARRAYSIZE )
		LogToFile("Warning: WeaponSlot structure has wrong size - 0x%X", sizeof(CWeaponSlot));

	if ( sizeof(CRunningScript) != CRunningScript_ARRAYSIZE )
		LogToFile("Warning: RunningScript class has wrong size - 0x%X", sizeof(CRunningScript));

	if ( sizeof(CPlayerInfo) != CPlayer_ARRAYSIZE )
		LogToFile("Warning: Player class has wrong size - 0x%X", sizeof(CPlayerInfo));

	if ( sizeof(CWeaponInfo) != CWeaponInfo_ARRAYSIZE )
		LogToFile("Warning: WeaponInfo class has wrong size - 0x%X", sizeof(CWeaponInfo));

	if ( sizeof(CDarkel) != CDarkel_ARRAYSIZE )
		LogToFile("Warning: Darkel class has wrong size - 0x%X", sizeof(CDarkel));

	if ( sizeof(RwDevice) != RwDevice_ARRAYSIZE )
		LogToFile("Warning: RwDevice structure has wrong size - 0x%X", sizeof(RwDevice));

	if ( sizeof(RwEngineInstance) != RwEngineInstance_ARRAYSIZE )
		LogToFile("Warning: RwEngineInstance structure has wrong size - 0x%X", sizeof(RwEngineInstance));

	if ( sizeof(CWanted) != CWanted_ARRAYSIZE )
		LogToFile("Warning: Wanted class has wrong size - 0x%X", sizeof(CWanted));

	if ( sizeof(CPhysical) != CPhysical_ARRAYSIZE )
		LogToFile("Warning: Physical class has wrong size - 0x%X", sizeof(CPhysical));

	if ( sizeof(CEntity) != CEntity_ARRAYSIZE )
		LogToFile("Warning: Entity class has wrong size - 0x%X", sizeof(CEntity));

	if ( sizeof(CPlaceable) != CPlaceable_ARRAYSIZE )
		LogToFile("Warning: Placeable class has wrong size - 0x%X", sizeof(CPlaceable));

	if ( sizeof(LoadedObjectInfo) != LoadedObjectInfo_ARRAYSIZE )
		LogToFile("Warning: LoadedObjectInfo structure has wrong size - 0x%X", sizeof(LoadedObjectInfo));

	if ( sizeof(CClock) != CClock_ARRAYSIZE )
		LogToFile("Warning: Clock class has wrong size - 0x%X", sizeof(CClock));

	if ( sizeof(CMenuManager) != CMenuManager_ARRAYSIZE )
		LogToFile("Warning: MenuManager class has wrong size - 0x%X", sizeof(CMenuManager));

	if ( sizeof(CVehicle) != CVehicle_ARRAYSIZE )
		LogToFile("Warning: Vehicle class has wrong size - 0x%X", sizeof(CVehicle));

	if ( sizeof(CPlayerPedData) != CPlayerData_ARRAYSIZE )
		LogToFile("Warning: PlayerData class has wrong size - 0x%X", sizeof(CPlayerPedData));

	if ( sizeof(CGridref) != CGridref_ARRAYSIZE )
		LogToFile("Warning: Gridref class has wrong size - 0x%X", sizeof(CGridref));

	if ( sizeof(COnscreenTimer) != COnscreenTimer_ARRAYSIZE )
		LogToFile("Warning: OnscreenTimer class has wrong size - 0x%X", sizeof(COnscreenTimer));
}*/

#ifdef INCLUDE_AUDIO_DEBUG

void __declspec(naked) GetDamnStack()
{
	if ( bShouldEcho )
	{
		_asm
		{
			mov		eax, [esp]
			mov		stackPtr, eax
		}
		LogToFile("CAudio::ReportFrontendAudioEvent called from 0x%X", stackPtr);
		_asm
		{
			ret 0Ch
		}
	}
	else
	{
		_asm
		{
			add     ecx, 0B4h
			mov		eax, 4DD4A0h
			jmp     eax
		}
	}
}

#endif

#ifdef EXTRA_ASH_LOGGING
#pragma message ("DEBUG: Including extra Ash logging...")

void __declspec(naked) GetTreeNodeFuncStuff()
{
	_asm
	{
		mov		eax, [esp]
		mov		stackPtr, eax
		pushad
	}
	LogToFile("sub_552CD0 called from 0x%X", stackPtr);
	_asm
	{
		popad
		sub		esp, 10h
		push	esi
		mov		esi, ecx
		mov		eax, 552CD6h
		jmp		eax
	}
}

void __declspec(naked) GetPedgrp()
{
	_asm
	{
		lea		ebx, [esp+52Ch-400h]
		pushad
		push	ebx
		call	LogToFile
		add		esp, 4
		popad
		add		esp, 0Ch
		test	al, al
		mov		eax, 5BD183h
		jmp		eax
	}
}

#endif

#ifdef TRACE_D3D_MARKER
#pragma message ("DEBUG: Including marker tracer...")

void __declspec(naked) TraceMarker()
{
	_asm
	{
		mov		eax, [esp]
		mov		stackPtr, eax
		pushad
	}
	LogToFile("C3DMarker::PlaceMarker called from 0x%X", stackPtr);
	_asm
	{
		popad
		sub		esp, 58h
		push	ebx
		push	ebp
		mov		eax, 725125h
		jmp		eax
	}
}

#endif

#ifdef EXTRA_WINDOWMODE_CHECKING
#pragma message ("DEBUG: Including Rw tracer...")

void __declspec(naked) TraceD3DStuff()
{
	_asm
	{
		mov		eax, [esp]
		mov		stackPtr, eax
		pushad
	}
	LogToFile("_D3D9System called from 0x%X", stackPtr);
	_asm
	{
		popad
		mov     eax, [esp+4]
		sub     esp, 470h
		mov		ecx, 7F5F7Ah
		jmp		ecx
	}
}

#endif

#ifdef TRACE_PROJECTILES
#pragma message ("DEBUG: Including CProjectileInfo tracer...")

void TraceProjectiles(CEntity* projectileOwner, int unkType, float posX, float posY, float posZ, float unkAngle, CVector* unkVector, CEntity* target)
{
	LogToFile("CProjectileInfo::AddProjectile(owner 0x%X, type %d, posX %f, posY %f, posZ %f, angle %f, vector %f %f %f, target 0x%X)", projectileOwner, unkType, posX, posY, posZ, unkAngle, (unkVector != 0) ? unkVector->x : 0.0, (unkVector != 0) ? unkVector->y : 0.0, (unkVector != 0) ? unkVector->z : 0.0, target);
	/*_asm
	{
		push	0FFh
		push	848DF3h
		mov		eax, 737C87h
		jmp		eax
	}*/
}

#endif

#ifdef TRACE_COLOUR_STUFF
#pragma message ("DEBUG: Including material ID tracer...")

void __declspec(naked) TraceMaterials()
{
	_asm
	{
		mov		stackPtr, eax
		pushad
	}
	LogToFile("Material: %X", stackPtr);
	_asm
	{
		popad
		mov     edi, [ecx]
		mov     [edi], edx
		mov     ebx, [ecx]
		//cmp     esi, 0FF3Ch
		push	4C837Fh
		retn
	}
}
#endif

#ifdef TRACE_STREAMING
#pragma message ("DEBUG: Including streaming tracer...")

void __declspec(naked) TraceStreaming_Requestmodel()
{
	_asm
	{
		mov		eax, [esp+4]
		mov		stackPtr, eax
		pushad
	}
	LogToFile("CStreaming::RequestModel(%d)", stackPtr);
	_asm
	{
		popad
		push    ebx
		mov     ebx, [esp+4+8]
		push	4087E5h
		retn
	}
}

#endif

#ifdef TRACE_CTEXT
#pragma message ("DEBUG: Including CText::Load tracer...")

void __declspec(naked) TraceCText_Load()
{
	_asm
	{
		mov		eax, [esp]
		mov		stackPtr, eax
		pushad
	}
	LogToFile("CText::Load called from 0x%X", stackPtr);
	_asm
	{
		popad
		sub		esp, 28h
		mov     eax, [esp+28h+4]
		push	6A01A7h
		retn
	}
}

void __declspec(naked) TraceInterface()
{
	_asm
	{
		pushad
	}
	LogToFile("Interface textures loaded");
	_asm
	{
		popad
		push	5BA690h
		retn
	}
}

#endif

#ifdef TRACE_UPCASE

void __declspec(naked) TraceUpcase_Pseudo(DWORD character, DWORD unknown)
{
	_asm
	{
		cmp byte ptr [esp+8], 1
		push 7192C5h
		retn
	}
}

void __declspec(naked) TraceUpcase(DWORD character, DWORD unknown)
{
	_asm
	{
		push	ecx
		mov		ecx, unknown
		push	ecx
		mov		ecx, character
		push	ecx
		call	TraceUpcase_Pseudo
		add		esp, 8
		movzx	ecx, al
		mov		stackPtr, ecx
		pop		ecx
		pushad
	}
	LogToFile("Original upcase params: %d %d Returned: %d", character, unknown, stackPtr);
	_asm
	{
		popad
		retn
	}
}

#endif

#ifdef TRACE_NODES_STREAMS

void __declspec(naked) TraceNodes()
{
	_asm
	{
		mov		esi, edi
		imul    esi, 98h
		mov		stackPtr, edi
		pushad
	}
	if ( !stackPtr )
		LogToFile("Node stream ID: 0");
	_asm
	{
		popad
		push	4076F4h
		retn
	}
}

#endif

#ifdef TRACE_STREAMS

unsigned int	soundID;
signed int		bankSlotID;
unsigned int	ret1;
unsigned int	ret2;

unsigned short	audioToPlay = 4;
signed short	slotToPlay = 1;
bool			bReplace = false;

void __declspec(naked) TraceStreamPointers()
{
	_asm
	{
		movzx	eax, word ptr [esp]
		mov		soundID, eax
		movsx	eax, word ptr [esp+4]
		mov		bankSlotID, eax
		cmp		bReplace, 1
		jne		TraceStreamPointers__NoReplace
		mov		ax, audioToPlay
		mov		word ptr [esp], ax
		mov		ax, slotToPlay
		mov		word ptr [esp+4], ax

TraceStreamPointers__NoReplace:
		mov		eax, 4E0280h
		call	eax
		mov		stackPtr, eax
		mov		ecx, [esp+14h+8]
		mov		ret1, ecx
		mov		cx, [esp+14h+0Ch]
		movzx	ecx, cx
		mov		ret2, ecx
	//	mov		word ptr [esp+14h+0Ch], 100
		pushad
	}
	LogToFile("Stream bank: %d sound: %d returned: 0x%X %d %d", bankSlotID, soundID, stackPtr, ret1, ret2);
	_asm
	{
		popad
		push	4D86FDh
		retn
	}
}

#endif

#ifdef TRACE_STREAMING2

static int	nHowMuchIWant, nWhoAmI;

void __declspec(naked) TraceStreamingRequests()
{
	_asm
	{
		mov		nHowMuchIWant, ecx
		mov		edi, memoryUsed
		mov		[edi], eax
		mov		eax, [esp+30h+8h]
		mov		nWhoAmI, eax
		pushad
	}
	if ( nWhoAmI != -1 )
		LogToFile("%d wants %dKB of memory", nWhoAmI, nHowMuchIWant / 1024);
	_asm
	{
		popad
		push	40CB88h
		retn
	}
}

#endif

#ifdef TRACE_FUCKING_TEXTURES

void __declspec(naked) TraceTXDLoads()
{
	static int		nTheRequest;
	static char*	cTheName;
	_asm
	{
		mov		nTheRequest, eax
//		mov		dword ptr [esp+4Ch-3Ch], eax
//		lea		edx, [ecx+8]
		lea		edx, [esp+58h-20h+8]
		mov		cTheName, edx
		pushad
	}
	LogToFile("%s loaded as %d", cTheName, nTheRequest);
	_asm
	{
		popad
		push	4075A0h
		retn
	}
}

void __declspec(naked) TraceTXDLoads2()
{
	static int		nSlot;
	static char*	cName;
	_asm
	{
		mov		nSlot, eax
		mov		ecx, [esp+4]
		mov		cName, ecx
		pushad
	}
	LogToFile("%s.txd got ID %d", cName, nSlot);
	_asm
	{
		popad
		retn
	}
}

#endif

#ifdef INCLUDE_SQUARE_RADAR

long double SquareRadar(CVector2D& pos)
{
	long double	dResult = sqrt(pos.x * pos.x + pos.y * pos.y);

	if ( dResult > M_SQRT2/2.0 )
	{
		pos.x /= dResult * M_SQRT1_2;
		pos.y /= dResult * M_SQRT1_2;
	}
	if ( pos.x > 1.0 )
		pos.x = 1.0;
	else if ( -1.0 > pos.x )
		pos.x = -1.0;
	if ( pos.y > 1.0 )
		pos.y = 1.0;
	else if ( -1.0 > pos.y )
		pos.y = -1.0;

	return dResult;
}

#endif

#ifdef V_RADAR_TEST

void __declspec(naked) Draw3DRadarHook()
{
	_asm
	{
		push		edi
		push		esi
		call		CRadar::Draw3DRadar
		add			esp, 30h
		pop			edi
		pop			esi
		pop			ebp
		pop			ebx
		add			esp, 2Ch
		retn
	}
}

void __declspec(naked) RadarTransformHook()
{
	_asm
	{
		push	edx
		mov		edx, [esp+4+8]
		push	edx
		push	eax
		call	CRadar::TransformRadarPointToScreenSpace
		add		esp, 8
		pop		edx
		retn
	}
}

#endif

#ifdef INCLUDE_TRIANGLE_RADAR

long double TriangleRadar(CVector2D& pos)
{
	long double	dResult = sqrt(pos.x * pos.x + pos.y * pos.y);

	if ( dResult > 1.1547005383792515290182975610039 )
	{
		pos.x /= dResult * (1.0/1.1547005383792515290182975610039);
		pos.y /= dResult * (1.0/1.1547005383792515290182975610039);
	}

	if ( pos.x > 1.0 )
		pos.x = 1.0;
	else if ( -1.0 > pos.x )
		pos.x = -1.0;

	if ( -1.0 > pos.y )
		pos.y = -1.0;

	if ( pos.x > 0.0 )
	{
		if ( pos.y > ((-2.0 * pos.x) + 1.0) )
			pos.y = (-2.0 * pos.x) + 1.0;
	}
	else
	{
		if ( pos.y > ((2.0 * pos.x) + 1.0) )
			pos.y = (2.0 * pos.x) + 1.0;
	}

	return dResult;
}

void __declspec(naked) TriangleRadarMask()
{
	static const float fPositions[] = { 0.0, 1.0, 0.0, 1.0, 1.0, -1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0 };
	_asm
	{
		lea		eax, [ebx*8+fPositions-8]
		lea		ecx, [esp+38h]
		push	eax
		push	ecx
		mov		eax, 583480h
		call	eax
		lea		eax, [ebx*8+fPositions+8]
		lea		ecx, [esp+48h]
		push	eax
		push	ecx
		mov		eax, 583480h
		call	eax
		lea		eax, [ebx*8+fPositions+18h]
		lea		ecx, [esp+58h]
		push	eax
		push	ecx
		mov		eax, 583480h
		call	eax
		add		esp, 18h
		push	585888h
		retn
	}
}


#endif

/*static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

DWORD WINAPI DoInternetConnectionTests(LPVOID lpParam)
{
	curl_global_init(CURL_GLOBAL_DEFAULT);

	CURL* curl_handle = curl_easy_init();
	curl_easy_setopt(curl_handle, CURLOPT_URL, "http://dl.dropbox.com/u/46581994/the_file.txt");

	curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);

	curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);

	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);

	if ( FILE* pagefile = fopen("my_output.txt", "w") )
	{
		curl_easy_setopt(curl_handle, CURLOPT_FILE, pagefile);

		curl_easy_perform(curl_handle);

		fclose(pagefile);
	}

	curl_easy_cleanup(curl_handle);


	return TRUE;
}*/

/*LONG CALLBACK ExecHandle(EXCEPTION_POINTERS* ep)
{
	char code[128];

	switch (ep->ExceptionRecord->ExceptionCode)
	{
		case EXCEPTION_ACCESS_VIOLATION:
			sprintf(code, "0x%x (EXCEPTION_ACCESS_VIOLATION)\r\n", ep->ExceptionRecord->ExceptionCode);
			break;
		case EXCEPTION_ILLEGAL_INSTRUCTION:
			sprintf(code, "0x%x (EXCEPTION_ILLEGAL_INSTRUCTION)\r\n", ep->ExceptionRecord->ExceptionCode);
			break;
		case EXCEPTION_INT_DIVIDE_BY_ZERO:
			sprintf(code, "0x%x (EXCEPTION_INT_DIVIDE_BY_ZERO)\r\n", ep->ExceptionRecord->ExceptionCode);
			break;
		case EXCEPTION_INT_OVERFLOW:
			sprintf(code, "0x%x (EXCEPTION_INT_OVERFLOW)\r\n", ep->ExceptionRecord->ExceptionCode);
			break;
		case EXCEPTION_FLT_UNDERFLOW:
			sprintf(code, "0x%x (EXCEPTION_FLT_UNDERFLOW)\r\n", ep->ExceptionRecord->ExceptionCode);
			break;
		case EXCEPTION_FLT_INEXACT_RESULT:
			sprintf(code, "0x%x (EXCEPTION_FLT_INEXACT_RESULT)\r\n", ep->ExceptionRecord->ExceptionCode);
			break;
		default:
			sprintf(code, "0x%x (UNKNOWN)\r\n", ep->ExceptionRecord->ExceptionCode);
			break;
	}
	/*char* szVersion;
	switch ( GameVersion )
	{
	case GAMEVER_10EU_noCD:
		szVersion = "1.0 EU no-CD";
		break;
	case GAMEVER_10US_noCD:
		szVersion = "1.0 US no-CD";
		break;
	case GAMEVER_11EU_noCD:
		szVersion = "1.01 EU no-CD";
		break;
	case GAMEVER_10EU:
		szVersion = "1.0 EU";
		break;
	default:
		szVersion = "Unknown";
		break;
	}*/
	/*if(iVersion == NULL) szVersion = "Unknown Version";
	else if(iVersion == VERSION_US_1) szVersion = "US 1.0";
	else if(iVersion == VERSION_EU_1) szVersion = "EU 1.0";*/

/*	CHAR szErrorString[1024];

	sprintf(szErrorString,
		"GTA: VCS PC Crashed, Debug Info Follows:\n\n"
		"GTA Info:\n\n"
		"Assembly Info:\n\n"
		"Exception At Address: 0x%08X\n"
		"Exception Code: %s\n"
		"Registers:\n"
		"EAX: 0x%08X\tEBX: 0x%08X\nECX: 0x%08X\tEDX: 0x%08X\n"
		"ESI: 0x%08X\tEDI: 0x%08X\nEBP: 0x%08X\tESP: 0x%08X\n"
		"EFlags: 0x%08X\n",
		ep->ContextRecord->Eip,
		code,
		ep->ContextRecord->Eax,
		ep->ContextRecord->Ebx,
		ep->ContextRecord->Ecx,
		ep->ContextRecord->Edx,
		ep->ContextRecord->Esi,
		ep->ContextRecord->Edi,
		ep->ContextRecord->Ebp,
		ep->ContextRecord->Esp,
		ep->ContextRecord->EFlags);

	if ( FILE* LogFile = fopen("VCSPC.log", "a") )
	{
		SYSTEMTIME	systemTime;

		GetLocalTime(&systemTime);
		fprintf(LogFile, "[%02d/%02d/%d %02d:%02d:%02d] ", systemTime.wDay, systemTime.wMonth, systemTime.wYear, systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
		fputs(szErrorString, LogFile);
		fputc('\n', LogFile);
		fclose(LogFile);
	}
	strcat(szErrorString, "\nCrash info has been dumped into VCSPC.LOG file, please contact mod developers for more information.");

	ShowCursor(true);
	HWND hWnd = FindWindow(0, "");
	SetForegroundWindow(hWnd);
	ShowCursor(true);
	MessageBox(hWnd, szErrorString, "GTA: Vice City Stories", 0);
	HWND HWNDDD = FindWindow(0, "");
	SetForegroundWindow(HWNDDD);

//	FILE* fHandle = fopen("errordump.txt","w");
//	fputs(szErrorString,fHandle);
//	fclose(fHandle);
	ExitProcess(0);
	return NULL;
}*/

/*LONG WINAPI CustomUnhandledExceptionFilter(LPEXCEPTION_POINTERS ExceptionInfo)
{
        // step 1: write minidump
        char error[1024];
        char filename[MAX_PATH];
        __time64_t time;
        tm* ltime;

        _time64(&time);
        ltime = _localtime64(&time);
        strftime(filename, sizeof(filename) - 1, "gta-vcs-%Y%m%d%H%M%S.dmp", ltime);
        _snprintf(error, sizeof(error) - 1, "A minidump has been written to %s. Please contact mod developers for more information.", filename);

        HANDLE hFile = CreateFile(filename, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

        if (hFile != INVALID_HANDLE_VALUE)
        {
                MINIDUMP_EXCEPTION_INFORMATION ex;
                memset(&ex, 0, sizeof(ex));
                ex.ThreadId = GetCurrentThreadId();
                ex.ExceptionPointers = ExceptionInfo;
                ex.ClientPointers = FALSE;

                if (FAILED(MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ex, NULL, NULL)))
                {
                        _snprintf(error, sizeof(error) - 1, "An error (0x%X) occurred during writing %s.", GetLastError(), filename);
                }

                CloseHandle(hFile);
        }
        else
        {
                _snprintf(error, sizeof(error) - 1, "An error (0x%X) occurred during creating %s.", GetLastError(), filename);
        }

        // step 2: exit the application
		char	errorCode[1024];
		sprintf(errorCode, "Fatal error (0x%08X) at 0x%08X.\n%s", ExceptionInfo->ExceptionRecord->ExceptionCode, ExceptionInfo->ExceptionRecord->ExceptionAddress, error);
		ShowCursor(true);
		HWND hWnd = FindWindow(0, "");
		SetForegroundWindow(hWnd);
		ShowCursor(true);
		MessageBox(NULL, errorCode, "GTA: Vice City Stories", MB_ICONERROR | MB_OK );
		HWND HWNDDD = FindWindow(0, "");
		SetForegroundWindow(HWNDDD);

		ExitProcess(0);
        return 0;
}*/

#ifdef INCLUDE_COOL_BINK_SCREEN

void __declspec(naked) PlayerVid()
{
	_asm push	eax
	CVideoPlayer::Release();
	CVideoPlayer::Create("MOVIES\\SCREENVID.BIK");
	_asm
	{
		pop		eax
		retn	8
	}
}

RpMaterial* RenderVideo3(RpMaterial* pMaterial, void* map)
{
	RwTexture*	myTex = RpMaterialGetTexture(pMaterial);
	_asm
	{
		push	map
		push	myTex
		mov		eax, 7F35D0h
		call	eax
		add		esp, 8
	}
	return pMaterial;
}

RpAtomic* RenderVideo2(RpAtomic* pAtomic, void* map)
{
	if ( RpAtomicGetFlags(pAtomic) & rpATOMICRENDER )
		RpGeometryForAllMaterials(RpAtomicGetGeometry(pAtomic), RenderVideo3, map);
	return pAtomic;
}

RpClump* RenderVid(CEntity* ent)
{
	if ( ent->ModelIndex() == 1343 )
	{
		if ( ent->m_pRwObject )
		{
			if ( CVideoPlayer::m_hBinkPlayer )
			{
				if ( !BinkWait(CVideoPlayer::m_hBinkPlayer) )
				{
					if ( BinkDoFrameAsyncWait(CVideoPlayer::m_hBinkPlayer, 1000) )
					{
						if ( RwRasterLock(CVideoPlayer::m_pVideoRaster, 0, rwRASTERLOCKREADWRITE)
							&& BinkBufferLock(CVideoPlayer::m_hBinkBuffer) )
						{
							BinkCopyToBuffer(CVideoPlayer::m_hBinkPlayer, CVideoPlayer::m_pVideoRaster->cpPixels, CVideoPlayer::m_hBinkBuffer->BufferPitch, CVideoPlayer::m_hBinkBuffer->Height, 0, 0, (CVideoPlayer::m_hBinkBuffer->SurfaceType & (~BINKSURFACEMASK)) | CVideoPlayer::m_bSurfaceMask);
							RwRasterUnlock(CVideoPlayer::m_pVideoRaster);
							BinkBufferUnlock(CVideoPlayer::m_hBinkBuffer);

							RpGeometryForAllMaterials(RpAtomicGetGeometry(reinterpret_cast<RpAtomic*>(ent->m_pRwObject)), RenderVideo3, CVideoPlayer::m_pVideoRaster);
						}

						if ( CVideoPlayer::m_hBinkPlayer->LastFrameNum >= CVideoPlayer::m_hBinkPlayer->Frames )
							BinkGoto(CVideoPlayer::m_hBinkPlayer, 1, NULL);
						else
							BinkNextFrame(CVideoPlayer::m_hBinkPlayer);

						if ( BinkShouldSkip(CVideoPlayer::m_hBinkPlayer) )
						{
							BinkDoFrameAsync(CVideoPlayer::m_hBinkPlayer, 0, CVideoPlayer::m_bExtraThreadIndex);
							BinkDoFrameAsyncWait(CVideoPlayer::m_hBinkPlayer, -1);
							BinkNextFrame(CVideoPlayer::m_hBinkPlayer);
						}

						BinkDoFrameAsync(CVideoPlayer::m_hBinkPlayer, 0, CVideoPlayer::m_bExtraThreadIndex);
					}
				}
			}
		}

	}
	return ent->m_pRwObject;
}

void __declspec(naked) RenderHook()
{
	_asm
	{
		push	ecx
		push	esi
		mov		esi, ecx
		push	esi
		call	RenderVid
		add		esp, 4
		push	534317h
		retn
	}
}

#endif

#ifdef LIST_MEMORY

void ListMemory()
{
	unsigned int* theAddress = (unsigned int*)0x57D454;
	for ( int i = 0; i < 34; ++i )
	{
		LogToFile("(void*)0x%X, ", *theAddress);
		++theAddress;
	}
	/*unsigned int origMin = 0x8CE008;
	unsigned int origMax = 0x8CE040;

	// scan the .text memory
	char* scanMin = (char*)0x401000;
	char* scanMax = (char*)0x856E00;
	char* current = scanMin;
	for ( ; current < scanMax; current += 1 )
	{
		unsigned int* intCur = (unsigned int*)current;
		// if the address points to something within our range of interest
		if ( *intCur >= origMin && *intCur <= origMax )
		{
			// patch it
			LogToFile("0x%X - %d", intCur, (*intCur) - (unsigned int)origMin);
//			*intCur += difference;
		}
	}*/
}

#endif

#ifdef DEBUG_GETMODELINFO_WRAPPER

CBaseModelInfo* GetModelInfoWrapper(const char* pName, int* pID)
{
	CBaseModelInfo*		pInfo = CModelInfo::GetModelInfo(pName, pID);

	if ( !pInfo )
		LogToFile("Model not found: %s", pName);

	return pInfo;
}


#endif

#ifdef RWERRORSET_HOOK

#include <d3d9.h>

#define gDevice (*(IDirect3DDevice9 **)0xC97C28)

RwError* RwErrorSetHook(RwError* code)
{
	return code;
}

WRAPPER RpAtomic* AtomicDefaultRenderCallBack_Hook(RpAtomic* atomic)
{
	_asm
	{
		push	esi
		mov		esi, [esp+8]
		mov		eax, 0x7491C5
		jmp		eax
	}
}

RpAtomic* AtomicDefaultRenderCallBack_Wireframe(RpAtomic* atomic)
{
	gDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	return AtomicDefaultRenderCallBack_Hook(atomic);
}

#endif

/*RpAtomic* myfunc(RpAtomic* pAtomic, float alpha)
{
	BYTE bAlpha = alpha;
	_asm
	{
		movzx	eax, bAlpha
		push	eax
		push	pAtomic
		mov		eax, 6D2960h
		call	eax
		add		esp, 8
		push	pAtomic
		mov		eax, 7491C0h
		call	eax
		add		esp, 4
	}
	return pAtomic;
}*/

void __declspec(naked) TempRadarFixFunc()
{
	_asm
	{
		fild	[esp+64h-3Ch]
		mov		esi, 2
		push	5754C6h
		retn
	}
}

void __declspec(naked) TempRadarFixFunc2()
{
	_asm
	{
		sub		[esp+4], 2
		sub		[esp+8], 2
		push	586520h
		retn
	}
}

void __declspec(naked) CamHack()
{
	_asm
	{
		mov		edi, 1080
		mov		ebx, 1920
		mov		eax, 5BD626h
		jmp		eax
	}
}

// Fixed
void TempExitFix()
{
	CUpdateManager::InstallIfNeeded();
	ExitProcess(0);
}

/*static const RwMatrix* pMat;
static const RwSphere* pSphere;

#include "TimeCycle.h"

struct CShadowCamera
{
	RwCamera*		m_pCamera;
	RwTexture*		m_pTexture;

	void			Rescale(RpLight* pLight)
	{
		/*RwMatrix*	pCamMat = RwFrameGetMatrix(RwCameraGetFrame(m_pCamera));
		RwMatrix*	pLightMat = RwFrameGetMatrix(RpLightGetFrame(pLight));

		pCamMat->right = pLightMat->right;
		pCamMat->up = pLightMat->up;
		pCamMat->at = pLightMat->at;

		RwMatrixUpdate(pCamMat);
		RwFrameUpdateObjects(RwCameraGetFrame(m_pCamera));*/
/*	}

	RwCamera*		SetCenter(RwV3d* vector)
	{
		RwFrame*	pCamFrame = RwCameraGetFrame(m_pCamera);
		RwMatrix*	pCamMat = RwFrameGetMatrix(pCamFrame);
		CMatrix		Helper(pCamMat);

		/*Helper.SetTranslateOnly(m_pCamera->farPlane * -0.5 * pCamFrame->modelling.at.x + vector->x,
			m_pCamera->farPlane * -0.5 * pCamFrame->modelling.at.y + vector->y,
			m_pCamera->farPlane * -0.5 * pCamFrame->modelling.at.z + vector->z);*/
		//Helper.SetTranslateOnly(538.648, -1065.9757, 11.0);



		//CVector		vecToSun(pMat->pos - CTimeCycle::m_VectorToSun[CTimeCycle::m_CurrentStoredValue]);
/*		CVector		vecToSun = -CTimeCycle::m_VectorToSun[CTimeCycle::m_CurrentStoredValue];
		vecToSun.z = 0.0f;
		vecToSun.Normalize();
		//CMatrix		ScalingMatrix;

		//ScalingMatrix.SetScale(1.0f);

		/*vecToSun.x = pMat->pos.x - 538.6483f;
		vecToSun.y = pMat->pos.y - -1065.9757f;
		vecToSun.z = pMat->pos.z - 11.0f;*/

/*		Helper.SetRotateOnly(-M_PI/2, 0.0, atan2(-vecToSun.x, vecToSun.y));

		vecToSun = pMat->pos - (vecToSun*pSphere->radius * 1.1f);

		Helper.SetTranslateOnly(vecToSun.x, vecToSun.y, vecToSun.z);
		//CMatrix		SecondHelper = Helper * ScalingMatrix;
		//Helper.CopyOnlyMatrix(SecondHelper);
		//Helper.SetTranslateOnly(vector->x, vector->y, vector->z);
		Helper.UpdateRW();

		CCoronas::RegisterCorona(111111111, nullptr, 255, 0, 0, 255, CVector(538.648, -1065.9757, 11.0), 0.25f, 1000.0f, gpCoronaTexture[1], 0, 0, 0, 0, 0.0f, false, 1.5f, false, 255, false, true);


		CVector		vecPos = *Helper.GetPos();
		CVector		vecRight = *Helper.GetPos() + *Helper.GetRight();
		CVector		vecUp = *Helper.GetPos() + *Helper.GetUp();
		CVector		vecAt = *Helper.GetPos() + *Helper.GetAt();

		/*vecRight += matrix->right;
		vecUp += matrix->up;
		vecAt += matrix->at;*/

		// pos
/*		CCoronas::RegisterCorona(111111112, nullptr, 255, 255, 0, 255, vecPos, 0.25f, 1000.0f, gpCoronaTexture[1], 0, 0, 0, 0, 0.0f, false, 1.5f, false, 255, false, true);

		// right
		CCoronas::RegisterCorona(111111112 + 1, nullptr, 255, 0, 0, 255, vecRight, 0.25f, 1000.0f, gpCoronaTexture[1], 0, 0, 0, 0, 0.0f, false, 1.5f, false, 255, false, true);

		// up
		CCoronas::RegisterCorona(111111112 + 2, nullptr, 0, 255, 0, 255, vecUp, 0.25f, 1000.0f, gpCoronaTexture[1], 0, 0, 0, 0, 0.0f, false, 1.5f, false, 255, false, true);

		// at
		CCoronas::RegisterCorona(111111112 + 3, nullptr, 0, 0, 255, 255, vecAt, 0.25f, 1000.0f, gpCoronaTexture[1], 0, 0, 0, 0, 0.0f, false, 1.5f, false, 255, false, true);


		/*pCamMat->pos.x = ;
		pCamMat->pos.y = ;
		pCamMat->pos.z = */

		//*pCamMat = *Helper.matrix;
		//memcpy(pCamMat, &Helper.matrix, sizeof(RwMatrix));

		//RwMatrixUpdate(pCamMat);
/*		RwFrameUpdateObjects(pCamFrame);
		RwFrameOrthoNormalize(pCamFrame);

		return m_pCamera;
	}
};

RwV3d* CheckRenderingSrc(RwV3d* pointsOut, const RwSphere* pointsIn, RwInt32 numPoints, const RwMatrix* matrix)
{
	//RwV3d		vecIn = pointsIn->center;

	/*vecIn.x *= 5.0f;
	vecIn.y *= 5.0f;
	vecIn.z *= 5.0f;*/
	//vecIn.x = -2.0f;
	//vecIn.y = 0.0;
	//vecIn.z = 0.0f;
/*	*pointsOut = matrix->pos;

	pMat = matrix;
	pSphere = pointsIn;

	//auto*		pVec = RwV3dTransformPoints(pointsOut, &vecIn, numPoints, matrix);

	return pointsOut;
}

float* RescaleHook(float* pointsOut, const RwV3d* pointsIn, RwInt32 numPoints, const RwMatrix* matrix)
{
	//RwV3d		vecIn = pointsIn->center;

	/*vecIn.x *= 5.0f;
	vecIn.y *= 5.0f;
	vecIn.z *= 5.0f;*/
	//vecIn.x = -2.0f;
	//vecIn.y = 0.0;
	//vecIn.z = 0.0f;
/*	pointsOut[0] = matrix->pos.x;
	pointsOut[1] = matrix->pos.y;
	pointsOut[2] = matrix->pos.z;
	pointsOut[3] *= 2.0f;
	//*pointsOut = matrix->pos;

	//pMat = matrix;
	//pSphere = pointsIn;

	//auto*		pVec = RwV3dTransformPoints(pointsOut, &vecIn, numPoints, matrix);

	return pointsOut;
}

static const RwV3d		FakeSunValues = { 0 };*/

void* __stdcall CorrectedMallocTest(int nSize, int nAlign)
{
	static int	nTotalSize = 0;
	int			nProperSize = nSize;

	if ( nSize % nAlign )
		nProperSize += (4-nSize % nAlign);

	nTotalSize += nProperSize;

	LogToFile("Requesting %d bytes, %d bytes used so far", nSize, nTotalSize);
	return malloc(nProperSize);
}

RpAtomic* RenderAtomicTest(RpAtomic* atomic)
{
	RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, 0);
	return AtomicDefaultRenderCallBack(atomic);
}

#include "SpeechRecognition.h"

__forceinline void Main_Patches()
{
	using namespace Memory;
	DWORD dwFunc;

	LogToFile("Patching miscellaneous game data...");

	// TEMPORARY WORKAROUND FOR A CRASH
	Patch<BYTE>(0x6195E0, 0xC3);

	//InjectHook(0x4A9CA0, &CorrectedMallocTest, PATCH_JUMP);
	// Fixes a heap corruption by R*
	Nop(0x5C25D3, 5);

#ifdef INCLUDE_SQUARE_RADAR
	InjectHook(0x5832F0, &SquareRadar, PATCH_JUMP);
	Patch<float>(0x585719, 0.0);
	Patch<float>(0x585721, 0.0);
	Patch<float>(0x585729, 0.0);
	Patch<float>(0x585731, 0.0);
	Patch<float>(0x585739, 0.0);
	Patch<float>(0x585741, 0.0);
	Patch<float>(0x585749, 0.0);
	Patch<float>(0x585751, 0.0);
#endif

#ifdef INCLUDE_TRIANGLE_RADAR
	InjectHook(0x5832F0, &TriangleRadar, PATCH_JUMP);
	InjectHook(0x585850, &TriangleRadarMask, PATCH_JUMP);
	Patch<DWORD>(0x58581B, 2);
	Patch<BYTE>(0x585895, 3);
	Patch<BYTE>(0x58589C, 3);
#endif

#ifdef V_RADAR_TEST
	InjectHook(0x58696E, &Draw3DRadarHook, PATCH_JUMP);
	InjectHook(0x586408, &RadarTransformHook);
	InjectHook(0x5864EB, &CRadar::Set3DVerts);
	InjectHook(0x586500, &CRadar::Render3D);
#endif

#ifdef INCLUDE_COOL_BINK_SCREEN
	InjectHook(0x534310, &RenderHook, PATCH_JUMP);
	InjectHook(0x60D77E, &PlayerVid, PATCH_JUMP);
#endif

#ifdef INCLUDE_AUDIO_DEBUG
	InjectHook(0x506EA0, &GetDamnStack, PATCH_JUMP);
#endif

#ifdef EXTRA_ASH_LOGGING
	InjectHook(0x552CD0, &GetTreeNodeFuncStuff, PATCH_JUMP);
	InjectHook(0x5BD17E, &GetPedgrp, PATCH_JUMP);
#endif

#ifdef TRACE_D3D_MARKER
	InjectHook(0x725120, &TraceMarker, PATCH_JUMP);
#endif

#ifdef EXTRA_WINDOWMODE_CHECKING
	InjectHook(0x7F5F70, &TraceD3DStuff, PATCH_JUMP);
#endif

#ifdef TRACE_PROJECTILES
	InjectHook(0x737C80, &TraceProjectiles, PATCH_JUMP);
#endif

#ifdef NO_AUDIO
	Patch<BYTE>(0x507750, 0xC3);
	//patch(0x4DD4A0, 0x0CC2, 4);
	//Patch<BYTE>(0x4D9870, 0xC3);
#endif

#ifdef TRACE_COLOUR_STUFF
	InjectHook(0x4C8379, &TraceMaterials, PATCH_JUMP);
#endif

#ifdef TRACE_STREAMING
	InjectHook(0x4087E0, &TraceStreaming_Requestmodel, PATCH_JUMP);
#endif

#ifdef TRACE_CTEXT
	InjectHook(0x6A01A0, &TraceCText_Load, PATCH_JUMP);
	InjectHook(0x5BD76A, &TraceInterface);
#endif

#ifdef TRACE_UPCASE
	InjectHook(0x7192C0, &TraceUpcase, PATCH_JUMP);
#endif

#ifdef TRACE_NODES_STREAMS
	InjectHook(0x4076EC, &TraceNodes, PATCH_JUMP);
#endif

#ifdef TRACE_FUCKING_TEXTURES
//	InjectHook(0x5B6449, &TraceTXDLoads);
	InjectHook(0x731CCB, &TraceTXDLoads2, PATCH_JUMP);
#endif

	//InjectHook(0x748813, &LoadNewWindowIcon);
	// SAORS connection
	//patch(0x868A5C, &saors, 4);

#ifdef ADD_UNFINISHED_WINDOWED_MODE
	// Windowed mode
	InjectHook(0x745C46, &InjectWindowedMode, PATCH_JUMP);
#endif

#ifdef TRACE_STREAMS
	InjectHook(0x4D86F8, &TraceStreamPointers, PATCH_JUMP);
#endif

#ifdef TRACE_STREAMING2
	InjectHook(0x40CB83, &TraceStreamingRequests, PATCH_JUMP);
#endif
/*#if DEBUG
	// Resolution selection box
	Nop(0x746225, 2);
#endif*/

	// Crash fix
//	call(0x552CD0, &NodeCrashFix, PATCH_JUMP);

#ifdef NEW_SHADOWS_TEST
	Patch<BYTE>(0x706825, 8);
	Patch<BYTE>(0x706832, 8);
	Patch<BYTE>(0x7064C2, 9);
	Patch<BYTE>(0x7064F9, 8);
	Patch<BYTE>(0x53E159, 0xC3);
	//Patch<DWORD>(0x70BDA0, 0x042474FF);
	//Patch<DWORD>(0x70BDA4, 0xC40350B9);
	//Patch<DWORD>(0x70BDA8, 0xADF2E800);
	//Patch<DWORD>(0x70BDAC, 0x90C3FFFF);
	Patch<DWORD>(0x707E09, 0x9090C031);
	Patch<BYTE>(0x707E0D, 0x90);
	//Patch<DWORD>(0x707E4F, 0x900CC483);
	//Patch<BYTE>(0x707E53, 0x90);
	Patch<const void*>(0x707E14, &FakeSunValues);
	Patch<const void*>(0x707E1B, &FakeSunValues);
	InjectHook(0x70663A, &CheckRenderingSrc);
	//InjectHook(0x706573, &RescaleHook);
	InjectHook(0x70664C, &CShadowCamera::SetCenter, PATCH_NOTHING);
	InjectHook(0x705981, &CShadowCamera::Rescale, PATCH_NOTHING);
	//static const float		fFrustumSize = 2.0f;
	//Patch<const void*>(0x7065CC, &fFrustumSize);
#endif

#ifdef MULTITHREADING_TEST
	Patch<DWORD>(0x53E9A3, 0x8C4835E);
	Patch<BYTE>(0x53E9A7, 0xC3);
	InjectHook(0x53EC09, &0x53E9AC, PATCH_JUMP);
#endif

#ifndef DONT_FIX_STREAMING
	InjectHook(0x4076EC, &NodeCrashFix2, PATCH_JUMP);
#endif

#ifdef DEBUG_GETMODELINFO_WRAPPER
	InjectHook(0x5B6B1B, &GetModelInfoWrapper);
	InjectHook(0x5B6CA5, &GetModelInfoWrapper);
#endif

#ifdef NO_REPLAY_TEXT
	Nop(0x53E28E, 5);
#endif

#ifdef RWERRORSET_HOOK
	InjectHook(0x808820, &RwErrorSetHook, PATCH_JUMP);
	//InjectHook(0x7491C0, &AtomicDefaultRenderCallBack_Wireframe, PATCH_JUMP);
#endif

	InjectHook(0x748EDA, &OnGameTermination);

	CAECustomBankLoader::Patch();

	// Fixes a crash when game is launched for the second time and first instance has no created window yet
	InjectHook(0x74872D, &IsAlreadyRunning);

	Patch<const void*>(0x7486C1, WindowProc);

	// CConfiscatedWeapons injectors
	InjectHook(0x442D06, &CGameLogic__Update_Wasted);
	InjectHook(0x442D2F, &CGameLogic__Update_Busted);

	// DOUBLE_RWHEELS SA bug fix
	Patch<WORD>(0x4C9290, 0xE281);
	Patch<int>(0x4C9292, ~(rwMATRIXTYPEMASK|rwMATRIXINTERNALIDENTITY));

	// Bumped up shadows quality
	Memory::Patch<BYTE>(0x706825, 8);
	Memory::Patch<BYTE>(0x706832, 8);
	Memory::Patch<BYTE>(0x7064C2, 9);
	Memory::Patch<BYTE>(0x7064F9, 8);

	// Don't change velocity when colliding with peds in a vehicle
	//call(0x5F12CA, &CPhysical__ApplyMoveForce_Nop, PATCH_CALL);
	Patch<DWORD>(0x5F12CA, 0x901CC483);	// add esp, 1Ch; Nop
	Nop(0x5F12CA + 4, 1);

	// Garages capacity tweak
	InjectHook(0x44BD8D, &GarageCapacityA, PATCH_JUMP);
	InjectHook(0x44BF1B, &GarageCapacityB, PATCH_JUMP);

	// Stronger AA
	/*Patch<BYTE>(0x57D0E1, MAX_AA + 1);
	patch(0x57D120, MAX_AA + 1, 4);*/

	// VCPD Cheetah & Vice Squad
	/*patch(0x461319, 0x50, 4); // switch 80 cases
	Patch<BYTE>(0x46195F, 3);
	patch(0x461908, &AssignViceSquadToVehicle, 4);*/
	Patch<BYTE>(0x6D23EB, 0);
	/*patch(0x40B707, (DWORD)&ViceSquadCheckInjectA - 0x40B70B, 4);
	Patch<BYTE>(0x421981, 0xE9);
	patch(0x421982, (DWORD)&ViceSquadCheckInjectC - 0x421986, 4);*/

	// 6ATV driving on water
	InjectHook(0x6AAB76, &ATVPreRenderHack, PATCH_JUMP);

	// plr.dff
	Patch(0x4668F9, "plr");

	// No player skin stuff
	Nop(0x5BA1FA, 5);

	// CDarkel stuff
	InjectHook(0x47E2E6, &CDarkel::StartFrenzyVCS);
	InjectHook(0x53E51D, &CDarkel::DrawMessages);

	// DB peds counter fix
	Patch<BYTE>(0x43DDD8, 0xFE);
	InjectHook(0x43DDDC, &DriveByKillFix, PATCH_JUMP);
	//Patch<BYTE>(0x43DDDD, 0xFA);

	// Looking left/right blocked with RC Bandit
	InjectHook(0x52780D, &LookLeftRightHack, PATCH_JUMP);

	// No beep in rampages
	Patch<float>(0x43DE26, 0.0f);
	Patch<float>(0x43DF38, 0.0f);

	Nop(0x43DB9E, 5);


	// 0394 bugfix
//	Patch<BYTE>(0x4842DA, 0);

	// 0351 - 0355 & 037A opcodes
	Patch<DWORD>(0x4836E8, 0x47474747);
	Patch<BYTE>(0x4836EC, 0x47);
	Patch<BYTE>(0x483711, 0x47);
	Patch<void*>(0x48371C, func_0352);

	// 03C1, 03C2, 03DA, 03DB, 03DD & 03DF opcodes
	Patch<WORD>(0x485801, 0x6767);
	Patch<WORD>(0x48581A, 0x6767);
	Patch<BYTE>(0x48581D, 0x67);
	Patch<BYTE>(0x48581F, 0x67);
	Patch<void*>(0x485854, func_03DF);

	// 051F, 0520, 0521, 052E and 052F opcodes
	Patch<WORD>(0x48DDDE, 0x4141);
	Patch<WORD>(0x48DDE0, 0x41);
	Patch<WORD>(0x48DDED, 0x4141);
	Patch<void*>(0x48DE38, func_052E);

	// 06BA, 06CB, 06CC opcodes
	Patch<void*>(0x498F68, func_06BA);
	Patch<void*>(0x498FAC, func_06BA);
	Patch<void*>(0x498FB0, func_06BA);

	// 0050, 0051, 0054 and 0055 opcodes
	Patch<void*>(0x466D90, func_0050);
	Patch<void*>(0x466D94, func_0050);
	Patch<void*>(0x466DA0, func_0050);
	Patch<void*>(0x466DA4, func_0050);

	// 014D and 0166 opcodes
	Patch<BYTE>(0x47D1CA, 0x0F);
	Patch<void*>(0x47D130, func_0166);

	// 01C6 opcode
	Patch<BYTE>(0x47E016, 0x3B);
	Patch<void*>(0x47E044, func_01C6);

	// 0821 opcode
	Patch<void*>(0x4744D4, func_0821);

	// 4 params in 048C
	Patch<BYTE>(0x48AD90, 4);
	Patch<SCRIPT_VAR*>(0x48AE2A, &scriptParams[3]);

	// SCM functions
	InjectHook(0x468D82, &CRunningScript::Init, PATCH_NOTHING);
	//call(0x468D50, &ZeroScriptsCounter, PATCH_JUMP);
//	call(0x5D4FC2, &CScriptFunction::SaveAllFunctions, PATCH_JUMP);
//	call(0x5D539D, &LoadFunctionBreak, PATCH_JUMP);

	// RIGHTSHOCK keypress fix
	InjectHook(0x52FA3C, &RightShockKeyHack, PATCH_JUMP);

	// 02FD as lowpriority text
	InjectHook(0x480B08, 0x69E360);


	// VCS HUD
	if ( !IVHudPresent() )
	{
		InjectHook(0x58FBD6, &CHud::DrawHUD);
		InjectHook(0x58FBDB, &CHud::DrawWanted);
		Patch<const void*>(0x58D896, &fWeaponIconHeight);
		Patch<const void*>(0x58D94D, &fWeaponIconHeight);
		Patch<const void*>(0x58D8CB, &fWeaponIconWidth);
		Patch<const void*>(0x58D935, &fWeaponIconWidth);
		//Patch<DWORD>(0x58D89B, HUD_TRANSPARENCY);

		/*Patch<WORD>(0x58DF18, 0xE990);
		Patch<BYTE>(0x58DE23, 0xEB);
		InjectHook(0x588B60, &CHud::GetYPosBasedOnHealth, PATCH_JUMP);
		Patch<BYTE>(0x58DD41, 0);
		Nop(0x58DE84, 6);
		Nop(0x58DE9F, 6);
		Nop(0x58DEBA, 6);
		Patch<char>(0x58DCEF, '\\');
		Patch<BYTE>(0x58DCDB, FONT_Hud);
		Patch<void*>(0x58DE77, &BaseColors[12].b);
		Patch<void*>(0x58DE92, &BaseColors[12].g);
		Patch<void*>(0x58DEAD, &BaseColors[12].r);
		Patch<const void*>(0x58DDFC, &fWLStarPosY);
		Patch<const void*>(0x58DEF5, &fWLStarPosY);
		Patch<const void*>(0x58DD0F, &fWLStarPosX);
		Patch<const void*>(0x58DCAA, &fWLStarHeight);
		Patch<const void*>(0x58DD70, &fWLStarHeight);
		Patch<const void*>(0x58DCC0, &fWLStarWidth);
		Patch<const void*>(0x58DD86, &fWLStarWidth);
		Patch<const void*>(0x58DFED, &fWLStarDistance);
		Patch<const void*>(0x58D9BA, &fWLStarAlpha);*/
		/*Patch<const void*>(0x5834C2, &fRadarWidth);
		Patch<const void*>(0x58781B, &fRadarWidth);
		Patch<const void*>(0x58A449, &fRadarWidth);
		Patch<const void*>(0x58A7E9, &fRadarWidth);
		Patch<const void*>(0x58A840, &fRadarWidth);
		Patch<const void*>(0x58A943, &fRadarWidth);
		Patch<const void*>(0x58A99D, &fRadarWidth);*/

		Patch<const void*>(0x5834F6, &fRadarHeight);
		Patch<const void*>(0x58A47D, &fRadarHeight);
		Patch<const void*>(0x58A632, &fRadarHeight);
		Patch<const void*>(0x58A6AB, &fRadarHeight);
		Patch<const void*>(0x58A70E, &fRadarHeight);
		Patch<const void*>(0x58A801, &fRadarHeight);
		Patch<const void*>(0x58A8AB, &fRadarHeight);
		Patch<const void*>(0x58A921, &fRadarHeight);
		Patch<const void*>(0x58A9D5, &fRadarHeight);

		Patch<const void*>(0x5834D4, &fRadarPosX);
		Patch<const void*>(0x58A469, &fRadarPosX);
		Patch<const void*>(0x58A5E2, &fRadarPosX);
		Patch<const void*>(0x58A6E6, &fRadarPosX);
		Patch<const void*>(0x58A79B, &fRadarPosX);
		Patch<const void*>(0x58A836, &fRadarPosX);
		Patch<const void*>(0x58A8E9, &fRadarPosX);
		Patch<const void*>(0x58A98A, &fRadarPosX);

		Patch<const void*>(0x583500, &fRadarPosY);
		Patch<const void*>(0x58A499, &fRadarPosY);
		Patch<const void*>(0x58A60E, &fRadarPosY);
		Patch<const void*>(0x58A71E, &fRadarPosY);
		Patch<const void*>(0x58A7C7, &fRadarPosY);
		Patch<const void*>(0x58A868, &fRadarPosY);
		Patch<const void*>(0x58A913, &fRadarPosY);
		Patch<const void*>(0x58A9C7, &fRadarPosY);
	}
	InjectHook(0x5BD76F, &CHud::Initialise);
	InjectHook(0x53BD51, &CHud::ReInitialise);
	InjectHook(0x53CA84, &CHud::ReInitialise);
	InjectHook(0x53BBA2, &CHud::Shutdown);
	InjectHook(0x4E9EF8, &RadioNameInject, PATCH_JUMP);
	InjectHook(0x588B44, &CHud::GetRidOfAllCustomHUDMessages, PATCH_JUMP);
	//InjectHook(0x588A4B, &HUDInitialiseBreak, PATCH_JUMP);
	InjectHook(0x588F50, &CHud::SetVehicleName, PATCH_JUMP);
	InjectHook(0x588BB0, &CHud::SetZoneName, PATCH_JUMP);
	InjectHook(0x58D542, &CHud::DrawAreaName);
	InjectHook(0x58FBE9, &CHud::DrawVehicleName);
	InjectHook(0x58FD01, &CHud::DrawBigMessage);
	InjectHook(0x58D56F, &CHud::DrawBigMessage);
	InjectHook(0x53E522, &CGarages::PrintMessages);
	InjectHook(0x58C83E, &BigMessage1Inject, PATCH_JUMP);
	InjectHook(0x58D3D9, &BigMessage2Inject, PATCH_JUMP);
	InjectHook(0x58CB9C, &BigMessage3Inject, PATCH_JUMP);
	Patch<void*>(0x58C278, &CHud::CarNameStage);
	Patch<void*>(0x58C286, &CHud::CarNameStage);
	Patch<void*>(0x58CAB5, &CHud::CarNameStage);
	Patch<void*>(0x58CAD3, &CHud::CarNameStage);
	Patch<void*>(0x58D27F, &CHud::m_ZoneState);
	Patch<void*>(0x58C28C, &CHud::m_ZoneState);
	Patch<void*>(0x58C295, &CHud::m_ZoneState);
	Patch<void*>(0x58CADC, &CHud::m_ZoneState);
	Patch<void*>(0x58CAE6, &CHud::m_ZoneState);
	Patch<void*>(0x58D289, &CHud::m_ZoneFadeTimer);
	Patch<void*>(0x58D33A, &CHud::BigMessage2Alpha);
	Patch<void*>(0x58D3A1, &CHud::BigMessage2Alpha);
	Patch<void*>(0x58D3A7, &CHud::BigMessage2Alpha);
	Patch<void*>(0x58D3AD, &CHud::BigMessage2Alpha);
	Patch<void*>(0x58D3C0, &CHud::BigMessage2Alpha);
	Nop(0x58C729, 6);

	Patch<void*>(0x58CAC7, &CHud::BigMessage3Alpha);
	Patch<void*>(0x58CB7B, &CHud::BigMessage3Alpha);
	Patch<void*>(0x58CB81, &CHud::BigMessage3Alpha);
	Patch<void*>(0x58CB94, &CHud::BigMessage3Alpha);
	/*Patch<void*>(0x58CA99, &hud.BigMessage3PosY);
	Patch<void*>(0x58CB1C, &hud.BigMessage3PosY);
	Patch<void*>(0x58CB44, &hud.BigMessage3PosY);*/
	Patch<BYTE>(0x58C8F1, 9);

	Patch<const void*>(0x58C39D, &fSubtitlesWidth);
	Patch<const void*>(0x58C425, &fSubtitlesWidth);
	Patch<const void*>(0x58C4E4, &fSubtitlesWidth);
	Patch<const void*>(0x58C387, &fSubtitlesHeight);
	Patch<const void*>(0x58C40F, &fSubtitlesHeight);
	Patch<const void*>(0x58C4CE, &fSubtitlesHeight);

	// User display
	InjectHook(0x588B50, &CHud::HelpMessageShown, PATCH_JUMP);
	InjectHook(0x571F07, &CUserDisplay::Init, PATCH_JUMP);
	InjectHook(0x53C08B, &CUserDisplay::Process);

	InjectHook(0x53E50E, &COnscreenTimer::ProcessForDisplay, PATCH_NOTHING);
	InjectHook(0x5720AF, &COnscreenTimer::Process, PATCH_NOTHING);
	InjectHook(0x47C2AC, &COnscreenTimer::AddClock, PATCH_NOTHING);
	InjectHook(0x47C2C3, &COnscreenTimer::AddClock, PATCH_NOTHING);
	InjectHook(0x484CA7, &COnscreenTimer::AddClock, PATCH_NOTHING);
	InjectHook(0x484CFC, &COnscreenTimer::AddCounter, PATCH_NOTHING);
	InjectHook(0x48C57D, &COnscreenTimer::AddCounter, PATCH_NOTHING);
	InjectHook(0x47C2E2, &COnscreenTimer::ClearClock, PATCH_NOTHING);
	InjectHook(0x47C301, &COnscreenTimer::ClearCounter, PATCH_NOTHING);
	InjectHook(0x48EEB6, &COnscreenTimer::SetFlashing, PATCH_NOTHING);
	InjectHook(0x48EECD, &COnscreenTimer::SetFlashing, PATCH_NOTHING);
	InjectHook(0x4720AF, &COnscreenTimer::SetSoundOnTime, PATCH_NOTHING);
	InjectHook(0x58FBEE, &CHud::DrawOnscreenTimer, PATCH_NOTHING);

	InjectHook(0x47C286, &RunningScriptPointerParamWrapper);
	InjectHook(0x47C2D4, &RunningScriptPointerParamWrapper);
	InjectHook(0x47C2F3, &RunningScriptPointerParamWrapper);
	InjectHook(0x484C59, &RunningScriptPointerParamWrapper);
	InjectHook(0x484CB5, &RunningScriptPointerParamWrapper);
	InjectHook(0x48C531, &RunningScriptPointerParamWrapper);
	InjectHook(0x48EE92, &RunningScriptPointerParamWrapper);
	InjectHook(0x472090, &RunningScriptPointerParamWrapper);

	Patch<void*>(0x53E50A, &CUserDisplay::OnscnTimer);
	Patch<void*>(0x5720AB, &CUserDisplay::OnscnTimer);
	Patch<void*>(0x47C29E, &CUserDisplay::OnscnTimer);
	Patch<void*>(0x47C2DE, &CUserDisplay::OnscnTimer);
	Patch<void*>(0x47C2FD, &CUserDisplay::OnscnTimer);
	Patch<void*>(0x484CA3, &CUserDisplay::OnscnTimer);
	Patch<void*>(0x484CF8, &CUserDisplay::OnscnTimer);
	Patch<void*>(0x48C579, &CUserDisplay::OnscnTimer);
	Patch<void*>(0x48EEB2, &CUserDisplay::OnscnTimer);
	Patch<void*>(0x48EEC9, &CUserDisplay::OnscnTimer);
	Patch<void*>(0x4720AB, &CUserDisplay::OnscnTimer);

	Patch<void*>(0x4687DB, &CUserDisplay::OnscnTimer.m_bPaused);
	Patch<void*>(0x484384, &CUserDisplay::OnscnTimer.m_bPaused);

	Patch<BYTE>(0x47C2AB, 0x57);
	Patch<BYTE>(0x484CA1, 0x57);
	Patch<BYTE>(0x484CF6, 0x57);
	Patch<BYTE>(0x48C577, 0x57);
	Patch<BYTE>(0x48EEB0, 0x57);
	Patch<BYTE>(0x48EEC7, 0x57);
	Patch<BYTE>(0x4720A9, 0x56);
	Patch<DWORD>(0x47C2D9, 0x50001F0F);
	Patch<DWORD>(0x47C2F8, 0x50001F0F);
	Nop(0x47C2A6, 3);
	Nop(0x484C9C, 3);
	Nop(0x484CF2, 3);
	Nop(0x48C573, 3);
	Nop(0x48EEAB, 3);
	Nop(0x48EEC2, 3);
	Nop(0x4720A5, 3);
	Nop(0x571EF8, 5);

	Nop(0x4DDE6F, 8);	// Pager sound fix

	// Onscreen timers stuff
	/*dwFunc = *(BYTE*)0x44CDA0;
	if ( (BYTE)dwFunc == 0xE9 )
		dwFunc = (*(DWORD*)0x44CDA1) + 0x44CDA5 + 0x56;
	else
		dwFunc = 0x44CDE8;*/
	//Patch<BYTE>(0x44CDEF, 8);
	//call(0x44CDE8, &OnscreenCounterColour, PATCH_JUMP);
	//Patch<WORD>(0x44CE40, 0x10EB);
	//call(0x44CE52, &TwoNumberCounterColour, PATCH_JUMP);

	/*dwFunc = *(BYTE*)0x44CC20;
	if ( (BYTE)dwFunc == 0xE9 )
	{
		dwFunc = (*(DWORD*)0x44CC21) + 0x44CC25 + 0x66;
		//Nop(dwFunc, 17);
		Patch<WORD>(dwFunc, 0x0FEB);
		dwFunc += 17;
		Patch<BYTE>(dwFunc, 0x68);
		++dwFunc;
	}
	else
		dwFunc = 0x44CC71;
	charptr(dwFunc, "%d:%02d");*/

	// Trip Skip
	Patch<float>(0x442587, 0.5f);
	Nop(0x4425BE, 5);
	Nop(0x58FC6A, 5);
	Patch(0x58FC7B, 1);
	Patch<const char*>(0x58FC7F, "H_TS_01");

	// Blue ~h~
	Patch<DWORD>(0x7191AC, 0x719085);

	// Racing checkpoints colours
	Patch<const void*>(0x49362D, &ScriptCheckpointsColours_IndirectTable);
	Patch<const void*>(0x493634, &ScriptCheckpointsColours_Jumptable);

	// Removed prostitute subtitiles
	Patch<WORD>(0x666951, 0x1FEB);
	Patch<WORD>(0x66724E, 0x1FEB);
	Patch<WORD>(0x66689D, 0x4AEB);
	Patch<WORD>(0x666E21, 0x22EB);
	Patch<DWORD>(0x667169, 0xEB04C483);
	Patch<BYTE>(0x66716D, 0x45);
	Nop(0x666E07, 5);

	// No stats update box
	Patch<BYTE>(0x55B980, 0xC3);

	// Spheres colours
	//dwFunc = 0x4810E0 + 0x2B;
//	patch(dwFunc, MARKER_SET_COLOR_A, 4);
//	dwFunc += 0x6;
	Patch<BYTE>(0x4810E0 + 0x2B, MARKER_SET_COLOR_B);
	//dwFunc += 0x2;
	Patch<BYTE>(0x4810E0 + 0x2B + 0x2, MARKER_SET_COLOR_G);
	//dwFunc += 0x2;
	Patch<BYTE>(0x4810E0 + 0x2B + 0x4, MARKER_SET_COLOR_R);

 	Patch<BYTE>(0x585CCB, MARKER_SET_COLOR_B);
	Patch<BYTE>(0x585CCD, MARKER_SET_COLOR_G);
	Patch<BYTE>(0x585CCF, MARKER_SET_COLOR_R);
	/*Patch<BYTE>(0x70CD0B, 0xB4);
	Patch<BYTE>(0x70CD0D, 0x82);
	Patch<BYTE>(0x70CD0F, 0xED);
	Patch<BYTE>(0x70CD58, 0xB4);
	Patch<BYTE>(0x70CD5A, 0x82);
	Patch<BYTE>(0x70CD5C, 0xED);
	Patch<BYTE>(0x70CDAB, 0xB4);
	Patch<BYTE>(0x70CDAD, 0x82);
	Patch<BYTE>(0x70CDAF, 0xED);*/

	// Growing/shrinking 3DMarkers
	Patch<float>(0x440F26, 0.0f);
	InjectHook(0x72576B, &C3DMarkerSizeHack, PATCH_CALL);
	Nop(0x725770, 1);

	// New style of markers
	InjectHook(0x725BA0, &C3DMarkers::PlaceMarkerSet, PATCH_JUMP);

	// Enex markers RGB
	InjectHook(0x440F38, &EnexMarkersColorBreak, PATCH_JUMP);

	// Font scale fix
	InjectHook(0x7193A0, &CFont::SetScaleLang, PATCH_JUMP);

	// Own textures loading
/*#if DEBUG
	call(0x5BD765, &LoadDevLogos, PATCH_NOTHING);
	call(0x53BBCB, &ReleaseDevLogos, PATCH_NOTHING);
#endif*/

	// Zebra Cab stuff
	InjectHook(0x4912D6, &func_0602, PATCH_JUMP);
	Patch<DWORD>(0x6AB35C, 0xC5);
	Patch<const void*>(0x6AB369, CAutomobile__PreRenderCoronasTable);
	InjectHook(0x6B19AC, &ZebraCabNitro, PATCH_JUMP);
	InjectHook(0x56F938, &ZebraCabCPlayerStuff, PATCH_JUMP);
	InjectHook(0x610384, &func_610310, PATCH_JUMP);
	InjectHook(0x6B4AE9, &func_6B4800, PATCH_JUMP);
	InjectHook(0x6D1AC6, &func_6D1AA0, PATCH_JUMP);
	InjectHook(0x40B646, &CStreaming::RequestSpecialDriverModel);
	InjectHook(0x613A43, &CStreaming::GetSpecialDriverModelID);
	InjectHook(0x40813E, &CStreaming::ReleaseSpecialDriverModel);

	// Quad dual lights
	Nop(0x6ABCAB, 2);

	// In car killed peds counter
	InjectHook(0x4B93D4, &InCarKilledCounterBreak, PATCH_JUMP);

	// No gang gestures
	Patch<BYTE>(0x6601D7, 0xEB);
	Patch<WORD>(0x660967, 0xE990);
	Patch<BYTE>(0x66298F, 0xEB);
	Patch<WORD>(0x6633C0, 0xE990);
	Patch<BYTE>(0x663E4D, 0xEB);

	// Ventoso sound
	Patch<DWORD>(0x4F781E, 0x8D);
	Patch<const void*>(0x4F7827, &audioGearboxSoundTable);

	// Car sounds DAT file
	InjectHook(0x4F77B8, &VehAudioHook, PATCH_JUMP);
	Nop(0x4F77EF, 1);

	// [s]Failed :([/s] Succeeded!
	//InjectHook(0x732B30, &RenderAtomicTest, PATCH_JUMP);
	//InjectHook(0x6C444B, &RotorsHook, PATCH_JUMP);
	//InjectHook(0x6C4400, &CHeli::Render, PATCH_JUMP);
	Patch(0x8716C8, &CHeli::Render_Stub);
	InjectHook(0x553318, &CVisibilityPlugins::RenderAlphaAtomics);

	// radio IDs
	Patch<BYTE>(0x489B8D, 9);
	Patch<DWORD>(0x489B91, 10);

	Patch<BYTE>(0x4E8385, 11);
	Patch<BYTE>(0x4E83A1, 10);
	Patch<const char*>(0x4E83C6, "FEA_FM%d");

	Patch<BYTE>(0x4EB3C7, 12);
	Patch<BYTE>(0x4EB3CE, 11);
	Patch<BYTE>(0x4EB424, 11);
	Patch<BYTE>(0x4EB47B, 11);

	Patch<BYTE>(0x4E9EE4, 11);
	Patch<BYTE>(0x4E9EE9, 12);
	Patch<BYTE>(0x4E9EEE, 11);

	Patch<BYTE>(0x573A2D, 11);
	Patch<BYTE>(0x573A31, 12);

	Patch<BYTE>(0x4E8295, 10);
	Patch<BYTE>(0x4E82A3, 11);

	Patch<BYTE>(0x4E83FB, 11);

	Patch<BYTE>(0x4EB8CB, 11);
	Patch<BYTE>(0x4EB907, 11);
	Patch<BYTE>(0x4EB91E, 11);

	Patch<BYTE>(0x4EB6A0, 11);
	Patch<BYTE>(0x4EB795, 11);
	Patch<BYTE>(0x4EB799, 12);
	Patch<BYTE>(0x4EB79D, 245);
	Patch<BYTE>(0x4EB7A3, 11);
	Patch<BYTE>(0x4EB7AB, 10);
	Patch<BYTE>(0x4EB7BD, 11);

	Patch<BYTE>(0x4EA68E, 10);
	Patch<BYTE>(0x4EA738, 10);
	Patch<BYTE>(0x4EA810, 10);
	Patch<BYTE>(0x4EA876, 10);
	Patch<BYTE>(0x4EA8E0, 10);

	Patch<BYTE>(0x4EA93B, 10);
	Patch<BYTE>(0x4EAAA2, 10);
	Patch<BYTE>(0x4EAAC0, 10);

//	Patch<BYTE>(0x4EAC86, 10);
	Patch<BYTE>(0x4EAD6D, 10);
	Patch<BYTE>(0x4EAD7F, 10);
	Patch<BYTE>(0x4EAD8E, 10);
//	Patch<BYTE>(0x4EADF0, 9);
	Patch<BYTE>(0x4EADF5, 10);
	Patch<BYTE>(0x4EAE19, 10);
//	Patch<BYTE>(0x4EAE46, 9);
//	Patch<BYTE>(0x4EAEDC, 9);
//	Patch<BYTE>(0x4EAF1E, 9);
//	Patch<BYTE>(0x4EAF87, 9);
	Patch<BYTE>(0x4EB03F, 10);
	Patch<BYTE>(0x4EB04D, 10);
	Patch<BYTE>(0x4EB07E, 10);

	Patch<BYTE>(0x4EB1CE, 10);
	Patch<BYTE>(0x4EB1DA, 10);
	Patch<BYTE>(0x4EB1EB, 10);
	Patch<BYTE>(0x4EB21E, 10);
	Patch<BYTE>(0x4EB3A0, 10);
	Patch<BYTE>(0x4EB3B0, 10);

	Patch<BYTE>(0x4EBA14, 11);
	Patch<BYTE>(0x4EBBA2, 10);
	Patch<BYTE>(0x4EBBEC, 10);
	Patch<BYTE>(0x4EBDF2, 10);

	Patch<BYTE>(0x4E9991, 11);
	Patch<BYTE>(0x4E99A6, 11);
	Patch<BYTE>(0x4E9A3B, 0xEB);
	Patch<BYTE>(0x4E9D03, 11);
	Patch<BYTE>(0x4E9D5A, 11);
	Patch<BYTE>(0x5B9494, 11);
	Patch<BYTE>(0x4E8074, 11);

//	Patch<BYTE>(0x4EA276, 0xEB);

	// Proper radio
	InjectHook(0x4EA670, &NoRadioCommercialsHack, PATCH_JUMP);
	InjectHook(0x4EA270, &CAERadioTrackManager::GetNextTrackByStation, PATCH_JUMP);
	Patch<const void*>(0x4EA6F1, &iRadioTracks);
	Patch<const void*>(0x4EA7A9, &iRadioTracks);

	Patch<const void*>(0x4EA76A, &iRadioIntros);
	Patch<const void*>(0x4EA770, &iRadioIntros);

	Patch<const void*>(0x4EA721, &iRadioOutros);
	Patch<const void*>(0x4EA727, &iRadioOutros);
	Patch<const void*>(0x4EA7D9, &iRadioOutros);
	Patch<const void*>(0x4EA7DF, &iRadioOutros);
	Patch<const void*>(0x4EA841, &iRadioOutros);
	Patch<const void*>(0x4EA847, &iRadioOutros);

	Patch<const void*>(0x4E9584, &iAdvertsRange[1]);
	Patch<const void*>(0x4E958A, &iAdvertsRange[0]);

	Patch<const void*>(0x4DC13B, &iBeatsStreams);

	Patch<BYTE>(0x4EA71E, 2);
	Patch<BYTE>(0x4EA767, 2);
	Patch<BYTE>(0x4EA7D6, 2);
	Patch<BYTE>(0x4EA83E, 2);

	static const float	fFadeRadius = 0.9f;
	Patch<const void*>(0x4D703B, &fFadeRadius);
//	Nop(0x4D7039, 6);
	Nop(0x4D6FF0, 2);

	Patch<DWORD>(0x4D6ED3, AMBIENCE_OFFSET+1);
	Patch<DWORD>(0x4D6F74, AMBIENCE_OFFSET+2);
	Patch<DWORD>(0x4D6F7E, AMBIENCE_OFFSET+3);
//	Patch<DWORD>(0x4D6F88, AMBIENCE_OFFSET+34);
//	Patch<DWORD>(0x4D6F94, AMBIENCE_OFFSET+4);
	Patch<DWORD>(0x4D6F9E, AMBIENCE_OFFSET+4);
	Patch<DWORD>(0x4D6FB1, CR_OFFSET+1/*AMBIENCE_OFFSET+5*/);
	Patch<DWORD>(0x4D6FC3, CR_OFFSET+10/*AMBIENCE_OFFSET+22*/);
	Patch<DWORD>(0x4D6FFC, AMBIENCE_OFFSET+5);
//	Patch<DWORD>(0x4D7006, AMBIENCE_OFFSET+33);
//	Patch<DWORD>(0x4D7010, AMBIENCE_OFFSET+27);
	Patch<DWORD>(0x4D7023, NJ_OFFSET+4/*AMBIENCE_OFFSET+4*/);
//	Patch<DWORD>(0x4D7063, AMBIENCE_OFFSET+8, 4);
//	Patch<DWORD>(0x4D70B9, AMBIENCE_OFFSET+9, 4);
//	Patch<DWORD>(0x4D70C6, AMBIENCE_OFFSET+11, 4);
//	Patch<DWORD>(0x4D70D0, AMBIENCE_OFFSET+13, 4);
//	Patch<DWORD>(0x4D70DA, AMBIENCE_OFFSET+17, 4);
//	Patch<DWORD>(0x4D70E4, AMBIENCE_OFFSET+19, 4);
//	Patch<DWORD>(0x4D70EE, AMBIENCE_OFFSET+21, 4);
//	Patch<DWORD>(0x4D70F8, AMBIENCE_OFFSET+23, 4);
//	Patch<DWORD>(0x4D7102, AMBIENCE_OFFSET+24, 4);
//	Patch<DWORD>(0x4D7109, AMBIENCE_OFFSET+38, 4);
//	Patch<DWORD>(0x4D7113, AMBIENCE_OFFSET+27, 4);
//	Patch<DWORD>(0x4D711A, AMBIENCE_OFFSET+35, 4);
//	Patch<DWORD>(0x4D7121, AMBIENCE_OFFSET+36, 4);
//	Patch<DWORD>(0x4D7128, AMBIENCE_OFFSET, 4);
//	Patch<DWORD>(0x4D712F, AMBIENCE_OFFSET+2, 4);
//	Patch<DWORD>(0x4D7136, AMBIENCE_OFFSET+16, 4);
//	Patch<DWORD>(0x4D713D, AMBIENCE_OFFSET+28, 4);
	Patch<DWORD>(0x4D7378, AMBIENCE_OFFSET);
	Patch<DWORD>(0x4D73B1, AMBIENCE_OFFSET);
//	Patch<DWORD>(0x4D74B5, AMBIENCE_OFFSET+31, 4);
//	Patch<DWORD>(0x4D74D5, AMBIENCE_OFFSET+31, 4);
	Patch<DWORD>(0x4D7532, AMBIENCE_OFFSET);

	Nop(0x5D3F08, 5);
	Nop(0x5D4108, 5);

	InjectHook(0x4E81F3, &CAERadioTrackManager::ResetExtra, PATCH_JUMP);
	InjectHook(0x5B962B, &RadioResetWrapper, PATCH_JUMP);
	InjectHook(0x5D40DA, &SaveRadios, PATCH_JUMP);
	InjectHook(0x5D42DA, &LoadRadios, PATCH_JUMP);

	Patch<BYTE>(0x4842CD, sizeof(iBeatsStreams)/sizeof(short) - 3);

	Patch<BYTE>(0x4F31A7, 0);

	// Radio EQ scratched
	Patch<DWORD>(0x4E9DB5, 0x909000B2);
	Patch<DWORD>(0x5B9530, 0x90045D88);

	// Cutscene data
	InjectHook(0x5B10C0, &SetCutsceneModelHack, PATCH_JUMP);
	Patch<const void*>(0x5AFA51, &cutsceneStreams);
	Patch<const void*>(0x5AFA5A, cutsceneStreams);
//	charptr(0x5B10C1, "csplr");
	Nop(0x4D5EA3, 5);
	Nop(0x4668F4, 2);

	// Fullscreen message menu blue
	Patch<WORD>(0x574056, 0x6AFF);
	Patch<BYTE>(0x574058, MENU_INACTIVE_B);
	Patch<BYTE>(0x574059, 0x6A);
	Patch<BYTE>(0x57405A, MENU_INACTIVE_G);
	Patch<BYTE>(0x57405B, 0x6A);
	Patch<BYTE>(0x57405C, MENU_INACTIVE_R);
	Patch<DWORD>(0x57405D, 0x1C244C8D);
	InjectHook(0x574061, 0x7170C0);

	// Own language switch
	Patch<BYTE>(0x69FCF7, NUM_LANGUAGES - 1);
	Patch<const void*>(0x69FCFD, &_CText__loadMission_Jumptable);
	Patch<BYTE>(0x6A01D4, NUM_LANGUAGES - 1);
	Patch<const void*>(0x6A01DA, &_CText__load_Jumptable);

#ifdef INCLUDE_MULTIFONTFILES
	// Multiple font files
	_asm
	{
		mov		eax, offset CText::ReloadFontsFiles
		mov		dwFunc, eax
	}
	call(0x5BA69E, &MultipleFontsTXD_Inject, PATCH_JUMP);
	call(0x7187D6, &MultipleFontsDAT_Inject, PATCH_JUMP);
	call(0x5BD7AB, &MultipleFontsFiles_Validator, PATCH_JUMP);
	call(0x5732A3, dwFunc, PATCH_NOTHING);
#endif
	// Fonts related things
	Patch<WORD>(0x718B6C, 4);
	Patch<BYTE>(0x718B6F, 0x85);
	Patch<WORD>(0x7199D3, 0x0CEB);
	Patch<void*>(0x71985E, CFont::Sprite);
	Patch<void*>(0x719B08, CFont::Sprite);
	Patch<void*>(0x718797, CFont::Size->bPropValue);
	Patch<void*>(0x7188E7, CFont::Size->bPropValue);
	Patch<void*>(0x719704, CFont::Size->bPropValue);
	Patch<void*>(0x7197A6, CFont::Size->bPropValue);
	Patch<void*>(0x719A91, CFont::Size->bPropValue);
	Patch<void*>(0x7188BF, &CFont::Size->bSpaceChar);
	Patch<void*>(0x7187AD, &CFont::Size->bUnpropValue);
	Patch<void*>(0x718985, &CFont::Size->bUnpropValue);
	Patch<void*>(0x71972E, &CFont::Size->bUnpropValue);
	Patch<void*>(0x7197DF, &CFont::Size->bUnpropValue);
	Patch<void*>(0x719AAF, &CFont::Size->bUnpropValue);
	InjectHook(0x7192C0, &CFont::FindSubFontCharacter, PATCH_JUMP);
	//call(0x5BA6BD, &LoadFontsHack, PATCH_JUMP);
	//call(0x7189B2, &ReleaseFontsHack, PATCH_JUMP);
	InjectHook(0x719490, &CFont::SetFontStyle, PATCH_JUMP);
	InjectHook(0x5BD76A, &CFont::Initialise);
	InjectHook(0x53BBA7, &CFont::Shutdown);
	Nop(0x718B64, 6);
	//call(0x7196DB, &CFont::FindSubFontCharacter, PATCH_NOTHING);
	//call(0x719770, &CFont::FindSubFontCharacter, PATCH_NOTHING);
	//call(0x7199C3, &CFont::FindSubFontCharacter, PATCH_NOTHING);

	// Language regional settings
	InjectHook(0x74747E, &InitialiseLanguage);

	// SPTA support
	InjectHook(0x5900B0, &LoadSplashes, PATCH_JUMP);
	InjectHook(0x58FF60, &LoadingScreen, PATCH_JUMP);

	Nop(0x574673, 5);
	Nop(0x5746A0, 5);
	Nop(0x5746CE, 5);
	Patch<WORD>(0x573088, 0xCB8B);
	InjectHook(0x572ED8, 0x57308A, PATCH_JUMP);
	InjectHook(0x57308A, &CMenuManager::ReadFrontendTextures);

	/*_asm
	{
		mov		eax, offset CSprite2d::SetTextureNoMask
		mov		dwFunc, eax
	}
	call(0x727270, dwFunc, PATCH_JUMP);*/


	// Autoupdater
	InjectHook(0x53E77C, &CUpdateManager::Process);
	InjectHook(0x53BF4E, &UpdaterProcessHack);
	InjectHook(0x579526, &UpdaterMenuDrawHack, PATCH_JUMP);
	InjectHook(0x579D50, &UpdaterTextSwap, PATCH_JUMP);
//	InjectHook(0x576E13, &MenuToggleHack, PATCH_JUMP
//	InjectHook(0x573680, &SetToNewMenuHack, PATCH_JUMP);
	Patch<const void*>(0x57CD84, PCMenuActionsTable);
	Patch<const void*>(0x57CD8B, PCMenuActionsAddresses);
	Patch<BYTE>(0x57CD74, sizeof(PCMenuActionsTable)-1);
	Patch<WORD>(0x573830, 0xCE8B);
	Patch<BYTE>(0x573832, 0x5E);
	InjectHook(0x573833, &CMenuManager::SwitchToNewScreenVCS, PATCH_JUMP);
	InjectHook(0x579330, &CMenuManager::MessageScreen, PATCH_JUMP);

	static DWORD	dwDummy;
	// Savegame compatibility
//	call(0x5D0F50, &C_PcSave::MakeTimestamp, PATCH_JUMP);
//	call(0x5D18EB, &TimestampSignatureHack, PATCH_JUMP);
	InjectHook(0x5D14D5, &CTheScripts::SaveAllScripts);
	InjectHook(0x5D18F0, &CTheScripts::LoadAllScripts);
	InjectHook(0x464BC0, &CTheScripts::ZeroMissionVars, PATCH_JUMP);
	InjectHook(0x5D19C0, &CEntryExitManager::Load);
	InjectHook(0x5D1598, &CEntryExitManager::Save);
	InjectHook(0x5D4640, &CPed::Load_Stub, PATCH_JUMP);
	InjectHook(0x5D5730, &CPed::Save_Stub, PATCH_JUMP);
	//InjectHook(0x579434, &0x53D840, PATCH_CALL);
	Patch<WORD>(0x5D334F, 0x08EB);
	Patch<void*>(0x5D327E, &dwDummy);
	Nop(0x5D0887, 2);
	Nop(0x5D150C, 5);
	Nop(0x5D1516, 5);
	Nop(0x5D1520, 5);
	Nop(0x5D1538, 5);
	// Stats - need map!
	Nop(0x5D1560, 5);
	Nop(0x5D157C, 5);

	Nop(0x5D0897, 2);
	Nop(0x5D1934, 5);
	Nop(0x5D193E, 5);
	Nop(0x5D1948, 5);
	Nop(0x5D1960, 5);
	Nop(0x5D1988, 5);
	Nop(0x5D19A4, 5);
	//Patch<DWORD>(0x579439, 0x90000CC2);
	InjectHook(0x53C71F, &LoadGameFailedMessage_Inject, PATCH_JUMP);

	// Save Fallback system
//	patch(0x5D18AC, 0x150, 4);
//	Patch<BYTE>(0x5D14AD, 0x0C);
//	Patch<WORD>(0x5D1C19, 0x05EB);
	Patch<BYTE>(0x5D18E0, 0x1C);
	Patch<BYTE>(0x5D1C12, 0xB8);
	Patch<DWORD>(0x5D1C13, SAVE_FALLBACK_BLOCK0_VERSION);
	InjectHook(0x6191E9, &SaveFallback_FallbackPreBeta3Names, PATCH_JUMP);
	InjectHook(0x5D188E, &SaveFallback_InjectOnLoad, PATCH_JUMP);
	InjectHook(0x5D149D, &SaveFallback_InjectOnSave, PATCH_JUMP);
	InjectHook(0x5D18E1, &SaveFallback_WrapAroundFirstBlock, PATCH_JUMP);
	// This CRT needs to be replaced
	InjectHook(0x6191D1, &CFileMgr::OpenFile);
	InjectHook(0x6191E4, &CFileMgr::Seek);
	InjectHook(0x6191F7, &CFileMgr::Read);
	InjectHook(0x61923C, &CFileMgr::CloseFile);
	Nop(0x5D18D8, 5);
	Nop(0x5D1EA9, 2);

	// Frame Limiter
	Patch<DWORD>(0x53E923, 0x42EB56);
	Patch<WORD>(0x748D81, 0x1B0);
	Patch<WORD>(0x748D98, 0xC030);
	InjectHook(0x748D83, &MaxosFrameLimitHack, PATCH_JUMP);
	InjectHook(0x748D9A, &MaxosFrameLimitHack, PATCH_JUMP);

	InjectHook(0x57CECA, &FrameLimit_SwitchInject, PATCH_JUMP);
	InjectHook(0x579EF3, &FrameLimit_StringInject);
	InjectHook(0x57CD05, &FrameLimit_SetFPS, PATCH_JUMP);
	/*call(0x573BB8, &FrameLimit_SetFPS2, PATCH_CALL);
	Nop(0x573BBD, 1);*/
	InjectHook(0x573BF0, &FrameLimit_SetFPS2, PATCH_JUMP);
	Nop(0x573BB8, 3);

	// No green blip
	Patch<BYTE>(0x53E1EC, 0xEB);

	// HJ
//	Patch<BYTE>(0x55AC70, sizeof(HJ_Stats_Jumptable) / sizeof(void*) - 1);
//	patch(0x55AC7A, &HJ_Stats_Jumptable, 4);

	// RefFix
	Patch<const void*>(0x6FB97A, &pRefFal);
	Patch<BYTE>(0x6FB9A0, 0);

	// Relocated sun
	Patch<float>(0x560A76, 1.0f);
	Nop(0x560A84, 6);

	// More vehicles
#if NUM_VEHICLE_MODELS > 212
	#pragma message ("INFO: Compiling more vehicles code...")
	ModelCarsMalloc = new ModelCarsData;
	if ( ModelCarsMalloc )
	{
		memset(ModelCarsMalloc, 0, sizeof(ModelCarsData));
		patch(0x4C64ED, &ModelCarsMalloc->count, 4);
		patch(0x4C6508, &ModelCarsMalloc->count, 4);
		patch(0x4C6771, &ModelCarsMalloc->count, 4);
		patch(0x4C6786, &ModelCarsMalloc->count, 4);
		patch(0x4C6853, &ModelCarsMalloc->count, 4);
		patch(0x84BCD1, ModelCarsMalloc, 4);
		patch(0x8562A1, ModelCarsMalloc, 4);

		patch(0x4C64F8, &ModelCarsMalloc->objects, 4);
		patch(0x4C6780, &ModelCarsMalloc->objects, 4);

		patch(0x4C5F5C, NUM_VEHICLES, 4);
		patch(0x4C6376, NUM_VEHICLES, 4);
	}
#endif

	// Pre-CDynamicStore code
	/*patch(0x4C6518, &CModelInfo::ms_pedModelStore.m_NumObjects, 4);
	patch(0x4C652F, &CModelInfo::ms_pedModelStore.m_NumObjects, 4);
	patch(0x4C67A1, &CModelInfo::ms_pedModelStore.m_NumObjects, 4);
	patch(0x4C67B3, &CModelInfo::ms_pedModelStore.m_NumObjects, 4);
	patch(0x4C6859, &CModelInfo::ms_pedModelStore.m_NumObjects, 4);*/
	//patch(0x84BCF1, CModelInfo::ms_pedModelStore, 4);
	//patch(0x8562B1, CModelInfo::ms_pedModelStore, 4);

	/*patch(0x4C6523, &CModelInfo::ms_pedModelStore.m_Objects, 4);
	patch(0x4C67AD, &CModelInfo::ms_pedModelStore.m_Objects, 4);*/

/*#if NUM_PED_MODELS != 278
	patch(0x4C67DC, NUM_PED_MODELS, 4);
	patch(0x4C6396, NUM_PED_MODELS, 4);
#endif*/

	//Patch<BYTE>(0x4C67E3, sizeof(CPedModelInfoVCS));
	//Patch<BYTE>(0x4C639B, sizeof(CPedModelInfoVCS));
	//Patch<BYTE>(0x4C6536, sizeof(CPedModelInfoVCS));
	//Patch<BYTE>(0x4C67AA, sizeof(CPedModelInfoVCS));

	// Modelinfo patches
	CModelInfo::Inject();

	//call(0x5B3D8E, &CModelInfo::AddDamageAtomicModel, PATCH_NOTHING);
	InjectHook(0x5B85DD, &CFileLoader::LoadObject);

	// Old style Rhino
	Patch<const void*>(0x6A80CA, &fRhinoHitStrength);

	// SF weather
	Patch<BYTE>(0x72A640, 0xE9);
	Patch<DWORD>(0x72A641, 0xE3);
	Patch<BYTE>(0x72A731, 0xC3);
	Patch<BYTE>(0x705065, 0xEB);

	// Force metric system
	InjectHook(0x56D220, &UseMetricSystem, PATCH_JUMP);

	// Radio icons tweaks
	InjectHook(0x579538, &CMenuManager::DrawRadioStationIcons, PATCH_NOTHING);
	/*patch(0x574702, 91, 4);
	patch(0x57470D, 10, 4);
	Patch<BYTE>(0x57480A, 11);
	patch(0x574889, 0x8120C283, 4);*/

	Patch<BYTE>(0x57A2BD, 4);

	// 12H Clock menu options
	InjectHook(0x577086, &Clock_SwitchInject, PATCH_JUMP);
	InjectHook(0x579E6A, &Clock_StringInject);

	// Pink menu header
	Patch<BYTE>(0x5795F3, 11);
	Patch<BYTE>(0x57F6C6, 11);

	// active menu entry RGB
	Patch<DWORD>(0x579A63, MENU_ACTIVE_B);
	Patch<DWORD>(0x579A68, MENU_ACTIVE_G);
	Patch<DWORD>(0x579A6D, MENU_ACTIVE_R);
	/*patch(0x5768D3, MENU_ACTIVE_B, 4);
	patch(0x5768D8, MENU_ACTIVE_G, 4);
	patch(0x5768DD, MENU_ACTIVE_R, 4);*/
	Patch<DWORD>(0x57615D, MENU_ACTIVE_B);
	Patch<DWORD>(0x576162, MENU_ACTIVE_G);
	Patch<DWORD>(0x576167, MENU_ACTIVE_R);

	// inactive menu entry RGB
	/*Patch<BYTE>(0x576913, MENU_INACTIVE_B);
	Patch<BYTE>(0x576915, MENU_INACTIVE_G);
	Patch<BYTE>(0x576917, MENU_INACTIVE_R);*/
	Patch<BYTE>(0x579767, MENU_INACTIVE_B);
	Patch<BYTE>(0x579769, MENU_INACTIVE_G);
	Patch<BYTE>(0x57976B, MENU_INACTIVE_R);
	/*Patch<BYTE>(0x579A88, MENU_INACTIVE_B);
	Patch<BYTE>(0x579A8A, MENU_INACTIVE_G);
	Patch<BYTE>(0x579A8C, MENU_INACTIVE_R);*/
	Patch<BYTE>(0x57FD08, MENU_INACTIVE_B);
	Patch<BYTE>(0x57FD0A, MENU_INACTIVE_G);
	Patch<BYTE>(0x57FD0C, MENU_INACTIVE_R);
	Patch<BYTE>(0x579F2E, MENU_LOCKED_B);
	Patch<BYTE>(0x579F30, MENU_LOCKED_G);
	Patch<BYTE>(0x579F32, MENU_LOCKED_R);
	InjectHook(0x579A87, &MenuEntryColourHack, PATCH_JUMP);


	// Custom Tracks slider
	Patch<BYTE>(0x57BD50, 0xB4);
	Patch<BYTE>(0x57BD56, 0x82);
	Patch<BYTE>(0x57BD58, 0xED);
	patch(0x57BCA0, &CMenuManager::ms_nRubberSlider, 4);
	patch(0x57BCB0, &CMenuManager::ms_nRubberSlider, 4);
	patch(0x57BD9A, &CMenuManager::ms_nRubberSlider, 4);
	patch(0x57BDE6, &CMenuManager::ms_nRubberSlider, 4);
	patch(0x57BCFD, &fCTSliderRight, 4);
	patch(0x57BD0B, &WidescreenSupport::fCTSliderRight, 4);
	patch(0x57BD42, &WidescreenSupport::fCTSliderLeft, 4);

	patch(0x57BE02, MENU_INACTIVE_B, 4);
	patch(0x57BE0B, MENU_INACTIVE_G, 4);
	patch(0x57BE10, MENU_INACTIVE_R, 4);

	// Fixed menu fonts
	Patch<BYTE>(0x579929, FONT_Eurostile);
	Patch<BYTE>(0x5799AD, FONT_Eurostile);
	//Patch<BYTE>(0x5799AD, FONT_PagerFont);
	Patch<BYTE>(0x57A20B, FONT_Eurostile);
	Patch<BYTE>(0x57E280, FONT_Eurostile);
	Patch<BYTE>(0x57FCC3, FONT_Eurostile);
	Patch<BYTE>(0x5760F7, FONT_Eurostile);
	Patch<BYTE>(0x57FA41, FONT_Eurostile/*FONT_RageItalic*/);

	// Smaller outline
	Patch<BYTE>(0x579A24, 1);
	Patch<BYTE>(0x579739, 1);
	Patch<BYTE>(0x575F21, 1);

	Patch<DWORD>(0x575F7C, 0x402444DB);
	Patch<BYTE>(0x575F81, 0xC1);
	Patch<DWORD>(0x576096, 0);
	InjectHook(0x575F22, 0x719570);
	Patch<BYTE>(0x575F28, ALIGN_Left);
	Patch<const void*>(0x575F58, &fMapZonePosX);
	Patch<const void*>(0x575F66, &WidescreenSupport::fMapZonePosX2);
	Patch<float>(0x575E6E, 1280.0);

	Patch<BYTE>(0x5795E9, 2);
	InjectHook(0x5795EA, 0x719570);

	// ONE intro splash (FINALLY)
	Patch<DWORD>(0x748EF8, 0x748AC6);
//	call(0x748ABB, &DeleteSecondSplash, PATCH_JUMP);
//	call(0x748AED, &RestoreSecondSplash, PATCH_NOTHING);
	Patch<BYTE>(0x748AE6, 0x14);

	// Outro splash
	InjectHook(0x57BA5F, &CMenuManager::DrawOutroSplash, PATCH_NOTHING);

	// Stats Menu
	InjectHook(0x57954A, &CMenuManager::PrintStats, PATCH_NOTHING);
	Patch<const void*>(0x577370, &StatsMenuActionHack);

	// Brightness
	Patch<float>(0x573B8A, 96.0 / 512.0);
	Patch<DWORD>(0x573B96, 96);
	Patch<const void*>(0x57A8A9, &fBrightnessStep);
	Patch<const void*>(0x573487, &fBrightnessStep2);
	Patch<const void*>(0x5734AD, &fBrightnessMax);
	Patch<const void*>(0x5734BC, &fBrightnessMax);

	// Tweaked draw distance
	InjectHook(0x5735C8, &DrawDistanceRecalc);

	// Widescreen
	Patch<DWORD>(0x745B71, 0x9090687D);
	Patch<DWORD>(0x74596C, 0x9090127D);
	Nop(0x745970, 2);
	Nop(0x745B75, 2);
	//Nop(0x745BC9, 2);
	Nop(0x7459E1, 2);

//	call(0x5BD61A, &CamHack, PATCH_JUMP);

	// Native res
	static char		aNoDesktopMode[64];
	RECT			desktop;
	GetWindowRect(GetDesktopWindow(), &desktop);
	_snprintf(aNoDesktopMode, sizeof(aNoDesktopMode), "Cannot find %dx%dx32 video mode", desktop.right, desktop.bottom);

	Patch<DWORD>(0x746363, desktop.right);
	Patch<DWORD>(0x746368, desktop.bottom);
	Patch<const char*>(0x7463C8, aNoDesktopMode);

	// BINK video player
	InjectHook(0x748AFA, &VideoPlayerCreate1, PATCH_JUMP);
	Patch<const void*>(0x748F04, &VideoPlayerCreate2);
	Patch<const void*>(0x748F0C, &VideoPlayerRelease);
//	InjectHook(0x748BC9, &VideoPlayerCreate2, PATCH_JUMP);
	InjectHook(0x748BB9, &VideoPlayerPlayNextFrame);
	InjectHook(0x7480D6, &VideoPlayerProc, PATCH_JUMP);

	// Disable re-initialization of DirectInput mouse device by the game
	Patch<BYTE>(0x576CCC, 0xEB);
	Patch<BYTE>(0x576EBA, 0xEB);
	Patch<BYTE>(0x576F8A, 0xEB);

	// Make sure DirectInput mouse device is set non-exclusive (may not be needed?)
	Patch<DWORD>(0x7469A0, 0x909000B0);

	// Commandline arguments
	Patch<const void*>(0x619C40, &CommandlineEventHack);
	InjectHook(0x7488FB, &ReadCommandlineFile, PATCH_JUMP);

	// DLC support
	Patch<const char*>(0x5BF8B2, CFileLoader::GetParticlesPath());
	Patch<const char*>(0x5BCFF7, CFileLoader::GetPedgrpPath());
	Patch<const char*>(0x5BC0AA, CFileLoader::GetPopcyclePath());
	Patch<const char*>(0x5BBADA, CFileLoader::GetTimecycPath());

	//Nop(0x53BC8B, 5);
	//call(0x53BC95, &CDLCManager::LoadLevelFiles, PATCH_CALL);
	InjectHook(0x748C3F, &LoadGameWithDLCHack);
	InjectHook(0x53BC95, &CFileLoader::ParseLevelFile, PATCH_CALL);
	InjectHook(0x53BC9B, &CFileLoader::ParseLevelFile, PATCH_CALL);
	InjectHook(0x53BCA0, &CFileLoader::LoadLevels, PATCH_CALL);
	InjectHook(0x5B8980, &CFileLoader::LoadEntryExit);
	InjectHook(0x5B8634, &CFileLoader::LoadWeaponObject);
	Patch<BYTE>(0x5B9299, 0xC3);
	Nop(0x5B931E, 3);
	Nop(0x5BCFEC, 5);
	Nop(0x5BD00B, 5);
	Nop(0x5BC09F, 5);
	Nop(0x5BC0BE, 5);
	Nop(0x5BBACF, 5);
	Nop(0x5BBAEA, 5);

	// ColAccel
	//InjectHook(0x53BC8B, &CColAccel::startCache);
	//Patch<WORD>(0x5B2BA1, 0x30EB);

	// Pools fixes
	InjectHook(0x4048E0, &VehiclePoolGetAt, PATCH_JUMP);
	InjectHook(0x404910, &PedPoolGetAt, PATCH_JUMP);

	// Children & custom pools
	InjectHook(0x5BF85B, &CPools::Initialise);
	InjectHook(0x53CA5C, &CPools::ShutDown);

	// Empire Buildings
	Patch<void*>(0x495F38, func_069C);
	Patch<void*>(0x495F3C, func_069C);
	Patch<void*>(0x495F40, func_069C);
	Patch<void*>(0x495F44, func_069C);
	Patch<void*>(0x495F48, func_069C);
	Patch<void*>(0x495F4C, func_069C);
	Patch<void*>(0x495F50, func_069C);
	Patch<void*>(0x495F68, func_069C);
	Patch<void*>(0x495F6C, func_069C);
	Patch<void*>(0x495F70, func_069C);
	Patch<void*>(0x495F74, func_069C);
	Patch<void*>(0x495F78, func_069C);
	Patch<void*>(0x495F7C, func_069C);

	Patch<const char*>(0x4111AE, "empire_perma");
	InjectHook(0x53C215, &CEmpireManager::Process);

	// Menu background
	Patch<DWORD>(0x57B764, 0x36EBC030);
	InjectHook(0x57B7A1, 0x57B9F4, PATCH_JUMP);
	Patch<WORD>(0x57B9F4, 0xCE8B);
	InjectHook(0x57B9F6, &CMenuManager::DrawBackEnd, PATCH_CALL);
	Patch<WORD>(0x57B9FB, 0x05EB);

	// More centered stuff in menu, sliders
	InjectHook(0x57A218, &MenuEntriesAlignHack, PATCH_JUMP);
	InjectHook(0x57A3B1, &MenuEntriesPositionHack_Inject, PATCH_JUMP);
	//Patch<BYTE>(0x57A219, ALIGN_Left);
	//patch(0x57A3DF, &fMenuTextsPosX, 4);
	//patch(0x57A3ED, &fMenuTextsPosX2, 4);

	// -//- related to sliders
	InjectHook(0x576860, &CMenuManager::DisplaySlider, PATCH_JUMP);
	/*patchf(0x57A881, 640.0 - MENU_TEXT_POSITION_RCOLUMN);
	patchf(0x57AA75, 640.0 - MENU_TEXT_POSITION_RCOLUMN);
	patchf(0x57ACB2, 640.0 - MENU_TEXT_POSITION_RCOLUMN);
	patchf(0x57AEA4, 640.0 - MENU_TEXT_POSITION_RCOLUMN);
	patchf(0x57B0D8, 640.0 - MENU_TEXT_POSITION_RCOLUMN);*/
	Patch<float>(0x57A881, 320.0 + MENU_TEXT_POSITION_RCOLUMN);
	Patch<float>(0x57AA75, 320.0 + MENU_TEXT_POSITION_RCOLUMN);
	Patch<float>(0x57ACB2, 320.0 + MENU_TEXT_POSITION_RCOLUMN);
	Patch<float>(0x57AEA4, 320.0 + MENU_TEXT_POSITION_RCOLUMN);
	Patch<float>(0x57B0D8, 320.0 + MENU_TEXT_POSITION_RCOLUMN);
	Patch<float>(0x57AC6F, MENU_SLIDER_POSY - MENU_SLIDER_HEIGHT);
	Patch<float>(0x57A841, MENU_SLIDER_POSY - MENU_SLIDER_HEIGHT / 2);
	Patch<float>(0x57AE6A, MENU_SLIDER_POSY - MENU_SLIDER_HEIGHT / 2);
	Patch<float>(0x57B09A, MENU_SLIDER_POSY - MENU_SLIDER_HEIGHT);
	Patch<float>(0x57AA35, MENU_SLIDER_POSY - 30.0 - MENU_SLIDER_HEIGHT);
	Patch<float>(0x57A82E, MENU_SLIDER_HEIGHT);
	Patch<float>(0x57AA25, MENU_SLIDER_HEIGHT);
	Patch<float>(0x57AC59, MENU_SLIDER_HEIGHT);
	Patch<float>(0x57AE5A, MENU_SLIDER_HEIGHT);
	Patch<float>(0x57B088, MENU_SLIDER_HEIGHT);
	Patch<const void*>(0x57A88F, &WidescreenSupport::fMenuSliderPosX);
	Patch<const void*>(0x57AA83, &WidescreenSupport::fMenuSliderPosX);
	Patch<const void*>(0x57ACC0, &WidescreenSupport::fMenuSliderPosX);
	Patch<const void*>(0x57AEB2, &WidescreenSupport::fMenuSliderPosX);
	Patch<const void*>(0x57B0E6, &WidescreenSupport::fMenuSliderPosX);
	Patch<const void*>(0x57A86A, &WidescreenSupport::fMenuSliderPosY4);
	Patch<const void*>(0x57AC9B, &WidescreenSupport::fMenuSliderPosY2);
	Patch<const void*>(0x57AE90, &WidescreenSupport::fMenuSliderPosY4);
	Patch<const void*>(0x57B0C1, &WidescreenSupport::fMenuSliderPosY2);
	Patch<const void*>(0x57AA5E, &WidescreenSupport::fMenuSliderPosY3);
	Patch<const void*>(0x57A7EC, &fMenuSliderWidth);
	Patch<const void*>(0x57A9E6, &fMenuSliderWidth);
	Patch<const void*>(0x57AC14, &fMenuSliderWidth);
	Patch<const void*>(0x57AE1E, &fMenuSliderWidth);
	Patch<const void*>(0x57B04C, &fMenuSliderWidth);
	Patch<const void*>(0x57A809, &WidescreenSupport::fMenuSliderWidth);
	Patch<const void*>(0x57AA00, &WidescreenSupport::fMenuSliderWidth);
	Patch<const void*>(0x57AC31, &WidescreenSupport::fMenuSliderWidth);
	Patch<const void*>(0x57AE38, &WidescreenSupport::fMenuSliderWidth);
	Patch<const void*>(0x57B066, &WidescreenSupport::fMenuSliderWidth);
	Patch<const void*>(0x57A84F, &WidescreenSupport::fMenuSliderHeight2);
	Patch<const void*>(0x57AA43, &WidescreenSupport::fMenuSliderHeight2);
	Patch<const void*>(0x57AC7D, &WidescreenSupport::fMenuSliderHeight2);
	Patch<const void*>(0x57AE78, &WidescreenSupport::fMenuSliderHeight2);
	Patch<const void*>(0x57B0A9, &WidescreenSupport::fMenuSliderHeight2);
	Patch<const void*>(0x57A813, &WidescreenSupport::f100);
	Patch<const void*>(0x57AA0A, &WidescreenSupport::f100);
	Patch<const void*>(0x57AC3B, &WidescreenSupport::f100);
	Patch<const void*>(0x57AE42, &WidescreenSupport::f100);
	Patch<const void*>(0x57B070, &WidescreenSupport::f100);
//	Patch<BYTE>(0x57A878, 0xEB);

	// Proper widescreen support
	patch(0x579667, &WidescreenSupport::f40, 4);
	patch(0x5795D1, &WidescreenSupport::f1pt3, 4);
	patch(0x579865, &WidescreenSupport::f60, 4);
	patch(0x57971A, &WidescreenSupport::f0pt49, 4);
	patch(0x579983, &WidescreenSupport::f0pt42, 4);
	patch(0x579A03, &WidescreenSupport::f0pt7, 4);
	patch(0x57A2A2, &WidescreenSupport::f0pt35, 4);
	patch(0x57A31B, &WidescreenSupport::f0pt56, 4);
	patch(0x57A370, &WidescreenSupport::f0pt7, 4);
	patch(0x57403F, &WidescreenSupport::fMenuMessageWidth, 4);
	patch(0x5740BE, &WidescreenSupport::f0pt56, 4);
	patch(0x574112, &WidescreenSupport::f95, 4);
	patch(0x574145, &WidescreenSupport::f95, 4);
	patch(0x5763C2, &WidescreenSupport::f0pt49, 4);
	patch(0x5764D0, &WidescreenSupport::f70, 4);
	patch(0x5765C2, &WidescreenSupport::f50, 4);
	patch(0x576605, &WidescreenSupport::f45, 4);
	patch(0x576648, &WidescreenSupport::f55, 4);
	patch(0x57668B, &WidescreenSupport::f50, 4);
	patch(0x576731, &WidescreenSupport::f50, 4);
	patch(0x576774, &WidescreenSupport::f45, 4);
	patch(0x5767B7, &WidescreenSupport::f55, 4);
	patch(0x5767FA, &WidescreenSupport::f50, 4);
	patch(0x575EE1, &WidescreenSupport::f0pt8, 4);
	patch(0x57533E, &WidescreenSupport::f60, 4);
	patch(0x575363, &WidescreenSupport::f580, 4);
	patch(0x575B86, &WidescreenSupport::f4, 4);
	patch(0x576030, &WidescreenSupport::f555, 4);
	patch(0x576060, &WidescreenSupport::f95, 4);
	patch(0x57613D, &WidescreenSupport::f0pt3, 4);
	patch(0x5761BF, &WidescreenSupport::f160, 4);
	patch(0x576217, &WidescreenSupport::f350, 4);

	patch(0x5795AF, &WidescreenSupport::f2pt1_h, 4);
	patch(0x579645, &WidescreenSupport::f28_h, 4);
	patch(0x579843, &WidescreenSupport::f97_centh, 4);
	patch(0x5796F8, &WidescreenSupport::f1pt2_h, 4);
	patch(0x57995A, &WidescreenSupport::f0pt95_h, 4);
	patch(0x5799D7, &WidescreenSupport::f1_h, 4);
	patch(0x57A261, &WidescreenSupport::f0pt95_h, 4);
	patch(0x57A2DD, &WidescreenSupport::f1_h, 4);
	patch(0x57A347, &WidescreenSupport::f1_h, 4);
	patch(0x57639A, &WidescreenSupport::f0pt7_h, 4);
	Patch<const void*>(0x714843, &fSkyMultFix);
	Patch<const void*>(0x714860, &fSkyMultFix);
	Patch<const void*>(0x70CEF8, &WidescreenSupport::fScreenCoorsFix);
	Patch<const void*>(0x71DA8D, &WidescreenSupport::fScreenCoorsFix);
//	patch(0x52C9DB, &WidescreenSupport::fSpawningFix, 4);	// Tmp
	///Patch<const void*>(0x514986, &WidescreenSupport::fAimpointFix);
	//Patch<const void*>(0x50AD53, &WidescreenSupport::fAimpointFix);		// Not needed?
	Patch<const void*>(0x58BBCB, &fTextBoxPosY);
	/*patch(0x58C0DE, &WidescreenSupport::fTextDrawsWidthMultiplier, 4);
	patch(0x58C12D, &WidescreenSupport::fTextDrawsWidthMultiplier, 4);
	patch(0x58C144, &WidescreenSupport::fTextDrawsWidthMultiplier, 4);*/
	InjectHook(0x57E3A5, &HelperPosXHack);
	InjectHook(0x57A186/*0x57A1BA*//*0x57A1C1*/, &MenuEntriesLeftColumnHack, PATCH_JUMP);
	//call(0x57A190, &MenuEntriesLeftColumnHack2, PATCH_JUMP);
	InjectHook(0x579BBF, &MenuEntriesPlaceSave, PATCH_JUMP);
	InjectHook(0x579C7F, &MenuEntriesPlaceSave2, PATCH_JUMP);
	InjectHook(0x72FD75, &WidescreenSupportRecalculateHack, PATCH_JUMP);
	InjectHook(0x745C99, &WidescreenSupportRecalculateHack2, PATCH_JUMP);
	InjectHook(0x746341, &WidescreenSupportRecalculateHack3, PATCH_CALL);
	InjectHook(0x6FF420, &WidescreenSupport::SetAspectRatio, PATCH_JUMP);
//	InjectHook(0x6FF410, &WidescreenSupport::SetFieldOfView, PATCH_JUMP);
	//InjectHook(0x72FCF9, &WidescreenFOVHack, PATCH_JUMP);
	InjectHook(0x514D63, &WidescreenFOVHack2, PATCH_CALL);
	//InjectHook(0x514878, &WidescreenBordersHack);
	InjectHook(0x51499E, &AimpointCalc, PATCH_JUMP);
	InjectHook(0x5BC8FF, &CameraInitHack, PATCH_JUMP);
	InjectHook(0x58C3C8, &WidescreenTextPositionHack, PATCH_CALL);
	InjectHook(0x58BB88, &WidescreenSupport::GetTextBoxPos, PATCH_CALL);
//	InjectHook(0x714841, &WidescreenSkyWidthHack, PATCH_CALL);
	InjectHook(0x57706E, &Widescreen_SwitchInject, PATCH_JUMP);
	InjectHook(0x579DE9, &Widescreen_StringInject);
//	InjectHook(0x58C0BB, &Widescreen_TextDrawsFix, PATCH_CALL);
	InjectHook(0x58C1EC, &Widescreen_TextDrawsFix2, PATCH_JUMP);
	InjectHook(0x72FC70, &CameraSize, PATCH_JUMP);
	InjectHook(0x514860, &CCamera::DrawBordersForWideScreen, PATCH_JUMP);
	Patch<WORD>(0x58BB8D, 0x05EB);
	Nop(0x50AD79, 6);
	Nop(0x58C3CD, 1);
//	Nop(0x714846, 1);
	Nop(0x514D68, 1);
	Nop(0x53E2AD, 2);
//	Nop(0x58C0C0, 1);

	// 64 radar tiles
	DWORD dwOldSlots = *(DWORD*)0x584C98;
	Patch<DWORD>(0x584C98, dwOldSlots - 0x10);
	Patch<DWORD>(0x584C9F, 7);
	Patch<DWORD>(0x584CAB, 56);
	Patch<DWORD>(0x584CDF, dwOldSlots - 0x10);
	Patch<DWORD>(0x584CEB, 56);
	Patch<BYTE>(0x584D0D, 8);
	Patch<BYTE>(0x584D11, 32);
	Patch<DWORD>(0x584D14, 64);
	Patch<DWORD>(0x584D24, dwOldSlots - 0x0C);
	Patch<const void*>(0x5858DA, &fRadarTileDimensions);
	Patch<const void*>(0x5858F9, &fRadarTileDimensions);
	Patch<const void*>(0x586894, &fRadarTileDimensions);
	Patch<const void*>(0x5868A3, &fRadarTileDimensions);
	Patch<const void*>(0x586362, &fRadarTileDimensions);
	Patch<const void*>(0x586370, &fRadarTileDimensions);
	Patch<const void*>(0x585907, &fRadarTileDimensions2);
	Patch<const void*>(0x5868CC, &fRadarTileDimensions2);
	Patch<DWORD>(0x588037, 64);
	Patch<BYTE>(0x586142, 7);
	Patch<BYTE>(0x586157, 7);
	Patch<BYTE>(0x586174, 0x2D);
	Patch<DWORD>(0x586344, 8);
	Patch<BYTE>(0x584B60, 7);
	Patch<BYTE>(0x584B64, 7);
	Patch<BYTE>(0x584B74, 7);
	Patch<BYTE>(0x584BBA, 7);
	Patch<BYTE>(0x584BC7, 7);
	Patch<BYTE>(0x58652F, 7);
	Patch<BYTE>(0x586542, 7);
	Patch<BYTE>(0x584BCC, 0x00);
	Patch<BYTE>(0x586553, 0x00);
	Patch<BYTE>(0x584B7D, 0x09);
	Patch<signed char>(0x584D9A, -4);
	Patch<DWORD>(0x584DA2, 3);
	Patch<DWORD>(0x586433, 0x8D * HUD_TRANSPARENCY / 0xFF);
	Patch<DWORD>(0x586438, 0xE0);
	Patch<DWORD>(0x58643D, 0xAE);
	Patch<BYTE>(0x586442, 0x7F);
	Patch<DWORD>(0x58647C, HUD_TRANSPARENCY);
	Patch<DWORD>(0x5864BD, HUD_TRANSPARENCY);
	Patch<DWORD>(0x58A789, HUD_TRANSPARENCY);
	Patch<DWORD>(0x58A88F, HUD_TRANSPARENCY);
	Patch<DWORD>(0x58A8D9, HUD_TRANSPARENCY);
	Patch<DWORD>(0x58A98F, HUD_TRANSPARENCY);
	InjectHook(0x58641A/*0x58645D*/, &OpaqueRadarHack, PATCH_JUMP);
#ifndef V_RADAR_TEST
	InjectHook(0x586500, &OpaqueRadarHack2);
#endif

	// Temp? Map screen
	Patch<float>(0x578825, 2000.0);
	Patch<float>(0x57883D, -2000.0);
	Patch<const void*>(0x57882B, &fRadarTileDimensions);
	Patch<const void*>(0x578856, &fRadarTileDimensions);
	Patch<const void*>(0x578843, &fMinusRadarTileDimensions);
	Patch<const void*>(0x578869, &fMinusRadarTileDimensions);
	InjectHook(0x5754C0, &TempRadarFixFunc, PATCH_JUMP);
	InjectHook(0x5756E0, &TempRadarFixFunc2);
	Patch<WORD>(0x5754B9, 0x02B3);

	// Infinite sea at 8.0
	Patch<float>(0x6E873F, 8.0);

	// Unhidden map
	patch(0x572130, 0xC301B0, 4);
	Patch<WORD>(0x5759F4, 0xE990);

	// Stats.html
	Patch<DWORD>(0x573E6C, 0x015D818A);
	Patch<DWORD>(0x573E70, 0xC0840000);
	Patch<WORD>(0x573E74, 0x6374);
	InjectHook(0x57B4A1, 0x573E6C);
	InjectHook(0x57B544, 0x573E6C);
	InjectHook(0x580078, 0x573E6C);
	//Patch<const char*>(0x57DE43, "<title>Grand Theft Auto Vice City Stories Stats</title><meta http-equiv=\"content-type\" content=\"text/html;charset=utf-8\" />\n");
	//Patch<const char*>(0x57DE59, "<table width=\"580\" align=\"center\" border=\"0\" cellpadding=\"5\" cellspacing=\"0\">\n");
	//Patch<const char*>(0x57DE90, "<strong><font color=\"#FFFFFF\">GRAND THEFT AUTO VICE CITY STORIES ");

	// TEMPORARY CUT
	Nop(0x5773EB, 6);
	/*charptr(0x57DEBC, L"%s</font></strong><br><font\n");
	charptr(0x57E05E, L"%s");
	charptr(0x57DF03, L"<td height=\"40\" colspan=\"2\"> <p><font color=\"#F0000C\" size=\"2\" face=\"Arial, Helvetica, sans-serif\"><strong><font color=\"#F0000C\" size=\"1\">%s: \n");
	charptr(0x57DF2E, L"%s</font><br>        %s: </strong>");


	call(0x69F7E0, &CStats::SACharsToASCII, PATCH_JUMP);
	//call(0x823A30, &fprintf, PATCH_JUMP);
	call(0x57DEC1, &fwprintf, PATCH_NOTHING);
	call(0x57E063, &fwprintf, PATCH_NOTHING);
	call(0x57DF08, &fwprintf, PATCH_NOTHING);
	call(0x57DF33, &fwprintf, PATCH_NOTHING);*/
	/*_asm
	{
		mov		eax, offset CMenuManager::SaveStatsHTML
		mov		dwFunc, eax
	}
	call(0x57DDE0, dwFunc, PATCH_JUMP);*/

	Patch<DWORD>(0x5759C5, offsetof(CMenuManager, textures[15]));

	InjectHook(0x58008D, &CMenuManager::AdditionalOptionInputVCS, PATCH_NOTHING);
	InjectHook(0x57B457, &CMenuManager::UserInputVCS, PATCH_NOTHING);

	// No arrow.txd
	Patch<DWORD>(0x57A511, 0xDBE9);
	Nop(0x57A516, 1);

	// .set alterations
#if SET_FILE_VERSION != 6
	static const DWORD dwSetFileVersion = SET_FILE_VERSION;

	Patch<BYTE>(0x57C983, SET_FILE_VERSION);
	Patch<BYTE>(0x530590, CONTROLS_FILE_VERSION);
	Patch<const void*>(0x57C69A, &dwSetFileVersion);
#endif
	Patch<const char*>(0x7489A0, "controls.set");
	//Patch<WORD>(0x57C6A7, 0x09EB);
	InjectHook(0x57C7E7, &SaveLanguageHack);
	InjectHook(0x57CABB, &LoadLanguageHack);
	InjectHook(0x57C6AD, &CControllerConfigManager::SaveToFile, PATCH_NOTHING);
	InjectHook(0x57C990, &CControllerConfigManager::LoadFromFile, PATCH_NOTHING);
	// Upgrading CRT of CControllerConfigManager::SaveSettings and CControllerConfigManager::LoadSettings
	InjectHook(0x52D220, &CFileMgr::Write);
	InjectHook(0x52D237, &CFileMgr::Write);
	InjectHook(0x530551, &CFileMgr::Read);
	InjectHook(0x530582, &CFileMgr::Read);
	InjectHook(0x5305AA, &CFileMgr::Read);
	InjectHook(0x530605, &CFileMgr::Read);
	InjectHook(0x530575, &CFileMgr::Seek);
	InjectHook(0x5305BF, &CFileMgr::Seek);
	InjectHook(0x5305D7, &CFileMgr::Seek);
	InjectHook(0x5305FC, &CFileMgr::Seek);
	InjectHook(0x7489A4, &CFileMgr::OpenFile);
	InjectHook(0x7489CC, &CFileMgr::CloseFile);
	Nop(0x57C6A7, 1);
	Nop(0x57C98A, 1);
	Nop(0x57C997, 6);

	// Larger 02A7 sphere
//	patchf(0x585CD4, 3.0);
//	patch(0x585CD4, 0x40400000, 4); // 3.0

	// Steady crosshair
	InjectHook(0x58E26C, &CPed::GetCrosshairSize, PATCH_NOTHING);

	// No plane radar stuff
	InjectHook(0x58A3AA, 0x58A5A8, PATCH_JUMP);
	Patch<WORD>(0x5869D2, 0xE990);
	Nop(0x5880B1, 5);

	// VCS radar colours
	InjectHook(0x584770, &CRadar::GetRadarTraceColour, PATCH_JUMP);
	Patch<DWORD>(0x5838B6, 5);
	Patch<DWORD>(0x583A17, 0);

	// No Tab stats
	Patch<BYTE>(0x58FC2C, 0xEB);

	// FPS meter & gridref stuff
	Patch<BYTE>(0x58FD0E, 0xE8);
	InjectHook(0x58FD13, &CHud::DrawPermanentTexts, PATCH_JUMP);
//	call(0x58FD18, &CHud::DrawDevLogos, PATCH_JUMP);
	//call(0x58FD18, &CGridref::Draw, PATCH_JUMP);

	// Bar
	Patch<BYTE>(0x590376, 11);
	Patch<BYTE>(0x590389, 11);
	Patch<BYTE>(0x5903A1, 11);
	Patch<const void*>(0x5903CA, &LoadingBarPosX);
	Patch<const void*>(0x5903E8, &LoadingBarPosY);
	Patch<const void*>(0x5903F6, &LoadingBarWidth);
	Patch<const void*>(0x590405, &LoadingBarHeight);
	Patch<BYTE>(0x590478, 2);
	//patch(0x590D2B, 93, 4);
	//patch(0x590D68, 93, 4);
	InjectHook(0x590480, &CHud::DrawSquareBar);

	// One loading music
	Patch<WORD>(0x5B9B1F, 0xC030);
	Nop(0x5B9B21, 3);
//	Nop(0x748CF6, 5);

	// No player.img & clothes.dat
	Nop(0x5A834D, 5);

	// No hunger
	Nop(0x5700F0, 5);

	// HUD.txd stuff
	/*patch(0x588852, &hudTextures, 4);
	patch(0x588862, (DWORD)&hudTextures + sizeof(hudTextures), 4);
	patch(0x58D984, &hudTextures, 4);

	patch(0x5BA87F, &hudTextures, 4);
	patch(0x5BA884, &hudTextureNames, 4);
	patch(0x5BA888, NUM_HUD_TEXTURES, 4);

	patch(0x58E3A1, &hudTextures[HUD_SiteM16], 4);
	patch(0x58E3F6, &hudTextures[HUD_SiteM16], 4);
	patch(0x58E44B, &hudTextures[HUD_SiteM16], 4);
	patch(0x58E498, &hudTextures[HUD_SiteM16], 4);
	patch(0x58E5AD, &hudTextures[HUD_SiteM16], 4);
	patch(0x58E602, &hudTextures[HUD_SiteM16], 4);
	patch(0x58E657, &hudTextures[HUD_SiteM16], 4);
	patch(0x58E6A4, &hudTextures[HUD_SiteM16], 4);
	patch(0x58E757, &hudTextures[HUD_Siterocket], 4);

	patch(0x58A54D, &hudTextures[HUD_RadarRingPlane], 4);

	patch(0x58A1EF, &hudTextures[HUD_Pager], 4);*/


	// No stream.ini, gta3.ini and clothes stuff
	Nop(0x5BA215, 5);
	Nop(0x5BF865, 5);
	Nop(0x5BF996, 5);
	Nop(0x5A8227, 5);
//	Nop(0x5B8F21, 5);

	// No peds.col
	Nop(0x5BD606, 5);

	// Ped colors!
	InjectHook(0x5E7687, &InjectPedMapping, PATCH_JUMP);
	InjectHook(0x5E7975, 0x4C8460, PATCH_JUMP);
	InjectHook(0x5DDBCE, &PedDataConstructorInject_Civilian, PATCH_JUMP);
	InjectHook(0x5DE39E, &PedDataConstructorInject_Civilian, PATCH_JUMP);
	InjectHook(0x5DDD7F, &PedDataConstructorInject_Cop, PATCH_JUMP);

	// Proper cop models
	Patch<const void*>(0x40E5CE, &CStreaming::ms_bCopBikeAllowed);
	InjectHook(0x407C00, &CStreaming::ChooseCopModel, PATCH_JUMP);
	InjectHook(0x407C50, &CStreaming::ChooseCopCarModel, PATCH_JUMP);
	InjectHook(0x40A150, &CStreaming::StreamCopModels, PATCH_JUMP);

	// Streaming tweaks
	/*static DWORD	dwFakeIntID;
	patch(0x440603, &dwFakeIntID, 4);
	patch(0x44069A, &dwFakeIntID, 4);
	patch(0x4406D3, &dwFakeIntID, 4);*/
//	Patch<BYTE>(0x4406DF, 0xAE);
//	Patch<WORD>(0x, 0xD230);
//	Patch<BYTE>(0x748E69, 0xEB);
	//call(0x748E6B, &TempExitFix, PATCH_NOTHING); // Temporary fix
	InjectHook(0x4083C0, &CdStreamClearNames, PATCH_JUMP);
	Patch<BYTE>(0x53BB60, NUM_IMG_FILES);
	Patch<DWORD>(0x406B75, NUM_STREAMS);
	Patch<BYTE>(0x406815, NUM_STREAMS);
	Patch<DWORD>(0x5B8E55, 21000);
	Patch<DWORD>(0x5B8EB0, 21000);
	Patch<DWORD>(0x5B8E6A, 0x8000000);
	Patch<DWORD>(0x408408, 0x90C35E5F);
	Patch<BYTE>(0x5B619C, 8);
	Patch<DWORD>(0x407667, 0x1824448A);
	Patch<WORD>(0x40766B, 0x8688);
	Patch<const void*>(0x40766D, &CStreaming::ms_cdImages->bEncryptionType);
	Patch<DWORD>(0x407671, 0xC78B5B5E);
	Patch<WORD>(0x407675, 0xC35F);
	InjectHook(0x5B61B0, &IMGEncryptionFindOut, PATCH_JUMP);
	InjectHook(0x5B8E1B, &static_cast<void(*)()>(CStreaming::LoadCdDirectory));
	InjectHook(0x5B61E6, &IMGEncryptionDo, PATCH_JUMP);
	Patch<const void*>(0x407614, &CStreaming::ms_cdImages);
//	patch(0x5B82FD, &CStreaming::ms_cdImages, 4);
	Patch<const void*>(0x407622, &CStreaming::ms_cdImages[NUM_IMG_FILES+1]);
//	patch(0x5B8303, &CStreaming::ms_cdImages[NUM_IMG_FILES+1], 4);
	Patch<const void*>(0x40763A, &CStreaming::ms_cdImages->cName);
//	patch(0x5B82F1, &CStreaming::ms_cdImages->cName, 4);
	Patch<const void*>(0x407656, &CStreaming::ms_cdImages->hHandle);
	Patch<const void*>(0x4075B8, &CStreaming::ms_cdImages->hHandle);
	Patch<const void*>(0x408FDC, &CStreaming::ms_cdImages->hHandle);
	Patch<const void*>(0x409D5A, &CStreaming::ms_cdImages->hHandle);
	Patch<const void*>(0x40A0B7, &CStreaming::ms_cdImages->hHandle);
	Patch<const void*>(0x40CC54, &CStreaming::ms_cdImages->hHandle);
	Patch<const void*>(0x40CCC7, &CStreaming::ms_cdImages->hHandle);
	Patch<const void*>(0x407663, &CStreaming::ms_cdImages->bNotPlayerIMG);
	Patch<const void*>(0x4066D6, gStreamFiles);
	Patch<const void*>(0x4066ED, gStreamFiles);
	Patch<const void*>(0x4067B7, gStreamFiles);
	Patch<const void*>(0x40686A, gStreamFiles);
	Patch<const void*>(0x406A48, gStreamFiles);
	Patch<const void*>(0x406B7C, gStreamFiles);
	Patch<const void*>(0x4067C6, &gStreamFiles[1]);
	Patch<const void*>(0x4067D1, &gStreamFiles[2]);
	Patch<const void*>(0x4067DC, &gStreamFiles[3]);
	Patch<const void*>(0x4067E7, &gStreamFiles[4]);
	Patch<const void*>(0x4067F2, &gStreamFiles[5]);
	Patch<const void*>(0x4067FD, &gStreamFiles[6]);
	Patch<const void*>(0x406808, &gStreamFiles[7]);
	Patch<const void*>(0x4066C7, gStreamNames);
	Patch<const void*>(0x406882, gStreamNames);
	Patch<const void*>(0x406B81, gStreamNames);
	Patch<const void*>(0x406B98, &gStreamNames[NUM_STREAMS+1]);
	Patch<const char*>(0x406C2B, "ANIM\\ANIM.IMG");
	Nop(0x5B927D, 5);
//	Nop(0x43E65D, 2);
//	Nop(0x43E669, 2);
#ifdef INCLUDE_STREAMING_TEXT
	InjectHook(0x40E120, &CStreaming::MakeSpaceFor, PATCH_JUMP);
#endif

	/*To change when increasing streaming upper limit:
	0x5B527A
	0x5545E6 and nearby
	0x555172 and nearby (NOT SURE)
	0x555B1C and nearby (NOT SURE)
	0x71BE74 (NOT SURE)

	Another:
	0x450A60 and nearby
	0x554D8D*/
	Patch<const void*>(0x5B527C, &fNewDrawDistance);
	Patch<const void*>(0x5545E8, &fNewDrawDistance);
	Patch<const void*>(0x554602, &fNewDrawDistance);
	Patch<const void*>(0x55462C, &fNewDrawDistance);
	Patch<float>(0x554D93, MAX_DRAW_DISTANCE);
	Patch<float>(0x555A74, MAX_DRAW_DISTANCE);

	// 12 save slots
	Patch<void*>(0x619166, SlotSaveDate);
	Patch<void*>(0x619191, SlotSaveDate);
	Patch<void*>(0x618D08, SlotSaveDate);
	Patch<void*>(0x619182, &SlotFileName[NUM_SAVE_SLOTS]);
	Patch<void*>(0x61916B, SlotFileName);
	Patch<void*>(0x619189, SlotFileName);
	Patch<void*>(0x5D0F4B, SlotFileName);
	Patch<void*>(0x579BD5, SlotValidation);
	Patch<void*>(0x57B6AD, SlotValidation);
	Patch<void*>(0x5772B3, &SlotValidation[1]);
	Patch<void*>(0x5772D3, &SlotValidation[1]);
	Patch<void*>(0x5797D7, &SlotValidation[1]);
	Patch<void*>(0x619157, &SlotValidation[1]);
	Patch<void*>(0x61922D, &SlotValidation[1]);
	Patch<void*>(0x619247, &SlotValidation[1]);

	Patch<DWORD>(0x61914D, NUM_SAVE_SLOTS);
	Patch<DWORD>(0x57378F, NUM_SAVE_SLOTS+1);
	Patch<BYTE>(0x619388, NUM_SAVE_SLOTS);

	Patch<BYTE>(0x57728F, ACTION_SAVE_12);
	Patch<BYTE>(0x5772FD, ACTION_SAVE_12);
	Patch<BYTE>(0x579921, ACTION_SAVE_12);
	Patch<BYTE>(0x579BB6, ACTION_SAVE_12);
	Patch<BYTE>(0x57A241, ACTION_SAVE_12);
	Patch<BYTE>(0x57B6A7, ACTION_SAVE_12);
	Patch<BYTE>(0x579903, ACTION_MISSIONPACK);
	Patch<BYTE>(0x579AFB, ACTION_MISSIONPACK);
	Patch<BYTE>(0x579CDA, ACTION_MISSIONPACK);
	//Patch<BYTE>(0x579D52, ACTION_JOYMOUSE);
	Patch<BYTE>(0x57B6F5, ACTION_CLICKORARROWS);

	Patch<DWORD>(0x57344F, sizeof(MenuItem));
	Patch<DWORD>(0x5736FD, sizeof(MenuItem));
	Patch<DWORD>(0x573703, sizeof(MenuItem));
	Patch<DWORD>(0x57371B, sizeof(MenuItem));
	Patch<DWORD>(0x573730, sizeof(MenuItem));
	Patch<DWORD>(0x573745, sizeof(MenuItem));
	Patch<DWORD>(0x573768, sizeof(MenuItem));
	Patch<DWORD>(0x573E93, sizeof(MenuItem));
	Patch<DWORD>(0x576B02, sizeof(MenuItem));
	Patch<DWORD>(0x576B18, sizeof(MenuItem));
	Patch<DWORD>(0x576B30, sizeof(MenuItem));
	Patch<DWORD>(0x576B4F, sizeof(MenuItem));
	Patch<DWORD>(0x57700A, sizeof(MenuItem));
	Patch<DWORD>(0x579562, sizeof(MenuItem));
	Patch<DWORD>(0x579678, sizeof(MenuItem));
	Patch<DWORD>(0x5796A9, sizeof(MenuItem));
	Patch<DWORD>(0x57981A, sizeof(MenuItem));
	Patch<DWORD>(0x5798D0, sizeof(MenuItem));
	Patch<DWORD>(0x579AAA, sizeof(MenuItem));
	Patch<DWORD>(0x579AD7, sizeof(MenuItem));
	Patch<DWORD>(0x579B24, sizeof(MenuItem));
	Patch<DWORD>(0x579B52, sizeof(MenuItem));
	Patch<DWORD>(0x579B69, sizeof(MenuItem));
	Patch<DWORD>(0x579C78, sizeof(MenuItem));
	Patch<DWORD>(0x579D14, sizeof(MenuItem));
	Patch<DWORD>(0x579D37, sizeof(MenuItem));
	Patch<DWORD>(0x579D8B, sizeof(MenuItem));
	Patch<DWORD>(0x57A182, sizeof(MenuItem));
	Patch<DWORD>(0x57A22E, sizeof(MenuItem));
	Patch<DWORD>(0x57A397, sizeof(MenuItem));
	Patch<DWORD>(0x57A44D, sizeof(MenuItem));
	Patch<DWORD>(0x57A543, sizeof(MenuItem));
	Patch<DWORD>(0x57A60B, sizeof(MenuItem));
	Patch<DWORD>(0x57A653, sizeof(MenuItem));
	Patch<DWORD>(0x57A69B, sizeof(MenuItem));
	Patch<DWORD>(0x57A6DC, sizeof(MenuItem));
	Patch<DWORD>(0x57A71F, sizeof(MenuItem));
	Patch<DWORD>(0x57A772, sizeof(MenuItem));
	Patch<DWORD>(0x57A7B6, sizeof(MenuItem));
	Patch<DWORD>(0x57B277, sizeof(MenuItem));
	Patch<DWORD>(0x57B4E4, sizeof(MenuItem));
	Patch<DWORD>(0x57B50D, sizeof(MenuItem));
	Patch<DWORD>(0x57B582, sizeof(MenuItem));
	Patch<DWORD>(0x57B5BD, sizeof(MenuItem));
	Patch<DWORD>(0x57B61F, sizeof(MenuItem));
	Patch<DWORD>(0x57B692, sizeof(MenuItem));
	Patch<DWORD>(0x57B6E7, sizeof(MenuItem));
	Patch<DWORD>(0x57C30D, sizeof(MenuItem));
	Patch<DWORD>(0x57CD60, sizeof(MenuItem));
	Patch<DWORD>(0x57D281, sizeof(MenuItem));
	Patch<DWORD>(0x57D2CA, sizeof(MenuItem));
	Patch<DWORD>(0x57D6D3, sizeof(MenuItem));
	Patch<DWORD>(0x57D6FB, sizeof(MenuItem));
	Patch<DWORD>(0x57E3EC, sizeof(MenuItem));
	Patch<DWORD>(0x57FDF9, sizeof(MenuItem));
	Patch<DWORD>(0x57FE4B, sizeof(MenuItem));
	Patch<DWORD>(0x57FE8A, sizeof(MenuItem));
	Patch<DWORD>(0x57FF54, sizeof(MenuItem));
	Patch<DWORD>(0x57FFA4, sizeof(MenuItem));
	Patch<DWORD>(0x58030B, sizeof(MenuItem));
	Patch<DWORD>(0x58048B, sizeof(MenuItem));
	Patch<DWORD>(0x5804E0, sizeof(MenuItem));
	Patch<DWORD>(0x5805C8, sizeof(MenuItem));

	Patch<BYTE>(0x57B29A, NUM_ENTRIES_PER_MENU);
	Patch<BYTE>(0x57FED9, NUM_ENTRIES_PER_MENU);
	Patch<BYTE>(0x57B521, NUM_ENTRIES_PER_MENU);
	Patch<DWORD>(0x57B5D2, NUM_ENTRIES_PER_MENU - 1);

	// Menu mouse hax
	InjectHook(0x57FE6B, &CMenuManager::GetTextYPos, PATCH_NOTHING);
	InjectHook(0x57FEAB, &CMenuManager::GetTextYPosNextItem, PATCH_NOTHING);
	// TODO: Needs memcpy and proper multibyte NOPs
	Patch<DWORD>(0x57FE53, 0x388C8D90);
	Patch<DWORD>(0x57FE92, 0x3A8C8D90);
	Patch<WORD>(0x57FE62, 0x0C89);
	Patch<BYTE>(0x57FE64, 0x24);
	Nop(0x57FE5B, 4);
	Nop(0x57FE65, 6);
	Nop(0x57FE9A, 7);
	Nop(0x57FEA2, 9);

	// No "To stop Carl..." message
	Patch<BYTE>(0x63E8DF, 0xEB);

	// Good Citizen Bonus
	InjectHook(0x68EBDD, &GoodCitizenBonusFlag, PATCH_JUMP);

	// Blur replaced to cam shake
	InjectHook(0x704E8A, &HighspeedCamShake);
	//InjectHook(0x704E8A, &&[](float fShake){ CamShakeNoPos(&TheCamera, fShake * 0.025f); });

	// No Heat Haze
	Patch<BYTE>(0x72C1B7, 0xEB);

	// arrow.dff as marker
	Patch<const float*>(0x725636, C3DMarkers::GetPosZMult());
	Patch<const float*>(0x7259A1, C3DMarkers::GetPosZMult());
	//Patch<const float*>(0x7232C7, C3DMarkers::GetPosZMult());
	Patch<const float*>(0x72564B, C3DMarkers::GetMovingMult());
	Patch<const float*>(0x7259A9, C3DMarkers::GetMovingMult());
	Nop(0x72563A, 6);
	Nop(0x72599F, 6);
	Nop(0x72502B, 6);
	Nop(0x725647, 2);
	InjectHook(0x725037, &InjectArrowMarker);
	Patch<BYTE>(0x726DA6, 5);
	Patch<DWORD>(0x7232C1, 0xC7C6DC);

	// Arrow sizes
	Patch<float>(0x585DA5, 2.5f);	// Vehicles
	Patch<float>(0x585E4D, 1.5f);	// Peds
	Patch<float>(0x585F8E, 1.0f);	// Objects

	// Own BaseColors::BaseColors
	// TODO: Come up with something nicer?
	InjectHook(0x84F1F5, &CRGBA::BaseColors__Constructor, PATCH_NOTHING);

	// Weapon sounds
	// Beretta
	Patch<float>(0x504FB0, 1.0f);
	Patch<BYTE>(0x504FB8, 0xFF);
	Patch<BYTE>(0x504FBE, 0xFF);
	Patch<BYTE>(0x504FC0, 0xFF);

	// Python
	Patch<float>(0x504FD7, 1.0f);
	Patch<BYTE>(0x504FDF, 0xFF);
	Patch<BYTE>(0x504FE1, 0x19);
	Patch<BYTE>(0x504FE3, 0x18);
	Patch<BYTE>(0x504FE5, 0xFF);
	Patch<BYTE>(0x504FE7, 0xFF);


	// Skorpion
	Patch<float>(0x505076, 1.0f);
	Patch<BYTE>(0x505084, 0xFF);
	Patch<BYTE>(0x505086, 0xFF);

	// MP5
	Patch<BYTE>(0x5050AB, 0xFF);
	Patch<BYTE>(0x5050AD, 0xFF);

	// Shotgun, Stubby
	Patch<BYTE>(0x505133, 0xFF);
	Patch<BYTE>(0x505135, 0xFF);
	Patch<const void*>(0x505250, &WeaponSounds_Shotgun);
	Patch<const void*>(0x505254, &WeaponSounds_Stubby);

	// SPAS
	Patch<float>(0x50510D, 0.75f);

	// AK47
	Patch<BYTE>(0x505036, 0xFF);
	Patch<BYTE>(0x505038, 0xFF);

	// M16
	Patch<const void*>(0x505268, &WeaponSounds_M16);

	// M249
	Patch<DWORD>(0x50527C, 0x505220); // Minigun sound
	Patch<BYTE>(0x685B5C, 0xEB); // Crosshair
	Patch<BYTE>(0x58E1BF, 36); // Crosshair
	Patch<BYTE>(0x742A44, 1); // INSTANT_HIT

	// FE_GAME
	InjectHook(0x4DDAE6, &BuySoundCentre, PATCH_JUMP);
	InjectHook(0x4DDD84, &ErrorSoundCentre, PATCH_JUMP);
	Patch<BYTE>(0x4DDAE0, 0x1A);
	Patch<BYTE>(0x4DDD7E, 0x1A);

	// All time blur
	/*Nop(0x704D6C, 2);
	patch(0x704D6F, 0, 4);
	call(0x704D73, 0x7030A0, PATCH_NOTHING);
	blurStages[0].fValueToActivate = 0.0;
	blurStages[0].imageWarp = 1;
	blurStages[0].objectsLooped = 2;*/

	// No csplay loading
//	Patch<BYTE>(0x4D5E93, 0);

	// Radar blip names
	InjectHook(0x5BA2CA, &CRadar::LoadTextures);

	// Global zone name
	strncpy((char*)0x86506C, "VICE_C", 7);

	// Animations
	//charptr(0x8A8B3C, "playersprint_armed");

	// weapon.dat names patches
	//VirtualProtect((char*)0x872CEC, 0x1B8, PAGE_EXECUTE_READWRITE, &dwProtect[0]);
	strncpy((char*)0x872D64, "M249", 16);
	//VirtualProtect((char*)0x872CEC, 0x1B8, dwProtect[0], &dwProtect[1]);

	// Blip textures patches
	//VirtualProtect((char*)0x8665C0, 0x39C, PAGE_EXECUTE_READWRITE, &dwProtect[0]);
	/*charptr(0x8D0898, "radar_JERRY");
	charptr(0x8D0890, "radar_PHIL");
	charptr(0x8D0880, "radar_MARTY");
	charptr(0x8D0878, "radar_LOUISE");
	charptr(0x8D0888, "radar_cshop");
	charptr(0x8D0838, "radar_save");
	charptr(0x8D0758, "radar_bomb");*/
//	strncpy((char*)0x866694, "radar_JERRY", 12);
//	strncpy((char*)0x8666A0, "radar_PHIL", 16);
//	strncpy((char*)0x8666B0, "radar_cshop", 16);
//	strncpy((char*)0x866750, "radar_save", 16);
	//VirtualProtect((char*)0x8665C0, 0x39C, dwProtect[0], &dwProtect[1]);

	// Intro splash screen names patches
	//VirtualProtect((char*)0x866CD8, 0x18, PAGE_EXECUTE_READWRITE, &dwProtect[0]);
	//strncpy((char*)0x866CD8, "title_pc", 12);
	//strncpy((char*)0x866CE8, "intro", 8);
	//VirtualProtect((char*)0x86A470, 0x18, PAGE_EXECUTE_READWRITE, &dwProtect[0]);
	//charptr(0x5901BE, "title_pc");
	//charptr(0x590192, "version");
	//patch(0x5901A6, 0x866CE8, 4);

	// Pager
	//VirtualProtect((char*)0x86A470, 0x18, PAGE_EXECUTE_READWRITE, &dwProtect[0]);
	/*strncpy((char*)0x86A470, "pagerA", 12);
	strncpy((char*)0x86A47C, "pager", 12);
	strncpy((char*)0x86A488, "barOutlineA", 16);
	strncpy((char*)0x86A498, "barOutline", 16);*/
	//VirtualProtect((char*)0x86A470, 0x18, dwProtect[0], &dwProtect[1]);

	// Handling data patches
	//VirtualProtect((char*)0x8D3978, 0xB7C, PAGE_EXECUTE_READWRITE, &dwProtect[0]);
	_patchHandling(HANDLING_COUGAR, "COUGAR");
	_patchHandling(HANDLING_CUBAN, "CUBAN");
	_patchHandling(HANDLING_IDAHO, "IDAHO");
	_patchHandling(HANDLING_PIMP, "PIMP");
	_patchHandling(HANDLING_GANGRAN, "GANGRAN");
	_patchHandling(HANDLING_6ATV, "6ATV");
	_patchHandling(HANDLING_SENTXS, "SENTXS");
	_patchHandling(HANDLING_STINGER, "STINGER");
	_patchHandling(HANDLING_POLICEM, "POLICEM");
	_patchHandling(HANDLING_DELUXO, "DELUXO");
	_patchHandling(HANDLING_DOUBLE, "DOUBLE");
	_patchHandling(HANDLING_ELECTRAP, "ELECTRAP");
	_patchHandling(HANDLING_STREETFI, "STREETFI");
	_patchHandling(HANDLING_ELECTRAG, "ELECTRAG");
	//VirtualProtect((char*)0x8D3978, 0xB7C, dwProtect[0], &dwProtect[1]);
}

__forceinline void PatchMenus()
{
#ifdef LIST_MEMORY
	ListMemory();
#endif
	using namespace Memory;

	Patch<void*>(0x57345A, &MenuEntriesList->entryList->action);
	Patch<void*>(0x57370A, &MenuEntriesList->startingMenuEntry);
	Patch<void*>(0x573713, &MenuEntriesList->prevMenu);
	Patch<void*>(0x573728, &MenuEntriesList->prevMenu);
	Patch<void*>(0x57373D, &MenuEntriesList->prevMenu);
	Patch<void*>(0x573752, &MenuEntriesList->prevMenu);
	Patch<void*>(0x573772, &MenuEntriesList->entryList->targetMenu);
	Patch<void*>(0x573EA9, &MenuEntriesList->entryList->action);
	Patch<void*>(0x576B08, &MenuEntriesList->entryList[1].targetMenu);
	Patch<void*>(0x576B1E, &MenuEntriesList->entryList[1].targetMenu);
	Patch<void*>(0x576B38, MenuEntriesList);
	Patch<void*>(0x576B58, &MenuEntriesList->entryList->entry);
	Patch<void*>(0x577017, &MenuEntriesList->entryList->action);
	Patch<void*>(0x57723D, &MenuEntriesList->entryList->targetMenu);
	Patch<void*>(0x577280, &MenuEntriesList->entryList->specialDescFlag);
	Patch<void*>(0x5772F2, &MenuEntriesList->entryList->specialDescFlag);
	Patch<void*>(0x579568, MenuEntriesList);
	Patch<void*>(0x57967E, MenuEntriesList);
	Patch<void*>(0x5796AF, &MenuEntriesList->entryList->action);
	Patch<void*>(0x57981F, &MenuEntriesList->entryList->entry);
	Patch<void*>(0x5798D6, &MenuEntriesList->entryList->action);
	Patch<void*>(0x5798FC, &MenuEntriesList->entryList->specialDescFlag);
	Patch<void*>(0x579AB2, &MenuEntriesList->entryList->align);
	Patch<void*>(0x579AE4, &MenuEntriesList->entryList->posX);
	Patch<void*>(0x579AEE, &MenuEntriesList->entryList->posY);
	Patch<void*>(0x579AF7, &MenuEntriesList->entryList->specialDescFlag);
	Patch<void*>(0x579B10, &MenuEntriesList->name[4]);
	Patch<void*>(0x579B17, &MenuEntriesList->entryList->posX);
	Patch<void*>(0x579B2E, &MenuEntriesList->name[6]);
	Patch<void*>(0x579B3A, &MenuEntriesList->entryList->posY);
	Patch<void*>(0x579B43, &MenuEntriesList->entryList->posX);
	Patch<void*>(0x579B5A, &MenuEntriesList->entryList->posY);
	Patch<void*>(0x579B70, &MenuEntriesList->entryList->action);
	Patch<void*>(0x579B7A, &MenuEntriesList->entryList->action);
	Patch<void*>(0x579B8B, &MenuEntriesList->entryList->entry);
	Patch<void*>(0x579B9F, &MenuEntriesList->entryList->specialDescFlag);
	//Patch<void*>(0x579BC3, &MenuEntriesList->entryList->posX);
	//Patch<void*>(0x579C83, &MenuEntriesList->entryList->posX);
	Patch<void*>(0x579D20, &MenuEntriesList->entryList->action);
	Patch<void*>(0x579D3F, &MenuEntriesList->name[6]);
	Patch<void*>(0x579D4A, &MenuEntriesList->entryList->posY);
	Patch<void*>(0x579D93, &MenuEntriesList->entryList->action);
	//Patch<void*>(0x57A18A, &MenuEntriesList->entryList->posY);
	//Patch<void*>(0x57A1BD, &MenuEntriesList->entryList->posX);
	Patch<void*>(0x57A235, &MenuEntriesList->entryList->specialDescFlag);
	Patch<void*>(0x57A39F, &MenuEntriesList->entryList->posY);
	Patch<void*>(0x57A455, &MenuEntriesList->entryList->align);
	Patch<void*>(0x57A469, &MenuEntriesList->entryList->posX);
	Patch<void*>(0x57A4B9, &MenuEntriesList->entryList->posX);
	Patch<void*>(0x57A4DA, &MenuEntriesList->entryList->posX);
	Patch<void*>(0x57A54F, &MenuEntriesList->entryList->posY);
	Patch<void*>(0x57A615, &MenuEntriesList->entryList->entry);
	Patch<void*>(0x57A65D, &MenuEntriesList->entryList->entry);
	Patch<void*>(0x57A6A5, &MenuEntriesList->entryList->entry);
	Patch<void*>(0x57A6E6, &MenuEntriesList->entryList->entry);
	Patch<void*>(0x57A729, &MenuEntriesList->entryList->entry);
	Patch<void*>(0x57A77C, &MenuEntriesList->entryList->entry);
	Patch<void*>(0x57A7BE, &MenuEntriesList->entryList->action);
	Patch<void*>(0x57B27E, &MenuEntriesList->entryList->action);
	Patch<void*>(0x57B4F2, &MenuEntriesList->entryList->action);
	Patch<void*>(0x57B519, &MenuEntriesList->entryList->action);
	Patch<void*>(0x57B52A, &MenuEntriesList->entryList->action);
	Patch<void*>(0x57B534, &MenuEntriesList->entryList->action);
	Patch<void*>(0x57B588, &MenuEntriesList->entryList->action);
	Patch<void*>(0x57B5A4, &MenuEntriesList->entryList->action);
	Patch<void*>(0x57B5C9, &MenuEntriesList->entryList->action);
	Patch<void*>(0x57B5E9, &MenuEntriesList->entryList[1].action);
	Patch<void*>(0x57B601, &MenuEntriesList->entryList->action);
	Patch<void*>(0x57B629, &MenuEntriesList->entryList->action);
	Patch<void*>(0x57B69C, &MenuEntriesList->entryList->specialDescFlag);
	Patch<void*>(0x57B6F1, &MenuEntriesList->entryList->specialDescFlag);
	Patch<void*>(0x57C313, &MenuEntriesList->entryList->action);
	Patch<void*>(0x57CD6B, &MenuEntriesList->entryList->action);
	Patch<void*>(0x57D26C, &MenuEntriesList->entryList[2].targetMenu);
	Patch<void*>(0x57D287, &MenuEntriesList->entryList[2].targetMenu);
	Patch<void*>(0x57D2D2, &MenuEntriesList->entryList[2].targetMenu);
	Patch<void*>(0x57D6D8, &MenuEntriesList->entryList->entry);
	Patch<void*>(0x57D701, &MenuEntriesList->entryList->entry);
	Patch<void*>(0x57E3F7, &MenuEntriesList->entryList->action);
	Patch<void*>(0x57FE0A, &MenuEntriesList->entryList->action);
	Patch<void*>(0x57FE25, &MenuEntriesList->entryList->entry);
	//Patch<void*>(0x57FE57, &MenuEntriesList->entryList->posY);
	//Patch<void*>(0x57FE96, &MenuEntriesList->entryList->posY);
	Patch<void*>(0x57FF5F, &MenuEntriesList->entryList->action);
	Patch<void*>(0x57FFAE, &MenuEntriesList->entryList->action);
	Patch<void*>(0x580316, &MenuEntriesList->entryList->action);
	Patch<void*>(0x580496, &MenuEntriesList->entryList->action);
	Patch<void*>(0x5804EB, &MenuEntriesList->entryList->action);
	Patch<void*>(0x5805D3, &MenuEntriesList->entryList->action);

	Patch<void*>(0x57FE57, &MenuEntriesList->entryList);
	Patch<void*>(0x57FE96, &MenuEntriesList->entryList);

	Patch<const char*>(0x57F75D, "FEM_CCR");
	Patch<const char*>(0x57F79C, "FEM_CFT");
	Patch<const char*>(0x57F702, "FEH_SCN");
	Patch<const char*>(0x57F709, "FEH_CCN");
	Patch<const char*>(0x579C56, "FEM_SL%X");

#ifdef DUMP_MENUS
	if ( FILE* hDumpFile = fopen("menu_dump.log", "w") )
	{
		fputs("{ ", hDumpFile);
		for ( int i = 0; i < NUM_MENUS; ++i )
		{
			fprintf(hDumpFile, "\t{ \"%s\", %d, %d,\n", MenuEntriesList[i].name, MenuEntriesList[i].prevMenu, MenuEntriesList[i].startingMenuEntry);
			for ( int j = 0; j < NUM_ENTRIES_PER_MENU; ++j )
			{
				int nPosX = MenuEntriesList[i].entryList[j].posX;
				if ( nPosX )
				{
					if ( !MenuEntriesList[i].entryList[j].screenHorAlign )
					{
						nPosX -= 320;
					}

					if ( MenuEntriesList[i].entryList[j].screenHorAlign == 1 )
					{
						nPosX = 640 - nPosX;
					}
				}

				int nPosY = MenuEntriesList[i].entryList[j].posY;
				if ( nPosY )
				{
					if ( !MenuEntriesList[i].entryList[j].screenVertAlign )
					{
						nPosY -= 224;
					}

					if ( MenuEntriesList[i].entryList[j].screenVertAlign == 1 )
					{
						nPosY = 448 - nPosY;
					}
				}

				int k = MenuEntriesList[i].entryList[j].specialDescFlag;
				static const char*	pDescText[] = { "ACTION_NONE", "ACTION_SAVE_1", "ACTION_SAVE_2", "ACTION_SAVE_3", "ACTION_SAVE_4", "ACTION_SAVE_5", "ACTION_SAVE_6", "ACTION_SAVE_7", "ACTION_SAVE_8",
													"ACTION_MISSIONPACK", "ACTION_JOYMOUSE", "ACTION_STANDARD", "ACTION_CLICKORARROWS", "ACTION_UPDATER" };
				assert(k >= ACTION_NONE && k <= ACTION_UPDATER);

				fprintf(hDumpFile, "\t\t%d, \"%s\", %s, %d, %d, %d, %d, %d, %d%s\n", MenuEntriesList[i].entryList[j].action,  MenuEntriesList[i].entryList[j].entry, pDescText[k], MenuEntriesList[i].entryList[j].targetMenu, nPosX, nPosY, MenuEntriesList[i].entryList[j].align, MenuEntriesList[i].entryList[j].screenVertAlign, MenuEntriesList[i].entryList[j].screenHorAlign, j != 11 ? "," : " },");
			}
			fputs("\n\n", hDumpFile);
		}
		fputs("}", hDumpFile);
		fclose(hDumpFile);
	}
#endif
}

__forceinline void AnimationStylesPatching()
{
	static const char*		player1armedAnims[] = {
									"walk_player", "run_1armed",
									"sprint_civi", "idle_stance",
									"roadcross", "walk_start" };

	using namespace Memory;

	// playersprint_armed
	Patch<const char*>(&animationStyles[60].namesPtr[2], "playersprint_armed");

	// player1armed
	strncpy(animationStyles[118].name, "player1armed", 16);
	strncpy(animationStyles[118].IFPName, "ped", 16);
	animationStyles[118].unk = 7;
	animationStyles[118].animsCount = 6;
	animationStyles[118].namesPtr = player1armedAnims;
	animationStyles[118].unkDataPtr = (CAnimationStyleUnkData*)0x8A7788;

	// player1armedf
	strncpy(animationStyles[119].name, "player1armedf", 16);
	strncpy(animationStyles[119].IFPName, "ped", 16);
	animationStyles[119].unk = 7;
	animationStyles[119].animsCount = 6;
	animationStyles[119].namesPtr = player1armedAnims;
	animationStyles[119].unkDataPtr = (CAnimationStyleUnkData*)0x8A7788;

	// player1armedm
	strncpy(animationStyles[120].name, "player1armedm", 16);
	strncpy(animationStyles[120].IFPName, "ped", 16);
	animationStyles[120].unk = 7;
	animationStyles[120].animsCount = 6;
	animationStyles[120].namesPtr = player1armedAnims;
	animationStyles[120].unkDataPtr = (CAnimationStyleUnkData*)0x8A7788;

	// Driving anims
	Patch<const char*>(0x8D108C, "CAR_sit");
	Patch<const char*>(0x8D10B0, "Drive_L");
	Patch<const char*>(0x8D10B4, "Drive_R");
	Patch<const char*>(0x8D10D0, "CAR_LB");
	Patch<const char*>(0x8D10F4, "Drive_L_slow");
	Patch<const char*>(0x8D10F8, "Drive_R_slow");

	// Checks inject
	InjectHook(0x6099DE, &ExtraWeaponAnimationsInject, PATCH_JUMP);
	Nop(0x6099A5, 2);
	Nop(0x609983, 6);

	// Animations number
	Patch<DWORD>(0x5BF6C7, 121);
}

__forceinline void UserFiles()
{
	using namespace Memory;

	Patch<const char*>(0x4F2FDF, "vcs_utrax.dat");
	Patch<const char*>(0x4F3313, "vcs_utrax.dat");
	Patch<const char*>(0x4F4AE9, "vcs_utrax.dat");
	Patch<const char*>(0x4F3073, "vcs_ufiles.dat");
	Patch<const char*>(0x4F330C, "vcs_ufiles.dat");
	Patch<const char*>(0x4F4A62, "vcs_ufiles.dat");

#ifdef INCLUDE_WINDOW_NAME
	Patch<const char*>(0x619608, "GTA: Vice City Stories");
#endif

	Patch<BYTE>(0x74503A, 0x9);
	Patch<const char*>(0x74503F, "\\GTA Vice City Stories User Files");

#ifdef DEVBUILD
	Patch<const char*>(0x57C672, "gta_vcsd.set");
	Patch<const char*>(0x57C902, "gta_vcsd.set");
	//Patch<const char*>(0x7489A0, "gta_vcsd.set");
#else
	Patch<const char*>(0x57C672, "gta_vcs.set");
	Patch<const char*>(0x57C902, "gta_vcs.set");
	//Patch<const char*>(0x7489A0, "gta_vcs.set");
#endif
	Patch<const char*>(0x619045, "GTAVCSsf");
}

void InjectDelayedPatches()
{
	if ( *(BYTE*)0x6A0050 == 0xE9 )
		CLEOGetTextFunc = (const char*(__thiscall*)(CText*, const char*))(*(int*)0x6A0051 + 0x6A0050 + 5);
	else
		CLEOGetTextFunc = nullptr;

	TheScriptsInitFunc = (void(*)())(*(int*)0x53BDD8 + 0x53BDD7 + 5);

	Memory::InjectHook(0x6A0050, &CText::Get, PATCH_JUMP);

	CUpdateManager::Init();
//	CDLCManager::InitialiseWithUpdater();

	//SpeechInject();

#ifdef MAKE_CONSOLE
	if ( AllocConsole() )
	{
		freopen("CONOUT$", "w", stdout);
        SetConsoleTitle("GTA VCS PC Debug Console");

		/*SMALL_RECT windowPos;
		windowPos.Left = 0;
		windowPos.Top = 1024;
		windowPos.Right = 1024;
		windowPos.Bottom = 1280;

		SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), true, &windowPos); */
		call(0x821982, &printf, PATCH_JUMP);
		LogToFile("Console initialised");
	}
#endif
}

void HighspeedCamShake(float shake)
{
	CamShakeNoPos(&TheCamera, shake * 0.025f);
}

void ViceSquadCheckInjectA(int townID)
{
	CStreaming::StreamCopModels(townID);
	CWanted* pWanted = FindPlayerWanted(-1);
	if ( pWanted->ShouldSendViceSquad() )
	{
		CStreaming::RequestModel(506, 2);
		CStreaming::RequestModel(215, 2);
	}
	else
	{
		CStreaming::SetModelIsDeletable(506);
		if ( CStreaming::ms_aInfoForModel[215].uLoadStatus != StreamingModelLoaded )
			CStreaming::SetModelIsDeletable(215);
	}
}

int ViceSquadCheckInjectB()
{
	CWanted* pWanted = FindPlayerWanted(-1);
	if ( pWanted->ShouldSendViceSquad()
		&& CStreaming::ms_aInfoForModel[215].uLoadStatus == StreamingModelLoaded
		&& CStreaming::ms_aInfoForModel[506].uLoadStatus == StreamingModelLoaded )
	{
		if ( random(0, 3) == 2 )
			LogToFile("Vice Squad sent");
			return 506;
	}
	return 0;
}

void InjectArrowMarker()
{
	int				modelID;
	CBaseModelInfo*	modelInfo = CModelInfo::GetModelInfo("arrow", &modelID);
	CStreaming::RequestModel(modelID, 2);
	CStreaming::LoadAllRequestedModels(false);

	RpAtomic*	atomic = modelInfo->CreateInstance();
	modelInfo->AddRef();

	RwFrame*	frame = RpAtomicGetFrame(atomic);
	RpClump*	clump = RpClumpCreate();
	RwFrameSetIdentity(frame);
	RpClumpAddAtomic(clump, atomic);
	RpClumpSetFrame(clump, frame);
	*arrowClump = clump;
	CTxdStore::PopCurrentTxd();
}

float __stdcall HelperPosXHack(int)
{
	return _x(30.0f);
}

#ifdef INCLUDE_MULTIFONTFILES

const char* GetFontsDATByLanguage()
{
	static const char*	cFontsDATNames[] = { "DATA\\FONTS.DAT", "DATA\\FONTSPL.DAT" };
	return cFontsDATNames[bLastFontsID];
}

const char* GetFontsTXDByLanguage()
{
	bLastFontsID = GetFontsIDByLanguage();

	static const char*	cFontsTXDNames[] = { "MODELS\\FONTS.TXD", "MODELS\\FONTSPL.TXD" };
	return cFontsTXDNames[bLastFontsID];
}

eFontFileIDs GetFontsIDByLanguage()
{
	switch ( FrontEndMenuManager.GetLanguage() )
	{
	case LANGUAGE_Polish:
		return FONTF_Polish;
	default:
		return FONTF_European;
	}
}

#endif

void InitialiseLanguage()
{
	FrontEndMenuManager.SetTitleLanguage(0);

	switch ( reinterpret_cast<int>(GetKeyboardLayout(0)) & 0x3FF )
	{
	case LANG_GERMAN:
	case LANG_HUNGARIAN:
		FrontEndMenuManager.SetKeyboardLayout(KEYBOARD_QWERTZ);
		break;
	case LANG_FRENCH:
		FrontEndMenuManager.SetKeyboardLayout(KEYBOARD_AZERTY);
		break;
	default:
		FrontEndMenuManager.SetKeyboardLayout(KEYBOARD_QWERTY);
		break;
	}

	LCID	LanguageCode = GetUserDefaultLCID();

	switch ( LCID_PRIMARY_LANG(LanguageCode) )
	{
	case LANG_GERMAN:
		FrontEndMenuManager.SetLanguage(LCID_SUBLANG(LanguageCode) == SUBLANG_GERMAN || LCID_SUBLANG(LanguageCode) == SUBLANG_GERMAN_AUSTRIAN || LCID_SUBLANG(LanguageCode) == SUBLANG_GERMAN_LIECHTENSTEIN ? LANGUAGE_German : LANGUAGE_English);
		break;
	//case LANG_SPANISH:
	//	FrontEndMenuManager.SetLanguage(LANGUAGE_Spanish);
	//	break;
	case LANG_POLISH:
		FrontEndMenuManager.SetLanguage(LANGUAGE_Polish);
		break;
	case LANG_PORTUGUESE:
		FrontEndMenuManager.SetLanguage(LCID_SUBLANG(LanguageCode) == SUBLANG_PORTUGUESE_BRAZILIAN ? LANGUAGE_Brazilian : LANGUAGE_English);
		break;
	case LANG_HUNGARIAN:
		FrontEndMenuManager.SetLanguage(LANGUAGE_Hungarian);
		break;
	case LANG_ROMANIAN:
		FrontEndMenuManager.SetLanguage(LANGUAGE_Romanian);
		break;
	default:
		FrontEndMenuManager.SetLanguage(LANGUAGE_English);
		break;
	}
	TheText.Load(false);
}

void LoadGameFailedMessage(unsigned char bMessageIndex)
{
	const char*		pMessage;

	switch ( bMessageIndex )
	{
	case 1:
		pMessage = "FES_LOC";
		break;
	case 2:
		pMessage = "FES_SFB";
		break;
	case 3:
		pMessage = "FES_SFN";
		break;
	default:
		pMessage = "FED_LFL";
		break;
	}

	for ( ;; )
	{
		MessageLoop();
		CPad::UpdatePads();
		FrontEndMenuManager.MessageScreen(pMessage, true, false);

		if ( ( currKeyState->enter && !prevKeyState->enter ) || ( currKeyState->extenter && !prevKeyState->extenter )
			|| CPad::GetPad(0)->CrossJustDown() )
			return;
	}
}

void MessageLoop()
{
	MSG Msg;

	while ( PeekMessage(&Msg, 0, 0, 0, PM_NOYIELD | PM_REMOVE) )
	{
		if ( Msg.message == WM_QUIT )
			RsGlobal.quit = TRUE;
		else
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
	}
}

void CdStreamClearNames()
{
	for ( int i = 0; i < NUM_IMG_FILES; ++i )
	{
		CStreaming::ms_cdImages[i].cName[0] = '\0';
		CStreaming::ms_cdImages[i].hHandle = nullptr;
	}
}

static bool		bNoTimeFix = false;

void ParseCommandlineFile()
{
	if ( FILE* hFile = fopen("commandline.txt", "r") )
	{
		char	cLine[1024];
		fgets(cLine, 1024, hFile);

		if ( cLine[0] )
		{
			ParseCommandlineArgument(strtok(cLine, " "));

			while ( char* pNextArg = strtok(nullptr, " ") )
				pNextArg = ParseCommandlineArgument(pNextArg);
		}

		fclose(hFile);
	}
}

char* ParseCommandlineArgument(char* pArg)
{
	if ( pArg )
	{
		if ( !_strnicmp(pArg, "-novcsname", 10) )
		{
			SetWindowTextW(RsGlobal.ps->window, L"GTA: San Andreas");
			RsGlobal.AppName = "GTA: San Andreas";
			return pArg;
		}

		if ( !_strnicmp(pArg, "-nointro", 8) )
		{
			// TODO: Define this variable properly
			*(DWORD*)0xC8D4C0 = 5;
			return pArg;
		}

		if ( !_strnicmp(pArg, "-notimefix", 10) )
		{
			bNoTimeFix = true;
			return pArg;
		}


		if ( !_strnicmp(pArg, "-nopostfx", 9) )
		{
			// TODO: Define this variable properly
			*(bool*)0xC402CF = true;
			return pArg;
		}

		if ( !_strnicmp(pArg, "-pedshadowquality", 18) )
		{
			pArg = strtok(nullptr, " ");
			int		nNewShadowQuality = atoi(pArg);

			if ( nNewShadowQuality >= 1 && nNewShadowQuality <= 4 )
			{
				Memory::Patch<BYTE>(0x706825, static_cast<BYTE>(nNewShadowQuality + 5));
				Memory::Patch<BYTE>(0x706832, static_cast<BYTE>(nNewShadowQuality + 5));
				Memory::Patch<BYTE>(0x7064C2, static_cast<BYTE>(nNewShadowQuality + 6));
				Memory::Patch<BYTE>(0x7064F9, static_cast<BYTE>(nNewShadowQuality + 5));
			}
			return pArg;
		}

#ifdef DEVBUILD
		if ( !_strnicmp(pArg, "-noautocheck", 13) )
		{
			CUpdateManager::DisableAutoCheck();
			return pArg;
		}

		if ( !_strnicmp(pArg, "-zombiedlc", 10) )
		{
			CDLCManager::ToggleDebugOverride(DLC_HALLOWEEN);
			return pArg;
		}

		/*if ( !_strnicmp(pArg, "-2dfx", 5) )
		{
			CDLCManager::ToggleDebugOverride(DLC_2DFX);
			return;
		}*/
#endif
	}
	return pArg;
}

BOOL IsAlreadyRunning()
{
	CreateEventW(nullptr, FALSE, TRUE, L"Grand theft auto San Andreas");

	if ( GetLastError() == ERROR_ALREADY_EXISTS )
	{
		// TODO: Make RsGlobal.AppName wide?
		HWND	hGameWindow = FindWindowA("Grand theft auto San Andreas", RsGlobal.AppName);
		if ( hGameWindow )
			SetForegroundWindow(hGameWindow);
		else
		{
			if ( RsGlobal.ps )
			{
				if ( RsGlobal.ps->window )
					SetForegroundWindow(RsGlobal.ps->window);
			}
		}

		// No point in hooking if we're about to terminate this process! ;)
		return TRUE;
	}

	InjectDelayedPatches();
	return FALSE;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch ( uMsg )
	{
	case WM_CHAR:
		FrontEndMenuManager.TypingKeyboardInput(wParam);
		return 0;
	default:
		return ((LRESULT(CALLBACK*)(HWND,UINT,WPARAM,LPARAM))0x747EB0)(hwnd, uMsg, wParam, lParam);
	}
}

void SaveLanguageHack(FILE* stream, const char* ptr, size_t len)
{
	UNREFERENCED_PARAMETER(len);

	((void(*)(const void*, size_t, size_t, FILE*))0x823674)(CText::GetLanguageAcronymByIndex(*ptr), 1, 2, stream);
}

void LoadLanguageHack(FILE* stream, void* buf, size_t len)
{
	UNREFERENCED_PARAMETER(len);

	// fread from an old CRT
	char		LangName[2];

	((void(*)(void*, size_t, size_t, FILE*))0x823521)(LangName, 1, sizeof(LangName), stream);

	*static_cast<unsigned char*>(buf) = CText::GetLanguageIndexByAcronym(LangName);
}

CVehicle* __fastcall VehiclePoolGetAt(CVehiclePool* pThis, int unused, int nIdentifier)
{
	UNREFERENCED_PARAMETER(unused);
	return pThis->GetAt(nIdentifier);
}

CPed* __fastcall PedPoolGetAt(CPedPool* pThis, int unused, int nIdentifier)
{
	UNREFERENCED_PARAMETER(unused);
	return pThis->GetAt(nIdentifier);
}

#if defined DEVBUILD && !defined MAKE_ZZCOOL_MOVIE_DEMO

void EnterAmazingScreenshotMode(bool bEnable)
{
	//DWORD dwProtect[2];
	//VirtualProtect((void*)0x401000, 0x4A2400, PAGE_EXECUTE_READWRITE, &dwProtect[0]);

	if ( bEnable )
	{
		Memory::Nop(0x53BF8A, 2);
		Memory::Nop(0x53BF96, 6);
		Memory::Nop(0x58FCC2, 4);
		Memory::Nop(0x58D4BE, 8);
		Memory::Patch<BYTE>(0x561B06, 1);
		Memory::Patch<BYTE>(0xB7CB49, 1);
	}
	else
	{
		Memory::Patch<WORD>(0x53BF8A, 0xC084);
		Memory::Patch<DWORD>(0x53BF96, 0x0298850F);
		Memory::Patch<DWORD>(0x58FCC2, 0x4D75C084);
		Memory::Patch<DWORD>(0x58D4BE, 0x850FC084);
		Memory::Patch<DWORD>(0x58D4C2, 0x000000AF);
		Memory::Patch<WORD>(0x53BF9A, 0);
		Memory::Patch<BYTE>(0x561B06, 0);
		Memory::Patch<BYTE>(0xB7CB49, 0);
	}
	//VirtualProtect((void*)0x401000, 0x4A2400, dwProtect[0], &dwProtect[1]);

	InAmazingScreenshotMode = bEnable;
}

/*void LoadDevLogos()
{
	static const char*			devLogosTexNames[] = {
									"vcspc", ""/*,
									"rpl", "" };*/
/*	replacedTXDLoadFunc();

	int slot = CTxdStore::AddTxdSlot("logos");
	CTxdStore::LoadTxd(slot, "MODELS\\LOGOS.TXD");
	CTxdStore::AddRef(slot);
	CTxdStore::PushCurrentTxd();
	CTxdStore::SetCurrentTxd(slot);

	CSprite2d*		texturesToLoad = DevLogos;
	const char**	namesToLoad = devLogosTexNames;

	do
	{
		texturesToLoad->Load(namesToLoad[0], namesToLoad[1]);
		++texturesToLoad;
		namesToLoad += 2;
	}
	while ( pointerDidNotReach(namesToLoad, devLogosTexNames) );
	CTxdStore::PopCurrentTxd();

	LogToFile("Dev logos loaded");
}

void ReleaseDevLogos()
{
	replacedTXDReleaseFunc();

	CSprite2d*	texturesToUnload = DevLogos;

	do
	{
		texturesToUnload->Clean();
		++texturesToUnload;
	}
	while ( pointerDidNotReach( texturesToUnload, DevLogos) );

	CTxdStore::RemoveTxdSlot(CTxdStore::FindTxdSlot("logos"));

	LogToFile("Dev logos released");
}*/

#endif



void __declspec(naked) ViceSquadCheckInjectC()
{
	_asm
	{
		call	ViceSquadCheckInjectB
		test	eax, eax
		jz		ViceSquadCheckInjectC__NOMODEL
		pop		ebx
		retn
ViceSquadCheckInjectC__NOMODEL:
		push	-1
		call	FindPlayerWanted
	//	add		esp, 4
		jmp		ViceSquadCheckInjectC_JumpBack
	}
}



void __declspec(naked) EnexMarkersColorBreak()
{
	_asm
	{
		push	96h
		push	64h
		push	ebx
		//push	00h
		jmp		_ProcessEnexMarkers_JumpBack
	}
}

void __declspec(naked) GarageCapacityA()
{
	_asm
	{
		cmp		cl, 10h
		jz		GarageCapacityAOne
		mov		al, 4
		jmp		GarageCapacityAReturn
GarageCapacityAOne:
		mov		al, 1
GarageCapacityAReturn:
		push	ebx
		push	0FFFFFFFFh
		mov		edi, 0x44BD9A
		jmp		edi
	}
}

void __declspec(naked) GarageCapacityB()
{
	_asm
	{
		cmp		al, 10h
		jz		GarageCapacityBOne
		mov		cl, 4
		jmp		GarageCapacityBReturn
GarageCapacityBOne:
		mov		cl, 1
GarageCapacityBReturn:
		push	eax
		mov		edi, 0x44BF25
		jmp		edi
	}
}

/*void __declspec(naked) FlamingStunt()
{
	_asm
	{
		cmp		esi, ebp
		jnz		FlamingStuntGoBack
	}
	tempString[0] = 0;
	_asm
	{
		push	offset aFlinst
		mov		eax, 0x55B68F
		jmp		eax

FlamingStuntGoBack:
		mov		eax, 0x55B5A5
		jmp		eax
	}
}

void __declspec(naked) DoubleFlamingStunt()
{
	_asm
	{
		cmp		esi, ebp
		jnz		DoubleFlamingStuntGoBack
	}
	tempString[0] = 0;
	_asm
	{
		push	offset aDbfins
		mov		eax, 0x55B68F
		jmp		eax

DoubleFlamingStuntGoBack:
		mov		eax, 0x55B5A5
		jmp		eax
	}
}

void __declspec(naked) TripleFlamingStunt()
{
	_asm
	{
		cmp		esi, ebp
		jnz		TripleFlamingStuntGoBack
	}
	tempString[0] = 0;
	_asm
	{
		push	offset aFltrst
		mov		eax, 0x55B68F
		jmp		eax

TripleFlamingStuntGoBack:
		mov		eax, 0x55B5A5
		jmp		eax
	}
}

void __declspec(naked) QuadrupleFlamingStunt()
{
	_asm
	{
		cmp		esi, ebp
		jnz		QuadrupleFlamingStuntGoBack
	}
	tempString[0] = 0;
	_asm
	{
		push	offset aFquins
		mov		eax, 0x55B68F
		jmp		eax

QuadrupleFlamingStuntGoBack:
		mov		eax, 0x55B5A5
		jmp		eax
	}
}*/

void __declspec(naked) func_0050()
{
	_asm
	{
		mov		eax, [esp+28h+4h]
		push	eax
		mov		ecx, esi
		call	CRunningScript::ProcessVCSCommands
		xor		al, al
		pop		edi
		pop		esi
		mov		ecx, [esp+20h-0Ch]
		mov		fs:0, ecx
		add		esp, 20h
		retn	4
	}
}

void __declspec(naked) func_0166()
{
	_asm
	{
		mov		eax, [esp+84h+4h]
		push	eax
		mov		ecx, esi
		call	CRunningScript::ProcessVCSCommands
		xor     al, al
		pop     edi
		pop     esi
		add     esp, 7Ch
		retn    4
	}
}

void __declspec(naked) func_01C6()
{
	_asm
	{
		push	1
		mov		ecx, esi
		call	CRunningScript::CollectParameters
		push	1
		jmp		func_01C6_JumpBack
	}
}

void __declspec(naked) func_0352()
{
	_asm
	{
		mov		eax, [esp+208h+4h]
		push	eax
		mov		ecx, esi
		call	CRunningScript::ProcessVCSCommands
		xor		al, al
		mov     ecx, [esp+208h-0Ch]
		pop     edi
		pop     esi
		pop     ebp
		mov     fs:0, ecx
		pop     ebx
		add     esp, 1F8h
		retn    4
	}
}

void __declspec(naked) func_03DF()
{
	_asm
	{
		mov		eax, [esp+168h+4h]
		push	eax
		mov		ecx, esi
		call	CRunningScript::ProcessVCSCommands
		xor		al, al
        mov     ecx, [esp+168h-0Ch]
        pop     edi
        pop     esi
        pop     ebp
        mov     fs:0, ecx
        pop     ebx
		add     esp, 158h
		retn    4
//		retn	15Ch

	}
}

void __declspec(naked) func_052E()
{
	_asm
	{
		//xor		ebx, ebx
        //mov		ecx, [esi+0x14]
        //mov		bx, [ecx-2]
		//push	ebx
		mov		eax, [esp+78h+4h]
		push	eax
		mov		ecx, esi
		call	CRunningScript::ProcessVCSCommands
		xor		al, al
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		add     esp, 68h
		retn    4
	}
}

void __declspec(naked) func_0821()
{
	_asm
	{
		push	ebp
		call	CRunningScript::ProcessVCSCommands
		xor		al, al
		mov		ecx, [esp+1B4h-0Ch]
		pop		edi
		pop		esi
		pop		ebp
		mov     fs:0, ecx
		pop     ebx
		add		esp, 1A4h
		retn	4
	}
}

void __declspec(naked) func_0602()
{
	_asm
	{
		cmp		ax, VT_CABBIE
		jz		func_0602_ReturnTrue
		cmp		ax, VT_ZEBRA
		jnz		func_0602_ReturnFalse

func_0602_ReturnTrue:
		jmp		func_0602_JumpBack

func_0602_ReturnFalse:
		jmp		func_0602_ElseJump
	}
}

void __declspec(naked) func_069C()
{
	_asm
	{
		push	ebp
		call	CRunningScript::ProcessVCSCommands
		xor		al, al
		mov     ecx, [esp+0F8h-0Ch]
		pop     edi
		pop     esi
		pop     ebp
		mov     fs:0, ecx
		pop     ebx
		add     esp, 0E8h
		retn    4
	}
}

void __declspec(naked) func_06BA()
{
	_asm
	{
		push	ebp
		call	CRunningScript::ProcessVCSCommands
		xor		al, al
		mov     ecx, [esp+1D8h-0Ch]
		pop     edi
		pop     esi
		pop     ebp
		mov     fs:0, ecx
		pop     ebx
		add     esp, 1C8h
		retn    4
	}
}

void __declspec(naked) ZebraCabNitro()
{
	_asm
	{
		cmp		ax, VT_CABBIE
		jz		TaxiCabNitro_ReturnTrue
		cmp		ax, VT_ZEBRA
		jnz		TaxiCabNitro_ReturnFalse

TaxiCabNitro_ReturnTrue:
		jmp		TaxiCabNitro_JumpBack

TaxiCabNitro_ReturnFalse:
		jmp		TaxiCabNitro_ElseJump
	}
}

void __declspec(naked) ZebraCabCPlayerStuff()
{
	_asm
	{
		cmp		dx, VT_CABBIE
		jz		ZebraCabCPlayerStuff_ReturnTrue
		cmp		dx, VT_ZEBRA
		jnz		ZebraCabCPlayerStuff_ReturnFalse

ZebraCabCPlayerStuff_ReturnTrue:
		jmp		ZebraCabCPlayerStuff_JumpBack

ZebraCabCPlayerStuff_ReturnFalse:
		jmp		ZebraCabCPlayerStuff_ElseJump
	}
}

void __declspec(naked) func_610310()
{
	_asm
	{
		cmp		eax, VT_CABBIE
		jz		func_610310_ReturnTrue
		cmp		eax, VT_ZEBRA
		jnz		func_610310_ReturnFalse

func_610310_ReturnTrue:
		jmp		func_610310_JumpBack

func_610310_ReturnFalse:
		jmp		func_610310_ElseJump
	}
}

void __declspec(naked) func_6B4800()
{
	_asm
	{
		cmp		ax, VT_CABBIE
		jz		func_6B4800_ReturnFalse
		cmp		ax, VT_ZEBRA
		jz		func_6B4800_ReturnFalse
		jmp		func_6B4800_JumpBack

func_6B4800_ReturnFalse:
		jmp		func_6B4800_ElseJump
	}
}

void __declspec(naked) func_6D1AA0()
{
	_asm
	{
		cmp		ax, VT_CABBIE
		jz		func_6D1AA0_ReturnTrue
		cmp		ax, VT_ZEBRA
		jnz		func_6D1AA0_ReturnFalse

func_6D1AA0_ReturnTrue:
		jmp		func_6D1AA0_JumpBack

func_6D1AA0_ReturnFalse:
		jmp		func_6D1AA0_ElseJump
	}
}

void __declspec(naked) GoodCitizenBonusFlag()
{
	_asm
	{
		or		byte ptr [ebx+0x46C], 0x800000
		jz		GoodCitizenBonusFlagJumpBack
		mov		ecx, 0x68EBE3
		jmp		ecx

GoodCitizenBonusFlagJumpBack:
		mov		ecx, 0x68F0A9
		jmp		ecx
	}
}

void __declspec(naked) InCarKilledCounterBreak()
{
	_asm
	{
		//mov		edx, [esp+4+14h]
		mov		eax, [esp+4+10h]
		//push	edx
		push	eax
		push	edi
		call	CDarkel::RegisterKillByPlayerVehicle
		add		esp, 8
		pop		esi
		pop		edi
		retn
	}
}

void __declspec(naked) Language_CASE_English()
{
	_asm
	{
		push	offset aEnglish_gxt
		mov		edx, 6A020Eh
		jmp		edx
	}
}

void __declspec(naked) Language_CASE_German()
{
	_asm
	{
		push	offset aGerman_gxt
		mov		edx, 6A020Eh
		jmp		edx
	}
}

void __declspec(naked) Language_CASE_Spanish()
{
	_asm
	{
		push	offset aSpanish_gxt
		mov		edx, 6A020Eh
		jmp		edx
	}
}

void __declspec(naked) Language_CASE_Brazilian()
{
	_asm
	{
		push	offset aBrazilian_gxt
		mov		edx, 6A020Eh
		jmp		edx
	}
}

void __declspec(naked) Language_CASE_Polish()
{
	_asm
	{
		push	offset aPolish_gxt
		mov		edx, 6A020Eh
		jmp		edx
	}
}

void __declspec(naked) Language_CASE_Hungarian()
{
	_asm
	{
		push	offset aHungarian_gxt
		mov		edx, 6A020Eh
		jmp		edx
	}
}

void __declspec(naked) Language_CASE_Romanian()
{
	_asm
	{
		push	offset aRomanian_gxt
		mov		edx, 6A020Eh
		jmp		edx
	}
}

void __declspec(naked) MissionLanguage_CASE_English()
{
	_asm
	{
		push	offset aEnglish_gxt
		mov		eax, 69FD31h
		jmp		eax
	}
}

void __declspec(naked) MissionLanguage_CASE_German()
{
	_asm
	{
		push	offset aGerman_gxt
		mov		eax, 69FD31h
		jmp		eax
	}
}

void __declspec(naked) MissionLanguage_CASE_Spanish()
{
	_asm
	{
		push	offset aSpanish_gxt
		mov		eax, 69FD31h
		jmp		eax
	}
}

void __declspec(naked) MissionLanguage_CASE_Brazilian()
{
	_asm
	{
		push	offset aBrazilian_gxt
		mov		eax, 69FD31h
		jmp		eax
	}
}

void __declspec(naked) MissionLanguage_CASE_Polish()
{
	_asm
	{
		push	offset aPolish_gxt
		mov		eax, 69FD31h
		jmp		eax
	}
}

void __declspec(naked) MissionLanguage_CASE_Hungarian()
{
	_asm
	{
		push	offset aHungarian_gxt
		mov		eax, 69FD31h
		jmp		eax
	}
}

void __declspec(naked) MissionLanguage_CASE_Romanian()
{
	_asm
	{
		push	offset aRomanian_gxt
		mov		eax, 69FD31h
		jmp		eax
	}
}

void __declspec(naked) CGameLogic__Update_Busted()
{
	_asm
	{
		call	CPlayerInfo::ArrestPlayer
		push	esi
		mov		ecx, offset weaponsToReturn
		call	CConfiscatedWeapons::SaveConfiscatedWeapons
		retn
	}
}

void __declspec(naked) CGameLogic__Update_Wasted()
{
	_asm
	{
		call	CPlayerInfo::KillPlayer
		push	esi
		mov		ecx, offset weaponsToReturn
		call	CConfiscatedWeapons::SaveConfiscatedWeapons
		retn
	}
}

void __declspec(naked) AssignViceSquadToVehicle()
{
	_asm
	{
		mov		[esp+170h+158h], 3
		mov		[esp+170h+15Dh], 1
		jmp		AssignCopToVehicleSwitch_JumpBack
	}
}

void __declspec(naked) Clock_SwitchInject()
{
	_asm
	{
		cmp		al, 2
		jnl		Clock_SwitchInject_ZeroTheValue
		inc		al
		jmp		Clock_SwitchInject_GoBack

Clock_SwitchInject_ZeroTheValue:
		xor		al, al

Clock_SwitchInject_GoBack:
		jmp		Clock_SwitchInject_JumpBack
	}
}

void __declspec(naked) Clock_StringInject()
{
	_asm
	{
		mov		ecx, [TheText]
		cmp		al, 1
		jnz		Clock_StringInject_Compare12H
		push	offset aFem_24H
		jmp		Clock_StringInject_GoBack

Clock_StringInject_Compare12H:
		cmp		al, 2
		jnz		Clock_StringInject_DisplayOFF
		push	offset aFem_12H

Clock_StringInject_GoBack:
		jmp		Clock_StringInject_JumpBack

Clock_StringInject_DisplayOFF:
		jmp		Clock_StringInject_JumpToOFF
	}
}

void __declspec(naked) RadioNameInject()
{
	_asm
	{
		push	eax
		push	esi
		call	CHud::DrawRadioName
		add		esp, 0Ch
		pop		edi
		pop		esi
		retn
	}
}

void __declspec(naked) RunningScriptPointerParamWrapper()
{
	_asm
	{
		push	eax
		mov		eax, 464790h
		call	eax		// CRunningScript::GetPointerToScriptVariable
		retn
	}
}

/*void __declspec(naked) OnscreenCounterColour()
{
	_asm
	{
		mov		byte ptr [esi+3Fh], 1
		test	ax, ax
		jz		OnscreenCounterColour_Pink
		mov     byte ptr [esi+40h], 7
		jmp		OnscreenCounterColour_Return

OnscreenCounterColour_Pink:
		mov     byte ptr [esi+40h], 8

OnscreenCounterColour_Return:
		pop		esi
		retn	10h
	}
}


void __declspec(naked) TwoNumberCounterColour()
{
	_asm
	{
		mov		[esi+40h], 8
		pop		esi
		retn	10h
	}
}*/

void __declspec(naked) ScriptCheckpointsColours_Normal()
{
	_asm
	{
		mov		byte ptr [esp+30h+10h], 0EDh
		mov		byte ptr [esp+30h+0Ch], 82h
		mov		byte ptr [esp+30h+8h], 0B4h
		mov		byte ptr [esp+30h+4h], 40h
		jmp		ScriptCheckpointsColours_JumpBack
	}
}

void __declspec(naked) ScriptCheckpointsColours_Others()
{
	_asm
	{
		mov		byte ptr [esp+30h+10h], 0EDh
		mov		byte ptr [esp+30h+0Ch], 82h
		mov		byte ptr [esp+30h+8h], 0B4h
		mov		byte ptr [esp+30h+4h], 80h
		jmp		ScriptCheckpointsColours_JumpBack
	}
}

void __declspec(naked) ScriptCheckpointsColours_Finish()
{
	_asm
	{
		mov		byte ptr [esp+30h+10h], bl
		mov		byte ptr [esp+30h+0Ch], 0FFh
		mov		byte ptr [esp+30h+8h], bl
		mov		byte ptr [esp+30h+4h], 40h
		jmp		ScriptCheckpointsColours_JumpBack
	}
}

void __declspec(naked) BigMessage1Inject()
{
	_asm
	{
		call	CHud::DrawBigMessage1
		sub		esp, 0Ch
		jmp		BigMessage1Inject_JumpBack
	}
}

void __declspec(naked) BigMessage2Inject()
{
	_asm
	{
		call	CHud::DrawBigMessage2
		pop		ecx
		retn
	}
}

void __declspec(naked) BigMessage3Inject()
{
	_asm
	{
		call	CHud::DrawBigMessage3
		pop		ecx
		retn
	}
}

void __declspec(naked) BigMessage6Inject()
{
	_asm
	{
		retn
	}
}

void __declspec(naked) ExtraWeaponAnimationsInject()
{
	_asm
	{
		cmp		edi, WEAPONTYPE_STUBBY
		jz		ExtraWeaponAnimationsInject_2HandWeap
		cmp		edi, WEAPONTYPE_M249
		jz		ExtraWeaponAnimationsInject_2HandWeap
		cmp		edi, WEAPONTYPE_BASEBALLBAT
		jz		ExtraWeaponAnimationsInject_1HandWeap
		cmp		edi, WEAPONTYPE_KATANA
		jz		ExtraWeaponAnimationsInject_1HandWeap
		cmp		edi, WEAPONTYPE_PISTOL
		jz		ExtraWeaponAnimationsInject_1HandWeap
		cmp		edi, WEAPONTYPE_UZI
		jz		ExtraWeaponAnimationsInject_1HandWeap
		cmp		edi, WEAPONTYPE_PYTHON
		jz		ExtraWeaponAnimationsInject_1HandWeap
		cmp		edi, WEAPONTYPE_INGRAMSL
		jz		ExtraWeaponAnimationsInject_1HandWeap
		cmp		edi, WEAPONTYPE_MP5
		jz		ExtraWeaponAnimationsInject_1HandWeap
		cmp		edi, WEAPONTYPE_SKORPION
		jz		ExtraWeaponAnimationsInject_1HandWeap
		mov		eax, [esi+480h]
		jmp		ExtraWeaponAnimationsInject_NoArmed
		//push	edi
		//call	CAnimationStyleDescriptor::DoesWeaponUse1ArmedAnim
		//add		esp, 4
		//test	al, al
		//jz		ExtraWeaponAnimationsInject_ReturnFalse

ExtraWeaponAnimationsInject_1HandWeap:
		mov		edi, 76h
		jmp		ExtraWeaponAnimationsInject_JumpBack

ExtraWeaponAnimationsInject_2HandWeap:
		mov		edi, 3Ch
		jmp		ExtraWeaponAnimationsInject_JumpBack
	}
}

void __declspec(naked) WeaponSounds_Shotgun()
{
	_asm
	{
		mov		edx, [esp+4+0Ch]
		push	3F800000h	// 1.0
		push	3F800000h	// 1.0
		push	0
		push	edx
		push	30
		push	29
		push	-1
		jmp		WeaponSounds_JumpBack
	}
}

void __declspec(naked) WeaponSounds_Stubby()
{
	_asm
	{
		mov		edx, [esp+4+0Ch]
		push	3F800000h	// 1.0
		push	3F800000h	// 1.0
		push	0
		push	edx
		push	23
		push	22
		push	21
		jmp		WeaponSounds_JumpBack
	}
}

void __declspec(naked) WeaponSounds_M16()
{
	_asm
	{
		mov		edx, [esp+4+0Ch]
		push	3F800000h	// 1.0
		push	3FD33333h	// 1.65
		push	0
		push	edx
		push	5
		push	4
		push	3
		jmp		WeaponSounds_JumpBack
	}
}

void __declspec(naked) ATVPreRenderHack()
{
	_asm
	{
		mov		eax, 969152h
		mov		al, [eax]
		test	al, al
		jnz		ATVPreRenderHack_ReturnTrue
		cmp		word ptr [esi+22h],	VT_6ATV
		jz		ATVPreRenderHack_ReturnTrue
		mov		eax, 6AAB89h
		jmp		eax

ATVPreRenderHack_ReturnTrue:
		mov		eax, 6AAB7Fh
		jmp		eax
	}
}

void __declspec(naked) InjectPedMapping()
{
	_asm
	{
		call	CPed::Remap
		mov     eax, [ebp+598h]
		jmp		InjectPedMapping_JumpBack
	}
}

void __declspec(naked) PedDataConstructorInject_Civilian()
{
	_asm
	{
		mov		edx, [esp+8]
		push	edx
		push	eax
		call	CPedData::Initialise
		add		esp, 8
		retn	8
	}
}

void __declspec(naked) PedDataConstructorInject_Cop()
{
	_asm
	{
		push	0FFFFh
		push	eax
		call	CPedData::Initialise
		add		esp, 18h
		retn	4
	}
}

void __declspec(naked) LightsOnFix()
{
	_asm
	{


	}
}

void __declspec(naked) NodeCrashFix()
{
	_asm
	{
		test	ecx, ecx
		jnz		NodeCrashFix_ReturnToCode
		retn	8

NodeCrashFix_ReturnToCode:
		sub     esp, 10h
		push	esi
		mov     esi, ecx
		jmp		NodeCrashFix_JumpBack
	}
}


void __declspec(naked) NodeCrashFix2()
{
#ifdef DEVBUILD
	static int	nRequest;
#endif
	_asm
	{
#ifdef DEVBUILD
		mov		nRequest, edi
		pushad
	}
	assert(nRequest);
	_asm
	{
		popad
#endif
		test	edi, edi
		jnz		NodeCrashFix2_ReturnToCode
		pop		edi
		pop		esi
		retn

NodeCrashFix2_ReturnToCode:
		mov     esi, edi
		imul    esi, 98h
		jmp		NodeCrashFix2_JumpBack
	}
}

/*void __declspec(naked) HUDInitialiseBreak()
{
	_asm
	{
		pop		edi
		jmp		CHud::Initialise
	}
}*/

/*void __declspec(naked) TimestampSignatureHack()
{
	_asm
	{
		mov		ecx, [esp+28Ch-274h]
		cmp		eax, ecx
		jz		TimestampSignatureHack_ReturnTrue
		mov		eax, [esp+28Ch+4]
		mov		[eax], 1
		jmp		TimestampSignatureHack_CheckFailed

TimestampSignatureHack_ReturnTrue:
		jmp		TimestampSignatureHack_JumpBack
	}
}*/

void __declspec(naked) LoadGameFailedMessage_Inject()
{
	_asm
	{
		movzx	eax, byte ptr [esp+10h-5]
		push	eax
		call	LoadGameFailedMessage
		add		esp, 4
		jmp		LoadGameFailedMessage_JumpBack
	}
}

void __declspec(naked) SaveFallback_InjectOnLoad()
{
	_asm
	{
		add     esp, 0Ch
		test	eax, eax
		jnz		SaveFallback_InjectOnLoad_Return
		push	0
		push	ebx
		call	C_PcSave::PerformBlockFallbackOnLoad
		add		esp, 8
		cmp		eax, -2
		jz		SaveFallback_InjectOnLoad_WrongTimestamp
		cmp		eax, -3
		jz		SaveFallback_InjectOnLoad_TooNew
		//mov		ebx, eax
		cmp		eax, 0
		jnl		SaveFallback_InjectOnLoad_ReturnTrue
		mov		eax, [esp+28Ch+4]
		mov		[eax], 2
		jmp		TimestampSignatureHack_CheckFailed

SaveFallback_InjectOnLoad_WrongTimestamp:
		mov		eax, [esp+28Ch+4]
		mov		[eax], 1
		jmp		TimestampSignatureHack_CheckFailed

SaveFallback_InjectOnLoad_TooNew:
		mov		eax, [esp+28Ch+4]
		mov		[eax], 3
		jmp		TimestampSignatureHack_CheckFailed

SaveFallback_InjectOnLoad_ReturnTrue:
		xor		eax, eax

SaveFallback_InjectOnLoad_Return:
		jmp		SaveFallback_InjectOnLoad_JumpBack
	}
}

void __declspec(naked) SaveFallback_InjectOnSave()
{
	_asm
	{
		ja		SaveFallback_InjectOnSave_Return
		push	ebx
		call	C_PcSave::SaveBlockVersion
		add		esp, 4
		jmp		SaveFallback_InjectOnSave_ReturnTrue

SaveFallback_InjectOnSave_Return:
		jmp		SaveFallback_InjectOnSave_ReturnFalse
	}
}

void __declspec(naked) SaveFallback_FallbackPreBeta3Names()
{
	_asm
	{
		push	ebp
		push	1
		push	4
		lea		edx, [esp+2CCh-23Ch]
		push	edx
		call	fread
		add		esp, 10h
		call	C_PcSave::MakeTimestamp
		cmp		[esp+2C0h-23Ch], eax
		jz		SaveFallback_FallbackPreBeta3Names_PreBeta
		push	SEEK_SET
		push	5
		push	ebp
		call	fseek
		add		esp, 0Ch

SaveFallback_FallbackPreBeta3Names_PreBeta:
		push	130h
		jmp		SaveFallback_FallbackPreBeta3Names_JumpBack
	}
}

void __declspec(naked) SaveFallback_WrapAroundFirstBlock()
{
	static DWORD	dwObj, dwDummy;

	_asm
	{
		mov		dwObj, ecx
		push	1
		push	[ecx]
		call	C_PcSave::PerformBlockFallbackOnLoad
		add		esp, 8
		mov		ecx, dwObj
		cmp		eax, -3
		jz		SaveFallback_InjectOnLoad_TooNew
		cmp		eax, 0
		jl		SaveFallback_InjectOnLoad_ReturnFalse
		push	0
		call	C_PcSave::LoadFirstBlock
		jmp		TimestampSignatureHack_JumpBack

SaveFallback_InjectOnLoad_ReturnFalse:
		mov		eax, [esp+28Ch+4]
		mov		[eax], 2
		jmp		TimestampSignatureHack_CheckFailed

SaveFallback_InjectOnLoad_TooNew:
		mov		eax, [esp+28Ch+4]
		mov		[eax], 3
		jmp		TimestampSignatureHack_CheckFailed
	}
}

void __declspec(naked) MenuEntriesAlignHack()
{
	_asm
	{
		mov		cl, [ebp+15Dh]
		cmp		cl, 9
		jz		MenuEntriesAlignHack_AlignToRight
		cmp		cl, 10
		jz		MenuEntriesAlignHack_AlignToRight
		cmp		cl, 16
		jz		MenuEntriesAlignHack_AlignToRight
		push	ALIGN_Left
		jmp		MenuEntriesAlignHack_Finish

MenuEntriesAlignHack_AlignToRight:
		push	ALIGN_Right

MenuEntriesAlignHack_Finish:
		call	CFont::SetOrientation
		jmp		MenuEntriesAlignHack_JumpBack
	}
}

void __declspec(naked) MenuEntriesPositionHack_Inject()
{
	_asm
	{
		push	ecx
		lea		ecx, [esp+134h-11Ch]
		push	ecx
		mov		ecx, ebp
		call	CMenuManager::GetRightColumnPos
		pop		ecx
		jmp		MenuEntriesPositionHack_JumpBack
	}
}

void __declspec(naked) MenuEntriesLeftColumnHack()
{
	_asm
	{
		lea		ecx, CMenuManager::ms_pMenus[eax+ecx].entryList
		//lea		ecx, [ecx]CMenuManager::ms_pMenus[0].entryList
		//add		eax, ecx
		push	esi
		push	ebx
		push	ecx
//		mov		ecx, ebp
		call	CMenuManager::DrawLeftColumn
		//fstp	[esp+130h-11Ch]
		mov		eax, 57A202h
		jmp		eax
	}
}

/*void __declspec(naked) MenuEntriesLeftColumnHack2()
{
	_asm
	{
		push	eax
		push	edx
//		mov		ecx, ebp
		call	CMenuManager::GetLeftColumnPos_Height
		fstp	[esp+134h-0FCh]
		pop		eax
		jmp		MenuEntriesLeftColumnHack2_JumpBack
	}
}*/

void __declspec(naked) MenuEntriesPlaceSave()
{
	_asm
	{
		mov		CMenuManager::ms_pMenus.entryList.posX[ebx+eax], 80
		or		CMenuManager::ms_pMenus.entryList.screenHorAlign[ebx+eax], 0F0h
		mov		eax, 579BC9h
		jmp		eax
	}
}

void __declspec(naked) MenuEntriesPlaceSave2()
{
	_asm
	{
		mov		CMenuManager::ms_pMenus.entryList.posX[ecx+edx], 0
		and		CMenuManager::ms_pMenus.entryList.screenHorAlign[ecx+edx], 0Fh
		mov		ecx, 579C89h
		jmp		ecx
	}
}

void __declspec(naked) MenuEntryColourHack()
{
	_asm
	{
		mov		eax, CUpdateManager::nInterfaceStatus
		cmp		eax, UPTMODULESTATE_NEW_UPDATES
		jnz		MenuEntryColourHack_ColourNormal
		mov		al, [ebp].bCurrentScreen
		cmp		al, 33
		jnz		MenuEntryColourHack_CheckMainScreen
		cmp		ebx, 6
		jz		MenuEntryColourHack_ColourGuided
		jmp		MenuEntryColourHack_ColourNormal

MenuEntryColourHack_CheckMainScreen:
		cmp		al, 34
		jnz		MenuEntryColourHack_CheckPauseScreen
		cmp		ebx, 1
		jz		MenuEntryColourHack_ColourGuided
		jmp		MenuEntryColourHack_ColourNormal

MenuEntryColourHack_CheckPauseScreen:
		cmp		al, 41
		jnz		MenuEntryColourHack_CheckUpdaterScreen
		cmp		ebx, 5
		jz		MenuEntryColourHack_ColourGuided
		jmp		MenuEntryColourHack_ColourNormal

MenuEntryColourHack_CheckUpdaterScreen:
		cmp		al, 44
		jnz		MenuEntryColourHack_ColourNormal
		cmp		ebx, 2
		jz		MenuEntryColourHack_ColourGuided

MenuEntryColourHack_ColourNormal:
		push	MENU_INACTIVE_B
		push	MENU_INACTIVE_G
		push	MENU_INACTIVE_R

MenuEntryColourHack_Return:
		push	579A8Dh
		retn

MenuEntryColourHack_ColourGuided:
		push	MENU_UPDATES_B
		push	MENU_UPDATES_G
		push	MENU_UPDATES_R
		jmp		MenuEntryColourHack_Return
	}
}

/*void __declspec(naked) SetToNewMenuHack()
{
	_asm
	{
		push    esi
		mov		esi, ecx
		mov		al, CUpdateManager::bSeenUpdaterScreenYet
		test	al, al
		jz		SetToNewMenuHack_DontColour
		mov		al, [esp+8]
		cmp		al, 33
		jnz		SetToNewMenuHack_CheckMainMenu
//		mov		ax, 3
		mov		[CMenuManager::nColourMenuEntries], 3
		jmp		SetToNewMenuHack_Return

SetToNewMenuHack_CheckMainMenu:
		cmp		al, 34
		jnz		SetToNewMenuHack_CheckUpdaterMenu
		mov		[CMenuManager::nColourMenuEntries], 5
		jmp		SetToNewMenuHack_Return

SetToNewMenuHack_CheckUpdaterMenu:
		cmp		al, 44
		jnz		SetToNewMenuHack_DontColour
		mov		[CMenuManager::nColourMenuEntries], 5
		jmp		SetToNewMenuHack_Return

SetToNewMenuHack_DontColour:
		mov		[CMenuManager::nColourMenuEntries], -1

SetToNewMenuHack_Return:
		mov		al, [esi].bCurrentScreen
		push	573689h
		retn
	}
}

/*void __declspec(naked)
{
	_asm
	{
//		push	0FFh
	}
}*/

void __declspec(naked) StatsMenuActionHack()
{
	_asm
	{
		mov		eax, [esp+18h]
		test	eax, eax
		jz		StatsMenuActionHack_NoReturn
		mov		eax, [esp+14h]
		mov		byte ptr [eax], 1
		pop		edi
		pop		esi
		pop		ebx
		retn	0Ch

StatsMenuActionHack_NoReturn:
		push	577108h
		retn
	}
}

void __declspec(naked) WidescreenSupportRecalculateHack()
{
	_asm
	{
		mov		eax, [RsGlobal]
		mov		[eax].MaximumHeight, edx
		mov		eax, [FrontEndMenuManager]
		mov		al, [eax].m_bAspectRatioMode
		test	al, al
		jnz		WidescreenSupportRecalculateHack_SkipRecalculation
		push	0
		push	edx
		push	ecx
		call	WidescreenSupport::Recalculate
		add		esp, 0Ch

WidescreenSupportRecalculateHack_SkipRecalculation:
		pop		ebp
		pop		edi
		add     esp, 34h
		retn
	}
}

void __declspec(naked) WidescreenSupportRecalculateHack2()
{
	_asm
	{
		mov		eax, [RsGlobal]
		mov		[eax].MaximumHeight, ecx
		push	0
		push	ecx
		push	edx
		call	WidescreenSupport::Recalculate
		add		esp, 0Ch
		retn
	}
}

void __declspec(naked) WidescreenSupportRecalculateHack3()
{
	_asm
	{
		push	ecx
		mov		ecx, [RsGlobal]
		mov		[ecx].MaximumHeight, eax
		push	0
		push	eax
		push	edx
		call	WidescreenSupport::Recalculate
		add		esp, 0Ch
		pop		ecx
		retn
	}
}

void __declspec(naked) WidescreenFOVHack()
{
	_asm
	{
		fld		[esp+4Ch]
		cmp		[CCamera::bDontTouchFOVInWidescreen], 0
		jnz		WidescreenFOVHack_DoFOV
		mov		eax, [TheCamera]
		mov		al, [eax].m_WideScreenOn
		test	al, al
		jz		WidescreenFOVHack_DoFOV
		fdiv	[esp+50h]
		fstp	[esp+14h]
		mov		eax, [esp+4Ch]
		mov		[esp+10h], eax
		jmp		WidescreenFOVHack_JumpBack

WidescreenFOVHack_DoFOV:
		//fmul	[WidescreenSupport::fFOVMultiplier]
		fst		[esp+14h]
		fmul	[esp+50h]
		fstp	[esp+10h]
		jmp		WidescreenFOVHack_JumpBack
	}
}

/*void __declspec(naked) WidescreenFOVHack2()
{
	_asm
	{
		cmp		[CCamera::bDontTouchFOVInWidescreen], 0
		jnz		WidescreenFOVHack2_DoFOV
		mov		eax, [TheCamera]
		mov		al, [eax].m_WideScreenOn
		test	al, al
		jz		WidescreenFOVHack2_DoFOV
		mov		eax, [fFOV]
		fld		[eax]
		retn

WidescreenFOVHack2_DoFOV:
		mov		eax, [ScreenAspectRatio]
		fld		[eax]
		mov		eax, [fFOV]
		fmul	[eax]
		//fmul	[WidescreenSupport::fFOVMultiplier]
		retn
	}
}*/

static float CalcFOV()
{
	float	TehFOV = ScreenAspectRatio > 1.0f ? *fFOV * ScreenAspectRatio : *fFOV;
	return	TehFOV >= 175.0f ? 175.0f : TehFOV;
}

void __declspec(naked) WidescreenFOVHack2()
{
	_asm
	{
		push	ecx
		call	CalcFOV
		pop		ecx
		retn
	}
}

void __declspec(naked) AimpointCalc()
{
	_asm
	{
		fstp	st
		mov		edx, [ScreenAspectRatio]
		fmul	[edx]
		mov		edi, [esp+1Ch+14h]
		mov     edx, edi
		mov		ebx, 5149A6h
		jmp		ebx
	}
}

void __declspec(naked) WidescreenBordersHack()
{
	_asm
	{
		push	ecx
		push	[esp+8]
		call	CCamera::GetScreenRect
		pop		ecx
		retn	4
	}
}

void __declspec(naked) WidescreenTextPositionHack()
{
	_asm
	{
		push	eax
		call	WidescreenSupport::GetTextPosition
		fmulp	st(1), st
		pop		eax
		retn
	}
}

/*void __declspec(naked) WidescreenSkyWidthHack()
{
	_asm
	{
		call	WidescreenSupport::GetSkyWidth
		fmulp	st(1), st
		retn
	}
}*/

void __declspec(naked) Widescreen_SwitchInject()
{
	_asm
	{
		cmp		al, 5
		jnl		Widescreen_SwitchInject_ZeroTheValue
		inc		al
		jmp		Widescreen_SwitchInject_Return


Widescreen_SwitchInject_ZeroTheValue:
		xor		al, al

Widescreen_SwitchInject_Return:
		mov		[esi].m_bAspectRatioMode, al
		mov		eax, [RsGlobal]
		push	1
		push	[eax].MaximumHeight
		push	[eax].MaximumWidth
		call	WidescreenSupport::Recalculate
		add		esp, 0Ch
		mov		ecx, esi
		jmp		Widescreen_SwitchInject_JumpBack
	}
}

void __declspec(naked) Widescreen_StringInject()
{
	_asm
	{
		movzx	eax, al
		push	eax
		push	offset aFem_Asd
		push	LENGTH gString
		push	offset gString
		call	_snprintf
		add		esp, 10h
		mov		ecx, [TheText]
		push	offset gString
		jmp		Clock_StringInject_JumpBack
	}
}

/*void __declspec(naked) Widescreen_TextDrawsFix()
{
	_asm
	{
		cmp		byte ptr [esi+0Bh], 0
		jz		Widescreen_TextDrawsFix_DontFix
		mov		eax, [WidescreenSupport::fScreenWidthDivider]
		mov		WidescreenSupport::fTextDrawsWidthMultiplier, eax
		jmp		Widescreen_TextDrawsFix_Return

Widescreen_TextDrawsFix_DontFix:
		mov		WidescreenSupport::fTextDrawsWidthMultiplier, 3ACCCCCDh

Widescreen_TextDrawsFix_Return:
		mov		eax, [RsGlobal]
		fild	[eax].MaximumHeight
		retn
	}
}*/

void __declspec(naked) Widescreen_TextDrawsFix2()
{
	static const float	fWidthDiv = 1.0f/640.0f;
	static const float	fHeightDiv = 1.0f/448.0f;
	_asm
	{
		lea		ecx, [esp+1A0h-190h]
		push	ecx
		sub		esp, 8
		fmul	dword ptr [esi+28h]
		fmul	[fWidthDiv]
//		fmul	[WidescreenSupport::fTextDrawsWidthMultiplier]
		fstp	[esp]
		fmul	dword ptr [esi+2Ch]
		fmul	[fHeightDiv]
		fstp	[esp+1ACh-1A8h]
		jmp		Widescreen_TextDrawsFix2_JumpBack
	}
}

void __declspec(naked) LookLeftRightHack()
{
	_asm
	{
		xor		bl, bl
		cmp		[ecx]CEntity.m_nModelIndex, VT_RCBANDIT
		jz		LookLeftRightHack_ReturnTrue
		cmp		eax, 5
		jnz		LookLeftRightHack_Return

LookLeftRightHack_ReturnTrue:
		inc		bl

LookLeftRightHack_Return:
		mov		eax, 52781Dh
		jmp		eax
	}
}

void __declspec(naked) DrawDistanceRecalc()
{
	_asm
	{
		mov		eax, 57C660h
		call	eax
		jmp		CModelInfo::RecalcDrawDistances
	}
}

void __declspec(naked) DriveByKillFix()
{
	_asm
	{
		cmp		edx, 1Ch
		jnz		DriveByKillFix_FalseNoPop
		mov		eax, [CWorld::Players]
		mov		eax, [eax]
		push	ecx
		mov		ecx, [eax+46Ch]
		test	ch, 1
		jz		DriveByKillFix_False
		mov		ecx, [eax+58Ch]
		test	ecx, ecx
		jz		DriveByKillFix_False
		pop		ecx
		jmp		DriveByKillFix_ReturnTrue

DriveByKillFix_False:
		pop		ecx

DriveByKillFix_FalseNoPop:
		jmp		DriveByKillFix_ReturnFalse
	}
}

/*DWORD CScriptFunction::scriptsCounter;

void __declspec(naked) ZeroScriptsCounter()
{
	_asm
	{
		sub		esp, 18h
		mov		CScriptFunction::scriptsCounter, 0
		push	ebx
		push	esi
		jmp		ZeroScriptsCounter_JumpBack
	}
}*/

void __declspec(naked) LoadFunctionBreak()
{
	_asm
	{
		add		esp, 24h
		jmp		CScriptFunction::LoadAllFunctions
	}
}

void __declspec(naked) RightShockKeyHack()
{
	_asm
	{
		push	ebx
		mov		ebx, [CWorld::Players]
		mov		ebx, [ebx]
		test	ebx, ebx
		jz		RightShockKeyHack_InMenuOrWTF
		push	ecx
		mov		ecx, [ebx+46Ch]
		test	ch, 1
		jz		RightShockKeyHack_OnFoot
		mov		ecx, [ebx+58Ch]
		test	ecx, ecx
		jz		RightShockKeyHack_OnFoot
		cmp     [ebx+530h], 32h
		jnz		RightShockKeyHack_OnFoot
		pop		ecx
		cmp		[ecx+edx*8+0F30h], eax
		jnz		RightShockKeyHack_Return
		jmp		RightShockKeyHack_PressedKeyWeNeed

RightShockKeyHack_OnFoot:
		pop		ecx

RightShockKeyHack_InMenuOrWTF:
		cmp		[ecx+edx*8+0D30h], eax
		jnz		RightShockKeyHack_Return

RightShockKeyHack_PressedKeyWeNeed:
		mov     word ptr [esi+26h], di

RightShockKeyHack_Return:
		pop		ebx
		jmp		RightShockKeyHack_JumpBack
	}
}

void __declspec(naked) C3DMarkerSizeHack()
{
	_asm
	{
		push	eax		// Not keeping it causes marker to flicker
		mov		ecx, esi
		call	C3DMarker::CalculateRealSize
		fst		C3DMarkers::m_PosZMult
		pop		eax
		retn
	}
}

void __declspec(naked) NoRadioCommercialsHack()
{
	_asm
	{
		mov		ecx, dword ptr [esp+4]
		cmp		cl, 0Ah
		jz		NoRadioCommercialsHack_Return
		movsx   eax, byte ptr [esp+0Ch]
		cmp		eax, 3
		jnl		NoRadioCommercialsHack_Return
		mov		eax, 3

NoRadioCommercialsHack_Return:
		jmp		NoRadioCommercialsHack_JumpBack
	}
}

void __declspec(naked) RadioResetWrapper()
{
	_asm
	{
		call	CAERadioTrackManager::ResetExtra
		mov		al, 1
		retn	4
	}
}

void __declspec(naked) SaveRadios()
{
	_asm
	{
		pop		ebx
		pop		ecx
		jmp		CAERadioTrackManager::Save
	}
}

void __declspec(naked) LoadRadios()
{
	_asm
	{
		pop		ebx
		pop		ecx
		jmp		CAERadioTrackManager::Load
	}
}

void __declspec(naked) VideoPlayerCreate1()
{
	CVideoPlayer::Create("MOVIES\\LOGO.BIK");
	_asm jmp		VideoPlayerCreate1_JumpBack
}

void __declspec(naked) VideoPlayerCreate2()
{
	CVideoPlayer::Release();
	CVideoPlayer::Create("MOVIES\\GTATITLES.BIK");
	*gameState = 4;
	_asm jmp		VideoPlayerPlayNextFrame_JumpBack
}

void __declspec(naked) VideoPlayerPlayNextFrame()
{
	if ( !CVideoPlayer::PlayNextFullscreenFrame() )
		++(*gameState);

	_asm jmp		VideoPlayerPlayNextFrame_JumpBack
}

void __declspec(naked) VideoPlayerProc()
{
	_asm
	{
		push	esi
		call	CVideoPlayer::WindowProc
		add		esp, 4
		jmp		VideoPlayerProc_JumpBack
	}
}

void __declspec(naked) VideoPlayerRelease()
{
	_asm
	{
		call	CVideoPlayer::Release
		jmp		VideoPlayerRelease_JumpBack
	}
}
/*void __declspec(naked) CRunningScriptInitExtension()
{
	_asm
	{
		pop     ebx
		mov		ecx, edx
		jmp		CRunningScript::AssignExtrasToScript
//		mov		word ptr [edx+03Ah], -1
//		mov		byte ptr [edx+0CAh], bl
//		mov		byte ptr [edx+0D3h], 1
	}
}*/

void __declspec(naked) MaxosFrameLimitHack()
{
	static float		fOne, fTwo;
	static const float	fSecond = 1000.0f;

	_asm
	{
		test	al, al
		jz		MaxosFrameLimitHack_NoLimit
		mov		eax, [RsGlobal]
		fild	[eax].frameLimit
		fdivr	[fSecond]
		fstp	[fOne]
		jmp		MaxosFrameLimitHack_LimitFrames

MaxosFrameLimitHack_NoLimit:
		mov		fOne, 41000000h

MaxosFrameLimitHack_LimitFrames:
		mov		eax, [esp+14h]
		mov		fTwo, eax
	}

	if( fOne > fTwo )
	{
		if ( !bNoTimeFix )
		{
			static int			nSyncDiff;

			nSyncDiff = static_cast<int>(fOne - fTwo) - 5;

			if( nSyncDiff > 0 )
 				MsgWaitForMultipleObjects(0, nullptr, FALSE, nSyncDiff, QS_ALLEVENTS);
		}
	}
	else
		RsEventHandler(rsIDLE, reinterpret_cast<void*>(1));

	_asm
	{
		mov		eax, 748DA3h
		jmp		eax
	}
}

void __declspec(naked) FrameLimit_SwitchInject()
{
	_asm
	{
		cmp		al, 4
		jnl		FrameLimit_SwitchInject_ZeroTheValue
		inc		al
		movzx	ecx, al
		mov		ecx, RsGlobalFrameLimits[ecx*4]
		mov		ebx, [RsGlobal]
		mov		[ebx].frameLimit, ecx
		jmp		FrameLimit_SwitchInject_GoBack

FrameLimit_SwitchInject_ZeroTheValue:
		xor		al, al

FrameLimit_SwitchInject_GoBack:
		jmp		FrameLimit_SwitchInject_JumpBack
	}
}

void __declspec(naked) FrameLimit_StringInject()
{
	_asm
	{
		mov		ecx, [TheText]
		test	al, al
		jz		FrameLimit_StringInject_Off
		movzx	eax, al
		push	offset gString
		push	0
		push	0
		push	0
		push	0
		push	0
		push	RsGlobalFrameLimits[eax*4]
		push	offset aFem_Fps
		call	CText::Get
		push	eax
		call	CMessages::InsertNumberInString
		add		esp, 20h
		mov		esi, offset gString
		jmp		FrameLimit_StringInject_JumpBack

FrameLimit_StringInject_Off:
		jmp		Clock_StringInject_JumpToOFF
	}
}

void __declspec(naked) FrameLimit_SetFPS()
{
	_asm
	{
		movzx	eax, byte ptr [edi].frameLimiterMode
		test	eax, eax
		jz		FrameLimit_SetFPS_Return
		mov		ecx, RsGlobalFrameLimits[eax*4]
		mov		eax, [RsGlobal]
		mov		[eax].frameLimit, ecx

FrameLimit_SetFPS_Return:
		pop		edi
		pop		esi
		add     esp, 44h
		retn
	}
}

void __declspec(naked) FrameLimit_SetFPS2()
{
	_asm
	{
		mov		[esi].frameLimiterMode, 2
		mov		[esi].hudMode, bl
		mov		ebx, RsGlobalFrameLimits+8
		mov		ecx, [RsGlobal]
		mov		[ecx].frameLimit, ebx
		mov		[esi].m_bAspectRatioMode, al
		mov		[esi].m_bSubtitlesEnabled, al
		push	1
		push	[ecx].MaximumHeight
		push	[ecx].MaximumWidth
		call	WidescreenSupport::Recalculate
		add		esp, 0Ch
		pop		ebx
		pop		esi
		retn	4
	}
}

void __declspec(naked) CameraInitHack()
{
	_asm
	{
		mov		[CCamera::bDontTouchFOVInWidescreen], bl
		pop		ebx
		add		esp, 0Ch
		retn
	}
}

void __declspec(naked) OpaqueRadarHack()
{
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
	_asm
	{
		mov		ecx, 586420h
		mov		al, [esp+140h-12Dh]
		test	al, al
		jmp		ecx
	}

}

/*void __declspec(naked) LoadFontsHack()
{
	CFont::Sprite[0].SetTexture("font1", "font1m");
	CFont::Sprite[1].SetTexture("font2", "font2m");
	CFont::Sprite[2].SetTexture("font3", "font3m");
	CFont::Sprite[3].SetTexture("font4", "font4m");
	_asm jmp LoadFontsHack_JumpBack
}

void __declspec(naked) ReleaseFontsHack()
{
	CFont::Sprite[0].Delete();
	CFont::Sprite[1].Delete();
	CFont::Sprite[2].Delete();
	CFont::Sprite[3].Delete();
	_asm jmp ReleaseFontsHack_JumpBack
}*/

void __declspec(naked) SetCutsceneModelHack()
{
	_asm
	{
//		push	edi
		push	esi
		call	CCutsceneMgr::SetProperCSPlrModel
		add		esp, 4
//		pop		edi
		jmp		SetCutsceneModelHack_JumpBack
	}
}

void __declspec(naked) ErrorSoundCentre()
{
	_asm
	{
		push	1Dh
		push	ebx
		lea		ecx, [esp+70h]
		mov		dword ptr [edx+8],eax
		mov		eax, 4EFE50h
		call	eax
		mov		ecx, dword ptr [esp+0FCh]
		lea		edx, [esp+38h]
		mov		dword ptr [esp+54h], ecx
		mov		edi, 111Bh
		push	edx
		mov		ecx, 0B62CB0h
		mov		word ptr [esp+92h], di
		mov		eax, 4EFB10h
		call	eax

		push	ebx
		push	ebx
		push	ebx
		push	ebx
		push    3F800000h
		push    3F574BC7h
		push    3F800000h
		push	ebp
		sub     esp, 0Ch
		mov		edx, esp
		mov     eax, [esp+11Ch-0DCh]
		mov     [edx], eax
		mov     eax, [esp+11Ch-0D4h]
		push	esi
		mov     ecx, [esp+0F0h-0D8h]
		mov     [edx+4], ecx
		push	1Bh
		push	4DDD89h
		retn
	}
}

void __declspec(naked) BuySoundCentre()
{
	_asm
	{
		push	1Dh
		push	ebx
		lea		ecx, [esp+70h]
		mov		dword ptr [edx+8],eax
		mov		eax, 4EFE50h
		call	eax
		mov		ecx, dword ptr [esp+0FCh]
		lea		edx, [esp+38h]
		mov		dword ptr [esp+54h], ecx
		mov		edi, 111Bh
		push	edx
		mov		ecx, 0B62CB0h
		mov		word ptr [esp+92h], di
		mov		eax, 4EFB10h
		call	eax

		push	ebx
		push	ebx
		push	ebx
		push	ebx
		push    3F800000h
		push    3F800000h
		push    3F800000h
		push	ebp
		sub     esp, 0Ch
		mov		edx, esp
		mov     eax, [esp+11Ch-0DCh]
		mov     [edx], eax
		mov     eax, [esp+11Ch-0D4h]
		push	esi
		mov     ecx, [esp+0F0h-0D8h]
		mov     [edx+4], ecx
		push	1Bh
		push	4DDAEBh
		retn
	}
}

void __declspec(naked) UpdaterProcessHack()
{
	_asm
	{
		call	CUpdateManager::Process
		push	6997E0h
		retn
	}
}

void __declspec(naked) UpdaterMenuDrawHack()
{
	_asm
	{
		cmp		eax, 45
		jnz		UpdaterMenuDrawHack_GoAhead
		mov		ecx, ebp
		call	CMenuManager::PrintDLCScreen
		mov		ecx, 57954Fh
		jmp		ecx

UpdaterMenuDrawHack_GoAhead:
		cmp		eax, 44
		jnz		UpdaterMenuDrawHack_GoAhead2
		mov		ecx, ebp
		call	CMenuManager::PrintUpdaterScreen
		mov		ecx, 57954Fh
		jmp		ecx

UpdaterMenuDrawHack_GoAhead2:
		cmp		eax, 49
		jnz		UpdaterMenuDrawHack_GoAhead3
		mov		ecx, ebp
		call	CMenuManager::PrintActivationScreen
		mov		ecx, 57954Fh
		jmp		ecx

UpdaterMenuDrawHack_GoAhead3:
		sub     eax, 2
		jz		UpdaterMenuDrawHack_DrawBrief
		mov		ecx, 57952Bh
		jmp		ecx

UpdaterMenuDrawHack_DrawBrief:
		mov		ecx, 57953Fh
		jmp		ecx
	}
}

void __declspec(naked) UpdaterTextSwap()
{
	_asm
	{
		cmp		cl, ACTION_UPDATER
		jnz		UpdaterTextSwap_NotUpdaterText
		call	CUpdateManager::GetGXTEntryForButton

UpdaterTextSwap_GetText:
		mov		ecx, [TheText]
		push	eax
		push	579D73h
		retn

UpdaterTextSwap_NotUpdaterText:
		cmp		cl, ACTION_DLC
		jnz		UpdaterTextSwap_NotDLCText
		push	[esp+130h-10Ch]
		mov		ecx, ebp
		call	CMenuManager::ProcessDLCSlot
		test	eax, eax
		jnz		UpdaterTextSwap_GetText
		xor		ebx, ebx
		//mov		dword ptr [esp+130h-114h], eax
		push	579D7Ah
		retn

UpdaterTextSwap_NotDLCText:
		cmp		cl, ACTION_SERIAL
		jnz		UpdaterTextSwap_NotSerial
		mov		cl, CMenuManager::m_bSerialFull
		test	cl, cl
		jnz		UpdaterTextSwap_DontColour
		push	0FF000000h + (MENU_LOCKED_B<<16) + (MENU_LOCKED_G<<8) + MENU_LOCKED_R
		call	CFont::SetColor
		add		esp, 4

UpdaterTextSwap_DontColour:
		mov		ecx, [TheText]
		push	579D72h
		retn

UpdaterTextSwap_NotSerial:
		cmp		cl, ACTION_JOYMOUSE
		mov		ecx, [TheText]
		push	579D58h
		retn
	}
}

void __declspec(naked) UpdaterMenuAction()
{
	_asm
	{
		call	CUpdateManager::HandleButtonClick
		pop     edi
		pop		esi
		mov		al, bl
		pop		ebx
		retn	8
	}
}

static bool		bThisIMGIsEncrypted;

void __stdcall DecryptIMG(CBlowFish* pEncryption, unsigned char* pBuf, size_t nSize, int iMode)
{
	pEncryption->Decrypt(pBuf, nSize, iMode);
}

void __declspec(naked) IMGEncryptionFindOut()
{
	_asm
	{
		cmp		dword ptr [esp+54h-28h], '2REV'
		jz		IMGEncryptionFindOut_NotEncrypted
		lea		edx, [esp+54h-28h]
		push	0
		push	8
		push	edx
		push	[esp+60h+0Ch]
		call	DecryptIMG
		mov		[bThisIMGIsEncrypted], 1
		jmp		IMGEncryptionFindOut_Return

IMGEncryptionFindOut_NotEncrypted:
		mov		[bThisIMGIsEncrypted], 0

IMGEncryptionFindOut_Return:
		mov		eax, [esp+54h-24h]
		mov		[esp+54h-38h], eax
		add     esp, 0Ch
		push	5B61C4h
		retn
	}
}

void __declspec(naked) IMGEncryptionDo()
{
	_asm
	{
		cmp		[bThisIMGIsEncrypted], 0
		jz		IMGEncryptionDo_NoEncryption
		lea		eax, [esp+58h-20h]
		push	1
		push	32
		push	eax
		push	[esp+64h+0Ch]
		call	DecryptIMG

IMGEncryptionDo_NoEncryption:
		movzx   eax, word ptr [esp+58h-1Ch]
		push	5B61EBh
		retn
	}

}

void __declspec(naked) CommandlineEventHack()
{
	_asm
	{
		push	esi
		call	ParseCommandlineArgument
		add		esp, 4
		pop		edi
		pop		esi
		mov		eax, rsEVENTPROCESSED
		retn
	}
}

void __declspec(naked) ReadCommandlineFile()
{
	_asm
	{
		call	ParseCommandlineFile
		mov		edx, [RsGlobal]
		mov		edx, [edx].MaximumWidth
		push	748901h
		retn
	}
}

void __declspec(naked) LoadGameWithDLCHack()
{
	_asm
	{
		call	CDLCManager::LoadLevelFiles
		mov		eax, 5BFA90h
		jmp		eax
	}
}

void __declspec(naked) DLCMenuAction()
{
	_asm
	{
		push	[esi]CMenuManager.currentMenuEntry
		call	CDLCManager::HandleButtonClick
		add		esp, 4
		pop     edi
		pop		esi
		mov		al, bl
		pop		ebx
		retn	8
	}
}

void __declspec(naked) ActivateSerialAction()
{
	_asm
	{
		mov		al, CMenuManager::m_bSerialFull
		test	al, al
		jz		ActivateSerialAction_DontActivate
		push	offset CMenuManager::m_strSerialCode
		call	CDLCManager::ActivateSerial
		add		esp, 4

ActivateSerialAction_DontActivate:
		pop     edi
		pop		esi
		mov		al, bl
		pop		ebx
		retn	8
	}
}

void __declspec(naked) AutoInstallUpdatesAction()
{
	_asm
	{
		mov		al, [CUpdateManager::bAutoInstallUpdates]
		test	al, al
		setz	al
		mov		[CUpdateManager::bAutoInstallUpdates], al
		call	CUpdateManager::SaveSettings
		pop		edi
		pop		esi
		mov		al, bl
		pop		ebx
		retn	8
	}
}

void __declspec(naked) CheckEveryAction()
{
	_asm
	{
		mov		al, [CUpdateManager::bCheckingPeriod]
		mov		dl, [esp+0Ch+4]
		cmp		dl, 0
		jl		CheckEveryAction_Previous
		inc		al
		cmp		al, 3
		jna		CheckEveryAction_Return
		xor		al, al
		jmp		CheckEveryAction_Return

CheckEveryAction_Previous:
		test	al, al
		jz		CheckEveryAction_ToMax
		dec		al
		jmp		CheckEveryAction_Return

CheckEveryAction_ToMax:
		mov		al, 3

CheckEveryAction_Return:
		mov		[CUpdateManager::bCheckingPeriod], al
		call	CUpdateManager::SaveSettings
		pop		edi
		pop		esi
		mov		al, bl
		pop		ebx
		retn	8
	}
}

void __declspec(naked) VehAudioHook()
{
	_asm
	{
		push	edx
		push	eax
		mov		ecx, ebp
		call	CAEVehicleAudioEntity::GetVehicleAudioSettings
		pop		edx
		mov		eax, 4F77D8h
		jmp		eax
	}
}

/*void __declspec(naked) RotorsHook()
{
	_asm
	{
		mov		ecx, esi
		call	CHeli::ProcessRotorsAlpha
		mov		ecx, 6C4521h
		jmp		ecx
	}
}*/

void __declspec(naked) Language6Action()
{
	_asm
	{
		cmp		[esi]CMenuManager.language, 5
		jz		Language6Action_ReturnFalse
		mov		[esi]CMenuManager.language, 5
		mov		eax, 57D429h
		jmp		eax

Language6Action_ReturnFalse:
		pop		edi
		pop		esi
		mov		al, bl
		pop		ebx
		retn	8
	}
}

void OpaqueRadarHack2(RwPrimitiveType primType, RwIm2DVertex *vertices, RwInt32 numVertices)
{
	RwIm2DRenderPrimitive(primType, vertices, numVertices);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, FALSE);
}

#ifdef INCLUDE_MULTIFONTFILES

void __declspec(naked) MultipleFontsTXD_Inject()
{
	_asm
	{
		call	GetFontsTXDByLanguage
		push	eax
		jmp		MultipleFontsTXD_JumpBack
	}
}

void __declspec(naked) MultipleFontsDAT_Inject()
{
	_asm
	{
		call	GetFontsDATByLanguage
		push	eax
		jmp		MultipleFontsDAT_JumpBack
	}
}

void __declspec(naked) MultipleFontsFiles_Validator()
{
	_asm
	{
		mov		bCanReloadFonts, al
		add		esp, 18h
		retn
	}
}

#endif

#ifdef ADD_UNFINISHED_WINDOWED_MODE

void __declspec(naked) InjectWindowedMode()
{
	_asm
	{
		push	0
		push	1
		push	100
		mov		edx, 72F460h
		call	edx
		//add		esp, 0Ch
		mov		edx, 0C920D0h
		mov		edx, [edx]
		mov     [edx+esi*4], eax
		//mov		ebx, eax
		//mov		eax, [esp+30h]
		//push	offset aWindowed
		//mov		ecx, TheText
		//call	CText::Get
		//mov		stackPtr, eax
		//pushad
		//popad
		//mov		[esp+30h], ecx
		push	100
		push	offset aWindowed
		push	eax
		//push	ebx
		call	strncpy
		add		esp, 18h
		mov		edx, 745C53h
		jmp		edx
	}
}

#endif

void LogToFile(const char* str, ...)
{
	if ( FILE* LogFile = fopen("VCSPC.log", "a") )
	{
		SYSTEMTIME	systemTime;
		va_list		arguments;
		char		TempString[MAX_PATH];

		va_start(arguments, str);
		vsnprintf(TempString, sizeof(TempString), str, arguments);
		GetLocalTime(&systemTime);
		fprintf(LogFile, "[%02d/%02d/%d %02d:%02d:%02d] ", systemTime.wDay, systemTime.wMonth, systemTime.wYear, systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
		fputs(TempString, LogFile);
		fputc('\n', LogFile);
		fclose(LogFile);
		va_end(arguments);

		OutputDebugString(TempString);
		OutputDebugString("\n");
	}
}