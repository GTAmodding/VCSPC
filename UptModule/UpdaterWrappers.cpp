#include "StdAfx.h"
#include "UpdaterWrappers.h"

#include "UptModule.h"

// CUpdaterClient001
long CUpdaterClient001::Process()
{
	return gUpdaterHandle->Process();
}

void CUpdaterClient001::ConnectToFTP()
{
	gUpdaterHandle->ConnectToFTP();
}

void CUpdaterClient001::DisconnectFromFTP()
{
	gUpdaterHandle->DisconnectFromFTP();
}
void CUpdaterClient001::Initialize()
{
	gUpdaterHandle->Initialize();
}

void CUpdaterClient001::RegisterMessageCallback(UptMessageCallback function)
{
	gUpdaterHandle->RegisterMessageCallback(function);
}

bool CUpdaterClient001::PerformUpdateCheck()
{
	return gUpdaterHandle->PerformUpdateCheck();
}

void CUpdaterClient001::PerformFilesDownload()
{
	gUpdaterHandle->PerformFilesDownload();
}

void CUpdaterClient001::LaunchInstaller()
{
	gUpdaterHandle->LaunchInstaller();
}

void CUpdaterClient001::InstallUpdates()
{
	gUpdaterHandle->InstallUpdates();
}

float CUpdaterClient001::GetProgress()
{
	return gUpdaterHandle->GetProgress();
}

bool CUpdaterClient001::UpdatesAvailable()
{
	return gUpdaterHandle->UpdatesAvailable();
}

bool CUpdaterClient001::UpdateServiceOn()
{
	return gUpdaterHandle->UpdateServiceOn();
}

void CUpdaterClient001::AddThisDLCToList(const char* pName, bool bEnable)
{
	gUpdaterHandle->AddThisDLCToList(pName, bEnable);
}

bool CUpdaterClient001::GetDLCStatus(const char* pName, bool bDefault)
{
	return gUpdaterHandle->GetDLCStatus(pName, bDefault);
}


void CDLCClient001::SendSerialCodeRequest(const std::string* request)
{
	gUpdaterHandle->SendSerialCodeRequest(request);
}

void CDLCClient001::RegisterOnFinishedRequestCallback(SerialCodeRequestCallback callback)
{
	gUpdaterHandle->RegisterOnFinishedRequestCallback(callback);
}