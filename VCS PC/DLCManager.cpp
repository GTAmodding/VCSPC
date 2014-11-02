#include "StdAfx.h"
#include "DLCManager.h"

#include "Frontend.h"
#include "Font.h"

CExpansionPack*			CDLCManager::m_pDLC[NUM_DLC_PACKS];
int						CDLCManager::m_nActiveDLCIndex[NUM_DLC_PACKS];

#ifdef DEVBUILD
bool					CDLCManager::m_bDebugOverride[NUM_DLC_PACKS];
#endif

IDLCClient001*			CDLCManager::m_pDLCCLient;
bool					CDLCManager::m_bContactingTheSite;

void CDLCManager::Initialise()
{
	m_pDLC[DLC_HALLOWEEN] = new CTimedExpansionPack("HALLOWEEN", CDate(30, 10), CDate(3, 11));
	m_pDLC[DLC_2DFX] = new CExpansionPack("2DFX");
	m_pDLC[DLC_THANKYOU] = new CPaidExpansionPack("THANKYOU");

	for ( int i = 0; i < NUM_DLC_PACKS; i++ )
		m_nActiveDLCIndex[i] = -1;
}

void CDLCManager::InitialiseWithUpdater()
{
	int		nCurrentArrayIndex = 0;

	for ( int i = 0; i < NUM_DLC_PACKS; ++i )
	{
		// Depends on current date
		bool		bDateInBounds = m_pDLC[i]->DateValid(GetCurrentDate());
		if ( !CUpdateManager::GetDLCStatus(m_pDLC[i]->GetName(), m_pDLC[i]->IsTimed()) )
			m_pDLC[i]->Activate(false);
		else
#ifdef DEVBUILD
			m_pDLC[i]->Activate(bDateInBounds || m_bDebugOverride[i]);
#else
			m_pDLC[i]->Activate(bDateInBounds);
#endif

		// Add this DLC to list if date is within bounds
#ifdef DEVBUILD
		if ( !m_pDLC[i]->IsShady() && (bDateInBounds || m_bDebugOverride[i]) )
#else
		if ( !m_pDLC[i]->IsShady() && bDateInBounds )
#endif
			m_nActiveDLCIndex[nCurrentArrayIndex++] = i;
	}

	m_pDLCCLient = CUpdateManager::GetMeDLCClient();
	m_pDLCCLient->RegisterOnFinishedRequestCallback(OnFinishedRequest);
}

void CDLCManager::Terminate()
{
	if ( m_pDLC[DLC_2DFX]->IsEnabled() )
		CProject2dfx::Shutdown();

	for ( int i = 0; i < NUM_DLC_PACKS; i++ )
		delete m_pDLC[i];
}

void CDLCManager::LoadLevelFiles()
{
	InitialiseWithUpdater();

	for ( int i = 0; i < NUM_DLC_PACKS; ++i )
	{
		if ( m_pDLC[i]->CheckInstallState(i) )
		{
			if ( m_pDLC[i]->IsEnabled() )
			{
				char		cLevelsPath[MAX_PATH];
				char		cDLCName[32];
				_snprintf(cLevelsPath, sizeof(cLevelsPath), "dlc\\dlc%d\\content.dat", i);

				if ( i == DLC_THANKYOU )
					CFileLoader::SetEncryptionType(1);
				
				if ( !CFileLoader::ParseLevelFile(cLevelsPath, cDLCName) )
					m_pDLC[i]->Activate(false);
				else
				{
					if ( _strnicmp(cDLCName, m_pDLC[i]->GetName(), sizeof(cDLCName)) )
						m_pDLC[i]->Activate(false);
				}

				if ( i == DLC_THANKYOU )
					CFileLoader::SetEncryptionType(0);
			}
		}
	}

	// Old call
	//CFileLoader::ParseLevelFile("DATA\\DEFAULT.DAT");

	// Project 2dfx patches
	if ( m_pDLC[DLC_2DFX]->IsEnabled() )
		CProject2dfx::Inject();
}

void CDLCManager::HandleButtonClick(int nMenuEntry)
{
	int				nDLC = m_nActiveDLCIndex[nMenuEntry-2];
	const char*		pDLCName = m_pDLC[nDLC]->GetName();

	// Toggle DLC on/off
	if ( m_pDLC[nDLC]->IsInstalled() )
		CUpdateManager::SetDLCStatus(pDLCName, CUpdateManager::GetDLCStatus(pDLCName, false) == false);
	else
		FrontEndMenuManager.SwitchToNewScreen(48);
}

void CDLCManager::ActivateSerial(const std::string* strSerial)
{
	// Make a serial code out of the passed array
	std::string			strFullSerial;
	
	for ( int i = 0; i < SERIAL_CODES_LENGTH; i++ )
		strFullSerial += strSerial[i];

	m_pDLCCLient->SendSerialCodeRequest(strFullSerial);
	m_bContactingTheSite = true;
}

void CDLCManager::Process()
{

}

void CDLCManager::OnFinishedRequest(const std::string& strOut)
{
	if ( strOut.find("fail") == 0 )
	{
		FrontEndMenuManager.RegisterDLCMessage("FEE_WRK");
		FrontEndMenuManager.SwitchToScreenAfterMessage(45);
	}
	else if ( strOut.find("unlocked") == 0 )
	{
		std::string		strDLCName = strOut.substr(9);

		// Find DLC by name
		for ( int i = 0; i < NUM_DLC_PACKS; i++ )
		{
			if ( strDLCName == m_pDLC[i]->GetName() )
			{
				if ( !m_pDLC[i]->IsActive() )
				{
					static char		cUnlockGXTName[8];

					_snprintf(cUnlockGXTName, sizeof(cUnlockGXTName), "FEE_U%02d", i);
					CUpdateManager::SetDLCStatus(m_pDLC[i]->GetName(), true);
					FrontEndMenuManager.RegisterDLCMessage(cUnlockGXTName);
					FrontEndMenuManager.SwitchToScreenAfterMessage(44);

					CUpdateManager::ForceUpdate();
				}
				else
				{
					FrontEndMenuManager.RegisterDLCMessage("FEE_ALA");
					FrontEndMenuManager.SwitchToScreenAfterMessage(45);
				}
				break;
			}
		}
	}
	else
	{
		FrontEndMenuManager.RegisterDLCMessage("FEE_ERR");
		FrontEndMenuManager.SwitchToScreenAfterMessage(45);
	}

	m_bContactingTheSite = false;
}