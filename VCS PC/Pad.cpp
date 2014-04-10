#include "StdAfx.h"
#include "Pad.h"

#include "X360Pad.h"
#include "Timer.h"

static CPad* const	Pads = (CPad*)0xB73458;

static CX360Pad*	pXboxPad = nullptr;

WRAPPER void CPad::UpdatePads() { EAXJMP(0x541DD0); }

static void CapturePad(int nPadID)
{
	if ( !pXboxPad )
		pXboxPad = new CX360Pad(0);

	if ( pXboxPad->RetrieveState() )
	{
		float		nTriggerVal;
		float		nDeadzone;
		float		nPadsState[2];
		CPad*		pPad = CPad::GetPad(0);

		Memory::Patch<BYTE>(0xB73402, 1);

		// Left thumbstick
		pXboxPad->GetThumbstickState(0, nPadsState);

		nDeadzone = nPadsState[1]*nPadsState[1] * (6.96228f*10e-8f) + abs(nPadsState[1]) * 0.00114067f + 7064.1f;	// QUADRATIC DEADZONE
		if ( abs(nPadsState[0]) <= nDeadzone )
			nPadsState[0] = 0;
		else
		{
			if ( nPadsState[0] > 0 )
				nPadsState[0] -= nDeadzone;
			else
				nPadsState[0] += nDeadzone;

			nPadsState[0] *= 127.0f/(32767.0f-nDeadzone);
		}

		nDeadzone = nPadsState[0]*nPadsState[0] * (6.96228f*10e-8f) + abs(nPadsState[0]) * 0.00114067f + 7064.1f;	// QUADRATIC DEADZONE
		if ( abs(nPadsState[1]) <= nDeadzone )
			nPadsState[1] = 0;
		else
		{
			if ( nPadsState[1] > 0 )
				nPadsState[1] -= nDeadzone;
			else
				nPadsState[1] += nDeadzone;

			nPadsState[1] *= 127.0f/(32767.0f-nDeadzone);//(1.0f/(1.0f - nDeadzone*(1.0f/32767.0f)));
		}

		/*if ( bPadLeftXInverted )
			nPadsState[0] = -nPadsState[0];

		if ( bPadLeftYInverted )
			nPadsState[1] = -nPadsState[1];*/

		if ( nPadsState[0] > 127.0f )
			nPadsState[0] = 127.0f;
		else if ( nPadsState[0] < -127.0f )
			nPadsState[0] = -127.0f;

		if ( nPadsState[1] > 127.0f )
			nPadsState[1] = 127.0f;
		else if ( nPadsState[1] < -127.0f )
			nPadsState[1] = -127.0f;

		pPad->PCTempJoyState.LEFTSTICKX = static_cast<short>(/*bPadLeftAxisSwapped ? nPadsState[1] :*/ nPadsState[0]);
		pPad->PCTempJoyState.LEFTSTICKY = static_cast<short>(/*bPadLeftAxisSwapped ? nPadsState[0] :*/ nPadsState[1]);

		// Right thumbstick
		pXboxPad->GetThumbstickState(1, nPadsState);

		nDeadzone = nPadsState[1]*nPadsState[1] * 7.70739f*10e-8f + abs(nPadsState[1]) * 0.00126274f + 7820.1f;
		if ( abs(nPadsState[0]) <= nDeadzone )
			nPadsState[0] = 0;
		else
		{
			if ( nPadsState[0] > 0 )
				nPadsState[0] -= nDeadzone;
			else
				nPadsState[0] += nDeadzone;

			nPadsState[0] *= 127.0f/(32767.0f-nDeadzone);
		}

		nDeadzone = nPadsState[0]*nPadsState[0] * 7.70739f*10e-8f + abs(nPadsState[0]) * 0.00126274f + 7820.1f;
		if ( abs(nPadsState[1]) <= nDeadzone )
			nPadsState[1] = 0;
		else
		{
			if ( nPadsState[1] > 0 )
				nPadsState[1] -= nDeadzone;
			else
				nPadsState[1] += nDeadzone;

			nPadsState[1] *= 127.0f/(32767.0f-nDeadzone);
		}

		/*if ( bPadRightXInverted )
			nPadsState[0] = -nPadsState[0];

		if ( bPadRightYInverted )
			nPadsState[1] = -nPadsState[1];*/

		if ( nPadsState[0] > 127.0f )
			nPadsState[0] = 127.0f;
		else if ( nPadsState[0] < -127.0f )
			nPadsState[0] = -127.0f;

		if ( nPadsState[1] > 127.0f )
			nPadsState[1] = 127.0f;
		else if ( nPadsState[1] < -127.0f )
			nPadsState[1] = -127.0f;

		pPad->PCTempJoyState.RIGHTSTICKX = static_cast<short>(/*bPadRightAxisSwapped ? nPadsState[1] :*/ nPadsState[0]);
		pPad->PCTempJoyState.RIGHTSTICKY = static_cast<short>(/*bPadRightAxisSwapped ? nPadsState[0] :*/ nPadsState[1]);

		// Triggers
		nTriggerVal = pXboxPad->GetButtonState(BUTTON_L2);
		pPad->PCTempJoyState.LEFTSHOULDER2 = static_cast<short>(nTriggerVal > XINPUT_GAMEPAD_TRIGGER_THRESHOLD ? (nTriggerVal-XINPUT_GAMEPAD_TRIGGER_THRESHOLD) * (255.0f/(255.0f-XINPUT_GAMEPAD_TRIGGER_THRESHOLD)) : 0);

		nTriggerVal = pXboxPad->GetButtonState(BUTTON_R2);
		pPad->PCTempJoyState.RIGHTSHOULDER2 = static_cast<short>(nTriggerVal > XINPUT_GAMEPAD_TRIGGER_THRESHOLD ? (nTriggerVal-XINPUT_GAMEPAD_TRIGGER_THRESHOLD) * (255.0f/(255.0f-XINPUT_GAMEPAD_TRIGGER_THRESHOLD)) : 0);

		// Start/Select
		pPad->PCTempJoyState.START = pXboxPad->GetButtonState(BUTTON_START);
		pPad->PCTempJoyState.SELECT = pXboxPad->GetButtonState(BUTTON_SELECT);

		// DPad
		pPad->PCTempJoyState.DPADUP = pXboxPad->GetButtonState(BUTTON_UP);
		pPad->PCTempJoyState.DPADLEFT = pXboxPad->GetButtonState(BUTTON_LEFT);
		pPad->PCTempJoyState.DPADDOWN = pXboxPad->GetButtonState(BUTTON_DOWN);
		pPad->PCTempJoyState.DPADRIGHT = pXboxPad->GetButtonState(BUTTON_RIGHT);

		// Face buttons
		pPad->PCTempJoyState.CROSS = pXboxPad->GetButtonState(BUTTON_CROSS);
		pPad->PCTempJoyState.CIRCLE = pXboxPad->GetButtonState(BUTTON_CIRCLE);
		pPad->PCTempJoyState.TRIANGLE = pXboxPad->GetButtonState(BUTTON_TRIANGLE);
		pPad->PCTempJoyState.SQUARE = pXboxPad->GetButtonState(BUTTON_SQUARE);

		// Bumper buttons
		pPad->PCTempJoyState.LEFTSHOULDER1 = pXboxPad->GetButtonState(BUTTON_L1);
		pPad->PCTempJoyState.LEFTSHOCK = pXboxPad->GetButtonState(BUTTON_L3);
		pPad->PCTempJoyState.RIGHTSHOULDER1 = pXboxPad->GetButtonState(BUTTON_R1);
		pPad->PCTempJoyState.RIGHTSHOCK = pXboxPad->GetButtonState(BUTTON_R3);

		// Secret button!!!
		pPad->PCTempJoyState.HOME = pXboxPad->GetButtonState(BUTTON_PS);

		// Rumble
		int							dwHowLongAgo = CTimer::m_snTimeInMilliseconds - pPad->LastTimeTouched;
		static unsigned int			dwLastTimeRumbleRegistered = 0;

		if ( pXboxPad->HasPadInHands() && dwHowLongAgo <= PADS_IDLE_TIME && pPad->ShakeFreq && pPad->ShakeDur /*&& !pMenuManager->bMenuActive*/ )
		{
			pXboxPad->Vibrate(pPad->ShakeFreq * 257, 0);
			if ( dwLastTimeRumbleRegistered )
			{
				pPad->ShakeDur -= static_cast<short>(CTimer::m_snTimeInMillisecondsPauseMode - dwLastTimeRumbleRegistered);

				if ( pPad->ShakeDur <= 0 )
				{
					pPad->ShakeDur = 0;
					dwLastTimeRumbleRegistered = 0;
					pXboxPad->Vibrate(0, 0);
				}
				else
					dwLastTimeRumbleRegistered = CTimer::m_snTimeInMillisecondsPauseMode;
			}
			else
				dwLastTimeRumbleRegistered = CTimer::m_snTimeInMillisecondsPauseMode;
		}
		else
		{
			if ( (!pXboxPad->HasPadInHands() || dwHowLongAgo > PADS_IDLE_TIME) && pPad->ShakeFreq )
			{
				pPad->ShakeFreq = 0;
				pPad->ShakeDur = 0;
			}
			dwLastTimeRumbleRegistered = 0;
			pXboxPad->Vibrate(0, 0);
		}
	}

}

