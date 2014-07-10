#include "StdAfx.h"
#include "Streaming.h"

#include "ModelInfo.h"
#include "CWanted.h"
#include "PlayerInfo.h"

CStreamingInfo* const	CStreaming::ms_aInfoForModel = (CStreamingInfo*)0x8E4CC0;

CdImage					CStreaming::ms_cdImages[NUM_IMG_FILES];
bool					CStreaming::ms_bCopBikeAllowed;

WRAPPER int CdStreamAddImage(const char* pName, bool bStandardIMG, unsigned char bEncryptionType) { WRAPARG(pName); WRAPARG(bStandardIMG); WRAPARG(bEncryptionType); EAXJMP(0x407610); }
WRAPPER void InitModelIndices() { EAXJMP(0x5B57C0); }
WRAPPER void CStreaming::RequestModel(int nIndex, int nPriority) { WRAPARG(nIndex); WRAPARG(nPriority); EAXJMP(0x4087E0); }
WRAPPER void CStreaming::SetModelIsDeletable(int nIndex) { WRAPARG(nIndex); EAXJMP(0x409C10); }
WRAPPER void CStreaming::RequestSpecialModel(int nIndex, const char* pName, int nPriority) { WRAPARG(nIndex); WRAPARG(pName); WRAPARG(nPriority); EAXJMP(0x409D10); }
WRAPPER void CStreaming::LoadAllRequestedModels(bool bPriorityOnly) { WRAPARG(bPriorityOnly); EAXJMP(0x40EA10); }
WRAPPER int CStreaming::RandomizeTaxiDriverIDByTown() { EAXJMP(0x407D50); }
WRAPPER bool CStreaming::RemoveLeastUsedModel(unsigned int nUnknown) { WRAPARG(nUnknown); EAXJMP(0x40CFD0); }
WRAPPER void CStreaming::DeleteRwObjectsBehindCamera(int nMemUsed) { WRAPARG(nMemUsed); EAXJMP(0x40D7C0); }
WRAPPER void CStreaming::Init() { EAXJMP(0x5B8AD0); }

WRAPPER void CStreaming::LoadCdDirectory(const char* pArchiveName, int nArchiveIndex, CBlowFish* pEncryption) { WRAPARG(pArchiveName); WRAPARG(nArchiveIndex); WRAPARG(pEncryption); EAXJMP(0x5B6170); }

void CStreaming::SetModelTxdIsDeletable(int nIndex)
{
	SetModelIsDeletable(CModelInfo::ms_modelInfoPtrs[nIndex]->GetTextureDict() + 20000);
}

void CStreaming::StreamCopModels(int nTownID)
{
	UNREFERENCED_PARAMETER(nTownID);

	if ( *activeInterior )
		return;

	// TODO: Concern SCM flag to ignore bikes
	CWanted*		pWanted = FindPlayerWanted(-1);
	ms_bCopBikeAllowed = pWanted ? pWanted->GetWantedLevel() < 3 : false;

	if ( ms_aInfoForModel[281].uLoadStatus != StreamingModelLoaded || ms_aInfoForModel[VT_POLICEM].uLoadStatus != StreamingModelLoaded )
	{
		RequestModel(281, 2);
		RequestModel(VT_POLICEM, 2);
	}

	if ( ms_bCopBikeAllowed && (ms_aInfoForModel[284].uLoadStatus != StreamingModelLoaded || ms_aInfoForModel[VT_ELECTRAP].uLoadStatus != StreamingModelLoaded)  )
	{
		RequestModel(284, 2);
		RequestModel(VT_ELECTRAP, 2);
	}
}

int CStreaming::ChooseCopModel()
{
	return ms_aInfoForModel[281].uLoadStatus == StreamingModelLoaded ? 281 : -1;
}

int CStreaming::ChooseCopCarModel(BOOL bIgnoreBikes)
{
	if ( bIgnoreBikes || (rand() % 100) > 50 || ms_aInfoForModel[284].uLoadStatus != StreamingModelLoaded || ms_aInfoForModel[VT_ELECTRAP].uLoadStatus != StreamingModelLoaded || !ms_bCopBikeAllowed )
		return ms_aInfoForModel[281].uLoadStatus == StreamingModelLoaded &&  ms_aInfoForModel[VT_POLICEM].uLoadStatus == StreamingModelLoaded ? VT_POLICEM : -1;

	return VT_ELECTRAP;
}

