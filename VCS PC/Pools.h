#ifndef __POOLS
#define __POOLS

template <class T, class U = T>
class CPool
{
public:
	U*				m_pSlots;
	union			tSlotInfos
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
	bool			m_bOwnsAllocations;

public:
	inline U*			GetSlots()
							{ return m_pSlots; }
	inline bool			IsFree(signed int nIndex)
							{ return m_pSlotInfos[nIndex].a.m_bFree; }
	inline int			GetSize()
							{ return m_nNumSlots; }
	inline bool			IsValid(signed int nIndex)
							{ return nIndex >= 0 && nIndex < m_nNumSlots && !m_pSlotInfos[nIndex].a.m_bFree; }
	inline bool			IsHandleFullyValid(signed int nIdentifier)
							{	signed int nIndex = nIdentifier >> 8;
								return nIndex >= 0 && nIndex < m_nNumSlots && m_pSlotInfos[nIndex].b == (nIdentifier & 0xFF); }

	CPool()
	{}

	CPool(int nSize)
	{
		m_pSlots = static_cast<U*>(operator new(sizeof(U) * nSize));
		m_pSlotInfos = static_cast<tSlotInfos*>(operator new(sizeof(tSlotInfos) * nSize));

		m_nNumSlots = nSize;
		m_nFirstFree = -1;
		m_bOwnsAllocations = true;	

		for ( int i = 0; i < nSize; ++i )
		{
			m_pSlotInfos[i].a.m_bFree = true;
			m_pSlotInfos[i].a.m_uID = 0;
		}
	}

	~CPool()
	{
		Flush();
	}


	void			Init(int nSize, void* pObjects, void* pInfos)
	{
		m_pSlots = pObjects;
		m_pSlotInfos = pInfos;

		m_nNumSlots = nSize;
		m_nFirstFree = -1;
		m_bOwnsAllocations = false;

		for ( int i = 0; i < nSize; ++i )
		{
			m_pSlotInfos[i].a.m_bFree = true;
			m_pSlotInfos[i].a.m_uID = 0;
		}
	}


	void			Flush()
	{
		if ( m_nNumSlots > 0 )
		{
			if ( m_bOwnsAllocations )
			{
				operator delete(m_pSlots);
				operator delete(m_pSlotInfos);
			}

			m_pSlots = nullptr;
			m_pSlotInfos = nullptr;
			m_nNumSlots = 0;
			m_nFirstFree = 0;
		}
	}

	T*				GetAt(int nIdentifier)
	{
		int nSlotIndex = nIdentifier >> 8;
		return nSlotIndex >= 0 && nSlotIndex < m_nNumSlots && m_pSlotInfos[nSlotIndex].b == (nIdentifier & 0xFF) ? &m_pSlots[nSlotIndex] : nullptr;
	}

	T*				GetAtIndex(int nIndex)
	{
		return nIndex >= 0 && nIndex < m_nNumSlots && !m_pSlotInfos[nIndex].a.m_bFree ? &m_pSlots[nIndex] : nullptr;
	}

	signed int			GetHandle(T* pObject)
	{
		return ((reinterpret_cast<U*>(pObject) - m_pSlots) << 8) + m_pSlotInfos[reinterpret_cast<U*>(pObject) - m_pSlots].b;
	}

	signed int			GetIndex(T* pObject)
	{
		return reinterpret_cast<U*>(pObject) - m_pSlots;
	}

	T*					New()
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

	T*					New(int nHandle)
	{
		nHandle >>= 8;

		m_pSlotInfos[nHandle].a.m_bFree = false;
		++m_pSlotInfos[nHandle].a.m_uID;
		m_nFirstFree = 0;

		while ( !m_pSlotInfos[m_nFirstFree].a.m_bFree )
			++m_nFirstFree;

		return &m_pSlots[nHandle];
	}

