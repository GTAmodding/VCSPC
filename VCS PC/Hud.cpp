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
#include "TxdStore.h"
#include "Rs.h"
#include "Sprite.h"
#include "Pad.h"
#include "Radar.h"
#include "Messages.h"
#include "CutsceneManager.h"

char					*CHud::m_pHelpMessage = (char*)0xBAA7A0;
char					*CHud::m_pLastHelpMessage = (char*)0xBAA610;
int						&CHud::m_nHelpMessageState = *(int*)0xBAA474;
int						&CHud::m_nHelpMessageTimer = *(int*)0xBAA47C;
int						&CHud::m_nHelpMessageFadeTimer = *(int*)0xBAA478;
char					*CHud::m_pHelpMessageToPrint = (char*)0xBAA480;
float					&CHud::m_fTextBoxNumLines = *(float*)0xBAA460;
char					&CHud::m_bHelpMessagePermanent = *(char*)0xBAA464;
float					&CHud::m_fHelpMessageTime = *(float *)0xBAA460;
bool					&CHud::m_bHelpMessageQuick = *(bool *)0xBAA472;
char					*CHud::MessageIDString = (char *)0x96C014;
char					(*CHud::m_BigMessage)[128] = (char(*)[128])0xBAACC0;
float					&CHud::m_fHelpMessageBoxWidth = *(float *)0x8D0934;

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
CSprite2d   		    CHud::Sprites[NUM_HUD_SPRITES];
CVector2D				CHud::m_vecFontScale;

static unsigned char	PagerOn;
static bool				PagerSoundPlayed;
static float			PagerXOffset;

static bool				CounterOnLastFrame[NUM_ONSCREEN_COUNTERS], PlaceOnLastFrame;
static unsigned short	CounterFlashTimer[NUM_ONSCREEN_COUNTERS], PlaceFlashTimer;

// Wrappers
WRAPPER void CHud::DrawBarChart(float fX, float fY, WORD wWidth, WORD wHeight, float fPercentage, BYTE drawBlueLine, BYTE drawPercentage, BYTE drawBorder, CRGBA dwColor, CRGBA dwForeColor)
{
	WRAPARG(fX); WRAPARG(fY); WRAPARG(wWidth); WRAPARG(wHeight); WRAPARG(fPercentage); WRAPARG(drawBlueLine); WRAPARG(drawPercentage); WRAPARG(drawBorder); WRAPARG(dwColor); WRAPARG(dwForeColor);
	EAXJMP(0x728640);
}
WRAPPER void CHud::DrawAfterFade(void) { EAXJMP(0x58D490); }
WRAPPER void CHud::SetHelpMessage(const char *text, bool b1, bool b2, bool b3) { EAXJMP(0x588BE0); }
WRAPPER void CHud::SetMessage(char *text) { EAXJMP(0x588F60); }

static const char* const			HudSpriteFilenames[NUM_HUD_SPRITES] = { "fist", "siteM16", "siterocket", "radardisc", "barInside", "barOutline", "pager", "sitesniper", "sitelaser", "laserdot", "triangle_64", "viewfinder_128", "hudnumbers" };

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

	for (int i = 0; i < NUM_HUD_SPRITES; ++i)
	{
		Sprites[i].SetTextureFromSPTA(HudSPTA, HudSpriteFilenames[i]);
		if (i == 0)
			HudSPTA.SetDirectory("overlays");
	}

	HudSPTA.CloseArchive();
	ReInitialise();
}

void CHud::ReInitialise()
{
	// Call SA ReInitialise function
	((void(*)())0x588880)();

	//#ifndef DEVBUILD
#if 1
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

	for (int i = 0; i < NUM_ONSCREEN_COUNTERS; ++i)
	{
		CounterOnLastFrame[i] = false;
		CounterFlashTimer[i] = 0;
	}

	PlaceOnLastFrame = false;
	PlaceFlashTimer = 0;
}

void CHud::Shutdown()
{
	for (int i = 0; i < NUM_HUD_SPRITES; ++i)
		Sprites[i].Delete();
}

void CHud::SetVehicleName(char* carName)
{
	pCarName = carName;
}

void CHud::SetZoneName(char* zoneName, bool bIgnoreIntID)
{
	if ((bIgnoreIntID || !*activeInterior && !m_ZoneState) && (!zoneName || strncmp(zoneName, "DUMMY", 5)))
		m_pZoneName = zoneName;
}

void CHud::SetPagerMessage(char* pMsg)
{
	unsigned short	wLoopCounter = 0;
	while (wLoopCounter < sizeof(m_PagerMessage))
	{
		if (!pMsg[wLoopCounter])
			break;
		m_PagerMessage[wLoopCounter] = pMsg[wLoopCounter];
		++wLoopCounter;
	}

	m_PagerMessage[wLoopCounter] = '\0';
}

void CHud::DrawHUD() {
	RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, reinterpret_cast<void*>(rwFILTERLINEARMIPLINEAR));

	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, reinterpret_cast<void*>(0));
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, reinterpret_cast<void*>(0));
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, reinterpret_cast<void*>(1));
	RwRenderStateSet(rwRENDERSTATESRCBLEND, reinterpret_cast<void*>(5));
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, reinterpret_cast<void*>(6));
	RwRenderStateSet(rwRENDERSTATEFOGENABLE, reinterpret_cast<void*>(0));
	RwRenderStateSet(rwRENDERSTATECULLMODE, reinterpret_cast<void*>(1));

	// 1st Player
	if (CWorld::Players[0].pPed) {
		if (!CDLCManager::GetDLC(DLC_HALLOWEEN)->IsEnabled()) {
			DrawWeaponIcon(0, _x((HUD_POS_X * 2.30f) + 104.0f), _y((HUD_POS_Y * 0.9f) + 24.5f), _width(76.0f), _height(72.0f));
			DrawWeaponAmmo(0, _x((HUD_POS_X * 2.30f) + 50.0f), _y((HUD_POS_Y * 0.9f) + 67.5f), FrontEndMenuManager.m_bEnableSkyMenu, _xleft(24.5f), _width(0.325f), _height(0.755f));

			// Clock
			char* am_pm;
			char ms_nGameClock[16];

			if (FrontEndMenuManager.GetHudMode() == 2) {
				unsigned char bHour = CClock::GetHours();

				am_pm = CClock::Convert24To12(bHour) ? "a" : "p";
				_snprintf(ms_nGameClock, sizeof(ms_nGameClock), bHour > 9 ? "%d:%02d" : " %d:%02d", bHour, CClock::GetMinutes());
			}
			else {
				am_pm = "";
				_snprintf(ms_nGameClock, sizeof(ms_nGameClock), "%02d:%02d", CClock::GetHours(), CClock::GetMinutes());
			}

			CHud::SetFontScale(_width(0.912f), _height(0.776f));
			CHud::PrintString(_x((HUD_POS_X * 2.30f) + 120.5f), _y((HUD_POS_Y * 0.9f) + 26.5f), am_pm, CRGBA(193, 140, 44, HUD_TRANSPARENCY_BACK)); // AM_PM

			CHud::SetFontScale(_width(FrontEndMenuManager.GetHudMode() == 2 ? 0.702f : 0.954f), _height((FrontEndMenuManager.GetHudMode() == 2 ? 0.728f : 0.748f)));
			CHud::PrintString(_x((HUD_POS_X * 2.30f) + 163.5f), _y((HUD_POS_Y * 0.9f) + 26.5f), ms_nGameClock, CRGBA(193, 140, 44, HUD_TRANSPARENCY_BACK)); // HOUR_MINUTES
		}

		// Money
		char m_dwMoneyString[16];

		int m_dwMoney = CWorld::Players[CWorld::PlayerInFocus].GetDisplayedScore();
		const char*	str;
		CRGBA color;

		if (m_dwMoney < 0) {
			m_dwMoney = -m_dwMoney;
			str = "-$%07d";
			color = { 155, 7, 7, HUD_TRANSPARENCY_BACK };
		}
		else {
			str = "$%08d";
			color = { 32, 110, 32, HUD_TRANSPARENCY_BACK };
		}

		_snprintf(m_dwMoneyString, sizeof(m_dwMoneyString), str, m_dwMoney);

		CHud::SetFontScale(_width(0.968f), _height(0.742f));
		CHud::PrintString(_x((HUD_POS_X * 2.30f) + 163.5f), _y((HUD_POS_Y * 0.9f) + 85.5f), m_dwMoneyString, CRGBA(color));

		// Info Bars
		PrintHealthBar(0, _x((HUD_POS_X * 2.30f) + 162.2f), _y((HUD_POS_Y * 0.9f) + 65.5f), _width(56.2f), _height(16.8f));
		PrintArmourBar(0, _x((HUD_POS_X * 2.30f) + 162.2f), _y((HUD_POS_Y * 0.9f) + 45.5f), _width(56.2f), _height(16.8f));
		PrintStaminaBar(0, _x((HUD_POS_X * 2.30f) + 162.2f), _y((HUD_POS_Y * 0.9f) + 45.5f), _width(56.2f), _height(16.8f));
	}

	// Pager
	DrawPager();

	// 2st player
	if (CWorld::Players[1].pPed) {

	}
}

