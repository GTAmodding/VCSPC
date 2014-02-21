#ifndef __CWEAPONINFO
#define __CWEAPONINFO

enum eFireType
{
    FIRETYPE_MELEE,
    FIRETYPE_INSTANT_HIT,
    FIRETYPE_PROJECTILE,
    FIRETYPE_AREA_EFFECT,
    FIRETYPE_CAMERA,
    FIRETYPE_USE,

    FIRETYPE_LAST_FIRETYPE
};

enum eWeaponSlot
{
    WEAPONSLOT_TYPE_UNARMED,
    WEAPONSLOT_TYPE_MELEE,
    WEAPONSLOT_TYPE_HANDGUN,
    WEAPONSLOT_TYPE_SHOTGUN,
    WEAPONSLOT_TYPE_SMG,        //4
    WEAPONSLOT_TYPE_MG, 
    WEAPONSLOT_TYPE_RIFLE,
    WEAPONSLOT_TYPE_HEAVY,
    WEAPONSLOT_TYPE_THROWN,
    WEAPONSLOT_TYPE_SPECIAL,    //9
    WEAPONSLOT_TYPE_GIFT,       //10
    WEAPONSLOT_TYPE_PARACHUTE,  //11
    WEAPONSLOT_TYPE_DETONATOR,  //12

    WEAPONSLOT_MAX
};

enum eWeaponState
{
    WEAPONSTATE_READY,
    WEAPONSTATE_FIRING,
    WEAPONSTATE_RELOADING,
    WEAPONSTATE_OUT_OF_AMMO,
    WEAPONSTATE_MELEE_MADECONTACT
};

enum eWeaponType
{
    WEAPONTYPE_UNARMED,
    WEAPONTYPE_BRASSKNUCKLE, 
    WEAPONTYPE_GOLFCLUB,
    WEAPONTYPE_NIGHTSTICK,
    WEAPONTYPE_KNIFE,
    WEAPONTYPE_BASEBALLBAT,
    WEAPONTYPE_SHOVEL,
    WEAPONTYPE_POOL_CUE,
    WEAPONTYPE_KATANA,
    WEAPONTYPE_CHAINSAW,
    
    // gifts
    WEAPONTYPE_DILDO1, // 10
    WEAPONTYPE_DILDO2,
    WEAPONTYPE_VIBE1,
    WEAPONTYPE_VIBE2,
    WEAPONTYPE_FLOWERS,
    WEAPONTYPE_CANE,

    WEAPONTYPE_GRENADE,
    WEAPONTYPE_TEARGAS,
    WEAPONTYPE_MOLOTOV,
    WEAPONTYPE_ROCKET,
    WEAPONTYPE_ROCKET_HS, // 20
    WEAPONTYPE_FREEFALL_BOMB,

    // FIRST SKILL WEAPON
    WEAPONTYPE_PISTOL,          // handguns
    WEAPONTYPE_UZI,
    WEAPONTYPE_PYTHON,
    WEAPONTYPE_SHOTGUN,         // shotguns
    WEAPONTYPE_STUBBY,
    WEAPONTYPE_SPAS12_SHOTGUN,
    WEAPONTYPE_INGRAMSL,       // submachine guns
    WEAPONTYPE_MP5,
    WEAPONTYPE_AK47, // 30      // machine guns 
    WEAPONTYPE_M16,          
    WEAPONTYPE_SKORPION,            // this uses stat from the micro_uzi
    // END SKILL WEAPONS
    
    WEAPONTYPE_SNIPER,
    WEAPONTYPE_LASER, 
    WEAPONTYPE_ROCKETLAUNCHER,  // specials
    WEAPONTYPE_M249,
    WEAPONTYPE_FLAMETHROWER,
    WEAPONTYPE_MINIGUN,
    WEAPONTYPE_REMOTE_SATCHEL_CHARGE,
    WEAPONTYPE_DETONATOR, // 40 // plastic explosive
    WEAPONTYPE_SPRAYCAN,
    WEAPONTYPE_EXTINGUISHER,
    WEAPONTYPE_CAMERA,
    WEAPONTYPE_NIGHTVISION,
    WEAPONTYPE_INFRARED,
    WEAPONTYPE_PARACHUTE,
    WEAPONTYPE_LAST_WEAPONTYPE,

    WEAPONTYPE_ARMOUR,
    // these are possible ways to die
    WEAPONTYPE_RAMMEDBYCAR,
    WEAPONTYPE_RUNOVERBYCAR, // 50
    WEAPONTYPE_EXPLOSION,
    WEAPONTYPE_UZI_DRIVEBY,
    WEAPONTYPE_DROWNING,
    WEAPONTYPE_FALL,
    WEAPONTYPE_UNIDENTIFIED,    // Used for damage being done
    WEAPONTYPE_ANYMELEE,
    WEAPONTYPE_ANYWEAPON,
    WEAPONTYPE_FLARE
};

class CWeaponInfo
{
private:
	DWORD				weaponType;
	DWORD				targetRange;
	DWORD				weaponRange;
	DWORD				dwModelID;
	DWORD				dwModelID2;
	DWORD				nSlot;
	DWORD				hexFlags;
	DWORD				animStyle;
	WORD				ammoClip;
	DWORD				fireOffsetX;
	DWORD				fireOffsetY;
	DWORD				fireOffsetZ;
	DWORD				skillLevel;
	DWORD				reqStatLevel;
	float				accuracy;
	DWORD				moveSpeed;
	DWORD				animLoopStart;
	DWORD				animLoopEnd;
	DWORD				animLoopFire;
	DWORD				animLoop2Start;
	DWORD				animLoop2End;
	DWORD				animLoop2Fire;
	DWORD				breakoutTime;
	DWORD				speed;
	DWORD				radius;
	DWORD				lifespan;
	DWORD				spread;
	DWORD				animStyle2;

public:
	float				GetAccuracy() 
							{ return accuracy; };
	DWORD				GetWeaponType() 
							{ return weaponType; };
	DWORD				GetClipSize() 
							{ return ammoClip; };
	DWORD				GetWeaponSlot() 
							{ return nSlot; };

	static CWeaponInfo*	GetWeaponInfo(eWeaponType weaponID, signed char bType);
};

//static_assert(sizeof(CWeaponInfo) == CWeaponInfo_ARRAYSIZE);

#endif