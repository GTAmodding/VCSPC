#include "StdAfx.h"
#include "Pad.h"

#include "X360Pad.h"
#include "Timer.h"
#include "Frontend.h"
#include "Ped.h"
#include "Camera.h"
#include "Vehicle.h"
#include "Font.h"
#include "ControlsMgr.h"

CMouseControllerState&	CPad::PCTempMouseControllerState = *(CMouseControllerState*)0xB73404;
CMouseControllerState&	CPad::NewMouseControllerState = *(CMouseControllerState*)0xB73418;
CMouseControllerState&	CPad::OldMouseControllerState = *(CMouseControllerState*)0xB7342C;

static CPad* const	Pads = (CPad*)0xB73458;

static CMouseControllerState&	pNewMouseControllerState = *(CMouseControllerState*)0xB73418;

bool	CPad::bSouthpaw;
bool	CPad::bInvertLook4Pad = true;
WORD	CPad::SavedMode;
bool	CPad::FailCameraChangeThisFrame = false;
short	CPad::ChangeStation_HoldTimer;
short	CPad::ChangeCamera_HoldTimer;

CX360Pad*	pXboxPad[1];

WRAPPER void CPad::UpdatePads() { EAXJMP(0x541DD0); }
WRAPPER void CPad::StartShake(short nTime, unsigned char nDur, unsigned int nNoShakeBeforeThis) { EAXJMP(0x53F920); }

// TEMP
bool bPadLeftAxisSwapped[1], bPadRightAxisSwapped[1];
bool bPadLeftXInverted[1], bPadLeftYInverted[1];
bool bPadRightXInverted[1], bPadRightYInverted[1];

float fLeftStickDeadzone[1], fRightStickDeadzone[1];
float fLeftStickSensitivity[1], fRightStickSensitivity[1];

float fFaceButtonSensitivity[1];

bool bSwapSticksDuringAiming[1];

bool bFreeAim;

float fSniperZoomVal;

void LoadINIFile()
{
	wchar_t*	wcModulePath = L".\\GInput.ini";

	FrontEndMenuManager.m_bVibrationEnabled = GetPrivateProfileIntW(L"GInput", L"Vibration", FALSE, wcModulePath) != FALSE;

	//CFont::bX360Buttons = GetPrivateProfileIntW(L"GInput", L"PlayStationButtons", FALSE, wcModulePath) == FALSE;
	/*bApplyMissionFixes = GetPrivateProfileIntW(L"GInput", L"ApplyMissionSpecificFixes", TRUE, wcModulePath) != FALSE;
	bApplyGXTFixes = bApplyMissionFixes && GetPrivateProfileIntW(L"GInput", L"ApplyGXTFixes", TRUE, wcModulePath) != FALSE;
	bCheatsFromPad = GetPrivateProfileIntW(L"GInput", L"CheatsFromPad", TRUE, wcModulePath) != FALSE;
	bDisableOnFocusLost = GetPrivateProfileIntW(L"GInput", L"DisableOnFocusLost", TRUE, wcModulePath) != FALSE;*/

	/*bMapPadOneToPadTwo = GetPrivateProfileIntW(L"GInput", L"MapPadOneToPadTwo", FALSE, wcModulePath) != FALSE;
	bFreeAim_User = GetPrivateProfileIntW(L"GInput", L"FreeAim", FALSE, wcModulePath) != FALSE;*/

	// Pad specific options
	for ( int i = 0; i < 1; i++ )
	{
		wchar_t		wcSectionName[] = { 'P', 'a', 'd', static_cast<wchar_t>('1' + i), '\0' };

		/*if ( bMapPadOneToPadTwo )
			i = 1;*/

		bSwapSticksDuringAiming[i] = GetPrivateProfileIntW(wcSectionName, L"SwapSticksDuringAiming", FALSE, wcModulePath) != FALSE;		
		CPad::SavedMode = static_cast<WORD>(GetPrivateProfileIntW(wcSectionName, L"ControlsSet", 1, wcModulePath) - 1);
		//bInvertLook[i] = GetPrivateProfileIntW(wcSectionName, L"InvertLook", FALSE, wcModulePath) == FALSE;

		bPadLeftXInverted[i] = GetPrivateProfileIntW(wcSectionName, L"InvertLeftXAxis", FALSE, wcModulePath) != FALSE;
		bPadLeftYInverted[i] = GetPrivateProfileIntW(wcSectionName, L"InvertLeftYAxis", FALSE, wcModulePath) != FALSE;
		bPadLeftAxisSwapped[i] = GetPrivateProfileIntW(wcSectionName, L"SwapLeftAxes", FALSE, wcModulePath) != FALSE;

		bPadRightXInverted[i] = GetPrivateProfileIntW(wcSectionName, L"InvertRightXAxis", FALSE, wcModulePath) != FALSE;
		bPadRightYInverted[i] = GetPrivateProfileIntW(wcSectionName, L"InvertRightYAxis", FALSE, wcModulePath) != FALSE;
		bPadRightAxisSwapped[i] = GetPrivateProfileIntW(wcSectionName, L"SwapRightAxes", FALSE, wcModulePath) != FALSE;

		//bSouthpaw[i] = GetPrivateProfileIntW(wcSectionName, L"Southpaw", FALSE, wcModulePath) != FALSE;

		// SCP options
		unsigned int nFaceSensitivity = GetPrivateProfileIntW(wcSectionName, L"FaceButtonsSensitivity", 50, wcModulePath);
		if ( nFaceSensitivity == 0 )
			fFaceButtonSensitivity[i] = std::numeric_limits<float>::infinity();
		else
		{
			fFaceButtonSensitivity[i] = 25500.0f / nFaceSensitivity;
			if ( fFaceButtonSensitivity[i] < 255.0f )
				fFaceButtonSensitivity[i] = 255.0f;
		}

		fLeftStickDeadzone[i] = static_cast<float>(GetPrivateProfileIntW(wcSectionName, L"LeftStickDeadzone", 24, wcModulePath)) * 0.01f;
		fLeftStickSensitivity[i] = static_cast<float>(GetPrivateProfileIntW(wcSectionName, L"LeftStickSensitivity", 100, wcModulePath)) * 0.01f;

		fRightStickDeadzone[i] = static_cast<float>(GetPrivateProfileIntW(wcSectionName, L"RightStickDeadzone", 27, wcModulePath)) * 0.01f;
		fRightStickSensitivity[i] = static_cast<float>(GetPrivateProfileIntW(wcSectionName, L"RightStickSensitivity", 100, wcModulePath)) * 0.01f;

		// Limit those
		if ( fLeftStickDeadzone[i] > 0.75f )
			fLeftStickDeadzone[i] = 0.75f;
		else if ( fLeftStickDeadzone[i] < 0.0f )
			fLeftStickDeadzone[i] = 0.0f;

		if ( fRightStickDeadzone[i] > 0.75f )
			fRightStickDeadzone[i] = 0.75f;
		else if ( fRightStickDeadzone[i] < 0.0f )
			fRightStickDeadzone[i] = 0.0f;

		if ( fLeftStickSensitivity[i] > 1.0f )
			fLeftStickSensitivity[i] = 1.0f;
		else if ( fLeftStickSensitivity[i] < 0.0f )
			fLeftStickSensitivity[i] = 0.0f;

		if ( fRightStickSensitivity[i] > 1.0f )
			fRightStickSensitivity[i] = 1.0f;
		else if ( fRightStickSensitivity[i] < 0.0f )
			fRightStickSensitivity[i] = 0.0f;

		// TODO: Temp
		//fSixaxisSteeringSensitivity[i] = static_cast<float>(GetPrivateProfileIntW(wcSectionName, L"SixaxisSensitivity", 150, wcModulePath)) * 0.01f;

/*#ifdef GINPUT_COMPILE_SA_VERSION
		bSixaxisReloading[i] = GetPrivateProfileIntW(wcSectionName, L"SixaxisReloading", FALSE, wcModulePath) != FALSE;
		bSixaxisHydraulics[i] = GetPrivateProfileIntW(wcSectionName, L"SixaxisHydraulics", FALSE, wcModulePath) != FALSE;
#endif
		bSixaxisCar[i] = GetPrivateProfileIntW(wcSectionName, L"SixaxisCarSteering", FALSE, wcModulePath) != FALSE;
#ifndef GINPUT_COMPILE_III_VERSION
		bSixaxisBike[i] = GetPrivateProfileIntW(wcSectionName, L"SixaxisBikeSteering", FALSE, wcModulePath) != FALSE;
#endif
		bSixaxisBoat[i] = GetPrivateProfileIntW(wcSectionName, L"SixaxisBoatSteering", FALSE, wcModulePath) != FALSE;
		bSixaxisHeli[i] = GetPrivateProfileIntW(wcSectionName, L"SixaxisHeliSteering", FALSE, wcModulePath) != FALSE;
		bSixaxisPlane[i] = GetPrivateProfileIntW(wcSectionName, L"SixaxisPlaneSteering", FALSE, wcModulePath) != FALSE;*/
	}

	//return GetPrivateProfileIntW(L"GInput", L"GuideLaunchesOverlay", FALSE, wcModulePath) != FALSE;
}

void OnModeChangePatches()
{
	using namespace Memory;

	if ( CPad::SavedMode == PAD_IV_CONTROLS_MODE )
	{
		// Circle/B goes back in menu
		Patch<BYTE>(0x58035B, offsetof(CPad, OldState.CIRCLE));
		Patch<BYTE>(0x580362, offsetof(CPad, NewState.CIRCLE));
		Patch<BYTE>(0x57F220, offsetof(CPad, OldState.CIRCLE));
		Patch<BYTE>(0x57F227, offsetof(CPad, NewState.CIRCLE));

		// Waypoint marker
		Patch<BYTE>(0x577496, offsetof(CPad, NewState.SQUARE));
		Patch<BYTE>(0x57749D, offsetof(CPad, OldState.SQUARE));
	}
	else
	{
		Patch<BYTE>(0x58035B, offsetof(CPad, OldState.TRIANGLE));
		Patch<BYTE>(0x580362, offsetof(CPad, NewState.TRIANGLE));
		Patch<BYTE>(0x57F220, offsetof(CPad, OldState.TRIANGLE));
		Patch<BYTE>(0x57F227, offsetof(CPad, NewState.TRIANGLE));

		// Waypoint marker
		Patch<BYTE>(0x577496, offsetof(CPad, NewState.CIRCLE));
		Patch<BYTE>(0x57749D, offsetof(CPad, OldState.CIRCLE));
	}
}

static void InitXInputPad()
{
	pXboxPad[0] = new CX360Pad(0);
	LoadINIFile();
	OnModeChangePatches();
}

static inline short PressureScale(float fButtonVal, int nPadID)
{
	fButtonVal *= fFaceButtonSensitivity[nPadID];
	if ( fButtonVal > 255.0f )
		return 255;
	return static_cast<short>(fButtonVal);
}

