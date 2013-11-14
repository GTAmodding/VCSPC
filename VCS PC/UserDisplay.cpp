#include "StdAfx.h"

CPager			CUserDisplay::Pager;
COnscreenTimer	CUserDisplay::OnscnTimer;

void CPager::Display()
{
	unsigned short	wCurChar = 0;
	char			cOutMessage[16];
	bool			bShouldFixSpaces = false;

	if ( Messages[0].pMessage )
	{
		char	cOutBuffer[256];

		CMessages::InsertNumberInString(Messages[0].pMessage, Messages[0].nMessageNumbers[0], Messages[0].nMessageNumbers[1], Messages[0].nMessageNumbers[2], Messages[0].nMessageNumbers[3], Messages[0].nMessageNumbers[4], Messages[0].nMessageNumbers[5], cOutBuffer);

		while ( wCurChar < bLetterSlots )
		{
			long	nFirstLetter = Messages[0].wFirstShownLetter + wCurChar;

			if ( nFirstLetter >= 0 )
			{
				char	cLetterToCopy = cOutBuffer[nFirstLetter];
				if ( cLetterToCopy )
				{
					bShouldFixSpaces = true;
					cOutMessage[wCurChar++] = cLetterToCopy;
				}
				else
					break;
			}
			else
				cOutMessage[wCurChar++] = ' ';
		}
	}
	cOutMessage[wCurChar] = '\0';
	if ( bShouldFixSpaces )
	{
		while ( wCurChar && cOutMessage[--wCurChar] == ' ' )
			cOutMessage[wCurChar] = '\0';
	}

	CHud::SetPagerMessage(cOutMessage);
}

void CPager::Init()
{
	bLetterSlots = 14;
	for ( int i = 0; i < NUM_MAX_PAGER_MESSAGES; ++i )
		Messages[i].pMessage = NULL;
}

void CPager::ClearMessages()
{
	for ( int i = 0; i < NUM_MAX_PAGER_MESSAGES; ++i )
		Messages[i].pMessage = NULL;
}

