#ifndef __UPTMODULE
#define __UPTMODULE

#include "..\common\Date.h"
#include "..\common\Updater.h"

#define LOGIN_DATA				"ftp://ronly:vcspc6912@modbase.pl"

#ifdef DEVBUILD
//#define HASHMAP_NAME			L"HASH_MAP"				// Retail hashmap
#define HASHMAP_NAME			L"HASH_MAP_DBUG"		// Debug hashmap
#else
#define HASHMAP_NAME			L"HASH_MAP"
#endif

#define MAC_ADDRESS_BUFFER_SIZE				15000

// DLC messages
#define DLC_MESSAGE_FAIL			"fail"
#define DLC_MESSAGE_UNLOCKED		"unlocked"

typedef std::pair<std::wstring,unsigned long>		HashPair;

// TODO: Try to organise it better

#pragma pack (push, 1)
struct HashEntry
{
	DWORD			bCompressedFile : 1;
	DWORD			bIgnoreIfAbsent : 1;
	DWORD			bRepackEXE : 1;
	signed short	nDLCNum;
	DWORD			nHash;
	DWORD			nSize;
};
#pragma pack (pop)

struct DLCEntry
{
	std::string		m_strName;
	CDate			m_launchDate, m_expirationDate;
};

// "Parent" updater class, wrapped by interface implementations
class CUpdater
{
private:
	bool										bInitialized : 1;
	bool										bCheckingForUpdates : 1;
	bool										bFilesReadyToInstall : 1;
	bool										bDownloadingInProgress : 1;
	bool										bAllReady : 1;
	bool										bUpdaterMustUpdate : 1;
	bool										bMoveFilesNow : 1;
	bool										bUpdateServiceOff : 1;
	unsigned char								bDownloadingStatus;
	long										nUpdateResult;
	DWORD										dwTotalDownloadSize;
	DWORD										dwDownloadedData;
	WORD										wNumberOfFiles;
	WORD										wCurrentReadFile;
	CURLM*										pCurlMulti;
	CURL*										pCurlUpdaterHandle;
	FILE*										hDownloadedFile;
	std::wstring								szFullGamePath;
	std::wstring								szFullCachePath;
	std::wstring								szCurrentFile;
	std::list<std::wstring>						filesList;
	std::list<std::wstring>::const_iterator		listIt;
	std::map<std::string,std::pair<bool, bool>>	m_DLCsMap;
	DLCEntry*									m_DLCsByIndex;

	UptMessageCallback							MessageFunction;

	// DLC handling part
	CURL*										pDLCCurl;
	std::string									strReturnedData;
	SerialCodeRequestCallback					OnFinishFunction;

	// updater.set
	UpdaterClientData001						savedSetData;

private:
	void				EchoMessage(const char* pMessage);
	void				EchoMessage(const wchar_t* pMessage);
	void				SetFileToDownload(const wchar_t* pFileName, bool bReadWriteAccess = false);
	void				CleanupCache();
	void				AddFileToDownloadList(const char* pFileToAdd);
	void				DownloadFile();
	long				ParseHashmap(bool& bFinished);
	void				DownloadNextFile(long& nStatus, bool bPopCurrentFile);
	void				MoveFiles(std::wstring& dirName);
	void				ReadSettingsFile();
	void				ProcessUpdaterMessage(CURLMsg* pMultiMsg, long& nStatus);

	inline void			CleanupFilesList()
		{ filesList.clear(); }
	inline bool			CheckThisDLC(signed short nIndex)
		{	std::string&	pName = m_DLCsByIndex[nIndex].m_strName;
			CDate			CurrentDate = GetCurrentDate();
			return m_DLCsMap.count(pName) != 0 ? (m_DLCsMap[pName].first && ( m_DLCsByIndex[nIndex].m_launchDate == static_cast<CDate>(0) || (m_DLCsByIndex[nIndex].m_launchDate <= CurrentDate && m_DLCsByIndex[nIndex].m_expirationDate > CurrentDate) )) : false; }
public:
	CUpdater()
		:	bInitialized(false), bCheckingForUpdates(false), bFilesReadyToInstall(false), bDownloadingInProgress(false),
			bAllReady(false), bUpdaterMustUpdate(false), bMoveFilesNow(false), bUpdateServiceOff(false), bDownloadingStatus(0), nUpdateResult(0),
			dwTotalDownloadSize(0), dwDownloadedData(0), pCurlMulti(nullptr), pCurlUpdaterHandle(nullptr),
			hDownloadedFile(nullptr), m_DLCsByIndex(nullptr), MessageFunction(nullptr)
	{
	}

	~CUpdater()
	{
		DisconnectFromFTP();
	}

	//void			Terminate();

	long			Process();
	void			ConnectToFTP();
	void			DisconnectFromFTP();
	void			Initialize();
	void			RegisterMessageCallback(UptMessageCallback function);
	bool			PerformUpdateCheck();
	void			PerformFilesDownload();
	void			LaunchInstaller();
	void			InstallUpdates();
	void			WriteSettingsFile();

	inline void		AddThisDLCToList(const char* pName, bool bEnable)
		{ m_DLCsMap[pName].first = bEnable; }
	inline void		SetThisDLCAsVerySpecial(const char* pName)
		{ m_DLCsMap[pName].second = true; }
	inline bool		GetDLCStatus(const char* pName, bool bDefault)
		{ if ( m_DLCsMap.count(pName) == 0 ) m_DLCsMap[pName].first = bDefault; return m_DLCsMap[pName].first; }
	inline float	GetProgress()
			{ return dwTotalDownloadSize ? (static_cast<float>(dwDownloadedData) * 100.0f) / dwTotalDownloadSize : 0.0f; }
	inline bool		UpdatesAvailable()
			{ return nUpdateResult > 0; }
	inline bool		UpdateServiceOn()
			{ return !bUpdateServiceOff; }
	inline void		AddProgress(unsigned long nProgress)
			{ dwDownloadedData += nProgress; if ( dwDownloadedData > dwTotalDownloadSize ) dwDownloadedData = dwTotalDownloadSize; }

	void			ReceiveSettings(unsigned int nVersion, const void* pSettings);
	void*			ReturnSettings(unsigned int nVersion, void* pBuf);
	bool			TimeToUpdate(time_t nExtraTime);

	// DLC handling part
	void			SendSerialCodeRequest(const std::string& request);
	void			RegisterOnFinishedRequestCallback(SerialCodeRequestCallback callback);
	void			ProcessDLCMessage(CURLMsg* pMultiMsg);
};

extern CUpdater*			gUpdaterHandle;

#endif