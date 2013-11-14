#ifndef __TIMER
#define __TIMER

class CTimer
{
public:
	static int&				m_snTimeInMilliseconds;
	static int&				m_snTimeInMillisecondsPauseMode;
	static float&			ms_fTimeStep;
	static unsigned int&	m_FrameCounter;
};

#endif