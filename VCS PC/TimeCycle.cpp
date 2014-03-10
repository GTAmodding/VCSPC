#include "StdAfx.h"
#include "TimeCycle.h"

int&				CTimeCycle::m_CurrentStoredValue = *(int*)0xB79FD0;
CVector* const		CTimeCycle::m_VectorToSun = (CVector*)0xB7CA50;