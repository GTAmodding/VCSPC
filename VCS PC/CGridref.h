#ifndef __CGRIDREF
#define __CGRIDREF

#define WIN32_LEAN_AND_MEAN

#define	FUNC_CGridref__GetIDByPlayerPosition				0x71D5E0

class CGridref
{
private:
	char			artistNames[100][32];

public:
	static void		Draw();
	static void		GetIDByPlayerPosition(BYTE* gridrefX, BYTE* gridrefY);
	static char*	GetNameByID(BYTE gridrefX, BYTE gridrefY);
};

static_assert(sizeof(CGridref) == CGridref_ARRAYSIZE, "CGridref class has wrong size!");

#endif