void CHud::DrawPager() {
	if (!m_PagerMessage[0])
	{
		if (PagerOn == 1)
		{
			PagerSoundPlayed = false;
			PagerOn = 2;
		}
	}
	if (m_PagerMessage[0] || PagerOn == 2)
	{
		if (!PagerOn)
		{
			PagerOn = 1;
			PagerXOffset = 200.0f;
		}
		if (PagerOn == 1)
		{
			if (PagerXOffset > 0.0f)
			{
				float	fStep = PagerXOffset * 0.06f;
				if (fStep > 6.0f)
					fStep = 6.0f;
				PagerXOffset -= fStep * CTimer::ms_fTimeStep;
			}
			if (!PagerSoundPlayed)
			{
				AudioEngine.ReportFrontendAudioEvent(46, 0.0f, 1.0f);
				PagerSoundPlayed = true;
			}
		}
		else
		{
			if (PagerOn == 2)
			{
				float	fStep = PagerXOffset * 0.06f;
				if (fStep < 1.2f)
					fStep = 1.2f;
				PagerXOffset += fStep * CTimer::ms_fTimeStep;
				if (PagerXOffset > 200.0f)
				{
					PagerOn = 0;
					PagerXOffset = 200.0f;
				}
			}
		}

		float fTextBoxOffset = 0.0f;

		if (CHud::m_HelpMessageState)
			fTextBoxOffset = 60.0f;
		else
			fTextBoxOffset = 0.0f;

		Sprites[HUD_Pager].Draw(CRect(_xleft(42.5f + ((HUD_POS_X * 2.15f) * 0.9f) - PagerXOffset), _y(102.0f + ((HUD_POS_Y * 0.85f) * 1.2f) + fTextBoxOffset), _xleft(213.0f + ((HUD_POS_X * 2.15f) * 0.9f) - PagerXOffset), _y(27.0f + ((HUD_POS_Y * 0.85f) * 1.2f) + fTextBoxOffset)), CRGBA(255, 255, 255, 255));

		CFont::SetProportional(false);
		CFont::SetScale(_width(0.5f), _height(1.1f));
		CFont::SetOrientation(ALIGN_Left);
		CFont::SetColor(CRGBA(7, 7, 7, 205));
		CFont::SetFontStyle(FONT_PagerFont);
		CFont::PrintString(_xleft(59.0f + ((HUD_POS_X * 2.15f) * 0.9f) - PagerXOffset), _y(40.0f + ((HUD_POS_Y * 0.85f) * 1.2f) + fTextBoxOffset), m_PagerMessage);
	}
}

void CHud::DrawWanted() {
	int		nWantedLevel = FindPlayerWanted(-1)->GetWantedLevel();
	int		nWantedLevelParole = FindPlayerWanted(-1)->GetWantedLevelBeforeParole();
	int		nTimeOfWLChange = FindPlayerWanted(-1)->GetTimeWantedLevelChanged();

	if (nWantedLevel > 0 || nWantedLevelParole > 0) {
		CHud::SetFontScale(_width(0.804f), _height(1.30f));

		float fCurrentPos = _x((HUD_POS_X * 2.30f) + 66.59f);
		for (int i = 0; i < 6; ++i, fCurrentPos -= _width(19.4f)) {
			if (nWantedLevel > i && (CTimer::m_snTimeInMilliseconds > nTimeOfWLChange + 2000 || ShowFlashingItem(150, 150))) {
				CHud::PrintString(fCurrentPos, _y((HUD_POS_Y * 0.9f) + 103.0f), "*", CRGBA(207, 149, 54, HUD_TRANSPARENCY_BACK));
			}
			else if (nWantedLevelParole > i && ShowFlashingItem(150, 150)) {
				CHud::PrintString(fCurrentPos, _y((HUD_POS_Y * 0.9f) + 103.0f), "*", CRGBA(BaseColors[12], HUD_TRANSPARENCY_BACK));
			}
		}
	}
}

char LaserScopeDot(CVector2D const& coords, float const& fDotSize) {
	return ((char(__thiscall*)(eWeaponType const&, CVector2D const&, float const&))0x73A8D0)(FindPlayerPed(0)->weaponSlots[FindPlayerPed(0)->m_bActiveWeapon].m_eWeaponType, coords, fDotSize);
}

