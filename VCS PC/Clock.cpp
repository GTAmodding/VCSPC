#include "StdAfx.h"
#include "Clock.h"

unsigned char&	CClock::ms_nGameClockHours = *(unsigned char*)0xB70153;
unsigned char&	CClock::ms_nGameClockMinutes = *(unsigned char*)0xB70152;
unsigned short&	CClock::ms_nGameClockSeconds = *(unsigned short*)0xB70150;

WRAPPER void CClock::Initialise(int msPerSecond) { EAXJMP(0x52CD90); }

bool CClock::GetIsTimeInRange(unsigned char bHourA, unsigned char bHourB)
{
	return bHourA > bHourB ? ms_nGameClockHours >= bHourA || ms_nGameClockHours < bHourB : ms_nGameClockHours >= bHourA && ms_nGameClockHours < bHourB;
}

bool CClock::Convert24To12(unsigned char& bHour)
{
	bool bPastMorning = bHour >= 12;
	bHour %= 12;
	if ( bHour == 0 )
		bHour = 12;

	return bPastMorning;
}