#include "StdAfx.h"

signed char	CAERadioTrackManager::bTracksPlayedRecently[NUM_RADIOSTATIONS];

signed char CAERadioTrackManager::GetNextTrackByStation(BYTE stationID)
{
	static const BYTE numberOfRadioTracks[NUM_RADIOSTATIONS] = 
	{ AA_STREAMS/3, CH_STREAMS/3, CO_STREAMS/3, CR_STREAMS/3, DS_STREAMS/3, HC_STREAMS/3, MH_STREAMS/3, MR_STREAMS/3, NJ_STREAMS/3, RE_STREAMS/3 };

	signed char	nextTrack = ( bTracksPlayedRecently[stationID] + 1 ) % numberOfRadioTracks[stationID];
	bTracksPlayedRecently[stationID] = nextTrack;

	return nextTrack;
}

void CAERadioTrackManager::ResetExtra()
{
	memset(bTracksPlayedRecently, -1, sizeof(bTracksPlayedRecently));
}

bool CAERadioTrackManager::Save()
{
	C_PcSave::PcClassSaveRoutine(bTracksPlayedRecently, sizeof(bTracksPlayedRecently));
	return true;
}

bool CAERadioTrackManager::Load()
{
	C_PcSave::PcClassLoadRoutine(bTracksPlayedRecently, sizeof(bTracksPlayedRecently));
	return true;
}

WRAPPER void CMusicManager::ReportFrontendAudioEvent(long nSoundID, float fUnk, float fVolume) { WRAPARG(nSoundID); WRAPARG(fUnk); WRAPARG(fVolume); EAXJMP(0x506EA0); }