#include "StdAfx.h"

CExpansionPack*			CDLCManager::m_pDLC[NUM_DLC_PACKS];
int						CDLCManager::m_nActiveDLCIndex[NUM_DLC_PACKS];

#ifdef DEVBUILD
bool					CDLCManager::m_bDebugOverride[NUM_DLC_PACKS];
#endif

void CDLCManager::Initialise()
{
	m_pDLC[DLC_HALLOWEEN] = new CTimedExpansionPack("HALLOWEEN", CDate(30, 10), CDate(3, 11));
	m_pDLC[DLC_2DFX] = new CExpansionPack("2DFX");

	for ( int i = 0; i < NUM_DLC_PACKS; i++ )
		m_nActiveDLCIndex[i] = -1;
}

void CDLCManager::InitialiseWithUpdater()
{
	int		nCurrentArrayIndex = 0;

	//if ( m_pDLC[DLC_HALLOWEEN] )
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
		if ( bDateInBounds || m_bDebugOverride[i] )
#else
		if ( bDateInBounds )
#endif
			m_nActiveDLCIndex[nCurrentArrayIndex++] = i;
	}
}

void CDLCManager::Terminate()
{
	if ( m_pDLC[DLC_2DFX]->IsEnabled() )
		CProject2dfx::Shutdown();

	for ( int i = 0; i < NUM_DLC_PACKS; i++ )
	{
		if ( m_pDLC[i] )
			delete m_pDLC[i];
	}
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

				if ( !CFileLoader::ParseLevelFile(cLevelsPath, cDLCName) )
					m_pDLC[i]->Activate(false);
				else
				{
					if ( _strnicmp(cDLCName, m_pDLC[i]->GetName(), sizeof(cDLCName)) )
						m_pDLC[i]->Activate(false);
				}
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