void __cdecl CHud::DrawCrosshairs() {
	CPed *player = FindPlayerPed(-1);
	int GetWeaponInfo = CWeaponInfo::GetWeaponInfo((eWeaponType)player->weaponSlots[player->m_bActiveWeapon].m_eWeaponType, 1)->dwModelID;
	WORD Mode = TheCamera.Cams[TheCamera.ActiveCam].Mode;

	float x = RsGlobal.MaximumWidth * CCamera::m_f3rdPersonCHairMultX;
	float y = RsGlobal.MaximumHeight *CCamera::m_f3rdPersonCHairMultY;

	float fHalfSize = 326.0f;
	CVector2D coords;
	float fRealDotSize;
	float f2DDotSizeW;
	float f2DDotSizeH;

	if (!FindPlayerPed(0)->pPlayerData->m_bHaveTargetSelected) {
		if (Mode != 53 || !TheCamera.m_bUseTransitionBeta || *(int*)0xA44490) {
			if (Mode == MODE_AIMWEAPON_FROMCAR || Mode == MODE_AIMWEAPON || Mode == MODE_AIMWEAPON_ATTACHED || Mode == MODE_ROCKETLAUNCHER || Mode == MODE_ROCKETLAUNCHER_HS || Mode == MODE_SNIPER || Mode == MODE_CAMERA) {
				switch (GetWeaponInfo) {
				case 357: // Sniper
					Sprites[HUD_SiteSniper].Draw(_xmiddle(-fHalfSize), _ymiddle(-fHalfSize), _width(fHalfSize), _height(fHalfSize), CRGBA(150, 255, 150, 255)); // Left Top
					Sprites[HUD_SiteSniper].Draw(_xmiddle(fHalfSize), _ymiddle(-fHalfSize), _width(-fHalfSize), _height(fHalfSize), CRGBA(150, 255, 150, 255)); // Right Top
					Sprites[HUD_SiteSniper].Draw(_xmiddle(-fHalfSize), _ymiddle(fHalfSize), _width(fHalfSize), _height(-fHalfSize), CRGBA(150, 255, 150, 255)); // Left Bottom
					Sprites[HUD_SiteSniper].Draw(_xmiddle(fHalfSize), _ymiddle(fHalfSize), _width(-fHalfSize), _height(-fHalfSize), CRGBA(150, 255, 150, 255)); // Right Bottom

					// PILLAR BOX
					CSprite2d::DrawRect(CRect(-2.0f, -2.0f, (RsGlobal.MaximumWidth * 1.33334 / RsGlobal.MaximumWidth / 640.0f) + 2.0f + _xmiddle(-320), RsGlobal.MaximumHeight + 2.0f), CRGBA(0, 0, 0, 255));
					CSprite2d::DrawRect(CRect(RsGlobal.MaximumWidth, -2.0, RsGlobal.MaximumWidth - (RsGlobal.MaximumWidth * 1.33334 / RsGlobal.MaximumWidth / 640.0f) - 2.0f - _xmiddle(-320), RsGlobal.MaximumHeight + 2.0f), CRGBA(0, 0, 0, 255));
					break;
				case 358: // Laser
					Sprites[HUD_SiteLaser].Draw(_xmiddle(-fHalfSize), _ymiddle(-fHalfSize), _width(fHalfSize), _height(fHalfSize), CRGBA(150, 255, 150, 255)); // Left Top
					Sprites[HUD_SiteLaser].Draw(_xmiddle(fHalfSize), _ymiddle(-fHalfSize), _width(-fHalfSize), _height(fHalfSize), CRGBA(150, 255, 150, 255)); // Right Top
					Sprites[HUD_SiteLaser].Draw(_xmiddle(-fHalfSize), _ymiddle(fHalfSize), _width(fHalfSize), _height(-fHalfSize), CRGBA(150, 255, 150, 255)); // Left Bottom
					Sprites[HUD_SiteLaser].Draw(_xmiddle(fHalfSize), _ymiddle(fHalfSize), _width(-fHalfSize), _height(-fHalfSize), CRGBA(150, 255, 150, 255)); // Right Bottom

					RwRenderStateSet(rwRENDERSTATESRCBLEND, reinterpret_cast<void *>(2));
					fRealDotSize = 25.0f;
					LaserScopeDot(coords, fRealDotSize);
					f2DDotSizeW = RsGlobal.MaximumWidth *  0.0015625 * 1.3334 / ScreenAspectRatio * fRealDotSize;
					f2DDotSizeH = RsGlobal.MaximumHeight * 0.002232143 * fRealDotSize;

					Sprites[HUD_LaserDot].Draw(coords.x - f2DDotSizeW / 2, coords.y - f2DDotSizeH / 2, f2DDotSizeW, f2DDotSizeH, CRGBA(155, 0, 0, 105)); // Laser Dot
					RwRenderStateSet(rwRENDERSTATESRCBLEND, reinterpret_cast<void *>(-1));

					// PILLAR BOX
					CSprite2d::DrawRect(CRect(-2.0f, -2.0f, (RsGlobal.MaximumWidth * 1.33334 / RsGlobal.MaximumWidth / 640.0f) + 2.0f + _xmiddle(-320), RsGlobal.MaximumHeight + 2.0f), CRGBA(0, 0, 0, 255));
					CSprite2d::DrawRect(CRect(RsGlobal.MaximumWidth, -2.0, RsGlobal.MaximumWidth - (RsGlobal.MaximumWidth * 1.33334 / RsGlobal.MaximumWidth / 640.0f) - 2.0f - _xmiddle(-320), RsGlobal.MaximumHeight + 2.0f), CRGBA(0, 0, 0, 255));
					break;
				case 359: // Rocketla
					Sprites[HUD_Siterocket].Draw(_xmiddle(-68.0f / 2), _ymiddle(-68.0f / 2), _width(68.0f), _height(68.0f), CRGBA(150, 255, 150, 255));
					break;
				case 367: // Camera
					Sprites[HUD_ViewFinder].Draw(CRect(0.0f, 0.0f + RsGlobal.MaximumHeight / 2, 0.0f + RsGlobal.MaximumWidth / 2, 0.0f), CRGBA(255, 255, 255, 255)); // Left Top
					Sprites[HUD_ViewFinder].Draw(CRect(RsGlobal.MaximumWidth, 0.0f + RsGlobal.MaximumHeight / 2, RsGlobal.MaximumWidth - RsGlobal.MaximumWidth / 2, 0.0f), CRGBA(255, 255, 255, 255)); // Right Top
					Sprites[HUD_ViewFinder].Draw(CRect(0.0f, RsGlobal.MaximumHeight - RsGlobal.MaximumHeight / 2, 0.0f + RsGlobal.MaximumWidth / 2, RsGlobal.MaximumHeight), CRGBA(255, 255, 255, 255)); // Left Bottom
					Sprites[HUD_ViewFinder].Draw(CRect(RsGlobal.MaximumWidth, RsGlobal.MaximumHeight - RsGlobal.MaximumHeight / 2, RsGlobal.MaximumWidth - RsGlobal.MaximumWidth / 2, RsGlobal.MaximumHeight), CRGBA(255, 255, 255, 255)); // Right Bottom
					break;
				default:
					Sprites[HUD_SiteM16].Draw(x - _width(28.0f / 2), y - _height(28.0f / 2), _width(28.0f), _height(28.0f), CRGBA(255, 255, 255, 255));
					break;
				}
			}
		}
	}
}

void __stdcall CHud::DrawWindowRect(CRect *coords, char *titleKey, char fadeState, CRGBA rgba, int a5, char bDrawBox) {
	unsigned int savedShade;
	unsigned int savedAlpha;
	RwRenderStateGet(rwRENDERSTATESHADEMODE, &savedShade);
	RwRenderStateSet(rwRENDERSTATESHADEMODE, reinterpret_cast<void *>(rwSHADEMODEGOURAUD));
	RwRenderStateGet(rwRENDERSTATEVERTEXALPHAENABLE, &savedAlpha);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, reinterpret_cast<void *>(TRUE));
	CSprite2d::DrawRect(CRect((coords->x1 + _xleft(-3.35f)), (coords->y2 + _y(2.0f)), (coords->x2 + _xleft(3.35f)), (coords->y1)), CRGBA(rgba.r, rgba.g, rgba.b, 125));
	RwRenderStateSet(rwRENDERSTATESHADEMODE, reinterpret_cast<void *>(savedShade));
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, reinterpret_cast<void *>(savedAlpha));
}

