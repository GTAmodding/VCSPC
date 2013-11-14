#ifndef __CPAD
#define __CPAD

#define WIN32_LEAN_AND_MEAN

#define FUNC_CPad__TestFireButton							0x53FFE0
#define FUNC_CPad__UpdatePads								0x541DD0

class CPad;
extern CPad*				pads;

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

class CPad
{
private:
	struct CControllerState
	{
		WORD	LR;
		WORD	UD;
		WORD	SLR;
		WORD	SUD;
		WORD	ACTION_SECFIRE;
		WORD	SCRLUP_LOOKLEFT;
		WORD	AIMWEAP_HANDBRK;
		WORD	SCRLDWN_LOOKRIGHT;
		WORD	GRPFWD_NEXTRADIO;
		WORD	GRPBCK_PREVRADIO;
		WORD	NO;
		WORD	YES;
		WORD	PADACTIVE;
		WORD	CHANGECAM;
		WORD	JUMP_REVERSE;
		WORD	ENTER_EXIT;
		WORD	SPRNT_ACCEL;
		WORD	FIREWEAP;
		WORD	CROUCH_HORN;
		WORD	LOOKBACK_SUBMISSION;
		WORD	NETWORK_TALK;
		WORD	WALK;
		WORD	MOUSELOOK;
		WORD	UTRACKSKIP;
	};
	CControllerState	NewState;
	CControllerState	OldState;
	WORD				SteeringLeftRightBuffer[10];
	DWORD				DrunkDrivingBufferUsed;
	CControllerState	PCTempKeyState;
	CControllerState	PCTempJoyState;
	CControllerState	PCTempMouseState;
	BYTE				Phase;
	WORD				Mode;
	WORD				ShakeDur;
	WORD				DisablePlayerControls;
	BYTE				ShakeFreq;
	BYTE				bHornHistory[5];
	BYTE				iCurrHornHistory;
	BYTE				JustOutOfFrontEnd;
	BYTE				bApplyBrakes;
	BYTE				bDisablePlayerEnterCar;
	BYTE				bDisablePlayerDuck;
	BYTE				bDisablePlayerFireWeapon;
	BYTE				bDisablePlayerFireWeaponWithL1;
	BYTE				bDisablePlayerCycleWeapon;
	BYTE				bDisablePlayerJump;
	BYTE				bDisablePlayerDisplayVitalStats;
	DWORD				LastTimeTouched;
	DWORD				AverageWeapon;
	DWORD				AverageEntries;
	DWORD				NoShakeBeforeThis;
	BYTE				NoShakeFreq;

public:
	bool				IsFireWeaponJustPressed() { return NewState.FIREWEAP && !OldState.FIREWEAP; };
	bool				IsActionOrSecondaryFireJustDown() { return NewState.ACTION_SECFIRE && !OldState.ACTION_SECFIRE; };
	static CPad*		GetPad(int padNum) { return &pads[padNum]; };
	CControllerState&	GetNewState() { return NewState; };
	CControllerState&	GetOldState() { return OldState; };

	WORD				TestFireButton();
	static void			UpdatePads();
};

static_assert(sizeof(CPad) == CPad_ARRAYSIZE, "CPad class has wrong size!");
static_assert(sizeof(CKeyState) == CKeyState_ARRAYSIZE, "CKeyState class has wrong size!");

#endif