#include "StdAfx.h"
#include "Stats.h"

#include "Text.h"
#include "Timer.h"

enum
{
	STATTYPE_EMPTY,
	STATTYPE_INT,
	STATTYPE_FLOAT,
	STATTYPE_PERCENT,
	STATTYPE_FPERCENT,
	STATTYPE_FMONEY,
	STATTYPE_TIME,
	STATTYPE_OUTOF
};

WRAPPER void CStats::Init(void) { EAXJMP(0x55C0C0); }
WRAPPER long double CStats::CalcPlayerStat(unsigned int statID) { WRAPARG(statID); EAXJMP(0x559AF0); }
WRAPPER void CStats::IncrementStat(unsigned int statID, float fAmount) { WRAPARG(statID); WRAPARG(fAmount); EAXJMP(0x55C180); }
WRAPPER const char* CStats::FindCriminalRatingNumber() { EAXJMP(0x55A210); }

inline bool UseMetricSystem()
{
	return true;
}

static inline int GetMinutesFromStat(int nTime)
{
	return nTime / 60;
}

static inline int GetSecondsFromStat(int nTime)
{
	return nTime % 60;
}

long CStats::ConstructStatLine(int nStat, int& nIndents)
{
	int		nTempValue = 0;
	nIndents = 0;

	if ( nStat == nTempValue++ )
	{
		// Percentage completed
		float	fProgress;
		if ( PlayerStatsFloat[TOTAL_PROGRESS] == 0.0f )
			fProgress = 0.0f;
		else
		{
			fProgress = PlayerStatsFloat[PROGRESS_MADE] / PlayerStatsFloat[TOTAL_PROGRESS] * 100.0f;
			if ( fProgress > 100.0f )
				fProgress = 100.0f;
		}
		BuildStatLine("PER_COM", &fProgress, STATTYPE_FPERCENT, nullptr);
		return 0;
	}

	if ( nStat == nTempValue++ )
	{
		// Mission attempts
		BuildStatLine("NMISON", &PlayerStatsInteger[MISSIONS_ATTEMPTED], STATTYPE_INT, nullptr);
		return 0;
	}
	
	// TODO: Current playing time - ST_TIME

	if ( nStat == nTempValue++ )
	{
		// Total playing time
		DWORD	dwPlayingTimeHours = CTimer::m_snTimeInMilliseconds / 3600000;
		DWORD	dwPlayingTimeMinutes = (CTimer::m_snTimeInMilliseconds / 60000) % 60;
		BuildStatLine("ST_OVTI", &dwPlayingTimeHours, STATTYPE_TIME, &dwPlayingTimeMinutes);
		return 0;
	}

	if ( nStat == nTempValue++ )
	{
		// Days passed in game
		BuildStatLine("DAYSPS", &PlayerStatsInteger[DAYS_PASSED], STATTYPE_INT, nullptr);
		return 0;
	}

	if ( nStat == nTempValue++ )
	{
		// Number of saves
		BuildStatLine("NUMSHV", &PlayerStatsInteger[TIMES_SAVED], STATTYPE_INT, nullptr);
		return 0;
	}

	/*if ( nStat == nTempValue++ )
	{
		// Empire Statistics
		BuildStatLine("EMP_STA", nullptr, STATTYPE_EMPTY, nullptr);
		return 0;
	}*/

	// TODO: Total cash spent on Empire development
	// TODO: Total cash spent on Empire repair
	// TODO: Biggest payday
	// TODO: Total Empire Earnings
	// TODO: Number of empire sites Vance owned
	// TODO: Percentage empire sites Vance owned

	// MISSING DATA HERE

	// TODO: Attacks successfull
	// TODO: Sites successfully defended

	// MISSING DATA HERE

	if ( nStat == nTempValue++ )
	{
		// Rampages passed
		BuildStatLine("FEST_RP", &PlayerStatsInteger[KILL_FRENZIES_PASSED], STATTYPE_OUTOF, &PlayerStatsInteger[TOTAL_KILL_FRENZIES]);
		return 0;
	}

	// TODO: Red Balloons burst

	if ( nStat == nTempValue++ )
	{
		// People you've wasted
		BuildStatLine("PE_WAST", &PlayerStatsInteger[PEOPLE_KILLED_BY_PLAYER], STATTYPE_INT, nullptr);
		return 0;
	}

	// TODO: Number of necks snapped

	if ( nStat == nTempValue++ )
	{
		// People wasted by others
		BuildStatLine("PE_WSOT", &PlayerStatsInteger[PEOPLE_KILLED_BY_OTHERS], STATTYPE_INT, nullptr);
		return 0;
	}

	if ( nStat == nTempValue++ )
	{
		// Road Vehicles destroyed
		BuildStatLine("CAR_EXP", &PlayerStatsInteger[CARS_DESTROYED], STATTYPE_INT, nullptr);
		return 0;
	}

	if ( nStat == nTempValue++ )
	{
		// Boats destroyed
		BuildStatLine("BOA_EXP", &PlayerStatsInteger[BOATS_DESTROYED], STATTYPE_INT, nullptr);
		return 0;
	}

	if ( nStat == nTempValue++ )
	{
		// Aircraft destroyed
		BuildStatLine("HEL_DST", &PlayerStatsInteger[HELICOPTERS_DESTROYED], STATTYPE_INT, nullptr);
		return 0;
	}

	if ( nStat == nTempValue++ )
	{
		// Tires popped with gunfire
		BuildStatLine("TYREPOP", &PlayerStatsInteger[TYRES_POPPED], STATTYPE_INT, nullptr);
		return 0;
	}

	if ( nStat == nTempValue++ )
	{
		// Total wanted stars attained
		BuildStatLine("ST_STAR", &PlayerStatsInteger[WANTED_STARS_ATTAINED], STATTYPE_INT, nullptr);
		return 0;
	}

	if ( nStat == nTempValue++ )
	{
		// Total wanted stars evaded
		BuildStatLine("ST_STGN", &PlayerStatsInteger[WANTED_STARS_EVADED], STATTYPE_INT, nullptr);
		return 0;
	}

	if ( nStat == nTempValue++ )
	{
		// Times you've been busted
		BuildStatLine("TM_BUST", &PlayerStatsInteger[TIMES_ARRESTED], STATTYPE_INT, nullptr);
		return 0;
	}

	if ( nStat == nTempValue++ )
	{
		// Times you've been wasted
		BuildStatLine("TM_DED", &PlayerStatsInteger[TIMES_DIED], STATTYPE_INT, nullptr);
		return 0;
	}

	if ( nStat == nTempValue++ )
	{
		// Number of headshots
		BuildStatLine("ST_HEAD", &PlayerStatsInteger[HEADS_POPPED], STATTYPE_INT, nullptr);
		return 0;
	}

	if ( nStat == nTempValue++ )
	{
		// Daily police spending
		static int		nLastDayWeCheckedThis = -1;
		static float	fLastSpending = 0.0f;

		if ( nLastDayWeCheckedThis != PlayerStatsInteger[DAYS_PASSED] )
		{
			nLastDayWeCheckedThis = PlayerStatsInteger[DAYS_PASSED];
			fLastSpending = floor(((CTimer::m_snTimeInMilliseconds & 0xFF) + 80) * 255.44f);
		}
		BuildStatLine("DAYPLC", &fLastSpending, STATTYPE_FMONEY, nullptr);
		return 0;
	}

	// Least favorite gang
	int		nTheMostKills = 0;
	int		nTheMostHatedGang = 0;
	int		nPedtype = 7;

	do
	{
		if ( PlayerStatsInteger[28 + nPedtype] > nTheMostKills )
		{
			nTheMostKills = PlayerStatsInteger[28 + nPedtype];
			nTheMostHatedGang = nPedtype;
		}
	}
	while ( ++nPedtype < 15 );

	if ( nTheMostHatedGang )
	{
		if ( nStat == nTempValue++ )
		{
			strcpy(gString, TheText.Get("ST_GANG"));
			switch ( nTheMostHatedGang )
			{
			case 7:
				{
					strcpy(gUString, TheText.Get("ST_GNG1"));
					return 0;
				}
			case 8:
				{
					strcpy(gUString, TheText.Get("ST_GNG2"));
					return 0;
				}
			case 9:
				{
					strcpy(gUString, TheText.Get("ST_GNG3"));
					return 0;
				}
			case 10:
				{
					strcpy(gUString, TheText.Get("ST_GNG4"));
					return 0;
				}
			case 11:
				{
					strcpy(gUString, TheText.Get("ST_GNG5"));
					return 0;
				}
			case 12:
				{
					strcpy(gUString, TheText.Get("ST_GNG6"));
					return 0;
				}
			case 13:
				{
					strcpy(gUString, TheText.Get("ST_GNG7"));
					return 0;
				}
			case 14:
				{
					strcpy(gUString, TheText.Get("ST_GNG8"));
					return 0;
				}
			}
		}
	}

	if ( nStat == nTempValue++ )
	{
		// Gang members wasted
		unsigned int	nWastedGangmembers = PlayerStatsInteger[28 + 7] + PlayerStatsInteger[28 + 8] + PlayerStatsInteger[28 + 9]
						+ PlayerStatsInteger[28 + 10] + PlayerStatsInteger[28 + 11]
						+ PlayerStatsInteger[28 + 12] + PlayerStatsInteger[28 + 13] + PlayerStatsInteger[28 + 14];
		BuildStatLine("GNG_WST", &nWastedGangmembers, STATTYPE_INT, nullptr);
		return 0;
	}

	if ( nStat == nTempValue++ )
	{
		// Criminals wasted
		BuildStatLine("DED_CRI", &PlayerStatsInteger[28 + 20], STATTYPE_INT, nullptr);
		return 0;
	}

	if ( nStat == nTempValue++ )
	{
		// Kgs of explosives used
		BuildStatLine("KGS_EXP", &PlayerStatsInteger[EXPLOSIVES_USED], STATTYPE_INT, nullptr);
		return 0;
	}

	if ( nStat == nTempValue++ )
	{
		// Bullets fired
		BuildStatLine("BUL_FIR", &PlayerStatsInteger[ROUNDS_FIRED], STATTYPE_INT, nullptr);
		return 0;
	}

	if ( nStat == nTempValue++ )
	{
		// Bullets that hit
		BuildStatLine("BUL_HIT", &PlayerStatsInteger[BULLETS_HIT], STATTYPE_INT, nullptr);
		return 0;
	}

	if ( nStat == nTempValue++ )
	{
		// Accuracy
		int		nAccuracy;
		if ( PlayerStatsInteger[ROUNDS_FIRED] )
			nAccuracy = static_cast<int>(PlayerStatsInteger[BULLETS_HIT] * 100.0f / PlayerStatsInteger[ROUNDS_FIRED]);
		else
			nAccuracy = 0;
		BuildStatLine("ACCURA", &nAccuracy, STATTYPE_PERCENT, nullptr);
		return 0;
	}

	if ( UseMetricSystem() )
	{
		if ( nStat == nTempValue++ )
		{
			// Distance travelled on foot (m)
			BuildStatLine("FESTDFM", &PlayerStatsFloat[DIST_FOOT], STATTYPE_FLOAT, nullptr);
			return 0;
		}

		if ( nStat == nTempValue++ )
		{
			// Distance travelled by swimming (m)
			BuildStatLine("DISTSWM", &PlayerStatsFloat[DIST_SWIMMING], STATTYPE_FLOAT, nullptr);
			return 0;
		}

		if ( nStat == nTempValue++ )
		{
			// Distance travelled by car (m)
			BuildStatLine("FESTDCM", &PlayerStatsFloat[DIST_CAR], STATTYPE_FLOAT, nullptr);
			return 0;
		}

		if ( nStat == nTempValue++ )
		{
			// Distance travelled by bike (m)
			float	fDist = PlayerStatsFloat[DIST_BIKE] + PlayerStatsFloat[DIST_CYCLE];
			BuildStatLine("DISTBIM", &fDist, STATTYPE_FLOAT, nullptr);
			return 0;
		}

		if ( nStat == nTempValue++ )
		{
			// Distance travelled by boat (m)
			BuildStatLine("DISTBOM", &PlayerStatsFloat[DIST_BOAT], STATTYPE_FLOAT, nullptr);
			return 0;
		}

		if ( nStat == nTempValue++ )
		{
			// Distance travelled by air (miles)
			float	fDist = PlayerStatsFloat[DIST_HELICOPTOR] + PlayerStatsFloat[DIST_PLANE];
			BuildStatLine("DISTHEM", &fDist, STATTYPE_FLOAT, nullptr);
			return 0;
		}

		if ( nStat == nTempValue++ )
		{
			// Total distance travelled (miles)
			float	fDist = PlayerStatsFloat[DIST_FOOT] + PlayerStatsFloat[DIST_SWIMMING] + PlayerStatsFloat[DIST_CAR] + PlayerStatsFloat[DIST_BIKE] + PlayerStatsFloat[DIST_CYCLE] + PlayerStatsFloat[DIST_BOAT] + PlayerStatsFloat[DIST_HELICOPTOR] + PlayerStatsFloat[DIST_PLANE];
			BuildStatLine("TOTDISM", &fDist, STATTYPE_FLOAT, nullptr);
			return 0;
		}
	}
	else
	{
		if ( nStat == nTempValue++ )
		{
			// Distance travelled on foot (miles)
			float	fDist = PlayerStatsFloat[DIST_FOOT] * METRES_TO_MILES;
			BuildStatLine("FEST_DF", &fDist, STATTYPE_FLOAT, nullptr);
			return 0;
		}

		if ( nStat == nTempValue++ )
		{
			// Distance travelled by swimming (miles)
			float	fDist = PlayerStatsFloat[DIST_SWIMMING] * METRES_TO_MILES;
			BuildStatLine("DISTSWI", &fDist, STATTYPE_FLOAT, nullptr);
			return 0;
		}

		if ( nStat == nTempValue++ )
		{
			// Distance travelled by car (miles)
			float	fDist = PlayerStatsFloat[DIST_CAR] * METRES_TO_MILES;
			BuildStatLine("FEST_DC", &fDist, STATTYPE_FLOAT, nullptr);
			return 0;
		}

		if ( nStat == nTempValue++ )
		{
			// Distance travelled by bike (miles)
			float	fDist = (PlayerStatsFloat[DIST_BIKE] + PlayerStatsFloat[DIST_CYCLE]) * METRES_TO_MILES;
			BuildStatLine("DISTBIK", &fDist, STATTYPE_FLOAT, nullptr);
			return 0;
		}

		if ( nStat == nTempValue++ )
		{
			// Distance travelled by boat (miles)
			float	fDist = PlayerStatsFloat[DIST_BOAT] * METRES_TO_MILES;
			BuildStatLine("DISTBOA", &fDist, STATTYPE_FLOAT, nullptr);
			return 0;
		}

		if ( nStat == nTempValue++ )
		{
			// Distance travelled by air (miles)
			float	fDist = (PlayerStatsFloat[DIST_HELICOPTOR] + PlayerStatsFloat[DIST_PLANE]) * METRES_TO_MILES;
			BuildStatLine("DISTHEL", &fDist, STATTYPE_FLOAT, nullptr);
			return 0;
		}

		if ( nStat == nTempValue++ )
		{
			// Total distance travelled (miles)
			float	fDist = (PlayerStatsFloat[DIST_FOOT] + PlayerStatsFloat[DIST_SWIMMING] + PlayerStatsFloat[DIST_CAR] + PlayerStatsFloat[DIST_BIKE] + PlayerStatsFloat[DIST_CYCLE] + PlayerStatsFloat[DIST_BOAT] + PlayerStatsFloat[DIST_HELICOPTOR] + PlayerStatsFloat[DIST_PLANE]) * METRES_TO_MILES;
			BuildStatLine("TOT_DIS", &fDist, STATTYPE_FLOAT, nullptr);
			return 0;
		}
	}

	if ( nStat == nTempValue++ )
	{
		// Longest insane jump distance (m)
		float	fDist = static_cast<float>(PlayerStatsInteger[MAX_JUMP_DISTANCE]);
		BuildStatLine("MXCARDM", &fDist, STATTYPE_FLOAT, nullptr);
		return 0;
	}

	if ( nStat == nTempValue++ )
	{
		// Greatest insane jump height (m)
		float	fHeight = static_cast<float>(PlayerStatsInteger[MAX_JUMP_HEIGHT]);
		BuildStatLine("MXCARJM", &fHeight, STATTYPE_FLOAT, nullptr);
		return 0;
	}

	if ( nStat == nTempValue++ )
	{
		// Maximum insane jump flips
		BuildStatLine("MXFLIP", &PlayerStatsInteger[MAX_JUMP_FLIPS], STATTYPE_INT, nullptr);
		return 0;
	}

	if ( nStat == nTempValue++ )
	{
		// Maximum insane jump rotation
		BuildStatLine("MXJUMP", &PlayerStatsInteger[MAX_JUMP_SPINS], STATTYPE_INT, nullptr);
		return 0;
	}

	if ( nStat == nTempValue++ )
	{
		// Greatest insane stunt
		gUString[0] = '\0';
		strcpy(gString, TheText.Get("BSTSTU"));
		return 0;
	}

	if ( nStat == nTempValue++ )
	{
		gString[0] = '\0';
		switch ( PlayerStatsInteger[BEST_STUNT] )
		{
		case 1:
			{
				strcpy(gUString, TheText.Get("INSTUN"));
				return 0;
			}
		case 2:
			{
				strcpy(gUString, TheText.Get("FLINST"));
				return 0;
			}
		case 3:
			{
				strcpy(gUString, TheText.Get("PRINST"));
				return 0;
			}
		case 4:
			{
				strcpy(gUString, TheText.Get("DBINST"));
				return 0;
			}
		case 5:
			{
				strcpy(gUString, TheText.Get("DBFINS"));
				return 0;
			}
		case 6:
			{
				strcpy(gUString, TheText.Get("DBPINS"));
				return 0;
			}
		case 7:
			{
				strcpy(gUString, TheText.Get("TRINST"));
				return 0;
			}
		case 8:
			{
				strcpy(gUString, TheText.Get("FLTRST"));
				return 0;
			}
		case 9:
			{
				strcpy(gUString, TheText.Get("PRTRST"));
				return 0;
			}
		case 10:
			{
				strcpy(gUString, TheText.Get("QUINST"));
				return 0;
			}
		case 11:
			{
				strcpy(gUString, TheText.Get("FQUINS"));
				return 0;
			}
		case 12:
			{
				strcpy(gUString, TheText.Get("PQUINS"));
				return 0;
			}
		default:
			{
				strcpy(gUString, TheText.Get("NOSTUC"));
				return 0;
			}
		}
	}

	/*if ( nStat == nTempValue++ )
	{
		// Unique jumps passed
		BuildStatLine("NOUNIF", &PlayerStatsInteger[UNIQUE_JUMPS_DONE], STATTYPE_OUTOF, &PlayerStatsInteger[TOTAL_UNIQUE_JUMPS]);
		return 0;
	}*/

	if ( nStat == nTempValue++ )
	{
		// Longest wheelie time (secs)
		int		nTime = static_cast<int>(PlayerStatsFloat[LONGEST_WHEELIE_TIME]);
		BuildStatLine("ST_WHEE", &nTime, STATTYPE_INT, nullptr);
		return 0;
	}

	if ( nStat == nTempValue++ )
	{
		// Longest wheelie distance (m)
		BuildStatLine("ST_WHED", &PlayerStatsFloat[LONGEST_WHEELIE_DIST], STATTYPE_FLOAT, nullptr);
		return 0;
	}

	if ( nStat == nTempValue++ )
	{
		// Longest stoppie time (secs)
		int		nTime = static_cast<int>(PlayerStatsFloat[LONGEST_STOPPIE_TIME]);
		BuildStatLine("ST_STOP", &nTime, STATTYPE_INT, nullptr);
		return 0;
	}

	if ( nStat == nTempValue++ )
	{
		// Longest stoppie distance (m)
		BuildStatLine("ST_STOD", &PlayerStatsFloat[LONGEST_STOPPIE_DIST], STATTYPE_FLOAT, nullptr);
		return 0;
	}

	if ( nStat == nTempValue++ )
	{
		// Longest face plant (m)
		BuildStatLine("ST_FAPD", &PlayerStatsFloat[LONGEST_FACE_PLANT], STATTYPE_FLOAT, nullptr);
		return 0;
	}

	if ( nStat == nTempValue++ )
	{
		// Longest 2 wheels time (secs)
		int		nTime = static_cast<int>(PlayerStatsFloat[LONGEST_2WHEEL_TIME]);
		BuildStatLine("ST_2WHE", &nTime, STATTYPE_INT, nullptr);
		return 0;
	}

	if ( nStat == nTempValue++ )
	{
		// Longest 2 wheels distance (m)
		BuildStatLine("ST_2WHD", &PlayerStatsFloat[LONGEST_2WHEEL_DIST], STATTYPE_FLOAT, nullptr);
		return 0;
	}

	/*if ( PlayerStatsInteger[CRIMINALS_CAUGHT] )
	{
		if ( nStat == nTempValue++ )
		{
			// Total vigilante justice served
			BuildStatLine("FEST_CC", &PlayerStatsInteger[CRIMINALS_CAUGHT], STATTYPE_INT, nullptr);
			return 0;
		}
	}

	if ( PlayerStatsInteger[VIGILANTE_LEVEL] )
	{
		if ( nStat == nTempValue++ )
		{
			// Highest 'Vigilante' level
			BuildStatLine("FEST_HV", &PlayerStatsInteger[VIGILANTE_LEVEL], STATTYPE_INT, nullptr);
			return 0;
		}
	}*/

	if ( PlayerStatsInteger[PASSENGERS_DELIVERED_IN_TAXI] )
	{
		if ( nStat == nTempValue++ )
		{
			// Total fares dropped off
			BuildStatLine("PASDRO", &PlayerStatsInteger[PASSENGERS_DELIVERED_IN_TAXI], STATTYPE_INT, nullptr);
			return 0;
		}
	}

	if ( PlayerStatsInteger[TAXI_MONEY_MADE] )
	{
		if ( nStat == nTempValue++ )
		{
			// Cash made as a taxi driver
			float	fMoney = static_cast<float>(PlayerStatsInteger[TAXI_MONEY_MADE]);
			BuildStatLine("MONTAX", &fMoney, STATTYPE_FMONEY, nullptr);
			return 0;
		}
	}

	if ( PlayerStatsInteger[LIVES_SAVED] )
	{
		if ( nStat == nTempValue++ )
		{
			// Patients saved
			BuildStatLine("FEST_LS", &PlayerStatsInteger[LIVES_SAVED], STATTYPE_INT, nullptr);
			return 0;
		}
	}

	if ( PlayerStatsInteger[AMBULANCE_LEVEL] )
	{
		if ( nStat == nTempValue++ )
		{
			// Highest 'Paramedic' level
			BuildStatLine("FEST_HA", &PlayerStatsInteger[AMBULANCE_LEVEL], STATTYPE_INT, nullptr);
			return 0;
		}
	}

	/*if ( PlayerStatsInteger[FIRES_EXTINGUISHED] )
	{
		if ( nStat == nTempValue++ )
		{
			// Total fires extinguished
			BuildStatLine("FEST_FE", &PlayerStatsInteger[FIRES_EXTINGUISHED], STATTYPE_INT, nullptr);
			return 0;
		}
	}

	if ( PlayerStatsInteger[FIREFIGHTER_LEVEL] )
	{
		if ( nStat == nTempValue++ )
		{
			// Highest 'Firefighter' level
			BuildStatLine("FIRELVL", &PlayerStatsInteger[FIREFIGHTER_LEVEL], STATTYPE_INT, nullptr);
			return 0;
		}
	}*/

	// TODO: Vehicles seized for the Civil Asset Forfeiture Impound

	if ( PlayerStatsInteger[BEST_SCORE_CRASH] )
	{
		if ( nStat == nTempValue++ )
		{
			//  Most damage caused on 'Crash'
			float		fMayhem = static_cast<float>(PlayerStatsInteger[BEST_SCORE_CRASH]);
			BuildStatLine("ST_CRA", &fMayhem, STATTYPE_FMONEY, nullptr);
			return 0;
		}
	}

	if ( PlayerStatsInteger[BEST_SCORE_RUSH] )
	{
		if ( nStat == nTempValue++ )
		{
			//  Most damage caused on 'Rush'
			float		fMayhem = static_cast<float>(PlayerStatsInteger[BEST_SCORE_RUSH]);
			BuildStatLine("ST_CRA2", &fMayhem, STATTYPE_FMONEY, nullptr);
			return 0;
		}
	}

	// TODO: Fastest time on 'Mashin' up the Mall'
	// TODO: Fastest time on 'Harbor Hover Race'
	// TODO: Fastest time on 'Haiti Hover Race'
	// TODO: Total Shoplifters apprehended // UNUSED?
	// TODO: Greatest value of goods saved on 'Crimewave' // UNUSED?
	// TODO: Highest 'Crimewave' level // UNUSED?
	// TODO: Highest 'Air Rescue' level
	// TODO: Highest 'Vice Sights' level
	// TODO: Sights viewed on 'Vice Sights'
	// TODO: Highest 'Fire Copter' level
	// TODO: Highest 'Beach Patrol' level
	// TODO: Fastest time on 'Crims On Wings'
	// TODO: Fastest time on 'Crims On Water Wings'

	if ( PlayerStatsInteger[BEST_TIME_PCJ1] )
	{
		if ( nStat == nTempValue++ )
		{
			// Fastest time on 'Playground On The Town'
			long	nMinutes = GetMinutesFromStat(PlayerStatsInteger[BEST_TIME_PCJ1]);
			long	nSeconds = GetSecondsFromStat(PlayerStatsInteger[BEST_TIME_PCJ1]);
			BuildStatLine("ST_PCJ1", &nMinutes, STATTYPE_TIME, &nSeconds);
			return 0;
		}
	}

	if ( PlayerStatsInteger[BEST_TIME_PCJ2] )
	{
		if ( nStat == nTempValue++ )
		{
			// Fastest time on 'Playground On The Point'
			long	nMinutes = GetMinutesFromStat(PlayerStatsInteger[BEST_TIME_PCJ2]);
			long	nSeconds = GetSecondsFromStat(PlayerStatsInteger[BEST_TIME_PCJ2]);
			BuildStatLine("ST_PCJ2", &nMinutes, STATTYPE_TIME, &nSeconds);
			return 0;
		}
	}

	if ( PlayerStatsInteger[BEST_TIME_PCJ3] )
	{
		if ( nStat == nTempValue++ )
		{
			// Fastest time on 'Playground On The Dock'
			long	nMinutes = GetMinutesFromStat(PlayerStatsInteger[BEST_TIME_PCJ3]);
			long	nSeconds = GetSecondsFromStat(PlayerStatsInteger[BEST_TIME_PCJ3]);
			BuildStatLine("ST_PCJ3", &nMinutes, STATTYPE_TIME, &nSeconds);
			return 0;
		}
	}

	if ( PlayerStatsInteger[BEST_TIME_PCJ4] )
	{
		if ( nStat == nTempValue++ )
		{
			// Fastest time on 'Playground On The Park'
			long	nMinutes = GetMinutesFromStat(PlayerStatsInteger[BEST_TIME_PCJ4]);
			long	nSeconds = GetSecondsFromStat(PlayerStatsInteger[BEST_TIME_PCJ4]);
			BuildStatLine("ST_PCJ4", &nMinutes, STATTYPE_TIME, &nSeconds);
			return 0;
		}
	}

	// TODO: Fastest time on 'Land, Sea and Air Ace'
	// TODO: Fastest time on 'Hyman Memorial O.D.T.'
	// TODO: Fastest time on 'Skywolf'
	// TODO: Best score at the driving range
	// TODO: Furthest drive on the range (m)
	// TODO: Best score on 'Phil's Shooting Range'
	// TODO: Best time on 'Phil's Shooting Range'
	// TODO: Best accuracy on 'Phil's Shooting Range'

	for ( int i = 0; i < 9; ++i )
	{
		if ( PlayerStatsInteger[TURISMO1_BEST_LAP + i*3] )
		{
			if ( nStat == nTempValue++ )
			{
				// Course name
				_snprintf(gUString, sizeof(gUString), "ST_R_%02d", i + 1);
				strcpy(gString, TheText.Get(gUString));
				gUString[0] = '\0';
				return 0;
			}

			if ( nStat == nTempValue++ )
			{
				// Best Position
				BuildStatLine("ST_BPOS", &PlayerStatsInteger[TURISMO1_BEST_POSITION + i*3], STATTYPE_INT, nullptr);
				nIndents = 1;
				return 0;
			}

			if ( nStat == nTempValue++ )
			{
				// Fastest Time
				long	nMinutes = GetMinutesFromStat(PlayerStatsInteger[TURISMO1_BEST_TIME + i*3]);
				long	nSeconds = GetSecondsFromStat(PlayerStatsInteger[TURISMO1_BEST_TIME + i*3]);
				BuildStatLine("ST_FTIM", &nMinutes, STATTYPE_TIME, &nSeconds);
				nIndents = 1;
				return 0;
			}

			if ( nStat == nTempValue++ )
			{
				// Fastest Lap
				long	nMinutes = GetMinutesFromStat(PlayerStatsInteger[TURISMO1_BEST_LAP + i*3]);
				long	nSeconds = GetSecondsFromStat(PlayerStatsInteger[TURISMO1_BEST_LAP + i*3]);
				BuildStatLine("ST_FLAP", &nMinutes, STATTYPE_TIME, &nSeconds);
				nIndents = 1;
				return 0;
			}
		}
	}

	// Sanchez Time Trials
	if ( PlayerStatsInteger[DTRIAL1_BEST_LAP] )
	{
		if ( nStat == nTempValue++ )
		{
			// Sanchez Time Trial
			gUString[0] = '\0';
			strcpy(gString, TheText.Get("ST_SATT"));
			return 0;
		}

		for ( int i = 0; i < 12; ++i )
		{
			if ( PlayerStatsInteger[DTRIAL1_BEST_LAP + i*2] )
			{
				if ( nStat == nTempValue++ )
				{
					// Course X
					_snprintf(gUString, sizeof(gUString), "ST_C_%02d", i + 1);
					strcpy(gString, TheText.Get(gUString));
					gUString[0] = '\0';
					nIndents = 1;
					return 0;
				}

				if ( nStat == nTempValue++ )
				{
					// Fastest Time
					long	nMinutes = GetMinutesFromStat(PlayerStatsInteger[DTRIAL1_BEST_TIME + i*2]);
					long	nSeconds = GetSecondsFromStat(PlayerStatsInteger[DTRIAL1_BEST_TIME + i*2]);
					BuildStatLine("ST_FTIM", &nMinutes, STATTYPE_TIME, &nSeconds);
					nIndents = 2;
					return 0;
				}

				if ( nStat == nTempValue++ )
				{
					// Fastest Lap
					long	nMinutes = GetMinutesFromStat(PlayerStatsInteger[DTRIAL1_BEST_LAP + i*2]);
					long	nSeconds = GetSecondsFromStat(PlayerStatsInteger[DTRIAL1_BEST_LAP + i*2]);
					BuildStatLine("ST_FLAP", &nMinutes, STATTYPE_TIME, &nSeconds);
					nIndents = 2;
					return 0;
				}
			}
		}

		if ( nStat == nTempValue++ )
		{
			// Longest Air
			BuildStatLine("ST_MAIR", &PlayerStatsFloat[FURTHEST_HOOP_DTRIAL], STATTYPE_FLOAT, nullptr);
			nIndents = 1;
			return 0;
		}
	}

	// BMX Time Trials
	if ( PlayerStatsInteger[BTRIAL1_BEST_LAP] )
	{
		if ( nStat == nTempValue++ )
		{
			// BMX Time Trial
			gUString[0] = '\0';
			strcpy(gString, TheText.Get("ST_BMXT"));
			return 0;
		}

		for ( int i = 0; i < 8; ++i )
		{
			if ( PlayerStatsInteger[BTRIAL1_BEST_LAP + i*2] )
			{
				if ( nStat == nTempValue++ )
				{
					// Course X
					_snprintf(gUString, sizeof(gUString), "ST_C_%02d", i + 1);
					strcpy(gString, TheText.Get(gUString));
					gUString[0] = '\0';
					nIndents = 1;
					return 0;
				}

				if ( nStat == nTempValue++ )
				{
					// Fastest Time
					long	nMinutes = GetMinutesFromStat(PlayerStatsInteger[BTRIAL1_BEST_TIME + i*2]);
					long	nSeconds = GetSecondsFromStat(PlayerStatsInteger[BTRIAL1_BEST_TIME + i*2]);
					BuildStatLine("ST_FTIM", &nMinutes, STATTYPE_TIME, &nSeconds);
					nIndents = 2;
					return 0;
				}

				if ( nStat == nTempValue++ )
				{
					// Fastest Lap
					long	nMinutes = GetMinutesFromStat(PlayerStatsInteger[BTRIAL1_BEST_LAP + i*2]);
					long	nSeconds = GetSecondsFromStat(PlayerStatsInteger[BTRIAL1_BEST_LAP + i*2]);
					BuildStatLine("ST_FLAP", &nMinutes, STATTYPE_TIME, &nSeconds);
					nIndents = 2;
					return 0;
				}
			}
		}

		if ( nStat == nTempValue++ )
		{
			// Longest Air
			BuildStatLine("ST_MAIR", &PlayerStatsFloat[FURTHEST_HOOP_BTRIAL], STATTYPE_FLOAT, nullptr);
			nIndents = 1;
			return 0;
		}
	}

	// Quad Bike Time Trials
	if ( PlayerStatsInteger[QTTRIAL1_BEST_LAP] )
	{
		if ( nStat == nTempValue++ )
		{
			// Quad Bike Time Trial
			gUString[0] = '\0';
			strcpy(gString, TheText.Get("ST_QBTT"));
			return 0;
		}

		for ( int i = 0; i < 6; ++i )
		{
			if ( PlayerStatsInteger[QTTRIAL1_BEST_LAP + i*2] )
			{
				if ( nStat == nTempValue++ )
				{
					// Course X
					_snprintf(gUString, sizeof(gUString), "ST_C_%02d", i + 1);
					strcpy(gString, TheText.Get(gUString));
					gUString[0] = '\0';
					nIndents = 1;
					return 0;
				}

				if ( nStat == nTempValue++ )
				{
					// Fastest Time
					long	nMinutes = GetMinutesFromStat(PlayerStatsInteger[QTTRIAL1_BEST_TIME + i*2]);
					long	nSeconds = GetSecondsFromStat(PlayerStatsInteger[QTTRIAL1_BEST_TIME + i*2]);
					BuildStatLine("ST_FTIM", &nMinutes, STATTYPE_TIME, &nSeconds);
					nIndents = 2;
					return 0;
				}

				if ( nStat == nTempValue++ )
				{
					// Fastest Lap
					long	nMinutes = GetMinutesFromStat(PlayerStatsInteger[QTTRIAL1_BEST_LAP + i*2]);
					long	nSeconds = GetSecondsFromStat(PlayerStatsInteger[QTTRIAL1_BEST_LAP + i*2]);
					BuildStatLine("ST_FLAP", &nMinutes, STATTYPE_TIME, &nSeconds);
					nIndents = 2;
					return 0;
				}
			}
		}

		if ( nStat == nTempValue++ )
		{
			// Longest Air
			BuildStatLine("ST_MAIR", &PlayerStatsFloat[FURTHEST_HOOP_QTTRIAL], STATTYPE_FLOAT, nullptr);
			nIndents = 1;
			return 0;
		}
	}

	// Monster Truck Time Trials
	if ( PlayerStatsInteger[MTRIAL1_BEST_LAP] )
	{
		if ( nStat == nTempValue++ )
		{
			// Monster Truck Time Trial
			gUString[0] = '\0';
			strcpy(gString, TheText.Get("ST_MOTT"));
			return 0;
		}

		for ( int i = 0; i < 6; ++i )
		{
			if ( PlayerStatsInteger[MTRIAL1_BEST_LAP + i*2] )
			{
				if ( nStat == nTempValue++ )
				{
					// Course X
					_snprintf(gUString, sizeof(gUString), "ST_C_%02d", i + 1);
					strcpy(gString, TheText.Get(gUString));
					gUString[0] = '\0';
					nIndents = 1;
					return 0;
				}

				if ( nStat == nTempValue++ )
				{
					// Fastest Time
					long	nMinutes = GetMinutesFromStat(PlayerStatsInteger[MTRIAL1_BEST_TIME + i*2]);
					long	nSeconds = GetSecondsFromStat(PlayerStatsInteger[MTRIAL1_BEST_TIME + i*2]);
					BuildStatLine("ST_FTIM", &nMinutes, STATTYPE_TIME, &nSeconds);
					nIndents = 2;
					return 0;
				}

				if ( nStat == nTempValue++ )
				{
					// Fastest Lap
					long	nMinutes = GetMinutesFromStat(PlayerStatsInteger[MTRIAL1_BEST_LAP + i*2]);
					long	nSeconds = GetSecondsFromStat(PlayerStatsInteger[MTRIAL1_BEST_LAP + i*2]);
					BuildStatLine("ST_FLAP", &nMinutes, STATTYPE_TIME, &nSeconds);
					nIndents = 2;
					return 0;
				}
			}
		}

		if ( nStat == nTempValue++ )
		{
			// Longest Air
			BuildStatLine("ST_MAIR", &PlayerStatsFloat[FURTHEST_HOOP_MTRIAL], STATTYPE_FLOAT, nullptr);
			nIndents = 1;
			return 0;
		}
	}

	if ( PlayerStatsInteger[PHOTOS_TAKEN] )
	{
		if ( nStat == nTempValue++ )
		{
			// Photos taken
			BuildStatLine("ST_PHOT", &PlayerStatsInteger[PHOTOS_TAKEN], STATTYPE_INT, nullptr);
			return 0;
		}
	}

	if ( nStat == nTempValue++ )
	{
		// Fish fed
		BuildStatLine("ST_DRWN", &PlayerStatsInteger[TIMES_DROWNED], STATTYPE_INT, nullptr);
		return 0;
	}

	// TODO: Seagulls Sniped

	if ( nStat == nTempValue++ )
	{
		// Pay 'n' Spray visits
		BuildStatLine("SPRAYIN", &PlayerStatsInteger[SPRAYINGS], STATTYPE_INT, nullptr);
		return 0;
	}

	if ( nStat == nTempValue++ )
	{
		// Weapon budget
		BuildStatLine("ST_WEAP", &PlayerStatsFloat[WEAPON_BUDGET], STATTYPE_FMONEY, nullptr);
		return 0;
	}

	if ( nStat == nTempValue++ )
	{
		// Auto repair and painting budget
		BuildStatLine("ST_AUTO", &PlayerStatsFloat[SPRAYING_BUDGET], STATTYPE_FMONEY, nullptr);
		return 0;
	}

	if ( nStat == nTempValue++ )
	{
		// Property destroyed
		float	fBudget = static_cast<float>(PlayerStatsInteger[PROPERTY_DESTROYED]);
		BuildStatLine("ST_DAMA", &fBudget, STATTYPE_FMONEY, nullptr);
		return 0;
	}

	// TODO: Highest media attention

	if ( nStat == nTempValue++ )
	{
		// Outfit changes
		BuildStatLine("TMSOUT", &PlayerStatsInteger[OUTFIT_CHANGES], STATTYPE_INT, nullptr);
		return 0;
	}

	// TODO: Unlocked outfits

	return nTempValue;
}