void CHud::DrawHelpText() {
	CVector2D	vecTextBoxPosn = (CVector2D(((HUD_POS_X * 2.15f) * 0.9f) + 48.50f, ((HUD_POS_Y * 0.85f) * 1.2f) + 29.25f));
	CVector2D	vecTextBoxFontScale = (CVector2D(0.48f, 1.08f));
	float		fTextBoxWidth = (287.0f);

	if (m_pHelpMessage[0]) {
		if (!CMessages::StringCompare(m_pHelpMessage, m_pLastHelpMessage, 400)) {
			switch (m_nHelpMessageState) {
			case 0:
				m_nHelpMessageState = 2;
				m_nHelpMessageTimer = 0;
				m_nHelpMessageFadeTimer = 0;
				CMessages::StringCopy(m_pHelpMessageToPrint, m_pHelpMessage, 400);
				m_fTextBoxNumLines = CFont::GetNumberLines(_xleft(vecTextBoxPosn.x), _y(vecTextBoxPosn.y), m_pHelpMessageToPrint) + 3;
				AudioEngine.ReportFrontendAudioEvent(32, 0.0f, 1.0f);
				break;
			case 1:
			case 2:
			case 3:
			case 4:
				m_nHelpMessageState = 4;
				m_nHelpMessageTimer = 5;
				break;
			default:
				break;
			}
			CMessages::StringCopy(m_pLastHelpMessage, m_pHelpMessage, 400);
		}

		float alpha = 255.0f;

		if (m_nHelpMessageState) {
			switch (m_nHelpMessageState) {
			case 2:
				if (!TheCamera.m_WideScreenOn) {
					m_nHelpMessageFadeTimer += 2 * (CTimer::ms_fTimeStep * 0.02f * 1000.0f);
					if (m_nHelpMessageFadeTimer > 0) {
						m_nHelpMessageFadeTimer = 0;
						m_nHelpMessageState = 1;
					}
					alpha = m_nHelpMessageFadeTimer * 0.001f * 255.0f;
				}
				break;
			case 3:
				m_nHelpMessageFadeTimer -= 2 * (CTimer::ms_fTimeStep * 0.02f * 1000.0f);
				if (m_nHelpMessageFadeTimer < 0 || TheCamera.m_WideScreenOn) {
					m_nHelpMessageFadeTimer = 0;
					m_nHelpMessageState = 0;
				}
				alpha = m_nHelpMessageFadeTimer * 0.001f * 255.0f;
				break;
			case 4:
				m_nHelpMessageFadeTimer -= 2 * (CTimer::ms_fTimeStep * 0.02f * 1000.0f);
				if (m_nHelpMessageFadeTimer < 0) {
					m_nHelpMessageFadeTimer = 0;
					m_nHelpMessageState = 2;
					CMessages::StringCopy(m_pHelpMessageToPrint, m_pLastHelpMessage, 400);
				}
				alpha = m_nHelpMessageFadeTimer * 0.001f * 255.0f;
				break;
			case 1:
				alpha = 255.0f;
				m_nHelpMessageFadeTimer = 600;
				if (!m_bHelpMessagePermanent) {
					if (m_nHelpMessageTimer > m_fHelpMessageTime * 1000.0f || m_bHelpMessageQuick && m_nHelpMessageTimer > 3000) {
						m_nHelpMessageState = 3;
						m_nHelpMessageFadeTimer = 600;
					}
				}
				break;
			default:
				break;
			}

			if (!CCutsceneMgr::ms_running) {
				m_nHelpMessageTimer += (CTimer::ms_fTimeStep * 0.02f * 1000.0f);
				if (m_BigMessage[0][0] || m_BigMessage[4][0] || MessageIDString[0]) {
					CFont::SetAlphaFade(255.0f);
					return;
				}

				CFont::SetProportional(true);
				CFont::SetAlphaFade(alpha);
				CFont::SetBackground(true, true);
				CFont::SetOrientation(ALIGN_Left);
				CFont::SetJustify(false);
				CFont::SetWrapx(_xleft(vecTextBoxPosn.x + fTextBoxWidth));
				CFont::SetFontStyle(FONT_Eurostile);
				CFont::SetDropShadowPosition(0);
				CFont::SetEdge(1);
				CFont::SetBackgroundColor(CRGBA(0, 0, 0, alpha));
				CFont::SetDropColor(CRGBA(0, 0, 0, 255));
				CFont::SetColor(CRGBA(255, 255, 255, 255));

				CFont::SetScale(_width(vecTextBoxFontScale.x), _height(vecTextBoxFontScale.y));
				CFont::PrintString(_xleft(vecTextBoxPosn.x), _y(vecTextBoxPosn.y), m_pHelpMessageToPrint);

				CFont::SetWrapx(RsGlobal.MaximumWidth);
				CFont::SetAlphaFade(255.0f);
				return;
			}
		}
	}
	else
		m_nHelpMessageState = 0;
}

void __cdecl DrawSpriteWithAngle(CSprite2d sprite, float x, float y, float r_angle, unsigned int width, unsigned int height, CRGBA const& color) {
	CVector2D posn[4];

	posn[0].x = x - width / 1.2f;

	posn[0].y = y + height / 1.2f;

	posn[1].x = x + width / 1.2f;

	posn[1].y = y + height / 1.2f;

	posn[2].x = x - width / 1.2f;

	posn[2].y = y - height / 1.2f;

	posn[3].x = x + width / 1.2f;

	posn[3].y = y - height / 1.2f;

	RotateVertices(posn, 4, x, y, r_angle);

	sprite.Draw(posn[2].x, posn[2].y, posn[3].x, posn[3].y, posn[0].x, posn[0].y, posn[1].x, posn[1].y, CRGBA(color));
}

void __cdecl CHud::DrawTarget(float a1, float a2, int a3, int a4, int a5, int a6, float a7, unsigned __int8 a8, unsigned __int8 a9, unsigned __int8 a10, __int16 a11, float a12, char a13) {
	WORD Mode = TheCamera.Cams[TheCamera.ActiveCam].Mode;

	if (a7 >= 0.0 && a7 <= 1.0)
		a7 = 1.35;
	else if (a7 >= 1.0 && a7 <= 2.0)
		a7 = 1.30;
	else if (a7 >= 2.0 && a7 <= 3.0)
		a7 = 1.25;
	else if (a7 >= 3.0 && a7 <= 4.0)
		a7 = 1.20;
	else if (a7 >= 4.0 && a7 <= 5.0)
		a7 = 1.15;
	else if (a7 >= 5.0 && a7 <= 6.0)
		a7 = 1.10;
	else if (a7 >= 6.0 && a7 <= 7.0)
		a7 = 1.05;
	else if (a7 >= 7.0 && a7 <= 8.0)
		a7 = 1.00;
	else if (a7 >= 8.0 && a7 <= 9.0)
		a7 = 0.95;
	else if (a7 >= 9.0 && a7 <= 10.0)
		a7 = 0.90;
	else if (a7 >= 10.0 && a7 <= 11.0)
		a7 = 0.85;
	else if (a7 >= 11.0 && a7 <= 12.0)
		a7 = 0.80;
	else if (a7 >= 12.0 && a7 <= 13.0)
		a7 = 0.75;
	else if (a7 >= 13.0 && a7 <= 14.0)
		a7 = 0.70;
	else if (a7 >= 14.0 && a7 <= 15.0)
		a7 = 0.65;
	else if (a7 >= 15.0 && a7 <= 16.0)
		a7 = 0.60;
	else if (a7 >= 16.0 && a7 <= 17.0)
		a7 = 0.55;
	else
		a7 = 0.50;

	float size = 6.8f;
	if (Mode == MODE_AIMWEAPON) {
		DrawSpriteWithAngle(Sprites[HUD_Triangle], a1 - _coords(32.0f * a7), a2 - _coords(6.0f * a7), -0.6, _width(size), _width(size), CRGBA(a8, a9, a10, a11)); // Left
		DrawSpriteWithAngle(Sprites[HUD_Triangle], a1 + _coords(32.0f * a7), a2 - _coords(6.0f * a7), 3.8, _width(size), _width(size), CRGBA(a8, a9, a10, a11)); // Right

		DrawSpriteWithAngle(Sprites[HUD_Triangle], a1, a2 + _coords(52.0f * a7), 1.6, _width(size), _width(size), CRGBA(a8, a9, a10, a11)); // Bottom
	}
}