static void CaptureXInputPad(int nPadID)
{
	/*if ( !bGameInFocus && bDisableOnFocusLost )
		return;*/

	if ( pXboxPad[nPadID]->RetrieveState() == ERROR_SUCCESS )
	{
		CPad*		pVCPad = CPad::GetPad(nPadID);
		XINPUT_GAMEPAD*	pGamepad = pXboxPad[nPadID]->GetPadState();

		double			dStickState[2];

		// Save input from a previous frame for cheat handling
		/*if ( nPadID == 0 )
			pVCPad->OldJoyState = pVCPad->NewJoyState;*/

		// Left thumbstick
		dStickState[0] = pGamepad->sThumbLX * (1.0/32767.0);
		dStickState[1] = pGamepad->sThumbLY * (1.0/32767.0);

		// Apply deadzone
		if ( std::abs(dStickState[0]) >= fLeftStickDeadzone[nPadID] )
		{
			if ( dStickState[0] > 0.0 )
				dStickState[0] -= fLeftStickDeadzone[nPadID];
			else
				dStickState[0] += fLeftStickDeadzone[nPadID];

			dStickState[0] /= (1.0-fLeftStickDeadzone[nPadID]);
		}
		else
			dStickState[0] = 0.0;

		if ( std::abs(dStickState[1]) >= fLeftStickDeadzone[nPadID] )
		{
			if ( dStickState[1] > 0.0 )
				dStickState[1] -= fLeftStickDeadzone[nPadID];
			else
				dStickState[1] += fLeftStickDeadzone[nPadID];

			dStickState[1] /= (1.0-fLeftStickDeadzone[nPadID]);
		}
		else
			dStickState[1] = 0.0;

		// Apply Bugbear's sensitivity
		dStickState[0] = (dStickState[0] * dStickState[0] * dStickState[0] * (1.0f - fLeftStickSensitivity[nPadID])) + (fLeftStickSensitivity[nPadID] * dStickState[0]);

		if ( bPadLeftXInverted[nPadID] )
			dStickState[0] = -dStickState[0];

		if ( !bPadLeftYInverted[nPadID] )
			dStickState[1] = -dStickState[1];

		// Just to make sure I didn't fuck up
		//assert(dStickState[0] >= -1.0 && dStickState[0] <= 1.0);
		//assert(dStickState[1] >= -1.0 && dStickState[1] <= 1.0);
		// Ash breaks on those, so guess it's better to normalize
		if ( dStickState[0] > 1.0 )
			dStickState[0] = 1.0;
		else if ( dStickState[0] < -1.0 )
			dStickState[0] = -1.0;
		if ( dStickState[1] > 1.0 )
			dStickState[1] = 1.0;
		else if ( dStickState[1] < -1.0 )
			dStickState[1] = -1.0;

		pVCPad->PCTempJoyState.LEFTSTICKX = static_cast<short>((bPadLeftAxisSwapped[nPadID] ? dStickState[1] : dStickState[0]) * 127.0f);
		pVCPad->PCTempJoyState.LEFTSTICKY = static_cast<short>((bPadLeftAxisSwapped[nPadID] ? dStickState[0] : dStickState[1]) * 127.0f);

		// Right thumbstick
		dStickState[0] = pGamepad->sThumbRX * (1.0/32767.0);
		dStickState[1] = pGamepad->sThumbRY * (1.0/32767.0);

		// Apply deadzone
		if ( std::abs(dStickState[0]) >= fRightStickDeadzone[nPadID] )
		{
			if ( dStickState[0] > 0.0 )
				dStickState[0] -= fRightStickDeadzone[nPadID];
			else
				dStickState[0] += fRightStickDeadzone[nPadID];

			dStickState[0] /= (1.0-fRightStickDeadzone[nPadID]);
		}
		else
			dStickState[0] = 0.0;

		if ( std::abs(dStickState[1]) >= fRightStickDeadzone[nPadID] )
		{
			if ( dStickState[1] > 0.0 )
				dStickState[1] -= fRightStickDeadzone[nPadID];
			else
				dStickState[1] += fRightStickDeadzone[nPadID];

			dStickState[1] /= (1.0-fRightStickDeadzone[nPadID]);
		}
		else
			dStickState[1] = 0.0;

		// Apply Bugbear's sensitivity
		dStickState[0] = (dStickState[0] * dStickState[0] * dStickState[0] * (1.0f - fRightStickSensitivity[nPadID])) + (fRightStickSensitivity[nPadID] * dStickState[0]);

		if ( bPadRightXInverted[nPadID] )
			dStickState[0] = -dStickState[0];

		if ( !bPadRightYInverted[nPadID] )
			dStickState[1] = -dStickState[1];

		// Just to make sure I didn't fuck up
		//assert(dStickState[0] >= -1.0 && dStickState[0] <= 1.0);
		//assert(dStickState[1] >= -1.0 && dStickState[1] <= 1.0);
		if ( dStickState[0] > 1.0 )
			dStickState[0] = 1.0;
		else if ( dStickState[0] < -1.0 )
			dStickState[0] = -1.0;
		if ( dStickState[1] > 1.0 )
			dStickState[1] = 1.0;
		else if ( dStickState[1] < -1.0 )
			dStickState[1] = -1.0;

		pVCPad->PCTempJoyState.RIGHTSTICKX = static_cast<short>((bPadRightAxisSwapped[nPadID] ? dStickState[1] : dStickState[0]) * 127.0f);
		pVCPad->PCTempJoyState.RIGHTSTICKY = static_cast<short>((bPadRightAxisSwapped[nPadID] ? dStickState[0] : dStickState[1]) * 127.0f);

		// Triggers
		float		nTriggerVal;

		nTriggerVal = pGamepad->bLeftTrigger;
		pVCPad->PCTempJoyState.LEFTSHOULDER2 = static_cast<short>(nTriggerVal > XINPUT_GAMEPAD_TRIGGER_THRESHOLD ? (nTriggerVal-XINPUT_GAMEPAD_TRIGGER_THRESHOLD) * (255.0f/(255.0f-XINPUT_GAMEPAD_TRIGGER_THRESHOLD)) : 0);

		nTriggerVal = pGamepad->bRightTrigger;
		pVCPad->PCTempJoyState.RIGHTSHOULDER2 = static_cast<short>(nTriggerVal > XINPUT_GAMEPAD_TRIGGER_THRESHOLD ? (nTriggerVal-XINPUT_GAMEPAD_TRIGGER_THRESHOLD) * (255.0f/(255.0f-XINPUT_GAMEPAD_TRIGGER_THRESHOLD)) : 0);

		// Start/Select
		pVCPad->PCTempJoyState.START = pGamepad->wButtons & XINPUT_GAMEPAD_START ? 255 : 0;
		pVCPad->PCTempJoyState.SELECT = pGamepad->wButtons & XINPUT_GAMEPAD_BACK ? 255 : 0;

		// DPad
		pVCPad->PCTempJoyState.DPADUP = pGamepad->wButtons & XINPUT_GAMEPAD_DPAD_UP ? 255 : 0;
		pVCPad->PCTempJoyState.DPADLEFT = pGamepad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT ? 255 : 0;
		pVCPad->PCTempJoyState.DPADDOWN = pGamepad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN ? 255 : 0;
		pVCPad->PCTempJoyState.DPADRIGHT = pGamepad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT ? 255 : 0;

		// Face buttons
		pVCPad->PCTempJoyState.CROSS = pGamepad->wButtons & XINPUT_GAMEPAD_A ? 255 : 0;
		pVCPad->PCTempJoyState.CIRCLE = pGamepad->wButtons & XINPUT_GAMEPAD_B ? 255 : 0;
		pVCPad->PCTempJoyState.TRIANGLE = pGamepad->wButtons & XINPUT_GAMEPAD_Y ? 255 : 0;
		pVCPad->PCTempJoyState.SQUARE = pGamepad->wButtons & XINPUT_GAMEPAD_X ? 255 : 0;

		// Bumper buttons
		pVCPad->PCTempJoyState.LEFTSHOULDER1 = pGamepad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER ? 255 : 0;
		pVCPad->PCTempJoyState.LEFTSHOCK = pGamepad->wButtons & XINPUT_GAMEPAD_LEFT_THUMB ? 255 : 0;
		pVCPad->PCTempJoyState.RIGHTSHOULDER1 = pGamepad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER ? 255 : 0;
		pVCPad->PCTempJoyState.RIGHTSHOCK = pGamepad->wButtons & XINPUT_GAMEPAD_RIGHT_THUMB ? 255 : 0;

		// Secret button!!!
		pVCPad->PCTempJoyState.HOME = pGamepad->wButtons & 0x0400 ? 255 : 0;

		// Cheats - pad 1 only
		/*if ( nPadID == 0 )
			pVCPad->NewJoyState.Update(pVCPad->PCTempJoyState);*/

		// Rumble
		if ( pVCPad->ShakeDur )
		{
			if ( pXboxPad[nPadID]->HasPadInHands() && pVCPad->InputHowLongAgo() <= PADS_IDLE_TIME )
			{
				pXboxPad[nPadID]->Vibrate(pVCPad->ShakeFreq * 257, 0);
				pVCPad->ShakeDur -= static_cast<short>(CTimer::ms_fTimeStep * (0.02f * 1000.0f));

				if ( pVCPad->ShakeDur <= 0 )
				{
					pVCPad->ShakeDur = 0;
					pXboxPad[nPadID]->Vibrate(0, 0);
				}
			}
			else
			{
				pVCPad->ShakeDur = 0;
				pXboxPad[nPadID]->Vibrate(0, 0);
			}
		}
	}

}

