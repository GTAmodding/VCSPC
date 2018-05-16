#include "StdAfx.h"
#include "Weather.h"

float& CWeather::Wind = *(float*)0xC812F0;
float& CWeather::Foggyness = *(float*)0xC81300;
int16 &CWeather::OldWeatherType = *(short*)0xC81320;
int16 &CWeather::NewWeatherType = *(short*)0xC8131C;
float &CWeather::InterpolationValue = *(float*)0xC8130C;
float &CWeather::UnderWaterness = *(float*)0xC8132C;
float &CWeather::InTunnelness = *(float*)0xC81334;
bool &CWeather::bScriptsForceRain = *(bool*)0xC812AC;
int16 &CWeather::WeatherRegion = *(short*)0xC81314;
bool &CWeather::LightningFlash = *(bool*)0xC812CC;

// Map from VCS to SA weather
int CWeather::weatherMap[8] = { SUNNY_SF, CLOUDY_SF, RAINY_SF, FOGGY_SF, EXTRASUNNY_SF, RAINY_COUNTRYSIDE, EXTRACOLORS_SA, EXTRASUNNY_COUNTRYSIDE };
// Map from SA to VCS weather
int CWeather::revWeatherMap[23] = {
	EXTRASUNNY_MIAMI,	// LA
	SUNNY_MIAMI,		// LA
	EXTRASUNNY_MIAMI,	// SMOG LA
	SUNNY_MIAMI,		// SMOG LA
	CLOUDY_MIAMI,		// LA
	SUNNY_MIAMI,		//
	EXTRASUNNY_MIAMI,	//
	CLOUDY_MIAMI,		//
	RAINY_MIAMI,		//
	FOGGY_MIAMI,		//
	SUNNY_MIAMI,		// VEGAS
	EXTRASUNNY_MIAMI,	// VEGAS
	CLOUDY_MIAMI,		// VEGAS
	ULTRASUNNY_MIAMI,	//
	SUNNY_MIAMI,		// COUNTRYSIDE
	CLOUDY_MIAMI,		// COUNTRYSIDE
	HURRICANE_MIAMI,	//
	EXTRASUNNY_MIAMI,	// DESERT
	SUNNY_MIAMI,		// DESERT
	HURRICANE_MIAMI,	// SANDSTORM
	SUNNY_MIAMI,		// UNDERWATER
	EXTRACOLORS_MIAMI,	//
	EXTRACOLORS_MIAMI,	// 2
};

// straight out of VCS, mapped to SA weathers by the above map
static uint8 weatherTypesList[256] = {
	1, 7, 7, 4, 4, 4, 2, 7, 7, 4, 7, 2, 7, 0, 7, 7, 4, 1, 2, 1, 1, 0, 0, 4, 7, 7, 7, 3, 1, 3, 7, 7, 7, 1, 7, 4, 4, 0, 7, 7, 7, 0, 7, 7, 7, 7, 3, 7, 7, 7, 4, 0, 7, 7, 7, 7, 3, 1, 3, 7, 7, 7, 4, 4,
	0, 7, 7, 4, 4, 4, 1, 7, 7, 3, 7, 7, 7, 0, 7, 7, 4, 4, 4, 4, 4, 7, 0, 7, 7, 7, 7, 3, 3, 3, 7, 1, 2, 1, 7, 4, 4, 0, 1, 1, 2, 1, 7, 7, 7, 7, 3, 1, 7, 7, 4, 0, 1, 7, 4, 7, 3, 3, 3, 7, 7, 7, 4, 4,
	0, 1, 2, 1, 4, 4, 7, 7, 7, 3, 7, 7, 7, 7, 7, 7, 0, 4, 4, 4, 4, 0, 7, 7, 7, 7, 7, 3, 3, 3, 7, 7, 7, 7, 7, 4, 4, 7, 7, 0, 7, 7, 1, 2, 2, 1, 3, 7, 7, 0, 0, 4, 7, 7, 7, 7, 1, 3, 3, 0, 7, 7, 4, 4,
	7, 7, 7, 4, 4, 4, 7, 7, 7, 3, 7, 7, 7, 7, 7, 7, 4, 4, 4, 4, 4, 7, 7, 7, 7, 7, 7, 3, 3, 3, 7, 7, 7, 7, 7, 4, 4, 7, 7, 7, 7, 7, 7, 7, 7, 7, 3, 7, 7, 7, 4, 4, 0, 2, 2, 1, 2, 5, 2, 2, 1, 1, 0, 0
};

WRAPPER void CWeather::Init(void) { EAXJMP(0x72A480); }
WRAPPER void CWeather::Update(void) { EAXJMP(0x72B850); }
WRAPPER void CWeather::RenderRainStreaks(void) { EAXJMP(0x72AF70); }

void
CWeather::UpdateWeatherRegion(CVector*)
{
	CWeather::WeatherRegion = 0;
}

uint8*
CWeather::FindWeatherTypesList(void)
{

	return weatherTypesList;
}

static StaticPatcher	Patcher([](){
					for(int i = 0; i < 256; i++)
						weatherTypesList[i] = CWeather::weatherMap[weatherTypesList[i]];
					Memory::InjectHook(0x72B916, CWeather::FindWeatherTypesList);
					Memory::InjectHook(0x72A640, CWeather::UpdateWeatherRegion, PATCH_JUMP);
					// 256 entries in weather type list
					Memory::Patch<uint32>(0x72B903 +2, 0x8000007F);
				});
