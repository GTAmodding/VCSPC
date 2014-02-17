#include "StdAfx.h"

unsigned char&	CClock::ms_nGameClockHours = *(unsigned char*)0xB70153;
unsigned char&	CClock::ms_nGameClockMinutes = *(unsigned char*)0xB70152;

bool CClock::GetIsTimeInRange(unsigned char bHourA, unsigned char bHourB)
{
	if ( bHourA > bHourB )
		return ms_nGameClockHours >= bHourA || ms_nGameClockHours < bHourB;
	return ms_nGameClockHours >= bHourA && ms_nGameClockHours < bHourB;
}

bool CClock::Convert24To12(unsigned char& bHour)
{
	bool bPastMorning = bHour >= 12;
	bHour %= 12;
	if ( !bHour )
		bHour = 12;

	return bPastMorning;
}