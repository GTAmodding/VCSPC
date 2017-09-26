#ifndef __CLOCK
#define __CLOCK

class CClock
{
private:
	static unsigned char&	ms_nGameClockHours;
	static unsigned char&	ms_nGameClockMinutes;
	static unsigned short&	ms_nGameClockSeconds;

public:
	static inline unsigned char		GetHours() 
						{ return ms_nGameClockHours; }
	static inline unsigned char		GetMinutes() 
						{ return ms_nGameClockMinutes; }
	static inline unsigned short		GetSeconds() 
						{ return ms_nGameClockSeconds; }

	static bool				GetIsTimeInRange(unsigned char bHourA, unsigned char bHourB);

	static bool				Convert24To12(unsigned char& bHour);
};

#endif