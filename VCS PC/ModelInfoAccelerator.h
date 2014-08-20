#ifndef __MODELACCEL
#define __MODELACCEL

#include "ModelInfo.h"

// Do we want modelaccel to compile?
#define USE_MODELACCEL

//#define MINFO_ALLOCATION_SIZE		20550		// Why the extra 550?
#define MINFO_ALLOCATION_SIZE		20000

class CModelInfoAccelerator
{
private:
	unsigned short*		m_pMemory;
	unsigned short		m_wCurrentModelInfo;
	bool				m_bEnded;
	bool				m_bFileExists;
	char				m_aFileName[16];

private:
	void				Init()
	{
		m_pMemory = nullptr;
		m_wCurrentModelInfo = 0;
		m_bEnded = false;
		m_bFileExists = false;
		m_aFileName[0] = '\0';
	}

	bool				GetModelInfoIdFile();
	void				EndOfLoadPhase();
	CBaseModelInfo*		GetModelInfo(int* pModelID, const char* pName);

	unsigned short		GetNextModelInfoId()
	{
		assert(m_wCurrentModelInfo < MINFO_ALLOCATION_SIZE);
		return m_pMemory[m_wCurrentModelInfo++];
	}

	void				AddModelInfoId(unsigned short nModelID)
	{
		assert(m_wCurrentModelInfo < MINFO_ALLOCATION_SIZE);
		m_pMemory[m_wCurrentModelInfo++] = nModelID;
	}

public:
	CModelInfoAccelerator()
	{
		Init();
	}

	void				Begin(const char* pFileName)
	{
		strncpy(m_aFileName, pFileName, 16);
		GetModelInfoIdFile();
	}

	void				End()
	{
		if ( !m_bEnded )
		{
			EndOfLoadPhase();
			m_bEnded = true;
		}
	}

	CBaseModelInfo*		GetEntry(const char* pName, int* pModelID);
};

extern CModelInfoAccelerator	gModelAccel;

#endif