#ifndef __GTAMAIN
#define __GTAMAIN

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

void LoadingScreen(const char *str1, const char *str2, const char *unused);

extern void (*DebugMenuProcess)(void);
extern void (*DebugMenuRender)(void);
void DebugMenuInit(void);

void DefinedState(void);

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
void Render2dStuff(void);

#endif __GTAMAIN
