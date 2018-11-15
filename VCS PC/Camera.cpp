#include "StdAfx.h"
#include "Camera.h"

#include "gtamain.h"
#include "Sprite.h"
#include "ControlsMgr.h"
#include "Pad.h"
#include "Ped.h"
#include "PlayerInfo.h"
#include "debugmenu_public.h"

bool CCamera::bDontTouchFOVInWidescreen;
float& CCamera::m_fMouseAccelHorzntl = *(float*)0xB6EC1C;
bool& CCamera::m_bUseMouse3rdPerson = *(bool*)0xB6EC2E;

WRAPPER void CamShakeNoPos(CCamera* pCamera, float fStrength) { WRAPARG(pCamera); WRAPARG(fStrength); EAXJMP(0x50A970); }

WRAPPER void CCamera::Process() { EAXJMP(0x52B730); }
WRAPPER void CCamera::ProcessFade() { EAXJMP(0x50B5D0); }
WRAPPER int CCamera::GetFadeStage() { EAXJMP(0x50AE20); }
WRAPPER int CCamera::GetLookDirection() { EAXJMP(0x50AE90); }
WRAPPER bool CCamera::IsPositionVisible(const CVector& vecPos, float fRadius) { WRAPARG(vecPos); WRAPARG(fRadius); EAXJMP(0x420D40); }
WRAPPER bool CCamera::IsSphereVisible_(const CVector& center, float fRadius, RwMatrix *mat) { WRAPARG(center); WRAPARG(fRadius); WRAPARG(mat); EAXJMP(0x420C40); }
WRAPPER float CCamera::Find3rdPersonQuickAimPitch(CCamera* pCamera) { WRAPARG(pCamera); EAXJMP(0x50AD40); }

void CCamera::DrawBordersForWideScreen()
{
	if ( bDontTouchFOVInWidescreen )
		return;

	CRect		ScreenRect;

	ScreenRect.x1 = -1000.0f;
	ScreenRect.y1 = -1000.0f;
	ScreenRect.x2 = -1000.0f;
	ScreenRect.y2 = -1000.0f;

	GetScreenRect(ScreenRect);

	/* May be unused?
	if ( m_BlurType == 0 || m_BlurType == 2 )
		m_imotionBlurAddAlpha = 80; */

	// Letterbox
	if ( ScreenRect.y1 > 0.0 && ScreenRect.y2 > 0.0 )
	{		
		CSprite2d::DrawRect(CRect(-5.0f, -5.0f, RsGlobal.MaximumWidth + 5.0f, ScreenRect.y1), CRGBA(0, 0, 0, 255));
		CSprite2d::DrawRect(CRect(-5.0f, ScreenRect.y2, RsGlobal.MaximumWidth + 5.0f, RsGlobal.MaximumHeight + 5.0f), CRGBA(0, 0, 0, 255));
	}
	// Pillarbox
	else if ( ScreenRect.x1 > 0.0 && ScreenRect.x2 > 0.0 )
	{		
		CSprite2d::DrawRect(CRect(-5.0f, -5.0f, ScreenRect.x1, RsGlobal.MaximumHeight + 5.0f), CRGBA(0, 0, 0, 255));
		CSprite2d::DrawRect(CRect(ScreenRect.x2, -5.0f, RsGlobal.MaximumWidth + 5.0f, RsGlobal.MaximumHeight + 5.0f), CRGBA(0, 0, 0, 255));
	}
}

