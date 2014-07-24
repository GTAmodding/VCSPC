#include "StdAfx.h"
#include "UpdateManager.h"

#include "Script.h"
#include "Frontend.h"
#include "Text.h"
#include "Timer.h"
#include "Font.h"
#include "Messages.h"

IUpdaterClient001*	CUpdateManager::pUptModuleInterface;
HMODULE				CUpdateManager::hUptModuleLibrary;
unsigned long		CUpdateManager::nActiveMessages;
long				CUpdateManager::nInterfaceStatus;
long				CUpdateManager::nLastInterfaceStatus;
long				CUpdateManager::nDrawingState;
bool				CUpdateManager::bSeenUpdaterScreenYet;
unsigned char		CUpdateManager::bForceUpdate;
char*				CUpdateManager::pMessages[NUM_MESSAGES_PER_UPT_SCREEN];

bool				CUpdateManager::bAutoInstallUpdates;
unsigned char		CUpdateManager::bCheckingPeriod;

#ifdef DEVBUILD
bool				CUpdateManager::bDisableAutoCheck = false;
#endif


void CUpdateManager::Init()
{
	hUptModuleLibrary = LoadLibrary("updater\\UptModule");
	if ( hUptModuleLibrary )
	{
		GetUptModuleFn interfaceFactory = (GetUptModuleFn)GetProcAddress(hUptModuleLibrary, "CreateUpdaterInstance");

		if ( interfaceFactory )
		{
			pUptModuleInterface = (IUpdaterClient001*)interfaceFactory(UPDATER_INTERFACE_CLIENT001);

			// Needs to be called VERY early
			CUpdateManager::SetThisDLCAsVerySpecial(CDLCManager::GetDLC(DLC_THANKYOU)->GetName());

			pUptModuleInterface->Initialize();
			pUptModuleInterface->RegisterMessageCallback(EchoMessage);

			for ( int i = 0; i < NUM_MESSAGES_PER_UPT_SCREEN; ++i )
			{
				pMessages[i] = new char[MAX_PATH];
				pMessages[i][0] = '\0';
			}

			bSeenUpdaterScreenYet = false;
			bForceUpdate = false;
			nActiveMessages = 0;
			nInterfaceStatus = nLastInterfaceStatus = UPTMODULESTATE_IDLE;

			// Read settings
			UpdaterClientData001		UpdaterSetData;

			if ( pUptModuleInterface->ReturnSettings(UPDATER_SETTINGS_VERSION, &UpdaterSetData) )
			{	
				bAutoInstallUpdates = UpdaterSetData.bAutoInstall;
				bCheckingPeriod = UpdaterSetData.bCheckPeriod;
			}
		}
		else
			pUptModuleInterface = nullptr;
	}
	else
		pUptModuleInterface = nullptr;
}

void CUpdateManager::Terminate()
{
	if ( hUptModuleLibrary )
	{
		FreeLibrary(hUptModuleLibrary);
		hUptModuleLibrary = nullptr;
		pUptModuleInterface = nullptr;

		for ( int i = 0; i < NUM_MESSAGES_PER_UPT_SCREEN; ++i )
			delete[] pMessages[i];
	}
}

void CUpdateManager::Process()
{
	if ( pUptModuleInterface )
		nInterfaceStatus = pUptModuleInterface->Process();

	//CDLCManager::Process();

	if ( bForceUpdate == 1 )
	{
		pUptModuleInterface->PerformUpdateCheck();
		++bForceUpdate;
	}

	if ( nLastInterfaceStatus != nInterfaceStatus )
	{
		switch ( nInterfaceStatus )
		{
		case UPTMODULESTATE_IDLE:
			if ( nLastInterfaceStatus == UPTMODULESTATE_CHECKING )
				bSeenUpdaterScreenYet = false;
			break;

		case UPTMODULESTATE_NEW_UPDATES:
			if ( bForceUpdate == 2 )
			{
				pUptModuleInterface->PerformFilesDownload();
				++bForceUpdate;
			}
			bSeenUpdaterScreenYet = false;
			break;

		case UPTMODULESTATE_ALL_READY:
			EchoMessage(TheText.Get("UPT_INS"));
			bSeenUpdaterScreenYet = false;
			break;
		}
		nLastInterfaceStatus = nInterfaceStatus;
	}

#ifdef DEVBUILD
	static bool				bCheckedThisSession = bDisableAutoCheck;
#else
	static bool				bCheckedThisSession = false;
#endif

	if ( !bCheckedThisSession )
	{
		if ( pUptModuleInterface->TimeToUpdate(GetTimeByOption()) )
		{
			if ( nInterfaceStatus == UPTMODULESTATE_IDLE )
			{
				pUptModuleInterface->PerformUpdateCheck();
				if ( bAutoInstallUpdates )
					bForceUpdate = 2;
			}
		}

		bCheckedThisSession = true;
	}


	/*static unsigned long	nTempCtr = 0;
	static bool				bTemp = false;

#ifdef DEVBUILD
	if ( bDisableAutoCheck )
		bTemp = true;
#endif

	if ( !bTemp )
	{
		if ( nInterfaceStatus != UPTMODULESTATE_IDLE )
			bTemp = true;
		else
		{
			if ( !FrontEndMenuManager.IsActive() && !CTheScripts::IsPlayerOnAMission() )
			{
				++nTempCtr;

				if ( nTempCtr > 500 )
				{
					if ( nInterfaceStatus == UPTMODULESTATE_IDLE )
						pUptModuleInterface->PerformUpdateCheck();
					bTemp = true;
				}
			}
		}
	}*/
}

