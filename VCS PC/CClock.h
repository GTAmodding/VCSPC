#ifndef __CCLOCK
#define __CCLOCK

#define WIN32_LEAN_AND_MEAN

class CClock
{
private:
	BYTE			bClockHasBeenStored;
	BYTE			__pad1[3];
	WORD			wStoredGameClockSeconds;
	BYTE			bStoredGameClockMinutes;
	BYTE			bStoredGameClockHours;
	BYTE			bStoredGameMonthDay;
	BYTE			bStoredGameMonth;
	BYTE			bGameDayOfWeek;
	WORD			wGameClockSeconds;
	BYTE			bGameClockMinutes;
	BYTE			bGameClockHours;
	BYTE			bGameMonthDay;
	BYTE			bGameMonth;
	DWORD			LastClockTick;
	DWORD			MillisecondsPerGameMinute;

public:
	BYTE			GetHour() 
						{ return bGameClockHours; };
	BYTE			GetMinute() 
						{ return bGameClockMinutes; };

	static bool		GetIsTimeInRange(unsigned char bHourA, unsigned char bHourB);

	static bool		Convert24To12(BYTE& bHour);
};

static_assert(sizeof(CClock) == CClock_ARRAYSIZE, "CClock class has wrong size!");

#endif