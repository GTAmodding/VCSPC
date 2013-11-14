#ifndef __CGANGWEAPONS
#define __CGANGWEAPONS

#define WIN32_LEAN_AND_MEAN

class CGangWeapons
{
private:
	DWORD		pad;
	DWORD		firstWeap;
	DWORD		secondWeap;
	DWORD		thirdWeap;

public:
	static void	Init();
};

static_assert(sizeof(CGangWeapons) == CGangWeapons_ARRAYSIZE, "CGangWeapons class has wrong size!");

#endif