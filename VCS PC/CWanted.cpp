#include "StdAfx.h"

BOOL CWanted::ShouldSendViceSquad()
{
	return m_nWantedLevel >= 3; //|| this->flags & m_viceSquadRequired;
}

CWanted* CWanted::GetPlayerCWanted(int playerID)
{
	DWORD dwFunc = 0x56E230;
	CWanted* dwReturn;

	_asm
	{
		mov		eax, playerID
		push	eax
		call	dwFunc
		add		esp, 4
		mov		dwReturn, eax
	}
	return dwReturn;
}