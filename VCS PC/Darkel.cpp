#include "StdAfx.h"
#include "Darkel.h"

#include "Stats.h"
#include "Replay.h"
#include "Font.h"
#include "Timer.h"
#include "Messages.h"
#include "Text.h"
#include "World.h"

bool&			CDarkel::bNeedHeadShot = *(bool*)0x969A49;
bool&			CDarkel::bStandardSoundAndMessages = *(bool*)0x969A4A;
unsigned int&	CDarkel::KillsNeeded = *(unsigned int*)0x96A6EC;
char*&			CDarkel::pStartMessage = *(char**)0x96A6D0;
int&			CDarkel::TimeOfFrenzyStart = *(int*)0x96A6E0;
int&			CDarkel::TimeLimit = *(int*)0x96A6E8;
eWeaponType&	CDarkel::WeaponType = *(eWeaponType*)0x96A700;
unsigned short&	CDarkel::Status = *(unsigned short*)0x96A704;

unsigned int	CDarkel::TotalKillsNeeded;

// Wrappers
WRAPPER void CDarkel::StartFrenzy(eWeaponType weapType, int nTimeLimit, unsigned short wKillsNeeded, int nModelToKill, char* pMsg, int nModelToKill2, int nModelToKill3, int nModelToKill4, bool bSoundMessages, bool bHeadShots)
{ WRAPARG(weapType); WRAPARG(nTimeLimit); WRAPARG(wKillsNeeded); WRAPARG(nModelToKill); WRAPARG(pMsg); WRAPARG(nModelToKill2); WRAPARG(nModelToKill3); WRAPARG(nModelToKill4); WRAPARG(bSoundMessages); WRAPARG(bHeadShots); EAXJMP(0x43D3B0); }
WRAPPER bool CDarkel::ThisPedShouldBeKilledForFrenzy(CPed* pPed) { WRAPARG(pPed); EAXJMP(0x43D2F0); }

void CDarkel::Init(void)
{
	Status = 0;
}

void CDarkel::StartFrenzyVCS(eWeaponType weapType, int nTimeLimit, unsigned short wKillsNeeded, int nModelToKill, char* pMsg, int nModelToKill2, int nModelToKill3, int nModelToKill4, bool bSoundMessages, bool bHeadShots)
{
	TotalKillsNeeded = wKillsNeeded;
	StartFrenzy(weapType, nTimeLimit, wKillsNeeded, nModelToKill, pMsg, nModelToKill2, nModelToKill3, nModelToKill4, bSoundMessages, bHeadShots);
}

void CDarkel::RegisterKillByPlayerVehicle(CPed* pKilledPed, eWeaponType lastWeaponDamage)
{
	if ( FrenzyOnGoing() )
	{
		if ( (lastWeaponDamage == WEAPONTYPE_RAMMEDBYCAR || lastWeaponDamage == WEAPONTYPE_RUNOVERBYCAR) && WeaponType == WEAPONTYPE_RUNOVERBYCAR )
		{
			if ( ThisPedShouldBeKilledForFrenzy(pKilledPed) )
				--KillsNeeded;

			CStats::IncrementStat(pKilledPed->GetPedType() == 6 ? HIGHEST_POLICE_KILLS_ON_SPREE : HIGHEST_PED_KILLS_ON_SPREE, 1.0f);
		}
	}
	CStats::IncrementStat(PEOPLE_KILLED_BY_PLAYER, 1.0f);
}

void CDarkel::DrawMessages()
{
	if ( CReplay::Mode == 1 )
		return;

	switch ( Status )
	{
	case 1:
	case 4:
		{
			bool	bIs2pRampage = CWorld::Players[1].GetPed() != nullptr;

			if ( pStartMessage )
			{
				if ( bStandardSoundAndMessages )
				{
					if ( CTimer::m_snTimeInMilliseconds - TimeOfFrenzyStart < 11000 && CTimer::m_snTimeInMilliseconds - TimeOfFrenzyStart >= 3000 )
						CMessages::AddBigMessage(pStartMessage, 3000, 0);
				}
				else
				{
					if ( CTimer::m_snTimeInMilliseconds - TimeOfFrenzyStart < 8000 )
						CMessages::AddBigMessage(pStartMessage, 3000, 0);
				}
			}
			CFont::SetBackground(0, 0);
			CFont::SetScale(_width(0.5f), _height(1.1f));
			CFont::SetOrientation(ALIGN_Right);
			CFont::SetRightJustifyWrap(0.0f);
			CFont::SetFontStyle(FONT_Eurostile);
			CFont::SetEdge(1);
			CFont::SetDropColor(CRGBA(0, 0, 0, 255));
			CFont::SetColor(BaseColors[4]);

			if ( TimeLimit >= 0 )
			{
				int		nTimeLeft = TimeOfFrenzyStart + TimeLimit - CTimer::m_snTimeInMilliseconds;
				float	fPosXOffset;
				int		nTextLength;

				sprintf(gString, "%d:%02d", nTimeLeft / 60000, (nTimeLeft % 60000) / 1000);
				nTextLength = strlen(gString);
				if ( nTextLength > 4 )
					fPosXOffset = 12.5f * (nTextLength - 4);
				else
					fPosXOffset = 0.0f;

				// TODO: Flashing
				CFont::SetProportional(true);
				CFont::PrintString(_x(88.0f + fPosXOffset), _y(!bIs2pRampage ? 148.0f : 148.0f + 50.0f), TheText.Get("TIME"));

				CFont::SetProportional(false);
				CFont::SetColor(BaseColors[11]);
				CFont::PrintString(_x(32.0f), _y(!bIs2pRampage ? 148.0f : 148.0f + 50.0f), gString);
			}

			float	fPosXOffset;
			int		nTextLength;

			_snprintf(gString, sizeof(gString), "%d/%d", TotalKillsNeeded - Max<unsigned int>(0, KillsNeeded), TotalKillsNeeded);
			nTextLength = strlen(gString);
			if ( nTextLength > 4 )
				fPosXOffset = 12.5f * (nTextLength - 4);
			else
				fPosXOffset = 0.0f;

			CFont::SetProportional(true);
			CFont::SetColor(BaseColors[8]);
			CFont::PrintString(_x(88.0f + fPosXOffset), _y(!bIs2pRampage ? 175.0f : 175.0f + 50.0f), TheText.Get("RAMP_KL"));

			CFont::SetProportional(false);
			CFont::PrintString(_x(32.0f), _y(!bIs2pRampage ? 175.0f : 175.0f + 50.0f), gString);
			return;
		}
	case 2:
		{
			if ( bStandardSoundAndMessages )
			{
				if ( CTimer::m_snTimeInMilliseconds - TimeOfFrenzyStart < 5000 )
					CMessages::AddBigMessage(TheText.Get("KILLPA"), 3000, 0);
			}
			return;
		}
	}
}