void CCamera::GetScreenRect(CRect& rect)
{
	float			fScreenRatio = ScreenAspectRatio;
	float			dScreenHeightWeWannaCut = ((-9.0f/16.0f) * fScreenRatio + 1.0f);
	float			dBorderProportionsFix = ((-144643.0f/50000.0f) * fScreenRatio * fScreenRatio) + ((807321.0f/100000.0f) * fScreenRatio) - (551143.0f/100000.0f);

	if ( dBorderProportionsFix < 0.0 )
		dBorderProportionsFix = 0.0;

	if ( dScreenHeightWeWannaCut > 0.0 )
	{
		// Letterbox
		rect.y1 = (RsGlobal.MaximumHeight / 2) * (dScreenHeightWeWannaCut - dBorderProportionsFix);
		rect.y2 = RsGlobal.MaximumHeight - ((RsGlobal.MaximumHeight / 2) * (dScreenHeightWeWannaCut + dBorderProportionsFix));
	}
	else
	{
		// Pillarbox
		dScreenHeightWeWannaCut = -dScreenHeightWeWannaCut;

		rect.x1 = (RsGlobal.MaximumWidth / 4) * dScreenHeightWeWannaCut;
		rect.x2 = RsGlobal.MaximumWidth - (RsGlobal.MaximumWidth / 4) * dScreenHeightWeWannaCut;
	}
}



/*
 * Debug Cam
 */

static uint32 thisptr;
static uint32 baseptr;

WRAPPER void CCam::Process(void) { EAXJMP(0x526FC0); }
WRAPPER void CCam::GetVectorsReadyForRW(void) { EAXJMP(0x509CE0); }
WRAPPER void CCamera::CopyCameraMatrixToRWCam(bool) { EAXJMP(0x50AFA0); }

static int toggleDebugCamSwitch;
static int toggleDebugCam;

enum Controlmode {
	CONTROL_CAMERA,
	CONTROL_PLAYER,
};
int controlMode = CONTROL_CAMERA;
int activatedFromKeyboard;

float gFOV = 70.0f;

#define PI M_PI
#define RAD2DEG(x) (float)(180.0f*(x)/M_PI)
#define DEG2RAD(x) (float)((x)/180.0f*M_PI)
#define JUSTDOWN(b) (pad->NewState.b && !pad->OldState.b)
#define KEYJUSTDOWN(k) ControlsManager.GetIsKeyboardKeyJustDown((RsKeyCodes)k)
#define KEYDOWN(k) ControlsManager.GetIsKeyboardKeyDown((RsKeyCodes)k)
#define CTRLJUSTDOWN(key) \
	       ((KEYDOWN(rsLCTRL) || KEYDOWN(rsRCTRL)) && KEYJUSTDOWN((RsKeyCodes)key) || \
	        (KEYJUSTDOWN(rsLCTRL) || KEYJUSTDOWN(rsRCTRL)) && KEYDOWN((RsKeyCodes)key))
#define CTRLDOWN(key) ((KEYDOWN(rsLCTRL) || KEYDOWN(rsRCTRL)) && KEYDOWN((RsKeyCodes)key))


void
CCamera::InitialiseCameraForDebugMode(void)
{
	CEntity *e;
	if(e = FindPlayerVehicle(-1, true))
		this->Cams[2].Source = e->GetCoords();
	else if(e = FindPlayerPed(-1))
		this->Cams[2].Source = e->GetCoords();
	this->Cams[2].Alpha = 0;
	this->Cams[2].Beta = 0;
	this->Cams[2].Mode = 6;
//	CPad::m_bMapPadOneToPadTwo = 1;

	Cams[2].Source = Cams[ActiveCam].Source;
	CVector nfront = Cams[ActiveCam].Front;
	float groundDist = sqrt(nfront.x*nfront.x + nfront.y*nfront.y);
	Cams[2].Beta = CGeneral::GetATanOfXY(nfront.x, nfront.y);
	Cams[2].Alpha = CGeneral::GetATanOfXY(groundDist, nfront.z);
	while(Cams[2].Beta >= PI) Cams[2].Beta -= 2.0f*PI;
	while(Cams[2].Beta < -PI) Cams[2].Beta += 2.0f*PI;
	while(Cams[2].Alpha >= PI) Cams[2].Alpha -= 2.0f*PI;
	while(Cams[2].Alpha < -PI) Cams[2].Alpha += 2.0f*PI;

//	LoadSavedCams();
}

