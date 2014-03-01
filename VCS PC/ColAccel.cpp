#include "StdAfx.h"
#include "ColAccel.h"

#include "FileMgr.h"

const char*&	CColAccel::mp_cCacheName = *(const char**)0x8D0F84;
int&			CColAccel::m_iCachingColSize = *(int*)0xBC409C;
int&			CColAccel::m_iCacheState = *(int*)0xBC40A0;
int&			CColAccel::m_iNumColItems = *(int*)0xBC40A8;
int&			CColAccel::m_iNumSections = *(int*)0xBC40B4;
int&			CColAccel::m_iNumColBounds = *(int*)0xBC40B8;
int&			CColAccel::m_iNumIPLItems = *(int*)0xBC40B0;
void*&			CColAccel::mp_caccColItems = *(void**)0xBC40A4;
void*&			CColAccel::mp_caccIPLItems = *(void**)0xBC40AC;
int*&			CColAccel::m_iSectionSize = *(int**)0xBC4098;
void*&			CColAccel::m_iplDefs = *(void**)0xBC4094;
void*&			CColAccel::m_colBounds = *(void**)0xBC4090;

void CColAccel::startCache()
{
	// Call SA startCache...
	((void(*)())0x5B31A0)();

	// ...and now finish what R* never did
	if ( FILE* hFile = CFileMgr::OpenFile(mp_cCacheName, "rb") )
	{
		// Parse generated cinfo.bin
		CFileMgr::Read(hFile, &m_iNumColItems, sizeof(int));
		mp_caccColItems = new char[m_iNumColItems * 48];
		CFileMgr::Read(hFile, mp_caccColItems, m_iNumColItems * 48);

		CFileMgr::Read(hFile, &m_iNumSections, sizeof(int));
		CFileMgr::Read(hFile, m_iSectionSize, m_iNumSections * sizeof(int));

		CFileMgr::Read(hFile, m_iplDefs, 0x3400);

		CFileMgr::Read(hFile, &m_iNumColBounds, sizeof(int));
		CFileMgr::Read(hFile, m_colBounds, m_iNumColBounds * 24);

		CFileMgr::Read(hFile, &m_iNumIPLItems, sizeof(int));
		mp_caccIPLItems = new char[m_iNumIPLItems * 20];
		CFileMgr::Read(hFile, mp_caccIPLItems, m_iNumIPLItems * 20);

		m_iCacheState = 2;

		CFileMgr::CloseFile(hFile);
	}
	else
	{
		// Generate a new one
		m_iNumSections = 0;
		m_iNumColBounds = 0;
		m_iNumColItems = 0;
		m_iNumIPLItems = 0;

		mp_caccColItems = new char[m_iCachingColSize * 48];
		mp_caccIPLItems = new char[20][64];

		m_iCacheState = 1;
	}
}

void CColAccel::endCache()
{
	((void(*)())0x5B2AD0)();
	
	delete mp_caccColItems;
	delete mp_caccIPLItems;
}