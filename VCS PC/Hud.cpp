#include "StdAfx.h"
#include "Hud.h"

#include "Vehicle.h"
#include "Ped.h"
#include "Stats.h"
#include "World.h"
#include "Timer.h"
#include "UserDisplay.h"
#include "DLCManager.h"
#include "Font.h"
#include "Audio.h"
#include "Text.h"
#include "Darkel.h"
#include "Camera.h"
#include "Clock.h"
#include "Frontend.h"
#include "PlayerInfo.h"

bool					CHud::bShouldFPSBeDisplayed;
bool					CHud::bLCSPS2Style;
int						CHud::CarNameStage;
const char*				CHud::pCarNameToDisplay;
const char*				CHud::pLastCarName;
const char*				CHud::pCarName;
int						CHud::CarNameShowingTime;
int						CHud::CarNameFadingTime;
int						CHud::m_ZoneState;
int						CHud::m_ZoneFadeTimer;
int						CHud::m_ZoneNameTimer;
const char*				CHud::m_pZoneName;
const char*				CHud::m_pLastZoneName;
const char*				CHud::m_ZoneToPrint;
float					CHud::BigMessage2Alpha;
float					CHud::BigMessage3Alpha;
int&					CHud::m_HelpMessageState = *(int*)0xBAA474;
short&					CHud::m_ItemToFlash = *(short*)0xBAB1DC;
char					CHud::m_PagerMessage[16];
CSprite2d* const		CHud::Sprites = (CSprite2d*)0xBAB1FC;

static unsigned char	PagerOn;
static bool				PagerSoundPlayed;
static float			PagerXOffset;

static bool				CounterOnLastFrame[NUM_ONSCREEN_COUNTERS], PlaceOnLastFrame;
static unsigned short	CounterFlashTimer[NUM_ONSCREEN_COUNTERS], PlaceFlashTimer;

// Wrappers
WRAPPER void CHud::DrawBarChart(float fX, float fY, WORD wWidth, WORD wHeight, float fPercentage, BYTE drawBlueLine, BYTE drawPercentage, BYTE drawBorder, CRGBA dwColor, CRGBA dwForeColor)
{	WRAPARG(fX); WRAPARG(fY); WRAPARG(wWidth); WRAPARG(wHeight); WRAPARG(fPercentage); WRAPARG(drawBlueLine); WRAPARG(drawPercentage); WRAPARG(drawBorder); WRAPARG(dwColor); WRAPARG(dwForeColor);
	EAXJMP(0x728640); }
WRAPPER void CHud::DrawWeaponIcon(CPed* pPed, int iX, int iY, float fAlpha) { WRAPARG(pPed); WRAPARG(iX); WRAPARG(iY); WRAPARG(fAlpha); EAXJMP(0x58D7D0); }

static const char* const			HudSpriteFilenames[NUM_HUD_SPRITES] = { "fist", "siteM16", "siterocket", "radardisc", "barOutline", "pager" };

void CHud::GetRidOfAllCustomHUDMessages()
{
	pCarName = nullptr;
	pCarNameToDisplay = nullptr;
	CarNameShowingTime = 0;
	CarNameFadingTime = 0;
	CarNameStage = 0;

	m_ZoneNameTimer = 0;
	m_pZoneName = nullptr;
	m_ZoneState = 0;

	CUserDisplay::Pager.ClearMessages();
}

void CHud::Initialise()
{
	CPNGArchive		HudSPTA("models\\hud.spta");

	// Temp
	HudSPTA.SetDirectory("weapons");

	for ( int i = 0; i < NUM_HUD_SPRITES; ++i )
	{
		Sprites[i].SetTextureFromSPTA(HudSPTA, HudSpriteFilenames[i]);
		if ( i == 0 )
			HudSPTA.SetDirectory("overlays");
	}

	HudSPTA.CloseArchive();
	ReInitialise();
}

void CHud::ReInitialise()
{
	// Call SA ReInitialise function
	((void(*)())0x588880)();

#ifndef DEVBUILD
	bShouldFPSBeDisplayed = false;
#else
	bShouldFPSBeDisplayed = true;
#endif
	bLCSPS2Style = false;
//	bDrawDevLogos = false;

	CarNameStage = 0;
	pLastCarName = nullptr;
	pCarName = nullptr;
	CarNameShowingTime = 0;
	CarNameFadingTime = 0;

	m_ZoneState = 0;
	m_ZoneNameTimer = 0;
	m_pZoneName = nullptr;
	m_pLastZoneName = nullptr;

	PagerOn = 0;
	PagerSoundPlayed = false;
	PagerXOffset = 200.0f;

	for ( int i = 0; i < NUM_ONSCREEN_COUNTERS; ++i )
	{
		CounterOnLastFrame[i] = false;
		CounterFlashTimer[i] = 0;
	}

	PlaceOnLastFrame = false;
	PlaceFlashTimer = 0;
}

void CHud::Shutdown()
{
	for ( int i = 0; i < NUM_HUD_SPRITES; ++i )
		Sprites[i].Delete();
}

void CHud::SetVehicleName(char* carName)
{
	pCarName = carName;
}

void CHud::SetZoneName(char* zoneName, bool bIgnoreIntID)
{
	if ( (bIgnoreIntID || !*activeInterior && !m_ZoneState) && (!zoneName || strncmp(zoneName, "DUMMY", 5)) )
		m_pZoneName = zoneName;
}

void CHud::SetPagerMessage(char* pMsg)
{
	unsigned short	wLoopCounter = 0;
	while ( wLoopCounter < sizeof(m_PagerMessage) )
	{
		if ( !pMsg[wLoopCounter] )
			break;
		m_PagerMessage[wLoopCounter] = pMsg[wLoopCounter];
		++wLoopCounter;
	}

	m_PagerMessage[wLoopCounter] = '\0';
}

