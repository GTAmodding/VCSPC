#include "StdAfx.h"

unsigned short CPad::TestFireButton()
{
	DWORD dwFunc = (DWORD)FUNC_CPad__TestFireButton;
	unsigned short wResult;
	_asm
	{
		mov		ecx, this
		call	dwFunc
		mov		wResult, ax
	}
	return wResult;
}

void CPad::UpdatePads()
{
	DWORD dwFunc = (DWORD)FUNC_CPad__UpdatePads;
	_asm call dwFunc
}