void CStats::BuildStatLine(char* pEntryName, void* pVal1, int nType, void* pVal2)
{
	if ( pEntryName )
	{
		const char*		pText = TheText.Get(pEntryName);

		gUString[0] = '\0';
		strncpy(gString, pText, sizeof(gString));
		if ( pVal1 )
		{
			if ( !pVal2 )
			{
				switch ( nType )
				{
				case STATTYPE_INT:
					{
						_snprintf(gUString, sizeof(gUString), "%d", *static_cast<int*>(pVal1));
						return;
					}
				case STATTYPE_FLOAT:
					{
						_snprintf(gUString, sizeof(gUString), "%.2f", *static_cast<float*>(pVal1));
						return;
					}
				case STATTYPE_PERCENT:
					{
						_snprintf(gUString, sizeof(gUString), "%d%%", *static_cast<int*>(pVal1));
						return;
					}
				case STATTYPE_FPERCENT:
					{
						_snprintf(gUString, sizeof(gUString), "%.1f%%", *static_cast<float*>(pVal1));
						return;
					}
				case STATTYPE_FMONEY:
					{
						_snprintf(gUString, sizeof(gUString), "$%.2f", *static_cast<float*>(pVal1));
						return;
					}
				}
			}
			else
			{
				switch ( nType )
				{
				case STATTYPE_TIME:
					{
						_snprintf(gUString, sizeof(gUString), "%d:%02d", *static_cast<int*>(pVal1), *static_cast<int*>(pVal2));
						return;
					}
				case STATTYPE_OUTOF:
					{
						_snprintf(gUString, sizeof(gUString), "%d %s %d", *static_cast<int*>(pVal1), TheText.Get("FEST_OO"), *static_cast<int*>(pVal2));
						return;
					}
				}
			}
		}
	}
}