void CHud::DrawHUD()
{
	char	string[16];
	CPed*	playerPed = CWorld::Players[CWorld::PlayerInFocus].GetPed();
	CPed*	secondPlayerPed = CWorld::Players[1].GetPed();

	/*if ( playerPed->pVehicle )
		reinterpret_cast<CAutomobile*>(playerPed->pVehicle)->DebugWheelDisplay();*/

	if ( !CDLCManager::GetDLC(DLC_HALLOWEEN)->IsEnabled() )
	{
		// Clock
		CFont::SetBackground(0, 0);
		CFont::SetProportional(false);
		CFont::SetFontStyle(FONT_Hud);
		CFont::SetOrientation(ALIGN_Right);
		CFont::SetRightJustifyWrap(0.0);
		CFont::SetEdge(0);
		CFont::SetDropColor(CRGBA(0, 0, 0, HUD_TRANSPARENCY));
		if ( FrontEndMenuManager.GetHudMode() == 2 )
		{
			unsigned char		bHour = CClock::GetHours();

			if ( CClock::Convert24To12(bHour) )
				_snprintf(string, sizeof(string), "%d:%02d]", bHour, CClock::GetMinutes());
			else
				_snprintf(string, sizeof(string), "%d:%02d[", bHour, CClock::GetMinutes());
			CFont::SetScale(_width(0.36f), _height(0.95f));
		}
		else
		{
			_snprintf(string, sizeof(string), "%02d:%02d", CClock::GetHours(), CClock::GetMinutes());
			CFont::SetScale(_width(0.45f), _height(0.95f));
		}
		CFont::SetColor(CRGBA(BaseColors[14], HUD_TRANSPARENCY));
		CFont::PrintString(_x(HUD_POS_X - 53.0f), _y(HUD_POS_Y - 21.5f), string);
		//CFont::SetEdge(0);
	}

	// 1st player healthbar
	PrintHealthForPlayer(CWorld::PlayerInFocus, _x(HUD_POS_X), _y(HUD_POS_Y + 20.5f));

	// 2nd player healthbar
	if ( secondPlayerPed )
		PrintHealthForPlayer(1, _x(HUD_POS_X), _y(HUD_POS_Y + 20.5f + HUD_POS_Y_2P_OFFSET));

	BYTE		bDraw1stPlayerOxygenBar = 0;
	BYTE		bDraw2ndPlayerOxygenBar = 0;
	CVehicle*	pVehicle = nullptr;
	CVehicle*	p2ndVehicle = nullptr;

	if( playerPed->GetPedFlags().bIsStanding && playerPed->GetPedFlags().bInVehicle )
		pVehicle = playerPed->GetVehiclePtr();

	if( secondPlayerPed && secondPlayerPed->GetPedFlags().bIsStanding && secondPlayerPed->GetPedFlags().bInVehicle )
		p2ndVehicle = secondPlayerPed->GetVehiclePtr();

	if (	playerPed->GetPedIntelligencePtr()->GetTaskSwim() ||
			(pVehicle && pVehicle->GetVehicleFlags().bIsDrowning) ||
			CStats::CalcPlayerStat(8) > CWorld::Players[CWorld::PlayerInFocus].GetPlayerData().m_fBreath )
	{
		bDraw1stPlayerOxygenBar = 1;
	}

	if ( secondPlayerPed )
	{
		if (	secondPlayerPed->GetPedIntelligencePtr()->GetTaskSwim() ||
				p2ndVehicle &&
				p2ndVehicle->GetVehicleFlags().bIsDrowning ||
				CStats::CalcPlayerStat(8) > CWorld::Players[1].GetPlayerData().m_fBreath )
		{
			bDraw2ndPlayerOxygenBar = 1;
		}
	}

	// 1st player armourbar
	if ( !bDraw1stPlayerOxygenBar )
		PrintArmourForPlayer(CWorld::PlayerInFocus, _x(HUD_POS_X), _y(HUD_POS_Y));
	else
		PrintBreathForPlayer(CWorld::PlayerInFocus, _x(HUD_POS_X), _y(HUD_POS_Y));

	// 2nd player armourbar
	if ( secondPlayerPed )
	{
		if ( !bDraw2ndPlayerOxygenBar )
			PrintArmourForPlayer(1, _x(HUD_POS_X), _y(HUD_POS_Y + HUD_POS_Y_2P_OFFSET));
		else
			PrintBreathForPlayer(1, _x(HUD_POS_X), _y(HUD_POS_Y + HUD_POS_Y_2P_OFFSET));
	}

	// Money
	int			displayedScore = CWorld::Players[CWorld::PlayerInFocus].GetDisplayedScore();
	const char*	moneyText;

	if ( displayedScore < 0 )
	{
		CFont::SetColor(CRGBA(BaseColors[0], HUD_TRANSPARENCY));
		displayedScore = -displayedScore;
		moneyText = "-$%07d";
	}
	else
	{
		CFont::SetColor(CRGBA(BaseColors[1], HUD_TRANSPARENCY));
		moneyText = "$%08d";
	}
	_snprintf(string, sizeof(string), moneyText, displayedScore);
	CFont::SetProportional(false);
	CFont::SetBackground(0, 0);
	CFont::SetScale(_width(0.53f), _height(0.95f));
	CFont::SetOrientation(ALIGN_Right);
	CFont::SetRightJustifyWrap(0.0);
	CFont::SetFontStyle(FONT_Hud);
	CFont::SetEdge(0);
	CFont::SetDropColor(CRGBA(0, 0, 0, HUD_TRANSPARENCY));

	CFont::PrintString(_x(HUD_POS_X - 115.5f), _y(HUD_POS_Y + 37.5f), string);

	// 1st player weapon icon
	DrawWeaponIcon(playerPed, _x(HUD_POS_X - 58.5f), _y(HUD_POS_Y - 21.0f));

	// 1st player weapon ammo
	DrawWeaponAmmo(playerPed, _x(HUD_POS_X - 89.5f), _y(HUD_POS_Y + 24.0f));

	// 2nd player weapon icon
	if ( secondPlayerPed )
	{
		DrawWeaponIcon(secondPlayerPed, _x(HUD_POS_X - 58.5f), _y(HUD_POS_Y - 21.0f + HUD_POS_Y_2P_OFFSET));

		// 2nd player weapon ammo
		DrawWeaponAmmo(secondPlayerPed, _x(HUD_POS_X - 89.5f), _y(HUD_POS_Y + 24.0 + HUD_POS_Y_2P_OFFSET));
	}

	// Pager
	if ( !m_PagerMessage[0] )
	{
		if ( PagerOn == 1 )
		{
			PagerSoundPlayed = false;
			PagerOn = 2;
		}
	}
	if ( m_PagerMessage[0] || PagerOn == 2 )
	{
		if ( !PagerOn )
		{
			PagerOn = 1;
			PagerXOffset = 200.0f;
		}
		if ( PagerOn == 1 )
		{
			if ( PagerXOffset > 0.0f )
			{
				float	fStep = PagerXOffset * 0.06f;
				if ( fStep > 6.0f )
					fStep = 6.0f;
				PagerXOffset -= fStep * CTimer::ms_fTimeStep;
			}
			if ( !PagerSoundPlayed )
			{
				AudioEngine.ReportFrontendAudioEvent(46, 0.0f, 1.0f);
				PagerSoundPlayed = true;
			}
		}
		else
		{
			if ( PagerOn == 2 )
			{
				float	fStep = PagerXOffset * 0.06f;
				if ( fStep < 1.2f )
					fStep = 1.2f;
				PagerXOffset += fStep * CTimer::ms_fTimeStep;
				if ( PagerXOffset > 200.0f )
				{
					PagerOn = 0;
					PagerXOffset = 200.0f;
				}
			}
		}

		Sprites[HUD_Pager].Draw(CRect(_xleft(32.5f - PagerXOffset), _y(117.5f), _xleft(202.5f - PagerXOffset), _y(32.5f)), CRGBA(255, 255, 255, 255));

		CFont::SetProportional(false);
		CFont::SetScale(_width(0.5f), _height(1.4f));
		CFont::SetOrientation(ALIGN_Left);
		CFont::SetColor(CRGBA(0, 0, 0, 255));
		CFont::SetFontStyle(FONT_PagerFont);
		CFont::PrintString(_xleft(50.0f - PagerXOffset), _y(47.5f), m_PagerMessage);
	}
}

