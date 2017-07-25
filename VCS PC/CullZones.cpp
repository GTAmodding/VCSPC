#include "StdAfx.h"
#include "CullZones.h"

WRAPPER bool CCullZones::PlayerNoRain(void) { EAXJMP(0x72DDC0); }
WRAPPER bool CCullZones::CamNoRain(void) { EAXJMP(0x72DDB0); }
