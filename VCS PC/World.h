#ifndef __WORLD
#define __WORLD

#include "General.h"
#include "PlayerInfo.h"

#define NUM_PLAYERS		2;

class CWorld
{
public:
	static CPlayerInfo* const	Players;
	static unsigned char&		PlayerInFocus;

public:
	static void					Add(CEntity* pEntity);
	static void					Remove(CEntity* pEntity);

	static void					ClearForRestart();
};

#endif