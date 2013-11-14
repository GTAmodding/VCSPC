#include "StdAfx.h"

DWORD CPedIntelligence::GetSwimTask()
{
	DWORD	dwFunc = (DWORD)FUNC_CPedIntelligence__GetSwimTask;
	DWORD	dwResult;
	_asm
	{
		mov		ecx, this
		call	dwFunc
		mov		dwResult, eax
	}
	return dwResult;
}