#ifndef __VISIBILITYPLUGINS
#define __VISIBILITYPLUGINS

#include "LinkList.h"

class CPed;

class CVisibilityPlugins
{
private:
	static int32 &ms_atomicPluginOffset;
	static int32 &ms_framePluginOffset;
	static int32 &ms_clumpPluginOffset;

public:
	typedef void	(*VisibilityCallback)(void*, float);
	struct AlphaObjectInfo
	{
		void*	pObject;
		VisibilityCallback callback;
		float	fCompareValue;

		friend bool operator < (const AlphaObjectInfo &a, const AlphaObjectInfo &b) 
		{ return a.fCompareValue < b.fCompareValue; }
	};

	static CLinkList<AlphaObjectInfo>&		m_alphaList;
	static CLinkList<AlphaObjectInfo>&		m_alphaBoatAtomicList;
	static CLinkList<AlphaObjectInfo>&		m_alphaEntityList;
	static CLinkList<AlphaObjectInfo>&		m_alphaUnderwaterEntityList;
	static CLinkList<AlphaObjectInfo>&		m_alphaReallyDrawLastList;
	static CLinkList<CPed*>&				ms_weaponPedsForPC;

public:
	static inline void						RenderOrderedList(CLinkList<AlphaObjectInfo>& list)
	{	for ( auto i = list.m_lnListTail.m_pPrev; i != &list.m_lnListHead; i = i->m_pPrev )
			i->V().callback(i->V().pObject, i->V().fCompareValue); }
	static inline void						RenderOrderedListBuildings(CLinkList<AlphaObjectInfo>& list)
	{	for ( auto i = list.m_lnListTail.m_pPrev; i != &list.m_lnListHead; i = i->m_pPrev )
			if(((CEntity*)i->V().pObject)->nType == ENTITY_TYPE_BUILDING)
				i->V().callback(i->V().pObject, i->V().fCompareValue);
	}

	static void InitAlphaAtomicList(void);
	static bool InsertEntityIntoSortedList(CEntity *ent, float sort);
	static bool InsertEntityIntoUnderwaterList(CEntity *ent, float sort);
	static void RenderEntity(CEntity *e, float dist);
	static void RenderAlphaAtomics();
	static void RenderWeaponPedsForPC();
	static void RenderBoatAlphaAtomics(void);
	static void RenderFadingEntities(void);
	static void RenderFadingUnderwaterEntities(void);
	static void RenderAllFadingBuildings(void);
	static void RenderReallyDrawLastObjects(void);
	static void SetupVehicleVariables(RpClump *clump);
	static uint16								GetAtomicId(RpAtomic*);
	static int32 GetClumpAlpha(RpClump *clump);
};

#endif