#include "StdAfx.h"

CExpansionPack*			CDLCManager::m_pDLC[NUM_DLC_PACKS];
int						CDLCManager::m_nActiveDLCIndex[NUM_DLC_PACKS];

#ifdef _DEBUG
bool					CDLCManager::m_bDebugOverride[NUM_DLC_PACKS];
#endif

void CDLCManager::Initialise()
{
	m_pDLC[DLC_HALLOWEEN] = new CTimedExpansionPack("HALLOWEEN", CDate(30, 10), CDate(3, 11));

	for ( int i = 0; i < NUM_DLC_PACKS; i++ )
		m_nActiveDLCIndex[i] = -1;
}

void CDLCManager::InitialiseWithUpdater()
{
	int		nCurrentArrayIndex = 0;
	if ( m_pDLC[DLC_HALLOWEEN] )
	{
		// Depends on current date
		bool		bDateInBounds = m_pDLC[DLC_HALLOWEEN]->DateValid(GetCurrentDate());
		if ( !CUpdateManager::GetDLCStatus(m_pDLC[DLC_HALLOWEEN]->GetName(), true) )
			m_pDLC[DLC_HALLOWEEN]->Activate(false);
		else
#ifdef _DEBUG
			m_pDLC[DLC_HALLOWEEN]->Activate(bDateInBounds || m_bDebugOverride[DLC_HALLOWEEN]);
#else
			m_pDLC[DLC_HALLOWEEN]->Activate(bDateInBounds);
#endif

		// Add this DLC to list if date is within bounds
#ifdef _DEBUG
		if ( bDateInBounds || m_bDebugOverride[DLC_HALLOWEEN] )
#else
		if ( bDateInBounds )
#endif
			m_nActiveDLCIndex[nCurrentArrayIndex++] = DLC_HALLOWEEN;
	}
}

void CDLCManager::Terminate()
{
	if ( m_pDLC[DLC_HALLOWEEN] )
		delete m_pDLC[DLC_HALLOWEEN];
}

void CDLCManager::LoadLevelFiles()
{
	InitialiseWithUpdater();

	for ( int i = 0; i < NUM_DLC_PACKS; ++i )
	{
		if ( m_pDLC[i]->IsEnabled() )
		{
			char		cLevelsPath[MAX_PATH];
			char		cDLCName[32];
			_snprintf(cLevelsPath, MAX_PATH, "dlc\\dlc%d\\content.dat", i);

			if ( !CFileLoader::ParseLevelFile(cLevelsPath, cDLCName) )
				m_pDLC[i]->Activate(false);

			if ( _strnicmp(cDLCName, m_pDLC[i]->GetName(), 32) )
				m_pDLC[i]->Activate(false);
		}
	}

	// Old call
	//CFileLoader::ParseLevelFile("DATA\\DEFAULT.DAT");
}

void CDLCManager::HandleButtonClick(int nMenuEntry)
{
	int				nDLC = m_nActiveDLCIndex[nMenuEntry-2];
	const char*		pDLCName = m_pDLC[nDLC]->GetName();

	// Toggle DLC on/off
	CUpdateManager::SetDLCStatus(pDLCName, CUpdateManager::GetDLCStatus(pDLCName, false) == false);
}