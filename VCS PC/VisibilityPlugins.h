#ifndef __VISIBILITYPLUGINS
#define __VISIBILITYPLUGINS

#include "LinkList.h"

class CVisibilityPlugins
{
public:
	struct AlphaObjectInfo
	{
		RpAtomic*	pAtomic;
		RpAtomic*	(*callback)(RpAtomic*, float);
		float		fCompareValue;

		friend bool operator < (const AlphaObjectInfo &a, const AlphaObjectInfo &b) 
		{ return a.fCompareValue < b.fCompareValue; }
	};

	static CLinkList<AlphaObjectInfo>&		m_alphaList;

public:
	static inline void						RenderOrderedList(CLinkList<AlphaObjectInfo>& list)
	{	for ( auto i = list.m_lnListTail.m_pPrev; i != &list.m_lnListHead; i = i->m_pPrev )
			i->V().callback(i->V().pAtomic, i->V().fCompareValue); }

	static void								RenderAlphaAtomics();
};

#endif