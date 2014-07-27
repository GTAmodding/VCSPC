#ifndef __VEHICLE
#define __VEHICLE

#include "General.h"
#include "ModelInfo.h"

enum eVehicleType
{
    VEHICLE_AUTOMOBILE,
    VEHICLE_MTRUCK,
    VEHICLE_QUAD,
    VEHICLE_HELI,
    VEHICLE_PLANE,
    VEHICLE_BOAT,
    VEHICLE_TRAIN,
    VEHICLE_FHELI,
    VEHICLE_FPLANE,
    VEHICLE_BIKE,
    VEHICLE_BMX,
    VEHICLE_TRAILER
};

struct CVehicleFlags
{
	//0x428
    unsigned char bIsLawEnforcer: 1; // Is this guy chasing the player at the moment
    unsigned char bIsAmbulanceOnDuty: 1; // Ambulance trying to get to an accident
    unsigned char bIsFireTruckOnDuty: 1; // Firetruck trying to get to a fire
    unsigned char bIsLocked: 1; // Is this guy locked by the script (cannot be removed)
    unsigned char bEngineOn: 1; // For sound purposes. Parked cars have their engines switched off (so do destroyed cars)
    unsigned char bIsHandbrakeOn: 1; // How's the handbrake doing ?
    unsigned char bLightsOn: 1; // Are the lights switched on ?
    unsigned char bFreebies: 1; // Any freebies left in this vehicle ?

	//0x429
    unsigned char bIsVan: 1; // Is this vehicle a van (doors at back of vehicle)
    unsigned char bIsBus: 1; // Is this vehicle a bus
    unsigned char bIsBig: 1; // Is this vehicle big
    unsigned char bLowVehicle: 1; // Need this for sporty type cars to use low getting-in/out anims
    unsigned char bComedyControls: 1; // Will make the car hard to control (hopefully in a funny way)
    unsigned char bWarnedPeds: 1; // Has scan and warn peds of danger been processed?
    unsigned char bCraneMessageDone: 1; // A crane message has been printed for this car allready
    // unsigned char bExtendedRange: 1; // This vehicle needs to be a bit further away to get deleted
    unsigned char bTakeLessDamage: 1; // This vehicle is stronger (takes about 1/4 of damage)

    //0x42A
    unsigned char bIsDamaged: 1; // This vehicle has been damaged and is displaying all its components
    unsigned char bHasBeenOwnedByPlayer : 1;// To work out whether stealing it is a crime
    unsigned char bFadeOut: 1; // Fade vehicle out
    unsigned char bIsBeingCarJacked: 1;
    unsigned char bCreateRoadBlockPeds : 1;// If this vehicle gets close enough we will create peds (coppers or gang members) round it
    unsigned char bCanBeDamaged: 1; // Set to FALSE during cut scenes to avoid explosions
    // unsigned char bUsingSpecialColModel : 1;
    // Is player vehicle using special collision model, stored in player strucure
    unsigned char bOccupantsHaveBeenGenerated : 1; // Is true if the occupants have already been generated. (Shouldn't happen again)
    unsigned char bGunSwitchedOff: 1; // Level designers can use this to switch off guns on boats
    
	//0x42B
    unsigned char bVehicleColProcessed : 1;// Has ProcessEntityCollision been processed for this car?
    unsigned char bIsCarParkVehicle: 1; // Car has been created using the special CAR_PARK script command
    unsigned char bHasAlreadyBeenRecorded : 1; // Used for replays
    unsigned char bPartOfConvoy: 1;
    unsigned char bHeliMinimumTilt: 1; // This heli should have almost no tilt really
    unsigned char bAudioChangingGear: 1; // sounds like vehicle is changing gear
    unsigned char bIsDrowning: 1; // is vehicle occupants taking damage in water (i.e. vehicle is dead in water)
    unsigned char bTyresDontBurst: 1; // If this is set the tyres are invincible

    //0x42C
    unsigned char bCreatedAsPoliceVehicle : 1;// True if this guy was created as a police vehicle (enforcer, policecar, miamivice car etc)
    unsigned char bRestingOnPhysical: 1; // Dont go static cause car is sitting on a physical object that might get removed
    unsigned char      bParking                    : 1;
    unsigned char      bCanPark                    : 1;
    unsigned char bFireGun: 1; // Does the ai of this vehicle want to fire it's gun?
    unsigned char bDriverLastFrame: 1; // Was there a driver present last frame ?
    unsigned char bNeverUseSmallerRemovalRange: 1;// Some vehicles (like planes) we don't want to remove just behind the camera.
    unsigned char bIsRCVehicle: 1; // Is this a remote controlled (small) vehicle. True whether the player or AI controls it.