void CHud::DrawWanted()
{
	int		nWantedLevel = FindPlayerWanted(-1)->GetWantedLevel();
	int		nWantedLevelParole = FindPlayerWanted(-1)->GetWantedLevelBeforeParole();
	int		nTimeOfWLChange = FindPlayerWanted(-1)->GetTimeWantedLevelChanged();

	if ( nWantedLevel > 0 || nWantedLevelParole > 0 )
	{
		//CFont::SetEdge(1);
		CFont::SetOrientation(ALIGN_Right);
		CFont::SetProportional(false);
		CFont::SetFontStyle(FONT_Hud);
		CFont::SetScale(_width(0.95f), _height(1.5f));

		float		fCurrentPos = _x(HUD_POS_X - 115.25f);
		for ( int i = 0; i < 6; ++i, fCurrentPos -= _width(20.0f) )
		{
			if ( nWantedLevel > i && ( CTimer::m_snTimeInMilliseconds > nTimeOfWLChange + 2000 || ShowFlashingItem(150, 150) /*CTimer::m_FrameCounter & 4*/ ) )
			{
				CFont::SetColor(CRGBA(BaseColors[6], HUD_TRANSPARENCY));
				CFont::PrintString(fCurrentPos, _y(HUD_POS_Y + 53.0f), "\\");
			}
			else if ( nWantedLevelParole > i && ShowFlashingItem(150, 150)/*CTimer::m_FrameCounter & 4*/ )
			{
				CFont::SetColor(CRGBA(BaseColors[12], HUD_TRANSPARENCY));
				CFont::PrintString(fCurrentPos, _y(HUD_POS_Y + 53.0f), "\\");
			}
		}

	}
}

void CHud::DrawOnscreenTimer()
{
	if ( CUserDisplay::OnscnTimer.IsEnabled() && !StyledText_5[0] && !Garage_MessageIDString[0] )
	{
		CFont::SetBackground(0, 0);
		CFont::SetScale(_width(0.5f), _height(1.0f));
		CFont::SetOrientation(ALIGN_Right);
		CFont::SetFontStyle(FONT_Eurostile);
		CFont::SetDropShadowPosition(1);

		static DWORD			dwTicksCounter = 0;
		static int				nCounterTimer = 0;
		bool					bRecalculateStuffThisTick;

		if ( CTimer::m_snTimeInMillisecondsPauseMode - nCounterTimer > 40 )	// Cap it at 25 FPS
		{
			bRecalculateStuffThisTick = true;
			++dwTicksCounter;
			nCounterTimer = CTimer::m_snTimeInMillisecondsPauseMode;
		}
		else
			bRecalculateStuffThisTick = false;

		COnscreenCounterEntry*	pCounters = CUserDisplay::OnscnTimer.GetCounter();
		COnscreenTimerEntry*	pTimer = CUserDisplay::OnscnTimer.GetTimer();
		unsigned char			bChildrenEntries = 0;

		for ( int i = 0; i < NUM_ONSCREEN_COUNTERS; ++i )
		{
			if ( !pCounters[i].m_bShown || pCounters[i].m_bForceRecheck )
			{
				pCounters[i].m_bForceRecheck = false;
				CounterOnLastFrame[i] = false;
			}

			if ( pCounters[i].m_bShown )
			{
				if ( pCounters[i].m_wOwnsWhat != 0xFFFF )
					bChildrenEntries += 3;
			}
		}

		if ( pTimer->m_bShown )
		{
			int		nRemainder = *pTimer->m_pVariable % 1000;
			if ( !pTimer->m_bFlashing || (*pTimer->m_pVariable < 750 || ( nRemainder < 750 && nRemainder > 250 )) )
			{
				CFont::SetProportional(false);
				CFont::SetDropColor(pTimer->m_shadowColour);
				CFont::SetColor(pTimer->m_colour);

				CFont::PrintString(_x(32.0f), _y(bChildrenEntries * 20.0f + 148.0f), pTimer->m_cDisplayedText);
				if ( pTimer->m_cGXTentry[0] )
				{
					float		fPosXOffset;
					int			nTextLength = strlen(pTimer->m_cDisplayedText);
					if ( nTextLength > 4 )
						fPosXOffset = 12.5f * (nTextLength - 4);
					else
						fPosXOffset = 0.0f;

					CFont::SetProportional(true);
					CFont::PrintString(_x(87.5f + fPosXOffset), _y(bChildrenEntries * 20.0f + 148.0f), TheText.Get(pTimer->m_cGXTentry));
				}
			}
		}

		for ( int i = 0; i < NUM_ONSCREEN_COUNTERS; ++i )
		{
			if ( pCounters[i].m_bShown )
			{
				if ( !CounterOnLastFrame[i] && pCounters[i].m_bFlashing )
					CounterFlashTimer[i] = 1;

				CounterOnLastFrame[i] = true;

				/*if ( CounterFlashTimer[i] )
				{
					if ( ++CounterFlashTimer[i] > 50 )
						CounterFlashTimer[i] = 0;
				}*/

				if ( CounterFlashTimer[i] )
				{
					if ( bRecalculateStuffThisTick )
					{
						if ( ++CounterFlashTimer[i] > 50 )
							CounterFlashTimer[i] = 0;
					}
				}

				if ( dwTicksCounter & 4 || !CounterFlashTimer[i] )
				{
					CFont::SetDropColor(pCounters[i].m_shadowColour);
					CFont::SetColor(pCounters[i].m_colour);

					if ( pCounters[i].m_wType != 3 )
					{
						if ( pCounters[i].m_wType == 1 )
							DrawBarChart(	_x(93.0f), _y(i * 20.0f + 173.0f), _width(61.0f),
											_height(18.0), pCounters[i].m_counterData.nBarFill,
											0, 0, 1, pCounters[i].m_colour,
											CRGBA(0, 0, 0, 0));
						else
						{
							CFont::SetProportional(false);
							CFont::PrintString(_x(32.0f), _y(i * 20.0f + 173.0f), pCounters[i].m_counterData.cDisplayedText);
						}
					}

					if ( pCounters[i].m_cGXTentry[0] )
					{
						float		fPosXOffset;
						int			nTextLength = pCounters[i].m_wType == 1 ? 5 : strlen(pCounters[i].m_counterData.cDisplayedText);
						if ( nTextLength > 4 )
							fPosXOffset = 12.5f * (nTextLength - 4);
						else
							fPosXOffset = 0.0f;

						CFont::SetProportional(true);
						if ( pCounters[i].m_wType != 3 )
							CFont::PrintString(_x(87.5f + fPosXOffset), _y(i * 20.0f + 173.0f), TheText.Get(pCounters[i].m_cGXTentry));
						else
							CFont::PrintString(_x(32.0f), _y(i * 20.0f + 160.0f), TheText.Get(pCounters[i].m_cGXTentry));
					}
				}
			}
		}
	}
}

