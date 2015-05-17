#ifndef __CPCSAVE
#define __CPCSAV

#include "General.h"
#include "Weapon.h"
#include "Ped.h"

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
	static int				PerformBlockFallbackOnLoad(int nBlock, bool bSpecialBlock0Call);
	static void				SaveBlockVersion(int nBlock);

	// Dirty
	void					LoadFirstBlock(DWORD* pTimestamp);
};

class CGenericGameStorage
{
public:
	friend const char*		GetSavedGameDateAndTime(int nSlot);

	static const char*		GetNameOfSavedGame(int nSlot);
};

class CPedSaveStructure
{
private:
	class CPedSaveStructure001
	{
	private:
		CVector			Coords;
		float			fHealth;
		float			fArmour;
		CWeapon			Weapons[13];
		unsigned char	bCreatedBy;
		unsigned char	bActiveWeapon;
		unsigned char	nAreaID;
		int				nEntryExitID;
		unsigned char	bFightStyle1;
		unsigned char	bFightStyle2;

	public:
		void			Construct(CPed* pPed);		// Should never be used
		void			Extract(CPed* pPed);
	};

	class CPedSaveStructure002
	{
	private:
		CVector			Coords;
		float			fHealth;
		float			fArmour;
		unsigned char	bCreatedBy;
		unsigned char	nAreaID;
		unsigned int	nEntryExitHash;
		unsigned char	bFightStyle1;
		unsigned char	bFightStyle2;

	public:
		void			Construct(CPed* pPed);
		void			Extract(CPed* pPed);
	};

public:
	static void			Construct(CPed* pPed);
	static void			Extract(CPed* pPed);

	static_assert(sizeof(CPedSaveStructure001) == 0x18C, "Wrong size: CPedSaveStructure001");
	static_assert(sizeof(CPedSaveStructure001) != sizeof(CPedSaveStructure002), "Ped save structure size conflicting");
};


// For hooking sake
extern int		SlotValidation[NUM_SAVE_SLOTS+1];
extern char		SlotSaveDate[NUM_SAVE_SLOTS][70];
extern char		SlotFileName[NUM_SAVE_SLOTS][MAX_PATH];

#endif