    //0x42D
    unsigned char bAlwaysSkidMarks: 1; // This vehicle leaves skidmarks regardless of the wheels' states.
    unsigned char bEngineBroken: 1; // Engine doesn't work. Player can get in but the vehicle won't drive
    unsigned char bVehicleCanBeTargetted : 1;// The ped driving this vehicle can be targetted, (for Torenos plane mission)
    unsigned char bPartOfAttackWave: 1; // This car is used in an attack during a gang war
    unsigned char bWinchCanPickMeUp: 1; // This car cannot be picked up by any ropes.
    unsigned char bImpounded: 1; // Has this vehicle been in a police impounding garage
    unsigned char bVehicleCanBeTargettedByHS  : 1;// Heat seeking missiles will not target this vehicle.
    unsigned char bSirenOrAlarm: 1; // Set to TRUE if siren or alarm active, else FALSE

    //0x42E
    unsigned char bHasGangLeaningOn: 1;
    unsigned char bGangMembersForRoadBlock : 1;// Will generate gang members if NumPedsForRoadBlock > 0
    unsigned char bDoesProvideCover: 1; // If this is false this particular vehicle can not be used to take cover behind.
    unsigned char bMadDriver: 1; // This vehicle is driving like a lunatic
    unsigned char bUpgradedStereo: 1; // This vehicle has an upgraded stereo
    unsigned char bConsideredByPlayer: 1; // This vehicle is considered by the player to enter
    unsigned char bPetrolTankIsWeakPoint : 1;// If false shootong the petrol tank will NOT Blow up the car
    unsigned char bDisableParticles: 1; // Disable particles from this car. Used in garage.

    //0x42F
    unsigned char bHasBeenResprayed: 1; // Has been resprayed in a respray garage. Reset after it has been checked.
    unsigned char bUseCarCheats: 1; // If this is true will set the car cheat stuff up in ProcessControl()
    unsigned char bDontSetColourWhenRemapping : 1;// If the texture gets remapped we don't want to change the colour with it.
    unsigned char bUsedForReplay: 1; // This car is controlled by replay and should be removed when replay is done.
};

class NOVMT CVehicle : public CPhysical
{
protected:
	BYTE			__pad1[752];
	CVehicleFlags	m_nVehicleFlags;
	BYTE			__pad5[48];
	class CPed*		m_pDriver;
    class CPed*		m_apPassengers[8];
	BYTE			__pad2[76];
	signed int		m_nTimeTillWeNeedThisCar;
	BYTE			__pad4[56];
	CEntity*		pDamagingEntity;
	BYTE			__pad3[128];
	eVehicleType	m_dwVehicleClass;
	eVehicleType	m_dwVehicleSubClass;
	short			m_wPreviousRemapTxd;
    short			m_wRemapTxd;
    RwTexture*		m_pRemapTexture;

public:
	static bool&	m_bEnableMouseSteering;
	static bool&	m_bEnableMouseFlying;

	CVehicleFlags&	GetVehicleFlags() 
						{ return m_nVehicleFlags; }
	CEntity*		GetDamagingEntity()
						{ return pDamagingEntity; }
	eVehicleType	GetSubClass()
		{ return m_dwVehicleSubClass; }

	virtual void	Render() override;

	void			SetWindowOpenFlag(unsigned char nWindow);
	void			ClearWindowOpenFlag(unsigned char nWindow);

	void			RenderForShadow(RpClump* pClump);

	static void		SetComponentAtomicAlpha(RpAtomic* pAtomic, int nAlpha);
};

class NOVMT CAutomobile : public CVehicle
{
public:
	BYTE		paddd[172];
	RwFrame*	m_pCarNode[20];		// May be wrong?
	BYTE		padding[432];
	float		m_fRotorSpeed;
	BYTE		__moarpad[72];

public:
	CColModel*	RenderAntennas();
	void		DebugWheelDisplay();

	// Test
	void		ControlWindows();
};

class NOVMT CHeli : public CAutomobile
{
public:
	inline void			Render_Stub()
		{ CHeli::Render(); }

	virtual void		Render() override;
};

class NOVMT CPlane : public CAutomobile
{
public:
	inline void			Render_Stub()
		{ CPlane::Render(); }

	virtual void		Render() override;
};

static_assert(sizeof(CVehicle) == 0x5A0, "Wrong size: CVehicle");

#endif