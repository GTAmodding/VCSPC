#ifndef __CCONFISCATEDWEAPONS
#define __CCONFISCATEDWEAPONS

#define WIN32_LEAN_AND_MEAN

class CConfiscatedWeapons
{
private:
	bool	bAnyWeaponsToReturn;
	DWORD	dwWeapType[13];
	DWORD	dwWeapAmmo[13];

public:
	void	SaveConfiscatedWeapons(const CPlayerInfo& pPlayer);
	void	ReturnConfiscatedWeapons(CPlayerInfo& pPlayer);
	bool	GetAnyWeaponsToReturn();
};

extern CConfiscatedWeapons weaponsToReturn;

#endif