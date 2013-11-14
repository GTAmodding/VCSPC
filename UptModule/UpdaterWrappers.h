#ifndef __UPDATERWRAPPERS
#define __UPDATERWRAPPERS

// Interfaces' implementations
class CUpdaterClient001 : public IUpdaterClient001
{
public:
	virtual					~CUpdaterClient001() { };

	virtual long			Process() override;
	virtual void			ConnectToFTP() override;
	virtual void			DisconnectFromFTP() override;
	virtual void			Initialize() override;
	virtual void			RegisterMessageCallback(UptMessageCallback function) override;
	virtual bool			PerformUpdateCheck() override;
	virtual void			PerformFilesDownload() override;
	virtual void			LaunchInstaller() override;
	virtual void			InstallUpdates() override;
	virtual float			GetProgress() override;
	virtual bool			UpdatesAvailable() override;
	virtual bool			UpdateServiceOn() override;

	virtual void			AddThisDLCToList(const char* pName, bool bEnable) override;
	virtual bool			GetDLCStatus(const char* pName, bool bDefault) override;
};

#endif