static void CaptureXInputPad_SCP(int nPadID)
{
	/*if ( !bGameInFocus && bDisableOnFocusLost )
		return;*/

	DWORD dwState = pXboxPad[nPadID]->RetrieveState_SCP();
	if ( dwState == ERROR_SUCCESS )
	{
		CPad*		pVCPad = CPad::GetPad(nPadID);

		// Fetch SCP data
		SCP_EXTN*	pPadData = pXboxPad[nPadID]->GetPadState_SCP();

		double			dStickState[2];

		// Save input from a previous frame for cheat handling
		/*if ( nPadID == 0 )
			pVCPad->OldJoyState = pVCPad->NewJoyState;*/

		// Left thumbstick
		dStickState[0] = pPadData->SCP_LX;
		dStickState[1] = pPadData->SCP_LY;

		// Apply deadzone
		if ( std::abs(dStickState[0]) >= fLeftStickDeadzone[nPadID] )
		{
			if ( dStickState[0] > 0.0 )
				dStickState[0] -= fLeftStickDeadzone[nPadID];
			else
				dStickState[0] += fLeftStickDeadzone[nPadID];

			dStickState[0] /= (1.0-fLeftStickDeadzone[nPadID]);
		}
		else
			dStickState[0] = 0.0;

		if ( std::abs(dStickState[1]) >= fLeftStickDeadzone[nPadID] )
		{
			if ( dStickState[1] > 0.0 )
				dStickState[1] -= fLeftStickDeadzone[nPadID];
			else
				dStickState[1] += fLeftStickDeadzone[nPadID];

			dStickState[1] /= (1.0-fLeftStickDeadzone[nPadID]);
		}
		else
			dStickState[1] = 0.0;

		// Apply Bugbear's sensitivity
		dStickState[0] = (dStickState[0] * dStickState[0] * dStickState[0] * (1.0f - fLeftStickSensitivity[nPadID])) + (fLeftStickSensitivity[nPadID] * dStickState[0]);

		if ( bPadLeftXInverted[nPadID] )
			dStickState[0] = -dStickState[0];

		if ( !bPadLeftYInverted[nPadID] )
			dStickState[1] = -dStickState[1];

		// Just to make sure I didn't fuck up
		assert(dStickState[0] >= -1.0 && dStickState[0] <= 1.0);
		assert(dStickState[1] >= -1.0 && dStickState[1] <= 1.0);

		pVCPad->PCTempJoyState.LEFTSTICKX = static_cast<short>((bPadLeftAxisSwapped[nPadID] ? dStickState[1] : dStickState[0]) * 127.0f);
		pVCPad->PCTempJoyState.LEFTSTICKY = static_cast<short>((bPadLeftAxisSwapped[nPadID] ? dStickState[0] : dStickState[1]) * 127.0f);

		// Right thumbstick
		dStickState[0] = pPadData->SCP_RX;
		dStickState[1] = pPadData->SCP_RY;

		// Apply deadzone
		if ( std::abs(dStickState[0]) >= fRightStickDeadzone[nPadID] )
		{
			if ( dStickState[0] > 0.0 )
				dStickState[0] -= fRightStickDeadzone[nPadID];
			else
				dStickState[0] += fRightStickDeadzone[nPadID];

			dStickState[0] /= (1.0-fRightStickDeadzone[nPadID]);
		}
		else
			dStickState[0] = 0.0;

		if ( std::abs(dStickState[1]) >= fRightStickDeadzone[nPadID] )
		{
			if ( dStickState[1] > 0.0 )
				dStickState[1] -= fRightStickDeadzone[nPadID];
			else
				dStickState[1] += fRightStickDeadzone[nPadID];

			dStickState[1] /= (1.0-fRightStickDeadzone[nPadID]);
		}
		else
			dStickState[1] = 0.0;

		// Apply Bugbear's sensitivity
		dStickState[0] = (dStickState[0] * dStickState[0] * dStickState[0] * (1.0f - fRightStickSensitivity[nPadID])) + (fRightStickSensitivity[nPadID] * dStickState[0]);

		if ( bPadRightXInverted[nPadID] )
			dStickState[0] = -dStickState[0];

		if ( !bPadRightYInverted[nPadID] )
			dStickState[1] = -dStickState[1];

		// Just to make sure I didn't fuck up
		assert(dStickState[0] >= -1.0 && dStickState[0] <= 1.0);
		assert(dStickState[1] >= -1.0 && dStickState[1] <= 1.0);

		pVCPad->PCTempJoyState.RIGHTSTICKX = static_cast<short>((bPadRightAxisSwapped[nPadID] ? dStickState[1] : dStickState[0]) * 127.0f);
		pVCPad->PCTempJoyState.RIGHTSTICKY = static_cast<short>((bPadRightAxisSwapped[nPadID] ? dStickState[0] : dStickState[1]) * 127.0f);

		// Triggers
		pVCPad->PCTempJoyState.LEFTSHOULDER2 = static_cast<short>(pPadData->SCP_L2 > (XINPUT_GAMEPAD_TRIGGER_THRESHOLD/255.0f) ? (pPadData->SCP_L2-(XINPUT_GAMEPAD_TRIGGER_THRESHOLD/255.0f)) * (255.0f/(1.0f-(XINPUT_GAMEPAD_TRIGGER_THRESHOLD/255.0f))) : 0);
		pVCPad->PCTempJoyState.RIGHTSHOULDER2 = static_cast<short>(pPadData->SCP_R2 > (XINPUT_GAMEPAD_TRIGGER_THRESHOLD/255.0f) ? (pPadData->SCP_R2-(XINPUT_GAMEPAD_TRIGGER_THRESHOLD/255.0f)) * (255.0f/(1.0f-(XINPUT_GAMEPAD_TRIGGER_THRESHOLD/255.0f))) : 0);

		// Start/Select
		pVCPad->PCTempJoyState.START = PressureScale(pPadData->SCP_START, nPadID);
		pVCPad->PCTempJoyState.SELECT = PressureScale(pPadData->SCP_SELECT, nPadID);

		// DPad
		pVCPad->PCTempJoyState.DPADUP = PressureScale(pPadData->SCP_UP, nPadID);
		pVCPad->PCTempJoyState.DPADLEFT = PressureScale(pPadData->SCP_LEFT, nPadID);
		pVCPad->PCTempJoyState.DPADDOWN = PressureScale(pPadData->SCP_DOWN, nPadID);
		pVCPad->PCTempJoyState.DPADRIGHT = PressureScale(pPadData->SCP_RIGHT, nPadID);

		// Face buttons
		pVCPad->PCTempJoyState.CROSS = PressureScale(pPadData->SCP_X, nPadID);
		pVCPad->PCTempJoyState.CIRCLE = PressureScale(pPadData->SCP_C, nPadID);
		pVCPad->PCTempJoyState.TRIANGLE = PressureScale(pPadData->SCP_T, nPadID);
		pVCPad->PCTempJoyState.SQUARE = PressureScale(pPadData->SCP_S, nPadID);

		// Bumper buttons
		pVCPad->PCTempJoyState.LEFTSHOULDER1 = PressureScale(pPadData->SCP_L1, nPadID);
		pVCPad->PCTempJoyState.LEFTSHOCK = PressureScale(pPadData->SCP_L3, nPadID);
		pVCPad->PCTempJoyState.RIGHTSHOULDER1 = PressureScale(pPadData->SCP_R1, nPadID);
		pVCPad->PCTempJoyState.RIGHTSHOCK = PressureScale(pPadData->SCP_R3, nPadID);

		// Secret button!!!
		pVCPad->PCTempJoyState.HOME = PressureScale(pPadData->SCP_PS, nPadID);

		// Sixaxis
		/*SCP_SIXAXIS*	pSixaxis = pXboxPad[nPadID]->GetSixaxisState();

		pVCPad->OldSixaxisState[nPadID] = pVCPad->NewSixaxisState[nPadID];

		short	nAccelX = -(pSixaxis->sAccelerometerX - SIXAXIS_IDLE_X);
		short	nAccelY = pSixaxis->sAccelerometerY - SIXAXIS_IDLE_Y;
		short	nAccelZ = pSixaxis->sAccelerometerZ - SIXAXIS_IDLE_Z;
		short	nGyro =  pSixaxis->sGyroscope - SIXAXIS_IDLE_GYRO;

		// Sixaxis deadzones
		if ( std::abs(nAccelX) > SIXAXIS_X_DEADZONE )
		{
			if ( nAccelX > 0 )
				nAccelX -= SIXAXIS_X_DEADZONE;
			else
				nAccelX += SIXAXIS_X_DEADZONE;

			pVCPad->NewSixaxisState[nPadID].ACCEL_X = nAccelX * (100.0f/(100.0f-SIXAXIS_X_DEADZONE));
		}
		else
			pVCPad->NewSixaxisState[nPadID].ACCEL_X = 0;

		if ( std::abs(nAccelY) > SIXAXIS_Y_DEADZONE )
		{
			if ( nAccelY > 0 )
				nAccelY -= SIXAXIS_Y_DEADZONE;
			else
				nAccelY += SIXAXIS_Y_DEADZONE;

			pVCPad->NewSixaxisState[nPadID].ACCEL_Y = nAccelY * (100.0f/(100.0f-SIXAXIS_Y_DEADZONE));
		}
		else
			pVCPad->NewSixaxisState[nPadID].ACCEL_Y = 0;

		if ( std::abs(nAccelZ) > SIXAXIS_Z_DEADZONE )
		{
			if ( nAccelZ > 0 )
				nAccelZ -= SIXAXIS_Z_DEADZONE;
			else
				nAccelZ += SIXAXIS_Z_DEADZONE;

			pVCPad->NewSixaxisState[nPadID].ACCEL_Z = nAccelZ * (100.0f/(100.0f-SIXAXIS_Z_DEADZONE));
		}
		else
			pVCPad->NewSixaxisState[nPadID].ACCEL_Z = 0;

		if ( std::abs(nGyro) > SIXAXIS_GYRO_DEADZONE )
		{
			if ( nGyro > 0 )
				nGyro -= SIXAXIS_GYRO_DEADZONE;
			else
				nGyro += SIXAXIS_GYRO_DEADZONE;

			pVCPad->NewSixaxisState[nPadID].GYRO = nGyro;
		}
		else
			pVCPad->NewSixaxisState[nPadID].GYRO = 0;

		if ( !(pVCPad->NewSixaxisState[nPadID] == pVCPad->OldSixaxisState[nPadID]) )
		{
			SIXAXIS_INPUT	tempSixaxis;
			tempSixaxis.ACCEL_X = pVCPad->NewSixaxisState[nPadID].ACCEL_X;
			tempSixaxis.ACCEL_Y = pVCPad->NewSixaxisState[nPadID].ACCEL_Y;
			tempSixaxis.ACCEL_Z = pVCPad->NewSixaxisState[nPadID].ACCEL_Z;
			tempSixaxis.GYRO = pVCPad->NewSixaxisState[nPadID].GYRO;

			CX360Pad::LaunchSixaxisFetchCallbacks(tempSixaxis, nPadID);
		}

		// Cheats - pad 1 only
		if ( nPadID == 0 )
			pVCPad->NewJoyState.Update(pVCPad->PCTempJoyState);*/

		// Rumble
		if ( pVCPad->ShakeDur )
		{
			if ( pXboxPad[nPadID]->HasPadInHands() && pVCPad->InputHowLongAgo() <= PADS_IDLE_TIME )
			{
				pXboxPad[nPadID]->Vibrate(pVCPad->ShakeFreq * 257, 0);
				pVCPad->ShakeDur -= static_cast<short>(CTimer::ms_fTimeStep * (0.02f * 1000.0f));

				if ( pVCPad->ShakeDur <= 0 )
				{
					pVCPad->ShakeDur = 0;
					pXboxPad[nPadID]->Vibrate(0, 0);
				}
			}
			else
			{
				pVCPad->ShakeDur = 0;
				pXboxPad[nPadID]->Vibrate(0, 0);
			}
		}
	}
	else if ( dwState == ERROR_NOT_SUPPORTED )
		CaptureXInputPad(nPadID);

}

CControllerState CPad::ReconcileTwoControllersInput(const CControllerState& rDevice1, const CControllerState& rDevice2)
{
	CControllerState			PadOut;

	if ( (rDevice1.LEFTSTICKX > 0 && rDevice2.LEFTSTICKX < 0) || (rDevice1.LEFTSTICKX < 0 && rDevice2.LEFTSTICKX > 0) )
		PadOut.LEFTSTICKX = 0;
	else if ( rDevice1.LEFTSTICKX >= 0 && rDevice2.LEFTSTICKX >= 0 )
		PadOut.LEFTSTICKX = Max(rDevice1.LEFTSTICKX, rDevice2.LEFTSTICKX);
	else
		PadOut.LEFTSTICKX = Min(rDevice1.LEFTSTICKX, rDevice2.LEFTSTICKX);

	if ( (rDevice1.LEFTSTICKY > 0 && rDevice2.LEFTSTICKY < 0) || (rDevice1.LEFTSTICKY < 0 && rDevice2.LEFTSTICKY > 0) )
		PadOut.LEFTSTICKY = 0;
	else if ( rDevice1.LEFTSTICKY >= 0 && rDevice2.LEFTSTICKY >= 0 )
		PadOut.LEFTSTICKY = Max(rDevice1.LEFTSTICKY, rDevice2.LEFTSTICKY);
	else
		PadOut.LEFTSTICKY = Min(rDevice1.LEFTSTICKY, rDevice2.LEFTSTICKY);

	if ( (rDevice1.RIGHTSTICKX > 0 && rDevice2.RIGHTSTICKX < 0) || (rDevice1.RIGHTSTICKX < 0 && rDevice2.RIGHTSTICKX > 0) )
		PadOut.RIGHTSTICKX = 0;
	else if ( rDevice1.RIGHTSTICKX >= 0 && rDevice2.RIGHTSTICKX >= 0 )
		PadOut.RIGHTSTICKX = Max(rDevice1.RIGHTSTICKX, rDevice2.RIGHTSTICKX);
	else
		PadOut.RIGHTSTICKX = Min(rDevice1.RIGHTSTICKX, rDevice2.RIGHTSTICKX);

	if ( (rDevice1.RIGHTSTICKY > 0 && rDevice2.RIGHTSTICKY < 0) || (rDevice1.RIGHTSTICKY < 0 && rDevice2.RIGHTSTICKY > 0) )
		PadOut.RIGHTSTICKY = 0;
	else if ( rDevice1.RIGHTSTICKY >= 0 && rDevice2.RIGHTSTICKY >= 0 )
		PadOut.RIGHTSTICKY = Max(rDevice1.RIGHTSTICKY, rDevice2.RIGHTSTICKY);
	else
		PadOut.RIGHTSTICKY = Min(rDevice1.RIGHTSTICKY, rDevice2.RIGHTSTICKY);

	PadOut.LEFTSHOULDER2 = Max(rDevice1.LEFTSHOULDER2, rDevice2.LEFTSHOULDER2);
	PadOut.RIGHTSHOULDER2 = Max(rDevice1.RIGHTSHOULDER2, rDevice2.RIGHTSHOULDER2);

	PadOut.LEFTSHOULDER1 = Max(rDevice1.LEFTSHOULDER1, rDevice2.LEFTSHOULDER1);
	PadOut.RIGHTSHOULDER1 = Max(rDevice1.RIGHTSHOULDER1, rDevice2.RIGHTSHOULDER1);
	PadOut.DPADUP = Max(rDevice1.DPADUP, rDevice2.DPADUP);
	PadOut.DPADDOWN = Max(rDevice1.DPADDOWN, rDevice2.DPADDOWN);
	PadOut.DPADLEFT = Max(rDevice1.DPADLEFT, rDevice2.DPADLEFT);
	PadOut.DPADRIGHT = Max(rDevice1.DPADRIGHT, rDevice2.DPADRIGHT);
	PadOut.SQUARE = Max(rDevice1.SQUARE, rDevice2.SQUARE);
	PadOut.TRIANGLE = Max(rDevice1.TRIANGLE, rDevice2.TRIANGLE);
	PadOut.CROSS = Max(rDevice1.CROSS, rDevice2.CROSS);
	PadOut.CIRCLE = Max(rDevice1.CIRCLE, rDevice2.CIRCLE);

	// Theoretically, these are not pressure sensitive, but won't hurt making them so
	PadOut.START = Max(rDevice1.START, rDevice2.START);
	PadOut.SELECT = Max(rDevice1.SELECT, rDevice2.SELECT);
	PadOut.LEFTSHOCK = Max(rDevice1.LEFTSHOCK, rDevice2.LEFTSHOCK);
	PadOut.RIGHTSHOCK = Max(rDevice1.RIGHTSHOCK, rDevice2.RIGHTSHOCK);

	PadOut.HOME = Max(rDevice1.HOME, rDevice2.HOME);

	PadOut.m_bPedWalk = (rDevice1.m_bPedWalk != 0 || rDevice2.m_bPedWalk != 0) ? 255 : 0;
	PadOut.m_bVehicleMouseLook = (rDevice1.m_bVehicleMouseLook != 0 || rDevice2.m_bVehicleMouseLook != 0) ? 255 : 0;
	PadOut.m_bRadioTrackSkip = (rDevice1.m_bRadioTrackSkip != 0 || rDevice2.m_bRadioTrackSkip != 0) ? 255 : 0;

	return PadOut;
}

void CPad::UpdateMouse()
{
	if ( IsForeground() )
	{
		OldMouseControllerState = NewMouseControllerState;
		NewMouseControllerState = PCTempMouseControllerState;

		// As TempMouseControllerState contains only raw data now, handle movement inversion here
		if ( !FrontEndMenuManager.m_bMenuActive )
		{
			if ( MousePointerStateHelper.m_bVerticalInvert )
				NewMouseControllerState.X = -NewMouseControllerState.X;
			if ( MousePointerStateHelper.m_bHorizontalInvert )
				NewMouseControllerState.Y = -NewMouseControllerState.Y;
		}
		
		// Clear mouse movement data and scroll data in temp buffer
		PCTempMouseControllerState.X = PCTempMouseControllerState.Y = PCTempMouseControllerState.Z = 0.0f;
		PCTempMouseControllerState.wheelDown = PCTempMouseControllerState.wheelUp = false;
		
		if ( NewMouseControllerState.CheckForInput() )
			LastTimeTouched = CTimer::m_snTimeInMilliseconds;
	}
}

