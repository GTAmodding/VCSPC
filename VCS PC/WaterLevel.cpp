#include "StdAfx.h"
#include "WaterLevel.h"

//WRAPPER void CWaterLevel::WaterLevelInitialise(void) { EAXJMP(0x6EAE80); }
WRAPPER bool CWaterLevel::GetWaterLevel(float x, float y, float z, float *level, char wave, CVector *w) { EAXJMP(0x6EB690); }
WRAPPER void CWaterLevel::PreRenderWater(void) { EAXJMP(0x6EB710); }
WRAPPER void CWaterLevel::RenderWaterFog(void) { EAXJMP(0x6E7760); }
WRAPPER void CWaterLevel::RenderWater(void) { EAXJMP(0x6EF650); }

void CWaterLevel::WaterLevelInitialise() {
    CFileMgr::SetDir("COMMON");
    ((char(__cdecl*)())0x6EAE80)();
    CFileMgr::SetDir("");
}

static StaticPatcher Patcher([](){
		// High detail water color multiplier is multiplied by 0.65 and added to 0.27, why?
		// Removing this silly calculation seems to work better.
		Memory::Nop(0x6E716B, 6);
		Memory::Nop(0x6E7176, 6);
});
