#include "StdAfx.h"
#include "PostEffects.h"

#include "Rs.h"
#include "TimeCycle.h"
#include "Scene.h"
#include "debugmenu_public.h"

bool CPostEffects::m_bTrailsEnabled;
static int blurEnabled = true;

WRAPPER void CPostEffects::ImmediateModeRenderStatesStore(void) { EAXJMP(0x700CC0); }
WRAPPER void CPostEffects::ImmediateModeRenderStatesSet(void) { EAXJMP(0x700D70); }
WRAPPER void CPostEffects::ImmediateModeRenderStatesReStore(void) { EAXJMP(0x700E00); }

RwRaster*		CPostEffects::ms_pCurrentFb;
RwRaster*		CPostEffects::ms_pBlurBuffer;
RwD3D9Vertex CPostEffects::ms_blurVerts[24];
RwImVertexIndex CPostEffects::ms_blurIndices[3*6] = {
	0, 1, 2, 2, 1, 3,
	4, 5, 6, 6, 5, 7,
	8, 9, 10, 10, 9, 11,
};
bool			CPostEffects::ms_bJustInitialised;

RwRaster*		CPostEffects::ms_pRadiosityRaster1;
RwRaster*		CPostEffects::ms_pRadiosityRaster2;
RwD3D9Vertex CPostEffects::ms_radiosityVerts[44];
RwImVertexIndex CPostEffects::ms_radiosityIndices[7*6] = {
	0, 1, 2, 2, 1, 3,
		4, 5, 2, 2, 5, 3,
	4, 5, 6, 6, 5, 7,
		8, 9, 6, 6, 9, 7,
	8, 9, 10, 10, 9, 11,
		12, 13, 10, 10, 13, 11,
	12, 13, 14, 14, 13, 15,
};

void CPostEffects::DoScreenModeDependentInitializations()
{
	// So far we don't need this
}

void CPostEffects::Close()
{
	Radiosity_Close();
}

void
makequad(RwD3D9Vertex *v, int width, int height, int texwidth = 0, int texheight = 0)
{
	float w, h, tw, th;
	w = width;
	h = height;
	tw = texwidth > 0 ? texwidth : w;
	th = texheight > 0 ? texheight : h;
	v[0].x = 0;
	v[0].y = 0;
	v[0].z = 0.0f;
	v[0].rhw = 1.0f;
	v[0].u = 0.5f / tw;
	v[0].v = 0.5f / th;
	v[0].emissiveColor = 0xFFFFFFFF;
	v[1].x = 0;
	v[1].y = h;
	v[1].z = 0.0f;
	v[1].rhw = 1.0f;
	v[1].u = 0.5f / tw;
	v[1].v = (h + 0.5f) / th;
	v[1].emissiveColor = 0xFFFFFFFF;
	v[2].x = w;
	v[2].y = 0;
	v[2].z = 0.0f;
	v[2].rhw = 1.0f;
	v[2].u = (w + 0.5f) / tw;
	v[2].v = 0.5f / th;
	v[2].emissiveColor = 0xFFFFFFFF;
	v[3].x = w;
	v[3].y = h;
	v[3].z = 0.0f;
	v[3].rhw = 1.0f;
	v[3].u = (w + 0.5f) / tw;
	v[3].v = (h + 0.5f) / th;
	v[3].emissiveColor = 0xFFFFFFFF;
}

void
CPostEffects::Radiosity_Close(void)
{
	if(ms_pRadiosityRaster1){
		RwRasterDestroy(ms_pRadiosityRaster1);
		ms_pRadiosityRaster1 = nullptr;
	}
	if(ms_pRadiosityRaster2){
		RwRasterDestroy(ms_pRadiosityRaster2);
		ms_pRadiosityRaster2 = nullptr;
	}
}

