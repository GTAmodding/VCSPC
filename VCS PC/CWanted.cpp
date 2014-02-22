#include "StdAfx.h"
#include "CWanted.h"

bool CWanted::ShouldSendViceSquad()
{
	return m_WantedLevel >= 3; //|| this->flags & m_viceSquadRequired;
}