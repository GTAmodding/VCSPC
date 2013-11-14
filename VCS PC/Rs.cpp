#include "stdafx.h"

RsEventStatus RsEventHandler(RsEvent eventID, void* param)
{
	DWORD	dwFunc = FUNC_RsEventHandler;
	RsEventStatus	bResult;
	_asm
	{
		push	param
		push	eventID
		call	dwFunc
		add		esp, 8
		mov		bResult, eax
	}
	return bResult;
}

bool RsCameraBeginUpdate()
{
	DWORD	dwFunc = (DWORD)FUNC_RsCameraBeginUpdate;
	bool	bResult;
	_asm
	{
		call	dwFunc
		mov		bResult, al
	}
	return bResult;
}

BOOL PsInitialize()
{
	DWORD	dwFunc = (DWORD)FUNC_PsInitialize;
	BOOL	bResult;
	_asm
	{
		call	dwFunc
		mov		bResult, eax
	}
	return bResult;
}

RwChar* RsPathnameCreate(const RwChar* srcBuffer)
{
	DWORD		dwFunc = FUNC_RsPathnameCreate;
	RwChar*		pResult;
	_asm
	{
		push	srcBuffer
		call	dwFunc
		mov		pResult, eax
		add		esp, 4
	}
	return pResult;
}

void RsPathnameDestroy(RwChar* buffer)
{
	DWORD		dwFunc = FUNC_RsPathnameDestroy;
	_asm
	{
		push	buffer
		call	dwFunc
		add		esp, 4
	}
}