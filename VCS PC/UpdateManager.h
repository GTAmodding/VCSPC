#ifndef __UPDATEMGR
#define __UPDATEMGR

#include "..\common\Updater.h"

#define NUM_MESSAGES_PER_UPT_SCREEN		7

class CUpdateManager
{
private:
	static IUpdaterClient001*	pUptModuleInterface;
	static HMODULE				hUptModuleLibrary;

	static unsigned long		nActiveMessages;
	static long					nInterfaceStatus;
	static long					nLastInterfaceStatus;
	static long					nDrawingState;
	static bool					bSeenUpdaterScreenYet;
	static char*				pMessages[NUM_MESSAGES_PER_UPT_SCREEN];

#ifdef DEVBUILD
	static bool					bDisableAutoCheck;
#endif

public:
	static inline bool			AnyTextDisplaysNow()
		{ return nDrawingState != 0; }

	static inline  bool			IsDownloading()
	{ return nInterfaceStatus == UPTMODULESTATE_DOWNLOADING || nInterfaceStatus == UPTMODULESTATE_ALL_READY; }

	static inline bool			AreUpdatesSeen()
		{ return bSeenUpdaterScreenYet; }

	static inline void			ReportUpdaterScreenSeen()
		{ bSeenUpdaterScreenYet = true; }

	static inline bool			GetDLCStatus(const char* pName, bool bDefault)
		{ return pUptModuleInterface->GetDLCStatus(pName, bDefault); }

	static inline void			SetDLCStatus(const char* pName, bool bStatus)
		{ pUptModuleInterface->AddThisDLCToList(pName, bStatus); }

#ifdef DEVBUILD
	static inline void			DisableAutoCheck()
		{ bDisableAutoCheck = true; }
#endif

	static void					Init();
	static void					Terminate();
	static void					Process();
	static void					Display();
	static void					HandleButtonClick();
	static void					InstallIfNeeded();
	static bool					UpdatesAvailable();
	static const char*			GetGXTEntryForButton();
	static float				GetDownloadProgress();
	static const char*			FetchMessage(long nIndex);
	static void					EchoMessage(const char* pMessage);
	static void CALLBACK		EchoMessage(const wchar_t* pMessage);
};

#endif