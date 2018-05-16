#ifndef __POSTEFFECTS
#define __POSTEFFECTS

#include <d3d9.h>

class CPostEffects
{
private:
	static void			Radiosity(int limit, int intensity);

	static void			BlurOverlay(float intensity, float offset, RwRGBA color);
	static void			Blur_Init(void);
	static void			Blur_Close(void);

public:
	static void			DoScreenModeDependentInitializations();

	static void			Radiosity_Init(void);
	static void			Radiosity_Close(void);

	static void			Render();
	static void			Initialise();
	static void			Update();
	static void			Close();

	static inline void	SetTrailsState(bool bEnable)
		{ m_bTrailsEnabled = bEnable; }
	static inline void	SwitchTrailsOnOff()
		{ m_bTrailsEnabled = m_bTrailsEnabled == false; }
	static inline bool	TrailsEnabled()
		{ return m_bTrailsEnabled; }

	static bool			m_bTrailsEnabled;
private:

	static RwRaster*	ms_pRadiosityRaster1;
	static RwRaster*	ms_pRadiosityRaster2;
	static RwD3D9Vertex	ms_radiosityVerts[44];
	static RwImVertexIndex	ms_radiosityIndices[7*6];

	static RwRaster*	ms_pCurrentFb;
	static RwRaster*	ms_pBlurBuffer;
	static RwD3D9Vertex	ms_blurVerts[24];
	static RwImVertexIndex	ms_blurIndices[3*6];
	static bool		ms_bJustInitialised;

	static void ImmediateModeRenderStatesStore(void);
	static void ImmediateModeRenderStatesSet(void);
	static void ImmediateModeRenderStatesReStore(void);
};

#endif