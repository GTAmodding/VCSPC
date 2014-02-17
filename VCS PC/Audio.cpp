#include "StdAfx.h"

signed char	CAERadioTrackManager::bTracksPlayedRecently[NUM_RADIOSTATIONS];

static std::map<short,tVehicleAudioSettings>	AudioSettingsMap;

WRAPPER void CAudioEngine::ReportFrontendAudioEvent(long nSoundID, float fUnk, float fVolume) { WRAPARG(nSoundID); WRAPARG(fUnk); WRAPARG(fVolume); EAXJMP(0x506EA0); }

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

void CAEVehicleAudioEntity::GetVehicleAudioSettings(short nModelID)
{
   	AudioSettings = AudioSettingsMap[nModelID];
}

void CAEVehicleAudioEntity::LoadVehicleAudioSettings()
{
	if ( FILE* hFile = CFileMgr::OpenFile("DATA\\VEHAUDIOSETS.DAT", "r") )
	{
		while ( const char* pLine = CFileLoader::LoadLine(hFile) )
		{
			if ( pLine[0] && pLine[0] != '#' )
			{
				signed int				nModelID, nVehType, nEngineOn, nEngineOff, nRadioSound, nHornTon, nDoorSound, nUnk, nRadioNum, nRadioType, nScannerName;
				tVehicleAudioSettings	NewEntry;
				char					ModelName[40];

				sscanf(pLine, "%s %d %d %d %d %f %f %d %f %d %d %d %d %d %f", ModelName, &nVehType, &nEngineOn, &nEngineOff, &nRadioSound,
					&NewEntry.BassDepth, &NewEntry.unk1, &nHornTon, &NewEntry.HornFreq, &nDoorSound, &nUnk, &nRadioNum, &nRadioType, &nScannerName,
					&NewEntry.EngineVolumeBoost);

				CModelInfo::GetModelInfo(ModelName, &nModelID);

				NewEntry.VehicleType = static_cast<signed char>(nVehType);
				NewEntry.EngineOnSound = static_cast<short>(nEngineOn);
				NewEntry.EngineOffSound = static_cast<short>(nEngineOff);
				NewEntry.RadioSoundType = static_cast<signed char>(nRadioSound);
				NewEntry.HornTon = static_cast<signed char>(nHornTon);
				NewEntry.DoorSound = static_cast<signed char>(nDoorSound);
				NewEntry.unk2 = static_cast<signed char>(nUnk);
				NewEntry.RadioNum = static_cast<signed char>(nRadioNum);
				NewEntry.RadioType = static_cast<signed char>(nRadioType);
				NewEntry.PoliceScannerName = static_cast<signed char>(nScannerName);

				AudioSettingsMap[static_cast<short>(nModelID)] = NewEntry;
			}
		}
		CFileMgr::CloseFile(hFile);
	}
}