CPad* CPad::GetPad(int nPad)
{
	return &Pads[nPad];
}

short CPad::AimWeaponLeftRight(CPed* pPed)
{
	if ( DisablePlayerControls )
		return 0;

	short	nMainInput;
	short	nAuxInput;
	if ( !bSouthpaw || !pXboxPad[CURRENT_XINPUT_PAD]->HasPadInHands() )
	{
		nMainInput = NewState.RIGHTSTICKX;
		nAuxInput = NewState.LEFTSTICKX;
	}
	else
	{
		nMainInput = NewState.LEFTSTICKX;
		nAuxInput = NewState.RIGHTSTICKX;
	}

	bool	bUseMouse = !CCamera::m_bUseMouse3rdPerson;

	if ( bUseMouse && pPed )
	{
		void*	pVehicle =  pPed->pVehicle;
		if ( pPed->GetAttachedEntity() || ( pPed->pedFlags.bInVehicle && pVehicle && *(CPed**)((BYTE*)pVehicle + 0x460) != pPed ) )
		{
			if ( std::abs(nAuxInput) > std::abs(nMainInput) )
				nMainInput = nAuxInput;
		}
	}

	return nMainInput;
}

short CPad::AimWeaponUpDown(CPed* pPed)
{
	if ( DisablePlayerControls )
		return 0;

	short	nMainInput;
	short	nAuxInput;
	if ( !bSouthpaw || !pXboxPad[CURRENT_XINPUT_PAD]->HasPadInHands() )
	{
		nMainInput = NewState.RIGHTSTICKY;
		nAuxInput = NewState.LEFTSTICKY;
	}
	else
	{
		nMainInput = NewState.LEFTSTICKY;
		nAuxInput = NewState.RIGHTSTICKY;
	}

	bool	bUseMouse = !CCamera::m_bUseMouse3rdPerson;

	if ( bUseMouse && pPed )
	{
		void*	pVehicle =  pPed->pVehicle;
		if ( pPed->GetAttachedEntity() || ( pPed->pedFlags.bInVehicle && pVehicle && *(CPed**)((BYTE*)pVehicle + 0x460) != pPed ) )
		{
			if ( std::abs(nAuxInput) > std::abs(nMainInput) )
				nMainInput = nAuxInput;
		}
	}

	return bInvertLook4Pad ? -nMainInput : nMainInput;
}

short CPad::SniperModeLookUpDown(CPed* pPed)
{
	if ( DisablePlayerControls )
		return 0;

	short	nMainInput;
	short	nAuxInput;
	if ( !bSwapSticksDuringAiming[CURRENT_XINPUT_PAD] )
	{
		nMainInput = NewState.RIGHTSTICKY;
		nAuxInput = Mode == PAD_IV_CONTROLS_MODE ? 0 : NewState.LEFTSTICKY;
	}
	else
	{
		nMainInput = NewState.LEFTSTICKY;
		nAuxInput = Mode == PAD_IV_CONTROLS_MODE ? 0 : NewState.RIGHTSTICKY;
	}

	if ( !CCamera::m_bUseMouse3rdPerson )
	{
		if ( std::abs(nAuxInput) > std::abs(nMainInput) )
			nMainInput = nAuxInput;
	}

	return bInvertLook4Pad ? -nMainInput : nMainInput;
}

short CPad::SniperModeLookLeftRight(CPed* pPed)
{
	if ( DisablePlayerControls )
		return 0;

	short	nMainInput;
	short	nAuxInput;
	if ( !bSwapSticksDuringAiming[CURRENT_XINPUT_PAD] )
	{
		nMainInput = NewState.RIGHTSTICKX;
		nAuxInput = Mode == PAD_IV_CONTROLS_MODE ? 0 : NewState.LEFTSTICKX;
	}
	else
	{
		nMainInput = NewState.LEFTSTICKX;
		nAuxInput = Mode == PAD_IV_CONTROLS_MODE ? 0 : NewState.RIGHTSTICKX;
	}

	if ( !CCamera::m_bUseMouse3rdPerson )
	{
		if ( std::abs(nAuxInput) > std::abs(nMainInput) )
			nMainInput = nAuxInput;
	}

	return nMainInput;
}

bool CPad::GetJetpackStrafeLeft()
{
	if ( DisablePlayerControls )
		return false;

	short	nInput;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		nInput = NewState.LEFTSHOULDER2 - NewState.RIGHTSHOULDER2;
		break;
	case CONTROLS_MODERN:
		nInput = NewState.LEFTSHOULDER1 - NewState.RIGHTSHOULDER1;
		break;
	default:
		nInput = 0;
		break;
	}

	if ( nInput > 0 )
	{
		return true;
	}
	return false;
}

bool CPad::GetJetpackStrafeRight()
{
	if ( DisablePlayerControls )
		return false;

	short	nInput;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		nInput = NewState.RIGHTSHOULDER2 - NewState.LEFTSHOULDER2;
		break;
	case CONTROLS_MODERN:
		nInput = NewState.RIGHTSHOULDER1 - NewState.LEFTSHOULDER1;
		break;
	default:
		nInput = 0;
		break;
	}

	if ( nInput > 0 )
	{
		return true;
	}
	return false;
}

short CPad::GetJetpackAscend()
{
	if ( DisablePlayerControls )
		return 0;

	return NewState.CROSS;
}

short CPad::GetJetpackDescend()
{
	if ( DisablePlayerControls )
		return 0;

	return NewState.SQUARE;
}

bool CPad::JustLockedOn()
{
	CPed*	pPed = FindPlayerPed(CURRENT_XINPUT_PAD);

	if ( pPed->GetWeaponSlots()[pPed->GetActiveWeapon()].IsTypeMelee() )
		return TargetJustDown();
	
	return JustLockedOn_NoMeleeCheck();
}

bool CPad::JustLockedOn_NoMeleeCheck()
{
	if ( DisablePlayerControls )
		return false;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		return NewState.RIGHTSHOULDER1 != 0 && OldState.RIGHTSHOULDER1 == 0;
	case CONTROLS_MODERN:
		return NewState.LEFTSHOULDER2 >= LOCK_ON_THRESHOLD && OldState.LEFTSHOULDER2 < LOCK_ON_THRESHOLD;
	}
	return false;
}

bool CPad::GetLockOn()
{
	if ( DisablePlayerControls )
		return false;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		return NewState.RIGHTSHOULDER1 != 0;
	case CONTROLS_MODERN:
		return NewState.LEFTSHOULDER2 >= LOCK_ON_THRESHOLD;
	}
	return false;
}

bool CPad::HasBeenTargetting()
{
	if ( DisablePlayerControls )
		return false;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		return OldState.RIGHTSHOULDER1 != 0;
	case CONTROLS_MODERN:
		return OldState.LEFTSHOULDER2 != 0;
	}
	return false;
}

short CPad::GetCinemaCam()
{
	return bSouthpaw && pXboxPad[0]->HasPadInHands() ? GetPad(0)->NewState.LEFTSTICKX : GetPad(0)->NewState.RIGHTSTICKX;
}

bool CPad::UserTracksSkipJustDown()
{
	return (NewState.m_bRadioTrackSkip != 0 && OldState.m_bRadioTrackSkip == 0) || ChangeStation_HoldTimer == -1;
}


bool CPad::SniperZoomIn()
{
	if ( DisablePlayerControls )
		return false;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		fSniperZoomVal = Max(NewState.SQUARE, NewState.RIGHTSHOULDER2);
		return NewState.SQUARE != 0 || NewState.RIGHTSHOULDER2 != 0;
	case CONTROLS_MODERN:
		if ( bSwapSticksDuringAiming[CURRENT_XINPUT_PAD] )
		{
			fSniperZoomVal = NewState.RIGHTSTICKY * -2.0f;
			return NewState.RIGHTSTICKY < 0;
		}
		fSniperZoomVal = NewState.LEFTSTICKY * -2.0f;
		return NewState.LEFTSTICKY < 0;
	}

	return false;
}

bool CPad::SniperZoomOut()
{
	if ( DisablePlayerControls )
		return false;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		fSniperZoomVal = Max(NewState.CROSS, NewState.LEFTSHOULDER2);
		return NewState.CROSS != 0 || NewState.LEFTSHOULDER2 != 0;
	case CONTROLS_MODERN:
		if ( bSwapSticksDuringAiming[CURRENT_XINPUT_PAD] )
		{
			fSniperZoomVal = NewState.RIGHTSTICKY * 2.0f;
			return NewState.RIGHTSTICKY > 0;
		}
		fSniperZoomVal = NewState.LEFTSTICKY * 2.0f;
		return NewState.LEFTSTICKY > 0;
	}

	return false;
}

bool CPad::ForceCameraBehindPlayer()
{
	if ( DisablePlayerControls )
		return false;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		return NewState.LEFTSHOULDER1 != 0;
	case CONTROLS_MODERN:
		return NewState.RIGHTSHOULDER1 != 0;
	}

	return false;
}


bool CPad::GetHydraTarget()
{
	if ( DisablePlayerControls )
		return false;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		return NewState.RIGHTSHOULDER1 != 0;
	case CONTROLS_MODERN:
		return NewState.SQUARE != 0;
	}
	return false;
}

bool CPad::HydraTargetJustDown()
{
	if ( DisablePlayerControls )
		return false;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		return NewState.RIGHTSHOULDER1 != 0 && OldState.RIGHTSHOULDER1 == 0;
	case CONTROLS_MODERN:
		return NewState.SQUARE != 0 && OldState.SQUARE == 0;
	}
	return false;
}

bool CPad::BunnyHopJustDown()
{
	if ( DisablePlayerControls )
		return false;

	static WORD		wOldPadMode = 0;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		if ( wOldPadMode != 0 )
		{
			wOldPadMode = 0;
			Memory::Patch<BYTE>(0x6C0736, offsetof(CPad, NewState.LEFTSHOULDER1));
		}
		return NewState.LEFTSHOULDER1 != 0 && OldState.LEFTSHOULDER1 == 0;
	case CONTROLS_MODERN:
		if ( wOldPadMode != 1 )
		{
			wOldPadMode = 1;
			Memory::Patch<BYTE>(0x6C0736, offsetof(CPad, NewState.SQUARE));
		}
		return NewState.SQUARE != 0 && OldState.SQUARE == 0;
	}
	return false;
}

bool CPad::LandingGearSwitchJustDown()
{
	if ( DisablePlayerControls )
		return false;

	if ( bSouthpaw && pXboxPad[CURRENT_XINPUT_PAD]->HasPadInHands() )
		return NewState.LEFTSHOCK != 0 && OldState.LEFTSHOCK == 0;

	return NewState.RIGHTSHOCK != 0 && OldState.RIGHTSHOCK == 0;
}

bool CPad::DiveJustDown(CPed* pPed)
{
	UNREFERENCED_PARAMETER(pPed);

	if ( DisablePlayerControls )
		return false;

	return NewState.CIRCLE != 0 && OldState.CIRCLE == 0;
}

short CPad::GetBmxHandBrake()
{
	if ( DisablePlayerControls )
		return 0;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		return NewState.RIGHTSHOULDER1;
	case CONTROLS_MODERN:
		return NewState.RIGHTSHOULDER2;
	}
	return 0;
}

short CPad::GetBmxAccelerate()
{
	if ( DisablePlayerControls )
		return 0;

	return NewState.CROSS;
}

short CPad::CarGunJustDown()
{
	if ( DisablePlayerControls )
		return 0;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		if ( NewState.CIRCLE != 0 && OldState.CIRCLE == 0 )
			return 1;
		if ( !pXboxPad[CURRENT_XINPUT_PAD]->HasPadInHands() )
		{
			if ( !bDisablePlayerFireWeaponWithL1 && NewState.LEFTSHOULDER1 != 0 && OldState.LEFTSHOULDER1 == 0 )
				return 2;
		}
		return 0;
	case CONTROLS_MODERN:
		// This function is used only for cars, so never returns 2 in IV controls mode
		return NewState.CIRCLE != 0 && OldState.CIRCLE == 0 ? 1 : 0;
	}
	return 0;
}

short CPad::PlaneGunJustDown()
{
	if ( DisablePlayerControls )
		return 0;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		if ( NewState.CIRCLE != 0 && OldState.CIRCLE == 0 )
			return 1;
		if ( !pXboxPad[CURRENT_XINPUT_PAD]->HasPadInHands() )
		{
			if ( !bDisablePlayerFireWeaponWithL1 && NewState.LEFTSHOULDER1 != 0 && OldState.LEFTSHOULDER1 == 0 )
				return 2;
		}
		return 0;
	case CONTROLS_MODERN:
		if ( NewState.CIRCLE != 0 && OldState.CIRCLE == 0 )
			return 1;
		if ( NewState.CROSS != 0 && OldState.CROSS == 0 )
			return 2;
		return 0;
	}
	return 0;
}

