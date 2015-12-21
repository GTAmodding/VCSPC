#ifndef __CPAD
#define __CPAD

#include "Timer.h"
#include "X360Pad.h"

#define PADS_IDLE_TIME		60000
#define CHANGE_STATION_HOLD_TIME	1000
#define	CHANGE_CAMERA_HOLD_TIME		1500

#define LOCK_ON_THRESHOLD			125

#define CURRENT_XINPUT_PAD	0	// Might need changing?
#define PAD_IV_CONTROLS_MODE 1

struct CKeyState
{
	WORD				FKeys[12];
	WORD				standardKeys[256];
	WORD				esc;
	WORD				insert;
	WORD				del;
	WORD				home;
	WORD				end;
	WORD				pgup;
	WORD				pgdn;
	WORD				up;
	WORD				down;
	WORD				left;
	WORD				right;
	WORD				scroll;
	WORD				pause;
	WORD				numlock;
	WORD				div;
	WORD				mul;
	WORD				sub;
	WORD				add;
	WORD				enter;
	WORD				decimal;
	WORD				num1;
	WORD				num2;
	WORD				num3;
	WORD				num4;
	WORD				num5;
	WORD				num6;
	WORD				num7;
	WORD				num8;
	WORD				num9;
	WORD				num0;
	WORD				back;
	WORD				tab;
	WORD				capslock;
	WORD				extenter;
	WORD				lshift;
	WORD				rshift;
	WORD				shift;
	WORD				lctrl;
	WORD				rctrl;
	WORD				lmenu;
	WORD				rmenu;
	WORD				lwin;
	WORD				rwin;
	WORD				apps;
};

class CMouseControllerState
{
public:
	bool	lmb;
	bool	rmb;
	bool	mmb;
	bool	wheelUp;
	bool	wheelDown;
	bool	bmx1;
	bool	bmx2;
	float	Z;
	float	X;
	float	Y;

public:
	inline bool		CheckForInput()
		{ return lmb || rmb || mmb || wheelUp || wheelDown || bmx1 || bmx2 || X != 0.0f || Y != 0.0f; }
};

class CControllerState
{
public:
	short LEFTSTICKX;
	short LEFTSTICKY;
	short RIGHTSTICKX;
	short RIGHTSTICKY;

	short LEFTSHOULDER1;
	short LEFTSHOULDER2;
	short RIGHTSHOULDER1;
	short RIGHTSHOULDER2;

	short DPADUP;
	short DPADDOWN;
	short DPADLEFT;
	short DPADRIGHT;

	short START;
	short SELECT;

	short SQUARE;
	short TRIANGLE;
	short CROSS;
	short CIRCLE;

	short LEFTSHOCK;
	short RIGHTSHOCK;

	short HOME;

	short m_bPedWalk;
	short m_bVehicleMouseLook;
	short m_bRadioTrackSkip;
};