void CHud::DrawPermanentTexts()
{
#if !defined NDEBUG && !defined SKIP_DEBUG_TEXTS
#if !defined COMPILE_RC && !defined MAKE_ZZCOOL_MOVIE_DEMO
	if ( bShouldFPSBeDisplayed && !InAmazingScreenshotMode )
	{
		char		debugText[64];
		if ( !CUpdateManager::AnyTextDisplaysNow() )
		{
			_snprintf(debugText, sizeof(debugText), "%d FPS", static_cast<int>(*currentFPS));
			CFont::SetBackground(0, 0);
			CFont::SetProportional(false);
			CFont::SetFontStyle(FONT_Eurostile);
			CFont::SetOrientation(ALIGN_Right);
			CFont::SetEdge(1);
			CFont::SetDropColor(CRGBA(0, 0, 0, 255));
			CFont::SetScale(_width(0.35f), _height(0.65f));
			CFont::SetColor(CRGBA(BaseColors[*currentFPS > 10.0f]));
			CFont::PrintString(_x(15.0f), _y(3.5f), debugText);
		}

#if defined INCLUDE_STREAMING_TEXT
		long double	percentUsage = ((long double)(*memoryUsed) / * memoryAvailable) * 100.0;

		sprintf(debugText, "STREAMING %dKB/%dKB (%d%% USED)", *memoryUsed / 1024, *memoryAvailable / 1024, (int)percentUsage);
		CFont::SetScale(_width(0.35f), _height(0.65f));
		CFont::SetProportional(true);
		CFont::SetOrientation(ALIGN_Left);
		CFont::SetColor(CRGBA(DEBUG_ORANGE_R, DEBUG_ORANGE_G, DEBUG_ORANGE_B));
		CFont::PrintString(_xleft(5.0f), _y(3.5f), debugText);

		sprintf(debugText, "MODELS REQUESTED: %u", *(DWORD*)0x8E4CB8);
		CFont::PrintString(_xleft(5.0f), _y(13.5f), debugText);
#elif defined SHOW_FOV_FANCY_RHYME
		sprintf(debugText, "FOV: %d", static_cast<int>(TheCamera.Cams[TheCamera.ActiveCam].FOV));
		CFont::SetScale(_width(0.35f), _height(0.65f));
		CFont::SetProportional(true);
		CFont::SetOrientation(ALIGN_Left);
		CFont::SetColor(CRGBA(DEBUG_ORANGE_R, DEBUG_ORANGE_G, DEBUG_ORANGE_B));
		CFont::PrintString(_xleft(10.0f), _y(3.5f), debugText);
#endif

		if ( CPed* pPlayerPed = CWorld::Players[0].GetPed() )
		{
			CVector&	coords = pPlayerPed->GetCoords();

			_snprintf(debugText, sizeof(debugText), "%.3f %.3f %.3f", coords.x, coords.y, coords.z);
			CFont::SetProportional(true);
			CFont::SetOrientation(ALIGN_Right);
			CFont::SetColor(CRGBA(0x0A, 0x57, 0x82));
			CFont::SetBackground(0, 0);
			CFont::SetFontStyle(FONT_Eurostile);
			CFont::SetEdge(1);
			CFont::SetDropColor(CRGBA(0, 0, 0, 255));
			CFont::SetScale(_width(0.35f), _height(0.65f));
			CFont::PrintString(_x(5.0f), _ydown(11.0f), debugText);
		}
		CFont::SetEdge(0);
	}
#else
	if ( !CUpdateManager::AnyTextDisplaysNow() )
	{
		CFont::SetBackground(0, 0);
		CFont::SetProportional(true);
		CFont::SetFontStyle(FONT_Eurostile);
		CFont::SetOrientation(ALIGN_Right);
		CFont::SetEdge(1);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetScale(_width(0.75f), _height(1.15f));
		CFont::SetColor(CRGBA(255, 255, 255));
		CFont::PrintString(_x(25.0), _y(3.0), "PRE-BETA3 BUILD");
	}
#endif
#ifdef DRAW_OVERSCAN_720P
	CSprite2d::DrawRect(CRect(-5.0f, -5.0f, 1285.0f, 36.0f), CRGBA(0, 0, 255, 100));
	CSprite2d::DrawRect(CRect(-5.0f, 720.0f - 36.0f, 1285.0f, 725.0f), CRGBA(0, 0, 255, 100));
	CSprite2d::DrawRect(CRect(-5.0f, -5.0f, 128.0f, 725.0f), CRGBA(0, 0, 255, 100));
	CSprite2d::DrawRect(CRect(1280.0f - 128.0f, -5.0f, 1285.0f, 725.0f), CRGBA(0, 0, 255, 100));

	CSprite2d::DrawRect(CRect(-5.0f, -5.0f, 1285.0f, 25.2f), CRGBA(255, 0, 0, 255));
	CSprite2d::DrawRect(CRect(-5.0f, 720.0f - 25.2f, 1285.0f, 725.0f), CRGBA(255, 0, 0, 255));
	CSprite2d::DrawRect(CRect(-5.0f, -5.0f, 44.8f, 725.0f), CRGBA(255, 0, 0, 255));
	CSprite2d::DrawRect(CRect(1280.0f - 44.8f, -5.0f, 1285.0f, 725.0f), CRGBA(255, 0, 0, 255));
#endif
#endif
	CUpdateManager::Display();
}

