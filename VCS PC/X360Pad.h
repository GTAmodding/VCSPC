#ifndef __X360PAD
#define __X360PAD

#define BUTTON_UP		XINPUT_GAMEPAD_DPAD_UP
#define BUTTON_DOWN		XINPUT_GAMEPAD_DPAD_DOWN
#define BUTTON_LEFT		XINPUT_GAMEPAD_DPAD_LEFT
#define BUTTON_RIGHT	XINPUT_GAMEPAD_DPAD_RIGHT
#define BUTTON_START	XINPUT_GAMEPAD_START
#define BUTTON_SELECT	XINPUT_GAMEPAD_BACK
#define BUTTON_L3		XINPUT_GAMEPAD_LEFT_THUMB
#define BUTTON_R3		XINPUT_GAMEPAD_RIGHT_THUMB
#define BUTTON_L1		XINPUT_GAMEPAD_LEFT_SHOULDER
#define BUTTON_R1		XINPUT_GAMEPAD_RIGHT_SHOULDER
#define BUTTON_PS		0x0400
#define BUTTON_CROSS	XINPUT_GAMEPAD_A
#define BUTTON_CIRCLE	XINPUT_GAMEPAD_B
#define BUTTON_SQUARE	XINPUT_GAMEPAD_X
#define BUTTON_TRIANGLE	XINPUT_GAMEPAD_Y
#define BUTTON_L2		-1
#define BUTTON_R2		-2

#define BUTTON_THUMBLXL -10
#define BUTTON_THUMBLXR -11
#define BUTTON_THUMBLYU -12
#define BUTTON_THUMBLYD -13
#define BUTTON_THUMBRXL -14
#define BUTTON_THUMBRXR -15
#define BUTTON_THUMBRYU -16
#define BUTTON_THUMBRYD -17

class CX360Pad
{
private:
	DWORD				dwPadIndex;
	DWORD				dwSavedPacket;
	XINPUT_STATE		sPadState;
	unsigned short		wSavedLeftMotorSpeed, wSavedRightMotorSpeed;
	bool				bIsConnected, bHasPadInHands;

	static DWORD		dwReferences;
	static HMODULE		hXinputLibrary;
	static DWORD		(WINAPI *XInputGetState)(DWORD dwUserIndex, XINPUT_STATE *pState);
	static DWORD		(WINAPI *XInputSetState)(DWORD dwUserIndex, XINPUT_VIBRATION *pVibration);

private:
	void				InitializeLibrary();

public:
	CX360Pad(DWORD dwPad)
		:	dwPadIndex(dwPad), dwSavedPacket(1), wSavedLeftMotorSpeed(0), wSavedRightMotorSpeed(0),
			bIsConnected(false),  bHasPadInHands(false)
	{
		++dwReferences;
	}

	~CX360Pad()
	{
		if ( RetrieveState() )
			Vibrate(0, 0);

		if ( !(--dwReferences) )
		{
			FreeLibrary(hXinputLibrary);
			hXinputLibrary = nullptr;
		}
	}

	inline bool		IsPadConnected()
				{ return bIsConnected; }

	inline bool		HasPadInHands()
				{ return bHasPadInHands; }

	bool					AnyNewInput();
	void					SetHasPadInHands(bool bHasIt);
	bool					RetrieveState();
	short					GetButtonState(int nButton);
	void					GetThumbstickState(int nWhich, float* pOut);
	void					Vibrate(unsigned short wLeftMotorSpeed, unsigned short wRightMotorSpeed);
};

#endif