#ifndef __EMPIREMGR
#define __EMPIREMGR

#include "General.h"
#include "Building.h"

#define NUM_EMPIRES				30
#define NUM_EMPIRE_TYPES		7		// Including Undeveloped

struct tEmpireType
{
	int					nNormalBuilding[3], nDamagedBuilding[3], nAdditionalModel[3][3];
};

/*struct tEmpireBuildingDataEntry
{
	CSimpleTransform		Placement;
	char					Subgroup;
};

struct tEmpireBuildingDataSection
{
	std::vector<tEmpireBuildingDataEntry>	Entry;
	std::map<char,unsigned int>				CountEntriesBySubgroup;
};*/

class CEmpireBuildingData
{
private:
	std::map<unsigned int,CSimpleTransform>				m_buildingData;

public:
	void*						operator new(size_t size);
	void						operator delete(void* ptr);

	CEmpireBuildingData()
	{}

	void						AddEntry(unsigned short nTypeID, char nSubgroup, unsigned char nIndex, const CSimpleTransform& vecPos);
	int							GetNumEntriesOfType(unsigned short nType);
	int							GetNumEntriesOfType(unsigned short nType, char nSubgroup);
	CSimpleTransform&			GetData(unsigned short nType, unsigned char nIndex);
	CSimpleTransform&			GetData(unsigned short nType, char nSubgroup, unsigned char nIndex);

	void						ReduceContainerSize();
};

class CEmpire
{
private:
	CSimpleTransform			m_placement;
	bool						m_bPlaced;
	bool						m_bDefined;
	int							m_nType, m_nScale;
	CBuilding*					m_pBuilding[3];

public:
	CEmpire()
		: m_bPlaced(false), m_bDefined(false), m_nType(-1), m_nScale(0)
	{ memset(m_pBuilding, 0, sizeof(m_pBuilding)); }

	inline CSimpleTransform*	GetTransform()
		{ return &m_placement; }
	inline bool					IsPlaced()
		{ return m_bPlaced; }
	inline bool					IsDefined()
		{ return m_bDefined; }
	inline CBuilding*			GetBuilding(int nIndex=0)
		{ return m_pBuilding[nIndex]; }

	inline void					MarkAsDefined()
		{ m_bDefined = true; }
	inline void					SetType(int nType)
		{ m_nType = nType; }
	inline void					SetScale(int nScale)
		{ m_nScale = nScale; }
	inline void					SetTransform(const CSimpleTransform& placement)
		{ m_placement.m_translate = placement.m_translate; m_placement.m_heading = placement.m_heading; }

	void						Place();
	void						Remove();
};

class CEmpireManager
{
private:
	static CEmpire				m_empire[NUM_EMPIRES];
	static tEmpireType			m_empireType[NUM_EMPIRE_TYPES];

public:
	static inline tEmpireType*	GetInfo(int nIndex)
		{ return &m_empireType[nIndex]; }
	static inline CEmpire*		GetEmpire(int nIndex)
		{	assert(nIndex >= 0 && nIndex < NUM_EMPIRES);
			return &m_empire[nIndex]; }

	static void					Initialise();
	static void					Process();
};

#endif