void
CPostEffects::Radiosity_Init(void)
{
	static float uOffsets[] = { -1.0f, 1.0f, 0.0f, 0.0f,   -1.0f, 1.0f, -1.0f, 1.0f };
	static float vOffsets[] = { 0.0f, 0.0f, -1.0f, 1.0f,   -1.0f, -1.0f, 1.0f, 1.0f };
	int i;
	RwUInt32 c;
	float w, h;
	assert(ms_pRadiosityRaster1 == nullptr);
	assert(ms_pRadiosityRaster2 == nullptr);
	ms_pRadiosityRaster1 = RwRasterCreate(256, 128, RwCameraGetRaster(Scene.camera)->depth, rwRASTERTYPECAMERATEXTURE);
	ms_pRadiosityRaster2 = RwRasterCreate(256, 128, RwCameraGetRaster(Scene.camera)->depth, rwRASTERTYPECAMERATEXTURE);

	w = 256;
	h = 128;

	// TODO: tex coords correct?
	makequad(ms_radiosityVerts, 256, 128);
	makequad(ms_radiosityVerts+4, RwCameraGetRaster(Scene.camera)->width, RwCameraGetRaster(Scene.camera)->height);

	// black vertices; at 8
	for(i = 0; i < 4; i++){
		ms_radiosityVerts[i+8] = ms_radiosityVerts[i];
		ms_radiosityVerts[i+8].emissiveColor = 0;
	}

	// two sets blur vertices; at 12
	c = D3DCOLOR_ARGB(0xFF, 36, 36, 36);
	for(i = 0; i < 2*4*4; i++){
		ms_radiosityVerts[i+12] = ms_radiosityVerts[i%4];
		ms_radiosityVerts[i+12].emissiveColor = c;
		switch(i%4){
		case 0:
			ms_radiosityVerts[i+12].u = (uOffsets[i/4] + 0.5f) / w;
			ms_radiosityVerts[i+12].v = (vOffsets[i/4] + 0.5f) / h;
			break;
		case 1:
			ms_radiosityVerts[i+12].u = (uOffsets[i/4] + 0.5f) / w;
			ms_radiosityVerts[i+12].v = (h + vOffsets[i/4] + 0.5f) / h;
			break;
		case 2:
			ms_radiosityVerts[i+12].u = (w + uOffsets[i/4] + 0.5f) / w;
			ms_radiosityVerts[i+12].v = (vOffsets[i/4] + 0.5f) / h;
			break;
		case 3:
			ms_radiosityVerts[i+12].u = (w + uOffsets[i/4] + 0.5f) / w;
			ms_radiosityVerts[i+12].v = (h + vOffsets[i/4] + 0.5f) / h;
			break;
		}
	}
}

void
CPostEffects::Radiosity(int limit, int intensity)
{
	int i;
	RwRaster *fb;
	RwRaster *fb1, *fb2, *tmp;

#ifndef NDEBUG
	if ( GetAsyncKeyState(VK_F4) & 0x8000 )
		return;
#endif

	fb = RwCameraGetRaster(Scene.camera);
	assert(ms_pRadiosityRaster1);
	assert(ms_pRadiosityRaster1);

	RwRect r;
	r.x = 0;
	r.y = 0;
	r.w = 256;
	r.h = 128;

	CPostEffects::ImmediateModeRenderStatesStore();
	CPostEffects::ImmediateModeRenderStatesSet();
	RwD3D9SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERLINEAR);
	RwD3D9SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	RwCameraEndUpdate(Scene.camera);

	RwRasterPushContext(ms_pRadiosityRaster2);
	RwRasterRenderScaled(fb, &r);
	RwRasterPopContext();

	RwCameraSetRaster(Scene.camera, ms_pRadiosityRaster2);
	RwCameraBeginUpdate(Scene.camera);

	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
	RwD3D9SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	RwD3D9SetRenderState(D3DRS_SRCBLEND, D3DBLEND_BLENDFACTOR);
	RwD3D9SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	RwD3D9SetRenderState(D3DRS_BLENDFACTOR, D3DCOLOR_ARGB(0xFF, limit/2, limit/2, limit/2));
	RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRILIST, ms_radiosityVerts, 4, ms_radiosityIndices, 6);

	fb1 = ms_pRadiosityRaster1;
	fb2 = ms_pRadiosityRaster2;
	for(i = 0; i < 4; i++){
		RwD3D9SetRenderTarget(0, fb1);

		RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
		RwD3D9SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRILIST, ms_radiosityVerts+8, 4, ms_radiosityIndices, 6);

		RwRenderStateSet(rwRENDERSTATETEXTURERASTER, fb2);
		RwRenderStateSet(rwRENDERSTATETEXTUREADDRESSU, (void*)rwTEXTUREADDRESSCLAMP);
		RwRenderStateSet(rwRENDERSTATETEXTUREADDRESSV, (void*)rwTEXTUREADDRESSCLAMP);
		RwD3D9SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		RwD3D9SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		RwD3D9SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		RwD3D9SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		if((i % 2) == 0)
			RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRILIST, ms_radiosityVerts+12, 4*4, ms_radiosityIndices, 6*7);
		else
			RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRILIST, ms_radiosityVerts+28, 4*4, ms_radiosityIndices, 6*7);

		tmp = fb1;
		fb1 = fb2;
		fb2 = tmp;
	}

	RwCameraEndUpdate(Scene.camera);
	RwCameraSetRaster(Scene.camera, fb);
	RwCameraBeginUpdate(Scene.camera);

	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, fb2);
	RwRenderStateSet(rwRENDERSTATETEXTUREADDRESSU, (void*)rwTEXTUREADDRESSCLAMP);
	RwRenderStateSet(rwRENDERSTATETEXTUREADDRESSV, (void*)rwTEXTUREADDRESSCLAMP);
	RwD3D9SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	RwD3D9SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	RwD3D9SetRenderState(D3DRS_SRCBLEND, D3DBLEND_BLENDFACTOR);
	RwD3D9SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	RwD3D9SetRenderState(D3DRS_BLENDFACTOR, D3DCOLOR_ARGB(0xFF, intensity*4, intensity*4, intensity*4));
	RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRILIST, ms_radiosityVerts+4, 4, ms_radiosityIndices, 6);
	RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRILIST, ms_radiosityVerts+4, 4, ms_radiosityIndices, 6);

	RwD3D9SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	CPostEffects::ImmediateModeRenderStatesReStore();
}

