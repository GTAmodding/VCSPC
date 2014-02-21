#include "StdAfx.h"

// Hidden there to reduce compilation time
#undef call		// omg
#include <boost\bimap.hpp>

typedef boost::bimap<CEntryExit*,std::string>		tEntryExitBimap;

static tEntryExitBimap								strUniqueNames;

CPool<CEntryExit>*&			CEntryExitManager::mp_poolEntryExits = *(CPool<CEntryExit>**)0x96A7D8;

WRAPPER int CEntryExitManager::AddOne(float fEntX, float fEntY, float fEntZ, float fEntA, float fEntRX, float fEntRY, float, float fExitX, float fExitY, float fExitZ, float fExitA, int nArea, int flags, int nExtraColors, int nTimeOn, int nTimeOff, int nUnkSky, const char* pGXTName)
{ WRAPARG(fEntX); WRAPARG(fEntY); WRAPARG(fEntZ); WRAPARG(fEntA); WRAPARG(fEntRX); WRAPARG(fEntRY); WRAPARG(fExitX); WRAPARG(fExitY); WRAPARG(fExitZ); WRAPARG(fExitA); WRAPARG(nArea); WRAPARG(flags); WRAPARG(nExtraColors); WRAPARG(nTimeOn); WRAPARG(nTimeOff); WRAPARG(nUnkSky); WRAPARG(pGXTName); EAXJMP(0x43FA00); }

void CEntryExit::AddToBimap(const char* pName)
{
	strUniqueNames.insert(tEntryExitBimap::value_type(this, pName));
}