void CPager::AddMessage(char* pMsg, short nSpeed, short nPriority)
{
	unsigned char	bLoopCounter = 0;

	while ( bLoopCounter < NUM_MAX_PAGER_MESSAGES )
	{
		if ( Messages[bLoopCounter].pMessage )
		{
			if ( Messages[bLoopCounter].wPriority < nPriority )
			{
				if ( bLoopCounter < NUM_MAX_PAGER_MESSAGES - 1 )
				{
					unsigned char bStackPointer = NUM_MAX_PAGER_MESSAGES - 1;
					do
					{
						Messages[bStackPointer].pMessage = Messages[bStackPointer - 1].pMessage;
						Messages[bStackPointer].wSpeed = Messages[bStackPointer - 1].wSpeed;
						Messages[bStackPointer].wFirstShownLetter = Messages[bStackPointer - 1].wFirstShownLetter;
						Messages[bStackPointer].wStringLength = Messages[bStackPointer - 1].wStringLength;
						Messages[bStackPointer].wPriority = Messages[bStackPointer - 1].wPriority;
						Messages[bStackPointer].nLastLetterChangeTime = Messages[bStackPointer - 1].nLastLetterChangeTime;
						Messages[bStackPointer].nMessageNumbers[0] = Messages[bStackPointer - 1].nMessageNumbers[0];
						Messages[bStackPointer].nMessageNumbers[1] = Messages[bStackPointer - 1].nMessageNumbers[1];
						Messages[bStackPointer].nMessageNumbers[2] = Messages[bStackPointer - 1].nMessageNumbers[2];
						Messages[bStackPointer].nMessageNumbers[3] = Messages[bStackPointer - 1].nMessageNumbers[3];
						Messages[bStackPointer].nMessageNumbers[4] = Messages[bStackPointer - 1].nMessageNumbers[4];
						Messages[bStackPointer].nMessageNumbers[5] = Messages[bStackPointer - 1].nMessageNumbers[5];
					}
					while ( --bStackPointer > bLoopCounter );
				}
				Messages[bLoopCounter].pMessage = pMsg;
				Messages[bLoopCounter].wSpeed = nSpeed;
				Messages[bLoopCounter].wFirstShownLetter = -(bLetterSlots+10);
				Messages[bLoopCounter].wStringLength = static_cast<short>(strlen(pMsg));
				Messages[bLoopCounter].wPriority = nPriority;
				Messages[bLoopCounter].nLastLetterChangeTime = CTimer::m_snTimeInMilliseconds + nSpeed;
				Messages[bLoopCounter].nMessageNumbers[0] = -1;
				Messages[bLoopCounter].nMessageNumbers[1] = -1;
				Messages[bLoopCounter].nMessageNumbers[2] = -1;
				Messages[bLoopCounter].nMessageNumbers[3] = -1;
				Messages[bLoopCounter].nMessageNumbers[4] = -1;
				Messages[bLoopCounter].nMessageNumbers[5] = -1;
				break;
			}
		}
		else
		{
			Messages[bLoopCounter].pMessage = pMsg;
			Messages[bLoopCounter].wSpeed = nSpeed;
			Messages[bLoopCounter].wFirstShownLetter = -(bLetterSlots+10);
			Messages[bLoopCounter].wStringLength = static_cast<short>(strlen(pMsg));
			Messages[bLoopCounter].wPriority = nPriority;
			Messages[bLoopCounter].nLastLetterChangeTime = CTimer::m_snTimeInMilliseconds + nSpeed;
			Messages[bLoopCounter].nMessageNumbers[0] = -1;
			Messages[bLoopCounter].nMessageNumbers[1] = -1;
			Messages[bLoopCounter].nMessageNumbers[2] = -1;
			Messages[bLoopCounter].nMessageNumbers[3] = -1;
			Messages[bLoopCounter].nMessageNumbers[4] = -1;
			Messages[bLoopCounter].nMessageNumbers[5] = -1;
			break;
		}
		++bLoopCounter;
	}
	if ( !bLoopCounter )
		CMessages::AddToPreviousBriefArray(Messages[0].pMessage, -1, -1, -1, -1, -1, -1, NULL);
}

void CPager::Process()
{
	if ( Messages[0].pMessage && Messages[0].wFirstShownLetter >= Messages[0].wStringLength )
	{
		unsigned char	bLoopCounter = 0;

		while ( bLoopCounter < NUM_MAX_PAGER_MESSAGES - 1 )
		{
			if ( !Messages[bLoopCounter + 1].pMessage )
				break;

			Messages[bLoopCounter].pMessage = Messages[bLoopCounter + 1].pMessage;
			Messages[bLoopCounter].wSpeed = Messages[bLoopCounter + 1].wSpeed;
			Messages[bLoopCounter].wFirstShownLetter = Messages[bLoopCounter + 1].wFirstShownLetter;
			Messages[bLoopCounter].wStringLength = Messages[bLoopCounter + 1].wStringLength;
			Messages[bLoopCounter].wPriority = Messages[bLoopCounter + 1].wPriority;
			Messages[bLoopCounter].nLastLetterChangeTime = Messages[bLoopCounter + 1].nLastLetterChangeTime;
			Messages[bLoopCounter].nMessageNumbers[0] = Messages[bLoopCounter + 1].nMessageNumbers[0];
			Messages[bLoopCounter].nMessageNumbers[1] = Messages[bLoopCounter + 1].nMessageNumbers[1];
			Messages[bLoopCounter].nMessageNumbers[2] = Messages[bLoopCounter + 1].nMessageNumbers[2];
			Messages[bLoopCounter].nMessageNumbers[3] = Messages[bLoopCounter + 1].nMessageNumbers[3];
			Messages[bLoopCounter].nMessageNumbers[4] = Messages[bLoopCounter + 1].nMessageNumbers[4];
			Messages[bLoopCounter].nMessageNumbers[5] = Messages[bLoopCounter + 1].nMessageNumbers[5];

			++bLoopCounter;
		}

		Messages[bLoopCounter].pMessage = NULL;
		if ( Messages[0].pMessage )
			CMessages::AddToPreviousBriefArray(Messages[0].pMessage, Messages[0].nMessageNumbers[0], Messages[0].nMessageNumbers[1], Messages[0].nMessageNumbers[2], Messages[0].nMessageNumbers[3], Messages[0].nMessageNumbers[4], Messages[0].nMessageNumbers[5], NULL);
	}
	Display();

	if ( Messages[0].pMessage )
	{
		if ( camera->m_WideScreenOn || !*bWants_To_Draw_Hud || StyledText_1[0] )
		{
			Messages[0].wFirstShownLetter = -(bLetterSlots+10);
			Messages[0].nLastLetterChangeTime = CTimer::m_snTimeInMilliseconds + Messages[0].wSpeed;
		}
		else
		{
			if ( CTimer::m_snTimeInMilliseconds > Messages[0].nLastLetterChangeTime )
			{
				++Messages[0].wFirstShownLetter;
				Messages[0].nLastLetterChangeTime = CTimer::m_snTimeInMilliseconds + Messages[0].wSpeed;
			}
		}
	}
}

