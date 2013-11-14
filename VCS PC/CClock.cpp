#include "StdAfx.h"

bool CClock::Convert24To12(BYTE& bHour)
{
	bool bPastMorning = bHour >= 12;
	bHour %= 12;
	if ( !bHour )
		bHour = 12;

	return bPastMorning;
}