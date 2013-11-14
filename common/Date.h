#ifndef __DATE
#define __DATE

class CDate
{
private:
	time_t	m_time;

public:
	CDate()
	{}

	CDate(const CDate& CopiedDate)
		: m_time(CopiedDate.m_time)
	{}

	CDate(time_t Time)
		: m_time(Time)
	{}

	CDate(unsigned int nDay, unsigned int nMonth, int nExtraYears = 0)
	{
		tm*		dateTime;
		time_t	rawTime;

		rawTime = time(nullptr);
		dateTime = localtime(&rawTime);

		dateTime->tm_sec = 0;
		dateTime->tm_min = 0;
		dateTime->tm_hour = 0;
		dateTime->tm_mday = nDay;
		dateTime->tm_mon = nMonth - 1;
		dateTime->tm_year += nExtraYears;
		//dateTime->tm_isdst = FALSE;

		m_time = mktime(dateTime);
	}

	operator time_t() { return m_time; }

	friend bool operator > (const CDate& left, const CDate& right)
		{ return left.m_time > right.m_time; }
	friend bool operator >= (const CDate& left, const CDate& right)
		{ return left.m_time >= right.m_time; }
	friend bool operator < (const CDate& left, const CDate& right)
		{ return left.m_time < right.m_time; }
	friend bool operator <= (const CDate& left, const CDate& right)
		{ return left.m_time <= right.m_time; }
	friend bool operator == (const CDate& left, const CDate& right)
		{ return left.m_time == right.m_time; }
};

inline CDate GetCurrentDate()
{
	return CDate(time(nullptr));
}

#endif