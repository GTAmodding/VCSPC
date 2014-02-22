#ifndef __CCONFISCATEDWEAPONS
#define __CCONFISCATEDWEAPONS

#include "PlayerInfo.h"

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