class CPad
{
public:
	CControllerState	NewState;
	CControllerState	OldState;
	WORD				SteeringLeftRightBuffer[10];
	DWORD				DrunkDrivingBufferUsed;
	CControllerState	PCTempKeyState;
	CControllerState	PCTempJoyState;
	CControllerState	PCTempMouseState;
	BYTE				Phase;
	WORD				Mode;
	short				ShakeDur;
	WORD				DisablePlayerControls;
	BYTE				ShakeFreq;
	BYTE				bHornHistory[5];
	BYTE				iCurrHornHistory;
	bool				JustOutOfFrontEnd;
	bool				bApplyBrakes;
	bool				bDisablePlayerEnterCar;
	bool				bDisablePlayerDuck;
	bool				bDisablePlayerFireWeapon;
	bool				bDisablePlayerFireWeaponWithL1;
	bool				bDisablePlayerCycleWeapon;
	bool				bDisablePlayerJump;
	bool				bDisablePlayerDisplayVitalStats;
	int					LastTimeTouched;
	DWORD				AverageWeapon;
	DWORD				AverageEntries;
	DWORD				NoShakeBeforeThis;
	BYTE				NoShakeFreq;

private:
	static CMouseControllerState&	PCTempMouseControllerState;
	static CMouseControllerState&	NewMouseControllerState;
	static CMouseControllerState&	OldMouseControllerState;

public:
	// GInput extension
	static bool				bInvertLook4Pad;
	static bool				bSouthpaw;
	static WORD				SavedMode;
	static short			ChangeStation_HoldTimer;
	static short			ChangeCamera_HoldTimer;
	static bool				FailCameraChangeThisFrame;

public:
	inline bool								CrossJustDown() { return NewState.CROSS && !OldState.CROSS; }
	inline bool								RightShockJustDown() { return NewState.RIGHTSHOCK && !OldState.RIGHTSHOCK; }
	static inline CMouseControllerState&	GetMouseStateBuffer() { return PCTempMouseControllerState; }
	inline int			InputHowLongAgo() { return CTimer::m_snTimeInMilliseconds - LastTimeTouched; }

	CControllerState	ReconcileTwoControllersInput(const CControllerState& rDevice1, const CControllerState& rDevice2);
	void				UpdateMouse();

	static CPad*		GetPad(int nPad);
	static void			UpdatePads();

	bool						SniperZoomIn();
	bool						SniperZoomOut();
	bool						ForceCameraBehindPlayer();
	bool						ChangeStationJustDown();
	bool						ChangeStationDownJustUp();
	bool						ChangeStationUpJustUp();
	bool						CollectPickupJustDown();
	bool						CycleCameraModeJustDown();
	bool						CycleCameraModeUpJustDown();
	bool						GetTarget();
	bool						ShiftTargetRightJustDown();
	bool						ShiftTargetLeftJustDown();
	bool						GetSprint();
	bool						TargetJustDown();
	bool						CycleWeaponRightJustDown();
	bool						CycleWeaponLeftJustDown();
	bool						ExitVehicleJustDown();
	bool						GetExitVehicle();
	bool						GetHorn();
	bool						HornJustDown();
	bool						GetLookLeft();
	bool						GetLookRight();
	bool						GetLookBehindForPed();
	bool						GetLookBehindForCar();
	bool						DuckJustDown();
	bool						GetDuck();
	bool						ConversationNoJustDown();
	bool						ConversationYesJustDown();
	bool						GetGroupControlForward();
	bool						GetGroupControlBack();
	short						GetAccelerate();
	short						GetBrake();
	short						GetHandBrake();
	short						GetPedWalkLeftRight();
	short						GetPedWalkUpDown();
	short						GetSteeringLeftRight();
	short						GetSteeringUpDown();
	short						GetCarGunLeftRight();
	short						GetCarGunUpDown();
	short						LookAroundLeftRight();
	short						LookAroundUpDown();
	short						GetBmxHandBrake();
	short						GetBmxAccelerate();
	short						CarGunJustDown();
	short						PlaneGunJustDown();
	short						HydraGunJustDown();
	short						GetCarGunFired();
	short						GetPlaneGunFired();
	short						AimWeaponLeftRight(class CPed* pPed);
	short						AimWeaponUpDown(class CPed* pPed);
	bool						WeaponJustDown(class CPed* pPed);
	bool						GetHydraTarget();
	bool						HydraTargetJustDown();
	bool						BunnyHopJustDown();
	bool						LandingGearSwitchJustDown();
	bool						DiveJustDown(class CPed* pPed);
	int							GetWeapon(class CPed* pPed);
	short						SniperModeLookUpDown(class CPed* pPed);
	short						SniperModeLookLeftRight(class CPed* pPed);
	bool						GetJetpackStrafeLeft();
	bool						GetJetpackStrafeRight();
	short						GetJetpackAscend();
	short						GetJetpackDescend();
	short						GetDisplayVitalStats(class CPed* pPed);
	bool						JustLockedOn();
	bool						JustLockedOn_NoMeleeCheck();
	bool						GetLockOn();
	bool						HasBeenTargetting();
	bool						UserTracksSkipJustDown();
	static short				GetCinemaCam();

