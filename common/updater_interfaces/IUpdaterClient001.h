#ifndef __UPDATERCLIENT001
#define __UPDATERCLIENT001

// Versions which use that interface: 1, 2, 3 (with new methods), 4, 5
class IUpdaterClient001
{
public:
	virtual					~IUpdaterClient001() {};

	virtual long			Process()=0;
	virtual void			ConnectToFTP()=0;
	virtual void			DisconnectFromFTP()=0;
	virtual void			Initialize()=0;
	virtual void			RegisterMessageCallback(UptMessageCallback function)=0;
	virtual bool			PerformUpdateCheck()=0;
	virtual void			PerformFilesDownload()=0;
	virtual void			LaunchInstaller()=0;
	virtual void			InstallUpdates()=0;
	virtual float			GetProgress()=0;
	virtual bool			UpdatesAvailable()=0;
	virtual bool			UpdateServiceOn()=0;

	// Added in version 3
	virtual void			AddThisDLCToList(const char* pName, bool bEnable)=0;
	virtual bool			GetDLCStatus(const char* pName, bool bDefault)=0;	

	virtual void			SetThisDLCAsVerySpecial(const char* pName)=0;
};

#endif