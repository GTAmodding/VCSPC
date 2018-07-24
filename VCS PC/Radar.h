#ifndef __RADAR
#define __RADAR

#include "General.h"
#include "Sprite.h"

#define NUM_BLIP_SPRITES	64

#define RADAR_POS_X HUD_POS_X + 2.0f
#define RADAR_POS_Y HUD_POS_Y - 4.0f

struct tRadarTrace {
    unsigned int   m_dwColour; // see eBlipColour
    unsigned int   m_dwEntityHandle;
    CVector        m_vPosition;
    unsigned short m_nCounter;
    float          m_fSphereRadius;
    unsigned short m_nBlipSize;
    class CEntryExit *m_pEntryExit;
    unsigned char  m_nBlipSprite; // see eRadarSprite
    unsigned char  m_bBright : 1; // It makes use of bright colors. Always set.
    unsigned char  m_bTrackingBlip : 1; // It is available.
    unsigned char  m_bShortRange : 1; // It doesn't show permanently on the radar.
    unsigned char  m_bFriendly : 1; // It is affected by BLIP_COLOUR_THREAT.   
    unsigned char  m_bBlipRemain : 1; // It has the priority over the entity (it will still appear after the entity's deletion).
    unsigned char  m_bBlipFade : 1; // Possibly a leftover. Always unset (unused).
    unsigned char  m_nCoordBlipAppearance : 2; // see eBlipAppearance
    unsigned char  m_nBlipDisplayFlag : 2; // see eBlipDisplay
    unsigned char  m_nBlipType : 4; // see eBlipType
};

enum eBlipType {
    BLIP_NONE,          // 0
    BLIP_CAR,           // 1
    BLIP_CHAR,          // 2
    BLIP_OBJECT,        // 3
    BLIP_COORD,         // 4 - Checkpoint.
    BLIP_CONTACTPOINT,  // 5 - Sphere.
    BLIP_SPOTLIGHT,     // 6
    BLIP_PICKUP,        // 7
    BLIP_AIRSTRIP       // 8
};

enum eBlipDisplay {
    BLIP_DISPLAY_NEITHER,    // 0
    BLIP_DISPLAY_MARKERONLY, // 1
    BLIP_DISPLAY_BLIPONLY,   // 2
    BLIP_DISPLAY_BOTH        // 3
};

class CRadar
{
public:
    // 2990.0 by default
    static float &m_radarRange;
    // static uint16_t MapLegendList[175];
    static unsigned short *MapLegendList;
    // num icons in legend
    static unsigned short &MapLegendCounter;
    // static CRGBA ArrowBlipColours[6];
    static CRGBA *ArrowBlipColour;
    // static tRadarTrace ms_RadarTrace[175];
    static tRadarTrace *ms_RadarTrace;
    // static CVector2D vec2DRadarOrigin;
    static CVector2D &vec2DRadarOrigin;
    // static CSprite2d RadarBlipSprites[64];
    static CSprite2d *RadarBlipSprites;
    // 
    static CRect &m_radarRect;
    // current airstrip index in airstrip_table
    static unsigned char &airstrip_location;
    // blip handle
    static int &airstrip_blip;

public:
	static void				Initialise(void);
	static void				ChangeBlipBrightness(int nBlipID, int nBrightness);
	static void				DrawRadarSection(int nX, int nY);
    static void             InitFrontEndMap();
    static void             DrawLegend(int x, int y, int blipType);

    static void             DrawRadarSectionMap(int x, int y, CRect rect);
    static void             LimitToMap(float* pX, float* pY);
    static bool             DisplayThisBlip(int spriteId, char priority);
    static void             AddBlipToLegendList(unsigned char arg0, int blipArrId);
    static void             TransformRealWorldPointToRadarSpace(CVector2D& out, CVector2D const& in);
    static float            LimitRadarPoint(CVector2D& point);
    static void             DrawBlips();
    static void             ClearBlip(int blipIndex);
    static void             SetBlipSprite(int blipIndex, int spriteId);
    static int              SetCoordBlip(eBlipType type, float x, float y, float z, int a5, eBlipDisplay display);

	static uint32			GetRadarTraceColour(int colour, bool bDark, bool bFriend);
	static void				LoadTextures();

	// 3D radar test
	static void				Draw3DRadar(int nX, int nY);
	static void				TransformRadarPointToScreenSpace(CVector2D& vecOut, const CVector2D& vecIn);
	static void				Set3DVerts(int nVerts, float* pX, float* pY, const CRGBA& rgb);
	static void				Render3D(void*, void*, int nVerts);
	static void				DrawRotatingRadarSprite(CSprite2d * texture, float x, float y, float r_angle, unsigned int width, unsigned int height, CRGBA const & color);
	static void				DrawRadarCentre(CSprite2d * sprite, float x, float y, float angle, unsigned int width, unsigned int height, CRGBA color);
	static void             DrawRadarSprites(BYTE iconID, float x, float y, unsigned __int8 alpha);
    static void             ShowRadarTraceWithHeight(float x, float y, unsigned int size, unsigned __int8 r, unsigned __int8 g, unsigned __int8 b, unsigned __int8 a, unsigned __int8 type_or_height);
    static void __fastcall  DrawRadarCircle(CSprite2d *sprite, int, CRect *rect, CRGBA *color);
	static void				TransformRadarPointToScreenSpaceVCS(CVector2D * out, CVector2D * in);
};

#endif