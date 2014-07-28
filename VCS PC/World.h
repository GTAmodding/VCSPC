#ifndef __WORLD
#define __WORLD

#include "General.h"
#include "PlayerInfo.h"

#define NUM_PLAYERS		2;

class CPtrNodeSingle
{
public:
	CEntity*		entity;
	CPtrNodeSingle*	next;
};

class CPtrNodeDouble
{
public:
	CEntity*		entity;
	CPtrNodeDouble*	next;
	CPtrNodeDouble*	prev;
};

class CPtrList
{
public:
	union
	{
		CPtrNodeSingle* sNode;
		CPtrNodeDouble* dNode;
	};
};

class CSector
{
public:
	CPtrList		building;
	CPtrList		dummy;
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