	bool						ReloadJustDown();
	int							PerformReload(class CPed* pPed);

	// PS2 functions
	void						StopShaking(short nPadID);
	static void					StopPadsShaking();
	void						StartShake(short nTime, unsigned char nDur, unsigned int nNoShakeBeforeThis);

	// New GInput functions	
	bool						FrontEndBackJustDown();
	bool						BlowUpRCBuggyJustDown();
	bool						GetBlowUpRCBuggy();
	//bool						GetLookLeftForHeli();		// Deprecated in 1.02
	//bool						GetLookRightForHeli();		// Deprecated in 1.02
	bool						GetLookBehindForHeli();
	bool						GetHydraulicsJump();
	bool						ChangeStationJustUp();
	bool						GetChangeStationHeld();
	bool						ChangeStationJustStoppedHolding();
	bool						ChangeStationJustHeld();
	bool						GetChangeStationDown();
	bool						GetChangeStationUp();
	bool						GetCycleCameraMode();
	bool						CycleCameraModeJustUp();
	bool						CycleCameraModeUpJustUp();
	bool						CycleCameraModeDownJustUp();
	bool						CycleCameraModeJustHeld();
	bool						CycleCameraModeJustStoppedHolding();
	bool						GetFreeAimTarget();
	bool						FreeAimTargetJustDisabled();
	short						GetBikeLeanUpDown();
	short						GetHeliForwardsBackwards();
	short						GetHeliAscend();
	short						GetHeliDescend();
	short						GetHeliRotationLeftRight();
	short						GetHydraulicsLeftRight();
	short						GetHydraulicsUpDown();
	float						GetHeliLookAroundFloat();

	inline bool					GetAnaloguePadUp()
					{ return NewState.LEFTSTICKY < -10 && OldState.LEFTSTICKY >= -5; }
	inline bool					GetAnaloguePadDown()
					{ return NewState.LEFTSTICKY > 10 && OldState.LEFTSTICKY <= 5; }
	inline bool					GetAnaloguePadLeft()
					{ return NewState.LEFTSTICKX < -10 && OldState.LEFTSTICKX >= -5; }
	inline bool					GetAnaloguePadRight()
					{ return NewState.LEFTSTICKX > 10 && OldState.LEFTSTICKX <= 5; }

	inline short				GetAnalogueLeftRight()
					{ return NewState.LEFTSTICKX; }
	inline short				GetAnalogueUpDown()
					{ return NewState.LEFTSTICKY; }

	inline bool					GetAnaloguePadLeftJustUp()
					{ return NewState.LEFTSTICKX == 0 && OldState.LEFTSTICKX < 0; }
	inline bool					GetAnaloguePadRightJustUp()
					{ return NewState.LEFTSTICKX == 0 && OldState.LEFTSTICKX > 0; }

	void						Update(short nPadID);

	static void					DoCheats();
	void						UpdateHoldTimers();
	void						UpdateHoldTimer(short& Timer, short nUpTimer, bool bDown);

	// Temp
	static void			Inject();
};

extern CX360Pad*					pXboxPad[1];
extern CKeyState*					activeKeyState;
extern CKeyState*					currKeyState;
extern CKeyState*					prevKeyState;

extern bool							bSwapSticksDuringAiming[1];

void OnModeChangePatches();

static_assert(sizeof(CPad) == 0x134, "Wrong size: CPad");
static_assert(sizeof(CKeyState) == 0x270, "Wrong size: CKeyState");
static_assert(sizeof(CMouseControllerState) == 0x14, "Wrong size: CMouseControllerState");

#endif