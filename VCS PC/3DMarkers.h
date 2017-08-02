#ifndef __3DMARKERS
#define __3DMARKERS

#include "General.h"

// VCS colors
#define MARKER_SET_COLOR_R	0xED
#define MARKER_SET_COLOR_G	0x82
#define MARKER_SET_COLOR_B	0xB4

// VC colors
/*#define MARKER_SET_COLOR_R	0xFC
#define MARKER_SET_COLOR_G	0x8A
#define MARKER_SET_COLOR_B	0xF2*/
#define MARKER_SET_COLOR_A	0xE4

enum MarkerType
{
	MARKER_DIAMOND_0,
	MARKER_CYLINDER,
	MARKER_UNUSED_2,
	MARKER_UNUSED_3,
	MARKER_HOOP,
	MARKER_ARROW,
	MARKER_DIAMOND_6
};

class C3dMarker
{
public:
	CMatrix	m_mat;  // local space to world space transform // 0
	RpAtomic	*m_pAtomic; // 72
	RpMaterial	*m_pMaterial; // 76
    
	uint16	m_nType; // 80
	bool	m_bIsUsed;  // has this marker been allocated this frame?    // 82
	uint8	m_bIsUsed2;
	uint32	m_nIdentifier; // 84

	RwRGBA	m_color; // 88 
	uint16	m_nPulsePeriod; // 92
	int16	m_nRotateRate;  // deg per frame (in either direction) // 94
	uint32	m_nStartTime; // 96
	float	m_fPulseFraction;  // 100
	float	m_fStdSize; // 104
	float	m_fSize; // 108
	float	m_fBrightness; // 112
	float	m_fCameraRange; // 116

	CVector	m_normal;           // Normal of the object point at             // 120
	// the following variables remember the last time we read the heigh of the
	// map. Using this we don't have to do this every frame and we can still have moving markers.
	uint16	m_LastMapReadX, m_LastMapReadY; // 132 / 134
	float	m_LastMapReadResultZ; // 136
	float	m_roofHeight; // 140
	CVector	m_lastPosition; // 144
	uint32	m_OnScreenTestTime;     // time last screen check was done // 156

public:
	long double		CalculateRealSize();
	void			Render(void);
};

class C3dMarkers
{
private:
	static float				m_PosZMult;
	static const float			m_MovingMultiplier;
	static C3dMarker			*m_aMarkerArray;	// [32]
	static int &NumActiveMarkers;
	static float &m_angleDiamond;
	static bool &IgnoreRenderLimit;

private:
	static C3dMarker*			PlaceMarker(unsigned int nIndex, unsigned short markerID, CVector& vecPos, float fSize, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, unsigned short pulsePeriod, float pulseFraction, short rotateRate, float normalX, float normalY, float normalZ, bool checkZ);
	static void				User3dMarkersDraw(void);
	static void				DirectionArrowsDraw(void);
	static RpClump				*LoadMarker(const char *name);

public:
	static RpClump				**m_pRpClumpArray;	// [7]
	static inline float*		GetPosZMult()
			{ return &m_PosZMult; };
	static inline const float*	GetMovingMult()
			{ return &m_MovingMultiplier; };

	static void					Init(void);
	static void					Render(void);
	// Last unused param removed
	static void					PlaceMarkerSet(unsigned int nIndex, unsigned short markerID, CVector& vecPos, float fSize, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, unsigned short pulsePeriod, float pulseFraction);
};

static_assert(sizeof(C3dMarker) == 0xA0, "Wrong size: C3DMarker");

#endif