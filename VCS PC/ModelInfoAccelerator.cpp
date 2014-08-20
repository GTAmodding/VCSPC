#include "StdAfx.h"
#include "ModelInfoAccelerator.h"

#ifdef USE_MODELACCEL

CModelInfoAccelerator	gModelAccel;

#endif

CBaseModelInfo* CModelInfoAccelerator::GetModelInfo(int* pModelID, const char* pName)
{
	signed short	nModelIndex = GetNextModelInfoId();
	if ( nModelIndex != 0xFFFF )
	{
		CBaseModelInfo*	pModelInfo = CModelInfo::ms_modelInfoPtrs[nModelIndex];
		if ( pModelInfo )
		{
			if ( pModelInfo->GetHash() == CKeyGen::GetUppercaseKey(pName) )
			{
				if ( pModelID )
					*pModelID = nModelIndex;
				return pModelInfo;
			}
		}
	}
	return nullptr;
}

bool CModelInfoAccelerator::GetModelInfoIdFile()
{
	FILE*	hFile = CFileMgr::OpenFile(m_aFileName, "rb");
	m_bFileExists = hFile != nullptr;
	m_pMemory = new unsigned short[MINFO_ALLOCATION_SIZE];
	memset(m_pMemory, 0, MINFO_ALLOCATION_SIZE * sizeof(unsigned short));

	if ( m_bFileExists )
	{
		CFileMgr::Read(hFile, m_pMemory, MINFO_ALLOCATION_SIZE * sizeof(unsigned short));
		CFileMgr::CloseFile(hFile);
	}
	return m_bFileExists;
}

void CModelInfoAccelerator::EndOfLoadPhase()
{
	if ( !m_bFileExists )
	{
		if ( FILE* hFile = CFileMgr::OpenFile(m_aFileName, "wb") )
		{
			CFileMgr::Write(hFile, m_pMemory, MINFO_ALLOCATION_SIZE * sizeof(unsigned short));
			CFileMgr::CloseFile(hFile);
		}
	}
	delete[] m_pMemory;
	m_pMemory = nullptr;
}

CBaseModelInfo* CModelInfoAccelerator::GetEntry(const char* pName, int* pModelID)
{
	if ( m_bFileExists )
		return GetModelInfo(pModelID, pName);
	else
	{
		CBaseModelInfo*	pNonCached = CModelInfo::GetModelInfo(pName, pModelID);
		AddModelInfoId(pNonCached ? static_cast<unsigned short>(*pModelID) : 0xFFFF);
		return pNonCached;
	}
}

#ifdef USE_MODELACCEL

static CBaseModelInfo* GetEntryWrapper(const char* pName, int* pModelID)
{
	return gModelAccel.GetEntry(pName, pModelID);
}

static StaticPatcher	Patcher([](){ 
						Memory::InjectHook(0x5B624E, GetEntryWrapper);
									});

#endif