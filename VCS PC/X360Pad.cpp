#include "StdAfx.h"
#include "X360Pad.h"

#include "Pad.h"

DWORD		CX360Pad::dwReferences = 0;
HMODULE		CX360Pad::hXinputLibrary = nullptr;
DWORD		(WINAPI *CX360Pad::XInputGetState)(DWORD dwUserIndex, XINPUT_STATE *pState) = nullptr;
DWORD		(WINAPI *CX360Pad::XInputSetState)(DWORD dwUserIndex, XINPUT_VIBRATION *pVibration) = nullptr;

void CX360Pad::InitializeLibrary()
{
	hXinputLibrary = LoadLibrary("xinput1_3");
	if ( hXinputLibrary )
	{
		XInputGetState = (DWORD(WINAPI*)(DWORD, XINPUT_STATE*))GetProcAddress(hXinputLibrary, (LPCSTR)100);
		if ( !XInputGetState )
			XInputGetState = (DWORD(WINAPI*)(DWORD, XINPUT_STATE*))GetProcAddress(hXinputLibrary, (LPCSTR)2);

		XInputSetState = (DWORD(WINAPI*)(DWORD, XINPUT_VIBRATION*))GetProcAddress(hXinputLibrary, (LPCSTR)3);
	}

}

bool CX360Pad::AnyNewInput()
{
	static DWORD	dwLastPacket = 1;

	if ( dwSavedPacket != dwLastPacket )
	{
		dwLastPacket = dwSavedPacket;
		return true;
	}
	return false;
}

void CX360Pad::SetHasPadInHands(bool bHasIt)
{
	if ( bHasPadInHands != bHasIt )
	{
		bHasPadInHands = bHasIt;

		// Store/retrieve pad mode
		/*CPad*	pPad = CPad::GetPad(0);

		if ( bHasIt )
			pPad->Mode = pPad->SavedMode;
		else
		{
			pPad->SavedMode = pPad->Mode;
			pPad->Mode = 0;
		}*/
	}
}

bool CX360Pad::RetrieveState()
{
	DWORD		dwState;
	memset(&sPadState, 0, sizeof(XINPUT_STATE));

	if ( !hXinputLibrary )
		InitializeLibrary();

	dwState = XInputGetState(dwPadIndex, &sPadState);
	bIsConnected = dwState == ERROR_SUCCESS;

	if ( dwState == ERROR_SUCCESS )
	{
		if ( sPadState.dwPacketNumber != dwSavedPacket )
		{
			dwSavedPacket = sPadState.dwPacketNumber;
			SetHasPadInHands(true);
		}
		return true;
	}
	return false;
}


short CX360Pad::GetButtonState(int nButton)
{
	switch ( nButton )
	{
	case BUTTON_L2:
		return sPadState.Gamepad.bLeftTrigger;
	case BUTTON_R2:
		return sPadState.Gamepad.bRightTrigger;
	default:
		return (sPadState.Gamepad.wButtons & nButton) ? 255 : 0;
	}
}

void CX360Pad::GetThumbstickState(int nWhich, float* pOut)
{
	if ( !nWhich )	// Left thumbstick
	{
		pOut[0] = sPadState.Gamepad.sThumbLX;
		pOut[1] = static_cast<float>(-sPadState.Gamepad.sThumbLY);
	}
	else	// Right thumbstick
	{
		pOut[0] = sPadState.Gamepad.sThumbRX;
		pOut[1] = static_cast<float>(-sPadState.Gamepad.sThumbRY);
	}
}

void CX360Pad::Vibrate(unsigned short wLeftMotorSpeed, unsigned short wRightMotorSpeed)
{
	if ( wSavedLeftMotorSpeed != wLeftMotorSpeed || wSavedRightMotorSpeed != wRightMotorSpeed )
	{
		XINPUT_VIBRATION	vibrationData;

		vibrationData.wLeftMotorSpeed = wLeftMotorSpeed;
		vibrationData.wRightMotorSpeed = wRightMotorSpeed;

		wSavedLeftMotorSpeed = wLeftMotorSpeed;
		wSavedRightMotorSpeed = wRightMotorSpeed;

		if ( !hXinputLibrary )
			InitializeLibrary();

		//XInputSetState(dwPadIndex, &vibrationData);
	}
}