	void				Delete(T* pObject)
	{
		int		nIndex = reinterpret_cast<U*>(pObject) - m_pSlots;
		m_pSlotInfos[nIndex].a.m_bFree = true;
		if ( nIndex < m_nFirstFree )
			m_nFirstFree = nIndex;
	}
};

template<class T, class Parent, class PPool>
class CChildrenPool
{
private:
	T*			m_pSlots;
	PPool*		m_pParentPool;

public:
	CChildrenPool(PPool* pParent)
		: m_pParentPool(pParent)
			{ m_pSlots = static_cast<T*>(operator new(sizeof(T) * pParent->GetSize())); }

	~CChildrenPool()
			{ operator delete(m_pSlots); }

	inline T*		GetAtIndex(int nIndex)
			{ return &m_pSlots[nIndex]; }

	inline T*		GetAtPointer(Parent* pParent)
			{ return &m_pSlots[m_pParentPool->GetIndex(pParent)]; }

	inline T*		GetAt(int nIdentifier)
			{ return m_pParentPool->IsHandleFullyValid(nIdentifier) ? GetAtIndex(nIdentifier >> 8) : nullptr; }
};


template <int PadSize, class T>
struct FakeClass : public T
{
	BYTE	pad[PadSize - sizeof(T)];
};

// Type definitions for specific pool types
#include "General.h"
#include "Ped.h"
#include "Vehicle.h"
#include "Building.h"
#include "ModelInfo.h"
#include "Tasks.h"

typedef CPool<CPed, FakeClass<0x7C4,CPed>>			CPedPool;
typedef CPool<CVehicle, FakeClass<0xA18,CVehicle>>	CVehiclePool;
typedef CPool<CBuilding>							CBuildingPool;
typedef CPool<CDummy>								CDummyPool;
typedef CPool<CColModel>							CColModelPool;
typedef CPool<CTask, FakeClass<128,CTask>>			CTaskPool;

typedef CPool<CEmpireBuildingData>					CEmpireBuildingDataPool;

typedef CChildrenPool<CPedEx,CPed,CPedPool>					CPedPoolEx;
typedef CChildrenPool<CBuildingEx,CBuilding,CBuildingPool>	CBuildingPoolEx;

class CPools
{
private:
	static CPedPool*&						ms_pPedPool;
	static CVehiclePool*&					ms_pVehiclePool;
	static CBuildingPool*&					ms_pBuildingPool;
	static CDummyPool*&						ms_pDummyPool;
	static CColModelPool*&					ms_pColModelPool;
	static CTaskPool*&						ms_pTaskPool;

	// VCS PC class extension
	static CEmpireBuildingDataPool*			ms_pEmpireBuildingDataPool;
	static CPedPoolEx*						ms_pPedPoolEx;
	static CBuildingPoolEx*				ms_pBuildingPoolEx;

public:
	static inline CPedPool*					GetPedPool()
		{ return ms_pPedPool; }
	static inline CVehiclePool*				GetVehiclePool()
		{ return ms_pVehiclePool; }
	static inline CBuildingPool*			GetBuildingPool()
		{ return ms_pBuildingPool; }
	static inline CDummyPool*				GetDummyPool()
		{ return ms_pDummyPool; }
	static inline CColModelPool*			GetColModelPool()
		{ return ms_pColModelPool; }
	static inline CTaskPool*				GetTaskPool()
		{ return ms_pTaskPool; }

	static inline CEmpireBuildingDataPool*	GetEmpireBuildingDataPool()
		{ return ms_pEmpireBuildingDataPool; }
	static inline CPedPoolEx*				GetPedPoolAux()
		{ return ms_pPedPoolEx; }
	static inline CBuildingPoolEx*			GetBuildingPoolAux()
		{ return ms_pBuildingPoolEx; }

	static void								Initialise();
	static void								ShutDown();
};

static_assert(sizeof(CPool<bool>) == 0x14, "Wrong size: CPool");

#endif