void CHud::PrintHealthForPlayer(int playerID, float posX, float posY)
{
	if ( m_ItemToFlash != FLASH_Healthbar || ShowFlashingItem(300, 300)/*CTimer::m_FrameCounter & 8*/ )
	{
		if ( CWorld::Players[playerID].GetLastTimeArmourLost() == CWorld::Players[playerID].GetLastTimeEnergyLost() || CWorld::Players[playerID].GetLastTimeEnergyLost() + BAR_ENERGY_LOSS_FLASH_DURATION < CTimer::m_snTimeInMilliseconds || ShowFlashingItem(150, 150)/*CTimer::m_FrameCounter & 4*/ )
		{
			if ( CWorld::Players[playerID].GetPed()->GetHealth() >= 10 || ShowFlashingItem(300, 300)/*CTimer::m_FrameCounter & 8*/ )
			{
				if ( bLCSPS2Style )
					DrawBarChart(	posX, posY, _width(53.0f), _height(14.5f),
									CWorld::Players[playerID].GetPed()->GetHealth() / CWorld::Players[playerID].GetMaxHealth() * 100.0,
									0, 0, 1, CRGBA(BaseColors[9], HUD_TRANSPARENCY), CRGBA(0, 0, 0, 0) );
				else
					DrawBarChartWithRoundBorder(	posX, posY, _width(53.0f), _height(14.5f),
									CWorld::Players[playerID].GetPed()->GetHealth() / CWorld::Players[playerID].GetMaxHealth() * 100.0,
									0, 0, 1, CRGBA(BaseColors[9], HUD_TRANSPARENCY), CRGBA(0, 0, 0, 0) );
			}
		}
	}
}

void CHud::PrintArmourForPlayer(int playerID, float posX, float posY)
{
	if ( m_ItemToFlash != FLASH_Armourbar || ShowFlashingItem(300, 300)/*CTimer::m_FrameCounter & 8*/ )
	{
		if ( CWorld::Players[playerID].GetLastTimeArmourLost() == 0 || CWorld::Players[playerID].GetLastTimeArmourLost() + BAR_ENERGY_LOSS_FLASH_DURATION < CTimer::m_snTimeInMilliseconds || ShowFlashingItem(150, 150)/*CTimer::m_FrameCounter & 4*/ )
		{
			if ( CWorld::Players[playerID].GetPed()->GetArmour() >= 1.0 )
			{
				if ( bLCSPS2Style )
					DrawBarChart(	posX, posY, _width(53.0f), _height(14.5f),
									CWorld::Players[playerID].GetPed()->GetArmour() / CWorld::Players[playerID].GetMaxArmour() * 100.0,
									0, 0, 1, CRGBA(0x09, 0xFF, 0xFF, HUD_TRANSPARENCY), CRGBA(0, 0, 0, 0) );
				else
					DrawBarChartWithRoundBorder(	posX, posY, _width(53.0f), _height(14.5f),
									CWorld::Players[playerID].GetPed()->GetArmour() / CWorld::Players[playerID].GetMaxArmour() * 100.0,
									0, 0, 1, CRGBA(0x09, 0xFF, 0xFF, HUD_TRANSPARENCY), CRGBA(0, 0, 0, 0) );
			}
		}
	}
}

void CHud::PrintBreathForPlayer(int playerID, float posX, float posY)
{
	if ( m_ItemToFlash != FLASH_Breathbar || ShowFlashingItem(300, 300)/*CTimer::m_FrameCounter & 8*/ )
	{
		if ( bLCSPS2Style )
			DrawBarChart(	posX, posY, _width(53.0f), _height(14.5f),
								CWorld::Players[playerID].GetPed()->GetPlayerData()->m_fBreath / CStats::CalcPlayerStat(8) * 100.0,
								0, 0, 1, CRGBA(BaseColors[13], HUD_TRANSPARENCY), CRGBA(0, 0, 0, 0) );
		else
			DrawBarChartWithRoundBorder(	posX, posY, _width(53.0f), _height(14.5f),
								CWorld::Players[playerID].GetPed()->GetPlayerData()->m_fBreath / CStats::CalcPlayerStat(8) * 100.0,
								0, 0, 1, CRGBA(BaseColors[13], HUD_TRANSPARENCY), CRGBA(0, 0, 0, 0) );
	}
}

void CHud::DrawWeaponAmmo(CPed* ped, float fX, float fY)
{
	char	AmmoText[16];

	int		weapAmmo = ped->GetWeaponSlots()[ped->GetActiveWeapon()].m_nAmmoTotal;
	eWeaponType		weapType = ped->GetWeaponSlots()[ped->GetActiveWeapon()].m_eWeaponType;
	int		weapAmmoInClip = ped->GetWeaponSlots()[ped->GetActiveWeapon()].m_nAmmoInClip;

	short	clipSize = CWeaponInfo::GetWeaponInfo(weapType, ped->GetWeaponSkill())->GetClipSize();

	if ( clipSize <= 1 || clipSize >= 1000 )
		sprintf(AmmoText, "%d", weapAmmo);
	else
	{
		int		ammoClipToShow;
		int		ammoRestToShow;

		if ( weapType == 37 )
		{
			int tempAmmoValue = 9999;
			if ( ( weapAmmo - weapAmmoInClip ) / 10 <= 9999 )
				tempAmmoValue = ( weapAmmo - weapAmmoInClip ) / 10;

			ammoClipToShow = weapAmmoInClip / 10;
			ammoRestToShow = tempAmmoValue;
		}
		else
		{
			int tempAmmoValue = weapAmmo - weapAmmoInClip;
			if ( weapAmmo - weapAmmoInClip > 9999 )
				tempAmmoValue = 9999;

			ammoClipToShow = weapAmmoInClip;
			ammoRestToShow = tempAmmoValue;
		}
		sprintf(AmmoText, "%d-%d", ammoRestToShow, ammoClipToShow);
	}
	CFont::SetBackground(0, 0);
	CFont::SetScale(_width(0.25f), _height(0.58f));
	CFont::SetOrientation(ALIGN_Center);
	CFont::SetCentreSize(RsGlobal.MaximumWidth);
	CFont::SetProportional(false);
	CFont::SetEdge(0);
	CFont::SetDropColor(CRGBA(0, 0, 0, 255));
	if ( bLCSPS2Style )
		CFont::SetFontStyle(FONT_Eurostile);
	else
		CFont::SetFontStyle(FONT_PagerFont);

	if (	weapAmmo - weapAmmoInClip < 9999
		&& !CDarkel::FrenzyOnGoing()
		&& weapType != 40
		&& ( weapType < 10 || weapType >= 16 )
		&& weapType != 46
		&& CWeaponInfo::GetWeaponInfo(weapType, 1)->GetWeaponType() != 5
		&& CWeaponInfo::GetWeaponInfo(weapType, 1)->GetWeaponSlot() > 1 )
	{
		CFont::SetColor(BaseColors[4]);
		CFont::PrintString(fX, fY, AmmoText);
	}
}