void CStreaming::RequestSpecialDriverModel(WORD carModel)
{
	// TODO: Enums
  switch ( carModel )
  {
    case 481:
		RequestModel(23, 10);
		return;
    case 448:
		RequestModel(155, 10);
		return;
    case 420:
    case 438:
	case 604:
		RequestModel(RandomizeTaxiDriverIDByTown(), 10);
		return;
    case 463:
		RequestModel(247, 10);
		RequestModel(248, 10);
		return;
    case 409:
		RequestModel(255, 10);
		return;
    case 423:
		RequestModel(264, 10);
		return;
    case 428:
		RequestModel(71, 10);
		return;
    default:
		return;
  }
}

void CStreaming::ReleaseSpecialDriverModel(WORD carModel)
{
	switch ( carModel )
	{
	case 481:
		{
			SetModelIsDeletable(23);
			SetModelTxdIsDeletable(23);
			return;
		}
	case 448:
		{
			SetModelIsDeletable(155);
			SetModelTxdIsDeletable(155);
			return;
		}
	case 420:
	case 438:
	case 604:
		{
			int taxiDriverModel = RandomizeTaxiDriverIDByTown();
			SetModelIsDeletable(taxiDriverModel);
			SetModelTxdIsDeletable(taxiDriverModel);
			return;
		}
	case 463:
		{
			SetModelIsDeletable(247);
			SetModelTxdIsDeletable(247);
			SetModelIsDeletable(248);
			SetModelTxdIsDeletable(248);
			return;
		}
	case 409:
		{
			SetModelIsDeletable(255);
			SetModelTxdIsDeletable(255);
			return;
		}
	case 423:
		{
			SetModelIsDeletable(264);
			SetModelTxdIsDeletable(264);
			return;
		}
	case 428:
		{
			SetModelIsDeletable(71);
			SetModelTxdIsDeletable(71);
			return;
		}
	}
}

int CStreaming::GetSpecialDriverModelID(WORD carModel)
{
	switch ( carModel )
	{
	case 481:
		return 23;
	case 448:
		return 155;
	case 420:
	case 438:
	case 604:
		return RandomizeTaxiDriverIDByTown();
	case 463:
		switch ( random(0, 3) )
		{
		case 0:
			return 247;
		case 1:
			return 248;
		default:
			return -1;
		}
	case 409:
		return 255;
	case 423:
		return 264;
	case 428:
		return 71;
	default:
		return -1;
	}
}

void CStreaming::MakeSpaceFor(int neededMemory)
{
	if ( *memoryUsed >= (*memoryAvailable - neededMemory) )
	{
		LogToFile("Not enough streaming memory to allocate %d bytes, freeing...", neededMemory);

		while ( RemoveLeastUsedModel(32) )
		{
			if ( *memoryUsed < (*memoryAvailable - neededMemory) )
				return;
		}
		LogToFile("Still not enough memory, removing invisible objects...");
		DeleteRwObjectsBehindCamera(*memoryAvailable - neededMemory);
	}
}

void CStreaming::LoadCdDirectory()
{
	if ( ms_cdImages[0].cName[0] )
	{

		for ( int i = 0; i < NUM_IMG_FILES && ms_cdImages[i].cName[0]; ++i )
		{
			unsigned char	encKey[2][24] = {	{	0x81, 0x45, 0x26, 0xFA, 0xDA, 0x7C, 0x6C, 0x11,
												0x86, 0x93, 0xCC, 0x90, 0x2B, 0xB7, 0xE2, 0x32,
												0x10, 0x0F, 0x56, 0x9B, 0x02, 0x8A, 0x6C, 0x5F },
											{	124, 216, 71, 196, 191, 42, 230, 227, 164, 92,
												149, 92, 214, 126, 96, 45, 11, 97, 63, 217, 62,
												171, 41, 221 } 
										} ;
			CBlowFish	blowFish(encKey[ms_cdImages[i].bEncryptionType], 24);
			if ( ms_cdImages[i].bNotPlayerIMG )
			{
				//blowFish.ResetChain();
				LoadCdDirectory(ms_cdImages[i].cName, i, &blowFish);
			}
		}
	}
}

