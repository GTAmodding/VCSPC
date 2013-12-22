#pragma warning(disable:4481)	// nonstandard extension used: override specifier 'override'
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#define WINVER 0x0501
#define _WIN32_WINNT 0x0501

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <mmsystem.h>
#include <dsound.h>
#include <ctime>
#include <cmath>
#include <vector>
#include <map>
#include <cassert>
#include "buildnumber.h"


//#define INCLUDE_AUDIO_DEBUG
#define INCLUDE_WINDOW_NAME
//#define INCLUDE_VCSIMG_LOADER
//#define INCLUDE_MULTIFONTFILES

#ifdef DEBUG
//#define EXTRA_ASH_LOGGING
//#define TRACE_D3D_MARKER
//#define EXTRA_WINDOWMODE_CHECKING
//#define ADD_UNFINISHED_WINDOWED_MODE
//#define TRACE_PROJECTILES
//#define NO_AUDIO
//#define TRACE_COLOUR_STUFF
//#define TRACE_STREAMING
//#define TRACE_STREAMING2
//#define TRACE_CTEXT
//#define TRACE_UPCASE
//#define TRACE_NODES_STREAMS
//#define INCLUDE_COOL_BINK_SCREEN
//#define INCLUDE_SQUARE_RADAR
//#define INCLUDE_TRIANGLE_RADAR
//#define TRACE_STREAMS
//#define TRACE_FUCKING_TEXTURES
//#define INCLUDE_STREAMING_TEXT
//#define LIST_MEMORY
//#define MAKE_CONSOLE
//#define DUMP_MENUS

//#define MAKE_ZZCOOL_MOVIE_DEMO

#endif

#define DONT_FIX_STREAMING

#define _x(a) (RsGlobal.MaximumWidth - WidescreenSupport::fProperWidthMultiplier * (a))
#define _xleft(a) (WidescreenSupport::fProperWidthMultiplier * (a))
#define _xmiddle(a) ((RsGlobal.MaximumWidth / 2) + WidescreenSupport::fProperWidthMultiplier * (a))
#define _y(a) (WidescreenSupport::fProperHeightMultiplier * (a))
#define _ydown(a) (RsGlobal.MaximumHeight - WidescreenSupport::fProperHeightMultiplier * (a))
#define _ymiddle(a) ((RsGlobal.MaximumHeight / 2) + WidescreenSupport::fProperHeightMultiplier * (a))
#define _width(a) _xleft(a)
#define _height(a) _y(a)

// TODO: Remove this, ugh
#define _patchHandling(h, n) strncpy((char*)((DWORD)0x8D3978 + h * 14), n, 14)

/*#define GAMEVER_10EU_noCD 0
#define GAMEVER_10US_HOODLUM 1
#define GAMEVER_11EU_noCD 2
#define GAMEVER_10EU 3
#define GAMEVER_UNKNOWN 255*/

#define MOD_VERSION	"v3.0 BETA"
#define VERSION_NAME "Blue Hesper"
#define VERSION_NAME_UPPERCASE "BLUE HESPER"
#define RELEASE_CANDIDATE "1"

#define CEntryData_ARRAYSIZE 0x12
#define CMenuItem_ARRAYSIZE 0xE2
#define CPed_ARRAYSIZE 0x79C
#define CWeaponSlot_ARRAYSIZE 0x1C
#define CPlayer_ARRAYSIZE 0x190
#define CRunningScript_ARRAYSIZE 0xE0
#define CWeaponInfo_ARRAYSIZE 0x70
#define CDarkel_ARRAYSIZE 0xCC0
#define RwDevice_ARRAYSIZE 0x38
#define RwEngineInstance_ARRAYSIZE 0x158
#define CWanted_ARRAYSIZE 0x29C
#define CPhysical_ARRAYSIZE 0x138
#define CEntity_ARRAYSIZE 0x38
#define CPlaceable_ARRAYSIZE 0x14
#define LoadedObjectInfo_ARRAYSIZE 0x14
#define CClock_ARRAYSIZE 0x1C
#define CMenuManager_ARRAYSIZE 0x1B78
#define CVehicle_ARRAYSIZE 0x5A0
#define CPlayerData_ARRAYSIZE 0xAC
#define CGridref_ARRAYSIZE 0xC80
#define CVehicleModelInfo_ARRAYSIZE 0x308
#define CPedModelInfo_ARRAYSIZE 0x44
#define COnscreenTimer_ARRAYSIZE 0x154
#define CCamera_ARRAYSIZE 0xD78
#define CBaseModelInfo_ARRAYSIZE 0x20
#define CText_ARRAYSIZE 0xA90
#define CGarages_ARRAYSIZE 0x1C
#define CPad_ARRAYSIZE 0x134
#define CKeyState_ARRAYSIZE 0x270
#define CAnimationStyleDescriptor_ARRAYSIZE 0x30
#define CPool_ARRAYSIZE 0x14
#define CGangWeapons_ARRAYSIZE 0x10
#define C3DMarker_ARRAYSIZE 0xA0
#define CFont_ARRAYSIZE 0x40

#define	PlayerStatsInteger	((int*)0xB78E20)
#define	PlayerStatsFloat	((float*)0xB79380)


void			LogToFile(const char* str, ...);
void			ToLower(char* str, BYTE len);
int				random(int a, int b);

#ifdef MAKE_CONSOLE
#define echo(x, ...) printf(x, __VA_ARGS__)
#else
#define echo(x, ...)
#endif