void
CCam::Process_Debug(float*, float, float, float)
{
	static float speed = 0.0f;
	static float panspeedX = 0.0f;
	static float panspeedY = 0.0f;

	if(CTRLJUSTDOWN('C')){
		if(controlMode == CONTROL_CAMERA) controlMode = CONTROL_PLAYER;
		else if(controlMode == CONTROL_PLAYER) controlMode = CONTROL_CAMERA;
	}

//	if(KEYJUSTDOWN('Z') && controlMode == CONTROL_CAMERA)
//		SaveCam(this);
//	if(KEYJUSTDOWN('X') && controlMode == CONTROL_CAMERA)
//		DeleteSavedCams();
//	if(KEYJUSTDOWN('Q') && controlMode == CONTROL_CAMERA)
//		PrevSavedCam(this);
//	if(KEYJUSTDOWN('E') && controlMode == CONTROL_CAMERA)
//		NextSavedCam(this);

	RwCameraSetNearClipPlane(Scene.camera, 0.9f);
	this->FOV = gFOV;
	this->Alpha += DEG2RAD(CPad::GetPad(1)->NewState.LEFTSTICKY*0.02f); // magic
	this->Beta  -= CPad::GetPad(1)->NewState.LEFTSTICKX * 0.02617994f * 0.052631579f; // magic
	if(controlMode == CONTROL_CAMERA && CPad::NewMouseControllerState.lmb){
		this->Alpha += DEG2RAD(CPad::NewMouseControllerState.Y/2.0f);
		this->Beta -= DEG2RAD(CPad::NewMouseControllerState.X/2.0f);
	}

	if(this->Alpha > DEG2RAD(89.5f)) this->Alpha = DEG2RAD(89.5f);
	if(this->Alpha < DEG2RAD(-89.5f)) this->Alpha = DEG2RAD(-89.5f);

	CVector vec;
	vec.x = this->Source.x + cos(this->Beta) * cos(this->Alpha) * 3.0f;
	vec.y = this->Source.y + sin(this->Beta) * cos(this->Alpha) * 3.0f;
	vec.z = this->Source.z + sin(this->Alpha) * 3.0f;

	if(CPad::GetPad(1)->NewState.SQUARE ||
	   KEYDOWN((RsKeyCodes)'W') && controlMode == CONTROL_CAMERA)
		speed += 0.1f;
	else if(CPad::GetPad(1)->NewState.CROSS ||
	        KEYDOWN((RsKeyCodes)'S') && controlMode == CONTROL_CAMERA)
		speed -= 0.1f;
	else
		speed = 0.0f;

	if(speed > 70.0f) speed = 70.0f;
	if(speed < -70.0f) speed = -70.0f;

	if(KEYDOWN((RsKeyCodes)rsRIGHT) && controlMode == CONTROL_CAMERA)
		panspeedX += 0.1f;
	else if(KEYDOWN((RsKeyCodes)rsLEFT) && controlMode == CONTROL_CAMERA)
		panspeedX -= 0.1f;
	else
		panspeedX = 0.0f;
	if(panspeedX > 70.0f) panspeedX = 70.0f;
	if(panspeedX < -70.0f) panspeedX = -70.0f;

	if(KEYDOWN((RsKeyCodes)rsUP) && controlMode == CONTROL_CAMERA)
		panspeedY += 0.1f;
	else if(KEYDOWN((RsKeyCodes)rsDOWN) && controlMode == CONTROL_CAMERA)
		panspeedY -= 0.1f;
	else
		panspeedY = 0.0f;
	if(panspeedY > 70.0f) panspeedY = 70.0f;
	if(panspeedY < -70.0f) panspeedY = -70.0f;

	this->Front = vec - this->Source;
	this->Front.Normalise();
	this->Source = this->Source + this->Front*speed;

	CVector up = { 0.0f, 0.0f, 1.0f };
	CVector right;
	right = CrossProduct(Front, up);
	up = CrossProduct(right, Front);
	Source = Source + up*panspeedY + right*panspeedX;

	if(this->Source.z < -450.0f)
		this->Source.z = -450.0f;

	CPad *pad = CPad::GetPad(1);
	if(JUSTDOWN(RIGHTSHOULDER2) ||
	   KEYJUSTDOWN(rsENTER) && controlMode == CONTROL_CAMERA){
		if(FindPlayerVehicle(-1, false)){
			CEntity *e = FindPlayerVehicle(-1, false);
			e->Teleport(this->Source.x, this->Source.y, this->Source.z, false);
		}else if(FindPlayerPed(-1)){
			CEntity *e = FindPlayerPed(-1);
			e->SetCoords(this->Source);
		}
	}

	if(controlMode == CONTROL_CAMERA && activatedFromKeyboard)
		CPad::GetPad(0)->DisablePlayerControls = 1;

	GetVectorsReadyForRW();
}

