#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <Windows.h>
#include <CommCtrl.h>
#include <Wincodec.h>
#include <map>
#include "..\common\Updater.h"
#include "AutoUpdater.h"

// TODO: SORT IT AND CLEAN IT, KURWA

#define MARGIN 10

#define APP_NAME L"GTA: Vice City Stories Updater"

RECT			WindowDimensions = {0, 0, 480, 290},		// Here you can change size of the window
	ButtonArea, ListBoxArea, ProgressBarArea;
HWND			hButton, hListBox, hProgressBar;


HINSTANCE			gInstance;
HANDLE				gUpdaterThread;
IUpdaterClient001*	gUpdater;
long				nLastState = UPTMODULESTATE_IDLE;
bool				bWantsToExitMuch = false;
bool				bInstantInstall = false;
bool				bLaunchGameByButton = false;

// String management function
const wchar_t*		GetUIMsg(UINT nMessageID);

ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void				InitUptModule();

//void CALLBACK		EchoMessage(const char* pMessage);
void CALLBACK		EchoMessage(const wchar_t* pMessage);

void				LaunchGame();

DWORD WINAPI UpdaterThread(LPVOID lpParameter)
{
	UNREFERENCED_PARAMETER(lpParameter);

	while ( !bWantsToExitMuch )
	{
		Sleep(1);
		long nUptStatus = gUpdater->Process();
		if ( !bInstantInstall )
		{
			if ( nUptStatus == UPTMODULESTATE_DOWNLOADING )
			{
				static int		nLastProgress = 0;
				long			nCurProgress = static_cast<int>(gUpdater->GetProgress());

				signed int		nProgressDelta = nCurProgress - nLastProgress;
				if ( nProgressDelta > 0 )
				{
					nLastProgress = nCurProgress;
					SendMessage( hProgressBar, PBM_DELTAPOS, ( WPARAM )nProgressDelta, 0 );
				}
			}
			else
			{
				if ( nUptStatus == UPTMODULESTATE_ALL_READY )
					SendMessage( hProgressBar, PBM_SETPOS,( WPARAM ) 100, 0 );
			}

			if ( nUptStatus != nLastState )
			{
				if ( nUptStatus == UPTMODULESTATE_CHECKING )
				{
					EnableWindow(hButton, FALSE);
					SetWindowText(hButton, GetUIMsg(IDS_CHECKING));
				}
				else
				{
					if ( nUptStatus == UPTMODULESTATE_DOWNLOADING )
					{
						EnableWindow(hButton, FALSE);
						SetWindowText(hButton, GetUIMsg(IDS_INSTALLING));
					}
					else
					{
						EnableWindow(hButton, TRUE);
						if ( nUptStatus == UPTMODULESTATE_NEW_UPDATES )
							SetWindowText(hButton, GetUIMsg(IDS_UPTINST));
						else
						{
							if ( nUptStatus == UPTMODULESTATE_ALL_READY )
							{
								EchoMessage(L"Files have been downloaded, press Install to apply the updates");
								SetWindowText(hButton, L"Install");
							}
							else
								SetWindowText(hButton, GetUIMsg(IDS_UPTCHECK));
						}
					}
				}
				nLastState = nUptStatus;
			}
		}
		else
		{
			if ( !bLaunchGameByButton )
			{
				EnableWindow(hButton, TRUE);
				SetWindowText(hButton, L"Launch the game");
				bLaunchGameByButton = true;
			}
		}
	}

	return TRUE;
}


