#ifndef __CLOUDS
#define __CLOUDS

class CClouds
{
public:
	static float ms_cameraRoll;
	static float ms_HorizonTilt;
	static float ms_horizonZ;
	static CRGBA ms_colourBottom;
	static CRGBA ms_colourTop;
	static CRGBA ms_colourBkGrd;

	static void Init(void);
	static void Update(void);
	static void MovingFogRender(void);
	static void VolumetricCloudsRender(void);
	static void Render(void);
	static void RenderBottomFromHeight(void);
	static void RenderSkyPolys(void);
	static void RenderBackground(uint16 topr, uint16 topg, uint16 topb, uint16 botr, uint16 botg, uint16 botb, uint8 alpha);
	static void RenderHorizon(void);
};


#endif
