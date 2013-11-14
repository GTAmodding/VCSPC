#include "StdAfx.h"

void CTxdStore::PushCurrentTxd()
{
	DWORD dwFunc = (DWORD)FUNC_CTxdStore__PushCurrentTxd;
	_asm call dwFunc
}

void CTxdStore::PopCurrentTxd()
{
	DWORD dwFunc = (DWORD)FUNC_CTxdStore__PopCurrentTxd;
	_asm call dwFunc
}

int CTxdStore::FindTxdSlot(const char *name)
{
	DWORD dwFunc = (DWORD)FUNC_CTxdStore__FindTxdSlot;
	_asm
	{
		push	name
		call	dwFunc
		add		esp, 4
	}
}

void CTxdStore::SetCurrentTxd(int slot)
{
	DWORD dwFunc = (DWORD)FUNC_CTxdStore__SetCurrentTxd;
	_asm
	{
		push	slot
		call	dwFunc
		add		esp, 4
	}
}

void CTxdStore::AddRef(int index)
{
	DWORD dwFunc = (DWORD)FUNC_CTxdStore__AddTxdRef;
	_asm
	{
		push	index
		call	dwFunc
		add		esp, 4
	}
}

int CTxdStore::AddTxdSlot(const char* slotName)
{
	DWORD dwFunc = (DWORD)FUNC_CTxdStore__AddTxdSlot;
	_asm
	{
		push	slotName
		call	dwFunc
		add		esp, 4
	}
}

void CTxdStore::RemoveTxdSlot(int slot)
{
	DWORD dwFunc = (DWORD)FUNC_CTxdStore__RemoveTxdSlot;
	_asm
	{
		push	slot
		call	dwFunc
		add		esp, 4
	}
}

void CTxdStore::LoadTxd(int slot, const char* fp)
{
	DWORD dwFunc = (DWORD)FUNC_CTxdStore__LoadTxd;
	_asm
	{
		push	fp
		push	slot
		call	dwFunc
		add		esp, 8
	}
}