short CPad::HydraGunJustDown()
{
	if ( DisablePlayerControls )
		return 0;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		{
			// Needs to invert controls?
			bool	bUsesPad = pXboxPad[CURRENT_XINPUT_PAD]->HasPadInHands();
			if ( NewState.CIRCLE != 0 && OldState.CIRCLE == 0 )
				return bUsesPad ? 1 : 2;
			if ( !bDisablePlayerFireWeaponWithL1 && NewState.LEFTSHOULDER1 != 0 && OldState.LEFTSHOULDER1 == 0 )
				return bUsesPad ? 2 : 1;
			return 0;
		}
	case CONTROLS_MODERN:
		{
			if ( NewState.CIRCLE != 0 && OldState.CIRCLE == 0 )
				return 1;
			if ( NewState.CROSS != 0 && OldState.CROSS == 0 )
				return 2;
			return 0;
		}
	}
	return 0;
}

short CPad::GetCarGunFired()
{
	if ( DisablePlayerControls )
		return 0;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		if ( NewState.CIRCLE != 0 )
			return 1;
		if ( !pXboxPad[CURRENT_XINPUT_PAD]->HasPadInHands() )
		{
			if ( !bDisablePlayerFireWeaponWithL1 && NewState.LEFTSHOULDER1 != 0 )
				return 2;
		}
		return 0;
	case CONTROLS_MODERN:
		return NewState.CIRCLE != 0 ? 1 : 0;
	}
	return 0;
}

short CPad::GetPlaneGunFired()
{
	if ( DisablePlayerControls )
		return 0;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		if ( NewState.CIRCLE != 0 )
			return 1;
		if ( !pXboxPad[CURRENT_XINPUT_PAD]->HasPadInHands() )
		{
			if ( !bDisablePlayerFireWeaponWithL1 && NewState.LEFTSHOULDER1 != 0 )
				return 2;
		}
		return 0;
	case CONTROLS_MODERN:
		if ( NewState.CIRCLE != 0 )
			return 1;
		if ( NewState.CROSS != 0 )
			return 2;
		return 0;
	}
	return 0;
}

bool CPad::ChangeStationDownJustUp()
{
	if ( DisablePlayerControls || ChangeStationJustStoppedHolding() )
		return false;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		if ( pXboxPad[CURRENT_XINPUT_PAD]->HasPadInHands() )
			return false;
		return NewState.DPADDOWN == 0 && OldState.DPADDOWN != 0;
	case CONTROLS_MODERN:
		return NewState.DPADLEFT == 0 && OldState.DPADLEFT != 0;
	}
	return false;
}

bool CPad::ChangeStationUpJustUp()
{
	if ( DisablePlayerControls || ChangeStationJustStoppedHolding() )
		return false;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		if ( pXboxPad[CURRENT_XINPUT_PAD]->HasPadInHands() )
			return NewState.LEFTSHOULDER1 == 0 && OldState.LEFTSHOULDER1 != 0;
		else
			return NewState.DPADUP == 0 && OldState.DPADUP != 0;
	case CONTROLS_MODERN:
		return NewState.DPADRIGHT == 0 && OldState.DPADRIGHT != 0;
	}
	return false;
}

bool CPad::WeaponJustDown(CPed* pPed)
{
	if ( DisablePlayerControls || bDisablePlayerFireWeapon )
		return false;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		{
			bool	bCanUseSecondary = false;
			if ( !pXboxPad[CURRENT_XINPUT_PAD]->HasPadInHands() && !bDisablePlayerFireWeaponWithL1 )
			{
				if ( pPed )
					bCanUseSecondary = pPed->GetPedIntelligencePtr()->GetTaskUseGun() || pPed->GetPedIntelligencePtr()->GetTaskGangDriveby() || pPed->GetAttachedEntity();
			}
			return bCanUseSecondary ? (NewState.CIRCLE != 0 && OldState.CIRCLE == 0) || (NewState.LEFTSHOULDER1 != 0 && OldState.LEFTSHOULDER1 == 0) : NewState.CIRCLE != 0 && OldState.CIRCLE == 0;
		}
	case CONTROLS_MODERN:
		{
			if ( !pPed )
				pPed = FindPlayerPed(CURRENT_XINPUT_PAD);
			
			bool		bShouldUseMeleeKey = CWeaponInfo::GetWeaponInfo(pPed->GetWeaponSlots()[pPed->GetActiveWeapon()].m_eWeaponType, 1)->weaponType == 5;	// USE
			CVehicle*	pVehicle = pPed->pVehicle;
			return bShouldUseMeleeKey || ( pPed->pedFlags.bInVehicle && pVehicle && *(CPed**)((BYTE*)pVehicle + 0x460) == pPed ) ? NewState.CIRCLE != 0 && OldState.CIRCLE == 0 : NewState.RIGHTSHOULDER2 != 0 && OldState.RIGHTSHOULDER2 == 0;
		}
	}
	return false;
}

int CPad::GetWeapon(CPed* pPed)
{
	if ( DisablePlayerControls || bDisablePlayerFireWeapon )
		return 0;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		{
			bool	bCanUseSecondary = false;
			if ( !pXboxPad[CURRENT_XINPUT_PAD]->HasPadInHands() && !bDisablePlayerFireWeaponWithL1 )
			{
				if ( pPed )
					bCanUseSecondary = pPed->GetPedIntelligencePtr()->GetTaskUseGun() || pPed->GetPedIntelligencePtr()->GetTaskGangDriveby() || pPed->GetAttachedEntity();
			}
			return bCanUseSecondary ? NewState.CIRCLE + NewState.LEFTSHOULDER1 : NewState.CIRCLE;
		}
	case CONTROLS_MODERN:
		{
			if ( !pPed )
				pPed = FindPlayerPed(CURRENT_XINPUT_PAD);

			bool		bShouldUseMeleeKey = CWeaponInfo::GetWeaponInfo(pPed->GetWeaponSlots()[pPed->GetActiveWeapon()].m_eWeaponType, 1)->weaponType == 5;	// USE
			CVehicle*	pVehicle = pPed->pVehicle;
			return bShouldUseMeleeKey || ( pPed->pedFlags.bInVehicle && pVehicle && *(CPed**)((BYTE*)pVehicle + 0x460) == pPed )  ? NewState.CIRCLE : NewState.RIGHTSHOULDER2;
		}
	}
	return 0;
}

bool CPad::ReloadJustDown()
{
	if ( DisablePlayerControls )
		return false;

	// Sixaxis override
	/*if ( bSixaxisReloading[CURRENT_XINPUT_PAD] )
	{
		if ( pXboxPad[CURRENT_XINPUT_PAD]->HasPadInHands() )
		{
			// Check for quick sweep up
			if ( NewSixaxisState[CURRENT_XINPUT_PAD].ACCEL_Y - OldSixaxisState[CURRENT_XINPUT_PAD].ACCEL_Y > 35.0f * fTimeStep )
				return true;
		}
	}*/

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		return NewState.LEFTSHOULDER2 != 0 && OldState.LEFTSHOULDER2 == 0 && NewState.RIGHTSHOULDER2 != 0 && OldState.RIGHTSHOULDER2 == 0;
	case CONTROLS_MODERN:
		return NewState.CIRCLE != 0 && OldState.CIRCLE == 0;
	}
	return false;
}

int CPad::PerformReload(CPed* pPed)
{
	if ( pPed )
	{
		if ( ReloadJustDown() )
		{
			BYTE		bCurrentSlot = pPed->GetActiveWeapon();
			if ( bCurrentSlot < WEAPONSLOT_TYPE_HEAVY )
			{
				CWeapon*		pCurrentWeapon = &pPed->GetWeaponSlots()[bCurrentSlot];
				CWeaponInfo*	pCurrentWeaponInfo = CWeaponInfo::GetWeaponInfo(pCurrentWeapon->m_eWeaponType, pPed->GetWeaponSkill());
				if ( pCurrentWeapon->m_nAmmoTotal > pCurrentWeapon->m_nAmmoInClip && pCurrentWeaponInfo->ammoClip > pCurrentWeapon->m_nAmmoInClip )
					pCurrentWeapon->m_eState = WEAPONSTATE_RELOADING;
			}
		}
	}
	return GetWeapon(pPed);
}

bool CPad::GetCycleCameraMode()
{
	return NewState.SELECT != 0;
}

bool CPad::CycleCameraModeJustUp()
{
	return CycleCameraModeUpJustUp() || CycleCameraModeDownJustUp();
}

bool CPad::CycleCameraModeUpJustUp()
{
	if ( CycleCameraModeJustStoppedHolding() || FailCameraChangeThisFrame )
		return false;

	return NewState.SELECT == 0 && OldState.SELECT != 0;
}

bool CPad::CycleCameraModeDownJustUp()
{
	return false;
}

bool CPad::GetTarget()
{
	if ( DisablePlayerControls )
		return false;
	
	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		return NewState.RIGHTSHOULDER1 != 0;
	case CONTROLS_MODERN:
		return NewState.LEFTSHOULDER2 != 0;
	}

	return false;
}

bool CPad::ShiftTargetRightJustDown()
{
	if ( DisablePlayerControls )
		return false;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		return NewState.RIGHTSHOULDER2 != 0 && OldState.RIGHTSHOULDER2 == 0 && NewState.LEFTSHOULDER2 == 0 && OldState.LEFTSHOULDER2 == 0;
	case CONTROLS_MODERN:
		return NewState.RIGHTSHOULDER1 != 0 && OldState.RIGHTSHOULDER1 == 0;
	}

	return false;
}

bool CPad::ShiftTargetLeftJustDown()
{
	if ( DisablePlayerControls )
		return false;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		return NewState.LEFTSHOULDER2 != 0 && OldState.LEFTSHOULDER2 == 0 && NewState.RIGHTSHOULDER2 == 0 && OldState.RIGHTSHOULDER2 == 0;
	case CONTROLS_MODERN:
		return NewState.LEFTSHOULDER1 != 0 && OldState.LEFTSHOULDER1 == 0;
	}

	return false;
}

bool CPad::TargetJustDown()
{
	if ( DisablePlayerControls )
		return false;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		return NewState.RIGHTSHOULDER1 != 0 && OldState.RIGHTSHOULDER1 == 0;
	case CONTROLS_MODERN:
		return NewState.LEFTSHOULDER2 != 0 && OldState.LEFTSHOULDER2 == 0;
	}

	return false;
}

bool CPad::CycleWeaponRightJustDown()
{
	if ( DisablePlayerControls )
		return false;

	if ( bDisablePlayerCycleWeapon )
		return false;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		return NewState.RIGHTSHOULDER2 != 0 && OldState.RIGHTSHOULDER2 == 0 && NewState.LEFTSHOULDER2 == 0 && OldState.LEFTSHOULDER2 == 0;
	case CONTROLS_MODERN:
		return NewState.DPADRIGHT != 0 && OldState.DPADRIGHT == 0;
	}

	return false;
}

bool CPad::CycleWeaponLeftJustDown()
{
	if ( DisablePlayerControls )
		return false;

	if ( bDisablePlayerCycleWeapon )
		return false;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		return NewState.LEFTSHOULDER2 != 0 && OldState.LEFTSHOULDER2 == 0 && NewState.RIGHTSHOULDER2 == 0 && OldState.RIGHTSHOULDER2 == 0;
	case CONTROLS_MODERN:
		return NewState.DPADLEFT != 0 && OldState.DPADLEFT == 0;
	}

	return false;
}

bool CPad::GetHorn()
{
	if ( DisablePlayerControls )
		return false;

	if ( bSouthpaw && pXboxPad[CURRENT_XINPUT_PAD]->HasPadInHands() )
		return NewState.RIGHTSHOCK != 0;
	return NewState.LEFTSHOCK != 0;
}

bool CPad::HornJustDown()
{
	if ( DisablePlayerControls )
		return false;

	if ( bSouthpaw && pXboxPad[CURRENT_XINPUT_PAD]->HasPadInHands() )
		return NewState.RIGHTSHOCK != 0 && OldState.RIGHTSHOCK == 0;
	return NewState.LEFTSHOCK != 0 && OldState.LEFTSHOCK == 0;
}

bool CPad::GetLookLeft()
{
	if ( DisablePlayerControls )
		return false;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		return NewState.LEFTSHOULDER2 != 0 && NewState.RIGHTSHOULDER2 == 0;
	case CONTROLS_MODERN:
		return NewState.LEFTSHOULDER1 != 0 && NewState.RIGHTSHOULDER1 == 0;
	}

	return false;
}

bool CPad::GetLookRight()
{
	if ( DisablePlayerControls )
		return false;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		return NewState.RIGHTSHOULDER2 != 0 && NewState.LEFTSHOULDER2 == 0;
	case CONTROLS_MODERN:
		return NewState.RIGHTSHOULDER1 != 0 && NewState.LEFTSHOULDER1 == 0;
	}

	return false;
}

