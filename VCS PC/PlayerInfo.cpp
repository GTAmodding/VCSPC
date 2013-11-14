#include "StdAfx.h"

// Wrappers
WRAPPER void CPlayerInfo::KillPlayer() { EAXJMP(0x56E580); }
WRAPPER void CPlayerInfo::ArrestPlayer() { EAXJMP(0x56E5D0); }