void
CPostEffects::Blur_Init(void)
{
	RwRaster *camras;
	int width, height;
	assert(ms_pCurrentFb == nullptr);
	assert(ms_pBlurBuffer == nullptr);

	camras = RwCameraGetRaster(Scene.camera);
	for(width = 1; width < camras->width; width *= 2);
	for(height = 1; height < camras->height; height *= 2);
	ms_pCurrentFb = RwRasterCreate(width, height, camras->depth, rwRASTERTYPECAMERATEXTURE);
	ms_pBlurBuffer = RwRasterCreate(width, height, camras->depth, rwRASTERTYPECAMERATEXTURE);
	ms_bJustInitialised = true;
}

void
CPostEffects::Blur_Close(void)
{
	if(ms_pCurrentFb){
		RwRasterDestroy(ms_pCurrentFb);
		ms_pCurrentFb = nullptr;
	}
	if(ms_pBlurBuffer){
		RwRasterDestroy(ms_pBlurBuffer);
		ms_pBlurBuffer = nullptr;
	}
}

void
CPostEffects::BlurOverlay(float intensityf, float offset, RwRGBA color)
{
	int i;
	int bufw, bufh;
	int screenw, screenh;
	int intensity;

	assert(ms_pCurrentFb);
	assert(ms_pBlurBuffer);

#ifndef NDEBUG
	if(GetAsyncKeyState(VK_F5) & 0x8000){
		ms_bJustInitialised = true;
		return;
	}
#endif

	bufw = ms_pCurrentFb->width;
	bufh = ms_pCurrentFb->height;
	screenw = RwCameraGetRaster(Scene.camera)->width;
	screenh = RwCameraGetRaster(Scene.camera)->height;

	makequad(ms_blurVerts, screenw, screenh, bufw, bufh);
	for(i = 0; i < 4; i++)
		ms_blurVerts[i].x += offset;
	makequad(ms_blurVerts+4, screenw, screenh, bufw, bufh);
	for(i = 4; i < 8; i++){
		ms_blurVerts[i].x += offset;
		ms_blurVerts[i].y += offset;
	}
	makequad(ms_blurVerts+8, screenw, screenh, bufw, bufh);
	for(i = 8; i < 12; i++)
		ms_blurVerts[i].y += offset;
	makequad(ms_blurVerts+12, screenw, screenh, bufw, bufh);
	for(i = 12; i < 16; i++)
		ms_blurVerts[i].emissiveColor = D3DCOLOR_ARGB(0xff, color.red, color.green, color.blue);
	makequad(ms_blurVerts+16, screenw, screenh, bufw, bufh);
	makequad(ms_blurVerts+20, screenw, screenh, bufw, bufh);
	for(i = 20; i < 24; i++)
		ms_blurVerts[i].emissiveColor = 0;

	CPostEffects::ImmediateModeRenderStatesStore();
	CPostEffects::ImmediateModeRenderStatesSet();
	RwD3D9SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERLINEAR);

	// get current frame
	RwCameraEndUpdate(Scene.camera);
	RwRasterPushContext(ms_pCurrentFb);
	RwRasterRenderFast(RwCameraGetRaster(Scene.camera), 0, 0);
	RwRasterPopContext();
	RwCameraBeginUpdate(Scene.camera);

	// blur frame
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, ms_pCurrentFb);
	RwD3D9SetRenderState(D3DRS_SRCBLEND, D3DBLEND_BLENDFACTOR);
	RwD3D9SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVBLENDFACTOR);
	intensity = intensityf*0.8f;
	RwD3D9SetRenderState(D3DRS_BLENDFACTOR, D3DCOLOR_ARGB(0xFF, intensity, intensity, intensity));
	RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRILIST, ms_blurVerts, 12, ms_blurIndices, 3*6);

	// add colour filter color
	RwD3D9SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	RwD3D9SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRILIST, ms_blurVerts+12, 4, ms_blurIndices, 6);

	// blend with last frame
	if(ms_bJustInitialised)
		ms_bJustInitialised = false;
	else{
		RwRenderStateSet(rwRENDERSTATETEXTURERASTER, ms_pBlurBuffer);
		RwD3D9SetRenderState(D3DRS_SRCBLEND, D3DBLEND_BLENDFACTOR);
		RwD3D9SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVBLENDFACTOR);
		RwD3D9SetRenderState(D3DRS_BLENDFACTOR, D3DCOLOR_ARGB(0xFF, 32, 32, 32));
		RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRILIST, ms_blurVerts+16, 4, ms_blurIndices, 6);
	}

	// blend with black. Is this real?
