#include "StdAfx.h"

CdImage	CStreaming::ms_cdImages[NUM_IMG_FILES];
bool	CStreaming::ms_bCopBikeAllowed;

WRAPPER int CdStreamAddImage(const char* pName, bool bStandardIMG) { WRAPARG(pName); WRAPARG(bStandardIMG); EAXJMP(0x407610); }
WRAPPER void InitModelIndices() { EAXJMP(0x5B57C0); }

// TODO: Sort these wrappers
void CStreaming::RequestModel(DWORD index, int a2)
{
	DWORD dwFunc = (DWORD)FUNC_CStreaming__RequestModel;
	_asm
	{
		mov		eax, a2
		push	eax
		mov		eax, index
		push	eax
		call	dwFunc
		add		esp, 8
	}
}

void CStreaming::SetModelIsDeletable(DWORD index)
{
	DWORD dwFunc = (DWORD)FUNC_CStreaming__ReleaseModel;
	_asm
	{
		mov		eax, index
		push	eax
		call	dwFunc
		add		esp, 4
	}
}

void CStreaming::ReleaseTexture(DWORD index)
{
	DWORD dwFunc = (DWORD)FUNC_CStreaming__ReleaseTexture;
	_asm
	{
		mov		eax, index
		push	eax
		call	dwFunc
		add		esp, 4
	}
}

void CStreaming::RequestSpecialModel(int index, const char* pName, int unk)
{
	DWORD dwFunc = (DWORD)FUNC_CStreaming__RequestSpecialModel;
	_asm
	{
		push	unk
		push	pName
		push	index
		call	dwFunc
		add		esp, 0Ch
	}
}

void CStreaming::LoadRequestedModels(int unk)
{
	DWORD dwFunc = (DWORD)FUNC_CStreaming__LoadRequestedModels;
	_asm
	{
		mov		eax, unk
		push	eax
		call	dwFunc
		add		esp, 4
	}
}

int CStreaming::RandomizeTaxiDriverIDByTown()
{
	DWORD dwFunc = (DWORD)FUNC_CStreaming__RandomizeTaxiDriverIDByTown;
	int	  iResult;
	_asm
	{
		call	dwFunc
		mov		iResult, eax
	}
	return iResult;
}

void CStreaming::ChangeDirectoryToUserDir()
{
	DWORD dwFunc = (DWORD)FUNC_CStreaming__ChangeDirectoryToUserDir;
	_asm call	dwFunc
}

bool CStreaming::RemoveLeastUsedModel(BYTE unkFlag)
{
	DWORD	dwFunc = (DWORD)FUNC_CStreaming__RemoveLeastUsedModel;
	bool	bResult;
	_asm
	{
		movzx	eax, unkFlag
		push	eax
		call	dwFunc
		mov		bResult, al
		add		esp, 4
	}
	return bResult;
}

void CStreaming::DeleteRwObjectsBehindCamera(int memoryUsed)
{
	DWORD dwFunc = (DWORD)FUNC_CStreaming__DeleteRwObjectsBehindCamera;
	_asm
	{
		push	memoryUsed
		call	dwFunc
		add		esp, 4
	}
}

void CStreaming::LoadCdDirectory(const char* pArchiveName, long nArchiveIndex, CBlowFish* pEncryption)
{
	_asm
	{
		push	pEncryption
		push	nArchiveIndex
		push	pArchiveName
		mov		eax, 5B6170h
		call	eax
		add		esp, 0Ch
	}
}

void CStreaming::StreamCopModels(int nTownID)
{
	UNREFERENCED_PARAMETER(nTownID);

	if ( *activeInterior )
		return;

	// TODO: Concern SCM flag to ignore bikes
	CWanted*		pWanted = FindPlayerWanted(-1);
	ms_bCopBikeAllowed = pWanted ? pWanted->GetWantedLevel() < 3 : false;

	if ( _loadedObjectInfo[281].bLoaded != true || _loadedObjectInfo[VT_POLICEM].bLoaded != true )
	{
		CStreaming::RequestModel(281, 2);
		CStreaming::RequestModel(VT_POLICEM, 2);
	}

	if ( ms_bCopBikeAllowed && (_loadedObjectInfo[284].bLoaded != true || _loadedObjectInfo[VT_ELECTRAP].bLoaded != true)  )
	{
		CStreaming::RequestModel(284, 2);
		CStreaming::RequestModel(VT_ELECTRAP, 2);
	}
}

int CStreaming::ChooseCopModel()
{
	return _loadedObjectInfo[281].bLoaded == true ? 281 : -1;
}

int CStreaming::ChooseCopCarModel(BOOL bIgnoreBikes)
{
	if ( bIgnoreBikes || (rand() % 100) > 50 || _loadedObjectInfo[284].bLoaded != true || _loadedObjectInfo[VT_ELECTRAP].bLoaded != true || !ms_bCopBikeAllowed )
		return  _loadedObjectInfo[281].bLoaded == true &&  _loadedObjectInfo[VT_POLICEM].bLoaded == true ? VT_POLICEM : -1;

	return VT_ELECTRAP;
}

void CStreaming::RequestSpecialDriverModel(WORD carModel)
{
	// TODO: Enum
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
	DWORD taxiDriverModel;
	switch ( carModel )
	{
	case 481:
		SetModelIsDeletable(23);
		ReleaseTexture(23);
		return;
	case 448:
		SetModelIsDeletable(155);
		ReleaseTexture(155);
		return;
	case 420:
	case 438:
	case 604:
		taxiDriverModel = RandomizeTaxiDriverIDByTown();
		SetModelIsDeletable(taxiDriverModel);
		ReleaseTexture(taxiDriverModel);
		return;
	case 463:
		SetModelIsDeletable(247);
		ReleaseTexture(247);
		SetModelIsDeletable(248);
		ReleaseTexture(248);
		return;
	case 409:
		SetModelIsDeletable(255);
		ReleaseTexture(255);
		return;
	case 423:
		SetModelIsDeletable(264);
		ReleaseTexture(264);
		return;
	case 428:
		SetModelIsDeletable(71);
		ReleaseTexture(71);
		return;
	default:
		return;
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
		unsigned char	encKey[24] = {	0x81, 0x45, 0x26, 0xFA, 0xDA, 0x7C, 0x6C, 0x11,
										0x86, 0x93, 0xCC, 0x90, 0x2B, 0xB7, 0xE2, 0x32,
										0x10, 0x0F, 0x56, 0x9B, 0x02, 0x8A, 0x6C, 0x5F };
		CBlowFish	blowFish(encKey, 24);
		long		nIndex = 0;

		for ( int i = 0; i < NUM_IMG_FILES && ms_cdImages[i].cName[0]; ++i, ++nIndex )
		{
			if ( ms_cdImages[i].bNotPlayerIMG )
			{
				blowFish.ResetChain();
				LoadCdDirectory(ms_cdImages[i].cName, nIndex, &blowFish);
			}
		}
	}
}

