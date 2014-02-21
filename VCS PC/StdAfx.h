#pragma warning(disable:4481)	// nonstandard extension used: override specifier 'override'
#pragma warning(disable:4401)	// member is bit field
#pragma warning(disable:4733)	// handler not registered as safe handler
#pragma warning(disable:4725)	// instruction may be inaccurate on some Pentiums

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
#include <string>
#include <map>
#include <cassert>
#include "buildnumber.h"


//#define INCLUDE_AUDIO_DEBUG
#define INCLUDE_WINDOW_NAME
//#define INCLUDE_VCSIMG_LOADER
//#define INCLUDE_MULTIFONTFILES

#ifdef DEVBUILD
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
//#define V_RADAR_TEST
//#define ROTORS_TEST

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

#define MOD_VERSION	"v3.0 BETA"
#define VERSION_NAME "Blue Hesper"
#define VERSION_NAME_UPPERCASE "BLUE HESPER"
#define RELEASE_CANDIDATE "1"

#define	PlayerStatsInteger	((int*)0xB78E20)
#define	PlayerStatsFloat	((float*)0xB79380)


// Some handy funcs
void			LogToFile(const char* str, ...);
inline void		ToLower(char* str, BYTE len)
{
	do
	{
		if ( *str >= 'A' && *str <= 'Z' )
			*str += 0x20;
		++str;
		--len;
	}
	while ( len );
}

template<typename T>
inline T random(T a, T b)
{
	return a + static_cast<T>(rand() * (1.0f/(RAND_MAX+1)) * (b - a));
}

#ifdef MAKE_CONSOLE
#define echo(x, ...) printf(x, __VA_ARGS__)
#else
#define echo(x, ...)
#endif

#if defined DEVBUILD && !defined MAKE_ZZCOOL_MOVIE_DEMO
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
#include "General.h"
#include "WidescreenSupport.h"
//#include "ColAccel.h"
#include "Queue.h"
#include "BankLoader.h"
#include "Coronas.h"
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
#include "Ped.h"
#include "Stats.h"
#include "Darkel.h"
#include "CVehicle.h"
#include "PlayerInfo.h"
#include "World.h"
#include "CConfiscatedWeapons.h"
#include "Pools.h"
#include "Script.h"
#include "Streaming.h"
#include "Text.h"
#include "RwEngineInstance.h"
#include "Font.h"
#include "Clock.h"
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
#include "3DMarkers.h"
#include "CShadows.h"
#include "Audio.h"
#include "Replay.h"
#include "VideoPlayer.h"
#include "UserDisplay.h"
#include "Messages.h"
#include "Radar.h"
#include "EntryExitMgr.h"

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
extern bool*				bHideStyledTextWhileFading;
extern float*				currentFPS;
extern RsGlobalType&		RsGlobal;
extern CRGBA*				BaseColors;
extern CMenuManager&		FrontEndMenuManager;
extern CAudioEngine&		AudioEngine;
extern std::pair<void*,int>* const materialRestoreData;
//extern CGridref*			gridref;
extern CText&				TheText;
extern void**				rwengine;
extern CCamera&				TheCamera;
extern CGarages*			garages;
extern CKeyState*			activeKeyState;
extern CKeyState*			currKeyState;
extern CKeyState*			prevKeyState;
//extern CSprite2d*			texFonts;
extern CGangWeapons*		gangWeapons;
extern CRunningScript**		pActiveScripts;
extern CRunningScript*		ScriptsArray;
extern RwIm2DVertex* const	aSpriteVertices;
extern RwTexture** const	gpCoronaTexture;
extern RwCamera*&			Scene;
extern void					(*TheScriptsInitFunc)();
extern const char*	(__thiscall *CLEOGetTextFunc)(CText*, const char*);

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