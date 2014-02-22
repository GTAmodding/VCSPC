#ifndef __DLCMGR
#define __DLCMGR

#include "..\common\DLCShared.h"
#include "UpdateManager.h"
#include "General.h"

// DLC headers
#include "Project2dfx.h"

class CExpansionPack
{
private:
	bool					m_bActive, m_bInstalled;
	const char*				m_pInternalName;

public:
	virtual bool			DateValid(const CDate& CurrentDate)
			{ UNREFERENCED_PARAMETER(CurrentDate); return true; }
	virtual bool			IsTimed()
			{ return false; }

	inline void				Activate(bool bState)
		{ LogToFile("Pack %s is %s", m_pInternalName, bState ? "ACTIVE" : "INACTIVE"); m_bActive = bState; }
	inline bool				CheckInstallState(int nIndex) {
		char		cLevelsPath[MAX_PATH];
		_snprintf(cLevelsPath, sizeof(cLevelsPath), "dlc\\dlc%d\\content.dat", nIndex);
		return m_bInstalled = GetFileAttributes(cLevelsPath) != INVALID_FILE_ATTRIBUTES; }

	inline bool				IsEnabled()
			{ return m_bActive; }
	inline bool				IsInstalled()
			{ return m_bInstalled; }
	inline bool				IsActive()
			{ return CUpdateManager::GetDLCStatus(m_pInternalName, false); }
	inline const char*		GetName()
			{ return m_pInternalName; }

	CExpansionPack(const char* pName)
		: m_bActive(false), m_bInstalled(false), m_pInternalName(pName)
	{
	}
};

class CTimedExpansionPack : public CExpansionPack
{
private:
	const CDate				m_launchDate, m_expirationDate;

public:
	virtual bool			DateValid(const CDate& CurrentDate) override
			{ return m_launchDate <= CurrentDate && m_expirationDate > CurrentDate; }
	virtual bool			IsTimed() override
			{ return true; }

	CTimedExpansionPack(const char* pName, const CDate& BeginDate, const CDate& EndDate)
		: CExpansionPack(pName), m_launchDate(BeginDate), m_expirationDate(EndDate)
	{}

	// Just to make the compiler shut the fuck up
	CTimedExpansionPack& operator=(const CTimedExpansionPack&) {}
};

class CDLCManager
{
private:
	// DLCs array
	static CExpansionPack*			m_pDLC[NUM_DLC_PACKS];
	static int						m_nActiveDLCIndex[NUM_DLC_PACKS];

#ifdef DEVBUILD
	static bool						m_bDebugOverride[NUM_DLC_PACKS];
#endif

public:
	static inline CExpansionPack*	GetDLC(int nDLC)
			{ return m_pDLC[nDLC]; }
	static inline int				GetActiveDLCWithIndex(int nIndex)
			{ return NUM_DLC_PACKS > nIndex ? m_nActiveDLCIndex[nIndex] : -1; }
	static inline bool				AnyDLCsAvailable()
			{ return GetActiveDLCWithIndex(0) != -1; }

	static void						Initialise();
	static void						InitialiseWithUpdater();
	static void						Terminate();
	static void						LoadLevelFiles();
	static void						HandleButtonClick(int nMenuEntry);

#ifdef DEVBUILD
	static inline void				ToggleDebugOverride(eExpansionPack eDLC)
			{ m_bDebugOverride[eDLC] = true; }
#endif
};

#endif