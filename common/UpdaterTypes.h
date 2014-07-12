#ifndef __UPDATERTYPES
#define __UPDATERTYPES

// Filename lies... constants are there too!

// Main updater version
#define UPDATER_VERSION		5

// updater.set current version
#define UPDATER_SETTINGS_VERSION		1

// Updater states
// TODO: Not sure how the updater will react if we ever change these IDs - backwards compatibility may break!
// Any ideas on a solution?
#define UPTMODULESTATE_IDLE			0
#define UPTMODULESTATE_CHECKING		1
#define UPTMODULESTATE_NEW_UPDATES	2
#define UPTMODULESTATE_DOWNLOADING	3
#define UPTMODULESTATE_ALL_READY	4
#define UPTMODULESTATE_INSTALLING	5

typedef void (CALLBACK *UptMessageCallback)(const wchar_t*);
typedef void (CALLBACK *SerialCodeRequestCallback)(const std::string&);

typedef IUpdaterClient* (*GetUptModuleFn)(const char* pName);
typedef IDLCClient* (*GetDLCFn)(const char* pName);

#endif