#include "StdAfx.h"
#include "Cheat.h"

#include "Pad.h"
#include "CutsceneManager.h"
#include "Text.h"
#include "Hud.h"
#include "Weather.h"
#include "World.h"
#include "Streaming.h"

/*
CHEAT1    Cheat activated
CHEAT2    Weapon cheat
CHEAT3    Health cheat
CHEAT4    Armor cheat
CHEAT5    Wanted level cheat
CHEAT6    Money cheat
CHEAT7    Weather cheat
CHEAT8    Cheat deactivated
*/

#include "Cheatdef.h"

char CCheat::m_CheatString[30];
char CCheat::m_CheatStringPad[30];
bool CCheat::m_aCheatsActive[NUMCHEATS];
bool *CCheat::m_aCheatsActive_original = (bool*)0x969130;
bool &CCheat::m_bHasPlayerCheated = *(bool*)0x96918C;

#define CHEAT(id, hash1, hash2, func, str1, str2) func,
void (*CCheat::m_aCheatFunctions[NUMCHEATS])(void) = {
	CHEATS
};
#undef CHEAT

#define CHEAT(id, hash1, hash2, func, str1, str2) hash1, hash2,
uint32 CCheat::m_aCheatHashKeys[NUMCHEATS*2] = {
	CHEATS
};
#undef CHEAT
#undef CHEATS

WRAPPER void VehicleCheat(int id) { EAXJMP(0x43A0B0); }

void
CCheat::ResetCheats(void)
{
	memset(m_aCheatsActive, 0, sizeof(m_aCheatsActive));
	memset(m_aCheatsActive_original, 0, 92);
	CWeather::ReleaseWeather();
	CTimer::ms_fTimeScale = 1.0f;
	m_CheatString[0] = '\0';
	m_CheatStringPad[0] = '\0';
	m_bHasPlayerCheated = false;
}

void
CCheat::AddToCheatString(char ch, char *cheatstring)
{
	int i, len;
	char cheat[30];

	if(CCutsceneMgr::ms_running)
		return;

	for(i = 28; i > 0; i--)
		cheatstring[i] = cheatstring[i-1];
	cheatstring[0] = ch;
	cheatstring[29] = '\0';

	len = strlen(cheatstring);
	if(len < 6)
		return;
	strncpy(cheat, cheatstring, 30);

	while(len >= 6){
		uint32 key = CKeyGen::GetKey(cheat);

		for(i = 0; i < NUMCHEATS; i++){
			if(key == m_aCheatHashKeys[i*2] || key == m_aCheatHashKeys[i*2+1]){
				const char *msg = TheText.Get("CHEAT1");
				CHud::SetHelpMessage(msg, true, false, false);
				if(m_aCheatFunctions[i])
					m_aCheatFunctions[i]();
				else
					m_aCheatsActive[i] = !m_aCheatsActive[i];
				m_CheatString[0] = '\0';
				return;
			}
		}

		cheat[--len] = '\0';
	}
}

void
CCheat::DoCheats(void)
{
	int i;
	for(i = 0; i < 256; i++)
		if(currKeyState->standardKeys[i] && !prevKeyState->standardKeys[i])
			AddToCheatString(i, m_CheatString);

	CPad *pad = CPad::GetPad(0);
#define BUTTONS \
	X(TRIANGLE, 't') \
	X(CIRCLE, 'c') \
	X(CROSS, 'x') \
	X(SQUARE, 's') \
	X(DPADUP, 'u') \
	X(DPADDOWN, 'd') \
	X(DPADLEFT, 'l') \
	X(DPADRIGHT, 'r') \
	X(LEFTSHOULDER1, '1') \
	X(LEFTSHOULDER2, '2') \
	X(RIGHTSHOULDER1, '3') \
	X(RIGHTSHOULDER2, '4')

#define X(but, ch) if(pad->OldState.but == 0 && pad->NewState.but != 0)	AddToCheatString(ch, m_CheatStringPad);

BUTTONS

#undef X
#undef BUTTONS

}