/*if ( dwAddress >= 0x7466D0 && dwAddress < 0x857000 )
	{
		if ( dwAddress < 0x7BA940 )
			dwAddress += 0x50;
		else
			dwAddress += 0x40;*/

#if defined DEBUG && !defined MAKE_ZZCOOL_MOVIE_DEMO
void			EnterAmazingScreenshotMode(bool bEnable);
#endif

#define RwEngineInstance (*rwengine)
#define RWFRAMESTATICPLUGINSSIZE 24

#include <rwsdk\rwcore.h>
#include <rwsdk\rpworld.h>
#include <rwsdk\rtpng.h>

#include <binkW32\bink.h>
#include <blowfish\Blowfish.h>
#include "VCSPC_SDK_Internal.h"
#include "VCSPC_SDK.h"
#include "..\common\Date.h"
#include "MemoryMgr.h"
#include "Rs.h"
#include "WidescreenSupport.h"
#include "General.h"
#include "Timer.h"
#include "PNGArchive.h"
#include "CCRC32.h"
#include "FileMgr.h"
#include "PcSave.h"
#include "Sprite.h"
#include "EmpireMgr.h"
#include "CCam.h"
#include "CCamera.h"
#include "Building.h"
#include "CWeaponInfo.h"
#include "CWeaponSlot.h"
#include "ModelInfo.h"
#include "CPedIntelligence.h"
#include "CWanted.h"
#include "CPed.h"
#include "Stats.h"
#include "Darkel.h"
#include "CVehicle.h"
#include "PlayerInfo.h"
#include "World.h"
#include "CConfiscatedWeapons.h"
#include "Pools.h"
#include "Script.h"
#include "Streaming.h"
#include "CText.h"
#include "RwEngineInstance.h"
#include "CFont.h"
#include "CClock.h"
#include "Hud.h"
#include "UpdateManager.h"
#include "DLCManager.h"
#include "Frontend.h"
#include "CutsceneManager.h"
#include "Garages.h"
//#include "CGridref.h"
#include "TxdStore.h"
#include "Pad.h"
#include "AnimMgr.h"
#include "CGangWeapons.h"
#include "C3DMarker.h"
#include "CShadows.h"
#include "Audio.h"
#include "Replay.h"
#include "VideoPlayer.h"
#include "UserDisplay.h"
#include "Messages.h"
#include "Radar.h"

extern char*				latestMissionName;
extern char*				PriorityText;
extern char*				StyledText_1;
extern char*				StyledText_2;
extern char*				StyledText_3;
extern char*				StyledText_4;
extern char*				StyledText_5;
extern char*				StyledText_6;
extern char*				StyledText_7;
extern char*				Garage_MessageIDString;
extern SCRIPT_VAR*			scriptParams;
extern SCRIPT_VAR*			scriptLocals;
extern void*				scmBlock;
extern DWORD*				CPopulation__allRandomPedThisType;
extern DWORD*				unkLastBreathCheck;
extern DWORD*				PlayerEnexEntryStage;
extern DWORD*				activeInterior;
extern DWORD*				memoryAvailable;
extern DWORD*				memoryUsed;
extern eFlash*				wFlashingComponentID;
extern long*				CTRubberMinPos;
extern long*				CTRubberMaxPos;
extern float*				WidthAspectRatio;
extern float*				HeightAspectRatio;
extern float*				ScreenAspectRatio;
extern float*				fFOV;
extern BYTE*				bWants_To_Draw_Hud;
extern BYTE*				radarGrey;
extern BYTE*				mpackNumber;
extern bool*				bHideStyledTextWhileFading;
extern float*				currentFPS;
extern RsGlobalType&		RsGlobal;
extern CClock*				clock_struct;
extern CRGBA*				BaseColors;
extern CMenuManager&		FrontEndMenuManager;
extern CMusicManager*		MusicManager;
extern std::pair<void*,int>* const materialRestoreData;
//extern CGridref*			gridref;
extern CText&				TheText;
extern void**				rwengine;
extern CCamera*				camera;
extern CGarages*			garages;
extern CKeyState*			activeKeyState;
extern CKeyState*			currKeyState;
extern CKeyState*			prevKeyState;
//extern CSprite2d*			texFonts;
extern CGangWeapons*		gangWeapons;
extern CRunningScript**		pActiveScripts;
extern CRunningScript*		ScriptsArray;
extern RwIm2DVertex* const	aSpriteVertices;
extern RwCamera*&			Scene;
extern void					(*BigMessageRestOfFunc)();
extern void					(*TheScriptsInitFunc)();
extern char*				(__thiscall *CLEOGetTextFunc)(CText* object, const char* key);

extern CCRC32				HashingClass;
extern bool					InAmazingScreenshotMode;
//extern BYTE					bLastPedPrimaryColour, bLastPedSecondaryColour, bLastPedTertiaryColour, bLastPedQuaternaryColour;
#ifdef INCLUDE_MULTIFONTFILES
extern bool					bCanReloadFonts;
extern eFontFileIDs			bLastFontsID;
#endif

extern char					gString[552];
extern char					gUString[552];

/*#if DEBUG
extern CSprite2d				DevLogos[1];
#endif*/

//extern wchar_t			StatsHTMLLine[256];
extern const float		fWeaponIconWidth;
extern const float		fWeaponIconHeight;
extern const float		fWLStarPosX;
extern const float		fWLStarPosY;
extern const float		fWLStarHeight;
extern const float		fWLStarWidth;
extern const float		fWLStarDistance;