bool CPad::GetLookBehindForPed()
{
	if ( DisablePlayerControls )
		return false;

	if ( bSouthpaw && pXboxPad[CURRENT_XINPUT_PAD]->HasPadInHands() )
		return NewState.LEFTSHOCK != 0;
	return NewState.RIGHTSHOCK != 0;
}

bool CPad::GetLookBehindForCar()
{
	if ( DisablePlayerControls )
		return false;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		return NewState.RIGHTSHOULDER2 != 0 && NewState.LEFTSHOULDER2 != 0;
	case CONTROLS_MODERN:
		return NewState.RIGHTSHOULDER1 != 0 && NewState.LEFTSHOULDER1 != 0;
	}

	return false;
}

short CPad::GetAccelerate()
{
	if ( DisablePlayerControls )
		return 0;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		return NewState.CROSS;
	case CONTROLS_MODERN:
		return NewState.RIGHTSHOULDER2;
	}

	return 0;
}

short CPad::GetBrake()
{
	if ( DisablePlayerControls )
		return 0;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		return NewState.SQUARE;
	case CONTROLS_MODERN:
		return NewState.LEFTSHOULDER2;
	}

	return 0;
}

short CPad::GetHandBrake()
{
	if ( DisablePlayerControls )
		return 0;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		return NewState.RIGHTSHOULDER1;
	case CONTROLS_MODERN:
		return NewState.CROSS;
	}

	return 0;
}

short CPad::GetPedWalkLeftRight()
{
	if ( DisablePlayerControls )
		return 0;

	if ( !pXboxPad[CURRENT_XINPUT_PAD]->HasPadInHands() )
		return NewState.LEFTSTICKX;

	short	nAxisInput = bSouthpaw ? NewState.RIGHTSTICKX : NewState.LEFTSTICKX;
	short	nDPadInput = Mode != PAD_IV_CONTROLS_MODE ? (NewState.DPADRIGHT - NewState.DPADLEFT) / 2 : 0;

	return std::abs(nAxisInput) > std::abs(nDPadInput) ? nAxisInput : nDPadInput;
}

short CPad::GetPedWalkUpDown()
{
	if ( DisablePlayerControls )
		return 0;

	if ( !pXboxPad[CURRENT_XINPUT_PAD]->HasPadInHands() )
		return NewState.LEFTSTICKY;

	short	nAxisInput = bSouthpaw? NewState.RIGHTSTICKY : NewState.LEFTSTICKY;
	short	nDPadInput = Mode != PAD_IV_CONTROLS_MODE ? (NewState.DPADDOWN - NewState.DPADUP) / 2 : 0;

	return std::abs(nAxisInput) > std::abs(nDPadInput) ? nAxisInput : nDPadInput;
}

short CPad::GetSteeringLeftRight()
{
	if ( DisablePlayerControls )
		return 0;

	if ( pXboxPad[CURRENT_XINPUT_PAD]->HasPadInHands() )
	{
		short	nAxisInput = bSouthpaw ? NewState.RIGHTSTICKX : NewState.LEFTSTICKX;
		short	nDPadInput = Mode != PAD_IV_CONTROLS_MODE ? (NewState.DPADRIGHT - NewState.DPADLEFT) / 2 : 0;

		SteeringLeftRightBuffer[0] = std::abs(nAxisInput) > std::abs(nDPadInput) ? nAxisInput : nDPadInput;
	}
	else
		SteeringLeftRightBuffer[0] = NewState.LEFTSTICKX;

	return SteeringLeftRightBuffer[DrunkDrivingBufferUsed];
}

short CPad::GetSteeringUpDown()
{
	if ( DisablePlayerControls )
		return 0;

	if ( pXboxPad[CURRENT_XINPUT_PAD]->HasPadInHands() )
	{
		short	nAxisInput = bSouthpaw ? NewState.RIGHTSTICKY : NewState.LEFTSTICKY;
		short	nDPadInput = Mode != PAD_IV_CONTROLS_MODE ? (NewState.DPADDOWN - NewState.DPADUP) / 2 : 0;

		return std::abs(nAxisInput) > std::abs(nDPadInput) ? nAxisInput : nDPadInput;
	}
	
	return NewState.LEFTSTICKY;
}

short CPad::GetCarGunLeftRight()
{
	if ( DisablePlayerControls )
		return 0;

	return bSouthpaw && pXboxPad[CURRENT_XINPUT_PAD]->HasPadInHands() ? NewState.LEFTSTICKX : NewState.RIGHTSTICKX;
}

short CPad::GetCarGunUpDown()
{
	if ( DisablePlayerControls )
		return 0;

	return bSouthpaw && pXboxPad[CURRENT_XINPUT_PAD]->HasPadInHands() ? NewState.LEFTSTICKY : NewState.RIGHTSTICKY;
}

short CPad::GetHydraulicsLeftRight()
{
	if ( DisablePlayerControls )
		return 0;

	short	nStickInput, nSixaxisInput;

	nSixaxisInput = 0;//bSixaxisHydraulics[CURRENT_XINPUT_PAD] ? -NewSixaxisState[CURRENT_XINPUT_PAD].ACCEL_X : 0;
	nStickInput = bSouthpaw && pXboxPad[CURRENT_XINPUT_PAD]->HasPadInHands() ? NewState.LEFTSTICKX : NewState.RIGHTSTICKX;

	if ( nSixaxisInput > 127 )
		nSixaxisInput = 127;
	else if ( nSixaxisInput < -127 )
		nSixaxisInput = -127;

	if ( nStickInput >= 0 && nSixaxisInput >= 0 )
		return Max(nStickInput, nSixaxisInput);
	else if ( nStickInput <= 0 && nSixaxisInput <= 0 )
		return Min(nStickInput, nSixaxisInput);

	return nStickInput;
}

short CPad::GetHydraulicsUpDown()
{
	if ( DisablePlayerControls )
		return 0;

	short	nStickInput, nSixaxisInput;

	nSixaxisInput = 0;//bSixaxisHydraulics[CURRENT_XINPUT_PAD] ? -NewSixaxisState[CURRENT_XINPUT_PAD].ACCEL_Y : 0;
	nStickInput = bSouthpaw && pXboxPad[CURRENT_XINPUT_PAD]->HasPadInHands() ? NewState.LEFTSTICKY : NewState.RIGHTSTICKY;

	if ( nSixaxisInput > 127 )
		nSixaxisInput = 127;
	else if ( nSixaxisInput < -127 )
		nSixaxisInput = -127;

	if ( nStickInput >= 0 && nSixaxisInput >= 0 )
		return Max(nStickInput, nSixaxisInput);
	else if ( nStickInput <= 0 && nSixaxisInput <= 0 )
		return Min(nStickInput, nSixaxisInput);

	return nStickInput;
}

bool CPad::DuckJustDown()
{
	if ( DisablePlayerControls )
		return false;

	if ( bDisablePlayerDuck )
		return false;

	if ( bSouthpaw && pXboxPad[CURRENT_XINPUT_PAD]->HasPadInHands() )
		return NewState.RIGHTSHOCK != 0 && OldState.RIGHTSHOCK == 0;
	return NewState.LEFTSHOCK != 0 && OldState.LEFTSHOCK == 0;
}

bool CPad::GetDuck()
{
	if ( DisablePlayerControls )
		return false;

	if ( bDisablePlayerDuck )
		return false;

	if ( bSouthpaw && pXboxPad[CURRENT_XINPUT_PAD]->HasPadInHands() )
		return NewState.RIGHTSHOCK != 0;
	return NewState.LEFTSHOCK != 0;
}

bool CPad::ConversationNoJustDown()
{
	return false;
}

bool CPad::ConversationYesJustDown()
{
	return false;
}

bool CPad::GetGroupControlForward()
{
	return false;
}

bool CPad::GetGroupControlBack()
{
	return false;
}

short CPad::LookAroundLeftRight()
{
	bool	bUsesMouse = false;
	float	fAxisInput = bSouthpaw && pXboxPad[CURRENT_XINPUT_PAD]->HasPadInHands() ? NewState.LEFTSTICKX : NewState.RIGHTSTICKX;

	if ( !bUsesMouse && std::abs(fAxisInput) > 65.0f && (DisablePlayerControls || (bSouthpaw && pXboxPad[CURRENT_XINPUT_PAD]->HasPadInHands() ? !NewState.LEFTSHOCK : !NewState.RIGHTSHOCK)) )
		return static_cast<short>((fAxisInput > 0.0f ? fAxisInput - 65.0f : fAxisInput + 65.0f) * ((255.0f-65.0f)/(127.0f-65.0f)));
	else if ( bUsesMouse )
		return static_cast<short>(fAxisInput);

	return 0;
}

short CPad::LookAroundUpDown()
{
	bool	bUsesMouse = false;
	float	fAxisInput = bSouthpaw && pXboxPad[CURRENT_XINPUT_PAD]->HasPadInHands() ? NewState.LEFTSTICKY : NewState.RIGHTSTICKY;

	if ( bInvertLook4Pad )
		fAxisInput = -fAxisInput;

	if ( !bUsesMouse && std::abs(fAxisInput) > 65.0f && (DisablePlayerControls || (bSouthpaw && pXboxPad[CURRENT_XINPUT_PAD]->HasPadInHands() ? !NewState.LEFTSHOCK : !NewState.RIGHTSHOCK)) )
		return static_cast<short>((fAxisInput > 0.0f ? fAxisInput - 65.0f : fAxisInput + 65.0f) * ((255.0f-65.0f)/(127.0f-65.0f)));
	else if ( bUsesMouse )
		return static_cast<short>(fAxisInput);

	return 0;
}

static int GetVehicleAppearance(void* pVehicle)
{
	return *(int*)((char*)pVehicle + 0x594);
}

void CPad::Update(short nPadID)
{
	// Call GTA CPad::Update
	((void (__thiscall*)(CPad*,short))0x541C40)(this, nPadID);

	// GInput stuff
	UpdateHoldTimers();
	FailCameraChangeThisFrame = false;

	// IV-style free aim
	CPed*			pPlayerPed = FindPlayerPed(CURRENT_XINPUT_PAD);

	if ( FindPlayerPed(1) )
		bFreeAim = false;
	else if ( !CCamera::m_bUseMouse3rdPerson )
	{
		/*if ( bFreeAim_User )
		{
			bFreeAim = true;
			if ( pPlayerPed )
			{
				CWeapon*		pWeapon = &pPlayerPed->GetWeaponSlots()[pPlayerPed->GetActiveWeapon()];
				if ( pWeapon->IsTypeMelee() )
					bFreeAim = false;
			}
		}*/
		if ( Mode == PAD_IV_CONTROLS_MODE )
		{
			if ( pPlayerPed )
			{
				CWeapon*		pWeapon = &pPlayerPed->GetWeaponSlots()[pPlayerPed->GetActiveWeapon()];
				if ( pWeapon->IsTypeMelee() )
					bFreeAim = false;
				else if ( !GetLockOn() )
					bFreeAim = true;
				else if ( JustLockedOn_NoMeleeCheck() )
				{
					bFreeAim = false;

					// Pick a target manually if the player was in free aim previously
					if ( HasBeenTargetting() )
					{
						if ( pPlayerPed->GetPedIntelligencePtr()->GetTaskUseGun() && CWeaponInfo::GetWeaponInfo(pWeapon->m_eWeaponType, 1)->hexFlags & 1 )	// CANAIM
								pPlayerPed->FindWeaponLockOnTarget();
					}
				}
			}
		}
		else
			bFreeAim = false;
	}
	else
		bFreeAim = true;

	/*// Sixaxis
	if ( void* pVehicle = FindPlayerVehicle(nPadID, false) )
	{
		DrivenVehicleType[nPadID] = GetVehicleAppearance(pVehicle);
		bVehicleHasHydraulics[nPadID] = bSixaxisHydraulics[nPadID] && DrivenVehicleType[nPadID] == VEHICLE_AUTOMOBILE ? (*(DWORD*)((char*)pVehicle + 0x38C) & 0x20000) != 0 : false;
	}*/
}

void CPad::StopShaking(short nPadID)
{
	ShakeFreq = 0;
	ShakeDur = 0;

	pXboxPad[nPadID]->Vibrate(0, 0);
}

void CPad::StopPadsShaking()
{
	for ( short i = 0; i < 1; i++ )
		GetPad(i)->StopShaking(i);
}

/*bool CPad::GetLookLeftForHeli()
{
	if ( DisablePlayerControls )
		return false;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
	case CONTROLS_MODERN:
	case 2:
	case 3:
		fHelicopterRotationVal = NewState.LEFTSHOULDER2 * (-1.0f/255.0f);
		return NewState.LEFTSHOULDER2 != 0 && NewState.RIGHTSHOULDER2 == 0;
	case 4:
		fHelicopterRotationVal = -1.0f;
		return NewState.LEFTSHOULDER1 != 0 && NewState.RIGHTSHOULDER1 == 0;
	}
	return false;
}

bool CPad::GetLookRightForHeli()
{
	if ( DisablePlayerControls )
		return false;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
	case CONTROLS_MODERN:
	case 2:
	case 3:
		fHelicopterRotationVal = NewState.RIGHTSHOULDER2 * (-1.0f/255.0f);
		return NewState.RIGHTSHOULDER2 != 0 && NewState.LEFTSHOULDER2 == 0;
	case 4:
		fHelicopterRotationVal = -1.0f;
		return NewState.RIGHTSHOULDER1 != 0 && NewState.LEFTSHOULDER1 == 0;
	}
	return false;
}*/

