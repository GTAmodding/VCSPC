#ifndef __MODELINFO
#define __MODELINFO

#include "EmpireMgr.h"
#include "General.h"

// Not used with CDynamicStore-based storages
#define NUM_VEHICLE_MODELS									212
#define NUM_PED_MODELS										278
#define NUM_DAMAGEATOMIC_MODELS								100

#define NUM_PED_COLOURS										128
#define NUM_POSSIBLE_COLOURS_FOR_PED						16

enum eVehicleTypes
{
    VT_LANDSTAL = 400,
    VT_BRAVURA,
    VT_BUFFALO,
    VT_LINERUN,
    VT_PEREN,
    VT_SENTINEL,
    VT_DUMPER,
    VT_FIRETRUK,
    VT_TRASH,
    VT_STRETCH,
    VT_MANANA,
    VT_INFERNUS,
    VT_VOODOO,
    VT_PONY,
    VT_MULE,
    VT_CHEETAH,
    VT_AMBULAN,
    VT_LEVIATHN,
    VT_MOONBEAM,
    VT_ESPERANT,
    VT_TAXI,
    VT_WASHING,
    VT_BOBCAT,
    VT_MRWHOOP,
    VT_BFINJECT,
    VT_HUNTER,
    VT_PREMIER,
    VT_ENFORCER,
    VT_SECURICA,
    VT_BANSHEE,
    VT_PREDATOR,
    VT_BUS,
    VT_RHINO,
    VT_BARRACKS,
    VT_HOTKNIFE,
    VT_ARTICT1,
    VT_PREVION,
    VT_COACH,
    VT_CABBIE,
    VT_STALLION,
    VT_RUMPO,
    VT_RCBANDIT,
    VT_ROMERO,
    VT_PACKER,
    VT_MONSTER,
    VT_ADMIRAL,
    VT_SQUALO,
    VT_SEASPAR,
    VT_PIZZABOY,
    VT_TRAM,
    VT_ARTICT2,
    VT_TURISMO,
    VT_SPEEDER,
    VT_REEFER,
    VT_TROPIC,
    VT_FLATBED,
    VT_YANKEE,
    VT_CADDY,
    VT_SOLAIR,
    VT_TOPFUN,
    VT_SKIMMER,
    VT_PCJ600,
    VT_FAGGIO,
    VT_FREEWAY,
    VT_RCBARON,
    VT_RCRAIDER,
    VT_GLENDALE,
    VT_OCEANIC,
    VT_SANCHEZ,
    VT_SPARROW,
    VT_PATRIOT,
    VT_QUAD,
    VT_COASTG,
    VT_DINGHY,
    VT_HERMES,
    VT_SABRE,
    VT_RUSTLER,
    VT_ZR350,
    VT_WALTON,
    VT_REGINA,
    VT_COMET,
    VT_BMXBOY,
    VT_BURRITO,
    VT_CAMPER,
    VT_MARQUIS,
    VT_BAGGAGE,
    VT_DOZER,
    VT_MAVERICK,
    VT_VCNMAV,
    VT_RANCHER,
    VT_FBIRANCH,
    VT_VIRGO,
    VT_GREENWOO,
    VT_JETMAX,
    VT_HOTRING,
    VT_SANDKING,
    VT_BLISTAC,
    VT_POLMAV,
    VT_BOXVILLE,
    VT_BENSON,
    VT_MESA,
    VT_RCGOBLIN,
    VT_HOTRINA,
    VT_HOTRINB,
    VT_BLOODRA,
    VT_RNCHLURE,
    VT_VICECHEE,
    VT_ELEGANT,
    VT_JOURNEY,
    VT_BMXGIRL,
    VT_MTBIKE,
    VT_BEAGLE,
    VT_CROPDUST,
    VT_STUNT,
    VT_PETRO,
    VT_RDTRAIN,
    VT_NEBULA,
    VT_MAJESTIC,
    VT_BUCCANEE,
    VT_SHAMAL,
    VT_HYDRA,
    VT_FCR900,
    VT_NRG500,
    VT_ELECTRAP,
    VT_CEMENT,
    VT_TOWTRUCK,
    VT_FORTUNE,
    VT_CADRONA,
    VT_FBITRUCK,
    VT_WILLARD,
    VT_FORKLIFT,
    VT_TRACTOR,
    VT_COMBINE,
    VT_FELTZER,
    VT_CUBAN,
    VT_SLAMVAN,
    VT_CHOLLO,
    VT_FREIGHT,
    VT_STREAK,
    VT_VORTEX,
    VT_VINCENT,
    VT_BULLET,
    VT_CLOVER,
    VT_SABRETB,
    VT_FIRELA,
    VT_HUSTLER,
    VT_INTRUDER,
    VT_PRIMO,
    VT_CARGOBOB,
    VT_IDAHO,
    VT_SUNRISE,
    VT_MERIT,
    VT_UTILITY,
    VT_NEVADA,
    VT_YOSEMITE,
    VT_WINDSOR,
    VT_MONSTERA,
    VT_MONSTERB,
    VT_URANUS,
    VT_JESTER,
    VT_PIMP,
    VT_STRATUM,
    VT_ELEGY,
    VT_RAINDANC,
    VT_RCTIGER,
    VT_FLASH,
    VT_TAHOMA,
    VT_SAVANNA,
    VT_BANDITO,
    VT_FREIFLAT,
    VT_STREAKC,
    VT_KART,
    VT_MOWER,
    VT_DUNERIDE,
    VT_SWEEPER,
    VT_BROADWAY,
    VT_TORNADO,
    VT_AT400,
    VT_DFT30,
    VT_GANGRAN,
    VT_STAFFORD,
    VT_STREETFI,
    VT_NEWSVAN,
    VT_6ATV,
    VT_PETROTR,
    VT_SENTXS,
    VT_ELECTRAG,
    VT_STINGER,
    VT_HOTDOG,
    VT_MOP50,
    VT_FREIBOX,
    VT_ARTICT3,
    VT_ANDROM,
    VT_DODO,
    VT_RCCAM,
    VT_LAUNCH,
    VT_COPCARLA,
    VT_POLICEM,
    VT_COPCARVG,
    VT_COPCARRU,
    VT_ANGEL,
    VT_SWATVAN,
    VT_DELUXO,
    VT_PHOENIX,
    VT_ZEBRA,
    VT_SADLSHIT,
    VT_BAGBOXA,
    VT_BAGBOXB,
    VT_TUGSTAIR,
    VT_BOXBURG,
    VT_FARMTR1,
    VT_UTILTR1,
    VT_MAX
};

