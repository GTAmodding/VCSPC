#ifndef __WORLD
#define __WORLD

#include "General.h"
#include "PlayerInfo.h"
#include "PtrList.h"

#define NUM_PLAYERS		2

class CColPoint
{
public:
	/* https://code.google.com/p/mtasa-blue/source/browse/tags/1.3.4/MTA10/game_sa/CColPointSA.h */
	CVector         m_vPoint;
	float field_C;
	CVector         m_vNormal;
	float field_1C;
	unsigned __int8 m_nSurfaceTypeA;
	unsigned __int8 m_nPieceTypeA;
	unsigned __int8 m_nLightingA;
	unsigned __int8 m_nSurfaceTypeB;
	unsigned __int8 m_nPieceTypeB;
	unsigned __int8 m_nLightingB;
	float           m_fDepth;

	void operator=(CColPoint const& right);
};

static_assert(sizeof(CColPoint) == 0x2C, "Wrong size: CColPoint");

class CSector
{
public:
	CPtrListSingleLink		building;
	CPtrListDoubleLink		dummy;
};


class CWorld
{
public:
	static CPlayerInfo* const	Players;
	static unsigned char&		PlayerInFocus;
	
	static CSector* const		ms_aSectors;

public:
	static void					Initialise(void);
	static void					Process(void);
	static void					Add(CEntity* pEntity);
	static void					Remove(CEntity* pEntity);
	static bool					ProcessLineOfSight(const CVector&, const CVector&, CColPoint&, CEntity*&, bool, bool, bool, bool, bool, bool, bool, bool);
	static double					FindGroundZFor3DCoord(float x, float y, float z, bool *b, CEntity **ent);

	static void					ClearForRestart();
};

#endif