void CHud::DrawRadioName(void* object, const char* radioName)
{
	CFont::SetFontStyle(FONT_RageItalic);
	CFont::SetJustify(false);
	CFont::SetBackground(0, 0);
	CFont::SetScale(_width(0.75f), _height(1.25f));
	CFont::SetProportional(true);
	CFont::SetOrientation(ALIGN_Center);
	//CFont::SetRightJustifyWrap(0.0f);
	CFont::SetDropShadowPosition(1);
	CFont::SetDropColor(CRGBA(0, 0, 0, 255));

	CFont::SetColor(BaseColors[*((DWORD*)object + 27) || *((DWORD*)object + 28) ? 12 : 2]);
	CFont::PrintString(RsGlobal.MaximumWidth * 0.5f, _y(13.0f), radioName);
	CFont::RenderFontBuffer();
}

void CHud::DrawVehicleName()
{
	if ( pCarName )
	{
		int			tmpStage = CarNameStage;
		const char* tmpCarName = pLastCarName;
		float		alpha = 0.0;

		if ( pCarName != pLastCarName )
		{
			if ( tmpStage )
			{
				if ( tmpStage <= 4 )
				{
					tmpStage = 4;
					CarNameStage = 4;
					CarNameShowingTime = 0;
				}
			}
			else
			{
				tmpStage = 1;
				CarNameStage = 1;
				CarNameShowingTime = 0;
				pCarNameToDisplay = pCarName;
			}
			tmpCarName = pCarName;
			pLastCarName = pCarName;
		}

		if ( tmpStage )
		{
			switch ( tmpStage )
			{
				case 2:
					CarNameFadingTime += CTimer::ms_fTimeStep * 0.02 * 1000.0;
					if ( CarNameFadingTime > 1000 )
					{
						CarNameFadingTime = 1000;
						CarNameStage = 1;
					}
					alpha = CarNameFadingTime * 0.001 * 255.0;
					break;

				case 3:
					CarNameFadingTime += CTimer::ms_fTimeStep * 0.02 * -1000.0;
					if ( CarNameFadingTime < 0 )
					{
						CarNameFadingTime = 0;
						CarNameStage = 0;
					}
					alpha = CarNameFadingTime * 0.001 * 255.0;
					break;

				case 4:
					CarNameFadingTime += CTimer::ms_fTimeStep * 0.02 * -1000.0;
					if ( CarNameFadingTime < 0 )
					{
						CarNameShowingTime = 0;
						CarNameStage = 2;
						pCarNameToDisplay = tmpCarName;
						CarNameFadingTime = 0;
					}
					alpha = CarNameFadingTime * 0.001 * 255.0;
					break;

				case 1:
					if ( CarNameShowingTime > 10000 )
					{
						CarNameStage = 3;
						CarNameFadingTime = 1000;
					}
					alpha = 255.0;
					break;

				default:
					break;
			}
		}
		if ( !PriorityText[0] )
		{
			CarNameShowingTime += CTimer::ms_fTimeStep * 0.02 * 1000.0;

			CFont::SetProportional(true);
			CFont::SetOrientation(ALIGN_Right);
			CFont::SetFontStyle(FONT_RageItalic);
			CFont::SetScaleLang(_width(1.0f), _height(1.5f));
			CFont::SetDropShadowPosition(2);
#ifdef COMPILE_SLANTED_TEXT
#pragma message ("INFO: Compiling slanted text...")
			CFont::SetSlantRefPoint(RsGlobal.MaximumWidth, _ydown(108.0));
			CFont::SetSlant(0.15);
#endif
			CFont::SetDropColor(CRGBA(0, 0, 0, (BYTE)alpha));
			CFont::SetColor(CRGBA(BaseColors[4], (BYTE)alpha));
			if( *bWants_To_Draw_Hud && alpha )
				CFont::PrintString(_x(42.5f), _ydown(69.0f), pCarNameToDisplay);
#ifdef COMPILE_SLANTED_TEXT
			CFont::SetSlant(0.0);
#endif
		}
	}
	else
	{
		CarNameStage = 0;
		CarNameShowingTime = 0;
		CarNameFadingTime = 0;
		pLastCarName = nullptr;
	}
}

