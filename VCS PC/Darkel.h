#ifndef __DARKEL
#define __DARKEL

#include "Ped.h"
#include "Weapon.h"

class CDarkel
{
private:
	static bool&			bNeedHeadShot;
	static bool&			bStandardSoundAndMessages;
	static unsigned int&	KillsNeeded;
	static char*&			pStartMessage;
	static int&				TimeOfFrenzyStart;
	static int&				TimeLimit;
	static eWeaponType&		WeaponType;
	static unsigned short&	Status;

	static unsigned int		TotalKillsNeeded;

private:
	static void				StartFrenzy(eWeaponType weapType, int nTimeLimit, unsigned short wKillsNeeded, int nModelToKill, char* pMsg, int nModelToKill2, int nModelToKill3, int nModelToKill4, bool bSoundMessages, bool bHeadShots);
	static bool				ThisPedShouldBeKilledForFrenzy(CPed* pPed);

public:
	static inline bool		FrenzyOnGoing()
		{ return Status == 1 || Status == 4; };

	static void				StartFrenzyVCS(eWeaponType weapType, int nTimeLimit, unsigned short wKillsNeeded, int nModelToKill, char* pMsg, int nModelToKill2, int nModelToKill3, int nModelToKill4, bool bSoundMessages, bool bHeadShots);
	static void				RegisterKillByPlayerVehicle(CPed* pKilledPed, eWeaponType lastWeaponDamage);
	static void				DrawMessages();
};

#endif