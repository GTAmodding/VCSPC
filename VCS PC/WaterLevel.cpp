#include "StdAfx.h"
#include "WaterLevel.h"

WRAPPER void CWaterLevel::WaterLevelInitialise(void) { EAXJMP(0x6EAE80); }
WRAPPER bool CWaterLevel::GetWaterLevel(float x, float y, float z, float *level, char wave, CVector *w) { EAXJMP(0x6EB690); }
WRAPPER void CWaterLevel::PreRenderWater(void) { EAXJMP(0x6EB710); }
WRAPPER void CWaterLevel::RenderWaterFog(void) { EAXJMP(0x6E7760); }
WRAPPER void CWaterLevel::RenderWater(void) { EAXJMP(0x6EF650); }
