#include "StdAfx.h"
#include "World.h"

#include "Building.h"

CPlayerInfo* const	CWorld::Players = (CPlayerInfo*)0xB7CD98;
unsigned char&		CWorld::PlayerInFocus = *(unsigned char*)0xB7CD74;

CSector* const		CWorld::ms_aSectors = (CSector*)0xB7D0B8;

WRAPPER void CWorld::Initialise(void) { EAXJMP(0x5631E0); }
WRAPPER void CWorld::Process(void) { EAXJMP(0x5684A0); }
WRAPPER void CWorld::Add(CEntity* pEntity) { WRAPARG(pEntity); EAXJMP(0x563220); }
WRAPPER void CWorld::Remove(CEntity* pEntity) { WRAPARG(pEntity); EAXJMP(0x563280); }
WRAPPER bool CWorld::ProcessLineOfSight(const CVector&, const CVector&, CColPoint&, CEntity*&, bool, bool, bool, bool, bool, bool, bool, bool)
										{ EAXJMP(0x56BA00); }
WRAPPER double CWorld::FindGroundZFor3DCoord(float x, float y, float z, bool *b, CEntity **ent) { EAXJMP(0x5696C0); }

void CWorld::ClearForRestart()
{
	// Call SA ClearForRestart
	((void(*)())0x564360)();

	// Clear shadows from CBuildings
	for ( int i = 0; i < 14400; i++ )
	{
		for ( auto it = ms_aSectors[i].building.m_psNodes; it; it = it->m_psNext )
			static_cast<CBuilding*>(it->m_pPtr)->ClearForRestart();
	}
}

static StaticPatcher	Patcher([](){ 
						Memory::InjectHook(0x53C5CB, CWorld::ClearForRestart);
									});