#include "StdAfx.h"
#include "Timer.h"

int&				CTimer::m_snTimeInMilliseconds = *(int*)0xB7CB84;
int&				CTimer::m_snTimeInMillisecondsPauseMode = *(int*)0xB7CB7C;
float&				CTimer::ms_fTimeStep = *(float*)0xB7CB5C;
unsigned int&		CTimer::m_FrameCounter = *(unsigned int*)0xB7CB4C;
bool &CTimer::m_CodePause = *(bool*)0xB7CB48;
bool &CTimer::m_UserPause = *(bool*)0xB7CB49;
bool &CTimer::bSkipProcessThisFrame = *(bool*)0xB7CB89;

WRAPPER void CTimer::Update() { EAXJMP(0x561B10); }
WRAPPER uint32 CTimer::GetCurrentTimeInCycles() { EAXJMP(0x561A80); }
WRAPPER uint32 CTimer::GetCyclesPerMillisecond() { EAXJMP(0x561A40); }

uint32 CTimer::GetCurrentTimeInMilliseconds() { return GetCurrentTimeInCycles() / GetCyclesPerMillisecond(); }
