#ifndef __UPDATERCOMMON
#define __UPDATERCOMMON

#ifdef BUILDING_DLL
#define UPDATER_EXPORT __declspec(dllexport)
#else
#define UPDATER_EXPORT __declspec(dllimport)
#endif

// Pseudo class definitions (or whatever it's called)
class IUpdaterClient;

// Types and macroes
#include "UpdaterTypes.h"

// Interfaces (their wrapper implementations are inside UpdaterWrappers.h/.cpp)
#include "updater_interfaces\IUpdaterClient001.h"	// Used in versions 1, 2, 3, 4


// Macroes used for interface creation
#define UPDATER_INTERFACE_CLIENT001				"UPTMODULE_INTERFACE_CLIENT001"

// Use this now
extern "C" UPDATER_EXPORT IUpdaterClient* CreateUpdaterInstance(const char* pName);

// Backwards compatibility only, don't use!
extern "C" UPDATER_EXPORT IUpdaterClient001* GetUpdaterInterface();

#endif