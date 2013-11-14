#ifndef __CTXDSTORE
#define __CTXDSTORE

#define FUNC_CTxdStore__PushCurrentTxd						0x7316A0
#define FUNC_CTxdStore__PopCurrentTxd						0x7316B0
#define FUNC_CTxdStore__FindTxdSlot							0x731850
#define FUNC_CTxdStore__SetCurrentTxd						0x7319C0
#define FUNC_CTxdStore__AddTxdRef							0x731A00
#define FUNC_CTxdStore__AddTxdSlot							0x731C80
#define FUNC_CTxdStore__RemoveTxdSlot						0x731CD0
#define FUNC_CTxdStore__LoadTxd								0x7320B0

class CTxdStore
{
public:
	static void			PushCurrentTxd();
	static void			PopCurrentTxd();
	static int			FindTxdSlot(const char *name);
	static void			SetCurrentTxd(int slot);
	static void			AddRef(int index);
	static int			AddTxdSlot(const char* slotName);
	static void			RemoveTxdSlot(int slot);
	static void			LoadTxd(int slot, const char* fp);
};

#endif