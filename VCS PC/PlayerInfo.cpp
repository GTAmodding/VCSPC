#include "StdAfx.h"

// Wrappers
WRAPPER void CPlayerInfo::KillPlayer() { EAXJMP(0x56E580); }
WRAPPER void CPlayerInfo::ArrestPlayer() { EAXJMP(0x56E5D0); }

CWanted* FindPlayerWanted(signed int nPlayerID)
{
	if ( nPlayerID < 0 )
		return CWorld::Players[CWorld::PlayerInFocus].GetPlayerData().m_Wanted;

	return CWorld::Players[nPlayerID].GetPlayerData().m_Wanted;
}