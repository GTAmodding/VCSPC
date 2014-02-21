#ifndef __CPCSAVE
#define __CPCSAV

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

	static unsigned int	MakeTimestamp();
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