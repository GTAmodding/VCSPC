#ifndef __CLOCK
#define __CLOCK

class CClock
{
private:
	static unsigned char&	ms_nGameClockHours;
	static unsigned char&	ms_nGameClockMinutes;

public:
	static inline unsigned char		GetHours() 
						{ return ms_nGameClockHours; }
	static inline unsigned char		GetMinutes() 
						{ return ms_nGameClockMinutes; }

	static bool				GetIsTimeInRange(unsigned char bHourA, unsigned char bHourB);

	static bool				Convert24To12(unsigned char& bHour);
};

#endif