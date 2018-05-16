#ifndef __TIMER
#define __TIMER

class CTimer
{
public:
	static int&				m_snTimeInMilliseconds;
	static int&				m_snTimeInMillisecondsPauseMode;
	static float&			ms_fTimeStep;
	static unsigned int&	m_FrameCounter;
	static bool	&m_CodePause;
	static bool	&m_UserPause;
	static bool	&bSkipProcessThisFrame;

	static void Update();
	static uint32 GetCurrentTimeInCycles();
	static uint32 GetCyclesPerMillisecond();
	static uint32 GetCurrentTimeInMilliseconds();
};

inline bool ShowFlashingItem(signed int nOnDuration, signed int nOffDuration)
{
	return CTimer::m_snTimeInMillisecondsPauseMode % (nOnDuration+nOffDuration) < nOnDuration;
}

#endif