void CHud::DrawAreaName()
{
	if ( m_pZoneName )
	{
		int tmpStage = m_ZoneState;
		float alpha = 0.0;

		if ( m_pZoneName != m_pLastZoneName )
		{
			switch ( m_ZoneState )
			{
				case 0:
					if( !*radarGrey && *bWants_To_Draw_Hud || *PlayerEnexEntryStage == 1 || *PlayerEnexEntryStage == 2)
					{
						tmpStage = 2;
						m_ZoneState = 2;
						m_ZoneNameTimer = 0;
						m_ZoneFadeTimer = 0;
						m_ZoneToPrint = m_pZoneName;
					}
					break;
				case 1:
				case 2:
				case 3:
					tmpStage = 4;
					m_ZoneState = 4;
				case 4:
					m_ZoneNameTimer = 0;
					break;
				default:
					break;
			}
			m_pLastZoneName = m_pZoneName;
		}

		alpha = 255.0;

		if ( tmpStage )
		{
			switch ( tmpStage )
			{
				case 1:
					alpha = 255.0;
					m_ZoneFadeTimer = 1000;

					if( m_ZoneNameTimer > 10000 )
					{
						m_ZoneState = 3;
						m_ZoneFadeTimer = 1000;
					}
					break;
				case 2:
					if ( !TheCamera.GetFading() )
					{
						if ( TheCamera.GetFadeStage() != 2 )
							m_ZoneFadeTimer += CTimer::ms_fTimeStep * 0.02 * 1000.0;
					}
					if ( m_ZoneFadeTimer > 1000 )
					{
						m_ZoneFadeTimer = 1000;
						m_ZoneState = 1;
					}
					if ( TheCamera.GetFadeStage() != 2 )
						alpha = m_ZoneFadeTimer * 0.001 * 255.0;
					else
					{
						alpha = 255.0;
						m_ZoneState = 3;
						m_ZoneFadeTimer = 1000;
					}
					break;
				case 3:
					if ( !TheCamera.GetFading() )
					{
						if ( TheCamera.GetFadeStage() != 2 )
							m_ZoneFadeTimer += CTimer::ms_fTimeStep * 0.02 * -1000.0;
					}
					if ( m_ZoneFadeTimer < 0 )
					{
						m_ZoneFadeTimer = 0;
						m_ZoneState = 0;
					}
					if ( TheCamera.GetFadeStage() != 2 )
						alpha = m_ZoneFadeTimer * 0.001 * 255.0;
					else
					{
						m_ZoneFadeTimer = 1000;
						alpha = 255.0;
					}
					break;
				case 4:
					m_ZoneFadeTimer += CTimer::ms_fTimeStep * 0.02 * -1000.0;

					if( m_ZoneFadeTimer < 0 )
					{
						m_ZoneFadeTimer = 0;
						m_ZoneState = 2;
						m_ZoneToPrint = m_pZoneName;
					}

					alpha = m_ZoneFadeTimer * 0.001 * 255.0;
					break;
			}
			if( PriorityText[0] || *(float*)0xBAA3E0 != 0.0 || *(float*)0xBAA3E4 != 0.0 )
			{
				m_ZoneState = 3;
			}
			else
			{
				m_ZoneNameTimer += CTimer::ms_fTimeStep * 0.02 * 1000.0;
				CFont::SetProportional(true);
				CFont::SetOrientation(ALIGN_Right);
				CFont::SetFontStyle(FONT_RageItalic);
				CFont::SetDropShadowPosition(2);
#ifdef COMPILE_SLANTED_TEXT
				CFont::SetSlantRefPoint(RsGlobal.MaximumWidth, _ydown(88.0));
				CFont::SetSlant(0.15);
#endif
				CFont::SetDropColor(CRGBA(0, 0, 0, alpha));
				CFont::SetScale(_width(1.2f), _height(1.7f));
				CFont::SetColor(CRGBA(BaseColors[4], alpha));
				CFont::PrintStringFromBottom(_x(42.5f), _ydown(19.0f), m_ZoneToPrint);
				CFont::SetDropShadowPosition(0);
#ifdef COMPILE_SLANTED_TEXT
				CFont::SetSlant(0.0);
#endif
			}
		}
	}
}

void CHud::DrawBigMessage(bool bHideOnFade)
{
	if ( bHideOnFade == *bHideStyledTextWhileFading )
	{
		// Big message 5
		if ( !StyledText_2[0] && StyledText_5[0] )
		{
		}
	}

	if ( bHideOnFade )
	{
		// Styled text 7
		if ( StyledText_7[0] )
		{
			CFont::SetBackground(0, 0);
			CFont::SetJustify(false);
			CFont::SetScaleLang(_width(0.6), _height(1.1));
			CFont::SetOrientation(ALIGN_Center);
			CFont::SetProportional(true);
			CFont::SetCentreSize(_width(RsGlobal.MaximumWidth - 140.0f));
			CFont::SetFontStyle(FONT_Eurostile);
			CFont::SetEdge(1);
			CFont::SetDropColor(CRGBA(0, 0, 0, 255));
			CFont::SetColor(BaseColors[9]);
			CFont::PrintString(RsGlobal.MaximumWidth / 2, _ymiddle(26.0), StyledText_7);
		}

		// Styled text 4
		if ( StyledText_4[0] )
		{
			CFont::SetBackground(0, 0);
			CFont::SetJustify(false);
			CFont::SetScaleLang(_width(0.6f), _height(1.35f));
			CFont::SetOrientation(ALIGN_Center);
			CFont::SetProportional(true);
			CFont::SetCentreSize(_width(RsGlobal.MaximumWidth - 140.0f));
			CFont::SetFontStyle(FONT_Eurostile);
			CFont::SetEdge(1);
			CFont::SetDropColor(CRGBA(0, 0, 0, 255));
			CFont::SetColor(BaseColors[6]);
			CFont::PrintString(RsGlobal.MaximumWidth / 2, _ydown(22.5f + WidescreenSupport::GetTextPosition())/*_ydown(103.0)*/, StyledText_4);
		}
		// Dirty
		((void(*)())0x58CFA8)();
	}
}

void CHud::DrawBigMessage1()
{
	CFont::SetBackground(0, 0);
	CFont::SetJustify(false);
	CFont::SetScaleLang(_width(1.3f), _height(1.8f));
	CFont::SetOrientation(ALIGN_Center);
	CFont::SetProportional(true);
	CFont::SetCentreSize(_width(RsGlobal.MaximumWidth - 50.0f));
	CFont::SetFontStyle(FONT_Pricedown);
	CFont::SetDropShadowPosition(3);
}

void CHud::DrawBigMessage2()
{
	CFont::SetBackground(0, 0);
	CFont::SetProportional(true);
	CFont::SetRightJustifyWrap(0.0);
	CFont::SetOrientation(ALIGN_Right);
	CFont::SetFontStyle(FONT_RageItalic);
	CFont::SetScale(_width(1.0f), _height(1.2f));
	CFont::SetDropShadowPosition(2);
	CFont::SetDropColor(CRGBA(0, 0, 0, BigMessage2Alpha));
	CFont::SetColor(CRGBA(0xDA, 0xA8, 0x02, BigMessage2Alpha));
	CFont::PrintStringFromBottom(_x(42.5f), _ydown(24.0f), StyledText_2);
}


