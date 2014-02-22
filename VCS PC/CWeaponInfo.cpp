#include "StdAfx.h"
#include "CWeaponInfo.h"

WRAPPER CWeaponInfo* CWeaponInfo::GetWeaponInfo(eWeaponType weaponID, signed char bType) { WRAPARG(weaponID); WRAPARG(bType); EAXJMP(0x743C60); }