#ifndef __CAERADIOTRACKMANAGER
#define __CAERADIOTRACKMANAGER

#define NUM_RADIOSTATIONS	10

#define AA_STREAMS			6
#define ADVERTS_STREAMS		15
#define AMBIENCE_STREAMS	6
#define BEATS_STREAMS		5
#define CH_STREAMS			27
#define CO_STREAMS			27
#define CR_STREAMS			18
#define CUTSCENE_STREAMS	NUM_CUTSCENES
#define DS_STREAMS			27
#define HC_STREAMS			15
#define MH_STREAMS			24
#define MR_STREAMS			12
#define NJ_STREAMS			12
#define RE_STREAMS			30

#define AA_OFFSET			0
#define ADVERTS_OFFSET		(AA_OFFSET+AA_STREAMS)
#define AMBIENCE_OFFSET		(ADVERTS_OFFSET+ADVERTS_STREAMS)
#define BEATS_OFFSET		(AMBIENCE_OFFSET+AMBIENCE_STREAMS)
#define CH_OFFSET			(BEATS_OFFSET+BEATS_STREAMS)
#define CO_OFFSET			(CH_OFFSET+CH_STREAMS)
#define CR_OFFSET			(CO_OFFSET+CO_STREAMS)
#define CUTSCENE_OFFSET		(CR_OFFSET+CR_STREAMS)
#define DS_OFFSET			(CUTSCENE_OFFSET+CUTSCENE_STREAMS)
#define HC_OFFSET			(DS_OFFSET+DS_STREAMS)
#define MH_OFFSET			(HC_OFFSET+HC_STREAMS)
#define MR_OFFSET			(MH_OFFSET+MH_STREAMS)
#define NJ_OFFSET			(MR_OFFSET+MR_STREAMS)
#define RE_OFFSET			(NJ_OFFSET+NJ_STREAMS)

struct tVehicleAudioSettings
{
	signed char		VehicleType;
	short			EngineOnSound;
	short			EngineOffSound;
	signed char		RadioSoundType;		// 0 - standard, 1 - HQ, 2 - bikes
	float			BassDepth;
	float			unk1;
	signed char		HornTon;
	float			HornFreq;
	signed char		DoorSound;
	signed char		unk2;
	signed char		RadioNum;
	signed char		RadioType;
	signed char		PoliceScannerName;
	float			EngineVolumeBoost;
};

class CAERadioTrackManager
{
private:
	static signed char	bTracksPlayedRecently[NUM_RADIOSTATIONS];

public:
	signed char			GetNextTrackByStation(BYTE stationID);
	static void			ResetExtra();
	static bool			Save();
	static bool			Load();
};

class CAEVehicleAudioEntity
{
private:
	BYTE					pad[128];
	tVehicleAudioSettings	AudioSettings;

public:
	void					GetVehicleAudioSettings(short nModelID);

	// VCS PC class extension
	static void				LoadVehicleAudioSettings();
};

class CAudioEngine
{
public:
	void				ReportFrontendAudioEvent(long nSoundID, float fUnk, float fVolume);
};

extern CAudioEngine&		AudioEngine;

static_assert(sizeof(tVehicleAudioSettings) == 0x24, "tVehicleAudioSettings has wrong size!");

#endif