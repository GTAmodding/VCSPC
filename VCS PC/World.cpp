#include "StdAfx.h"
#include "World.h"

#include "Building.h"

CPlayerInfo* const	CWorld::Players = (CPlayerInfo*)0xB7CD98;
unsigned char&		CWorld::PlayerInFocus = *(unsigned char*)0xB7CD74;

CSector* const		CWorld::ms_aSectors = (CSector*)0xB7D0B8;

WRAPPER void CWorld::Add(CEntity* pEntity) { WRAPARG(pEntity); EAXJMP(0x563220); }
WRAPPER void CWorld::Remove(CEntity* pEntity) { WRAPARG(pEntity); EAXJMP(0x563280); }

void CWorld::ClearForRestart()
{
	// Call SA ClearForRestart
	((void(*)())0x564360)();

	// Clear shadows from CBuildings
	for ( int i = 0; i < 14400; i++ )
	{
		for ( auto it = ms_aSectors[i].building.sNode; it; it = it->next )
			static_cast<CBuilding*>(it->entity)->ClearForRestart();
	}
}

static StaticPatcher	Patcher([](){ 
						Memory::InjectHook(0x53C5CB, CWorld::ClearForRestart);
									});