static void __declspec(naked)
switchDefaultHook(void)
{
	_asm{
		movsx	eax, word ptr [esi+0Ch]
		cmp	eax,6
		jne	def

		// standard code for most cases:
		mov     ecx, [esp+0x10]
		mov     edx, [esi+0D4h]
		mov     eax, [esp+0x14]
		push    ecx
		push    edx
		push    eax
		lea     ecx, [esp+0x2C]
		push    ecx
		mov     ecx, esi
		call	CCam::Process_Debug
		push	0x527CF3
		retn

	def:
		mov	[esp+0x38], 0
		push	0x527C65
		retn
	}
}


static void
toggleCam(void)
{
	CCamera *cam = (CCamera*)thisptr;
	CPad *pad = CPad::GetPad(1);
	int keydown = CTRLJUSTDOWN('B') || toggleDebugCam;
	if(JUSTDOWN(CIRCLE) || keydown){
		toggleDebugCam = 0;
		cam->WorldViewerBeingUsed = !cam->WorldViewerBeingUsed;
		if(cam->WorldViewerBeingUsed){
			activatedFromKeyboard = keydown;
			cam->InitialiseCameraForDebugMode();
		}
	}
}

static void __declspec(naked)
toggleCamHook(void)
{
	_asm{
		mov	thisptr, esi
		call	toggleCam

		mov	ecx, ds:0xC1703C	// Scene.camera
		push	0x52B893
		retn
	}
}

static void
processCam(void)
{
	CCamera *cam = (CCamera*)thisptr;
	if(cam->WorldViewerBeingUsed)
		cam->Cams[2].Process();
	
	InitExtraStuff();
}

static void __declspec(naked)
processCamHook(void)
{
	_asm{
		mov	thisptr, esi
		call	processCam

		movzx	ecx, [esi+0x59]	// active Cam
		imul	ecx, 238h
		push	0x52BA10
		retn
	}
}

static void
copyVectors(void)
{
	CCamera *cam = (CCamera*)thisptr;
	CVector *source = (CVector*)(baseptr - 0x80);
	CVector *front = (CVector*)(baseptr - 0x74);
	CVector *up = (CVector*)(baseptr - 0x8c);
	float *fov = (float*)(baseptr - 0x58);

	*source = cam->Cams[2].Source;
	*front = cam->Cams[2].Front;
	*up = cam->Cams[2].Up;
	*fov = cam->Cams[2].FOV;

	*(bool*)0xB70143 = 0;
}

static void __declspec(naked)
copyVectorsHook(void)
{
	_asm{
		mov	al,[esi + 0x56]	// WorldViewerBeingUsed
		test	al,al
		jz	nodebug

		mov	thisptr, esi
		lea	eax, [esp+0xb0]
		mov	baseptr, eax
		call	copyVectors
		push	0x52C53D
		retn

	nodebug:
		movzx	eax, [esi+0x59]	// active Cam
		imul	eax, 238h
		push	0x52C3E3
		retn
	}
}

WRAPPER void CIplStore__AddIplsNeededAtPosn(CVector *pos) { EAXJMP(0x4045B0); }

