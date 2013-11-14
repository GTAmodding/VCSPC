#include "StdAfx.h"

void CConfiscatedWeapons::SaveConfiscatedWeapons(const CPlayerInfo& pPlayer)
{
	memset(this, 0, sizeof(CConfiscatedWeapons));
	BYTE bLoopCounter = 0;

	do
	{
		dwWeapType[bLoopCounter] = pPlayer.GetPed()->GetWeaponSlots()[bLoopCounter].dwType;
		dwWeapAmmo[bLoopCounter] = pPlayer.GetPed()->GetWeaponSlots()[bLoopCounter].dwAmmoRemaining;
		if ( dwWeapType[bLoopCounter] )
			bAnyWeaponsToReturn = true;

		LogToFile("Saved to ConfiscatedWeapons class: group %d type %d ammo %d", bLoopCounter, dwWeapType[bLoopCounter], dwWeapAmmo[bLoopCounter]);
		bLoopCounter++;
	}
	while ( bLoopCounter < 13 );
}

void CConfiscatedWeapons::ReturnConfiscatedWeapons(CPlayerInfo& pPlayer)
{
	if ( bAnyWeaponsToReturn )
	{
		BYTE bLoopCounter = 0;
		do
		{
			if ( dwWeapType[bLoopCounter] )
			{
				pPlayer.GetPed()->GiveWeapon(dwWeapType[bLoopCounter], dwWeapAmmo[bLoopCounter]);
				LogToFile("Restored from ConfiscatedWeapons class: group %d type %d ammo %d", bLoopCounter, dwWeapType[bLoopCounter], dwWeapAmmo[bLoopCounter]);
			}
			bLoopCounter++;
		}
		while ( bLoopCounter < 13 ) ;
	}
}


bool CConfiscatedWeapons::GetAnyWeaponsToReturn()
{
	LogToFile("ConfiscatedWeapons.bAnyWeaponsToReturn: %d", bAnyWeaponsToReturn);
	return bAnyWeaponsToReturn;
}
		