WRAPPER void CCheat::VehicleCheat(int id) { EAXJMP(0x43A0B0); }

void
CCheat::WeaponCheat1(void)
{
	CStreaming::RequestModel(MI_BRASSKNUCKLE, StreamingModelRequested);
	CStreaming::RequestModel(MI_KNIFECUR, StreamingModelRequested);
	CStreaming::RequestModel(MI_BERETTA, StreamingModelRequested);
	CStreaming::RequestModel(MI_CHROMEGUN, StreamingModelRequested);
	CStreaming::RequestModel(MI_SKORPION, StreamingModelRequested);
	CStreaming::RequestModel(MI_AK47, StreamingModelRequested);
	CStreaming::RequestModel(MI_FLAME, StreamingModelRequested);
	CStreaming::RequestModel(MI_SNIPER, StreamingModelRequested);
	CStreaming::LoadAllRequestedModels(false);
	FindPlayerPed(-1)->GiveWeapon(WEAPONTYPE_BRASSKNUCKLE, 1);
	FindPlayerPed(-1)->GiveWeapon(WEAPONTYPE_KNIFE, 1);
	FindPlayerPed(-1)->GiveWeapon(WEAPONTYPE_PISTOL, 100);
	FindPlayerPed(-1)->GiveWeapon(WEAPONTYPE_SHOTGUN, 50);
	FindPlayerPed(-1)->GiveWeapon(WEAPONTYPE_SKORPION, 150);
	FindPlayerPed(-1)->GiveWeapon(WEAPONTYPE_AK47, 120);
	FindPlayerPed(-1)->GiveWeapon(WEAPONTYPE_FLAMETHROWER, 25);
	FindPlayerPed(-1)->GiveWeapon(WEAPONTYPE_SNIPER, 25);
	CStreaming::SetModelIsDeletable(MI_BRASSKNUCKLE);
	CStreaming::SetModelIsDeletable(MI_KNIFECUR);
	CStreaming::SetModelIsDeletable(MI_BERETTA);
	CStreaming::SetModelIsDeletable(MI_CHROMEGUN);
	CStreaming::SetModelIsDeletable(MI_SKORPION);
	CStreaming::SetModelIsDeletable(MI_AK47);
	CStreaming::SetModelIsDeletable(MI_FLAME);
	CStreaming::SetModelIsDeletable(MI_SNIPER);
}

void
CCheat::WeaponCheat2(void)
{
	CStreaming::RequestModel(MI_KATANA, StreamingModelRequested);
	CStreaming::RequestModel(MI_TEARGAS, StreamingModelRequested);
	CStreaming::RequestModel(MI_PYTHON, StreamingModelRequested);
	CStreaming::RequestModel(MI_BUDDYSHOT, StreamingModelRequested);
	CStreaming::RequestModel(MI_INGRAMSL, StreamingModelRequested);
	CStreaming::RequestModel(MI_M16, StreamingModelRequested);
	CStreaming::RequestModel(MI_ROCKETLA, StreamingModelRequested);
	CStreaming::RequestModel(MI_LASER, StreamingModelRequested);
	CStreaming::LoadAllRequestedModels(false);
	FindPlayerPed(-1)->GiveWeapon(WEAPONTYPE_KATANA, 1);
	FindPlayerPed(-1)->GiveWeapon(WEAPONTYPE_TEARGAS, 10);	// since no remote grenades
	FindPlayerPed(-1)->GiveWeapon(WEAPONTYPE_PYTHON, 40);
	FindPlayerPed(-1)->GiveWeapon(WEAPONTYPE_STUBBY, 25);
	FindPlayerPed(-1)->GiveWeapon(WEAPONTYPE_INGRAMSL, 100);
	FindPlayerPed(-1)->GiveWeapon(WEAPONTYPE_M16, 150);
	FindPlayerPed(-1)->GiveWeapon(WEAPONTYPE_LASER, 21);
	CStreaming::SetModelIsDeletable(MI_KATANA);
	CStreaming::SetModelIsDeletable(MI_TEARGAS);
	CStreaming::SetModelIsDeletable(MI_PYTHON);
	CStreaming::SetModelIsDeletable(MI_BUDDYSHOT);
	CStreaming::SetModelIsDeletable(MI_INGRAMSL);
	CStreaming::SetModelIsDeletable(MI_M16);
	CStreaming::SetModelIsDeletable(MI_ROCKETLA);
	CStreaming::SetModelIsDeletable(MI_LASER);
}