CControllerState CPad::ReconcileTwoControllersInput(const CControllerState& rDevice1, const CControllerState& rDevice2)
{
	CControllerState			PadOut;

	if ( (rDevice1.LEFTSTICKX > 0 && rDevice2.LEFTSTICKX < 0) || (rDevice1.LEFTSTICKX < 0 && rDevice2.LEFTSTICKX > 0) )
		PadOut.LEFTSTICKX = 0;
	else
	{
		if ( rDevice1.LEFTSTICKX >= 0 && rDevice2.LEFTSTICKX >= 0 )
			PadOut.LEFTSTICKX = max(rDevice1.LEFTSTICKX, rDevice2.LEFTSTICKX);
		else
			PadOut.LEFTSTICKX = min(rDevice1.LEFTSTICKX, rDevice2.LEFTSTICKX);
	}

	if ( (rDevice1.LEFTSTICKY > 0 && rDevice2.LEFTSTICKY < 0) || (rDevice1.LEFTSTICKY < 0 && rDevice2.LEFTSTICKY > 0) )
		PadOut.LEFTSTICKY = 0;
	else
	{
		if ( rDevice1.LEFTSTICKY >= 0 && rDevice2.LEFTSTICKY >= 0 )
			PadOut.LEFTSTICKY = max(rDevice1.LEFTSTICKY, rDevice2.LEFTSTICKY);
		else
			PadOut.LEFTSTICKY = min(rDevice1.LEFTSTICKY, rDevice2.LEFTSTICKY);
	}

	if ( (rDevice1.RIGHTSTICKX > 0 && rDevice2.RIGHTSTICKX < 0) || (rDevice1.RIGHTSTICKX < 0 && rDevice2.RIGHTSTICKX > 0) )
		PadOut.RIGHTSTICKX = 0;
	else
	{
		if ( rDevice1.RIGHTSTICKX >= 0 && rDevice2.RIGHTSTICKX >= 0 )
			PadOut.RIGHTSTICKX = max(rDevice1.RIGHTSTICKX, rDevice2.RIGHTSTICKX);
		else
			PadOut.RIGHTSTICKX = min(rDevice1.RIGHTSTICKX, rDevice2.RIGHTSTICKX);
	}

	if ( (rDevice1.RIGHTSTICKY > 0 && rDevice2.RIGHTSTICKY < 0) || (rDevice1.RIGHTSTICKY < 0 && rDevice2.RIGHTSTICKY > 0) )
		PadOut.RIGHTSTICKY = 0;
	else
	{
		if ( rDevice1.RIGHTSTICKY >= 0 && rDevice2.RIGHTSTICKY >= 0 )
			PadOut.RIGHTSTICKY = max(rDevice1.RIGHTSTICKY, rDevice2.RIGHTSTICKY);
		else
			PadOut.RIGHTSTICKY = min(rDevice1.RIGHTSTICKY, rDevice2.RIGHTSTICKY);
	}

	PadOut.LEFTSHOULDER2 = max(rDevice1.LEFTSHOULDER2, rDevice2.LEFTSHOULDER2);
	PadOut.RIGHTSHOULDER2 = max(rDevice1.RIGHTSHOULDER2, rDevice2.RIGHTSHOULDER2);

	PadOut.LEFTSHOULDER1 = (rDevice1.LEFTSHOULDER1 != 0 || rDevice2.LEFTSHOULDER1 != 0) ? 255 : 0;
	PadOut.RIGHTSHOULDER1 = (rDevice1.RIGHTSHOULDER1 != 0 || rDevice2.RIGHTSHOULDER1 != 0) ? 255 : 0;
	PadOut.DPADUP = (rDevice1.DPADUP != 0 || rDevice2.DPADUP != 0) ? 255 : 0;
	PadOut.DPADDOWN = (rDevice1.DPADDOWN != 0 || rDevice2.DPADDOWN != 0) ? 255 : 0;
	PadOut.DPADLEFT = (rDevice1.DPADLEFT != 0 || rDevice2.DPADLEFT != 0) ? 255 : 0;
	PadOut.DPADRIGHT = (rDevice1.DPADRIGHT != 0 || rDevice2.DPADRIGHT != 0) ? 255 : 0;
	PadOut.SQUARE = (rDevice1.SQUARE != 0 || rDevice2.SQUARE != 0) ? 255 : 0;
	PadOut.TRIANGLE = (rDevice1.TRIANGLE != 0 || rDevice2.TRIANGLE != 0) ? 255 : 0;
	PadOut.CROSS = (rDevice1.CROSS != 0 || rDevice2.CROSS != 0) ? 255 : 0;
	PadOut.CIRCLE = (rDevice1.CIRCLE != 0 || rDevice2.CIRCLE != 0) ? 255 : 0;

	PadOut.START = (rDevice1.START != 0 || rDevice2.START != 0) ? 255 : 0;
	PadOut.SELECT = (rDevice1.SELECT != 0 || rDevice2.SELECT != 0) ? 255 : 0;
	PadOut.LEFTSHOCK = (rDevice1.LEFTSHOCK != 0 || rDevice2.LEFTSHOCK != 0) ? 255 : 0;
	PadOut.RIGHTSHOCK = (rDevice1.RIGHTSHOCK != 0 || rDevice2.RIGHTSHOCK != 0) ? 255 : 0;

	PadOut.HOME = (rDevice1.HOME != 0 || rDevice2.HOME != 0) ? 255 : 0;

	PadOut.m_bPedWalk = (rDevice1.m_bPedWalk != 0 || rDevice2.m_bPedWalk != 0) ? 255 : 0;
	PadOut.m_bVehicleMouseLook = (rDevice1.m_bVehicleMouseLook != 0 || rDevice2.m_bVehicleMouseLook != 0) ? 255 : 0;
	PadOut.m_bRadioTrackSkip = (rDevice1.m_bRadioTrackSkip != 0 || rDevice2.m_bRadioTrackSkip != 0) ? 255 : 0;

	return PadOut;
}

CPad* CPad::GetPad(int nPad)
{
	return &Pads[nPad];
}

void CPad::Inject()
{
	using namespace Memory;

#ifdef DEVBUILD
	InjectHook(0x541DDE, CapturePad);
	InjectMethod(0x53F530, ReconcileTwoControllersInput, PATCH_JUMP);

	Nop(0x748813, 5);
#endif
}