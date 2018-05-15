#ifndef __TXDSTORE
#define __TXDSTORE

#include "Pools.h"

class CTxdEntry
{
public:
	RwTexDictionary*	m_pDictionary;
	unsigned short		m_wUsageCount;
	signed short		m_wParentIndex;
	unsigned int		m_nHash;

public:
	inline void			SetParent(signed short nIndex)
		{ m_wParentIndex = nIndex; }
};

class CTxdStore
{
private:
	static CPool<CTxdEntry>*&		ms_pTxdPool;

public:
	static void			PushCurrentTxd();
	static void			PopCurrentTxd();
	static int			AddTxdSlot(const char* pFileName);
	static int			FindTxdSlot(const char* pFileName);
	static void			SetCurrentTxd(int slot);
	static void			Create(int index);
	static void			AddRef(int index);
	static void			RemoveTxdSlot(int slot);
	static void			LoadTxd(int slot, const char* fp);

	static inline int	GetTxdSlot(const char* pFileName)
		{ int	nSlot = FindTxdSlot(pFileName); return nSlot == -1 ? AddTxdSlot(pFileName) : nSlot; }
	static inline CPool<CTxdEntry>*		GetPool()
		{ return ms_pTxdPool; }
};


#endif