#ifndef __RSGLOBALTYPE
#define __RSGLOBALTYPE

struct PsGlobalType
{
	HWND	window;
	DWORD	instance;
	DWORD	fullscreen;
	DWORD	lastMousePos_X;
	DWORD	lastMousePos_Y;
	DWORD	unk;
	DWORD	diInterface;
	DWORD	diMouse;
	void*	diDevice1;
	void*	diDevice2;
};

struct RsGlobalType
{
	const char*		AppName;
	DWORD			MaximumWidth;
	DWORD			MaximumHeight;
	DWORD			frameLimit;
	BOOL			quit;
	PsGlobalType*	ps;
	void*			keyboard;
	void*			mouse;
	void*			pad;
};

#define FUNC_RsEventHandler						0x619B60
#define FUNC_RsCameraBeginUpdate				0x745210
#define FUNC_PsInitialize						0x747420
#define FUNC_RsPathnameCreate					0x745470
#define FUNC_RsPathnameDestroy					0x7454E0

enum RsEvent
{
	rsCAMERASIZE     = 0,
	rsCOMMANDLINE    = 1,
	rsFILELOAD       = 2,
	rsINPUTDEVICEATTACH  = 4,
	rsPLUGINATTACH   = 9,
	rsRWINITIALIZE   = 21,
	rsRWTERMINATE    = 22,
	rsSELECTDEVICE   = 23,
	rsINITIALIZE     = 24,
	rsTERMINATE      = 25,
	rsIDLE           = 26,
	rsRENDER         = 27,
	rsKEYDOWN        = 28,
	rsKEYUP          = 29,
	rsQUITAPP        = 30,
	rsPADBUTTONDOWN  = 31,
	rsPADBUTTONUP    = 32,
	rsPREINITCOMMANDLINE  = 37,
	rsACTIVATE       = 38
};

enum RsEventStatus
{
    rsEVENTERROR,
    rsEVENTPROCESSED,
    rsEVENTNOTPROCESSED
};
typedef enum RsEventStatus RsEventStatus;

RsEventStatus	RsEventHandler(RsEvent eventID, void* param);
bool			RsCameraBeginUpdate();
RwChar*			RsPathnameCreate(const RwChar* srcBuffer);
void			RsPathnameDestroy(RwChar* buffer);
BOOL			PsInitialize();

const DWORD RsGlobalFrameLimits[] = { 0, 25, 30, 50, 60 };

#endif