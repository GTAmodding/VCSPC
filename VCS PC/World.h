#ifndef __WORLD
#define __WORLD

#include "General.h"
#include "PlayerInfo.h"
#include "PtrList.h"

#define NUM_PLAYERS		2;

class CSector
{
public:
	CPtrListSingleLink		building;
	CPtrListSingleLink		dummy;
};


class CWorld
{
public:
	static CPlayerInfo* const	Players;
	static unsigned char&		PlayerInFocus;
	
	static CSector* const		ms_aSectors;

public:
	static void					Add(CEntity* pEntity);
	static void					Remove(CEntity* pEntity);

	static void					ClearForRestart();
};

#endif