#include "StdAfx.h"

CPlayerInfo* const	CWorld::Players = (CPlayerInfo*)0xB7CD98;
unsigned char&		CWorld::PlayerInFocus = *(unsigned char*)0xB7CD74;

WRAPPER void CWorld::Add(CEntity* pEntity) { WRAPARG(pEntity); EAXJMP(0x563220); }
WRAPPER void CWorld::Remove(CEntity* pEntity) { WRAPARG(pEntity); EAXJMP(0x563280); }