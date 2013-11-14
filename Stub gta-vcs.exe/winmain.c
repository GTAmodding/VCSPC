#define WIN32_LEAN_AND_MEAN

#define WINVER 0x0500
#define _WIN32_WINNT 0x0500

#include <windows.h>
#include "resource.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
	PROCESS_INFORMATION pi;
	STARTUPINFO			si;
	wchar_t*			pExeName;
	int					i;

	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nShowCmd);

	si.cb = sizeof(si);

	// A custom, simple memset
	for ( i = 1; i < sizeof(si)/4; ++i )
		((int*)&si)[i] = 0;

	if ( GetFileAttributes(L".\\gta_sa.exe") != INVALID_FILE_ATTRIBUTES )
		pExeName = L".\\gta_sa.exe";
	else
	{
		if ( GetFileAttributes(L".\\gta-sa.exe") != INVALID_FILE_ATTRIBUTES )
			pExeName = L".\\gta-sa.exe";
		else
		{
			MessageBox(0, L"San Andreas executable was not found in Grand Theft Auto: Vice City Stories game directory.", L"GTA: Vice City Stories", MB_OK | MB_ICONERROR);
			return 0;
		}
	}
	CreateProcess(pExeName, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

	return 0;
}