void CHud::DrawBigMessage3()
{
	CFont::SetBackground(0, 0);
	CFont::SetScale(_width(1.3), _height(1.8));
	CFont::SetProportional(true);
	CFont::SetJustify(false);
	CFont::SetOrientation(ALIGN_Center);
	CFont::SetFontStyle(FONT_Pricedown);
	CFont::SetDropShadowPosition(3);
	CFont::SetDropColor(CRGBA(0, 0, 0, BigMessage3Alpha));
	CFont::SetColor(CRGBA(BaseColors[10], BigMessage3Alpha));
	CFont::PrintStringFromBottom(RsGlobal.MaximumWidth / 2, _ymiddle(-30.0), StyledText_3);
}

/*void CHud::DrawDevLogos()
{
#if DEBUG
	if ( !bDrawDevLogos )
		return;

	DevLogos[0].DrawTexturedRect(&CRect(
											_x(170.0), _y(460.0),
											_x(170.0) + _width(175.0),
											_y(460.0) - _height(175.0)),
										&CRGBA(255, 255, 255, 255));

	/*DevLogos[1].DrawTexturedRect(&CRect(
											_x(637.5), _y(450.0),
											_x(637.5) + _width(45.0),
											_y(450.0) - _height(45.0)),
										&RwRGBA(255, 255, 255, 255));*/

/*#endif
}*/


void CHud::DrawBarChartWithRoundBorder(float fX, float fY, WORD wWidth, WORD wHeight, float fPercentage, BYTE drawBlueLine, BYTE drawPercentage, BYTE drawBorder, CRGBA dwColor, CRGBA dwForeColor)
{
	DrawBarChart(fX, fY, wWidth, wHeight, fPercentage, drawBlueLine, drawPercentage, 0, dwColor, dwForeColor);

	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, FALSE);

	Sprites[HUD_BarOutline].Draw(CRect(fX - _width(2.0f), fY - _height(1.5f), fX + wWidth + _width(2.0f), fY + wHeight + _height(1.5f)), CRGBA(0, 0, 0, dwColor.a));
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, FALSE);
}

/*void CHud::DrawSquareBar(float fX, float fY, WORD wWidth, WORD wHeight, float fPercentage, BYTE drawBlueLine, BYTE drawShadow, BYTE drawBorder, CRGBA dwColour, CRGBA dwForeColor)
{
	CRect coords;

	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
	RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rwSHADEMODEFLAT);

	wHeight /= 2.0;
	fY += wHeight;
	if (fPercentage < 0.0)
		fPercentage = 0.0;
	if ( drawBorder )
	{
		/*RwRGBA backColour(0, 0, 0, 255);

		coords.x1 = fX - (_x((640.0 - 1.0 * drawBorder)));
		coords.y1 = fY - (_y(1.0 * drawBorder));
		coords.x2 = fX + wWidth + (_width(1.0 * drawBorder));
		coords.y2 = fY + wHeight + (_height(1.0 * drawBorder));
		CDraw::Rect(&coords, &backColour);*/

/*		CDraw::Rect(&CRect(	fX - (_x((640.0 - 1.0 * drawBorder))),
								fY - (_y(1.0 * drawBorder)),
								fX + wWidth + (_width(1.0 * drawBorder)),
								fY + wHeight + (_height(1.0 * drawBorder))),
					&CRGBA(0, 0, 0, 255));
	}
	if ( drawShadow )
	{
		CRGBA shadowColour(0, 0, 0, 200);

		coords.x1 = fX + (_x((640.0 - 2.0 * drawShadow)));
		coords.y1 = fY + (_y(2.0 * drawShadow));
		coords.x2 = fX + wWidth + (_width(2.0 * drawShadow));
		coords.y2 = fY + wHeight + (_height(2.0 * drawShadow));
		CDraw::Rect(&coords, &shadowColour);
	}
	CRGBA whiteColour(255, 255, 255, 255);

	coords.x1 = fX;
	coords.y1 = fY;
	coords.x2 = fX + wWidth;
	coords.y2 = fY + wHeight;
	CDraw::Rect(&coords, &whiteColour);
	coords.x2 = fX + wWidth * ((fPercentage + 1.0) / 100.0);
	CDraw::Rect(&coords, &dwColour);
}*/

// This is just ugh
void CHud::DrawSquareBar(float fX, float fY, WORD wWidth, WORD wHeight, float fPercentage, BYTE drawBlueLine, BYTE drawShadow, BYTE drawBorder, CRGBA dwColour, CRGBA dwForeColor)
{
	CRect coords;

	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, nullptr);
	RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rwSHADEMODEFLAT);

	wWidth *= ScreenAspectRatio * (3.0f/4.0f);

	wHeight /= 2.0;
	fY += wHeight;
	if (fPercentage < 0.0)
		fPercentage = 0.0;
	if ( drawBorder )
	{
		/*RwRGBA backColour(0, 0, 0, 255);

		coords.x1 = fX - (_x((640.0 - 1.0 * drawBorder)));
		coords.y1 = fY - (_y(1.0 * drawBorder));
		coords.x2 = fX + wWidth + (_width(1.0 * drawBorder));
		coords.y2 = fY + wHeight + (_height(1.0 * drawBorder));
		CDraw::Rect(&coords, &backColour);*/

		CSprite2d::DrawRect(CRect(	fX - (_xleft((1.0 * drawBorder))),
								fY - (_y(1.0 * drawBorder)),
								fX + wWidth + (_width(1.0 * drawBorder)),
								fY + wHeight + (_height(1.0 * drawBorder))),
					CRGBA(0, 0, 0, 255));
	}
	if ( drawShadow )
	{
		CRGBA shadowColour(0, 0, 0, 200);

		coords.x1 = fX + (_xleft((2.0 * drawShadow)));
		coords.y1 = fY + (_y(2.0 * drawShadow));
		coords.x2 = fX + wWidth + (_width(2.0 * drawShadow));
		coords.y2 = fY + wHeight + (_height(2.0 * drawShadow));
		CSprite2d::DrawRect(coords, shadowColour);
	}
	CRGBA whiteColour(255, 255, 255, 255);

	coords.x1 = fX;
	coords.y1 = fY;
	coords.x2 = fX + wWidth;
	coords.y2 = fY + wHeight;
	CSprite2d::DrawRect(coords, whiteColour);
	coords.x2 = fX + wWidth * ((fPercentage + 1.0) / 100.0);
	CSprite2d::DrawRect(coords, dwColour);
}

float CHud::GetYPosBasedOnHealth(unsigned char plrID, float position, signed char offset)
{
	UNREFERENCED_PARAMETER(plrID);
	UNREFERENCED_PARAMETER(offset);
	return position;
}

bool CHud::HelpMessageShown()
{
	return m_HelpMessageState != 0 || m_PagerMessage[0];
}