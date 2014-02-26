#ifndef __GROUPEDBUILDINGS_H
#define __GROUPEDBUILDINGS_H

#include "General.h"
#include "Building.h"

#define NUM_GROUPED_BUILDINGS			4
#define NUM_MAX_BUILDINGS_IN_GROUP		7

class CGroupedBuilding
{
private:
	unsigned int			m_nGroupNameHash;
	CBuilding*				m_pActualBuilding;
	CSimpleTransform		m_placement;
	int						m_nCurrentID;
	int						m_nModelsInGroup;
	int						m_nModelID[NUM_MAX_BUILDINGS_IN_GROUP];

public:
	inline unsigned int		GetHash()
			{ return m_nGroupNameHash; }
	inline CBuilding*		GetBuilding()
			{ return m_pActualBuilding; }

	void					RegisterOne(const char* pGroupName, const CVector& vecPos, float fAngle, int nNumObjs,
								const char* pModel1, const char* pModel2, const char* pModel3, const char* pModel4,
								const char* pModel5, const char* pModel6, const char* pModel7);
	void					SetActiveBuilding(int nIndex);
};

class CGroupedBuildings
{
private:
	static int							m_nGroupsCount;
	static CGroupedBuilding				m_groups[NUM_GROUPED_BUILDINGS];

public:
	static inline int					GetNumGroups()
			{ return m_nGroupsCount; }
	static inline CGroupedBuilding*		GetGroup(int nIndex)
			{ assert(nIndex >= 0 && nIndex < NUM_GROUPED_BUILDINGS); return &m_groups[nIndex]; }

	static void							Initialise();
};

#endif