template<class T, unsigned int nCount>
class CStore
{
public:
	unsigned int		m_NumObjects;
	T					m_Objects[nCount];
};

template<class T>
class CDynamicStore
{
public:
	struct Entry
	{
		Entry*			m_pPrev;
		T				m_entry;
	};

private:
	Entry*				m_pCurrent;

public:
	inline CDynamicStore()
		: m_pCurrent(nullptr)
	{}

	~CDynamicStore()
	{
		Entry*		pCurrentEntry = m_pCurrent;

		while ( pCurrentEntry )
		{
			Entry*	pPrevEntry = pCurrentEntry->m_pPrev;
			delete pCurrentEntry;
			pCurrentEntry = pPrevEntry;
		}
	}

	inline Entry*			GetFirstEntry()
		{ return m_pCurrent; }

	inline T*				Add()
		{	Entry* pNewEntry = new Entry; pNewEntry->m_pPrev = m_pCurrent; m_pCurrent = pNewEntry;
			return &m_pCurrent->m_entry; }
};

// TODO: Move to a separate file?
typedef struct
{
    CVector     vecMin;
    CVector     vecMax;
    CVector     vecOffset;
    FLOAT       fRadius;
} CBoundingBox;


typedef struct
{
    CVector     vecCenter;
    float       fRadius;
} CColSphere;


typedef struct
{
    CVector     min;
    CVector     max;
} CColBox;


/*typedef struct
{
    unsigned short  v1;
    unsigned short  v2;
    unsigned short  v3;
    EColSurface     material;
    CColLighting    lighting;
} CColTriangle;


typedef struct
{
    BYTE pad0 [ 12 ];
} CColTrianglePlane;*/


typedef struct
{
    char version[4];
    DWORD size;
    char name[0x18];
} ColModelFileHeader;

typedef struct
{
    WORD                            numColSpheres;
    WORD                            numColBoxes;
    WORD                            numColTriangles;
    BYTE                            ucNumWheels;
    BYTE                            pad3;
    CColSphere*						pColSpheres;
    CColBox*						pColBoxes;
    void*                           pSuspensionLines;
    void*                           pUnknown;
    void*							pColTriangles;
    void*							pColTrianglePlanes;
} CColData;

class CColModel
{
public:
    CBoundingBox	                boundingBox;
    BYTE                            level;
    BYTE                            unknownFlags;
    BYTE                            pad [ 2 ];
    CColData*						pColData;

public:
	~CColModel()
	{
		// Dirty!
		((void(__thiscall*)(CColModel*))0x40F700)(this);
	}

