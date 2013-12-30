#include "StdAfx.h"

WRAPPER bool CClock::GetIsTimeInRange(unsigned char bHourA, unsigned char bHourB) { WRAPARG(bHourA); WRAPARG(bHourB); EAXJMP(0x52CEE0); }

bool CClock::Convert24To12(BYTE& bHour)
{
	bool bPastMorning = bHour >= 12;
	bHour %= 12;
	if ( !bHour )
		bHour = 12;

	return bPastMorning;
}