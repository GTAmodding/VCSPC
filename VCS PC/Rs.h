#ifndef __RS
#define __RS

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
	unsigned int	MaximumWidth;
	unsigned int	MaximumHeight;
	unsigned int	frameLimit;
	BOOL			quit;
	PsGlobalType*	ps;
	void*			keyboard;
	void*			mouse;
	void*			pad;
};

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

RsEventStatus	RsEventHandler(RsEvent eventID, void* param);
RwChar*			RsPathnameCreate(const RwChar* srcBuffer);
void			RsPathnameDestroy(RwChar* buffer);
BOOL			RsCameraBeginUpdate(RwCamera* pCamera);

void			DoRWStuffEndOfFrame();

extern RsGlobalType&		RsGlobal;
extern const DWORD			RsGlobalFrameLimits[5];

#endif