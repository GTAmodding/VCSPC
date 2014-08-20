#include "StdAfx.h"
#include "CWanted.h"

bool CWanted::AreMiamiViceRequired()
{
	return m_WantedLevel >= 3;
}