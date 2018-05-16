#include "StdAfx.h"
#include "Audio.h"

#include "PcSave.h"
#include "ModelInfo.h"

signed char	CAERadioTrackManager::bTracksPlayedRecently[NUM_RADIOSTATIONS];

WRAPPER void CAudioEngine::Service(void) { EAXJMP(0x507750); }
WRAPPER void CAudioEngine::ReportFrontendAudioEvent(long nSoundID, float fUnk, float fVolume) { WRAPARG(nSoundID); WRAPARG(fUnk); WRAPARG(fVolume); EAXJMP(0x506EA0); }
WRAPPER void CAudioEngine::SetMusicMasterVolume(signed char nVolume) { WRAPARG(nVolume); EAXJMP(0x506DE0); };
WRAPPER void CAudioEngine::SetEffectsMasterVolume(signed char nVolume) { WRAPARG(nVolume); EAXJMP(0x506E10); }
WRAPPER void CAudioEngine::SetRadioAutoRetuneOnOff(bool bRetune) { WRAPARG(bRetune); EAXJMP(0x506F80); }
WRAPPER void CAudioEngine::RetuneRadio(signed char nStation) { WRAPARG(nStation); EAXJMP(0x507E10); }
WRAPPER const char* CAudioEngine::GetRadioStationName(signed char nStation) { WRAPARG(nStation); EAXJMP(0x507000); }


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
	AudioSettings = static_cast<CVehicleModelInfoVCS*>(CModelInfo::ms_modelInfoPtrs[nModelID])->GetAudioSettings();
}

void CAEVehicleAudioEntity::LoadVehicleAudioSettings()
{
	LoadVehicleAudioSettings("DATA\\VEHAUDIO.DAT");

	if ( auto* pAudioSets = CFileLoader::GetVehAudioList() )
	{
		for ( auto it = pAudioSets->cbegin(); it != pAudioSets->cend(); it++ )
			LoadVehicleAudioSettings(it->c_str());
	}
}

void CAEVehicleAudioEntity::LoadVehicleAudioSettings(const char* pFilename)
{
	if ( FILE* hFile = CFileMgr::OpenFile(pFilename, "r") )
	{
		while ( const char* pLine = CFileLoader::LoadLine(hFile) )
		{
			if ( pLine[0] && pLine[0] != '#' )
			{
				signed int				nVehType, nEngineOn, nEngineOff, nRadioSound, nHornTon, nDoorSound, nUnk, nRadioNum, nRadioType, nScannerName;
				float					BassDepth, unk1, HornFreq, EngineVolumeBoost;
				char					ModelName[24];

				sscanf(pLine, "%s %d %d %d %d %f %f %d %f %d %d %d %d %d %f", ModelName, &nVehType, &nEngineOn, &nEngineOff, &nRadioSound,
					&BassDepth, &unk1, &nHornTon, &HornFreq, &nDoorSound, &nUnk, &nRadioNum, &nRadioType, &nScannerName,
					&EngineVolumeBoost);

				tVehicleAudioSettings&	NewEntry = static_cast<CVehicleModelInfoVCS*>(CModelInfo::GetModelInfo(ModelName, nullptr))->GetAudioSettings();

				NewEntry.VehicleType = static_cast<signed char>(nVehType);
				NewEntry.EngineOnSound = static_cast<short>(nEngineOn);
				NewEntry.EngineOffSound = static_cast<short>(nEngineOff);
				NewEntry.RadioSoundType = static_cast<signed char>(nRadioSound);
				NewEntry.BassDepth = BassDepth;
				NewEntry.unk1 = unk1;
				NewEntry.HornTon = static_cast<signed char>(nHornTon);
				NewEntry.HornFreq = HornFreq;
				NewEntry.DoorSound = static_cast<signed char>(nDoorSound);
				NewEntry.unk2 = static_cast<signed char>(nUnk);
				NewEntry.RadioNum = static_cast<signed char>(nRadioNum);
				NewEntry.RadioType = static_cast<signed char>(nRadioType);
				NewEntry.PoliceScannerName = static_cast<signed char>(nScannerName);
				NewEntry.EngineVolumeBoost = EngineVolumeBoost;
			}
		}
		CFileMgr::CloseFile(hFile);
	}
}