#ifndef __EMPIREMGR
#define __EMPIREMGR

#define NUM_EMPIRES				30
#define NUM_EMPIRE_TYPES		7		// Including Undeveloped

class CBuilding;

struct tEmpireType
{
	int					nNormalBuilding[3], nDamagedBuilding[3];
};

class CEmpireBuildingData
{
private:
	std::map<unsigned short,std::vector<CSimpleTransform>>		m_buildingData;

public:
	inline void					AddEntry(unsigned short nType, const CSimpleTransform& vecPos)
		{ m_buildingData[nType].push_back(vecPos); }

	void						ReduceContainerSize();
};

class CEmpire
{
private:
	CSimpleTransform			m_placement;
	bool						m_bPlaced;
	bool						m_bDefined;
	int							m_nType, m_nScale;
	CBuilding*					m_pBuilding;

public:
	CEmpire()
		: m_bPlaced(false), m_bDefined(false), m_pBuilding(nullptr), m_nType(-1), m_nScale(0)
	{}

	inline CSimpleTransform*	GetTransform()
		{ return &m_placement; }
	inline bool					IsPlaced()
		{ return m_bPlaced; }
	inline bool					IsDefined()
		{ return m_bDefined; }

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