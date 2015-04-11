#ifndef __POSTEFFECTS
#define __POSTEFFECTS

#include <d3d9.h>

#define NUM_POSTFX_VERTICES		24
#define NUM_POSTFX_INDICES		42

class CPostEffects
{
private:
	static bool			SetUpBackBuffers();
	static void			SetUpShaders();

	static void			Trails();

public:
	static void			DoScreenModeDependentInitializations();

	static void			Render();
	static void			Initialise();
	static void			Close();

private:
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