void
CCheat::WeaponCheat3(void)
{
	CStreaming::RequestModel(MI_CHNSAW, StreamingModelRequested);
	CStreaming::RequestModel(MI_GRENADE, StreamingModelRequested);
	CStreaming::RequestModel(MI_PYTHON, StreamingModelRequested);
	CStreaming::RequestModel(MI_SHOTGSPA, StreamingModelRequested);
	CStreaming::RequestModel(MI_MPLNG, StreamingModelRequested);
	CStreaming::RequestModel(MI_M16, StreamingModelRequested);
	CStreaming::RequestModel(MI_MINIGUN, StreamingModelRequested);
	CStreaming::RequestModel(MI_LASER, StreamingModelRequested);
	CStreaming::LoadAllRequestedModels(false);
	FindPlayerPed(-1)->GiveWeapon(WEAPONTYPE_CHAINSAW, 1);
	FindPlayerPed(-1)->GiveWeapon(WEAPONTYPE_GRENADE, 10);
	FindPlayerPed(-1)->GiveWeapon(WEAPONTYPE_PYTHON, 40);
	FindPlayerPed(-1)->GiveWeapon(WEAPONTYPE_SPAS12_SHOTGUN, 30);
	FindPlayerPed(-1)->GiveWeapon(WEAPONTYPE_MP5, 100);
	FindPlayerPed(-1)->GiveWeapon(WEAPONTYPE_M16, 150);
	FindPlayerPed(-1)->GiveWeapon(WEAPONTYPE_MINIGUN, 500);
	FindPlayerPed(-1)->GiveWeapon(WEAPONTYPE_LASER, 21);
	CStreaming::SetModelIsDeletable(MI_CHNSAW);
	CStreaming::SetModelIsDeletable(MI_GRENADE);
	CStreaming::SetModelIsDeletable(MI_PYTHON);
	CStreaming::SetModelIsDeletable(MI_SHOTGSPA);
	CStreaming::SetModelIsDeletable(MI_MPLNG);
	CStreaming::SetModelIsDeletable(MI_M16);
	CStreaming::SetModelIsDeletable(MI_MINIGUN);
	CStreaming::SetModelIsDeletable(MI_LASER);
}

void CCheat::ArmourCheat(void) { FindPlayerPed(-1)->fArmour = CWorld::Players[0].GetMaxArmour(); }
void CCheat::MoneyCheat(void) { CWorld::Players[CWorld::PlayerInFocus].Score += 250000; }

void CCheat::BmxCheat(void) { VehicleCheat(481); }
void CCheat::ApacheCheat(void) { VehicleCheat(425); }
void CCheat::QuadCheat(void) { VehicleCheat(471); }
void CCheat::TankCheat(void) { VehicleCheat(432); }
void CCheat::SasquatchCheat(void) { VehicleCheat(444); }
void CCheat::GarbageTruckCheat(void) { VehicleCheat(408); }
void CCheat::InfernusCheat(void) { VehicleCheat(411); }
void CCheat::InsurrectionCheat(void) { VehicleCheat(477); }

void CCheat::SlowTimeCheat(void) { if(CTimer::ms_fTimeScale > 0.25f) CTimer::ms_fTimeScale *= 0.5f; }
void CCheat::FastTimeCheat(void) { if(CTimer::ms_fTimeScale < 4.0f) CTimer::ms_fTimeScale *= 2.0f; }

