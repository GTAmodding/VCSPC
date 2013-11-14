#include "StdAfx.h"

DWORD CWeaponInfo::GetWeaponModel(eWeaponType weaponID)
{
	DWORD dwFunc = (DWORD)FUNC_CWeaponInfo__GetWeaponModel;
	DWORD dwReturn;
	_asm
	{
		mov		eax, weaponID
		push	eax
		call	dwFunc
		mov		dwReturn, eax
	}
	return dwReturn;
}

CWeaponInfo* CWeaponInfo::GetWeaponInfo(eWeaponType weaponID, unsigned char bType)
{
	DWORD dwFunc = (DWORD)FUNC_CWeaponInfo__GetWeaponInfo;
	CWeaponInfo* dwReturn;
	_asm
	{
		movzx	eax, bType
		push	eax
		mov		eax, weaponID
		push	eax
		call	dwFunc
		add		esp, 8
		mov		dwReturn, eax
	}
	return dwReturn;
}