int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	MSG msg;

	if ( !pCmdLine[0] )
	{
		wchar_t		wcTempPath[MAX_PATH];

		GetModuleFileName(hInstance, wcTempPath, MAX_PATH);

		wchar_t*	pSlash = wcsrchr(wcTempPath, '\\');

		*pSlash = '\0';
		pSlash = wcsrchr(wcTempPath, '\\');
		*pSlash = '\0';	
		if ( !SetCurrentDirectory(wcTempPath) )
			return FALSE;
	}
	else
	{
		if ( wcsstr(pCmdLine, L"-install") )
		{
			if ( !SetCurrentDirectory(&pCmdLine[9]) )
				return FALSE;
			bInstantInstall = true;
		}
	}

	gUpdater = (IUpdaterClient001*)CreateUpdaterInstance(UPDATER_INTERFACE_CLIENT001);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
		return FALSE;

	InitUptModule();

	//hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_AUTOUPDATER));

	if ( bInstantInstall )
	{
		EchoMessage(L"The update is being installed...");
		gUpdater->InstallUpdates();
	}

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0) > 0)
	{
		if ( msg.message == WM_QUIT || bWantsToExitMuch )
			break;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	/*while ( !bWantsToExitMuch )
	{
		if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
		{
			if ( msg.message == WM_QUIT )
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			long nUptStatus = gUpdater->Process();
			if ( !bInstantInstall )
			{
				if ( nUptStatus == UPTMODULESTATE_DOWNLOADING )
				{
					static int		nLastProgress = 0;
					long			nCurProgress = static_cast<int>(gUpdater->GetProgress());

					signed int		nProgressDelta = nCurProgress - nLastProgress;
					if ( nProgressDelta > 0 )
					{
						nLastProgress = nCurProgress;
						SendMessage( hProgressBar, PBM_DELTAPOS, ( WPARAM )nProgressDelta, 0 );
					}
				}
				else
				{
					if ( nUptStatus == UPTMODULESTATE_ALL_READY )
						SendMessage( hProgressBar, PBM_SETPOS,( WPARAM ) 100, 0 );
				}

				if ( nUptStatus != nLastState )
				{
					if ( nUptStatus == UPTMODULESTATE_CHECKING )
					{
						EnableWindow(hButton, FALSE);
						SetWindowText(hButton, GetUIMsg(IDS_CHECKING));
					}
					else
					{
						if ( nUptStatus == UPTMODULESTATE_DOWNLOADING )
						{
							EnableWindow(hButton, FALSE);
							SetWindowText(hButton, GetUIMsg(IDS_INSTALLING));
						}
						else
						{
							EnableWindow(hButton, TRUE);
							if ( nUptStatus == UPTMODULESTATE_NEW_UPDATES )
								SetWindowText(hButton, GetUIMsg(IDS_UPTINST));
							else
							{
								if ( nUptStatus == UPTMODULESTATE_ALL_READY )
								{
									EchoMessage(L"Files have been downloaded, press Install to apply the updates");
									SetWindowText(hButton, L"Install");
								}
								else
									SetWindowText(hButton, GetUIMsg(IDS_UPTCHECK));
							}
						}
					}
					nLastState = nUptStatus;
				}
			}
			else
			{
				if ( !bLaunchGameByButton )
				{
					EnableWindow(hButton, TRUE);
					SetWindowText(hButton, L"Launch the game");
					bLaunchGameByButton = true;
				}
			}
		}
	}*/

	if ( bWantsToExitMuch )
	{
		if ( bLaunchGameByButton )
			LaunchGame();
		else
			gUpdater->LaunchInstaller();
	}

	return (int) msg.wParam;
}

void CALLBACK EchoMessage(const wchar_t* pMessage)
{
	long	nNumEntries = SendMessage(hListBox, LB_GETCOUNT, 0, 0);

	if ( nNumEntries > 8 )
		SendMessage(hListBox, LB_DELETESTRING, 0, 0);

	SendMessage(hListBox, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(pMessage));

}

