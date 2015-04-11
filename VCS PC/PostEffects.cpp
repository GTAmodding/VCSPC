#include "StdAfx.h"
#include "PostEffects.h"

#include "Rs.h"

RwRaster*		CPostEffects::ms_pTrailsRaster1;
RwRaster*		CPostEffects::ms_pTrailsRaster2;
void*			CPostEffects::m_pRadiosityPS;
void*			CPostEffects::m_pPostEffectsVS;
void*			CPostEffects::m_pPostEffectsVertDecl;

IDirect3DVertexBuffer9*	CPostEffects::m_pPostEffectsVertexBuffer;
IDirect3DIndexBuffer9*	CPostEffects::m_pPostEffectsIndexBuffer;
RwUInt32			CPostEffects::m_dwVertexBufferSize, CPostEffects::m_dwVertexBufferStride, CPostEffects::m_dwVertexBufferOffset;


//RwD3D9Vertex	CPostEffects::m_postEffectsVerts[NUM_POSTFX_VERTICES];

void CPostEffects::DoScreenModeDependentInitializations()
{
	// So far we don't need this
}

bool CPostEffects::SetUpBackBuffers()
{
	RwInt32		nWidth = RwRasterGetWidth(RwCameraGetRaster(Camera));
	RwInt32		nHeight = RwRasterGetHeight(RwCameraGetRaster(Camera));
	if ( ms_pTrailsRaster1 && ms_pTrailsRaster2 )
	{
		// Recreate if needed
		if ( RwRasterGetWidth(ms_pTrailsRaster1) != nWidth || RwRasterGetHeight(ms_pTrailsRaster1) != nHeight )
		{
			RwRasterDestroy(ms_pTrailsRaster1);
			RwRasterDestroy(ms_pTrailsRaster2);
		}
		else
		{
			// No need to recreate, just exit
			return false;
		}
	}

	ms_pTrailsRaster1 = RwRasterCreate(nWidth, nHeight, RwRasterGetDepth(RwCameraGetRaster(Camera)), rwRASTERTYPECAMERATEXTURE);
	ms_pTrailsRaster2 = RwRasterCreate(nWidth, nHeight, RwRasterGetDepth(RwCameraGetRaster(Camera)), rwRASTERTYPECAMERATEXTURE);

	return true;
}

