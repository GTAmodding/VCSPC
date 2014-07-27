#ifndef __CPAD
#define __CPAD

#define PADS_IDLE_TIME		60000

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

public:
	inline bool			CrossJustDown() { return NewState.CROSS && !OldState.CROSS; }
	inline bool			RightShockJustDown() { return NewState.RIGHTSHOCK && !OldState.RIGHTSHOCK; }

	CControllerState	ReconcileTwoControllersInput(const CControllerState& rDevice1, const CControllerState& rDevice2);

	static CPad*		GetPad(int nPad);
	static void			UpdatePads();

	// Temp
	static void			Inject();
};

extern CKeyState*					activeKeyState;
extern CKeyState*					currKeyState;
extern CKeyState*					prevKeyState;

static_assert(sizeof(CPad) == 0x134, "Wrong size: CPad");
static_assert(sizeof(CKeyState) == 0x270, "Wrong size: CKeyState");

#endif