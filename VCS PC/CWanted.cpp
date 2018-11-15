#include "StdAfx.h"
#include "CWanted.h"

WRAPPER void CWanted::UpdateEachFrame() { EAXJMP(0x562360); }

bool CWanted::AreMiamiViceRequired()
{
	return m_WantedLevel >= 3;
}

bool CWanted::AreSwatRequired()
{
	return m_WantedLevel >= 4;
}

bool CWanted::AreFbiRequired()
{
	return m_WantedLevel >= 5;
}

bool CWanted::AreArmyRequired()
{
	return m_WantedLevel >= 6;
}