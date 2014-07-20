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

enum RsKeyCodes
{
	rsESC = 1000,

	rsF1,
	rsF2,
	rsF3,
	rsF4,
	rsF5,
	rsF6,
	rsF7,
	rsF8,
	rsF9,
	rsF10,
	rsF11,
	rsF12,

	rsINS,
	rsDEL,
	rsHOME,
	rsEND,
	rsPGUP,
	rsPGDN,

	rsUP,
	rsDOWN,
	rsLEFT,
	rsRIGHT,

	rsPADINS,
	rsPADDEL,
	rsPADHOME,
	rsPADEND,
	rsPADPGUP,
	rsPADPGDN,

	rsPADUP,
	rsPADDOWN,
	rsPADLEFT,
	rsPADRIGHT,

	rsNUMLOCK,
	rsDIVIDE,
	rsTIMES,
	rsMINUS,
	rsPLUS,
	rsPADENTER,
	rsPAD5,

	rsPAD11, rsPAD22,	// ??

	rsBACKSP,
	rsTAB,
	rsCAPSLK,
	rsENTER,
	rsLSHIFT,
	rsRSHIFT,
	rsSHIFT,
	rsLCTRL,
	rsRCTRL,
	rsLALT,
	rsRALT,
	rsLWIN,
	rsRWIN,
	rsAPPS,

	rsNULL
};
typedef enum RsKeyCodes RsKeyCodes;

// R* RW function extensions
RwTexture*		RwTextureGtaStreamRead(RwStream* stream);

RsEventStatus	RsEventHandler(RsEvent eventID, void* param);
RwChar*			RsPathnameCreate(const RwChar* srcBuffer);
void			RsPathnameDestroy(RwChar* buffer);
BOOL			RsCameraBeginUpdate(RwCamera* pCamera);

void			DoRWStuffEndOfFrame();
void			DefinedState2d();
void			CameraSize(RwCamera* camera, RwRect* rect, float fViewWindow, float fAspectRatio);

extern RsGlobalType&		RsGlobal;
extern const DWORD			RsGlobalFrameLimits[5];
extern bool&				bAnisotSupported;

#endif