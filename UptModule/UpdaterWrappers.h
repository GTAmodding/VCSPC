#ifndef __UPDATERWRAPPERS
#define __UPDATERWRAPPERS

#include "..\common\Updater.h"

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

	virtual void			SetThisDLCAsVerySpecial(const char* pName) override;

	virtual void			SaveSettings(unsigned int nStructVersion, const void* pSettings) override;
	virtual void*			ReturnSettings(unsigned int nStructVersion, void* pBuf) override;
	virtual bool			TimeToUpdate(time_t nExtraTime);
};

class CDLCClient001 : public IDLCClient001
{
	virtual					~CDLCClient001() { };

	virtual void			SendSerialCodeRequest(const std::string& request) override;
	virtual void			RegisterOnFinishedRequestCallback(SerialCodeRequestCallback callback) override;
};

#endif