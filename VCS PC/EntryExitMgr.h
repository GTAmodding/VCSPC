#ifndef __ENTRYEXITMGR
#define __ENTRYEXITMGR

class CEntryExit
{
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
	void			AddToBimap(const char* pName);
};

class CEntryExitManager
{
private:
	static CPool<CEntryExit>*&			mp_poolEntryExits;

public:
	static inline CPool<CEntryExit>*	GetPool()
		{ return mp_poolEntryExits; }

	static int							AddOne(float fEntX, float fEntY, float fEntZ, float fEntA, float fEntRX, float fEntRY, float, float fExitX, float fExitY, float fExitZ, float fExitA, int nArea, int flags, int nExtraColors, int nTimeOn, int nTimeOff, int nUnkSky, const char* pGXTName);
};

static_assert(sizeof(CEntryExit) == 0x3C, "Wrong size: CEntryExit");

#endif