void COnscreenTimer::Init()
{
	m_bEnabled = true;
	m_bPaused = false;
	m_bColourOverridden = false;
	m_bShadowColourOverridden = false;

	m_timer.m_pVariable = nullptr;
	m_timer.m_bShown = false;

	for ( int i = 0; i < NUM_ONSCREEN_COUNTERS; ++i )
	{
		m_counter[i].m_pVariable = nullptr;
		m_counter[i].m_bShown = false;
	}
}

void COnscreenTimer::ProcessForDisplay()
{
	m_bEnabled = false;

	// Timer
	if ( m_timer.m_pVariable )
	{
		m_bEnabled = true;
		m_timer.m_bShown = true;
		_snprintf(m_timer.m_cDisplayedText, sizeof(m_timer.m_cDisplayedText), "%d:%02d", *m_timer.m_pVariable / 1000 / 60 % 100, *m_timer.m_pVariable / 1000 % 60);
	}
	else
		m_timer.m_bShown = false;

	// Counters
	for ( int i = 0; i < NUM_ONSCREEN_COUNTERS; ++i )
	{
		if ( m_counter[i].m_pVariable )
		{
			int		nNum, nSecondNum;

			nNum = *m_counter[i].m_pVariable;
			nSecondNum = m_counter[i].m_pSecondVariable ? *m_counter[i].m_pSecondVariable : 4;

			switch ( m_counter[i].m_wType )
			{
			case 1:
				m_counter[i].m_counterData.nBarFill = nNum;
				break;
			case 2:
				_snprintf(m_counter[i].m_counterData.cDisplayedText, sizeof(m_counter[i].m_counterData.cDisplayedText), "%d/%d", nNum, nSecondNum);
				break;
			default:
				_snprintf(m_counter[i].m_counterData.cDisplayedText, sizeof(m_counter[i].m_counterData.cDisplayedText), m_counter[i].m_bShowsDollar ? "$%d" : "%d", nNum);
				break;
			}

			m_bEnabled = true;
			m_counter[i].m_bShown = true;
		}
	}
}

