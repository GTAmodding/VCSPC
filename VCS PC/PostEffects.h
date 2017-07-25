#ifndef __POSTEFFECTS
#define __POSTEFFECTS

#include <d3d9.h>

#define NUM_POSTFX_VERTICES		40
#define NUM_POSTFX_INDICES		42

class CPostEffects
{
private:
	static void			Render_Trails();

public:
	static void			DoScreenModeDependentInitializations();

	static void			Render();
	static void			Initialise();
	static void			Close();

	static void			Init_Trails();
	static void			Close_Trails();

	static inline void	SetTrailsState(bool bEnable)
		{ m_bTrailsEnabled = bEnable; }
	static inline void	SwitchTrailsOnOff()
		{ m_bTrailsEnabled = m_bTrailsEnabled == false; }
	static inline bool	TrailsEnabled()
		{ return m_bTrailsEnabled; }
	static bool IsVisionFXActive(void);
	static bool &m_bNightVision;
	static bool &m_bInfraredVision;

private:
	static bool			m_bTrailsEnabled;

	static bool			m_bTrailsInitialised;
	static RwRaster*	ms_pTrailsRaster1;
	static RwRaster*	ms_pTrailsRaster2;

	static void*					m_pRadiosityPS;
	static void*					m_pPostEffectsVS;
	static void*					m_pPostEffectsVertDecl;
	static IDirect3DVertexBuffer9*	m_pPostEffectsVertexBuffer;
	static IDirect3DIndexBuffer9*	m_pPostEffectsIndexBuffer;
	static RwUInt32					m_dwVertexBufferSize, m_dwVertexBufferStride, m_dwVertexBufferOffset;


	//static RwD3D9Vertex	m_postEffectsVerts[NUM_POSTFX_VERTICES];
};

#endif