void CPostEffects::SetUpShaders()
{
	if ( !m_pRadiosityPS )
		m_pRadiosityPS = RwD3D9CreatePixelShaderFromResource(IDR_RADIOSITYPS);
	if ( !m_pPostEffectsVS )
		m_pPostEffectsVS = RwD3D9CreateVertexShaderFromResource(IDR_POSTFXVS);

	static const D3DVERTEXELEMENT9 vertexElements[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
		{ 0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		//{ 0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
		D3DDECL_END()
	};

	if ( !m_pPostEffectsVertDecl )
		RwD3D9CreateVertexDeclaration(vertexElements, &m_pPostEffectsVertDecl);


	// Set up verts
	// This DOES need to be reinit
	if ( m_pPostEffectsVertexBuffer )
		RwD3D9DestroyVertexBuffer(m_dwVertexBufferStride, m_dwVertexBufferSize, m_pPostEffectsVertexBuffer, m_dwVertexBufferOffset);

	RwD3D9Vertex*	m_postEffectsVerts;
	RwD3D9CreateVertexBuffer(sizeof(m_postEffectsVerts[0]), sizeof(m_postEffectsVerts[0]) * NUM_POSTFX_VERTICES,
		reinterpret_cast<void**>(&m_pPostEffectsVertexBuffer), &m_dwVertexBufferOffset);

	m_dwVertexBufferStride = sizeof(m_postEffectsVerts[0]);
	m_dwVertexBufferSize = m_dwVertexBufferStride * NUM_POSTFX_VERTICES;

	int		nWidth = RsGlobal.MaximumWidth*256/640;
	int		nHeight = RsGlobal.MaximumHeight*128/480;

	float rasterWidth = static_cast<float>(RwRasterGetWidth(ms_pTrailsRaster1));
	float rasterHeight = static_cast<float>(RwRasterGetHeight(ms_pTrailsRaster1));
	float halfU = 0.5f / rasterWidth;
	float halfV = 0.5f / rasterHeight;
	float uMax = nWidth / rasterWidth;
	float vMax = nHeight / rasterHeight;
	float xMax = -1.0f + 2*uMax;
	float yMax = 1.0f - 2*vMax;

	float xOffsetScale = nWidth/256.0f;
	float yOffsetScale = nHeight/128.0f;

	const float uOffsets[] = { -1.0f, 1.0f, 0.0f, 0.0f };
	const float vOffsets[] = { 0.0f, 0.0f, -1.0f, 1.0f };

	// Lock the vertex buffer
	m_pPostEffectsVertexBuffer->Lock(m_dwVertexBufferOffset, sizeof(m_postEffectsVerts[0]) * NUM_POSTFX_VERTICES,
		reinterpret_cast<void**>(&m_postEffectsVerts), 0);

	m_postEffectsVerts[0].x = -1.0f;
	m_postEffectsVerts[0].y = 1.0f;
	m_postEffectsVerts[0].z = 0.0f;
	m_postEffectsVerts[0].rhw = 1.0f;
	m_postEffectsVerts[0].u = 0.0f + halfU;
	m_postEffectsVerts[0].v = 0.0f + halfV;
	m_postEffectsVerts[0].emissiveColor = 0xFFFFFFFF;

	m_postEffectsVerts[1].x = -1.0f;
	m_postEffectsVerts[1].y = yMax;
	m_postEffectsVerts[1].z = 0.0f;
	m_postEffectsVerts[1].rhw = 1.0f;
	m_postEffectsVerts[1].u = 0.0f + halfU;
	m_postEffectsVerts[1].v = vMax + halfV;
	m_postEffectsVerts[1].emissiveColor = 0xFFFFFFFF;

	m_postEffectsVerts[2].x = xMax;
	m_postEffectsVerts[2].y = 1.0f;
	m_postEffectsVerts[2].z = 0.0f;
	m_postEffectsVerts[2].rhw = 1.0f;
	m_postEffectsVerts[2].u = uMax + halfU;
	m_postEffectsVerts[2].v = 0.0f + halfV;
	m_postEffectsVerts[2].emissiveColor = 0xFFFFFFFF;

	m_postEffectsVerts[3].x = xMax;
	m_postEffectsVerts[3].y = yMax;
	m_postEffectsVerts[3].z = 0.0f;
	m_postEffectsVerts[3].rhw = 1.0f;
	m_postEffectsVerts[3].u = uMax + halfU;
	m_postEffectsVerts[3].v = vMax + halfV;
	m_postEffectsVerts[3].emissiveColor = 0xFFFFFFFF;

	m_postEffectsVerts[4].x = -1.0f;
	m_postEffectsVerts[4].y = 1.0f;
	m_postEffectsVerts[4].z = 0.0f;
	m_postEffectsVerts[4].rhw = 1.0f;
	m_postEffectsVerts[4].u = 0.0f + halfU;
	m_postEffectsVerts[4].v = 0.0f + halfV;
	m_postEffectsVerts[4].emissiveColor = 0xFFFFFFFF;

	m_postEffectsVerts[5].x = -1.0f;
	m_postEffectsVerts[5].y = -1.0f;
	m_postEffectsVerts[5].z = 0.0f;
	m_postEffectsVerts[5].rhw = 1.0f;
	m_postEffectsVerts[5].u = 0.0f + halfU;
	m_postEffectsVerts[5].v = vMax + halfV;
	m_postEffectsVerts[5].emissiveColor = 0xFFFFFFFF;

	m_postEffectsVerts[6].x = 1.0f;
	m_postEffectsVerts[6].y = 1.0f;
	m_postEffectsVerts[6].z = 0.0f;
	m_postEffectsVerts[6].rhw = 1.0f;
	m_postEffectsVerts[6].u = uMax + halfU;
	m_postEffectsVerts[6].v = 0.0f + halfV;
	m_postEffectsVerts[6].emissiveColor = 0xFFFFFFFF;

	m_postEffectsVerts[7].x = 1.0f;
	m_postEffectsVerts[7].y = -1.0f;
	m_postEffectsVerts[7].z = 0.0f;
	m_postEffectsVerts[7].rhw = 1.0f;
	m_postEffectsVerts[7].u = uMax + halfU;
	m_postEffectsVerts[7].v = vMax + halfV;
	m_postEffectsVerts[7].emissiveColor = 0xFFFFFFFF;

	RwUInt32 c = 0xFF000000 | 0x010101 * 38;
	for(int i = 8; i < 24; i++){
		int idx = (i-8)/4;
		m_postEffectsVerts[i].x = m_postEffectsVerts[i%4].x;
		m_postEffectsVerts[i].y = m_postEffectsVerts[i%4].y;
		m_postEffectsVerts[i].z = 0.0f;
		m_postEffectsVerts[i].rhw = 1.0f;
		switch(i%4){
		case 0:
			m_postEffectsVerts[i].u = 0.0f + xOffsetScale*uOffsets[idx]/rasterWidth + halfU;
			m_postEffectsVerts[i].v = 0.0f + yOffsetScale*vOffsets[idx]/rasterHeight + halfV;
			break;
		case 1:
			m_postEffectsVerts[i].u = 0.0f + xOffsetScale*uOffsets[idx]/rasterWidth + halfU;
			m_postEffectsVerts[i].v = vMax + yOffsetScale*vOffsets[idx]/rasterHeight + halfV;
			break;
		case 2:
			m_postEffectsVerts[i].u = uMax + xOffsetScale*uOffsets[idx]/rasterWidth + halfU;
			m_postEffectsVerts[i].v = 0.0f + yOffsetScale*vOffsets[idx]/rasterHeight + halfV;
			break;
		case 3:
			m_postEffectsVerts[i].u = uMax + xOffsetScale*uOffsets[idx]/rasterWidth + halfU;
			m_postEffectsVerts[i].v = vMax + yOffsetScale*vOffsets[idx]/rasterHeight + halfV;
			break;
		}
		m_postEffectsVerts[i].emissiveColor = c;
	}

	m_pPostEffectsVertexBuffer->Unlock();

	// Set up indices
	// This DOES NOT need to be reinit
	if ( !m_pPostEffectsIndexBuffer )
	{
		RwD3D9IndexBufferCreate(NUM_POSTFX_INDICES, reinterpret_cast<void**>(&m_pPostEffectsIndexBuffer));

		RwImVertexIndex*	pIndexBuffer;
		static const RwImVertexIndex vcsIndices1[NUM_POSTFX_INDICES] = {
			0, 1, 2, 1, 2, 3,
			4, 5, 2, 5, 2, 3,
			4, 5, 6, 5, 6, 7,
			8, 9, 6, 9, 6, 7,
			8, 9, 10, 9, 10, 11,
			12, 13, 10, 13, 10, 11,
			12, 13, 14, 13, 14, 15,
		};

		m_pPostEffectsIndexBuffer->Lock(0, 0, reinterpret_cast<void**>(&pIndexBuffer), 0);
		memcpy(pIndexBuffer, vcsIndices1, sizeof(vcsIndices1));
		m_pPostEffectsIndexBuffer->Unlock();
	}
}

void CPostEffects::Close()
{
	if ( ms_pTrailsRaster1 )
	{
		RwRasterDestroy(ms_pTrailsRaster1);
		ms_pTrailsRaster1 = nullptr;
	}
	if ( ms_pTrailsRaster2 )
	{
		RwRasterDestroy(ms_pTrailsRaster2);
		ms_pTrailsRaster2 = nullptr;
	}

	RwD3D9DeletePixelShader(m_pRadiosityPS);
	RwD3D9DeleteVertexShader(m_pPostEffectsVS);
	RwD3D9DeleteVertexDeclaration(m_pPostEffectsVertDecl);
	RwD3D9DestroyVertexBuffer(m_dwVertexBufferStride, m_dwVertexBufferSize, m_pPostEffectsVertexBuffer, m_dwVertexBufferOffset);
	m_pPostEffectsIndexBuffer->Release();
}

void CPostEffects::Trails()
{
	RwRaster*	pTempRaster1 = ms_pTrailsRaster1;
	RwRaster*	pTempRaster2 = ms_pTrailsRaster2;

#ifndef NDEBUG
	D3DPERF_BeginEvent(0xFFFFFFFF, L"radiosity");
#endif
	RwRect	vcsRect;
	float	radiosityColors[4];

	vcsRect.x = 0;
	vcsRect.y = 0;
	vcsRect.w = RsGlobal.MaximumWidth*256/640;
	vcsRect.h = RsGlobal.MaximumHeight*128/480;

	// First pass - render downsampled and darkened frame to buffer2

	RwCameraEndUpdate(Camera);
	RwRasterPushContext(pTempRaster1);
	RwRasterRenderScaled(RwCameraGetRaster(Camera), &vcsRect);
	RwRasterPopContext();
	RwD3D9SetRenderTarget(0, pTempRaster2);

	RwTexture		tempTexture;
	tempTexture.raster = pTempRaster1;
	RwD3D9SetTexture(&tempTexture, 0);

	RwD3D9SetVertexDeclaration(m_pPostEffectsVertDecl);

	RwD3D9SetIndices(m_pPostEffectsIndexBuffer);
	RwD3D9SetStreamSource(0, m_pPostEffectsVertexBuffer, m_dwVertexBufferOffset, m_dwVertexBufferStride);

	RwD3D9SetVertexShader(m_pPostEffectsVS);
	RwD3D9SetPixelShader(m_pRadiosityPS);

	radiosityColors[0] = 80.0f/255.0f;
	radiosityColors[1] = 1.0f;
	radiosityColors[2] = 1.0f;
	RwD3D9SetPixelShaderConstant(0, radiosityColors, 1);

	int blend, srcblend, destblend, depthtest;
	RwRenderStateGet(rwRENDERSTATEZTESTENABLE, &depthtest);
	RwRenderStateGet(rwRENDERSTATEVERTEXALPHAENABLE, &blend);
	RwRenderStateGet(rwRENDERSTATESRCBLEND, &srcblend);
	RwRenderStateGet(rwRENDERSTATEDESTBLEND, &destblend);

	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)FALSE);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)0);
	RwD3D9SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERLINEAR);
	RwD3D9DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2);
	//RwD3D9DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 6, 2, vcsIndices1, m_postEffectsVerts, sizeof(m_postEffectsVerts[0]));

	// Second pass - render brightened buffer2 to buffer1 (then swap buffers)
	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	for(int i = 0; i < 4; i++){
		RwD3D9SetRenderTarget(0, pTempRaster1);

		RwD3D9SetPixelShader(m_pRadiosityPS);
		RwD3D9SetTexture(NULL, 0);
		RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)0);
		RwD3D9DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2);
		//RwD3D9DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 6, 2, vcsIndices1, m_postEffectsVerts, sizeof(m_postEffectsVerts[0]));

		RwD3D9SetPixelShader(NULL);
		tempTexture.raster = pTempRaster2;
		RwD3D9SetTexture(&tempTexture, 0);
		RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)1);
		RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDONE);
		RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDONE);
		RwD3D9DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 8, 0, 6*7, 0, 2*7);
		//RwD3D9DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 6*7, 2*7, vcsIndices1, m_postEffectsVerts+8, sizeof(m_postEffectsVerts[0]));
		RwRaster *tmp = pTempRaster1;
		pTempRaster1 = pTempRaster2;
		pTempRaster2 = tmp;
	}

	RwCameraBeginUpdate(Camera);

	tempTexture.raster = pTempRaster2;
	RwD3D9SetTexture(&tempTexture, 0);
	RwD3D9DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 4, 0, 6, 0, 2);
	//RwD3D9DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 6, 2, vcsIndices1, m_postEffectsVerts+4, sizeof(m_postEffectsVerts[0]));

	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)depthtest);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)blend);
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)srcblend);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)destblend);
	RwD3D9SetVertexShader(NULL);

#ifndef NDEBUG
	D3DPERF_EndEvent();
#endif
}

void CPostEffects::Render()
{
#ifndef NDEBUG
	if ( GetAsyncKeyState(VK_F3) & 0x8000 )
		return;
#endif
	Trails();
}

void CPostEffects::Initialise()
{
	if ( SetUpBackBuffers() )	// Returns true if reinit is needed
		SetUpShaders();
}

static StaticPatcher	Patcher([](){ 
						Memory::InjectHook(0x53E227, CPostEffects::Render);
						Memory::InjectHook(0x5BD779, CPostEffects::Initialise);
						Memory::InjectHook(0x7482EB, CPostEffects::Initialise);
						Memory::InjectHook(0x745C7D, CPostEffects::Initialise);
						Memory::InjectHook(0x53BC27, CPostEffects::Close);
						Memory::Nop(0x53C0D5, 5);
									});