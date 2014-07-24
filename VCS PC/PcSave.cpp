#include "StdAfx.h"
#include "PcSave.h"

#include "EntryExitMgr.h"
#include "Streaming.h"

const DWORD C_PcSave::ms_dwBlockVersions[] = { SAVE_FALLBACK_BLOCK0_VERSION, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1 };

static_assert(sizeof(C_PcSave::ms_dwBlockVersions)/sizeof(DWORD) == 28, "Wrong block versions amount in C_PcSave!");

int		SlotValidation[NUM_SAVE_SLOTS+1];
char	SlotSaveDate[NUM_SAVE_SLOTS][70];
char	SlotFileName[NUM_SAVE_SLOTS][MAX_PATH];

#define	FUNC_C_PcSave__PcClassSaveRoutine					
#define	FUNC_C_PcSave__PcClassLoadRoutine					
#define FUNC_C_PcSave__LoadFirstBlock						

WRAPPER bool C_PcSave::PcClassSaveRoutine(const void* pSource, unsigned int dwSize) { WRAPARG(pSource); WRAPARG(dwSize); EAXJMP(0x5D1270); }
WRAPPER bool C_PcSave::PcClassLoadRoutine(void* pDest, unsigned int dwSize) { WRAPARG(pDest); WRAPARG(dwSize); EAXJMP(0x5D1300); }
WRAPPER void C_PcSave::LoadFirstBlock(DWORD* pTimestamp) { WRAPARG(pTimestamp); EAXJMP(0x5D1EA0); }

unsigned int C_PcSave::MakeTimestamp()
{
	static unsigned int nComputedHash = 0;

	if ( !nComputedHash )
		nComputedHash = HashHelper.FullCRC(reinterpret_cast<unsigned char*>("VCSPCUPDATERSAVE"), 16);
	return nComputedHash;
}

int C_PcSave::PerformBlockFallbackOnLoad(int nBlock, bool bSpecialBlock0Call)
{
	DWORD	dwVersion;

	if ( !bSpecialBlock0Call )
	{
		PcClassLoadRoutine(&dwVersion, sizeof(DWORD));

		if ( !nBlock ) // Special Block 0 handling
		{
			//PcClassLoadRoutine(&dwVersion[1], sizeof(DWORD));
			if ( dwVersion != MakeTimestamp() )
			{
				return -2;
			}
			return nBlock;
		}
	}
	else
	{
		dwVersion = nBlock;
		nBlock = 0;
	}

	if ( dwVersion < ms_dwBlockVersions[nBlock] )
	{
		switch ( nBlock )
		{
		case 25:
			CEntryExitManager::SetForFallback(dwVersion, ms_dwBlockVersions[nBlock]);
			return nBlock;
		}
		/*switch ( nBlock )
		{
		case 0:
			return true;
		}*/

/*#if DEBUG
		assert(ms_dwBlockVersions[nBlock] == dwVersion);
#endif*/
		LogToFile("Save Fallback error: Block %d has version %d, but no fallback code is available (current version: %d)", nBlock, dwVersion, ms_dwBlockVersions[nBlock]);
		return -1;
	}
	else
	{
		if ( dwVersion > ms_dwBlockVersions[nBlock] )
		{
			LogToFile("Save Fallback error: Block %d has version %d which is newer than current game build (current version: %d)", nBlock, dwVersion, ms_dwBlockVersions[nBlock]);
			return -3;
		}
	}
	return nBlock;
}

void C_PcSave::SaveBlockVersion(int nBlock)
{
	if ( nBlock )
		PcClassSaveRoutine(reinterpret_cast<const void*>(&ms_dwBlockVersions[nBlock]), sizeof(DWORD));
	else
	{
		unsigned long nHash = MakeTimestamp();
		PcClassSaveRoutine(&nHash, sizeof(DWORD));
	}
}

const char* GetSavedGameDateAndTime(int nSlot)
{
	return SlotSaveDate[nSlot];
}

const char* CGenericGameStorage::GetNameOfSavedGame(int nSlot)
{
	return SlotFileName[nSlot];
}

DEPRECATED void CPedSaveStructure::CPedSaveStructure001::Construct(CPed* pPed)
{
	Coords = *pPed->GetCoords();
	fHealth = pPed->fHealth;
	fArmour = pPed->fArmour;
	bCreatedBy = pPed->PedCreatedBy;
	bActiveWeapon = pPed->m_bActiveWeapon;
	nAreaID = pPed->m_areaCode;
	bFightStyle1 = pPed->bFightingStyle;
	bFightStyle2 = pPed->bFightingStyleExtra;
	nEntryExitID = -1;
	if ( pPed->pCurrentEntryExit )
	{
		CEntryExit*		pEnex = pPed->pCurrentEntryExit->pPairedEnex ? pPed->pCurrentEntryExit->pPairedEnex : pPed->pCurrentEntryExit;
		if ( pEnex->nAreaID )
			nEntryExitID = CEntryExitManager::GetPool()->GetIndex(pEnex);
	}

	memcpy(Weapons, pPed->weaponSlots, sizeof(Weapons));
}

