#ifndef __ENTRYEXITMGR
#define __ENTRYEXITMGR

#include "General.h"
#include "Pools.h"

class CEntryExit;
typedef boost::bimap<CEntryExit*,unsigned int>		tEntryExitBimap;

class CEntryExit
{
private:
	static tEntryExitBimap				strUniqueNames;

public:
	char			Name[8];
	CRect			Entry;
	float			EntryZ;
	float			EntryHeading;
	CVector			Exit;
	float			ExitHeading;
	unsigned short	wFlags;
	unsigned char	nAreaID;
	unsigned char	nExtraColors;
	unsigned char	bTimeOn;
	unsigned char	bTimeOff;
	unsigned char	nUnkFlag;
	CEntryExit*		pPairedEnex;

public:
	static inline tEntryExitBimap*		GetBimap()
			{ return &strUniqueNames; }
	static inline bool		IsDefined(unsigned int nNameHash)
		{ return strUniqueNames.right.find(nNameHash) != strUniqueNames.right.end(); }
	inline void				AddToBimap(unsigned int nNameHash)
		{ strUniqueNames.insert(tEntryExitBimap::value_type(this, nNameHash)); }
	
};

class CEntryExitManager
{
private:
	static CPool<CEntryExit>*&			mp_poolEntryExits;
	static int&							ms_entryExitStackPosn;
	static CEntryExit** const			ms_entryExitStack;

	// VCS PC class extension
	static unsigned int					m_dwFallbackFrom, m_dwFallbackTo;

public:
	static inline void SetForFallback(unsigned int dwFrom, unsigned int dwTo)
		{ m_dwFallbackFrom = dwFrom; m_dwFallbackTo = dwTo; }
	static inline CPool<CEntryExit>*	GetPool()
		{ return mp_poolEntryExits; }

	static int							AddOne(float fEntX, float fEntY, float fEntZ, float fEntA, float fEntRX, float fEntRY, float, float fExitX, float fExitY, float fExitZ, float fExitA, int nArea, int flags, int nExtraColors, int nTimeOn, int nTimeOff, int nUnkSky, const char* pGXTName);
	static bool							Load();
	static bool							Save();
};

static_assert(sizeof(CEntryExit) == 0x3C, "Wrong size: CEntryExit");

#endif