void CUpdateManager::Display()
{
	switch ( nInterfaceStatus )
	{
	case UPTMODULESTATE_CHECKING:
		{		
			nDrawingState = 1;
			break;
		}
	case UPTMODULESTATE_IDLE:
	case UPTMODULESTATE_NEW_UPDATES:
		{
			if ( bSeenUpdaterScreenYet || (nInterfaceStatus == UPTMODULESTATE_IDLE && !nDrawingState) )
			{
				nDrawingState = 0;
				break;
			}
			if ( UpdatesAvailable() )
			{
				nDrawingState = 2;
			}
			else
			{
				nDrawingState = 3;
			}
			break;
		}
	case UPTMODULESTATE_DOWNLOADING:
		{
			nDrawingState = 4;
			break;
		}
	case UPTMODULESTATE_ALL_READY:
		{
			if ( bSeenUpdaterScreenYet )
			{
				nDrawingState = 0;
				break;
			}
			nDrawingState = 5;
			break;
		}
	default:
		{
			nDrawingState = 0;
			break;
		}
	}

	static unsigned long	nTextAlpha = 255;
	static signed long		nTextShowingTime = 0;
	static signed long		nTextFadingTime = 0;
	static long				nTextShowStage = 2;
	static long				nLastDrawingState = 0;

	if ( nLastDrawingState != nDrawingState )
	{
		switch ( nDrawingState )
		{
		case 1:
			nTextShowStage = 1;
			break;
		case 2:
		case 4:
			nTextShowStage = 4;
			break;
		case 3:
		case 5:
			nTextShowingTime = 0;
			nTextShowStage = 2;
			break;
		}
		nLastDrawingState = nDrawingState;
	}

	// TODO: Get rid of it when timers are rewritten
	static DWORD	dwPerformanceStepCounter = 0;

	float fStep = 0.05f * (CTimer::m_snTimeInMillisecondsPauseMode - dwPerformanceStepCounter);

	dwPerformanceStepCounter = CTimer::m_snTimeInMillisecondsPauseMode;
	/*if ( FrontEndMenuManager.IsActive() )
		fStep = 1000.0 / (*currentFPS);
	else
		fStep = CTimer::ms_fTimeStep;*/


	switch ( nTextShowStage )
	{
	case 1:
		nTextFadingTime += static_cast<long>(fStep * 20.0f);
		if ( nTextFadingTime > 1000 )
		{
			nTextFadingTime = 1000;
			nTextShowStage = 5;
		}
		nTextAlpha = static_cast<unsigned long>(nTextFadingTime * 0.255f);
		break;
	case 2:
		nTextShowingTime += static_cast<long>(fStep * 20.0f);
		if ( nTextShowingTime > 10000 )
		{
			nTextShowStage = 3;
			nTextFadingTime = 1000;
		}
	case 4:
		nTextAlpha = 255;
		break;
	case 3:
	case 5:
		nTextFadingTime -= static_cast<long>(fStep * 20.0f);
		if ( nTextFadingTime < 0 )
		{
			nTextFadingTime = 0;
			if ( nTextShowStage == 5 )
				nTextShowStage = 1;
			else
			{
				nDrawingState = 0;
				nTextShowStage = 0;
			}
		}
		nTextAlpha = static_cast<long>(nTextFadingTime * 0.255f);
		break;
	}

	if ( nDrawingState )
	{
		CFont::SetBackground(0, 0);
		CFont::SetProportional(true);
		CFont::SetFontStyle(FONT_Eurostile);
		CFont::SetOrientation(ALIGN_Right);
		CFont::SetEdge(1);
		CFont::SetScale(_width(0.35f), _height(0.65f));
		CFont::SetDropColor(CRGBA(0, 0, 0, static_cast<unsigned char>(nTextAlpha)));
		if ( nDrawingState == 2 || nDrawingState == 5 )
			CFont::SetColor(CRGBA(MENU_UPDATES_R, MENU_UPDATES_G, MENU_UPDATES_B, static_cast<unsigned char>(nTextAlpha)));
		else
			CFont::SetColor(CRGBA(255, 255, 255, static_cast<unsigned char>(nTextAlpha)));

		switch ( nInterfaceStatus )
		{
		case UPTMODULESTATE_IDLE:
			CFont::PrintString(_x(15.0f), _y(3.5f), TheText.Get(pUptModuleInterface->UpdateServiceOn() ? "FEU_NOU" : "FEU_USA"));
			break;

		case UPTMODULESTATE_CHECKING:
			CFont::PrintString(_x(15.0f), _y(3.5f), TheText.Get("FEU_CHK"));
			break;
		case UPTMODULESTATE_NEW_UPDATES:
			CFont::PrintString(_x(15.0f), _y(3.5f), TheText.Get("FEU_NEW"));
			break;

		case UPTMODULESTATE_DOWNLOADING:
			CMessages::InsertNumberInString(TheText.Get("FEU_DLU"), static_cast<signed long>(pUptModuleInterface->GetProgress()), -1, -1, -1, -1, -1, gString);
			CFont::PrintString(_x(15.0f), _y(3.5f), gString);
			break;

		case UPTMODULESTATE_ALL_READY:
			CFont::PrintString(_x(15.0f), _y(3.5f), TheText.Get("FEU_RDY"));
			break;
		}
	}


}

