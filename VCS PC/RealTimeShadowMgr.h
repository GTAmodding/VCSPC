#ifndef __REALTIMESHADOWMGR
#define __REALTIMESHADOWMGR

#define NUM_MAX_REALTIME_SHADOWS		64

struct tShadowQualitySettings
{
	unsigned char		nQuality, nQualityAfterResample;
	unsigned char		nBlurPasses;
	bool				bUseGradient, bDedicatedCamsForPlayer;
};

class CShadowCamera
{
public:
	RwCamera*		m_pCamera;
	RwTexture*		m_pTexture;

public:
	CShadowCamera()
		: m_pCamera(nullptr), m_pTexture(nullptr)
	{}

	RwCamera*		Create(int nSize);
	void			Destroy();
	void			SetLight(RpLight* pLight);
	void			MakeGradientRaster();
	RwCamera*		SetCenter(RwV3d* pVector);
	RwRaster*		RasterResample(RwRaster* pRaster);
	RwRaster*		RasterBlur(RwRaster* pRaster, int nPasses);
	RwRaster*		RasterGradient(RwRaster* pRaster);
	void			InvertRaster();

	RwCamera*		Update(RpAtomic* pAtomic);
	RwCamera*		Update(RpClump* pClump, CPhysical* pEntity);
	void			ReInit();
};

class CRealTimeShadow
{
private:
	CPhysical*		m_pEntity;
	bool			m_bRenderedThisFrame;
	unsigned char	m_nIntensity;
	bool			m_bUsePlayerHelperCams;		// VCS PC class extension
	CShadowCamera	m_Camera;
	bool			m_bDrawResample;
	CShadowCamera	m_ResampledCamera;
	int				m_dwBlurPasses;
	bool			m_bDrawGradient;
	signed int		m_nRwObjectType;
	RpLight*		m_pLight;
	RwSphere		m_BoundingSphere;
	RwSphere		m_BaseSphere;

public:
	inline class CPhysical*		GetOwner()
		{ return m_pEntity; }
	inline void					SetRenderedThisFrame()
		{ m_bRenderedThisFrame = true; }
	inline void					ResetIntensity()
		{ m_nIntensity = 0; }
	inline void					ForceFullIntensity()
		{ m_nIntensity = 100; }
	inline void					ClearOwner()
		{ m_pEntity = nullptr; }
	//void*			operator new(size_t size)
	//	{ return GtaOperatorNew(size); }

	CRealTimeShadow()
		:	m_pEntity(nullptr), m_nRwObjectType(-1), m_pLight(nullptr), m_dwBlurPasses(0), 
			m_bDrawResample(false), m_bDrawGradient(false), m_bRenderedThisFrame(false), m_nIntensity(0),
			m_bUsePlayerHelperCams(false)
	{}

	~CRealTimeShadow()
	{
		Destroy();
	}

	void			ReInit()
	{
		m_Camera.ReInit();
		if ( m_bDrawResample ) m_ResampledCamera.ReInit();
	}

	void			Destroy();

	void			Create(int nSize, int nSizeResampled, bool bResample, int nBlurPasses, bool bGradient, bool bUsePlayerCams);
	RwTexture*		Update();
	bool			SetShadowedObject(CPhysical* pObject);
};


class CRealTimeShadowManager
{
public:
	bool					m_bInitialised;
	bool					m_bNeedsReinit;
	bool					m_bPlayerHelperCamsInUse;	// VCS PC class extension
	CRealTimeShadow*		m_pShadows[NUM_MAX_REALTIME_SHADOWS];
	CShadowCamera			m_BlurCamera;
	CShadowCamera			m_GradientCamera;

	// VCS PC class extension
	CShadowCamera			m_BlurCamera_Player;
	CShadowCamera			m_GradientCamera_Player;

public:
	CRealTimeShadowManager()
		: m_bInitialised(false), m_bNeedsReinit(false), m_bPlayerHelperCamsInUse(false)
	{
		memset(m_pShadows, 0, sizeof(m_pShadows));
	}

	void					ResetForChangedSettings()
	{	for ( int i = 0; i < NUM_MAX_REALTIME_SHADOWS; i++ ) ReturnRealTimeShadow(m_pShadows[i]);
		Exit(); Init(); }

	void					ReturnRealTimeShadow(CRealTimeShadow* pShadow);
	void					Init();
	void					Exit();
	void					DoShadowThisFrame(CPhysical* pEntity);
	void					GetRealTimeShadow(CPhysical* pEntity);	
	void					ReInit();
};

RpAtomic* ShadowCameraRenderCB(RpAtomic* pAtomic, void* pData);

extern CRealTimeShadowManager	g_realTimeShadowMan;

static_assert(sizeof(CRealTimeShadow) == 0x4C, "Wrong size: CRealTimeShadow");
//static_assert(sizeof(CRealTimeShadowManager) == 0x54, "Wrong size: CRealTimeShadowManager");

#endif