#ifndef __CWEAPONSLOT
#define __CWEAPONSLOT

#define WIN32_LEAN_AND_MEAN

struct CWeaponSlot
{
	eWeaponType	dwType;
	DWORD	dwState;
	DWORD	dwAmmoInClip;
	DWORD	dwAmmoRemaining;
	DWORD	field_10;
	BYTE	field_14;
	BYTE	field_15;
	BYTE	field_16;
	BYTE	field_17;
	void*	pParticle;
};

//static_assert(sizeof(CWeaponSlot) == CWeaponSlot_ARRAYSIZE);

#endif