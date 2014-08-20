#include "StdAfx.h"
#include "FileMgr.h"

#include "ModelInfo.h"
#include "DLCManager.h"
#include "Streaming.h"
#include "EntryExitMgr.h"
#include "Audio.h"
#include "GroupedBuildings.h"
#include "TxdStore.h"
#include "Object.h"
#include "ColAccel.h"

tFileLoaderList_IMG*	CFileLoader::m_pImagesList;
tFileLoaderList*		CFileLoader::m_pObjectsList;
tFileLoaderList*		CFileLoader::m_pScenesList;
tFileLoaderList*		CFileLoader::m_pCollisionsList;

tFileLoaderList*		CFileLoader::m_pCarcolsList;
tFileLoaderList*		CFileLoader::m_pVehAudioList;

char					CFileLoader::m_cParticlesPath[64] = "MODELS\\PARTICLE.TXD";
char					CFileLoader::m_cPedgrpPath[64] = "DATA\\PEDGRP.DAT";
char					CFileLoader::m_cPopcyclePath[64] = "DATA\\POPCYCLE.DAT";
char					CFileLoader::m_cTimecycPath[64] = "DATA\\TIMECYC.DAT";
char					CFileLoader::m_cFrontendPath[64] = "";
char					CFileLoader::m_cP2dfxPath[64];

unsigned char			CFileLoader::m_bCurrentEncryptionType;

// Wrappers
WRAPPER void CFileMgr::SetDirMyDocuments() { EAXJMP(0x538860); }
WRAPPER void CFileMgr::SetDir(const char* pDir) { WRAPARG(pDir); EAXJMP(0x5387D0); }

WRAPPER void CFileLoader::LoadObjectTypes(const char* pFileName) { WRAPARG(pFileName); EAXJMP(0x5B8400); }
WRAPPER void CFileLoader::LoadScene(const char* pFileName) { WRAPARG(pFileName); EAXJMP(0x5B8700); }
WRAPPER void CFileLoader::LoadCollisionFile(const char* pFileName, unsigned char bUnk) { WRAPARG(pFileName); WRAPARG(bUnk); EAXJMP(0x5B4E60); }

// Hacky trick to call a part of old CFileLoader::LoadLevel
static WRAPPER void InitPostIDEStuff() { EAXJMP(0x5B924E); }
static WRAPPER void InitPostLoadLevelStuff() { EAXJMP(0x5B930F); }

void SetAtomicModelInfoFlags(CAtomicModelInfo* pInfo, unsigned int dwFlags)
{
	((void(*)(CAtomicModelInfo*,unsigned int))0x5B3B20)(pInfo, dwFlags);

	// VCS PC flags

	// Project realtime shadow?
	if ( dwFlags & 0x40000 )
		pInfo->SetCastShadowFlag();
}

std::string CFileLoader::TranslatePath(const char* pFileName, const char* pDLCName)
{
	// Find known device classes
	const char*		pDLCPos = strstr(pFileName, "dlc:");

	// Find the one closest to 0
	// Nothing for now

	if ( pDLCPos )
	{
		// We need to tokerize the string a bit
		std::string		strOutString;
		if ( pDLCPos != pFileName )
			strOutString.append(pFileName, pDLCPos);

		// Translate dlc: device class to an actual path
		// Find DLC index by name
		for ( int i = 0; i < NUM_DLC_PACKS; i++ )
		{
			if ( !_stricmp(pDLCName, CDLCManager::GetDLC(i)->GetName()) )
			{
				strOutString += "DLC\\DLC";
				strOutString += std::to_string(static_cast<long long>(i));
				strOutString += (pDLCPos+4);

				return strOutString;
			}
		}
	}

	return pFileName;
}