if(0){
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
	RwD3D9SetRenderState(D3DRS_SRCBLEND, D3DBLEND_BLENDFACTOR);
	RwD3D9SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVBLENDFACTOR);
	RwD3D9SetRenderState(D3DRS_BLENDFACTOR, D3DCOLOR_ARGB(0xFF, 32, 32, 32));
	RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRILIST, ms_blurVerts+20, 4, ms_blurIndices, 6);
}

	RwCameraEndUpdate(Scene.camera);
	RwRasterPushContext(ms_pBlurBuffer);
	RwRasterRenderFast(RwCameraGetRaster(Scene.camera), 0, 0);
	RwRasterPopContext();
	RwCameraBeginUpdate(Scene.camera);

	RwD3D9SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	CPostEffects::ImmediateModeRenderStatesReStore();
}


void CPostEffects::Render()
{
	RwRGBA blurcol;
	if(m_bTrailsEnabled)
		Radiosity(CTimeCycle::m_CurrentColours.radiosityLimit, CTimeCycle::m_CurrentColours.radiosityIntensity);
	if(blurEnabled){
		blurcol.red = CTimeCycle::m_CurrentColours.blurr;
		blurcol.green = CTimeCycle::m_CurrentColours.blurg;
		blurcol.blue = CTimeCycle::m_CurrentColours.blurb;
		BlurOverlay(CTimeCycle::m_CurrentColours.blura, CTimeCycle::m_CurrentColours.bluroffset, blurcol);
	}
}

void CPostEffects::Initialise()
{
//	if(m_bTrailsEnabled){
		Radiosity_Close();
		Radiosity_Init();
//	}
	Blur_Close();
	Blur_Init();
}

static StaticPatcher	Patcher([](){ 
	Memory::InjectHook(0x53E227, CPostEffects::Render);
	Memory::InjectHook(0x5BD779, CPostEffects::Initialise);
	Memory::InjectHook(0x7482EB, CPostEffects::Initialise);
	Memory::InjectHook(0x745C7D, CPostEffects::Initialise);
	Memory::InjectHook(0x53BC27, CPostEffects::Close);
	Memory::Nop(0x53C0D5, 5);

	if(DebugMenuLoad()){
		DebugMenuAddVarBool8("Rendering|Post FX", "Enable Radiosity", (int8*)&CPostEffects::m_bTrailsEnabled, NULL);
		DebugMenuAddVarBool32("Rendering|Post FX", "Enable Blur", &blurEnabled, NULL);
	}
});