void COnscreenTimer::Process()
{
	if ( CReplay::Mode != 1 && !m_bPaused )
	{
		if ( m_timer.m_pVariable )
		{
			int			nStep = static_cast<int>(CTimer::ms_fTimeStep * 0.02 * 1000.0);
			int			nPrevTime = *m_timer.m_pVariable;

			if ( m_timer.m_bCountsDown )
			{
				*m_timer.m_pVariable -= nStep;
				if ( nPrevTime >= 0 )
				{
					if ( nPrevTime / 1000 < m_timer.m_nSoundOnTime && !camera->m_WideScreenOn )
					{
						MusicManager->ReportFrontendAudioEvent(33, 0.0f, 1.0f);
						m_timer.m_bFlashing = true;
					}
					else
						m_timer.m_bFlashing = false;
				}
				else
				{
					*m_timer.m_pVariable = 0;
					m_timer.m_pVariable = nullptr;
					m_timer.m_bShown = false;
				}
			}
			else
				*m_timer.m_pVariable += nStep;
		}						
	}
}

void COnscreenTimer::AddClock(int* pVar, const char* pGXT, bool bCountsDown)
{
	if ( m_timer.m_pVariable )
		return;

	m_timer.m_pVariable = pVar;
	m_timer.m_bCountsDown = bCountsDown;
	m_timer.m_nSoundOnTime = 11;

	if ( m_bColourOverridden )
	{
		m_bColourOverridden = false;
		m_timer.m_colour = m_customColour;
	}
	else
		m_timer.m_colour = BaseColors[9];

	if ( m_bShadowColourOverridden )
	{
		m_bShadowColourOverridden = false;
		m_timer.m_shadowColour = m_customShadowColour;
	}
	else
	{
		m_timer.m_shadowColour.r = 0;
		m_timer.m_shadowColour.g = 0;
		m_timer.m_shadowColour.b = 0;
		m_timer.m_shadowColour.a = 255;
	}

	if ( pGXT )
		strncpy(m_timer.m_cGXTentry, pGXT, 8);
	else
		m_timer.m_cGXTentry[0] = '\0';
}

void COnscreenTimer::AddCounter(int* pVar, unsigned short wType, const char* pGXT, unsigned short wLine)
{
	if ( m_counter[wLine].m_pVariable )
		return;

	m_counter[wLine].m_pVariable = pVar;
	m_counter[wLine].m_pSecondVariable = nullptr;
	m_counter[wLine].m_wType = wType;
	m_counter[wLine].m_bFlashing = true;
	m_counter[wLine].m_bShowsDollar = false;
	m_counter[wLine].m_bForceRecheck = false;
	m_counter[wLine].m_wOwnsWhat = 0xFFFF;

	if ( m_bColourOverridden )
	{
		m_bColourOverridden = false;
		m_counter[wLine].m_colour = m_customColour;
	}
	else
		m_counter[wLine].m_colour = BaseColors[8];

	if ( m_bShadowColourOverridden )
	{
		m_bShadowColourOverridden = false;
		m_counter[wLine].m_shadowColour = m_customShadowColour;
	}
	else
	{
		m_counter[wLine].m_shadowColour.r = 0;
		m_counter[wLine].m_shadowColour.g = 0;
		m_counter[wLine].m_shadowColour.b = 0;
		m_counter[wLine].m_shadowColour.a = 255;
	}

	if ( pGXT )
		strncpy(m_counter[wLine].m_cGXTentry, pGXT, 8);
	else
		m_counter[wLine].m_cGXTentry[0] = '\0';
}