void CCheat::UltraSunnyWeatherCheat(void) { CWeather::ForceWeatherNow(CWeather::weatherMap[ULTRASUNNY_MIAMI]); }
void CCheat::ExtraSunnyWeatherCheat(void) { CWeather::ForceWeatherNow(CWeather::weatherMap[EXTRASUNNY_MIAMI]); }
void CCheat::SunnyWeatherCheat(void) { CWeather::ForceWeatherNow(CWeather::weatherMap[SUNNY_MIAMI]); }
void CCheat::CloudyWeatherCheat(void) { CWeather::ForceWeatherNow(CWeather::weatherMap[CLOUDY_MIAMI]); }
void CCheat::RainyWeatherCheat(void) { CWeather::ForceWeatherNow(CWeather::weatherMap[RAINY_MIAMI]); }
void CCheat::FoggyWeatherCheat(void) { CWeather::ForceWeatherNow(CWeather::weatherMap[FOGGY_MIAMI]); }


// SA cheats

WRAPPER void CCheat::HealthCheat(void) { EAXJMP(0x438D60); }
WRAPPER void CCheat::WantedLevelUpCheat(void) { EAXJMP(0x438E90); }
WRAPPER void CCheat::WantedLevelDownCheat(void) { EAXJMP(0x438F20); }
WRAPPER void CCheat::SuicideCheat(void) { EAXJMP(0x438FF0); }
WRAPPER void CCheat::BlowupCarsCheat(void) { EAXJMP(0x439D80); }
WRAPPER void CCheat::EverybodyAttacksPlayerCheat(void) { EAXJMP(0x439C70); }
WRAPPER void CCheat::MayhemCheat(void) { EAXJMP(0x439B20); }

void CCheat::BlackCarsCheat(void) {
		m_aCheatsActive[BLACKCARS] = !m_aCheatsActive[BLACKCARS];
		if(m_aCheatsActive[BLACKCARS])
			m_aCheatsActive[WHITECARS] = 0;
}
void CCheat::WhiteCarsCheat(void) {
		m_aCheatsActive[WHITECARS] = !m_aCheatsActive[WHITECARS];
		if(m_aCheatsActive[WHITECARS])
			m_aCheatsActive[BLACKCARS] = 0;
}
void CCheat::NoTractionCheat(void) { m_aCheatsActive_original[28] = !m_aCheatsActive_original[28]; }
void CCheat::BackToTheFutureCheat(void) { m_aCheatsActive_original[34] = !m_aCheatsActive_original[34]; }
void CCheat::OnlyWheelsCheat(void) { m_aCheatsActive_original[27] = !m_aCheatsActive_original[27]; }
void CCheat::GreenLightsCheat(void) { m_aCheatsActive_original[30] = !m_aCheatsActive_original[30]; }
void CCheat::MadDriverCheat(void) { m_aCheatsActive_original[31] = !m_aCheatsActive_original[31]; }
void CCheat::WeaponsForAllCheat(void) { m_aCheatsActive_original[16] = !m_aCheatsActive_original[16]; }


static StaticPatcher Patcher([](){
	Memory::InjectHook(0x438450, CCheat::ResetCheats, PATCH_JUMP);

	// patch black and white vehicle cheats
	Memory::Patch(0x4C8514 + 1, &CCheat::m_aCheatsActive[CCheat::BLACKCARS]);
	Memory::Patch(0x4C8521 + 1, &CCheat::m_aCheatsActive[CCheat::WHITECARS]);
	Memory::Patch<int8>(0x4C8537 + 2, 1);
	Memory::Patch<int8>(0x4C853F + 2, 1);
	Memory::Patch<int8>(0x4C8543 + 2, 1);
	Memory::Patch<int8>(0x4C8546 + 2, 1);

	Memory::Patch(0x52CF2C + 1, &CCheat::m_aCheatsActive[CCheat::FASTCLOCK]);
});
