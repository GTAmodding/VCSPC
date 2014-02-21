#ifndef __POOLS
#define __POOLS

template <class CBase, class CDerived = CBase>
class CPool
{
private:
	CDerived*		m_pSlots;
	union
	{
		struct
		{
			unsigned char	m_uID	: 7;
			bool			m_bFree	: 1;
		}				a;
		signed char		b;
	}*				m_pSlotInfos;
	int				m_nNumSlots;
	int				m_nFirstFree;
	bool			m_bInitialised;

public:
	inline CDerived*	GetSlots()
							{ return m_pSlots; }
	inline bool			IsFree(unsigned int nIndex)
							{ return m_pSlotInfos[nIndex].a.m_bFree; }
	inline int			GetSize()
							{ return m_nNumSlots; }
	inline bool			IsValid(unsigned int nIndex)
							{ return nIndex < m_nNumSlots && !m_pSlotInfos[nIndex].a.m_bFree }

	CBase*				GetAt(int nIdentifier)
	{
		int nSlotIndex = nIdentifier >> 8;
		return nSlotIndex >= 0 && nSlotIndex < m_nNumSlots && m_pSlotInfos[nSlotIndex].b == (nIdentifier & 0xFF) ? &m_pSlots[nSlotIndex] : nullptr;
	}

	CBase*				GetAtIndex(int nIndex)
	{
		return nIndex >= 0 && nIndex < m_nNumSlots && !m_pSlotInfos[nIndex].a.m_bFree ? &m_pSlots[nIndex] : nullptr;
	}

	signed int			GetHandle(CBase* pObject)
	{
		return ((reinterpret_cast<CDerived*>(pObject) - m_pSlots) << 8) + m_pSlotInfos[reinterpret_cast<CDerived*>(pObject) - m_pSlots].b;
	}

	signed int			GetIndex(CBase* pObject)
	{
		return reinterpret_cast<CDerived*>(pObject) - m_pSlots;
	}

	CBase*				New()
	{
		bool		bReachedTop = false;
		do
		{
			if ( ++m_nFirstFree >= m_nNumSlots )
			{
				if ( bReachedTop )
				{
					m_nFirstFree = -1;
					return nullptr;
				}
				bReachedTop = true;
				m_nFirstFree = 0;
			}
		}
		while ( !m_pSlotInfos[m_nFirstFree].a.m_bFree );
		m_pSlotInfos[m_nFirstFree].a.m_bFree = false;
		++m_pSlotInfos[m_nFirstFree].a.m_uID;
		return &m_pSlots[m_nFirstFree];
	}

	void				Delete(CBase* pObject)
	{
		int		nIndex = reinterpret_cast<CDerived*>(pObject) - m_pSlots;
		m_pSlotInfos[nIndex].a.m_bFree = true;
		if ( nIndex < m_nFirstFree )
			m_nFirstFree = nIndex;
	}
};

template <int PadSize, class CBase>
struct FakeClass : public CBase
{
	BYTE	pad[PadSize - sizeof(CBase)];
};

// Type definitions for specific pool types
typedef CPool<CPed, FakeClass<0x7C4,CPed>>			CPedPool;
typedef CPool<CVehicle, FakeClass<0xA18,CVehicle>>	CVehiclePool;
typedef CPool<CBuilding>							CBuildingPool;
typedef CPool<CDummy>								CDummyPool;
typedef CPool<CColModel>							CColModelPool;

class CPools
{
private:
	static CPedPool*&			ms_pPedPool;
	static CVehiclePool*&		ms_pVehiclePool;
	static CBuildingPool*&		ms_pBuildingPool;
	static CDummyPool*&			ms_pDummyPool;
	static CColModelPool*&		ms_pColModelPool;

public:
	static inline CPedPool*			GetPedPool()
		{ return ms_pPedPool; }
	static inline CVehiclePool*		GetVehiclePool()
		{ return ms_pVehiclePool; }
	static inline CBuildingPool*	GetBuildingPool()
		{ return ms_pBuildingPool; }
	static inline CDummyPool*		GetDummyPool()
		{ return ms_pDummyPool; }
	static inline CColModelPool*	GetColModelPool()
		{ return ms_pColModelPool; }
};

static_assert(sizeof(CPool<bool, bool>) == 0x14, "Wrong size: CPool");

#endif