void CUpdateManager::HandleButtonClick()
{
	switch ( nInterfaceStatus )
	{
	case UPTMODULESTATE_IDLE:
		pUptModuleInterface->PerformUpdateCheck();
		return;
	case UPTMODULESTATE_NEW_UPDATES:
		pUptModuleInterface->PerformFilesDownload();
		return;
	default:
		return;
	}
}

void CUpdateManager::InstallIfNeeded()
{
	if ( nInterfaceStatus == UPTMODULESTATE_ALL_READY )
		pUptModuleInterface->LaunchInstaller();

	// Can be terminated now
	//CUpdateManager::Terminate();

	// DIRTY WORKAROUND #2
	//ExitProcess(0);
}

float CUpdateManager::GetDownloadProgress()
{
	if ( nInterfaceStatus == UPTMODULESTATE_DOWNLOADING )
		return pUptModuleInterface->GetProgress();

	if ( nInterfaceStatus == UPTMODULESTATE_ALL_READY )
		return 100.0;

	return 0.0;
}

const char* CUpdateManager::GetGXTEntryForButton()
{
	switch ( nInterfaceStatus )
	{
	case UPTMODULESTATE_CHECKING:
		CFont::SetColor(CRGBA(MENU_LOCKED_R, MENU_LOCKED_G, MENU_LOCKED_B));
		return TheText.Get("FEU_UCK");
	case UPTMODULESTATE_NEW_UPDATES:
		return TheText.Get("FEU_UPN");
	case UPTMODULESTATE_DOWNLOADING:
		CFont::SetColor(CRGBA(MENU_LOCKED_R, MENU_LOCKED_G, MENU_LOCKED_B));
		return TheText.Get("FEU_UDL");
	case UPTMODULESTATE_ALL_READY:
		CFont::SetColor(CRGBA(MENU_LOCKED_R, MENU_LOCKED_G, MENU_LOCKED_B));
	default:
		return TheText.Get("FEU_UPC");
	}
}

const char* CUpdateManager::FetchMessage(long nIndex)
{
	if ( pUptModuleInterface )
		return pMessages[nIndex];

	return NULL;
}

bool CUpdateManager::UpdatesAvailable()
{
	if ( pUptModuleInterface )
		return pUptModuleInterface->UpdatesAvailable();

	return false;
}

void CUpdateManager::EchoMessage(const char* pMessage)
{
	if ( nActiveMessages == NUM_MESSAGES_PER_UPT_SCREEN )
	{
		for ( int i = 1; i < NUM_MESSAGES_PER_UPT_SCREEN; ++i )
			strncpy(pMessages[i-1], pMessages[i], MAX_PATH);
	}
	else
		++nActiveMessages;

	strncpy(pMessages[nActiveMessages-1], pMessage, MAX_PATH);
}

void CALLBACK CUpdateManager::EchoMessage(const wchar_t* pMessage)
{
	if ( nActiveMessages == NUM_MESSAGES_PER_UPT_SCREEN )
	{
		for ( int i = 1; i < NUM_MESSAGES_PER_UPT_SCREEN; ++i )
			strncpy(pMessages[i-1], pMessages[i], MAX_PATH);
	}
	else
		++nActiveMessages;

	wcstombs(pMessages[nActiveMessages-1], pMessage, MAX_PATH);
}

IDLCClient001* CUpdateManager::GetMeDLCClient()
{
	GetDLCFn interfaceFactory = (GetDLCFn)GetProcAddress(hUptModuleLibrary, "CreateUpdaterInstance");

	return interfaceFactory ? (IDLCClient001*)interfaceFactory(DLC_INTERFACE_CLIENT001) : nullptr;
}

void CUpdateManager::SaveSettings()
{
	UpdaterClientData001		SettingsStruct;

	SettingsStruct.bAutoInstall = bAutoInstallUpdates;
	SettingsStruct.bCheckPeriod = bCheckingPeriod;
	SettingsStruct.nTimeLastChecked = 0;

	pUptModuleInterface->SaveSettings(UPDATER_SETTINGS_VERSION, &SettingsStruct);
}

time_t CUpdateManager::GetTimeByOption()
{
	switch ( bCheckingPeriod )
	{
	case 0:
		// 1 day
		return 60*60*24;
	case 1:
		// 3 days
		return 60*60*24*3;
	case 2:
		// 1 week
		return 60*60*24*7;
	case 3:
		// 2 weeks
		return 60*60*24*14;
	}
	return 0;
}