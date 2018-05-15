#ifndef __SHADOWS
#define __SHADOWS

enum eShadowQuality
{
	SHADOW_QUALITY_OFF,
	SHADOW_QUALITY_LOWEST,
	SHADOW_QUALITY_LOW,
	SHADOW_QUALITY_MEDIUM,
	SHADOW_QUALITY_HIGH,
	SHADOW_QUALITY_HIGHEST,

	SHADOW_QUALITY_UNDEFINED
};

class CShadows
{
private:
	// VCS PC class extension
	static eShadowQuality			m_bShadowQuality;
	static float					m_fShadowDistMult;

public:
	static void						RenderIndicatorShadow(unsigned int nIndex, unsigned char, RwTexture*, CVector* pPos, float radiusX, float, float, float radiusY, short);

	static inline void				SetShadowQuality(eShadowQuality nQuality)
		{ m_bShadowQuality = nQuality; }
	static inline eShadowQuality	GetShadowQuality()
		{ return m_bShadowQuality; }
	static inline void				SetShadowDistance(float fMult)
		{ m_fShadowDistMult = fMult; }
	static inline float				GetShadowDistance()
		{ return m_fShadowDistMult; }

	static inline bool				DontRenderShadowsForPoles()
		{ return m_bShadowQuality == SHADOW_QUALITY_OFF || m_bShadowQuality > SHADOW_QUALITY_MEDIUM; }

	static void Init(void);
	static void UpdateStaticShadows(void);
	static void RenderStaticShadows(void);
	static void RenderStoredShadows(void);
	static bool						StoreRealTimeShadowForVehicle(class CVehicle* pVehicle);
	static void						StoreRealTimeShadowForObject(class CObject* pObject);
	static void						StoreRealTimeShadowForBuilding(class CBuilding* pBuilding);
	static void						InitialiseChangedSettings();
	static bool						ThisPropCanHaveShadow(CEntity* pEntity);
	static void						SetRealTimeShadowDistances(CEntity* pEntity);
	static float					GetRealTimeShadowDistances(CEntity* pEntity);
	static void						CalcPedShadowValues(float x, float y, float z, float *sfx, float *sfy, float *ssx, float *ssy, float *sdx, float *sdy);
	static void						Inject();
};

//extern float&	MAX_DISTANCE_REALTIME_SHADOWS, &MAX_DISTANCE_REALTIME_SHADOWS_SQR;
//extern float	MAX_DISTANCE_PED_SHADOWS, MAX_DISTANCE_PED_SHADOWS_SQR;
//extern float	MAX_DISTANCE_CAR_SHADOWS, MAX_DISTANCE_CAR_SHADOWS_SQR;

#endif