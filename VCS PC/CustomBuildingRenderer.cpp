#include "StdAfx.h"
#include "CustomBuildingRenderer.h"

#include "Scene.h"
#include "PipelineCommon.h"
#include "YCoCg.h"
#include "TimeCycle.h"


RwInt32& CCustomBuildingDNPipeline::ms_extraVertColourPluginOffset = *(RwInt32*)0x8D12BC;

RwInt32 CCustomBuildingDNPipeline::pluginExtraVertColourStreamGetSizeCB(const void* object, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
	//return -1;
	pluginExtraVertColour*	pData = EXTRAVERTCOLOURPLUGIN(object);
	return pData->nightColours ? sizeof(pData->nightColours) + sizeof(*pData->nightColours) * RpGeometryGetNumVertices(static_cast<const RpGeometry*>(object)) : -1;
}

RwStream* CCustomBuildingDNPipeline::pluginExtraVertColourStreamWriteCB(RwStream* stream, RwInt32 binaryLength, const void* object, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
	pluginExtraVertColour*	pData = EXTRAVERTCOLOURPLUGIN(object);
	RwStreamWrite(stream, pData->nightColours, sizeof(pData->nightColours));
	if ( pData->nightColours )
		RwStreamWrite(stream, pData->nightColours, sizeof(*pData->nightColours) * RpGeometryGetNumVertices(static_cast<const RpGeometry*>(object)));
	return stream;
}

void CCustomBuildingDNPipeline::Inject()
{
	Memory::Patch(0x5D7310, pluginExtraVertColourStreamGetSizeCB);
	Memory::Patch(0x5D7315, pluginExtraVertColourStreamWriteCB);
}



enum {
	// common
	REG_transform	= 0,
	REG_ambient	= 4,
	REG_matCol	= 19,
	REG_surfProps	= 20,

	REG_shaderParams= 29,
	// UVA
	REG_texmat	= 32,
};

static void *buildingVS, *buildingPS, *buildingYCG1PS, *buildingYCG2PS;

void
CCustomBuildingDNPipeline::CustomPipeRenderCB(RwResEntry *repEntry, void *object, RwUInt8 type, RwUInt32 flags)
{
	RpAtomic *atomic;
	RxD3D9ResEntryHeader *resEntryHeader;
	RxD3D9InstanceData *instancedData;
	RpMaterial *material;
	RwInt32	numMeshes;
	RwBool hasAlpha;
	float transform[16];

	// TODO: init this somewhere else?
	if(buildingVS == NULL)
		buildingVS = RwD3D9CreateVertexShaderFromFile("building");
	if(buildingPS == NULL)
		buildingPS = RwD3D9CreatePixelShaderFromFile("building");
	if(buildingYCG1PS == NULL)
		buildingYCG1PS = RwD3D9CreatePixelShaderFromFile("buildingYCG1");
	if(buildingYCG2PS == NULL)
		buildingYCG2PS = RwD3D9CreatePixelShaderFromFile("buildingYCG2");

	atomic = (RpAtomic*)object;

	pipeGetComposedTransformMatrix(atomic, transform);
	RwD3D9SetVertexShaderConstant(0, transform, 4);

	resEntryHeader = (RxD3D9ResEntryHeader*)(repEntry + 1);
	instancedData = (RxD3D9InstanceData*)(resEntryHeader + 1);;
	if(resEntryHeader->indexBuffer)
		RwD3D9SetIndices(resEntryHeader->indexBuffer);
	_rwD3D9SetStreams(resEntryHeader->vertexStream, resEntryHeader->useOffsets);
	RwD3D9SetVertexDeclaration(resEntryHeader->vertexDeclaration);

	int alphafunc, alpharef;
	int src, dst;
	int fog;
	int zwrite;
	RwRenderStateGet(rwRENDERSTATEALPHATESTFUNCTIONREF, &alpharef);
	RwRenderStateGet(rwRENDERSTATEALPHATESTFUNCTION, &alphafunc);
	RwRenderStateGet(rwRENDERSTATESRCBLEND, &src);
	RwRenderStateGet(rwRENDERSTATEDESTBLEND, &dst);
	RwRenderStateGet(rwRENDERSTATEFOGENABLE, &fog);
	RwRenderStateGet(rwRENDERSTATEZWRITEENABLE, &zwrite);

	RwD3D9SetVertexShader(buildingVS);
	RwD3D9SetVertexShaderConstant(REG_ambient, &pAmbient->color, 1);

	numMeshes = resEntryHeader->numMeshes;
	while(numMeshes--){
		material = instancedData->material;

		// Is YCoCg texture?
		if(material->texture && YCOCGPLUGINDATACONST(material->texture)->bYCoCgType != 0)
			RwD3D9SetPixelShader(YCOCGPLUGINDATACONST(material->texture)->bYCoCgType == 2 ? buildingYCG2PS : buildingYCG1PS);
		else
			RwD3D9SetPixelShader(buildingPS);
		pipeSetTexture(material->texture, 0);

		int effect = RpMatFXMaterialGetEffects(material);
		if(effect == rpMATFXEFFECTUVTRANSFORM){
			RwMatrix *m1, *m2;
			RpMatFXMaterialGetUVTransformMatrices(material, &m1, &m2);
			RwD3D9SetVertexShaderConstant(REG_texmat, m1, 4);
		}else{
			RwMatrix m;
			RwMatrixSetIdentity(&m);
			RwD3D9SetVertexShaderConstant(REG_texmat, &m, 4);
		}

		hasAlpha = instancedData->vertexAlpha || instancedData->material->color.alpha != 255;
		RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)hasAlpha);

		RwRGBAReal color;
		RwRGBARealFromRwRGBA(&color, &material->color);
		color.red *= CTimeCycle::m_CurrentColours.vertmultr;
		color.green *= CTimeCycle::m_CurrentColours.vertmultg;
		color.blue *= CTimeCycle::m_CurrentColours.vertmultb;
		RwD3D9SetVertexShaderConstant(REG_matCol, &color, 1);

		RwD3D9SetVertexShaderConstant(REG_surfProps, &material->surfaceProps, 1);

		D3D9RenderDual(true, resEntryHeader, instancedData);
		instancedData++;
	}
	RwD3D9SetVertexShader(NULL);
	RwD3D9SetPixelShader(NULL);
	RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, (void*)alpharef);
	RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTION, (void*)alphafunc);
	RwD3D9SetTexture(NULL, 1);
	RwD3D9SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
}


static StaticPatcher	Patcher([](){
						CCustomBuildingDNPipeline::Inject();
						Memory::Patch(0x5D67F3 +1, CCustomBuildingDNPipeline::CustomPipeRenderCB);
						Memory::Patch(0x5D7B0A +1, CCustomBuildingDNPipeline::CustomPipeRenderCB);
									});