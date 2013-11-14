#include "StdAfx.h"

WRAPPER void CWorld::Add(CEntity* pEntity) { WRAPARG(pEntity); EAXJMP(0x563220); }
WRAPPER void CWorld::Remove(CEntity* pEntity) { WRAPARG(pEntity); EAXJMP(0x563280); }