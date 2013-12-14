#include "StdAfx.h"

tFileLoaderList*		CFileLoader::m_pImagesList;
tFileLoaderList*		CFileLoader::m_pObjectsList;
tFileLoaderList*		CFileLoader::m_pScenesList;
tFileLoaderList*		CFileLoader::m_pCollisionsList;

char					CFileLoader::m_cParticlesPath[64] = "MODELS\\PARTICLE.TXD";
char					CFileLoader::m_cPedgrpPath[64] = "DATA\\PEDGRP.DAT";
char					CFileLoader::m_cPopcyclePath[64] = "DATA\\POPCYCLE.DAT";
char					CFileLoader::m_cTimecycPath[64] = "DATA\\TIMECYC.DAT";
char					CFileLoader::m_cFrontendPath[64] = "\0";

// Wrappers
WRAPPER void CFileLoader::LoadObjectTypes(const char* pFileName) { WRAPARG(pFileName); EAXJMP(0x5B8400); }
WRAPPER void CFileLoader::LoadScene(const char* pFileName) { WRAPARG(pFileName); EAXJMP(0x5B8700); }
WRAPPER void CFileLoader::LoadCollisionFile(const char* pFileName, unsigned char bUnk) { WRAPARG(pFileName); WRAPARG(bUnk); EAXJMP(0x5B4E60); }

// Hacky trick to call a part of old CFileLoader::LoadLevel
static WRAPPER void InitPostIDEStuff() { EAXJMP(0x5B924E); }
static WRAPPER void InitPostLoadLevelStuff() { EAXJMP(0x5B930F); }

const char* CFileLoader::LoadLine(FILE* hFile)
{
	static char		cLineBuffer[512];

	if ( !CFileMgr::ReadLine(hFile, cLineBuffer, 512) )
		return nullptr;

	for ( int i = 0; cLineBuffer[i]; ++i )
	{
		if ( cLineBuffer[i] == '\n' )
			cLineBuffer[i] = '\0';
		else if ( cLineBuffer[i] < ' ' || cLineBuffer[i] == ',' )
			cLineBuffer[i] = ' ';
	}

	const char* p = cLineBuffer;
	while ( *p <= ' ' )
	{
		if ( !*p++ )
			break;
	}
	return p;
}

void CFileLoader::LoadLevels()
{
	RwTexDictionary*	pPushedDictionary = RwTexDictionaryGetCurrent();
	
	if ( !pPushedDictionary )
	{
		pPushedDictionary = RwTexDictionaryCreate();
		RwTexDictionarySetCurrent(pPushedDictionary);
	}

	Memory::Patch<DWORD>(0x590D2B, 31 + m_pObjectsList->size() + m_pScenesList->size());
	Memory::Patch<DWORD>(0x590D68, 31 + m_pObjectsList->size() + m_pScenesList->size());

	// IMG
	for ( auto it = m_pImagesList->cbegin(); it != m_pImagesList->cend(); it++ )
	{
		CdStreamAddImage(it->c_str(), true);
	}

	// IDE
	for ( auto it = m_pObjectsList->cbegin(); it != m_pObjectsList->cend(); it++ )
	{
		LoadingScreenLoadingFile(it->c_str());
		LoadObjectTypes(it->c_str());
	}

	// COLFILE
	for ( auto it = m_pCollisionsList->cbegin(); it != m_pCollisionsList->cend(); it++ )
	{
		//LoadingScreenLoadingFile(it->c_str());
		LoadCollisionFile(it->c_str(),  false);
	}

	// Pre-IPL stuff
	InitModelIndices();

	// Wraps some calls
#ifdef _INCLUDE_BETA_CODE
	CEmpireManager::Initialise();
#endif
	InitPostIDEStuff();
	CPedModelInfoVCS::LoadPedColours();

	for ( int i = 0; i < 20000; ++i )
	{
		if ( CModelInfo::ms_modelInfoPtrs[i] )
			CModelInfo::ms_modelInfoPtrs[i]->ConvertAnimFileIndex();
	}

	// IPL
	for ( auto it = m_pScenesList->cbegin(); it != m_pScenesList->cend(); it++ )
	{
		LoadingScreenLoadingFile(it->c_str());
		LoadScene(it->c_str());
	}

	EndLevelLists();
	RwTexDictionarySetCurrent(pPushedDictionary);
	InitPostLoadLevelStuff();
}

bool CFileLoader::ParseLevelFile(const char* pFileName, char* pDLCName)
{
	BeginLevelLists();

	if ( FILE* hFile = CFileMgr::OpenFile(pFileName, "r") )
	{
		while ( const char* pLine = LoadLine(hFile) )
		{
			if ( pLine[0] && pLine[0] != '#' )
			{
				if ( !_strnicmp(pLine, "IMG", 3) )
					m_pImagesList->push_back(pLine+4);
				else if ( !_strnicmp(pLine, "IDE", 3) )
					m_pObjectsList->push_back(pLine+4);
				else if ( !_strnicmp(pLine, "COLFILE", 7) )
					m_pCollisionsList->push_back(pLine+8);
				else if ( !_strnicmp(pLine, "IPL", 3) )
					m_pScenesList->push_back(pLine+4);

				// DLC overloads
				else if ( !_strnicmp(pLine, "DLC", 3) )	// NEVER!!! use in non-DLC level file
					strncpy(pDLCName, pLine+4, 32);
				else if ( !_strnicmp(pLine, "PARTICLES", 9) )
					strncpy(m_cParticlesPath, pLine+10, 64);
				else if ( !_strnicmp(pLine, "PEDGRP", 6) )
					strncpy(m_cPedgrpPath, pLine+7, 64);
				else if ( !_strnicmp(pLine, "POPCYCLE", 8) )
					strncpy(m_cPopcyclePath, pLine+9, 64);
				else if ( !_strnicmp(pLine, "TIMECYCLE", 9) )
					strncpy(m_cTimecycPath, pLine+10, 64);
				else if ( !_strnicmp(pLine, "FRONTEND", 8) )
					strncpy(m_cFrontendPath, pLine+9, 64);
			}
		}
		CFileMgr::CloseFile(hFile);
		return true;
	}
	return false;
}