void LaunchGame()
{
	PROCESS_INFORMATION pi;
	STARTUPINFO			si = { sizeof(si) };

	CreateProcess(L".\\gta-vcs.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
}

void InitWindowPos()
{
	HWND DesktopWnd;
	DesktopWnd = GetDesktopWindow();

	RECT DesktopWndRect;
	GetWindowRect(DesktopWnd, &DesktopWndRect);

	WindowDimensions.left = DesktopWndRect.right / 2 - WindowDimensions.right / 2;
	WindowDimensions.top = DesktopWndRect.bottom / 2 - WindowDimensions.bottom / 2;
}

#define BUTTON_WIDTH	150
#define BUTTON_HEIGHT	50

void InitAreas(HWND hWnd)
{
	RECT Client;
	GetClientRect(hWnd, &Client);

	SetRect(&ButtonArea, Client.right - MARGIN - BUTTON_WIDTH, Client.bottom - BUTTON_HEIGHT - MARGIN, BUTTON_WIDTH, BUTTON_HEIGHT);
	SetRect(&ListBoxArea, MARGIN, MARGIN, (Client.right-2*MARGIN), 160);
	SetRect(&ProgressBarArea, MARGIN, MARGIN + 160, (Client.right-2*MARGIN), 20);
}

void ButtonPushed()
{
	//Tutaj obs³uga wciœniêcia przycisku!!! :)
	if ( !bLaunchGameByButton )
	{
		if ( nLastState == UPTMODULESTATE_IDLE )
			gUpdater->PerformUpdateCheck();
		else
		{
			if ( nLastState == UPTMODULESTATE_NEW_UPDATES )
				gUpdater->PerformFilesDownload();
			else
				bWantsToExitMuch = true;
		}
	}
	else
		bWantsToExitMuch = true;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_BTNFACE+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= L"VCSPCAPUP";
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
   /*INITCOMMONCONTROLSEX	ctrls;

   ctrls.dwSize = sizeof(INITCOMMONCONTROLSEX);
   ctrls.dwICC = ICC_STANDARD_CLASSES;*/

	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof( INITCOMMONCONTROLSEX );
	icc.dwICC = ICC_BAR_CLASSES; // toolbary, statusbary, tooltipy i oczywiœcie progressbary
	InitCommonControlsEx(&icc);

   gInstance = hInstance; // Store instance handle in our global variable

   InitWindowPos();
//	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

   hWnd = CreateWindow(L"VCSPCAPUP", APP_NAME, 
	   WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
	   WindowDimensions.left, 
	   WindowDimensions.top, 
	   WindowDimensions.right, 
	   WindowDimensions.bottom, 
	   NULL, NULL, hInstance, NULL);
   if (!hWnd) return FALSE;

   InitAreas(hWnd);
   hButton = CreateWindow(WC_BUTTON, !bInstantInstall ? GetUIMsg(IDS_UPTCHECK) : L"Installing...",
	   WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 
	   ButtonArea.left, ButtonArea.top, ButtonArea.right, ButtonArea.bottom, 
	   hWnd, NULL, hInstance, NULL);
   if(!hButton) return FALSE;

   if ( bInstantInstall )
	   EnableWindow(hButton, FALSE);

   hListBox = CreateWindowEx(0, WC_LISTBOX, L"ListBox",
	   WS_TABSTOP | WS_VISIBLE | WS_CHILD | LBS_HASSTRINGS | WS_BORDER, 
	   ListBoxArea.left, ListBoxArea.top, ListBoxArea.right, ListBoxArea.bottom, 
	   hWnd, NULL, hInstance, NULL);
   if(!hListBox) return FALSE;

	hProgressBar = CreateWindowEx( 0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
		ProgressBarArea.left, ProgressBarArea.top, ProgressBarArea.right, ProgressBarArea.bottom, hWnd,( HMENU ) 200, hInstance, NULL );
	if(!hProgressBar) return FALSE;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void InitUptModule()
{
	gUpdaterThread = CreateThread(nullptr, 0, UpdaterThread, nullptr, 0, nullptr);
	gUpdater->Initialize();
	gUpdater->RegisterMessageCallback(EchoMessage);
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		if((HWND)lParam == hButton)
			ButtonPushed();
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

const wchar_t* GetUIMsg(UINT nMessageID)
{
	static std::map<UINT,std::wstring>	MessagesMap;

	auto	entryIt = MessagesMap.find(nMessageID);

	if ( entryIt == MessagesMap.end() )
	{
		// Entry not found, add it
		wchar_t			wcTempMessage[100];
		if ( LoadString(gInstance, nMessageID, wcTempMessage, 100) )
		{
			MessagesMap[nMessageID] = wcTempMessage;
			return MessagesMap[nMessageID].c_str();
		}
		else
			throw nMessageID;
	}
	return entryIt->second.c_str();
}