void COnscreenTimer::AddCounterTwoVars(int* pVar, int* pSecVar, const char* pGXT, unsigned short wLine, const char* pChildGXT, unsigned short wChildLine)
{
	if ( !m_counter[wLine].m_pVariable )
	{
		m_counter[wLine].m_pVariable = pVar;
		m_counter[wLine].m_pSecondVariable = pSecVar;
		m_counter[wLine].m_wType = 2;
		m_counter[wLine].m_bFlashing = true;
		m_counter[wLine].m_bShowsDollar = false;
		m_counter[wLine].m_bForceRecheck = false;
		m_counter[wLine].m_wOwnsWhat = wChildLine;

		if ( m_bColourOverridden )
			m_counter[wLine].m_colour = m_customColour;
		else
			m_counter[wLine].m_colour = BaseColors[8];

		if ( m_bShadowColourOverridden )
			m_counter[wLine].m_shadowColour = m_customShadowColour;
		else
		{
			m_counter[wLine].m_shadowColour.r = 0;
			m_counter[wLine].m_shadowColour.g = 0;
			m_counter[wLine].m_shadowColour.b = 0;
			m_counter[wLine].m_shadowColour.a = 255;
		}

		if ( pGXT )
			strncpy(m_counter[wLine].m_cGXTentry, pGXT, 8);
		else
			m_counter[wLine].m_cGXTentry[0] = '\0';
	}

	if ( pChildGXT )
	{
		strncpy(m_counter[wChildLine].m_cGXTentry, pChildGXT, 8);

		m_counter[wChildLine].m_wType = 3;
		m_counter[wChildLine].m_pVariable = nullptr;
		m_counter[wChildLine].m_pSecondVariable = nullptr;
		m_counter[wChildLine].m_wOwnsWhat = 0xFFFF;
		m_counter[wChildLine].m_bShown = true;
		m_counter[wChildLine].m_bFlashing = true;
		m_counter[wChildLine].m_bForceRecheck = true;

		if ( m_bColourOverridden )
			m_counter[wChildLine].m_colour = m_customColour;
		else
			m_counter[wChildLine].m_colour = BaseColors[8];

		if ( m_bShadowColourOverridden )
			m_counter[wChildLine].m_shadowColour = m_customShadowColour;
		else
		{
			m_counter[wChildLine].m_shadowColour.r = 0;
			m_counter[wChildLine].m_shadowColour.g = 0;
			m_counter[wChildLine].m_shadowColour.b = 0;
			m_counter[wChildLine].m_shadowColour.a = 255;
		}
	}

	m_bColourOverridden = false;
	m_bShadowColourOverridden = false;
}

void COnscreenTimer::ClearClock(int* pVar)
{
	if ( m_timer.m_pVariable == pVar )
	{
		m_timer.m_pVariable = nullptr;
		m_timer.m_cGXTentry[0] = '\0';
		m_timer.m_bShown = false;
	}
}

void COnscreenTimer::ClearCounter(int* pVar)
{
	for ( int i = 0; i < NUM_ONSCREEN_COUNTERS; ++i )
	{
		if ( m_counter[i].m_pVariable == pVar )
		{
			m_counter[i].m_pVariable = nullptr;
			m_counter[i].m_cGXTentry[0] = '\0';
			m_counter[i].m_bShown = false;

			if ( m_counter[i].m_wOwnsWhat != 0xFFFF )
			{
				m_counter[m_counter[i].m_wOwnsWhat].m_cGXTentry[0] = '\0';
				m_counter[m_counter[i].m_wOwnsWhat].m_bShown = false;
			}
			return;
		}
	}
}

void COnscreenTimer::SetFlashing(int* pVar, bool bFlash)
{
	for ( int i = 0; i < NUM_ONSCREEN_COUNTERS; ++i )
	{
		if ( m_counter[i].m_pVariable == pVar )
		{
			m_counter[i].m_bFlashing = bFlash;
			return;
		}
	}
}

void COnscreenTimer::SetDollarFormat(int* pVar, bool bDollarFormat)
{
	for ( int i = 0; i < NUM_ONSCREEN_COUNTERS; ++i )
	{
		if ( m_counter[i].m_pVariable == pVar )
		{
			m_counter[i].m_bShowsDollar = bDollarFormat;
			return;
		}
	}
}

void COnscreenTimer::SetSoundOnTime(int* pVar, int nSoundOnTime)
{
	if ( m_timer.m_pVariable == pVar )
		m_timer.m_nSoundOnTime = nSoundOnTime;
}


void CUserDisplay::Init()
{
	OnscnTimer.Init();
	Pager.Init();
}

void CUserDisplay::Process()
{
	DWORD dwFunc = 0x5720A0;
	_asm call dwFunc

	Pager.Process();
}