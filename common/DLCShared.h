#ifndef __DLCSHARED
#define __DLCSHARED

#define DLC_SETTINGS_FILE_VERSION		2

// This enum index is used as a DLC directory index, so keep indexes explicit
enum eExpansionPack
{
	// Halloween DLC (zombies and such) (active 29th October - 1st November)
	DLC_HALLOWEEN			= 0,

	// Project 2dfx
	DLC_2DFX				= 1,

	// You pay, you get this
	DLC_THANKYOU			= 2,

	// Keep this untouched
	NUM_DLC_PACKS
};

#endif