void CHud::DrawOnscreenTimer()
{
	if (CUserDisplay::OnscnTimer.IsEnabled() && !StyledText_5[0] && !Garage_MessageIDString[0])
	{
		CFont::SetBackground(0, 0);
		CFont::SetScale(_width(0.5f), _height(1.0f));
		CFont::SetOrientation(ALIGN_Right);
		CFont::SetFontStyle(FONT_Eurostile);
		CFont::SetDropShadowPosition(1);

		static DWORD			dwTicksCounter = 0;
		static int				nCounterTimer = 0;
		bool					bRecalculateStuffThisTick;

		if (CTimer::m_snTimeInMillisecondsPauseMode - nCounterTimer > 40)	// Cap it at 25 FPS
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

		for (int i = 0; i < NUM_ONSCREEN_COUNTERS; ++i)
		{
			if (!pCounters[i].m_bShown || pCounters[i].m_bForceRecheck)
			{
				pCounters[i].m_bForceRecheck = false;
				CounterOnLastFrame[i] = false;
			}

			if (pCounters[i].m_bShown)
			{
				if (pCounters[i].m_wOwnsWhat != 0xFFFF)
					bChildrenEntries += 3;
			}
		}

		if (pTimer->m_bShown)
		{
			int		nRemainder = *pTimer->m_pVariable % 1000;
			if (!pTimer->m_bFlashing || (*pTimer->m_pVariable < 750 || (nRemainder < 750 && nRemainder > 250)))
			{
				CFont::SetProportional(false);
				CFont::SetDropColor(pTimer->m_shadowColour);
				CFont::SetColor(pTimer->m_colour);

				CFont::PrintString(_x(46.0f + (HUD_POS_X * 2.30f)), _y((HUD_POS_Y * 0.9f) + bChildrenEntries * 20.0f + 148.0f), pTimer->m_cDisplayedText);
				if (pTimer->m_cGXTentry[0])
				{
					float		fPosXOffset;
					int			nTextLength = strlen(pTimer->m_cDisplayedText);
					if (nTextLength > 4)
						fPosXOffset = 12.5f * (nTextLength - 4);
					else
						fPosXOffset = 0.0f;

					CFont::SetProportional(true);
					CFont::PrintString(_x((HUD_POS_X * 2.30f) + 102.5f + fPosXOffset), _y((HUD_POS_Y * 0.9f) + bChildrenEntries * 20.0f + 148.0f), TheText.Get(pTimer->m_cGXTentry));
				}
			}
		}

		for (int i = 0; i < NUM_ONSCREEN_COUNTERS; ++i)
		{
			if (pCounters[i].m_bShown)
			{
				if (!CounterOnLastFrame[i] && pCounters[i].m_bFlashing)
					CounterFlashTimer[i] = 1;

				CounterOnLastFrame[i] = true;

				/*if ( CounterFlashTimer[i] )
				{
					if ( ++CounterFlashTimer[i] > 50 )
						CounterFlashTimer[i] = 0;
				}*/

				if (CounterFlashTimer[i])
				{
					if (bRecalculateStuffThisTick)
					{
						if (++CounterFlashTimer[i] > 50)
							CounterFlashTimer[i] = 0;
					}
				}

				if (dwTicksCounter & 4 || !CounterFlashTimer[i])
				{
					CFont::SetDropColor(pCounters[i].m_shadowColour);
					CFont::SetColor(pCounters[i].m_colour);

					if (pCounters[i].m_wType != 3)
					{
						if (pCounters[i].m_wType == 1)
							DrawBarChart(_x((HUD_POS_X * 2.30f) + 110.0f), _y((HUD_POS_Y * 0.9f) + i * 20.0f + 173.0f), _width(61.0f),
								_height(18.0), pCounters[i].m_counterData.nBarFill,
								0, 0, 1, pCounters[i].m_colour,
								CRGBA(0, 0, 0, 0));
						else
						{
							CFont::SetProportional(false);
							CFont::PrintString(_x((HUD_POS_X * 2.30f) + 47.0f), _y((HUD_POS_Y * 0.9f) + i * 20.0f + 173.0f), pCounters[i].m_counterData.cDisplayedText);
						}
					}

					if (pCounters[i].m_cGXTentry[0])
					{
						float		fPosXOffset;
						int			nTextLength = pCounters[i].m_wType == 1 ? 5 : strlen(pCounters[i].m_counterData.cDisplayedText);
						if (nTextLength > 4)
							fPosXOffset = 12.5f * (nTextLength - 4);
						else
							fPosXOffset = 0.0f;

						CFont::SetProportional(true);
						if (pCounters[i].m_wType != 3)
							CFont::PrintString(_x((HUD_POS_X * 2.30f) + 102.5f + fPosXOffset), _y((HUD_POS_Y * 0.9f) + i * 20.0f + 173.0f), TheText.Get(pCounters[i].m_cGXTentry));
						else
							CFont::PrintString(_x((HUD_POS_X * 2.30f) + 47.0f), _y((HUD_POS_Y * 0.9f) + i * 20.0f + 160.0f), TheText.Get(pCounters[i].m_cGXTentry));
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
	if (bShouldFPSBeDisplayed && !InAmazingScreenshotMode)
	{
		char		debugText[64];
		if (!CUpdateManager::AnyTextDisplaysNow())
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
			CFont::PrintString(_x(0.0f), _y(0.0f), debugText);
		}

#if defined INCLUDE_STREAMING_TEXT
		long double	percentUsage = ((long double)(*memoryUsed) / *memoryAvailable) * 100.0;

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

		if (CPed* pPlayerPed = CWorld::Players[0].GetPed())
		{
			CVector&	coords = pPlayerPed->GetCoords();

			_snprintf(debugText, sizeof(debugText), "%.3f %.3f %.3f %.3f", coords.x, coords.y, coords.z, FindPlayerPed(-1)->GetHeading());
			CFont::SetProportional(true);
			CFont::SetOrientation(ALIGN_Right);
			CFont::SetColor(CRGBA(0x0A, 0x57, 0x82));
			CFont::SetBackground(0, 0);
			CFont::SetFontStyle(FONT_Eurostile);
			CFont::SetEdge(1);
			CFont::SetDropColor(CRGBA(0, 0, 0, 255));
			CFont::SetScale(_width(0.35f), _height(0.65f));
			CFont::PrintString(_x(0.0f), _ydown(11.0f), debugText);
		}
		CFont::SetEdge(0);
	}
#else
	if (!CUpdateManager::AnyTextDisplaysNow())
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

	// UPDATER removed for now
	//	CUpdateManager::Display();
}

void CHud::DrawProgressBarWithSprites(float x, float y, float width, float height, float progress, CRGBA color) {
	if (progress >= 100.0f)
		progress = 100.0f;
	else if (progress <= 0.0f)
		progress = 0.0f;

	// Back
	Sprites[HUD_BarInside].Draw(CRect((x + width * (progress / 100.f)), (y), (x + width), (y + height)), CRGBA(color.r * 0.50, color.g * 0.50, color.b * 0.50, color.a), progress / 100.f, 1.0f, 1.0f, 1.0f, progress / 100.f, 0.0f, 1.0f, 0.0f);

	if (progress > 0.0f) {
		Sprites[HUD_BarInside].Draw(CRect((x), (y), (x + width * (progress / 100.f)), (y + height)), CRGBA(color.r, color.g, color.b, color.a), 0.0f, 1.0f, progress / 100.f, 1.0f, 0.0f, 0.0f, progress / 100.f, 0.0f);
	}

	// Outline
	Sprites[HUD_BarOutline].Draw(CRect((x), (y), (x + width), (y + height)), CRGBA(0, 0, 0, color.a));
}

void CHud::PrintHealthBar(int playerID, float x, float y, float width, float height) {
	if (m_ItemToFlash != FLASH_Healthbar || ShowFlashingItem(300, 300)) {
		if (CWorld::Players[playerID].GetLastTimeArmourLost() == CWorld::Players[playerID].GetLastTimeEnergyLost() || CWorld::Players[playerID].GetLastTimeEnergyLost() + BAR_ENERGY_LOSS_FLASH_DURATION < CTimer::m_snTimeInMilliseconds || ShowFlashingItem(150, 150)) {
			if (CWorld::Players[playerID].GetPed()->GetHealth() >= 10 || ShowFlashingItem(300, 300)) {
				DrawProgressBarWithSprites(x, y, width, height, CWorld::Players[playerID].GetPed()->GetHealth() / CWorld::Players[playerID].GetMaxHealth() * 100.0, CRGBA(BaseColors[9], HUD_TRANSPARENCY_BACK));

				if (CWorld::Players[playerID].GetPed()->fMaxHealth >= 150.0f) {
					CHud::SetFontScale(_width(1.60f), _height(1.20f));
					CHud::PrintString(x - _xleft(5.0f), y - _y(1.0f), "+", CRGBA(255, 255, 255, HUD_TRANSPARENCY_BACK)); // +
				}
			}
		}
	}
}

void CHud::PrintArmourBar(int playerID, float x, float y, float width, float height) {
	if (m_ItemToFlash != FLASH_Armourbar || ShowFlashingItem(300, 300)) {
		if (CWorld::Players[playerID].GetLastTimeArmourLost() == 0 || CWorld::Players[playerID].GetLastTimeArmourLost() + BAR_ENERGY_LOSS_FLASH_DURATION < CTimer::m_snTimeInMilliseconds || ShowFlashingItem(150, 150)) {
			if (CWorld::Players[playerID].GetPed()->GetArmour() >= 1.0 && (!CWorld::Players[playerID].GetPed()->bSubmergedInWater || CWorld::Players[playerID].GetPed()->GetVehiclePtr() && !CWorld::Players[playerID].GetPed()->GetVehiclePtr()->bTouchingWater)) {
				DrawProgressBarWithSprites(x, y, width, height, CWorld::Players[playerID].GetPed()->GetArmour() / CWorld::Players[playerID].GetMaxArmour() * 100.0, CRGBA(9, 255, 250, HUD_TRANSPARENCY_BACK));

				if (CWorld::Players[playerID].GetMaxArmour() >= 150.0f) {
					CHud::SetFontScale(_width(1.60f), _height(1.20f));
					CHud::PrintString(x - _xleft(5.0f), y - _y(1.0f), "+", CRGBA(255, 255, 255, HUD_TRANSPARENCY_BACK)); // +
				}
			}
		}
	}
}

void CHud::PrintStaminaBar(int playerID, float x, float y, float width, float height) {
	static float alpha = 0.0f;
	if (m_ItemToFlash != FLASH_Staminabar || ShowFlashingItem(300, 300)) {
		if (CWorld::Players[playerID].GetPed()->bSubmergedInWater || (CWorld::Players[playerID].GetPed()->GetVehiclePtr() && CWorld::Players[playerID].GetPed()->GetVehiclePtr()->bTouchingWater)) {
			if (alpha < HUD_TRANSPARENCY_BACK)
				alpha += CTimer::ms_fTimeScale * 0.01 * 1000.0;
		}
		else {
			if (alpha > 0.0f)
				alpha += CTimer::ms_fTimeScale * 0.01 * -1000.0;
		}

		if (CWorld::Players[playerID].GetPed()->GetHealth() > 0.0)
			DrawProgressBarWithSprites(x, y, width, height, CWorld::Players[playerID].GetPed()->GetPlayerData()->m_fStamina / CStats::CalcPlayerStat(8) * 100.0, CRGBA(0, 30, 117, static_cast<float>(alpha)));

	}
}

static Reversed DrawWeaponIcon_kill(0x58D7D0, 0x58D99F);
void CHud::DrawWeaponIcon(int playerID, float x, float y, float width, float height) {
	RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, reinterpret_cast<void *>(rwFILTERLINEAR));
	int weapModel = CWeaponInfo::GetWeaponInfo((eWeaponType)CWorld::Players[playerID].pPed->weaponSlots[CWorld::Players[playerID].pPed->GetActiveWeapon()].m_eWeaponType, 1)->dwModelID;
	if (weapModel <= 0)
		Sprites[HUD_Fist].Draw(CRect(x, y + (height), x + (width), y), CRGBA(255, 255, 255, HUD_TRANSPARENCY_BACK));
	else {
		CBaseModelInfo *model = CModelInfo::GetModelInfo(weapModel);
		CTxdEntry *txd = CTxdStore::GetPool()->GetAtIndex(CModelInfo::ms_modelInfoPtrs[weapModel]->GetTextureDict());
		if (txd && txd->m_pDictionary) {
			RwTexture *iconTex = RwTexDictionaryFindHashNamedTexture(txd->m_pDictionary, CKeyGen::AppendStringToKey(model->m_hashKey, "ICON"));
			if (iconTex) {
				RwRenderStateSet(rwRENDERSTATEZTESTENABLE, 0);
				RwRenderStateSet(rwRENDERSTATETEXTURERASTER, iconTex->raster);
				CSprite::RenderOneXLUSprite(x + (width / 2), y + (height / 2), 1.0f, (width / 2), (height / 2), 255, 255, 255, HUD_TRANSPARENCY_BACK, 1.0f, HUD_TRANSPARENCY_BACK, 0, 0);
				RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, 0);
			}
		}
	}
}

void CHud::DrawWeaponAmmo(int playerID, float x, float y, bool bPS2Behaviour, float offsetCenter, float width, float height) {
	char AmmoText[16];

	int weapAmmo = CWorld::Players[playerID].pPed->weaponSlots[CWorld::Players[playerID].pPed->GetActiveWeapon()].m_nAmmoTotal;
	int weaponType = CWorld::Players[playerID].pPed->weaponSlots[CWorld::Players[playerID].pPed->GetActiveWeapon()].m_eWeaponType;
	int weapAmmoInClip = CWorld::Players[playerID].pPed->weaponSlots[CWorld::Players[playerID].pPed->GetActiveWeapon()].m_nAmmoInClip;

	short clipSize = CWeaponInfo::GetWeaponInfo((eWeaponType)CWorld::Players[playerID].pPed->weaponSlots[CWorld::Players[playerID].pPed->GetActiveWeapon()].m_eWeaponType, CWorld::Players[playerID].pPed->GetWeaponSkill())->ammoClip;
	static float fOffsetX = 0.0f;

	if (clipSize <= 1 || clipSize >= 1000) {
		sprintf(AmmoText, "%d", weapAmmo);

		CFont::SetOrientation(ALIGN_Center);
		fOffsetX = offsetCenter;
	}
	else {
		int		ammoClipToShow;
		int		ammoRestToShow;

		if (weaponType == 37) {
			int tempAmmoValue = 9999;
			if ((weapAmmo - weapAmmoInClip) / 10 <= 9999)
				tempAmmoValue = (weapAmmo - weapAmmoInClip) / 10;

			ammoClipToShow = weapAmmoInClip / 10;
			ammoRestToShow = tempAmmoValue;
		}
		else {
			int tempAmmoValue = weapAmmo - weapAmmoInClip;
			if (weapAmmo - weapAmmoInClip > 9999)
				tempAmmoValue = 9999;

			ammoClipToShow = weapAmmoInClip;
			ammoRestToShow = tempAmmoValue;
		}
		sprintf(AmmoText, "%d-%d", ammoRestToShow, ammoClipToShow);

		if (bPS2Behaviour) {
			CFont::SetOrientation(ALIGN_Right);
			fOffsetX = 0.0f;
		}
		else {
			CFont::SetOrientation(ALIGN_Center);
			fOffsetX = offsetCenter;
		}
	}

	CFont::SetBackground(0, 0);
	CFont::SetScale(width, height);
	CFont::SetCentreSize(RsGlobal.MaximumWidth);
	CFont::SetProportional(false);
	CFont::SetEdge(0);
	CFont::SetDropColor(CRGBA(0, 0, 0, 255));
	CFont::SetColor(CRGBA(BaseColors[4], HUD_TRANSPARENCY_FRONT));
	CFont::SetFontStyle(FONT_PagerFont);

	if (weapAmmo - weapAmmoInClip < 9999
		&& !CDarkel::FrenzyOnGoing()
		&& weaponType != 40
		&& (weaponType < 10 || weaponType >= 16)
		&& weaponType != 46
		&& CWeaponInfo::GetWeaponInfo((eWeaponType)weaponType, 1)->GetWeaponType() != 5
		&& CWeaponInfo::GetWeaponInfo((eWeaponType)weaponType, 1)->GetWeaponSlot() > 1)
		CFont::PrintString(x - fOffsetX, y, AmmoText);
}

void CHud::DrawRadioName(void* object, const char* radioName) {
	CFont::SetBackground(0, 0);
	CFont::SetProportional(true);
	CFont::SetCentreSize(RsGlobal.MaximumWidth);
	CFont::SetFontStyle(FONT_RageItalic);
	CFont::SetJustify(false);
	CFont::SetScale(_width(0.85f), _height(1.8f));
	CFont::SetOrientation(ALIGN_Center);
	CFont::SetDropShadowPosition(1);
	CFont::SetDropColor(CRGBA(0, 0, 0, 255));
	CFont::SetColor(BaseColors[*((DWORD*)object + 27) || *((DWORD*)object + 28) ? 12 : 2]);

	CFont::PrintString(RsGlobal.MaximumWidth * 0.5f, _y(22.0f + UI_SAFEZONE), radioName);
}

void CHud::DrawVehicleName()
{
	if (pCarName)
	{
		int			tmpStage = CarNameStage;
		const char* tmpCarName = pLastCarName;
		float		alpha = 0.0;

		if (pCarName != pLastCarName)
		{
			if (tmpStage)
			{
				if (tmpStage <= 4)
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

		if (tmpStage)
		{
			switch (tmpStage)
			{
			case 2:
				CarNameFadingTime += CTimer::ms_fTimeStep * 0.02 * 1000.0;
				if (CarNameFadingTime > 1000)
				{
					CarNameFadingTime = 1000;
					CarNameStage = 1;
				}
				alpha = CarNameFadingTime * 0.001 * 255.0;
				break;

			case 3:
				CarNameFadingTime += CTimer::ms_fTimeStep * 0.02 * -1000.0;
				if (CarNameFadingTime < 0)
				{
					CarNameFadingTime = 0;
					CarNameStage = 0;
				}
				alpha = CarNameFadingTime * 0.001 * 255.0;
				break;

			case 4:
				CarNameFadingTime += CTimer::ms_fTimeStep * 0.02 * -1000.0;
				if (CarNameFadingTime < 0)
				{
					CarNameShowingTime = 0;
					CarNameStage = 2;
					pCarNameToDisplay = tmpCarName;
					CarNameFadingTime = 0;
				}
				alpha = CarNameFadingTime * 0.001 * 255.0;
				break;

			case 1:
				if (CarNameShowingTime > 10000)
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
		if (!PriorityText[0])
		{
			CarNameShowingTime += CTimer::ms_fTimeStep * 0.02 * 1000.0;

			CFont::SetProportional(true);
			CFont::SetOrientation(ALIGN_Right);
			CFont::SetFontStyle(FONT_RageItalic);
			CFont::SetScale(_width(1.0f), _height(2.0f));
			CFont::SetDropShadowPosition(2);
#ifdef COMPILE_SLANTED_TEXT
#pragma message ("INFO: Compiling slanted text...")
			CFont::SetSlantRefPoint(RsGlobal.MaximumWidth, _ydown(108.0));
			CFont::SetSlant(0.15);
#endif
			CFont::SetDropColor(CRGBA(0, 0, 0, (BYTE)alpha));
			CFont::SetColor(CRGBA(BaseColors[4], (BYTE)alpha));
			if (*bWants_To_Draw_Hud && alpha)
				CFont::PrintString(_x((HUD_POS_X * 2.30f) + 52.0f), _ydown((HUD_POS_Y * 1.0f) + 88.0f), pCarNameToDisplay);

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
	if (m_pZoneName)
	{
		int tmpStage = m_ZoneState;
		float alpha = 0.0;

		if (m_pZoneName != m_pLastZoneName)
		{
			switch (m_ZoneState)
			{
			case 0:
				if (!*radarGrey && *bWants_To_Draw_Hud || *PlayerEnexEntryStage == 1 || *PlayerEnexEntryStage == 2)
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

		if (tmpStage)
		{
			switch (tmpStage)
			{
			case 1:
				alpha = 255.0;
				m_ZoneFadeTimer = 1000;

				if (m_ZoneNameTimer > 10000)
				{
					m_ZoneState = 3;
					m_ZoneFadeTimer = 1000;
				}
				break;
			case 2:
				if (!TheCamera.GetFading())
				{
					if (TheCamera.GetFadeStage() != 2)
						m_ZoneFadeTimer += CTimer::ms_fTimeStep * 0.02 * 1000.0;
				}
				if (m_ZoneFadeTimer > 1000)
				{
					m_ZoneFadeTimer = 1000;
					m_ZoneState = 1;
				}
				if (TheCamera.GetFadeStage() != 2)
					alpha = m_ZoneFadeTimer * 0.001 * 255.0;
				else
				{
					alpha = 255.0;
					m_ZoneState = 3;
					m_ZoneFadeTimer = 1000;
				}
				break;
			case 3:
				if (!TheCamera.GetFading())
				{
					if (TheCamera.GetFadeStage() != 2)
						m_ZoneFadeTimer += CTimer::ms_fTimeStep * 0.02 * -1000.0;
				}
				if (m_ZoneFadeTimer < 0)
				{
					m_ZoneFadeTimer = 0;
					m_ZoneState = 0;
				}
				if (TheCamera.GetFadeStage() != 2)
					alpha = m_ZoneFadeTimer * 0.001 * 255.0;
				else
				{
					m_ZoneFadeTimer = 1000;
					alpha = 255.0;
				}
				break;
			case 4:
				m_ZoneFadeTimer += CTimer::ms_fTimeStep * 0.02 * -1000.0;

				if (m_ZoneFadeTimer < 0)
				{
					m_ZoneFadeTimer = 0;
					m_ZoneState = 2;
					m_ZoneToPrint = m_pZoneName;
				}

				alpha = m_ZoneFadeTimer * 0.001 * 255.0;
				break;
			}
			if (PriorityText[0] || *(float*)0xBAA3E0 != 0.0 || *(float*)0xBAA3E4 != 0.0)
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
				CFont::SetScale(_width(1.0f), _height(2.0f));
				CFont::SetColor(CRGBA(BaseColors[4], alpha));
				CFont::PrintString(_x((HUD_POS_X * 2.30f) + 52.0f), _ydown((HUD_POS_Y * 1.0f) + 56.0f), m_ZoneToPrint);
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
	if (bHideOnFade == *bHideStyledTextWhileFading)
	{
		// Big message 5
		if (!StyledText_2[0] && StyledText_5[0])
		{
		}
	}

	if (bHideOnFade)
	{
		// Styled text 7
		if (StyledText_7[0])
		{
			CFont::SetBackground(0, 0);
			CFont::SetJustify(false);
			CFont::SetScaleLang(_width(1.0), _height(2.0));
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
		if (StyledText_4[0])
		{
			CFont::SetBackground(0, 0);
			CFont::SetJustify(false);
			CFont::SetScaleLang(_width(1.0f), _height(2.0f));
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
	CFont::SetScaleLang(_width(1.4f), _height(2.2f));
	CFont::SetOrientation(ALIGN_Center);
	CFont::SetProportional(true);
	CFont::SetCentreSize(_width(RsGlobal.MaximumWidth - 50.0f));
	CFont::SetFontStyle(FONT_Pricedown);
	CFont::SetDropShadowPosition(3);
}

void CHud::DrawBigMessage2() {
	CFont::SetBackground(0, 0);
	CFont::SetProportional(true);
	CFont::SetRightJustifyWrap(0.0);
	CFont::SetOrientation(ALIGN_Right);
	CFont::SetFontStyle(FONT_RageItalic);
	CFont::SetScale(_width(1.2f), _height(2.4f));
	CFont::SetDropShadowPosition(2);
	CFont::SetDropColor(CRGBA(0, 0, 0, 255));
	CFont::SetColor(CRGBA(0xDA, 0xA8, 0x02, 255));
	CFont::PrintString(_x((HUD_POS_X * 2.30f) + 52.0f), _ydown((HUD_POS_Y * 1.0f) + 58.0f), StyledText_2);
}

void CHud::DrawBigMessage3()
{
	CFont::SetBackground(0, 0);
	CFont::SetScale(_width(1.4), _height(2.2));
	CFont::SetProportional(true);
	CFont::SetJustify(false);
	CFont::SetOrientation(ALIGN_Center);
	CFont::SetFontStyle(FONT_Pricedown);
	CFont::SetDropShadowPosition(3);
	CFont::SetDropColor(CRGBA(0, 0, 0, BigMessage3Alpha));
	CFont::SetColor(CRGBA(BaseColors[10], BigMessage3Alpha));
	CFont::PrintStringFromBottom(RsGlobal.MaximumWidth / 2, _ymiddle(-30.0), StyledText_3);
}

void CHud::DrawSubtitles(float x, float y, char* str) {
	float fPositionX, fPositionY;

	if (TheCamera.m_WideScreenOn) {
		fPositionX = RsGlobal.MaximumWidth * 0.50f;
		fPositionY = 71.5f;
		CFont::SetCentreSize(RsGlobal.MaximumWidth - _xleft(280.0f));
	}
	else {
		if (CFont::GetStringWidth(str, true, false) > (RsGlobal.MaximumWidth * 0.45f) * ScreenAspectRatio)
			fPositionX = RsGlobal.MaximumWidth * 0.55f;
		else
			fPositionX = RsGlobal.MaximumWidth * 0.50f;

		fPositionY = 64.5f;

		CFont::SetCentreSize(RsGlobal.MaximumWidth - _xleft(260.0f));
	}

	CFont::SetEdge(1);
	CFont::SetScale(_width(0.5f), _height(1.0f));

	CFont::PrintStringFromBottom(fPositionX, _ydown(((HUD_POS_Y * 1.0f) * 3.6f) + fPositionY), str);
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

	Sprites[HUD_BarOutline].Draw(CRect(fX - _width(2.0f), fY - _height(1.5f), fX + wWidth + _width(2.0f), fY + wHeight + _height(1.5f)), CRGBA(0, 0, 0, dwColor.a + 8));
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, FALSE);
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

void CHud::SetFontScale(float w, float h) {
	m_vecFontScale.x = w;
	m_vecFontScale.y = h;
}

void CHud::PrintString(float x, float y, std::string text, CRGBA color) {
	const float fSpriteWidth(512), fSpriteHeight(256);

	float fFontScale(m_vecFontScale.x), fontHeight(m_vecFontScale.y);
	CRect rect;
	rect.x1 = x;

	Sprites[HUD_NUMBERS].SetRenderState();

	for (int i = 0; i < text.length(); i++) {
		float fCharWidth(52), fCharHeight(64), fSpacing(0.25);
		char chr = text[i] - 48;

		if (text[i] == ':')
			fCharWidth = 32;
		if (text[i] == 'a') {
			fCharWidth = 64;
			chr = 11;
		}
		if (text[i] == 'p') {
			fCharWidth = 64;
			chr = 12;
		}
		if (text[i] == '+')
			chr = 13;
		if (text[i] == '$')
			chr = 21;
		if (text[i] == '*') {
			chr = 22;
			fCharWidth *= 2.0;
			fCharHeight *= 2.0;
		}

		rect.x2 = rect.x1 + (fCharWidth / 4) * fFontScale;
		rect.y1 = y;
		rect.y2 = y + 20.0 * fontHeight;
		float U1lef = (64 / fSpriteWidth) * (chr % 8);
		float V3top = (68 / fSpriteHeight) * (chr / 8);
		float U2rig = U1lef + fCharWidth / fSpriteWidth;
		float V4top = V3top;
		float U3lef = U1lef;
		float V1bot = V3top + fCharHeight / fSpriteHeight;
		float U4rig = U2rig;
		float V2bot = V1bot;
		CSprite2d::AddToBuffer(rect, &color, U1lef, V1bot, U2rig, V2bot, U3lef, V3top, U4rig, V4top);
		rect.x1 += (fCharWidth / 4 + fSpacing) * fFontScale;
	}
	CSprite2d::RenderVertexBuffer();
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, reinterpret_cast<void*>(0));
}