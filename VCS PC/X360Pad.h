#ifndef __X360PAD
#define __X360PAD

#include "XInput_SCP.h"
#include <xinput.h>

#define SIXAXIS_IDLE_X		512
#define SIXAXIS_IDLE_Y		512
#define SIXAXIS_IDLE_Z		400
#define	SIXAXIS_IDLE_GYRO	500

#define SIXAXIS_X_DEADZONE		20
#define SIXAXIS_Y_DEADZONE		20
#define SIXAXIS_Z_DEADZONE		20
#define SIXAXIS_GYRO_DEADZONE	10

typedef struct
{
	WORD				sAccelerometerX;
	WORD				sAccelerometerY;
	WORD				sAccelerometerZ;

	WORD				sGyroscope;
} SCP_SIXAXIS;

class CX360Pad
{
private:
	DWORD				dwPadIndex;
	XINPUT_STATE		sPadState;

	SCP_EXTN			sExtendedPadState;

	// GInput custom hooks
	//SCP_SIXAXIS			sSixaxisState;

	unsigned short		wSavedLeftMotorSpeed, wSavedRightMotorSpeed;
	bool				bIsConnected, bHasPadInHands, bSCPSupported, bSixaxisWorking;

	static DWORD		(WINAPI *XInputGetState_GInput)(DWORD dwUserIndex, XINPUT_STATE *pState);
	// SCP Driver extension
	static DWORD		(WINAPI *XInputGetExtended_GInput)(DWORD dwUserIndex, SCP_EXTN* pPressure);
	static bool			bInitialised;

private:
	static void			InitializeLibrary();

public:
	CX360Pad(DWORD dwPad)
		:	dwPadIndex(dwPad), wSavedLeftMotorSpeed(0), wSavedRightMotorSpeed(0),
			bIsConnected(false),  bHasPadInHands(false), bSCPSupported(false), bSixaxisWorking(false)
	{
	};

	virtual ~CX360Pad()
	{
		Vibrate(0, 0);
	}

	bool			IsPadConnected() const
				{ return bIsConnected; };

	bool			HasPadInHands() const
				{ return bHasPadInHands; };

	bool					CheckForInput();
	void					SetHasPadInHands(bool bHasIt);
	DWORD					RetrieveState();

	void					Vibrate(unsigned short wLeftMotorSpeed, unsigned short wRightMotorSpeed);
	//void					RetrieveSixaxisState_BT(UCHAR* Buffer, UCHAR Model);

	inline XINPUT_GAMEPAD*	GetPadState()
		{ return &sPadState.Gamepad; }

	// SCP Driver extension
	bool					CheckForInput_SCP();
	DWORD					RetrieveState_SCP();
	SCP_EXTN*				GetPadState_SCP()
		{ return &sExtendedPadState; }
	/*SCP_SIXAXIS*			GetSixaxisState()
		{ return &sSixaxisState; }*/

	static void				FinalShutdown();
};

#endif