#ifndef __PLAYERINFO
#define __PLAYERINFO

class CPlayerCrosshair
{
private:
    BYTE		bActivated;
    float		TargetX, TargetY;
};

class CPlayerInfo
{
private:
	CPed*			pPed;											// 0x0
	CPlayerPedData	data;									// 0x4
	CVehicle*		pVehicle;									// 0xB0
	CVehicle*		pSpecCar;									// 0xB4
	DWORD			Score;										// 0xB8
	DWORD			DisplayedScore;								// 0xBC
	DWORD			CollectablesPickedUp;							// 0xC0
	DWORD			TotalNumCollectables;							// 0xC4
	DWORD			nLastBumpedTimer;								// 0xC8
	DWORD			TimeHasHadPassengerInTaxi;					// 0xCC
	DWORD			VehicleTimeCounter;							// 0xD0
	BYTE			bKeepTaxiTimerScore;							// 0xD4
	BYTE			PlayerTryingToExitCar;							// 0xD5
	void*			pLastTargetVehicle;							// 0xD8
	BYTE			isWastedBusted;								// 0xDC
	BYTE			bAfterRemoteVehicleExplosion;					// 0xDD
	BYTE			bCreateRemoteVehicleExplosion;					// 0xDE
	BYTE			bFadeAfterRemoteVehicleExplosion;				// 0xDF
	int				TimeOfRemoteVehicleExplosion;					// 0xE0
	int				LastTimeEnergyLost;							// 0xE4
	int				LastTimeArmourLost;							// 0xE8
	int				LastTimeBigGunFired;							// 0xEC
	DWORD			TimesUpsideDownInARow;						// 0xF0
	DWORD			TimesStuckInARow;								// 0xF4
	DWORD			nCarTwoWheelCounter;							// 0xF8
	float			fCarTwoWheelDist;								// 0xFC
	DWORD			nCarLess3WheelCounter;						// 0x100
	DWORD			nBikeRearWheelCounter;						// 0x104
	float			fBikeRearWheelDist;							// 0x108
	DWORD			nBikeFrontWheelCounter;						// 0x10C
	float			fBikeFrontWheelDist;							// 0x110
	DWORD			nTempBufferCounter;							// 0x114
	DWORD			nBestCarTwoWheelsTimeMs;						// 0x118
	float			fBestCarTwoWheelsDistM;						// 0x11C
	DWORD			nBestBikeWheelieTimeMs;						// 0x120
	float			fBestBikeWheelieDistM;						// 0x124
	DWORD			nBestBikeStoppieTimeMs;						// 0x128
	float			fBestBikeStoppieDistM;						// 0x12C
	WORD			CarDensityForCurrentZone;						// 0x130
	float			RoadDensityAroundPlayer;						// 0x134
	DWORD			TimeOfLastCarExplosionCaused;					// 0x138
	DWORD			ExplosionMultiplier;							// 0x13C
	DWORD			HavocCaused;									// 0x140
	WORD			TimeLastEaten;									// 0x144
	float			fCurrentChaseRating;							// 0x148
	BYTE			InfiniteSprint;								// 0x14C
	BYTE			FastReload;									// 0x14D
	BYTE			FireProof;										// 0x14E
	BYTE			MaxHealth;										// 0x14F
	BYTE			MaxArmour;										// 0x150
	BYTE			GetOutOfJailFree;								// 0x151
	BYTE			FreeHealthCare;								// 0x152
	BYTE			CanDoDriveBy;									// 0x153
	BYTE			BustedAudioStatus;								// 0x154
	BYTE			LastBustMessageNumber;							// 0x155
	CPlayerCrosshair Crosshair;							// 0x156
	char			m_skinName[32];								// 0x164
	char*			m_pSkinTexture;								// 0x184
	BYTE			m_bParachuteReferenced;						// 0x188
	DWORD			m_nRequireParachuteTimer;						// 0x18C

public:
	inline CPed*		GetPed() const
						{ return pPed; };
	inline CPlayerPedData&	GetPlayerData()
						{ return data; };
	inline BYTE			GetMaxHealth() const
						{ return MaxHealth; };
	inline BYTE			GetMaxArmour() const
						{ return MaxArmour; };
	inline int			GetDisplayedScore() const
						{ return DisplayedScore; };
	inline int			GetLastTimeEnergyLost() const
						{ return LastTimeEnergyLost; };
	inline int			GetLastTimeArmourLost() const
						{ return LastTimeArmourLost; };

	void			KillPlayer();
	void			ArrestPlayer();
};

CWanted* FindPlayerWanted(signed int nPlayerID);

static_assert(sizeof(CPlayerInfo) == 0x190, "Wrong size: CPlayerInfo");

#endif
