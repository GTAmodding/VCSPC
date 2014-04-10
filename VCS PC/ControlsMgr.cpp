#include "StdAfx.h"
#include "ControlsMgr.h"

WRAPPER void CControllerConfigManager::SaveSettings(FILE* hFile) { WRAPARG(hFile); EAXJMP(0x52D200); }
WRAPPER bool CControllerConfigManager::LoadSettings(FILE* hFile) { WRAPARG(hFile); EAXJMP(0x530530); }

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

void CControllerConfigManager::LoadFromFile()
{
	if ( FILE* hFile = CFileMgr::OpenFile("controls.set", "rb") )
	{
		LoadSettings(hFile);

		CFileMgr::CloseFile(hFile);
	}
}