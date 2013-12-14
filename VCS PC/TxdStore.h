#ifndef __TXDSTORE
#define __TXDSTORE

class CTxdStore
{
public:
	static void			PushCurrentTxd();
	static void			PopCurrentTxd();
	static int			AddTxdSlot(const char* slotName);
	static int			FindTxdSlot(const char* name);
	static void			SetCurrentTxd(int slot);
	static void			AddRef(int index);
	static void			RemoveTxdSlot(int slot);
	static void			LoadTxd(int slot, const char* fp);
};

#endif