bool CPad::FrontEndBackJustDown()
{
	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		return NewState.TRIANGLE != 0 && OldState.TRIANGLE == 0;
	case PAD_IV_CONTROLS_MODE:
		return NewState.CIRCLE != 0 && OldState.CIRCLE == 0;
	}
	return false;
}

bool CPad::GetHydraulicsJump()
{
	if ( DisablePlayerControls )
		return false;

	/*if ( bSixaxisHydraulics[CURRENT_XINPUT_PAD] )
	{
		if ( NewSixaxisState[CURRENT_XINPUT_PAD].ACCEL_Z - OldSixaxisState[CURRENT_XINPUT_PAD].ACCEL_Z > 50.0f * fTimeStep )
			return true;
	}*/

	if ( bSouthpaw && pXboxPad[CURRENT_XINPUT_PAD]->HasPadInHands() )
		return NewState.LEFTSHOCK != 0;
	return NewState.RIGHTSHOCK != 0;
}

bool CPad::GetChangeStationDown()
{
	if ( DisablePlayerControls )
		return false;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		return NewState.DPADDOWN != 0;
	case CONTROLS_MODERN:
		return NewState.DPADLEFT != 0;
	}
	return false;
}

bool CPad::GetChangeStationUp()
{
	if ( DisablePlayerControls )
		return false;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		return NewState.DPADUP != 0;
	case CONTROLS_MODERN:
		return NewState.DPADRIGHT != 0;
	}
	return false;
}

bool CPad::ChangeStationJustStoppedHolding()
{
	return ChangeStation_HoldTimer > CHANGE_STATION_HOLD_TIME;
}

bool CPad::CycleCameraModeJustHeld()
{
	return ChangeCamera_HoldTimer == -1;
}

bool CPad::CycleCameraModeJustStoppedHolding()
{
	return ChangeCamera_HoldTimer > CHANGE_CAMERA_HOLD_TIME;
}

bool CPad::GetFreeAimTarget()
{
	return Mode == 1 ? NewState.LEFTSHOULDER2 <= 150 : true;
}

bool CPad::FreeAimTargetJustDisabled()
{
	return Mode == 1 ? NewState.LEFTSHOULDER2 > 150 && OldState.LEFTSHOULDER1 <= 150 : false;
}

short CPad::GetBikeLeanUpDown()
{
	if ( DisablePlayerControls || pXboxPad[CURRENT_XINPUT_PAD]->HasPadInHands() )
		return 0;

	return NewState.RIGHTSTICKY;
}

short CPad::GetHeliForwardsBackwards()
{
	if ( DisablePlayerControls || pXboxPad[CURRENT_XINPUT_PAD]->HasPadInHands() )
		return 0;

	return NewState.RIGHTSTICKY;
}

float CPad::GetHeliLookAroundFloat()
{
	if ( DisablePlayerControls )
		return 0.0f;

	switch ( Mode )
	{
	case CONTROLS_STANDARD_1:
	case CONTROLS_STANDARD_2:
		return (NewState.RIGHTSHOULDER2 - NewState.LEFTSHOULDER2) * (1.0f/255.0f);
	case CONTROLS_MODERN:
		return (NewState.RIGHTSHOULDER1 - NewState.LEFTSHOULDER1) * (1.0f/255.0f);
	}
	return 0.0f;
}

void CPad::UpdateHoldTimers()
{
	// Change station
	UpdateHoldTimer(ChangeStation_HoldTimer, CHANGE_STATION_HOLD_TIME, GetChangeStationUp() || GetChangeStationDown());

	// Change camera
	UpdateHoldTimer(ChangeCamera_HoldTimer, CHANGE_CAMERA_HOLD_TIME, GetCycleCameraMode());
}

void CPad::UpdateHoldTimer(short& Timer, short nUpTimer, bool bDown)
{
	if ( bDown )
	{
		if ( Timer > 0 )
		{
			Timer -= static_cast<short>(CTimer::ms_fTimeStep * (0.02f * 1000.0f));
			if ( Timer <= 0 )
			{
				// Just held
				Timer = -1;
			}
		}
		else if ( Timer == -1 )
		{
			// Held
			Timer = -2;
		}
	}
	else if ( Timer < 0 )
	{
		// Just up
		Timer = nUpTimer + 1;
	}
	else
	{
		// Not pressed
		Timer = nUpTimer;
	}
}

/*bool CPad::ShouldSixaxisLeftRight()
{
	switch ( DrivenVehicleType[CURRENT_XINPUT_PAD] )
	{
	case VEHICLE_AUTOMOBILE:
#ifdef GINPUT_COMPILE_SA_VERSION
	case VEHICLE_MTRUCK:
#endif
		// No hydraulics only
		return !bVehicleHasHydraulics[CURRENT_XINPUT_PAD] && (bSixaxisCar[CURRENT_XINPUT_PAD] || SixaxisOverride[CURRENT_XINPUT_PAD]);
#ifndef GINPUT_COMPILE_III_VERSION
#ifdef GINPUT_COMPILE_SA_VERSION
	case VEHICLE_QUAD:
	case VEHICLE_BMX:	// Might need to be separate?
#endif
	case VEHICLE_BIKE:
		return bSixaxisBike[CURRENT_XINPUT_PAD] || SixaxisOverride[CURRENT_XINPUT_PAD];
#endif
	case VEHICLE_HELI:
#ifdef GINPUT_COMPILE_SA_VERSION
	case VEHICLE_FHELI:
#endif
		return bSixaxisHeli[CURRENT_XINPUT_PAD] || SixaxisOverride[CURRENT_XINPUT_PAD];
	case VEHICLE_PLANE:
#ifdef GINPUT_COMPILE_SA_VERSION
	case VEHICLE_FPLANE:
#endif
		return bSixaxisPlane[CURRENT_XINPUT_PAD] || SixaxisOverride[CURRENT_XINPUT_PAD];
	case VEHICLE_BOAT:
		return bSixaxisBoat[CURRENT_XINPUT_PAD] || SixaxisOverride[CURRENT_XINPUT_PAD];
	}

	return false;
}

bool CPad::ShouldSixaxisUpDown()
{
	switch ( DrivenVehicleType[CURRENT_XINPUT_PAD] )
	{
#ifndef GINPUT_COMPILE_III_VERSION
	case VEHICLE_BIKE:
#ifdef GINPUT_COMPILE_SA_VERSION
	case VEHICLE_QUAD:
	case VEHICLE_BMX:	// Might need to be separate?
#endif
		return bSixaxisBike[CURRENT_XINPUT_PAD] || SixaxisOverride[CURRENT_XINPUT_PAD];
#endif
	case VEHICLE_HELI:
#ifdef GINPUT_COMPILE_SA_VERSION
	case VEHICLE_FHELI:
#endif
		return bSixaxisHeli[CURRENT_XINPUT_PAD] || SixaxisOverride[CURRENT_XINPUT_PAD];
	case VEHICLE_PLANE:
#ifdef GINPUT_COMPILE_SA_VERSION
	case VEHICLE_FPLANE:
#endif
		return bSixaxisPlane[CURRENT_XINPUT_PAD] || SixaxisOverride[CURRENT_XINPUT_PAD];
	case VEHICLE_BOAT:
		return bSixaxisBoat[CURRENT_XINPUT_PAD];
	}

	return false;
}*/

static WORD* pIsEscKeyPressed = (WORD*)0xB733A8;
static WORD* pWasEscKeyPressed = (WORD*)0xB73138;
void __declspec(naked) MenuToggle()
{
	_asm
	{
		// Escape
		mov		ecx, [pIsEscKeyPressed]
		cmp		word ptr [ecx], 0
		jne		MenuToggle_Pressed

		// Start
		cmp		[eax]CPad.NewState.START, 0
		je		MenuToggle_NotPressed

MenuToggle_Pressed:
		mov		ecx, [pWasEscKeyPressed]
		cmp		word ptr [ecx], 0
		jne		MenuToggle_NotPressed

		cmp		[eax]CPad.OldState.START, 0
		jne		MenuToggle_NotPressed

MenuToggle_DeffoPressed:
		xor		al, al
		push	576BAFh
		retn

MenuToggle_NotPressed:
		// Triangle/Circle release check (exiting only)
		cmp		[esi].m_bMenuActive, 0
		je		MenuToggle_DeffoNotPressed

		cmp		CPad::SavedMode, PAD_IV_CONTROLS_MODE
		je		MenuToggle_CircleCheck
		cmp		[eax]CPad.NewState.TRIANGLE, 0
		jne		MenuToggle_DeffoNotPressed
		cmp		[eax]CPad.OldState.TRIANGLE, 0	
		je		MenuToggle_DeffoNotPressed
		jmp		MenuToggle_DeffoPressed

MenuToggle_CircleCheck:
		cmp		[eax]CPad.NewState.CIRCLE, 0
		jne		MenuToggle_DeffoNotPressed
		cmp		[eax]CPad.OldState.CIRCLE, 0	
		jne		MenuToggle_DeffoPressed

MenuToggle_DeffoNotPressed:
		test	ecx, ecx
		push	576BAFh
		retn
	}
}

void __declspec(naked) BmxAccelerateHook()
{
	_asm
	{
		cmp     [esi].m_dwVehicleSubClass, VEHICLE_BMX
		je		BmxAccelerateHook_Bmx
		jmp		CPad::GetAccelerate

BmxAccelerateHook_Bmx:
		jmp		CPad::GetBmxAccelerate
	}
}

void __declspec(naked) BmxHandBrakeHook()
{
	_asm
	{
		cmp     [esi].m_dwVehicleSubClass, VEHICLE_BMX
		je		BmxAccelerateHook_Bmx
		jmp		CPad::GetHandBrake

BmxAccelerateHook_Bmx:
		jmp		CPad::GetBmxHandBrake
	}
}

void __declspec(naked) HandleKeyDownHack()
{
	_asm
	{
		push	0
		mov		ecx, [pXboxPad]
		call	CX360Pad::SetHasPadInHands
		mov		eax, 743DF0h
		jmp		eax
	}
}

