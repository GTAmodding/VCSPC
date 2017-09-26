#ifndef __WEATHER_H
#define __WEATHER_H

enum WeatherType {
	SUNNY_SF = 5,
	EXTRASUNNY_SF = 6,
	CLOUDY_SF = 7,
	RAINY_SF = 8,
	FOGGY_SF = 9,
	EXTRASUNNY_COUNTRYSIDE = 13,
	RAINY_COUNTRYSIDE = 16,
	EXTRACOLORS_SA = 21,

	SUNNY_MIAMI = 0,
	CLOUDY_MIAMI = 1,
	RAINY_MIAMI = 2,
	FOGGY_MIAMI = 3,
	EXTRASUNNY_MIAMI = 4,
	HURRICANE_MIAMI = 5,
	EXTRACOLORS_MIAMI = 6,
	ULTRASUNNY_MIAMI = 7,
};

class CWeather
{
public:
	static float&		Foggyness;
	static int16 &OldWeatherType;
	static int16 &NewWeatherType;
	static float &InterpolationValue;
	static float &UnderWaterness;
	static float &InTunnelness;
	static bool &bScriptsForceRain;
	static int16 &WeatherRegion;
	static bool &LightningFlash;

	static int weatherMap[8];
	static int revWeatherMap[23];

	static void UpdateWeatherRegion(CVector*);
	static uint8 *FindWeatherTypesList(void);
};

#endif