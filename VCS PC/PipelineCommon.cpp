#include "StdAfx.h"
#include "PipelineCommon.h"
#include <DirectXMath.h>


RwTexture *&gpWhiteTexture = *(RwTexture**)0xB4E3EC;

void
RwToD3DMatrix(void *d3d, RwMatrix *rw)
{
	D3DMATRIX *m = (D3DMATRIX*)d3d;
	m->m[0][0] = rw->right.x;
	m->m[0][1] = rw->up.x;
	m->m[0][2] = rw->at.x;
	m->m[0][3] = rw->pos.x;
	m->m[1][0] = rw->right.y;
	m->m[1][1] = rw->up.y;
	m->m[1][2] = rw->at.y;
	m->m[1][3] = rw->pos.y;
	m->m[2][0] = rw->right.z;
	m->m[2][1] = rw->up.z;
	m->m[2][2] = rw->at.z;
	m->m[2][3] = rw->pos.z;
	m->m[3][0] = 0.0f;
	m->m[3][1] = 0.0f;
	m->m[3][2] = 0.0f;
	m->m[3][3] = 1.0f;
}

float *RwD3D9D3D9ViewTransform = (float*)0xC9BC80;
float *RwD3D9D3D9ProjTransform = (float*)0x8E2458;

void
transpose(void *dst, void *src)
{
	float (*m1)[4] = (float(*)[4])dst;
	float (*m2)[4] = (float(*)[4])src;
	m1[0][0] = m2[0][0];
	m1[0][1] = m2[1][0];
	m1[0][2] = m2[2][0];
	m1[0][3] = m2[3][0];
	m1[1][0] = m2[0][1];
	m1[1][1] = m2[1][1];
	m1[1][2] = m2[2][1];
	m1[1][3] = m2[3][1];
	m1[2][0] = m2[0][2];
	m1[2][1] = m2[1][2];
	m1[2][2] = m2[2][2];
	m1[2][3] = m2[3][2];
	m1[3][0] = m2[0][3];
	m1[3][1] = m2[1][3];
	m1[3][2] = m2[2][3];
	m1[3][3] = m2[3][3];
}

static DirectX::XMMATRIX pipeWorldMat, pipeViewMat, pipeProjMat;

void
pipeGetComposedTransformMatrix(RpAtomic *atomic, float *out)
{
	RwMatrix *world = RwFrameGetLTM(RpAtomicGetFrame(atomic));

	RwToD3DMatrix(&pipeWorldMat, world);
	transpose(&pipeViewMat, RwD3D9D3D9ViewTransform);
	transpose(&pipeProjMat, RwD3D9D3D9ProjTransform);

	DirectX::XMMATRIX combined = DirectX::XMMatrixMultiply(pipeProjMat, DirectX::XMMatrixMultiply(pipeViewMat, pipeWorldMat));
	memcpy(out, &combined, 64);
}

void
pipeUploadMatCol(int flags, RpMaterial *m, int loc)
{
	static float white[4] = { 1.0, 1.0, 1.0, 1.0 };
	if(flags & rpGEOMETRYMODULATEMATERIALCOLOR){
		RwRGBAReal color;
		RwRGBARealFromRwRGBA(&color, &m->color);
		RwD3D9SetVertexShaderConstant(loc, &color, 1);
	}else
		RwD3D9SetVertexShaderConstant(loc, white, 1);
}

void
pipeUploadZero(int loc)
{
	static float z[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	RwD3D9SetVertexShaderConstant(loc, (void*)z, 1);
}

void
pipeUploadLightDirection(RpLight *light, int loc)
{
	float c[4];
	if(RpLightGetFlags(light) & rpLIGHTLIGHTATOMICS){
		RwV3d *at = RwMatrixGetAt(RwFrameGetLTM(RpLightGetFrame(light)));
		c[0] = at->x;
		c[1] = at->y;
		c[2] = at->z;
		c[3] = 1.0f;
		RwD3D9SetVertexShaderConstant(loc, (void*)c, 1);
	}else
		pipeUploadZero(loc);
}

void
D3D9Render(RxD3D9ResEntryHeader *resEntryHeader, RxD3D9InstanceData *instanceData)
{
	if(resEntryHeader->indexBuffer)
		RwD3D9DrawIndexedPrimitive(resEntryHeader->primType, instanceData->baseIndex, 0, instanceData->numVertices, instanceData->startIndex, instanceData->numPrimitives);
	else
		RwD3D9DrawPrimitive(resEntryHeader->primType, instanceData->baseIndex, instanceData->numPrimitives);
}

void
D3D9RenderDual(int dual, RxD3D9ResEntryHeader *resEntryHeader, RxD3D9InstanceData *instanceData)
{
	RwBool hasAlpha;
	int alphafunc, alpharef;
	int zwrite;
	// this also takes texture alpha into account
	RwD3D9GetRenderState(D3DRS_ALPHABLENDENABLE, &hasAlpha);
	RwRenderStateGet(rwRENDERSTATEZWRITEENABLE, &zwrite);
	RwRenderStateGet(rwRENDERSTATEALPHATESTFUNCTION, &alphafunc);
	if(dual && hasAlpha && zwrite){
		RwRenderStateGet(rwRENDERSTATEALPHATESTFUNCTIONREF, &alpharef);
		RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, (void*)128);
		RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTION, (void*)rwALPHATESTFUNCTIONGREATEREQUAL);
		RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);
		D3D9Render(resEntryHeader, instanceData);
		RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTION, (void*)rwALPHATESTFUNCTIONLESS);
		RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);
		D3D9Render(resEntryHeader, instanceData);
		RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)zwrite);
		RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, (void*)alpharef);
		RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTION, (void*)alphafunc);
	}else if(!zwrite){
		RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTION, (void*)rwALPHATESTFUNCTIONALWAYS);
		D3D9Render(resEntryHeader, instanceData);
		RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTION, (void*)alphafunc);
	}else
		D3D9Render(resEntryHeader, instanceData);
}