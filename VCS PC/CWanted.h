#ifndef __CWANTED
#define __CWANTED

class CWanted
{
private:
	DWORD			m_nWantedLevel;
	BYTE			__pad1[26];
	bool			m_PoliceBackOff : 1;
	bool			m_PoliceBackOffGarage : 1;
	bool			m_EverybodyBackOff : 1;
	bool			m_swatRequired : 1;
	bool			m_fbiRequired : 1;
	bool			m_armyRequired : 1;
	bool			m_viceSquadRequired : 1;
	BYTE			__pad2[636];

public:
	static CWanted*	GetPlayerCWanted(int playerID);	

	BOOL			ShouldSendViceSquad();
};

static_assert(sizeof(CWanted) == CWanted_ARRAYSIZE, "CWanted class has wrong size!");
//static_assert(sizeof(CWanted) == CWanted_ARRAYSIZE);

#endif