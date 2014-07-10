#ifndef __OBJECT
#define __OBJECT

class NOVMT CObject : public CPhysical
{
public:
	void*				m_pObjectList;
	unsigned char		m_nObjectType;
	__int8				field_13D;
	__int16				field_13E;
	bool				bObjectFlag0 : 1;
	bool				bObjectFlag1 : 1;
	bool				bObjectFlag2 : 1;
	bool				bObjectFlag3 : 1;
	bool				bObjectFlag4 : 1;
	bool				bObjectFlag5 : 1;
	bool				m_bIsExploded : 1;
	bool				bObjectFlag7 : 1;
	bool				m_bIsLampPost : 1;
	bool				m_bIsTargetable : 1;
	bool				m_bIsBroken : 1;
	bool				m_bTrainCrossEnabled : 1;
	bool				m_bIsPhotographed : 1;
	bool				m_bIsLiftable : 1;
	bool				bObjectFlag14 : 1;
	bool				m_bIsDoor : 1;
	bool				m_bHasNoModel : 1;
	bool				m_bIsScaled : 1;
	bool				m_bCanBeAttachedToMagnet : 1;
	bool				bObjectFlag19 : 1;
	bool				bObjectFlag20 : 1;
	bool				bObjectFlag21 : 1;
	bool				m_bFadingIn : 1;
	bool				m_bAffectedByColBrightness : 1;
	bool				bObjectFlag24 : 1;
	bool				m_bDoNotRender : 1;
	bool				m_bFadingIn2 : 1;
	bool				bObjectFlag27 : 1;
	bool				bObjectFlag28 : 1;
	bool				bObjectFlag29 : 1;
	bool				bObjectFlag30 : 1;
	bool				bObjectFlag31 : 1;
	unsigned char		m_nColDamageEffect;
	__int8 field_145;
	__int8 field_146;
	__int8 field_147;
	unsigned char		m_nLastWeaponDamage;
	unsigned char		m_nColBrightness;
	__int16             m_wCarPartModelIndex;
	// this is used for detached car parts
	unsigned __int8     m_nCarColor[4];
	// time when this object must be deleted
	unsigned __int32    m_dwRemovalTime;
	float               m_fHealth;
	// this is used for door objects
	float               m_fDoorStartAngle;
	float               m_fScale;
	void*				m_pObjectInfo;
	void*				m_pFire; // CFire *
	short				field_168;
	// this is used for detached car parts
	short				m_wPaintjobTxd;
	// this is used for detached car parts
	RwTexture*          m_pPaintjobTex;
	void*				m_pDummyObject;
	// time when particles must be stopped
	signed int			m_dwTimeToStopParticles;
	float               m_fParticlesIntensity;

public:
	virtual void		Render() override;
};

static_assert(sizeof(CObject) == 0x17C, "Wrong size: CObject");

#endif