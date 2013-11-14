#include "StdAfx.h"

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
WRAPPER bool CDarkel::func_43D2F0(CPed* pPed) { WRAPARG(pPed); EAXJMP(0x43D2F0); }


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
			if ( func_43D2F0(pKilledPed) )
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
			CFont::SetTextBackground(0, 0);
			CFont::SetTextLetterSize(_width(0.5f), _height(1.1f));
			CFont::SetTextAlignment(ALIGN_Right);
			CFont::SetTextWrapX(0.0f);	// ?
			CFont::SetFontStyle(FONT_Eurostile);
			CFont::SetTextOutline(1);
			CFont::SetTextBorderRGBA(CRGBA(0, 0, 0, 255));
			CFont::SetTextColour(BaseColors[4]);

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
				CFont::SetTextUseProportionalValues(true);
				CFont::PrintString(_x(88.0f + fPosXOffset), _y(148.0f), gxt->GetText("TIME"));

				CFont::SetTextUseProportionalValues(false);
				CFont::SetTextColour(BaseColors[11]);
				CFont::PrintString(_x(32.0f), _y(148.0f), gString);
			}

			float	fPosXOffset;
			int		nTextLength;

			sprintf(gString, "%d/%d", TotalKillsNeeded - max(0, KillsNeeded), TotalKillsNeeded);
			nTextLength = strlen(gString);
			if ( nTextLength > 4 )
				fPosXOffset = 12.5f * (nTextLength - 4);
			else
				fPosXOffset = 0.0f;

			CFont::SetTextUseProportionalValues(true);
			CFont::SetTextColour(BaseColors[8]);
			CFont::PrintString(_x(88.0f + fPosXOffset), _y(175.0f), gxt->GetText("RAMP_KL"));

			CFont::SetTextUseProportionalValues(false);
			CFont::PrintString(_x(32.0f), _y(175.0f), gString);
			return;
		}
	case 2:
		{
			if ( bStandardSoundAndMessages )
			{
				if ( CTimer::m_snTimeInMilliseconds - TimeOfFrenzyStart < 5000 )
					CMessages::AddBigMessage(gxt->GetText("KILLPA"), 3000, 0);
			}
			return;
		}
	}
}