void CPedSaveStructure::CPedSaveStructure001::Extract(CPed* pPed)
{
	pPed->SetCoords(Coords);
	pPed->fHealth = fHealth;
	pPed->fArmour = fArmour;
	pPed->m_bActiveWeapon = bActiveWeapon;
	pPed->SetCharCreatedBy(bCreatedBy);
	pPed->bFightingStyle = bFightStyle1;
	pPed->bFightingStyleExtra = bFightStyle2;

	for ( int i = 0; i < 13; i++ )
	{
		if ( Weapons[i].m_eWeaponType )
		{
			int		nModelID1 = CWeaponInfo::GetWeaponInfo(Weapons[i].m_eWeaponType, 1)->dwModelID;
			int		nModelID2 = CWeaponInfo::GetWeaponInfo(Weapons[i].m_eWeaponType, 1)->dwModelID2;;

			if ( nModelID1 != -1 )
				CStreaming::RequestModel(nModelID1, 8);

			if ( nModelID2 != -1 )
				CStreaming::RequestModel(nModelID2, 8);

			if ( nModelID1 != -1 || nModelID2 != -1 )
				CStreaming::LoadAllRequestedModels(false);

			pPed->GiveWeapon(Weapons[i].m_eWeaponType, Weapons[i].m_nAmmoTotal);
			pPed->GetWeaponSlots()[i].m_nAmmoInClip = Weapons[i].m_nAmmoInClip;
		}
	}

	pPed->SetCurrentWeapon(bActiveWeapon);
	pPed->m_areaCode = nAreaID;

	if ( nEntryExitID == -1 )
		pPed->pCurrentEntryExit = nullptr;
	else
	{
		// Fallback code

		// ID -> name -> hash assignment
		auto*		pBimap = CEntryExit::GetBimap();
		assert(nEntryExitID == 2 || nEntryExitID == 4);

		auto it = pBimap->right.find(HashHelper.FullCRC(nEntryExitID == 2 ? reinterpret_cast<unsigned char*>("SAFE005") : reinterpret_cast<unsigned char*>("SAFE003"), 7));
		
		assert(it != pBimap->right.end());
		pPed->pCurrentEntryExit = it != pBimap->right.end() ? it->second : nullptr;
	}
}



void CPedSaveStructure::CPedSaveStructure002::Construct(CPed* pPed)
{
	Coords = *pPed->GetCoords();
	fHealth = pPed->fHealth;
	fArmour = pPed->fArmour;
	bCreatedBy = pPed->PedCreatedBy;
	nAreaID = pPed->m_areaCode;
	bFightStyle1 = pPed->bFightingStyle;
	bFightStyle2 = pPed->bFightingStyleExtra;
	nEntryExitHash = 0xFFFFFFFF;
	if ( pPed->pCurrentEntryExit )
	{
		CEntryExit*		pEnex = pPed->pCurrentEntryExit->pPairedEnex ? pPed->pCurrentEntryExit->pPairedEnex : pPed->pCurrentEntryExit;
		if ( pEnex->nAreaID )
			nEntryExitHash = CEntryExit::GetBimap()->left.find(pEnex)->second;
	}
}

void CPedSaveStructure::CPedSaveStructure002::Extract(CPed* pPed)
{
	pPed->SetCoords(Coords);
	pPed->fHealth = fHealth;
	pPed->fArmour = fArmour;
	pPed->SetCharCreatedBy(bCreatedBy);
	pPed->bFightingStyle = bFightStyle1;
	pPed->bFightingStyleExtra = bFightStyle2;
	pPed->m_areaCode = nAreaID;

	if ( nEntryExitHash == 0xFFFFFFFF )
		pPed->pCurrentEntryExit = nullptr;
	else
	{
		auto*	pBimap = CEntryExit::GetBimap();
		auto	it = pBimap->right.find(nEntryExitHash);

		pPed->pCurrentEntryExit = it != pBimap->right.end() ? it->second : nullptr;
	}
}

void CPedSaveStructure::Extract(CPed* pPed)
{
	unsigned int		nStructSize;

	C_PcSave::PcClassLoadRoutine(&nStructSize, sizeof(nStructSize));

	if ( nStructSize == sizeof(CPedSaveStructure001) )
	{
		CPedSaveStructure001		Struct;

		C_PcSave::PcClassLoadRoutine(&Struct, sizeof(Struct));
		Struct.Extract(pPed);
	}
	else if ( nStructSize == sizeof(CPedSaveStructure002) )
	{
		CPedSaveStructure002		Struct;

		C_PcSave::PcClassLoadRoutine(&Struct, sizeof(Struct));
		Struct.Extract(pPed);
	}

}

void CPedSaveStructure::Construct(CPed* pPed)
{
	// ALWAYS use the newest struct here
	unsigned int			nStructSize = sizeof(CPedSaveStructure002);
	CPedSaveStructure002	Struct;

	Struct.Construct(pPed);
	C_PcSave::PcClassSaveRoutine(&nStructSize, sizeof(nStructSize));
	C_PcSave::PcClassSaveRoutine(&Struct, sizeof(Struct));
}