static
void copyToRw(void)
{
	CCamera *cam = (CCamera*)thisptr;

	CVector right;
	right = CrossProduct(cam->Cams[2].Up, cam->Cams[2].Front);
	cam->GetMatrix()->matrix.right = *(RwV3d*)&right;
	cam->GetMatrix()->matrix.up = *(RwV3d*)&cam->Cams[2].Front;
	cam->GetMatrix()->matrix.at = *(RwV3d*)&cam->Cams[2].Up;
	cam->GetMatrix()->matrix.pos = *(RwV3d*)&cam->Cams[2].Source;
	CDraw::SetFOV(cam->Cams[2].FOV);
	cam->m_vecGameCamPos = cam->Cams[2].Source;
	cam->CopyCameraMatrixToRWCam(false);
	CIplStore__AddIplsNeededAtPosn(&cam->m_vecGameCamPos);
}

static void __declspec(naked)
copyToRWHook(void)
{
	_asm{
		mov	al,[esi + 0x56]	// WorldViewerBeingUsed
		test	al,al
		jz	nodebug

		pop	ebp	// remove argument to thiscall
		mov	thisptr, esi
		call	copyToRw
		mov	ecx, esi
		push	0x52C9B6
		retn

	nodebug:
		mov	eax,0x50AFA0
		call	eax
		push	0x52C990
		retn
	}
}


static StaticPatcher	Patcher([](){
	static const float	fRegularFov = CAMERA_FOV_REGULAR;
	Memory::Patch<const void*>(0x52C9DB, &fRegularFov);
	Memory::Patch<const void*>(0x522F3A, &fRegularFov);
	Memory::Patch<const void*>(0x522F5D, &fRegularFov);
	Memory::Patch<const void*>(0x521CB7, &fRegularFov);
	Memory::Patch<const void*>(0x521CC4, &fRegularFov);
	Memory::Patch<const void*>(0x521CD0, &fRegularFov);
	Memory::Patch<const void*>(0x521D43, &fRegularFov);
	Memory::Patch<const void*>(0x521D50, &fRegularFov);
	Memory::Patch<const void*>(0x521D5C, &fRegularFov);
	Memory::Patch<const void*>(0x524B76, &fRegularFov);
	Memory::Patch<const void*>(0x524B9A, &fRegularFov);
	Memory::Patch<const void*>(0x524BA2, &fRegularFov);
	Memory::Patch<const void*>(0x524BD3, &fRegularFov);

	static const float	fIncarMaxFov = CAMERA_FOV_INCAR_MAX;
	Memory::Patch<const void*>(0x524BB4, &fIncarMaxFov);

	Memory::Patch<float>(0x522F7A, CAMERA_FOV_REGULAR);
	Memory::Patch<float>(0x521632, CAMERA_FOV_REGULAR);
	Memory::Patch<float>(0x524BE4, CAMERA_FOV_REGULAR);

	Memory::Patch<float>(0x524BC5, CAMERA_FOV_INCAR_MAX);


	// Patch in Debug camera
	Memory::InjectHook(0x52B88D, toggleCamHook, PATCH_JUMP);
	Memory::InjectHook(0x52BA06, processCamHook, PATCH_JUMP);
	Memory::InjectHook(0x52C3D9, copyVectorsHook, PATCH_JUMP);
	Memory::InjectHook(0x52C98B, copyToRWHook, PATCH_JUMP);
	Memory::InjectHook(0x527C5D, switchDefaultHook, PATCH_JUMP);

	if (DebugMenuLoad()) {
		DebugMenuEntry *e;
		static const char *controlStr[] = { "Camera", "Player" };
		DebugMenuAddCmd("Debug", "Toggle Debug Camera", []() { toggleDebugCam = 1; });
		e = DebugMenuAddVar("Debug", "Debug Camera Control", &controlMode, NULL, 1, CONTROL_CAMERA, CONTROL_PLAYER, controlStr);
		DebugMenuEntrySetWrap(e, true);
		DebugMenuAddVar("Debug", "Debug Camera FOV", &gFOV, NULL, 1.0f, 5.0f, 180.0f);
	}

});