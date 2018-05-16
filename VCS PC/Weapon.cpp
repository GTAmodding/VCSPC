#include "StdAfx.h"
#include "Weapon.h"

WRAPPER void CWeapon::InitialiseWeapons(void) { EAXJMP(0x73A300); }
WRAPPER void CWeapon::UpdateWeapons(void) { EAXJMP(0x73A360); }
WRAPPER CWeaponInfo* CWeaponInfo::GetWeaponInfo(eWeaponType weaponID, signed char bType) { WRAPARG(weaponID); WRAPARG(bType); EAXJMP(0x743C60); }

bool CWeapon::IsTypeMelee() const
{
	return CWeaponInfo::GetWeaponInfo(m_eWeaponType, 1)->weaponType == FIRETYPE_MELEE;
}