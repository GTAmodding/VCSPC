#include "StdAfx.h"
#include "PostEffects.h"

WRAPPER void CPostEffects::DoScreenModeDependentInitializations() { EAXJMP(0x7046D0); }

struct QuadVertex
{
	RwReal      x, y, z;
	RwReal      rhw;
	RwUInt32    emissiveColor;
	RwReal      u, v;
	RwReal      u1, v1;
};

RwRaster *target1, *target2;
RwRect smallRect;
void *quadVertexDecl = NULL;
void *postfxVS, *colorFilterPS, *radiosityPS;
static RwImVertexIndex* quadIndices = (RwImVertexIndex*)0x8D5174;

static QuadVertex vcsVertices[24];
RwRect vcsRect;
RwImVertexIndex vcsIndices1[] = {
	0, 1, 2, 1, 2, 3,
		4, 5, 2, 5, 2, 3,
	4, 5, 6, 5, 6, 7,
		8, 9, 6, 9, 6, 7,
	8, 9, 10, 9, 10, 11,
		12, 13, 10, 13, 10, 11,
	12, 13, 14, 13, 14, 15,
};

static QuadVertex quadVertices[4];

void Radiosity(int col1, int nSubdivs, int unknown, int col2)
{
	float radiosityColors[4];
	RwTexture tempTexture;
	RwRaster *vcsBuffer1, *vcsBuffer2;
	int width, height;
	RwRaster *camraster;

	float uOffsets[] = { -1.0f, 1.0f, 0.0f, 0.0f };
	float vOffsets[] = { 0.0f, 0.0f, -1.0f, 1.0f };

	D3DPERF_BeginEvent(0xFFFFFFFF, L"radiosity");

	if ( target1 == nullptr )
	{
		HMODULE thisModule;
		GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)Radiosity, &thisModule);

		HRSRC		resource = FindResource(thisModule, MAKEINTRESOURCE(IDR_RADIOSITYPS), RT_RCDATA);
		RwUInt32*	shader = static_cast<RwUInt32*>(LoadResource(thisModule, resource));
		RwD3D9CreatePixelShader(shader, reinterpret_cast<void**>(&radiosityPS));
		FreeResource(shader);

		resource = FindResource(thisModule, MAKEINTRESOURCE(IDR_POSTFXVS), RT_RCDATA);
		shader = static_cast<RwUInt32*>(LoadResource(thisModule, resource));
		RwD3D9CreateVertexShader(shader, reinterpret_cast<void**>(&postfxVS));
		FreeResource(shader);

		static const D3DVERTEXELEMENT9 vertexElements[] =
		{
			{ 0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			{ 0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
			{ 0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			{ 0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
			D3DDECL_END()
		};

		RwD3D9CreateVertexDeclaration(vertexElements, &quadVertexDecl);


		target1 = RwRasterCreate(2048, 2048, 32, 5);
		target2 = RwRasterCreate(2048, 2048, 32, 5);
	}

	width = vcsRect.w = RsGlobal.MaximumWidth*256/640;
	height = vcsRect.h = RsGlobal.MaximumHeight*128/480;

	vcsBuffer1 = target1;
	vcsBuffer2 = target2;

	float rasterWidth = RwRasterGetWidth(target1);
	float rasterHeight = RwRasterGetHeight(target1);
	float halfU = 0.5 / rasterWidth;
	float halfV = 0.5 / rasterHeight;
	float uMax = width / rasterWidth;
	float vMax = height / rasterHeight;
	float xMax = -1.0f + 2*uMax;
	float yMax = 1.0f - 2*vMax;
	float xOffsetScale = width/256.0f;
	float yOffsetScale = height/128.0f;
	//if(config->radiosityOffset == 0.0f)
	//	xOffsetScale = yOffsetScale = 0.0f;

	vcsVertices[0].x = -1.0f;
	vcsVertices[0].y = 1.0f;
	vcsVertices[0].z = 0.0f;
	vcsVertices[0].rhw = 1.0f;
	vcsVertices[0].u = 0.0f + halfU;
	vcsVertices[0].v = 0.0f + halfV;
	vcsVertices[0].emissiveColor = 0xFFFFFFFF;

	vcsVertices[1].x = -1.0f;
	vcsVertices[1].y = yMax;
	vcsVertices[1].z = 0.0f;
	vcsVertices[1].rhw = 1.0f;
	vcsVertices[1].u = 0.0f + halfU;
	vcsVertices[1].v = vMax + halfV;
	vcsVertices[1].emissiveColor = 0xFFFFFFFF;

	vcsVertices[2].x = xMax;
	vcsVertices[2].y = 1.0f;
	vcsVertices[2].z = 0.0f;
	vcsVertices[2].rhw = 1.0f;
	vcsVertices[2].u = uMax + halfU;
	vcsVertices[2].v = 0.0f + halfV;
	vcsVertices[2].emissiveColor = 0xFFFFFFFF;

	vcsVertices[3].x = xMax;
	vcsVertices[3].y = yMax;
	vcsVertices[3].z = 0.0f;
	vcsVertices[3].rhw = 1.0f;
	vcsVertices[3].u = uMax + halfU;
	vcsVertices[3].v = vMax + halfV;
	vcsVertices[3].emissiveColor = 0xFFFFFFFF;

	vcsVertices[4].x = -1.0f;
	vcsVertices[4].y = 1.0f;
	vcsVertices[4].z = 0.0f;
	vcsVertices[4].rhw = 1.0f;
	vcsVertices[4].u = 0.0f + halfU;
	vcsVertices[4].v = 0.0f + halfV;
	vcsVertices[4].emissiveColor = 0xFFFFFFFF;

	vcsVertices[5].x = -1.0f;
	vcsVertices[5].y = -1.0f;
	vcsVertices[5].z = 0.0f;
	vcsVertices[5].rhw = 1.0f;
	vcsVertices[5].u = 0.0f + halfU;
	vcsVertices[5].v = vMax + halfV;
	vcsVertices[5].emissiveColor = 0xFFFFFFFF;

	vcsVertices[6].x = 1.0f;
	vcsVertices[6].y = 1.0f;
	vcsVertices[6].z = 0.0f;
	vcsVertices[6].rhw = 1.0f;
	vcsVertices[6].u = uMax + halfU;
	vcsVertices[6].v = 0.0f + halfV;
	vcsVertices[6].emissiveColor = 0xFFFFFFFF;

	vcsVertices[7].x = 1.0f;
	vcsVertices[7].y = -1.0f;
	vcsVertices[7].z = 0.0f;
	vcsVertices[7].rhw = 1.0f;
	vcsVertices[7].u = uMax + halfU;
	vcsVertices[7].v = vMax + halfV;
	vcsVertices[7].emissiveColor = 0xFFFFFFFF;

	RwUInt32 c = 0xFF000000 | 0x010101 * 38;
	for(int i = 8; i < 24; i++){
		int idx = (i-8)/4;
		vcsVertices[i].x = vcsVertices[i%4].x;
		vcsVertices[i].y = vcsVertices[i%4].y;
		vcsVertices[i].z = 0.0f;
		vcsVertices[i].rhw = 1.0f;
		switch(i%4){
		case 0:
			vcsVertices[i].u = 0.0f + xOffsetScale*uOffsets[idx]/rasterWidth + halfU;
			vcsVertices[i].v = 0.0f + yOffsetScale*vOffsets[idx]/rasterHeight + halfV;
			break;
		case 1:
			vcsVertices[i].u = 0.0f + xOffsetScale*uOffsets[idx]/rasterWidth + halfU;
			vcsVertices[i].v = vMax + yOffsetScale*vOffsets[idx]/rasterHeight + halfV;
			break;
		case 2:
			vcsVertices[i].u = uMax + xOffsetScale*uOffsets[idx]/rasterWidth + halfU;
			vcsVertices[i].v = 0.0f + yOffsetScale*vOffsets[idx]/rasterHeight + halfV;
			break;
		case 3:
			vcsVertices[i].u = uMax + xOffsetScale*uOffsets[idx]/rasterWidth + halfU;
			vcsVertices[i].v = vMax + yOffsetScale*vOffsets[idx]/rasterHeight + halfV;
			break;
		}
		vcsVertices[i].emissiveColor = c;
	}

	// First pass - render downsampled and darkened frame to buffer2

	RwCameraEndUpdate(Camera);
	RwRasterPushContext(vcsBuffer1);
	camraster = RwCameraGetRaster(Camera);
	RwRasterRenderScaled(camraster, &vcsRect);
	RwRasterPopContext();
	RwCameraSetRaster(Camera, vcsBuffer2);
	RwCameraBeginUpdate(Camera);

	tempTexture.raster = vcsBuffer1;
	RwD3D9SetTexture(&tempTexture, 0);

	RwD3D9SetVertexDeclaration(quadVertexDecl);

	RwD3D9SetVertexShader(postfxVS);
	RwD3D9SetPixelShader(radiosityPS);

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
	RwD3D9DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 6, 2, vcsIndices1, vcsVertices, sizeof(QuadVertex));

	// Second pass - render brightened buffer2 to buffer1 (then swap buffers)
	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	for(int i = 0; i < 4; i++){
		RwCameraEndUpdate(Camera);
		RwCameraSetRaster(Camera, vcsBuffer1);
		RwCameraBeginUpdate(Camera);

		RwD3D9SetPixelShader(radiosityPS);
		RwD3D9SetTexture(NULL, 0);
		RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)0);
		RwD3D9DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 6, 2, vcsIndices1, vcsVertices, sizeof(QuadVertex));

		RwD3D9SetPixelShader(NULL);
		tempTexture.raster = vcsBuffer2;
		RwD3D9SetTexture(&tempTexture, 0);
		RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)1);
		RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDONE);
		RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDONE);
		RwD3D9DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 6*7, 2*7, vcsIndices1, vcsVertices+8, sizeof(QuadVertex));
		RwRaster *tmp = vcsBuffer1;
		vcsBuffer1 = vcsBuffer2;
		vcsBuffer2 = tmp;
	}

	RwCameraEndUpdate(Camera);
	RwCameraSetRaster(Camera, camraster);
	RwCameraBeginUpdate(Camera);

	tempTexture.raster = vcsBuffer2;
	RwD3D9SetTexture(&tempTexture, 0);
	RwD3D9DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 6, 2, vcsIndices1, vcsVertices+4, sizeof(QuadVertex));

	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)depthtest);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)blend);
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)srcblend);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)destblend);
	RwD3D9SetVertexShader(NULL);

	D3DPERF_EndEvent();
}

void CPostEffects::Render()
{
	if ( GetAsyncKeyState(VK_F3) & 0x8000 )
		return;
	Radiosity(200, 0, 0, 200);
}

static StaticPatcher	Patcher([](){ 
						Memory::InjectHook(0x53E227, CPostEffects::Render); 
									});