#include "StdAfx.h"

const DWORD C_PcSave::ms_dwBlockVersions[] = { SAVE_FALLBACK_BLOCK0_VERSION, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

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

unsigned long C_PcSave::MakeTimestamp()
{
	static unsigned long nComputedHash = 0;

	if ( !nComputedHash )
		nComputedHash = HashingClass.FullCRC(reinterpret_cast<unsigned char*>("VCSPCUPDATERSAVE"), 16);
	return nComputedHash;
}

long C_PcSave::PerformBlockFallbackOnLoad(long nBlock, bool bSpecialBlock0Call)
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