#include "StdAfx.h"
#include "TxdStore.h"


CPool<CTxdEntry>*&		CTxdStore::ms_pTxdPool = *(CPool<CTxdEntry>**)0xC8800C;

WRAPPER void CTxdStore::PushCurrentTxd() { EAXJMP(0x7316A0); }
WRAPPER void CTxdStore::PopCurrentTxd() { EAXJMP(0x7316B0); }
WRAPPER int CTxdStore::FindTxdSlot(const char* name) { WRAPARG(name); EAXJMP(0x731850); }
WRAPPER void CTxdStore::SetCurrentTxd(int slot) { WRAPARG(slot); EAXJMP(0x7319C0); }
WRAPPER void CTxdStore::AddRef(int index) { WRAPARG(index); EAXJMP(0x731A00); }
WRAPPER int CTxdStore::AddTxdSlot(const char* slotName) { WRAPARG(slotName); EAXJMP(0x731C80); }
WRAPPER void CTxdStore::RemoveTxdSlot(int slot) { WRAPARG(slot); EAXJMP(0x731CD0); }
WRAPPER void CTxdStore::LoadTxd(int slot, const char* fp) { WRAPARG(slot); WRAPARG(fp); EAXJMP(0x7320B0); }