	void operator delete(void* ptr);
};

class C2dEffect
{
public:
	CVector							vecPos;
	BYTE							bType;
	DWORD							nCount;
};

class CTimeInfo
{
public:
	unsigned char					bTimeOn, bTimeOff;
	signed short					nPairedModel;

public:
	CTimeInfo(signed short nModel)
		: nPairedModel(nModel)
	{}
};

class CTempColModels
{
public:
	static CColModel&				ms_colModelWeapon;
};

class CAtomicModelInfo;
class CDamageAtomicModelInfo;
class CLodAtomicModelInfo;

class CBaseModelInfo
{
public:
    unsigned int			ulHashKey;                  // +4   Generated by CKeyGen::GetUppercaseKey(char const *) called by CBaseModelInfo::SetModelName(char const *)
    unsigned short          usNumberOfRefs: 16;         // +8
    short					usTextureDictionary: 16;    // +10
    unsigned char           ucAlpha: 8;                 // +12

    unsigned char           ucNumOf2DEffects: 8;        // +13
    unsigned short          usUnknown: 16;              // +14     Something with 2d effects

    unsigned char           ucDynamicIndex: 8;          // +16

    unsigned char           dwUnknownFlag9: 1;          // +17
    unsigned char           dwUnknownFlag10: 1;
    unsigned char           dwUnknownFlag11: 1;
    unsigned char           dwUnknownFlag12: 1;
    unsigned char           dwUnknownFlag13: 1;
    unsigned char           dwUnknownFlag14: 1;
    unsigned char           dwUnknownFlag15: 1;
    unsigned char           dwUnknownFlag16: 1;

    // Flags used by CBaseModelInfo
	unsigned char           bHasBeenPreRendered: 1;     // +18
	unsigned char           bAlphaTransparency: 1;
	unsigned char           bIsLod: 1;
	unsigned char           bDontCastShadowsOn: 1;
	unsigned char           bDontWriteZBuffer: 1;
	unsigned char           bDrawAdditive: 1;
	unsigned char           bDrawLast: 1;
	unsigned char           bDoWeOwnTheColModel: 1;

    unsigned char           dwUnknownFlag25: 1;         // +19
    unsigned char           dwUnknownFlag26: 1;
    unsigned char           dwUnknownFlag27: 1;
    unsigned char           bSwaysInWind: 1;
    unsigned char           bCollisionWasStreamedWithModel: 1;  // CClumpModelInfo::SetCollisionWasStreamedWithModel(unsigned int)
    unsigned char           bDontCollideWithFlyer: 1;           // CAtomicModelInfo::SetDontCollideWithFlyer(unsigned int)
    unsigned char           bHasComplexHierarchy: 1;            // CClumpModelInfo::SetHasComplexHierarchy(unsigned int)
    unsigned char           bWetRoadReflection: 1;              // CAtomicModelInfo::SetWetRoadReflection(unsigned int)
    CColModel*				pColModel;                  // +20      CColModel: public CBoundingBox
    float                   fLodDistanceUnscaled;       // +24      Scaled is this value multiplied with flt_B6F118
    RwObject*               pRwObject;                  // +28

public:
	virtual							~CBaseModelInfo() {}
	virtual CAtomicModelInfo*		AsAtomicModelInfoPtr() { return nullptr; }
    virtual CDamageAtomicModelInfo*	AsDamageAtomicModelInfoPtr() { return nullptr; }
    virtual CLodAtomicModelInfo*	AsLodAtomicModelInfoPtr() { return nullptr; }
    virtual unsigned char			GetModelType()=0;
	virtual CTimeInfo*				GetTimeInfo() { return nullptr; }
    virtual void					Init();
    virtual void					Shutdown();
    virtual void					DeleteRwObject()=0;
    virtual int						GetRwModelType()=0;
	virtual RpAtomic*				CreateInstance_(RwMatrix* pMatrix)=0;
    virtual RpAtomic*				CreateInstance()=0;
	virtual void					SetAnimFile(const char* pName) { UNREFERENCED_PARAMETER(pName); }
	virtual void					ConvertAnimFileIndex() {}
	virtual int						GetAnimFileIndex() { return -1; }

	CBaseModelInfo()
		: usNumberOfRefs(0), usTextureDictionary(-1)
	{}

	inline CColModel*		GetColModel() { return pColModel; }
	inline unsigned int		GetHash() { return ulHashKey; }
	inline short			GetTextureDict() { return usTextureDictionary; }

