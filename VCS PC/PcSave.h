#ifndef __CPCSAVE
#define __CPCSAVE

#define WIN32_LEAN_AND_MEAN

#define	FUNC_C_PcSave__PcClassSaveRoutine					0x5D1270
#define	FUNC_C_PcSave__PcClassLoadRoutine					0x5D1300
#define FUNC_C_PcSave__LoadFirstBlock						0x5D1EA0

// Special Block 0 handling
#define SAVE_FALLBACK_BLOCK0_VERSION						1

#define NUM_SAVE_SLOTS										12

class C_PcSave
{
public:
	static const DWORD		ms_dwBlockVersions[];

public:
	static bool				PcClassSaveRoutine(const void* pSource, unsigned int dwSize);
	static bool				PcClassLoadRoutine(void* pDest, unsigned int dwSize);

	static unsigned long	MakeTimestamp();
	static long				PerformBlockFallbackOnLoad(long nBlock, bool bSpecialBlock0Call);
	static void				SaveBlockVersion(int nBlock);

	// Dirty
	void					LoadFirstBlock(DWORD* pTimestamp);
};

// For hooking sake
extern int		SlotValidation[NUM_SAVE_SLOTS+1];
extern char		SlotSaveDate[NUM_SAVE_SLOTS][70];
extern char		SlotFileName[NUM_SAVE_SLOTS][MAX_PATH];

#endif