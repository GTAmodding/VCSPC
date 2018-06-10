#ifndef __WATERLEVEL
#define __WATERLEVEL

class CWaterLevel
{
public:
	static void WaterLevelInitialise();
	static bool GetWaterLevel(float x, float y, float z, float *level, char wave, CVector *w);
	static void PreRenderWater(void);
	static void RenderWaterFog(void);
	static void RenderWater(void);
};

#endif