void CPad::Inject()
{
	using namespace Memory;

	InjectHook(0x541DDE, CaptureXInputPad_SCP);
	InjectHook(0x541DD7, &UpdateMouse);

	InjectHook(0x53F530, &ReconcileTwoControllersInput, PATCH_JUMP);

	InjectHook(0x748813, InitXInputPad);

	// GInput
	// TODO: Might need sorting
	InjectHook(0x541D70, CPad::StopPadsShaking, PATCH_JUMP);
	InjectHook(0x53FB50, &CPad::StopShaking, PATCH_JUMP);

	InjectHook(0x541E0B, &CPad::Update);

	// IV controls
	InjectHook(0x541040, &CPad::AimWeaponLeftRight, PATCH_JUMP);
	InjectHook(0x5410C0, &CPad::AimWeaponUpDown, PATCH_JUMP);

	InjectHook(0x53FFE0, &CPad::CarGunJustDown, PATCH_JUMP);
	InjectHook(0x6E34C8, &CPad::PlaneGunJustDown);
	InjectHook(0x6E34FF, &CPad::HydraGunJustDown);
	InjectHook(0x6E35F1, &CPad::HydraGunJustDown);

	// Inverting Hydra controls // That was a bad idea
	//Patch<WORD>(0x6E3506, 2);
	//Patch<WORD>(0x6E35F8, 1);

	InjectHook(0x4EB753, &CPad::ChangeStationDownJustUp);
	InjectHook(0x4EB72A, &CPad::ChangeStationUpJustUp);

	// CPad::CollectPickupJustDown - compatible

	InjectHook(0x541200, &CPad::ConversationNoJustDown, PATCH_JUMP);
	InjectHook(0x5411D0, &CPad::ConversationYesJustDown, PATCH_JUMP);

	InjectHook(0x5404F0, &CPad::CycleCameraModeDownJustUp, PATCH_JUMP);
	InjectHook(0x5404A0, &CPad::CycleCameraModeUpJustUp, PATCH_JUMP);
	InjectHook(0x52B6BC, &CPad::CycleCameraModeJustUp);

	InjectHook(0x60DA7E, &CPad::CycleWeaponLeftJustDown);
	InjectHook(0x60D8BF, &CPad::CycleWeaponRightJustDown);

	InjectHook(0x540720, &CPad::DuckJustDown, PATCH_JUMP);

	InjectHook(0x540AE0, &CPad::ForceCameraBehindPlayer, PATCH_JUMP);

	InjectHook(0x5403F0, &CPad::GetAccelerate, PATCH_JUMP);

	InjectHook(0x540080, &CPad::GetBrake, PATCH_JUMP);

	InjectHook(0x540950, &CPad::GetAnaloguePadUp, PATCH_JUMP);
	InjectHook(0x540980, &CPad::GetAnaloguePadDown, PATCH_JUMP);
	InjectHook(0x5409B0, &CPad::GetAnaloguePadLeft, PATCH_JUMP);
	InjectHook(0x5409E0, &CPad::GetAnaloguePadRight, PATCH_JUMP);

	InjectHook(0x53FF90, &CPad::GetCarGunFired, PATCH_JUMP);
	InjectHook(0x6E347E, &CPad::GetPlaneGunFired);
	InjectHook(0x6E34A8, &CPad::GetPlaneGunFired);
	InjectHook(0x6E3782, &CPad::GetPlaneGunFired);
	InjectHook(0x6E37B0, &CPad::GetPlaneGunFired);

	InjectHook(0x53FC50, &CPad::GetCarGunLeftRight, PATCH_JUMP);
	InjectHook(0x53FC10, &CPad::GetCarGunUpDown, PATCH_JUMP);

	InjectHook(0x687928, &CPad::GetDuck);

	// CPad::GetExitVehicle - compatible
	// CPad::ExitVehicleJustDown - compatible

	InjectHook(0x60D38C, &CPad::GetGroupControlForward);
	InjectHook(0x60D45B, &CPad::GetGroupControlBack);

	InjectHook(0x540040, &CPad::GetHandBrake, PATCH_JUMP);

	InjectHook(0x53FEE0, &CPad::GetHorn, PATCH_JUMP);
	InjectHook(0x53FF30, &CPad::HornJustDown, PATCH_JUMP);

	Patch<WORD>(0x541CE2, 0xCB8B);
	InjectHook(0x541CE4, &CPad::GetHorn, PATCH_CALL);
	Patch<WORD>(0x541CE9, 0x34EB);

	InjectHook(0x53FE70, &CPad::GetLookBehindForCar, PATCH_JUMP);
	InjectHook(0x53FEC0, &CPad::GetLookBehindForPed, PATCH_JUMP);
	InjectHook(0x53FDD0, &CPad::GetLookLeft, PATCH_JUMP);
	InjectHook(0x53FE10, &CPad::GetLookRight, PATCH_JUMP);

	InjectHook(0x6C4B24, &CPad::GetHeliLookAroundFloat);
	InjectHook(0x6CB1FE, &CPad::GetHeliLookAroundFloat);
	Patch<DWORD>(0x6C4B29, 0x098C9ED9);
	Patch<DWORD>(0x6C4B2D, 0x1EEB0000);
	Patch<WORD>(0x6CB203, 0x3CEB);

	// CPad::GetJump - compatible
	// CPad::JumpJustDown - compatible

	// CPad::GetMeleeAttack - compatible
	InjectHook(0x53FC90, &CPad::GetPedWalkLeftRight, PATCH_JUMP);
	InjectHook(0x53FD30, &CPad::GetPedWalkUpDown, PATCH_JUMP);

	// CPad::GetSprint - compatible
	// CPad::SprintJustDown - compatible

	InjectHook(0x53FB80, &CPad::GetSteeringLeftRight, PATCH_JUMP);
	InjectHook(0x53FBD0, &CPad::GetSteeringUpDown, PATCH_JUMP);

	InjectHook(0x540670, &CPad::GetTarget, PATCH_JUMP);
	InjectHook(0x5406B0, &CPad::TargetJustDown, PATCH_JUMP);
	InjectHook(0x6E36B4, &CPad::GetHydraTarget);
	InjectHook(0x6E368D, &CPad::HydraTargetJustDown);

	InjectHook(0x540180, &CPad::GetWeapon, PATCH_JUMP);
	InjectHook(0x540250, &CPad::WeaponJustDown, PATCH_JUMP);

	InjectHook(0x68628E, &CPad::PerformReload);
	InjectHook(0x686FC2, &CPad::PerformReload);

	// CPad::LookAroundLeftRight - compatible
	// CPad::LookAroundUpDown - compatible

	// CPad::MeleeAttackJustDown - compatible

	InjectHook(0x540850, &CPad::ShiftTargetLeftJustDown, PATCH_JUMP);
	InjectHook(0x540880, &CPad::ShiftTargetRightJustDown, PATCH_JUMP);

	InjectHook(0x540B30, &CPad::SniperZoomIn, PATCH_JUMP);
	InjectHook(0x540B80, &CPad::SniperZoomOut, PATCH_JUMP);

	InjectHook(0x540BD0, &CPad::SniperModeLookLeftRight, PATCH_JUMP);
	InjectHook(0x540CC0, &CPad::SniperModeLookUpDown, PATCH_JUMP);

	InjectHook(0x6A0E3D, &CPad::GetHydraulicsJump);

	// Sixaxis support
	//InjectHook(0x6A0E53, &CPad::GetHydraulicsLeftRight);
	//InjectHook(0x6A0E70, &CPad::GetHydraulicsUpDown);

	// BMX bunnyhop hooks
	InjectHook(0x6C05EB, &CPad::BunnyHopJustDown, PATCH_CALL);
	Patch<WORD>(0x6C05E9, 0xC88B);
	Patch<DWORD>(0x6C05F0, 0x6074C084);
	Patch<WORD>(0x6C05F4, 0x08EB);

	// BMX steering hooks
	InjectHook(0x6C18BC, &CPad::GetBmxAccelerate);
	InjectHook(0x6BE6E4, BmxAccelerateHook);
	InjectHook(0x6BE748, BmxAccelerateHook);

	InjectHook(0x6BE368, BmxHandBrakeHook);

	// Cinematic camera
	InjectHook(0x51D7BC, CPad::GetCinemaCam);
	Patch<DWORD>(0x51D7C1, 0x90C0BF0F);
	Patch<BYTE>(0x51D7C6, 0x44);

	// Landing gear Southpaw support
	Nop(0x6CB329, 5);
	Patch<WORD>(0x6CB333, 0xC88B);
	InjectHook(0x6CB335, &CPad::LandingGearSwitchJustDown, PATCH_CALL);
	Patch<DWORD>(0x6CB33A, 0x9090C084);
	Patch<BYTE>(0x6CB33F, 0x84);

	Patch<WORD>(0x6C92A0, 0xC88B);
	InjectHook(0x6C92A2, &CPad::LandingGearSwitchJustDown, PATCH_CALL);
	Patch<DWORD>(0x6C92A7, 0x9090C084);
	Patch<WORD>(0x6C92AB, 0x7490);

	// Proper diving via Circle in IV mode
	InjectHook(0x688B2F, &CPad::DiveJustDown);

	// User Tracks skipping
	InjectHook(0x4EBE20, &CPad::UserTracksSkipJustDown);

	// Replay toggles
	//InjectHook(0x4605AA, ReplayKeyHack1);
	//InjectHook(0x460604, ReplayKeyHack2);

	// Jetpack steering
	InjectHook(0x67EDEA, &CPad::GetJetpackStrafeLeft);
	InjectHook(0x67EE2C, &CPad::GetJetpackStrafeRight);
	InjectHook(0x67ED45, &CPad::GetJetpackAscend);
	InjectHook(0x67EDDA, &CPad::GetJetpackAscend);
	InjectHook(0x67ED33, &CPad::GetJetpackDescend);
	InjectHook(0x67EDC8, &CPad::GetJetpackDescend);

	// Sniper zooming
	Patch<const void*>(0x50F306, &fSniperZoomVal);
	Patch<const void*>(0x50F33A, &fSniperZoomVal);
	Patch<const void*>(0x5108FD, &fSniperZoomVal);
	Patch<const void*>(0x510875, &fSniperZoomVal);

	InjectHook(0x7448A5, HandleKeyDownHack);

	Patch<const void*>(0x47A64A, *(bool**)0x50A851);
	Patch<BYTE>(0x47A655, 0x74);

	// Camera not messing up if Joypad mode was seletected before installing GInput
	// m_nController forces itself to 0
	Patch<WORD>(0x57CBC2, 0x978D);		// lea edx, [edi].m_nController
	Patch<DWORD>(0x57CBCC, 0x02C601B1);	// mov cl, 1 \ mov byte ptr [edx], 0
	Patch<BYTE>(0x57CBD0, 0x00);
	//Nop(0x57CBCC, 1);
	//Patch<DWORD>(0x57CBCD, 0x909001B1);

	// Mouse + Keys/Joypad text removed from Redefine Controls menu
	Nop(0x57F737, 5);

	InjectHook(0x576B9F, MenuToggle, PATCH_JUMP);
	Patch<WORD>(0x576C89, 0x11EB);
	Patch<WORD>(0x576E77, 0x11EB);

	// Vibration not defaulting to 1
	Nop(0x5744A3, 4);

	// CPad::Mode not saving
	Patch<DWORD>(0x5D1CD5, 0x9090C931);
	Nop(0x5D1CD9, 3);
	Nop(0x5D1F6E, 7);

	// Joypad buttons not bindable
	Patch<DWORD>(0x530B00, 0x0004C2);
	
	// Joypad handler removed
	Nop(0x744A33, 5);

	// Dancing button
	// mov		ecx, esi
	// lea		eax, [esp+168h+v197]
	// push		eax
	// call		CRunningScript::GetFixedDanceButtons
	// jmp		SetTexture
	/*Patch<DWORD>(0x4840B2, 0x848DCE8B);
	Patch<DWORD>(0x4840B6, 0x0000D024);
	Patch<WORD>(0x4840BA, 0x5000);
	InjectHook(0x4840BC, &CRunningScript::GetFixedDanceButtons, PATCH_CALL);
	Patch<WORD>(0x4840C1, 0x06EB);
	Patch<WORD>(0x4840CF, 0x08EB);*/
	
	// push		ecx
	// mov		ecx, esi
	// call		CRunningScript::GetFixedBeatData
	// push		3
	// mov		ecx, esi
	// jmp		StoreParameters
	/*Patch<BYTE>(0x46C43D, 0x51);
	Patch<WORD>(0x46C43E, 0xCE8B);
	InjectHook(0x46C440, &CRunningScript::GetFixedBeatData, PATCH_CALL);
	Patch<DWORD>(0x46C445, 0xCE8B036A);
	Patch<WORD>(0x46C449, 0x11EB);*/

	// setle	al
	// push		eax
	// push		ecx
	// mov		ecx, esi
	// call		CRunningScript::GetFixedBeatData_Alt
	// push		3
	// mov		ecx, esi
	// jmp		StoreParameters
	/*Patch<BYTE>(0x46C468, 0x51);
	Patch<WORD>(0x46C469, 0xCE8B);
	InjectHook(0x46C46B, &CRunningScript::GetFixedBeatData_Alt, PATCH_CALL);
	Patch<DWORD>(0x46C470, 0xCE8B036A);
	Patch<WORD>(0x46C474, 0x10EB);*/

	// Sixaxis for lowrider competition
	/*Patch<BYTE>(0x48AEEC, 0x56);
	InjectHook(0x48AEED, GetAxesWithSixaxis, PATCH_CALL);
	Patch<WORD>(0x48AEF2, 0x21EB);
	Nop(0x48AF19, 6);*/

	// Map screen hacks
	/*InjectHook(0x575139, OverviewMapToggled, PATCH_CALL);
	Patch<DWORD>(0x57513E, 0x2B75C084);
	Patch<WORD>(0x575142, 0x05EB);
	//Nop(0x575163, 3);
	//InjectHook(0x575166, OverviewMapButton, PATCH_CALL);

	InjectHook(0x578877, MenuOptionsToggled, PATCH_CALL);
	Patch<DWORD>(0x57887C, 0x3375C084);
	Patch<WORD>(0x578880, 0x05EB);

	InjectHook(0x578B8F, MenuOptionsToggled, PATCH_CALL);
	Patch<DWORD>(0x578B94, 0x5075C084);
	Patch<WORD>(0x578B98, 0x05EB);

	InjectHook(0x578BE8, LegendKeyToggled, PATCH_CALL);
	Patch<DWORD>(0x578BED, 0x5F75C084);
	Patch<WORD>(0x578BF1, 0x05EB);*/

	// Test - free aim on joypad
	InjectHook(0x686910, &CPad::JustLockedOn);

	Patch<const void*>(0x48A461, &bFreeAim);
	Patch<const void*>(0x60D351, &bFreeAim);
	Patch<const void*>(0x685A66, &bFreeAim);
	Patch<const void*>(0x685CB6, &bFreeAim);

	Patch<const void*>(0x686906, &bFreeAim);
	Patch<const void*>(0x6869B1, &bFreeAim);
	Patch<const void*>(0x686C65, &bFreeAim);
	Patch<const void*>(0x686CE7, &bFreeAim);

	Patch<const void*>(0x60B65F, &bFreeAim);
	Patch<const void*>(0x60CCA0, &bFreeAim);
}

static StaticPatcher	Patcher([](){ 
						CPad::Inject();
						});