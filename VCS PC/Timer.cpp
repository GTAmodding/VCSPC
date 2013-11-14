#include "StdAfx.h"

int&				CTimer::m_snTimeInMilliseconds = *(int*)0xB7CB84;
int&				CTimer::m_snTimeInMillisecondsPauseMode = *(int*)0xB7CB7C;
float&				CTimer::ms_fTimeStep = *(float*)0xB7CB5C;
unsigned int&		CTimer::m_FrameCounter = *(unsigned int*)0xB7CB4C;