#include "StdAfx.h"
#include "World.h"

#include "Pools.h"

CPlayerInfo* const	CWorld::Players = (CPlayerInfo*)0xB7CD98;
unsigned char&		CWorld::PlayerInFocus = *(unsigned char*)0xB7CD74;

WRAPPER void CWorld::Add(CEntity* pEntity) { WRAPARG(pEntity); EAXJMP(0x563220); }
WRAPPER void CWorld::Remove(CEntity* pEntity) { WRAPARG(pEntity); EAXJMP(0x563280); }

void CWorld::ClearForRestart()
{
	// Clear shadows from CBuildings

	// TODO: Do it via sectors (Bas?)
	CBuildingPool*		pPool = CPools::GetBuildingPool();

	if ( pPool )
	{
		int		nCount = pPool->GetSize();
		for ( int i = 0; i < nCount; i++ )
		{
			if ( !pPool->IsFree(i) )
				pPool->GetAtIndex(i)->ClearForRestart();
		}
	}

	// Call SA ClearForRestart
	((void(*)())0x564360)();
}

static StaticPatcher	Patcher([](){ 
						Memory::InjectHook(0x53C5CB, CWorld::ClearForRestart);
									});