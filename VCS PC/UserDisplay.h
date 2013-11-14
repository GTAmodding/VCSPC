#ifndef __USERDISPLAY
#define __USERDISPLAY

#define NUM_MAX_PAGER_MESSAGES	8

#define NUM_ONSCREEN_COUNTERS	4

class CPager
{
private:
	unsigned char		bLetterSlots;
	struct
	{
		char*				pMessage;
		short				wSpeed;
		short				wFirstShownLetter;
		short				wStringLength;
		short				wPriority;
		int					nLastLetterChangeTime;
		int					nMessageNumbers[6];
	}					Messages[NUM_MAX_PAGER_MESSAGES];

private:
	void				Display();

public:
	void				Init();
	void				ClearMessages();
	void				AddMessage(char* pMsg, short nSpeed, short nPriority);
	void				Process();
};

class COnscreenTimerEntry
{
public:
	int*				m_pVariable;
	char				m_cGXTentry[8];
	char				m_cDisplayedText[32];
	bool				m_bShown, m_bCountsDown, m_bFlashing;
	CRGBA				m_colour, m_shadowColour;
	int					m_nSoundOnTime;
};

class COnscreenCounterEntry
{
public:
	int*				m_pVariable;
	int*				m_pSecondVariable;
	char				m_cGXTentry[8];
	union
	{
		int				nBarFill;
		char			cDisplayedText[32];
	}					m_counterData;
	unsigned short		m_wType, m_wOwnsWhat;
	bool				m_bShown, m_bFlashing, m_bShowsDollar, m_bForceRecheck;
	CRGBA				m_colour, m_shadowColour;
};

class COnscreenTimer
{
private:
	COnscreenTimerEntry		m_timer;
	COnscreenCounterEntry	m_counter[NUM_ONSCREEN_COUNTERS];

	CRGBA					m_customColour, m_customShadowColour;
	bool					m_bColourOverridden, m_bShadowColourOverridden;

public:	// For patching
	bool					m_bEnabled, m_bPaused;

public:
	inline bool				IsEnabled()
		{ return m_bEnabled; };

	inline COnscreenTimerEntry*	GetTimer()
		{ return &m_timer; };

	inline COnscreenCounterEntry* GetCounter()
		{ return m_counter; };  

	inline void				SetColourOverride(BYTE bR, BYTE bG, BYTE bB, BYTE bA)
		{ m_bColourOverridden = true; m_customColour.r = bR; m_customColour.g = bG; m_customColour.b = bB; m_customColour.a = bA; };
	
	inline void				SetShadowColourOverride(BYTE bR, BYTE bG, BYTE bB, BYTE bA)
		{ m_bShadowColourOverridden = true; m_customShadowColour.r = bR; m_customShadowColour.g = bG; m_customShadowColour.b = bB; m_customShadowColour.a = bA; };

	void					Init();
	void					ProcessForDisplay();
	void					Process();
	void					AddClock(int* pVar, const char* pGXT, bool bCountsDown);
	void					AddCounter(int* pVar, unsigned short wType, const char* pGXT, unsigned short wLine);
	void					AddCounterTwoVars(int* pVar, int* pSecVar, const char* pGXT, unsigned short wLine, const char* pChildGXT = nullptr, unsigned short wChildLine = 0xFFFF);
	void					ClearClock(int* pVar);
	void					ClearCounter(int* pVar);
	void					SetFlashing(int* pVar, bool bFlash);
	void					SetDollarFormat(int* pVar, bool bDollarFormat);
	void					SetSoundOnTime(int* pVar, int nSoundOnTime);
};

class CUserDisplay
{
public:
	static CPager			Pager;
	static COnscreenTimer	OnscnTimer;

public:
	static void				Init();
	static void				Process();
};

#endif