const char* CFileLoader::LoadLine(FILE* hFile)
{
	static char		cLineBuffer[512];

	if ( !CFileMgr::ReadLine(hFile, cLineBuffer, sizeof(cLineBuffer)) )
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

int CFileLoader::LoadObject(const char* pLine)
{
	int				objID; //objCount;
	char			modelName[24], texName[24];
	//float			drawDist[3];
	float			fDrawDist;
	unsigned int	flags;

	if ( sscanf(pLine, "%d %s %s %f %d", &objID, modelName, texName, &fDrawDist, &flags) != 5 )
	{
		LogToFile("ERROR: Invalid line found when parsing an IDE file: %s", pLine);
		return -1;
	}
	if ( fDrawDist < 4.0f )
		return -1;
	/*if ( sscanf(pLine, "%d %s %s %f %d", &objID, modelName, texName, &fDrawDist, &flags) != 5 || drawDist[0] < 4.0f )
	{
		// No instances of such lines in files whatsoever
		// Might have been used by LodAtomicModelInfos?
		/*if ( sscanf(pLine, "%d %s %s %d", &objID, modelName, texName, &objCount) != 4 )
			return -1;

		switch ( objCount )
		{
		case 1:
			sscanf(pLine, "%d %s %s %d %f %d", &objID, modelName, texName, &objCount, &drawDist[0], &flags);
			break;
		case 2:
			sscanf(pLine, "%d %s %s %d %f %f %d", &objID, modelName, texName, &objCount, &drawDist[0], &drawDist[1], &flags);
			break;
		case 3:
			sscanf(pLine, "%d %s %s %d %f %f %f %d", &objID, modelName, texName, &objCount, &drawDist[0], &drawDist[1], &drawDist[2], &flags);
			break;
		}
	}*/

	if ( !CModelInfo::ms_modelInfoPtrs[objID] )
	{
		CAtomicModelInfo*		pModel;

		if ( flags & 0x1000 )
			pModel = CModelInfo::AddDamageAtomicModel(objID);
		else
			pModel = CModelInfo::AddAtomicModel(objID);

		pModel->fLodDistanceUnscaled = fDrawDist;
		pModel->ulHashKey = CKeyGen::GetUppercaseKey(modelName);
		pModel->SetTexDictionary(texName);
		SetAtomicModelInfoFlags(pModel, flags);

		/*// TEMP dbug code
		unsigned int		nTemp = *(unsigned short*)((BYTE*)pModel + 0x12);

		if ( !(nTemp & 0x100) )
		{
			nTemp &= 0x7800;
			assert(nTemp != 0x3000);
		}*/
	}
	return objID;
}

void CFileLoader::LoadEntryExit(const char* pLine)
{
	int						nTimeOn = 0, nTimeOff = 24;
	int						nSkyUnk = 2;
	float					fEntX, fEntY, fEntZ, fEntA, fEntRX, fEntRY, fExitX, fExitY, fExitZ, fExitA, fUnused;
	int						nArea, flags, nExtraColors;
	char					IntName[16], UniqueName[16] = "";
	static unsigned int		nFakeHash = 0;

	sscanf(	pLine, "%f %f %f %f %f %f %f %f %f %f %f %d %d %s %d %d %d %d %s",
			&fEntX, &fEntY, &fEntZ, &fEntA, &fEntRX, &fEntRY, &fUnused, &fExitX, &fExitY, &fExitZ, &fExitA,
			&nArea, &flags, IntName, &nExtraColors, &nSkyUnk, &nTimeOn, &nTimeOff, UniqueName);
	
	
	auto			nLen = strlen(UniqueName);
	unsigned int	nHash = nLen ? HashHelper.FullCRC(reinterpret_cast<unsigned char*>(UniqueName), nLen) : nFakeHash++;

	assert(!CEntryExit::IsDefined(nHash));
	if ( !CEntryExit::IsDefined(nHash) )
	{
		char*	pQuote = strrchr(IntName, '\"');
		if ( pQuote )
		{
			*pQuote = '\0';
			pQuote = IntName+1;
		}
		else
			pQuote = IntName;

		int		nEnexID = CEntryExitManager::AddOne(fEntX, fEntY, fEntZ, fEntA, fEntRX, fEntRY, fUnused, fExitX, fExitY, fExitZ, fExitA, nArea, flags, nExtraColors, nTimeOn, nTimeOff, nSkyUnk, pQuote);
	
		if ( CEntryExit* pEnex = CEntryExitManager::GetPool()->GetAtIndex(nEnexID) )
		{
			pEnex->AddToBimap(nHash);

			if ( flags & 1 )
				pEnex->wFlags |= 1;
			if ( flags & 2 )
				pEnex->wFlags |= 2;
			if ( flags & 4 )
				pEnex->wFlags |= 4;
			if ( flags & 8 )
				pEnex->wFlags |= 8;
			if ( flags & 0x10 )
				pEnex->wFlags |= 0x10;
			if ( flags & 0x20 )
				pEnex->wFlags |= 0x20;
			if ( flags & 0x40 )
				pEnex->wFlags |= 0x40;
			if ( flags & 0x80 )
				pEnex->wFlags |= 0x80;
		}
	}
}

int CFileLoader::LoadWeaponObject(const char* pLine)
{
	int		nObjID = -1;
	float	fDrawDist;
	char	modelName[24], texName[24], animName[16];

	sscanf(pLine, "%d %s %s %s %f", &nObjID, modelName, texName, animName, &fDrawDist);

	CWeaponModelInfo*	pModelInfo = CModelInfo::AddWeaponModel(nObjID);

	pModelInfo->ulHashKey = CKeyGen::GetUppercaseKey(modelName);
	pModelInfo->fLodDistanceUnscaled = fDrawDist;

	pModelInfo->SetTexDictionary(texName);
	//CTxdStore::GetPool()->GetAtIndex(pModelInfo->usTextureDictionary)->SetParent(CTxdStore::GetTxdSlot("weapshare"));

	pModelInfo->SetAnimFile(animName);
	pModelInfo->SetColModel(&CTempColModels::ms_colModelWeapon, false);

	return nObjID;
}

/*std::map<int, std::string>	TempMap;

void LogName(int nIndex, const char* pName)
{
	TempMap[nIndex] = pName;
}

void __declspec(naked) CatchVehNames()
{
	_asm
	{
		lea		eax, [esp+0F8h-30h]
		push	eax
		push	ecx
		call	LogName
		add		esp, 8
		mov		eax, [esp+0F8h-94h]
		push	eax
		mov		eax, 4C6770h
		call	eax
		mov		ecx, 5B6FD6h
		jmp		ecx
	}
}

void DumpVehicleAudioSettings()
{
	if ( FILE* hFile = CFileMgr::OpenFile("vehaudiosettings.txt", "w") )
	{
		for ( short i = 400; i < 612; i++ )
		{
			tVehicleAudioSettings		VehicleAudio = ((tVehicleAudioSettings(__stdcall*)(short))0x4F5C10)(i);
			fprintf(hFile, "%s,\t%d,\t%d,\t%d,\t%d,\t%g,\t%g,\t%d,\t%g,\t%d,\t%d,\t%d,\t%d,\t%d,\t%g\n", TempMap[i].c_str(), VehicleAudio.VehicleType, VehicleAudio.EngineOnSound, VehicleAudio.EngineOffSound, VehicleAudio.RadioSoundType, VehicleAudio.BassDepth, VehicleAudio.unk1, VehicleAudio.HornTon, VehicleAudio.HornFreq, VehicleAudio.DoorSound, VehicleAudio.unk2, VehicleAudio.RadioNum, VehicleAudio.RadioType, VehicleAudio.PoliceScannerName, VehicleAudio.EngineVolumeBoost);
		}

		CFileMgr::CloseFile(hFile);
	}
}*/

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
	//Memory::InjectHook(0x5B6FD0, CatchVehNames, PATCH_JUMP);

	// IMG
	for ( auto it = m_pImagesList->cbegin(); it != m_pImagesList->cend(); it++ )
	{
		CdStreamAddImage(it->first.c_str(), true, it->second);
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
	MatchAllModelStrings();
	InitModelIndices();

	// Wraps some calls
	CEmpireManager::Initialise();
	CStreaming::Init();
	InitPostIDEStuff();
	CPedModelInfoVCS::LoadPedColours();
	CAEVehicleAudioEntity::LoadVehicleAudioSettings();
	CGroupedBuildings::Initialise();

	if ( CDLCManager::GetDLC(DLC_2DFX)->IsEnabled() )
		CProject2dfx::Init();

	//DumpVehicleAudioSettings();

	auto*	pStorage = CModelInfo::GetDrawDistanceStorage();

	pStorage->Init();
	for ( int i = 0; i < 20000; ++i )
	{
		if ( CModelInfo::ms_modelInfoPtrs[i] )
		{
			CModelInfo::ms_modelInfoPtrs[i]->ConvertAnimFileIndex();

			unsigned char		nModelType = CModelInfo::ms_modelInfoPtrs[i]->GetModelType();
			if ( nModelType == 1 || nModelType == 3 || nModelType == 5 || nModelType == 8 )
				pStorage->AddToList(i, CModelInfo::ms_modelInfoPtrs[i]);
		}
	}

	pStorage->Shrink();

	// IPL
	for ( auto it = m_pScenesList->cbegin(); it != m_pScenesList->cend(); it++ )
	{
		LoadingScreenLoadingFile(it->c_str());
		LoadScene(it->c_str());
	}

	EndLevelLists();
	EndCarcols();
	EndVehAudio();
	RwTexDictionarySetCurrent(pPushedDictionary);
	InitPostLoadLevelStuff();

	if ( CDLCManager::GetDLC(DLC_2DFX)->IsEnabled() )
		CProject2dfx::EndRegistering();

#ifdef USE_COLACCEL
	CColAccel::endCache();
#endif
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
					m_pImagesList->push_back(make_pair(TranslatePath(pLine+4, pDLCName), m_bCurrentEncryptionType));
				else if ( !_strnicmp(pLine, "IDE", 3) )
					m_pObjectsList->push_back(TranslatePath(pLine+4, pDLCName));
				else if ( !_strnicmp(pLine, "COLFILE", 7) )
					m_pCollisionsList->push_back(TranslatePath(pLine+8, pDLCName));
				else if ( !_strnicmp(pLine, "IPL", 3) )
					m_pScenesList->push_back(TranslatePath(pLine+4, pDLCName));

				// DLC overloads
				else if ( !_strnicmp(pLine, "DLC", 3) )	// NEVER!!! use in non-DLC level file
					strncpy(pDLCName, pLine+4, 32);
				else if ( !_strnicmp(pLine, "PARTICLES", 9) )
					strncpy(m_cParticlesPath, TranslatePath(pLine+10, pDLCName).c_str(), 64);
				else if ( !_strnicmp(pLine, "PEDGRP", 6) )
					strncpy(m_cPedgrpPath, TranslatePath(pLine+7, pDLCName).c_str(), 64);
				else if ( !_strnicmp(pLine, "POPCYCLE", 8) )
					strncpy(m_cPopcyclePath, TranslatePath(pLine+9, pDLCName).c_str(), 64);
				else if ( !_strnicmp(pLine, "TIMECYCLE", 9) )
					strncpy(m_cTimecycPath, TranslatePath(pLine+10, pDLCName).c_str(), 64);
				else if ( !_strnicmp(pLine, "FRONTEND", 8) )
					strncpy(m_cFrontendPath, TranslatePath(pLine+9, pDLCName).c_str(), 64);
				else if ( !_strnicmp(pLine, "P2DFX", 5) )
					strncpy(m_cP2dfxPath, TranslatePath(pLine+6, pDLCName).c_str(), 64);
				else if ( !_strnicmp(pLine, "CARCOLS", 7) )
					BeginCarcols(), m_pCarcolsList->push_back(TranslatePath(pLine+8, pDLCName));
				else if ( !_strnicmp(pLine, "VEHAUDIO", 8) )
					BeginVehAudio(), m_pVehAudioList->push_back(TranslatePath(pLine+9, pDLCName));
			}
		}
		CFileMgr::CloseFile(hFile);
		return true;
	}
	return false;
}


static StaticPatcher	Patcher([](){ 
						Memory::InjectHook(0x5B3F7B, SetAtomicModelInfoFlags);
									});