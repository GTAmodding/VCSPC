#pragma warning(disable:4481)	// nonstandard extension used: override specifier 'override'
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#define WINVER 0x0501
#define _WIN32_WINNT 0x0501
#define CURL_STATICLIB
#include <curl\curl.h>
#include <list>
#include <map>
#include <string>
#include <windows.h>
#include <shlwapi.h>
#include <shellapi.h>

#include "..\common\Date.h"
#include "..\common\Updater.h"
#include "..\common\DLCShared.h"
#include "UptModule.h"
#include "UpdaterWrappers.h"
#include "CCRC32.H"

// Global variables
extern CUpdater*			gUpdaterHandle;