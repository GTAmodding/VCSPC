#ifndef __CWANTED
#define __CWANTED

class CWanted
{
private:
	int				m_nWantedLevel;
	int				m_nWantedLevelBeforeParole;
	int				m_LastTimeWantedDecreased;
	int				m_LastTimeWantedLevelChanged;
	int				m_TimeOfParole;
	float			m_fMultiplier;
	unsigned char	m_nCopsInPursuit;
	unsigned char	m_nMaxCopsInPursuit;
	unsigned char	m_nMaxCopCarsInPursuit;
	bool			m_nCopsBeatingSuspect;
	unsigned short	m_nChanceOnRoadBlock;
	bool			m_PoliceBackOff : 1;
	bool			m_PoliceBackOffGarage : 1;
	bool			m_EverybodyBackOff : 1;
	bool			m_swatRequired : 1;
	bool			m_fbiRequired : 1;
	bool			m_armyRequired : 1;
	bool			m_viceSquadRequired : 1;
	int				current_chase_time;
	int				current_chase_time_counter;
	bool			m_bTimeCounting;
	int				m_WantedLevel;
	int				m_WantedLevelBeforeParole;
	BYTE			__pad2[616];

public:
	inline int		GetWantedLevel() { return m_WantedLevel; }
	inline int		GetWantedLevelBeforeParole() { return m_WantedLevelBeforeParole; }
	inline int		GetTimeWantedLevelChanged() { return m_LastTimeWantedLevelChanged; }

	bool			ShouldSendViceSquad();
};

static_assert(sizeof(CWanted) == 0x29C, "Wrong size: CWanted");

#endif