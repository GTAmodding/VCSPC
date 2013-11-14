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
	unsigned short	m_wSlotSize;

public:
	inline int		GetSize()
						{ return m_nNumSlots; }
	inline bool		IsValid(unsigned int nIndex)
						{ return nIndex >= 0 && nIndex < m_nNumSlots && !m_pSlotInfos[nIndex].a.m_bFree }

	CBase*			GetAt(int nIdentifier)
	{
		int nSlotIndex = nIdentifier >> 8;
		if ( nSlotIndex >= 0 && nSlotIndex < m_nNumSlots && m_pSlotInfos[nSlotIndex].b == (nIdentifier & 0xFF) )
			return &m_pSlots[nSlotIndex];
		else
			return nullptr;
	}

	signed int		GetIndex(CBase* pObject)
	{
		return ((reinterpret_cast<CDerived*>(pObject) - m_pSlots) << 8) + m_pSlotInfos[reinterpret_cast<CDerived*>(pObject) - m_pSlots].b;
	}

	signed int		GetIndexOnly(CBase* pObject)
	{
		return reinterpret_cast<CDerived*>(pObject) - m_pSlots;
	}

	CBase*			New()
	{
		bool		bReachedTop = false;
		do
		{
			if ( ++m_nFirstFree == m_nNumSlots )
			{
				if ( bReachedTop )
					return nullptr;
				bReachedTop = true;
				m_nFirstFree = 0;
			}
		}
		while ( !m_pSlotInfos[m_nFirstFree].a.m_bFree );
		m_pSlotInfos[m_nFirstFree].a.m_bFree = false;
		++m_pSlotInfos[m_nFirstFree].a.m_uID;
		return &m_pSlots[m_nFirstFree];
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

class CPools
{
private:
	static CPedPool*&			ms_pPedPool;
	static CVehiclePool*&		ms_pVehiclePool;
	static CBuildingPool*&		ms_pBuildingPool;

public:
	static inline CPedPool*			GetPedPool()
		{ return ms_pPedPool; }
	static inline CVehiclePool*		GetVehiclePool()
		{ return ms_pVehiclePool; }
	static inline CBuildingPool*	GetBuildingPool()
		{ return ms_pBuildingPool; }
};

static_assert(sizeof(CPool<bool, bool>) == CPool_ARRAYSIZE, "CPool class has wrong size!");

#endif