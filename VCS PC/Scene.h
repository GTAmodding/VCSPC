#ifndef __LIGHTING
#define __LIGHTING

extern RpLight *&pAmbient;
extern RpLight *&pDirect;
extern RpLight **pExtraDirectionals;
extern int &NumExtraDirLightsInWorld;
extern RwRGBAReal& AmbientLightColourForFrame;

void SetLightsWithTimeOfDayColour(RpWorld*);
void SetBrightMarkerColours(void);
void ReSetAmbientAndDirectionalColours(void);
void SetAmbientColours(RwRGBAReal *color);
void SetDirectionalColours(RwRGBAReal *color);
void ActivateDirectional(void);
void DeActivateDirectional();

#endif __LIGHTING
