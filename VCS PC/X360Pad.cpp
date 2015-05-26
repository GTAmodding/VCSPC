#include "StdAfx.h"
#include "X360Pad.h"

#include "Pad.h"
#include "Camera.h"

DWORD		(WINAPI *CX360Pad::XInputGetState_GInput)(DWORD dwUserIndex, XINPUT_STATE *pState) = nullptr;
DWORD		(WINAPI *CX360Pad::XInputGetExtended_GInput)(DWORD dwUserIndex, SCP_EXTN* pPressure) = nullptr;
bool		CX360Pad::bInitialised = false;


void CX360Pad::InitializeLibrary()
{
	if ( HMODULE hXinputLibrary = GetModuleHandle("xinput1_3") )
	{
		XInputGetState_GInput = (DWORD(WINAPI*)(DWORD, XINPUT_STATE*))GetProcAddress(hXinputLibrary, (LPCSTR)100);
		if ( !XInputGetState_GInput )
			XInputGetState_GInput = XInputGetState;

		// SCP Driver Package extension
		XInputGetExtended_GInput = (DWORD(WINAPI*)(DWORD, SCP_EXTN*))GetProcAddress(hXinputLibrary, "XInputGetExtended");

		bInitialised = true;
	}
}

void CX360Pad::SetHasPadInHands(bool bHasIt)
{
	if ( bHasPadInHands != bHasIt )
	{
		bHasPadInHands = bHasIt;

		CPad::GetPad(dwPadIndex)->Mode = bHasIt ? CPad::SavedMode : 0;
	}

	CCamera::m_bUseMouse3rdPerson = !bHasIt;
}

DWORD CX360Pad::RetrieveState()
{
	ZeroMemory(&sPadState.Gamepad, sizeof(sPadState.Gamepad));

	if ( !bInitialised )
		InitializeLibrary();

	DWORD dwState = XInputGetState_GInput(dwPadIndex, &sPadState);
	bIsConnected = dwState == ERROR_SUCCESS;

	if ( dwState == ERROR_SUCCESS )
	{
		if ( CheckForInput() )
			SetHasPadInHands(true);
	}
	return dwState;
}

void CX360Pad::Vibrate(unsigned short wLeftMotorSpeed, unsigned short wRightMotorSpeed)
{
	if ( wSavedLeftMotorSpeed != wLeftMotorSpeed || wSavedRightMotorSpeed != wRightMotorSpeed )
	{
		XINPUT_VIBRATION	vibrationData;

		vibrationData.wLeftMotorSpeed = wSavedLeftMotorSpeed = wLeftMotorSpeed;
		vibrationData.wRightMotorSpeed = wSavedRightMotorSpeed = wRightMotorSpeed;

		XInputSetState(dwPadIndex, &vibrationData);
	}
}

bool CX360Pad::CheckForInput()
{
	return	sPadState.Gamepad.wButtons || sPadState.Gamepad.bLeftTrigger > 25 || sPadState.Gamepad.bRightTrigger > 25 ||
			// This could be a MotionInJoy driver bug, but sThumbLY and sThumbRY are -1 for me when idle
			// (it happened on another DualShock 3 too so I guess the pad itself is fine)
			// so I'll apply a tiny threshold for these checks
			sPadState.Gamepad.sThumbLX > 7000 || sPadState.Gamepad.sThumbLY > 7000 || sPadState.Gamepad.sThumbRX > 7000 || sPadState.Gamepad.sThumbRY > 7000 ||
			sPadState.Gamepad.sThumbLX < -7000 || sPadState.Gamepad.sThumbLY < -7000 || sPadState.Gamepad.sThumbRX < -7000 || sPadState.Gamepad.sThumbRY < -7000;
}

