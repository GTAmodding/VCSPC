#include "StdAfx.h"

bool CWanted::ShouldSendViceSquad()
{
	return m_WantedLevel >= 3; //|| this->flags & m_viceSquadRequired;
}