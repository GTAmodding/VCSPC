#ifndef __LIGHTING
#define __LIGHTING

struct GlobalScene
{
	RpWorld *world;
	RwCamera *camera;
};
extern GlobalScene &Scene;

extern RpLight *&pAmbient;
extern RpLight *&pDirect;
extern RpLight **pExtraDirectionals;
extern int &NumExtraDirLightsInWorld;
extern RwRGBAReal& AmbientLightColourForFrame;

void SetLightsWithTimeOfDayColour(RpWorld*);
void SetBrightMarkerColours(void);
void ReSetAmbientAndDirectionalColours(void);
void SetAmbientColours(void);
void SetAmbientColours(RwRGBAReal *color);
void SetDirectionalColours(void);
void SetDirectionalColours(RwRGBAReal *color);
void ActivateDirectional(void);
void DeActivateDirectional();

void RenderScene(void);
void RenderReflectionScene(void);
void RenderEffects(void);

#endif __LIGHTING