	void					SetColModel(CColModel* pModel, bool bInitPaired);
	void					RecalcDrawDistance(float fOldDist);
	void					SetTexDictionary(const char* pDict);
	void					AddRef();
};

class CClumpModelInfo : public CBaseModelInfo
{
public:
	int						nAnimIndex;

public:
	virtual unsigned char			GetModelType() override { return 5; }
	virtual void					Init() override;
	virtual void					DeleteRwObject() override;
	virtual int						GetRwModelType() override { return rpCLUMP; }
	virtual RpAtomic*				CreateInstance_(RwMatrix* pMatrix) override;
	virtual RpAtomic*				CreateInstance() override;
	virtual void					SetAnimFile(const char* pName) override;
	virtual void					ConvertAnimFileIndex() override;
	virtual int						GetAnimFileIndex() override { return nAnimIndex; }
	virtual CColModel*				GetBoundingBox() { return pColModel; }
	virtual void					SetClump(RpClump* pClump);
};

class CAtomicModelInfo : public CBaseModelInfo
{
private:
	// VCS PC class extension (NONSTATIC)
	CEmpireBuildingData*			pEmpireData;

public:
	virtual CAtomicModelInfo*		AsAtomicModelInfoPtr() override { return this; }
	virtual unsigned char			GetModelType() override { return 1; }
	virtual void					Shutdown() override;
	virtual void					DeleteRwObject() override;
	virtual int						GetRwModelType() override { return rpATOMIC; }
	virtual RpAtomic*				CreateInstance_(RwMatrix* pMatrix) override;
	virtual RpAtomic*				CreateInstance() override;
	virtual void					SetAtomic(RpAtomic* pAtomic);

	inline void						InitEmpireData()
		{ if ( !pEmpireData ) pEmpireData = new CEmpireBuildingData; }
	inline CEmpireBuildingData*		GetEmpireData()
		{ return pEmpireData; }

	CAtomicModelInfo()
		: pEmpireData(nullptr)
	{}
};

class CDamageAtomicModelInfo : public CAtomicModelInfo
{
public:
	RpAtomic*				pAtomic2;

public:
	virtual CDamageAtomicModelInfo*	AsDamageAtomicModelInfoPtr() override { return this; }
	virtual void					Init() override;
	virtual void					DeleteRwObject() override;
	virtual RpAtomic*				CreateInstance_(RwMatrix* pMatrix) override;
	virtual RpAtomic*				CreateInstance() override;
};

class CTimeModelInfo : public CAtomicModelInfo
{
private:
	CTimeInfo				timeInfo;

public:
	virtual unsigned char			GetModelType() override { return 3; }
	virtual CTimeInfo*				GetTimeInfo() override { return &timeInfo; }

	CTimeModelInfo()
		: timeInfo(-1)
	{}
};

class CWeaponModelInfo : public CClumpModelInfo
{
	int						WeaponInfo;
};

class CPedModelInfo : public CClumpModelInfo
{
public:
	DWORD					MotionAnimGroup;
	DWORD					DefPedType;
	DWORD					DefPedStats;
	WORD					CanDriveCars;
	WORD					PedFlags;
	CColModel*				HitColModel;
	BYTE					FirstRadioStation;
	BYTE					SecondRadioStation;
	BYTE					Race;
	WORD					AudioPedType;
	WORD					FirstVoice;
	WORD					LastVoice;
	WORD					NextVoice;

public:
	virtual unsigned char			GetModelType() override { return 7; }
	virtual void					DeleteRwObject() override;
	virtual void					SetClump(RpClump* pClump) override;

	CPedModelInfo()
		: HitColModel(nullptr)
	{}

	virtual ~CPedModelInfo()
	{
		delete HitColModel;
	}
};

class CVehicleModelInfo : public CClumpModelInfo
{
	// TODO: Document

private:
	static RwTexture*&				ms_pRemapTexture;
	static RwTexture*&				ms_pLightsTexture;
	static RwTexture*&				ms_pLightsOnTexture;
	static bool* const				ms_lightsOn;
	static unsigned char* const		ms_currentCol;
	static CRGBA* const				ms_vehicleColourTable;

	static const char*&				ms_pCurrentCarcolsFile;

public:
	static void						LoadVehicleColours();

public:
	static inline void				SetRemap(RwTexture* pTex)
		{ ms_pRemapTexture = pTex; }

	static RpAtomic*				SetEditableMaterialsCB(RpAtomic* pMaterial, void* pData);
	static void						ResetEditableMaterials();

