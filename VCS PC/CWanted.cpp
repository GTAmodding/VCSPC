#include "StdAfx.h"
#include "CWanted.h"

WRAPPER void CWanted::UpdateEachFrame() { EAXJMP(0x562360); }

bool CWanted::AreMiamiViceRequired()
{
	return m_WantedLevel >= 3;
}