// SCP Driver extension
DWORD CX360Pad::RetrieveState_SCP()
{
	if ( XInputGetExtended_GInput == nullptr )
		return ERROR_NOT_SUPPORTED;

	if ( !bInitialised )
		InitializeLibrary();

	DWORD dwState = XInputGetExtended_GInput(dwPadIndex, &sExtendedPadState);
	bIsConnected = dwState == ERROR_SUCCESS;
	bSCPSupported = dwState != ERROR_NOT_SUPPORTED;

	if ( dwState == ERROR_SUCCESS )
	{
		if ( CheckForInput_SCP() )
			SetHasPadInHands(true);
	}

	return dwState;
}

bool CX360Pad::CheckForInput_SCP()
{
	return	sExtendedPadState.SCP_UP > 0.0f || sExtendedPadState.SCP_RIGHT > 0.0f || sExtendedPadState.SCP_DOWN > 0.0f || sExtendedPadState.SCP_LEFT > 0.0f ||
			sExtendedPadState.SCP_LX > (7000.0f/32767.0f) || sExtendedPadState.SCP_LX < (-7000.0f/32767.0f) || sExtendedPadState.SCP_LY > (7000.0f/32767.0f) || sExtendedPadState.SCP_LY < (-7000.0f/32767.0f) ||
			sExtendedPadState.SCP_L1 > 0.0f || sExtendedPadState.SCP_L2 > (25.0f/255.0f) || sExtendedPadState.SCP_L3 > 0.0f ||
			sExtendedPadState.SCP_RX > (7000.0f/32767.0f) || sExtendedPadState.SCP_RX < (-7000.0f/32767.0f) || sExtendedPadState.SCP_RY > (7000.0f/32767.0f) || sExtendedPadState.SCP_RY < (-7000.0f/32767.0f) ||
			sExtendedPadState.SCP_R1 > 0.0f || sExtendedPadState.SCP_R2 > (25.0f/255.0f) || sExtendedPadState.SCP_R3 > 0.0f ||
			sExtendedPadState.SCP_T > 0.0f || sExtendedPadState.SCP_C > 0.0f || sExtendedPadState.SCP_X > 0.0f || sExtendedPadState.SCP_S > 0.0f ||
			sExtendedPadState.SCP_SELECT > 0.0f || sExtendedPadState.SCP_START > 0.0f || sExtendedPadState.SCP_PS > 0.0f;
}


// Custom Sixaxis handling
/*void CX360Pad::RetrieveSixaxisState_BT(UCHAR* Buffer, UCHAR Model)
{
	if ( Model == 1 )	// DS3
	{
		sSixaxisState.sAccelerometerX = (Buffer[41] << 8) + Buffer[42];
		sSixaxisState.sAccelerometerY = (Buffer[43] << 8) + Buffer[44];
		sSixaxisState.sAccelerometerZ = (Buffer[45] << 8) + Buffer[46];
		sSixaxisState.sGyroscope = (Buffer[47] << 8) + Buffer[48];

		bSixaxisWorking = true;

		// Sign extend to 16 bit
		/*if ( sSixaxisState.sAccelerometerX & 0x200 )
			sSixaxisState.sAccelerometerX |= 0xFC00;
		if ( sSixaxisState.sAccelerometerY & 0x200 )
			sSixaxisState.sAccelerometerY |= 0xFC00;
		if ( sSixaxisState.sAccelerometerZ & 0x200 )
			sSixaxisState.sAccelerometerZ |= 0xFC00;
		if ( sSixaxisState.sGyroscope & 0x200 )
			sSixaxisState.sGyroscope |= 0xFC00;*/
/*	}
	else if ( Model == 2 ) // DS4
	{
		sSixaxisState.sAccelerometerX = (Buffer[13] << 8) + Buffer[14];
		sSixaxisState.sAccelerometerY = (Buffer[15] << 8) + Buffer[16];
		sSixaxisState.sAccelerometerZ = (Buffer[17] << 8) + Buffer[18];
		sSixaxisState.sGyroscope = (Buffer[19] << 8) + Buffer[10];

		bSixaxisWorking = true;
	}
}*/