	static void						SetVehicleColour(unsigned char primaryColour, unsigned char secondaryColour, unsigned char tertiaryColour, unsigned char quaternaryColour);
	static RpMaterial*				SetEditableMaterialsCB(RpMaterial* pMaterial, void* pData);
	static void						LoadAllVehicleColours();
};

class CPedModelInfoVCS : public CPedModelInfo // VCS PC class extension
{
public:
	unsigned char			m_primaryPedColours[NUM_POSSIBLE_COLOURS_FOR_PED];
	unsigned char			m_secondaryPedColours[NUM_POSSIBLE_COLOURS_FOR_PED];
	unsigned char			m_tertiaryPedColours[NUM_POSSIBLE_COLOURS_FOR_PED];
	unsigned char			m_quaternaryPedColours[NUM_POSSIBLE_COLOURS_FOR_PED];
	unsigned char			m_bNumPossibleColours;
	unsigned char			m_bLastColourChoice;

	static CRGBA			ms_pedColourTable[NUM_PED_COLOURS];
	static unsigned char	ms_currentCol[4];

public:
	static void				LoadPedColours();
	static void				SetPedColour(unsigned char primaryColour, unsigned char secondaryColour, unsigned char tertiaryColour, unsigned char quaternaryColour);
	static void				SetEditableMaterials(RpClump* pClump);
	static RpAtomic*		SetEditableMaterialsCB(RpAtomic* pAtomic, void* pData);
	static RpMaterial*		SetEditableMaterialsCB(RpMaterial* pMaterial, void* pData);

	void					GetRandomPedColour(BYTE& colour1, BYTE& colour2, BYTE& colour3, BYTE& colour4);
};

class CModelInfoDrawDistStorage
{
private:
	unsigned short*	m_pMalloc;
	int				m_nMin, m_nMax;

private:
	inline unsigned short	Pack(float value)
		{ return static_cast<unsigned short>(round(value * 20000 / 1000)); }
	inline float	Unpack(unsigned short value)
	{ return static_cast<float>(value) * 1000 / 20000; }

public:
	inline void		Init()
		{ m_nMin = m_nMax = 0; m_pMalloc = new unsigned short[20000]; }
	inline void		Shrink()
	{	unsigned short* pNewMalloc = new unsigned short[m_nMax-m_nMin];
		memcpy(pNewMalloc, &m_pMalloc[m_nMin], (m_nMax-m_nMin) * sizeof(unsigned short));
		delete m_pMalloc;
		m_pMalloc = pNewMalloc;
	}

	inline void		Shutdown()
		{ delete m_pMalloc; }

	void			AddToList(int nIndex, CBaseModelInfo* pModelInfo);
	void			RecalcDrawDistances();
};

class CModelInfo
{
public:
	static CBaseModelInfo** const						ms_modelInfoPtrs;

#if NUM_VEHICLES > 212
	ModelCarsData*		ModelCarsMalloc;
#endif
	static CDynamicStore<CPedModelInfoVCS>				ms_pedModelStore;
	static CDynamicStore<CAtomicModelInfo>				ms_atomicModelStore;
	static CDynamicStore<CDamageAtomicModelInfo>		ms_damageAtomicModelStore;
	static CDynamicStore<CTimeModelInfo>				ms_timeModelStore;
	static CModelInfoDrawDistStorage					ms_drawDistStorage;

public:
	static inline CModelInfoDrawDistStorage*			GetDrawDistanceStorage()
		{ return &ms_drawDistStorage; }
	static inline void									RecalcDrawDistances()
		{ ms_drawDistStorage.RecalcDrawDistances(); }

	static inline unsigned short						GetModelInfoUInt16(const char* pName)
		{ int	nID; GetModelInfo(pName, &nID); return static_cast<unsigned short>(nID); }

	static CBaseModelInfo*								GetModelInfo(const char* pName, int* pOutID = nullptr);
	static CAtomicModelInfo*							AddAtomicModel(int nModelID);
	static bool											IsBikeModel(int modelID);
	static CPedModelInfoVCS*							AddPedModel(int nModelID);
	static CDamageAtomicModelInfo*						AddDamageAtomicModel(int nModelID);
	static CTimeModelInfo*								AddTimeModel(int nModelID);
	static CWeaponModelInfo*							AddWeaponModel(int nModelID);
	static void											ShutDown();

	static void											Inject();
};

extern std::pair<void*,int>* const materialRestoreData;

static_assert(sizeof(CBaseModelInfo) == 0x20, "Wrong size: CBaseModelInfo");
static_assert(sizeof(CPedModelInfo) == 0x44, "Wrong size: CPedModelInfo");

#endif