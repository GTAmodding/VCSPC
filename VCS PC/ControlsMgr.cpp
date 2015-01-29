#include "StdAfx.h"
#include "ControlsMgr.h"

#include "Pad.h"

WRAPPER void CControllerConfigManager::SaveSettings(FILE* hFile) { WRAPARG(hFile); EAXJMP(0x52D200); }
WRAPPER bool CControllerConfigManager::LoadSettings(FILE* hFile) { WRAPARG(hFile); EAXJMP(0x530530); }
WRAPPER bool CControllerConfigManager::GetIsKeyboardKeyJustDown(RsKeyCodes eKey) { WRAPARG(eKey); EAXJMP(0x52E450); }
WRAPPER bool CControllerConfigManager::GetIsKeyboardKeyDown(RsKeyCodes eKey) { WRAPARG(eKey); EAXJMP(0x52DDB0); }
WRAPPER void CControllerConfigManager::ReinitControls() { EAXJMP(0x531F20); }

void CControllerConfigManager::SaveToFile()
{
	if ( FILE* hFile = CFileMgr::OpenFile("controls.set", "wb") )
	{
		const DWORD		dwFileVer = CONTROLS_FILE_VERSION;

		CFileMgr::Write(hFile, reinterpret_cast<const char*>(&dwFileVer), sizeof(dwFileVer));
		SaveSettings(hFile);

		CFileMgr::CloseFile(hFile);
	}
}

bool CControllerConfigManager::LoadFromFile()
{
	bool	bResult = false;

	if ( FILE* hFile = CFileMgr::OpenFile("controls.set", "rb") )
	{
		bResult = LoadSettings(hFile);

		CFileMgr::CloseFile(hFile);
	}
	return bResult;
}


// RawInput mouse handler
void RegisterRawInputMouse()
{
    RAWINPUTDEVICE Rid[1];
    Rid[0].usUsagePage = 1; 
    Rid[0].usUsage = 2; 
    Rid[0].dwFlags = RIDEV_INPUTSINK;   
	Rid[0].hwndTarget = RsGlobal.ps->window;

    RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));
}

void RegisterMouseMovement(RAWINPUT* raw)
{
	// Interpret data
	CMouseControllerState&	StateBuf = CPad::GetMouseStateBuffer();
	
	// Movement
	StateBuf.X = static_cast<float>(raw->data.mouse.lLastX);
	StateBuf.Y = static_cast<float>(raw->data.mouse.lLastY);

	// LMB
	if ( !StateBuf.lmb )
		StateBuf.lmb = (raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) != false;
	else
		StateBuf.lmb = (raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP) == false;

	// RMB
	if ( !StateBuf.rmb )
		StateBuf.rmb = (raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) != false;
	else
		StateBuf.rmb = (raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP) == false;

	// MMB
	if ( !StateBuf.mmb )
		StateBuf.mmb = (raw->data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN) != false;
	else
		StateBuf.mmb = (raw->data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP) == false;

	// 4th button
	if ( !StateBuf.bmx1 )
		StateBuf.bmx1 = (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN) != false;
	else
		StateBuf.bmx1 = (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_UP) == false;

	// 5th button
	if ( !StateBuf.bmx2 )
		StateBuf.bmx2 = (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN) != false;
	else
		StateBuf.bmx2 = (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_UP) == false;

	// Scroll
	if ( raw->data.mouse.usButtonFlags & RI_MOUSE_WHEEL )
	{
		StateBuf.Z = static_cast<signed short>(raw->data.mouse.usButtonData);
		if ( StateBuf.Z < 0.0 )
			StateBuf.wheelDown = true;
		else if ( StateBuf.Z > 0.0 )
			StateBuf.wheelUp = true;
	}
}

static StaticPatcher	Patcher([](){ 
						Memory::InjectHook(0x74880B, RegisterRawInputMouse);
						Memory::Patch<BYTE>(0x7469A0, 0xC3);
						Memory::Patch<DWORD>(0x53F2D0, 0x900004C2);
									});