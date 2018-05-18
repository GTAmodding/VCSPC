#ifndef __CHEAT
#define __CHEAT

#include "Cheatdef.h"

#define CHEAT(id, hash1, hash2, func, str1, str2) id,

class CCheat
{
public:
	enum {
		CHEATS
		NUMCHEATS
	};

	static char m_CheatString[30];
	static char m_CheatStringPad[30];
	static bool m_aCheatsActive[NUMCHEATS];
	static bool *m_aCheatsActive_original;
	static void (*m_aCheatFunctions[NUMCHEATS])(void);
	static uint32 m_aCheatHashKeys[NUMCHEATS*2];
	static bool &m_bHasPlayerCheated;

	static void ResetCheats(void);
	static void AddToCheatString(char key, char *cheatstring);
	static void DoCheats(void);

	static void VehicleCheat(int id);

	static void WeaponCheat1(void);
	static void WeaponCheat2(void);
	static void WeaponCheat3(void);
	static void HealthCheat(void);
	static void ArmourCheat(void);
	static void MoneyCheat(void);
	static void WantedLevelUpCheat(void);
	static void WantedLevelDownCheat(void);
	static void SuicideCheat(void);
	static void BmxCheat(void);
	static void ApacheCheat(void);
	static void QuadCheat(void);
	static void TankCheat(void);
	static void SasquatchCheat(void);
	static void GarbageTruckCheat(void);
	static void InfernusCheat(void);
	static void InsurrectionCheat(void);
	static void BlackCarsCheat(void);
	static void WhiteCarsCheat(void);
	static void OnlyWheelsCheat(void);
	static void NoTractionCheat(void);
	static void BackToTheFutureCheat(void);
	static void BlowupCarsCheat(void);
	static void GreenLightsCheat(void);
	static void MadDriverCheat(void);
	static void EverybodyAttacksPlayerCheat(void);
	static void WeaponsForAllCheat(void);
	static void MayhemCheat(void);
	static void SlowTimeCheat(void);
	static void FastTimeCheat(void);
	static void UltraSunnyWeatherCheat(void);
	static void ExtraSunnyWeatherCheat(void);
	static void SunnyWeatherCheat(void);
	static void CloudyWeatherCheat(void);
	static void RainyWeatherCheat(void);
	static void FoggyWeatherCheat(void);
};

#undef CHEAT
#undef CHEATS

#endif
