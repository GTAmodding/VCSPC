#include "StdAfx.h"
#include "NeoCarpipe.h"

#include <DirectXMath.h>
#include "Rs.h"
#include "TxdStore.h"
#include "Clock.h"
#include "Weather.h"
#include "PipelineCommon.h"
#include "Scene.h"
#include "VisibilityPlugins.h"
#include "YCoCg.h"
#include "TimeCycle.h"

//#define DEBUGTEX

enum PipeSwitch {
	PIPE_NEO,
	PIPE_VCS
};

struct CustomEnvMapPipeMaterialData
{
	int8_t scaleX;
	int8_t scaleY;
	int8_t transScaleX;
	int8_t transScaleY;
	int8_t shininess;
	uint8_t pad3;
	uint16_t renderFrameCounter;
	RwTexture *texture;
};
RwInt32 &CCustomCarEnvMapPipeline__ms_envMapPluginOffset = *(RwInt32*)0x8D12C4;

struct CustomSpecMapPipeMaterialData
{
	float specularity;
	RwTexture *texture;
};
RwInt32 &CCustomCarEnvMapPipeline__ms_specularMapPluginOffset = *(RwInt32*)0x8D12CC;

RwTexDictionary *neoTxd;

int CarPipe::PipeSwitch;

void
neoInit(void)
{
	ONCE;

	CTxdStore::PushCurrentTxd();
	CTxdStore::SetCurrentTxd(CTxdStore::FindTxdSlot("particle"));
	neoCarPipeInit();
	CTxdStore::PopCurrentTxd();
}

#define INTERP_SETUP \
		int h1 = CClock::GetHours();								  \
		int h2 = (h1+1)%24;										  \
		int w1 = CWeather::OldWeatherType;								  \
		int w2 = CWeather::NewWeatherType;								  \
		float timeInterp = (CClock::GetSeconds()/60.0f + CClock::GetMinutes())/60.0f;	  \
		float c0 = (1.0f-timeInterp)*(1.0f-CWeather::InterpolationValue);				  \
		float c1 = timeInterp*(1.0f-CWeather::InterpolationValue);					  \
		float c2 = (1.0f-timeInterp)*CWeather::InterpolationValue;					  \
		float c3 = timeInterp*CWeather::InterpolationValue;
#define INTERP(v) v[h1][w1]*c0 + v[h2][w1]*c1 + v[h1][w2]*c2 + v[h2][w2]*c3;
#define INTERPF(v,f) v[h1][w1].f*c0 + v[h2][w1].f*c1 + v[h1][w2].f*c2 + v[h2][w2].f*c3;


InterpolatedFloat::InterpolatedFloat(float init)
{
	curInterpolator = 61;	// compared against second
	for(int h = 0; h < 24; h++)
		for(int w = 0; w < NUMWEATHERS; w++)
			data[h][w] = init;
}

void
InterpolatedFloat::Read(char *s, int line, int field)
{
	sscanf(s, "%f", &data[line][field]);
}

float
InterpolatedFloat::Get(void)
{
	if(curInterpolator != CClock::GetSeconds()){
		INTERP_SETUP
		curVal = INTERP(data);
		curInterpolator = CClock::GetSeconds();
	}
	return curVal;
}

void
InterpolatedFloat::Copy(int dst, int src)
{
	for(int h = 0; h < 24; h++)
		data[h][dst] = data[h][src];
}

InterpolatedColor::InterpolatedColor(const Color &init)
{
	curInterpolator = 61;	// compared against second
	for(int h = 0; h < 24; h++)
		for(int w = 0; w < NUMWEATHERS; w++)
			data[h][w] = init;
}

void
InterpolatedColor::Read(char *s, int line, int field)
{
	int r, g, b, a;
	sscanf(s, "%i, %i, %i, %i", &r, &g, &b, &a);
	data[line][field] = Color(r/255.0f, g/255.0f, b/255.0f, a/255.0f);
}

Color
InterpolatedColor::Get(void)
{
	if(curInterpolator != CClock::GetSeconds()){
		INTERP_SETUP
		curVal.r = INTERPF(data, r);
		curVal.g = INTERPF(data, g);
		curVal.b = INTERPF(data, b);
		curVal.a = INTERPF(data, a);
		curInterpolator = CClock::GetSeconds();
	}
	return curVal;
}

void
InterpolatedColor::Copy(int dst, int src)
{
	for(int h = 0; h < 24; h++)
		data[h][dst] = data[h][src];
}

void
InterpolatedLight::Read(char *s, int line, int field)
{
	int r, g, b, a;
	sscanf(s, "%i, %i, %i, %i", &r, &g, &b, &a);
	data[line][field] = Color(r/255.0f, g/255.0f, b/255.0f, a/100.0f);
}

void
neoReadWeatherTimeBlock(FILE *file, InterpolatedValue *interp)
{
	char buf[128], *p;
	int c;
	int line, field;

	line = 0;
	c = getc(file);
	while(c != EOF && line < 24){
		field = 0;
		if(c != EOF && c != '#'){
			while(c != EOF && c != '\n' && field < 8){
				p = buf;
				while(c != EOF && c == '\t')
					c = getc(file);
				*p++ = c;
				while(c = getc(file), c != EOF && c != '\t' && c != '\n')
					*p++ = c;
				*p++ = '\0';
				interp->Read(buf, line, CWeather::weatherMap[field]);
				field++;
			}
			line++;
		}
		while(c != EOF && c != '\n')
			c = getc(file);
		c = getc(file);
	}
	ungetc(c, file);
	// fill unused slots with similar data
	for(c = 0; c < NUMWEATHERS; c++)
		interp->Copy(c, CWeather::weatherMap[CWeather::revWeatherMap[c]]);
}

#if 0
RxPipeline*
neoCreatePipe(void)
{
	RxPipeline *pipe;
	RxPipelineNode *node;

	pipe = RxPipelineCreate();
	if(pipe){
		RxLockedPipe *lpipe;
		lpipe = RxPipelineLock(pipe);
		if(lpipe){
			RxNodeDefinition *nodedef= RxNodeDefinitionGetD3D8AtomicAllInOne();
			RxLockedPipeAddFragment(lpipe, 0, nodedef, NULL);
			RxLockedPipeUnlock(lpipe);

			node = RxPipelineFindNodeByName(pipe, nodedef->name, NULL, NULL);
			RxD3D8AllInOneSetInstanceCallBack(node, D3D8AtomicDefaultInstanceCallback_fixed);
			RxD3D8AllInOneSetRenderCallBack(node, rxD3D8DefaultRenderCallback_xbox);
			return pipe;
		}
		RxPipelineDestroy(pipe);
	}
	return NULL;
}

void
CustomPipe::CreateRwPipeline(void)
{
	rwPipeline = neoCreatePipe();
}

void
CustomPipe::SetRenderCallback(RxD3D8AllInOneRenderCallBack cb)
{
	RxPipelineNode *node;
	RxNodeDefinition *nodedef = RxNodeDefinitionGetD3D8AtomicAllInOne();
	node = RxPipelineFindNodeByName(rwPipeline, nodedef->name, NULL, NULL);
	originalRenderCB = RxD3D8AllInOneGetRenderCallBack(node);
	RxD3D8AllInOneSetRenderCallBack(node, cb);
}

void
CustomPipe::Attach(RpAtomic *atomic)
{
	RpAtomicSetPipeline(atomic, rwPipeline);
	/*
	 * From rw 3.7 changelog:
	 *
	 *	07/01/03 Cloning atomics with MatFX - BZ#3430
	 *	  When cloning cloning an atomic with matfx applied the matfx copy callback
	 *	  would also setup the matfx pipes after they had been setup by the atomic
	 *	  clone function. This could overwrite things like skin or patch pipes with
	 *	  generic matfx pipes when it shouldn't.
	 *
	 * Since we're not using 3.5 we have to fix this manually:
	 */
	*RWPLUGINOFFSET(int, atomic, MatFXAtomicDataOffset) = 0;
}

RpAtomic*
CustomPipe::setatomicCB(RpAtomic *atomic, void *data)
{
	((CustomPipe*)data)->Attach(atomic);
	return atomic;
}
#endif


enum {
	LOC_combined    = 0,
	LOC_world       = 4,
	LOC_tex         = 8,
	LOC_eye         = 12,
	LOC_directDir   = 13,
	LOC_ambient     = 15,
	LOC_matCol      = 16,
	LOC_directCol   = 17,
	LOC_lightDir    = 18,
	LOC_lightCol    = 24,

	LOC_directSpec  = 30,	// for carpipe
	LOC_reflProps   = 31,
	LOC_surfProps	= 32,
};


WRAPPER void CRenderer__RenderEverythingBarRoads(void) { EAXJMP(0x553AA0); }
WRAPPER void CRenderer__RenderFadingInEntities(void) { EAXJMP(0x553220); }

InterpolatedFloat CarPipe::fresnel(0.4f);
InterpolatedFloat CarPipe::power(18.0f);
InterpolatedLight CarPipe::diffColor(Color(0.0f, 0.0f, 0.0f, 0.0f));
InterpolatedLight CarPipe::specColor(Color(0.7f, 0.7f, 0.7f, 1.0f));
void *CarPipe::vertexShaderPass1;
void *CarPipe::vertexShaderPass2;
void *CarPipe::pixelShader;
void *CarPipe::pixelShaderYCG1;
void *CarPipe::pixelShaderYCG2;

void *CarPipe::vcsVertexShader;
void *CarPipe::vcsPixelShader;
void *CarPipe::vcsPixelShaderYCG1;
void *CarPipe::vcsPixelShaderYCG2;
// reflection map
RwCamera *CarPipe::reflectionCam;
RwTexture *CarPipe::reflectionMask;
RwTexture *CarPipe::reflectionTex;
RwIm2DVertex CarPipe::screenQuad[4];
RwImVertexIndex CarPipe::screenindices[6] = { 0, 1, 2, 0, 2, 3 };

CarPipe carpipe;

WRAPPER void RenderScene(void) { EAXJMP(0x53DF40); }

void
RenderScene_hook(void)
{
	RenderScene();
	CarPipe::RenderEnvTex();
}

void
neoCarPipeInit(void)
{
	ONCE;
	carpipe.Init();
	CarPipe::SetupEnvMap();
	Memory::InjectHook(0x53EABF, RenderScene_hook);
}

//
// Reflection map
//

int envMapSize = 256;
RpWorld *&scene = *(RpWorld**)0xC17038;

void
CarPipe::SetupEnvMap(void)
{
	reflectionMask = RwTextureRead("CarReflectionMask", NULL);

	RwRaster *envFB = RwRasterCreate(envMapSize, envMapSize, 0, rwRASTERTYPECAMERATEXTURE);
	RwRaster *envZB = RwRasterCreate(envMapSize, envMapSize, 0, rwRASTERTYPEZBUFFER);
	reflectionCam = RwCameraCreate();
	RwCameraSetRaster(reflectionCam, envFB);
	RwCameraSetZRaster(reflectionCam, envZB);
	RwCameraSetFrame(reflectionCam, RwFrameCreate());
	RwCameraSetNearClipPlane(reflectionCam, 0.1f);
	RwCameraSetFarClipPlane(reflectionCam, 250.0f);
	RwV2d vw;
	vw.x = vw.y = 0.4f;
	RwCameraSetViewWindow(reflectionCam, &vw);
	RpWorldAddCamera(scene, reflectionCam);

	reflectionTex = RwTextureCreate(envFB);
	RwTextureSetFilterMode(reflectionTex, rwFILTERLINEAR);

	MakeScreenQuad();
}

void
CarPipe::MakeQuadTexCoords(bool textureSpace)
{
	float minU, minV, maxU, maxV;
	if(textureSpace){
		minU = minV = 0.0f;
		maxU = maxV = 1.0f;
	}else{
		assert(0 && "not implemented");
	}
	screenQuad[0].u = minU;
	screenQuad[0].v = minV;
	screenQuad[1].u = minU;
	screenQuad[1].v = maxV;
	screenQuad[2].u = maxU;
	screenQuad[2].v = maxV;
	screenQuad[3].u = maxU;
	screenQuad[3].v = minV;
}

void
CarPipe::MakeScreenQuad(void)
{
	int width = reflectionTex->raster->width;
	int height = reflectionTex->raster->height;
	screenQuad[0].x = 0.0f;
	screenQuad[0].y = 0.0f;
	screenQuad[0].z = RwIm2DGetNearScreenZ();
	screenQuad[0].rhw = 1.0f / reflectionCam->nearPlane;
	screenQuad[0].emissiveColor = 0xFFFFFFFF;
	screenQuad[1].x = 0.0f;
	screenQuad[1].y = height;
	screenQuad[1].z = screenQuad[0].z;
	screenQuad[1].rhw = screenQuad[0].rhw;
	screenQuad[1].emissiveColor = 0xFFFFFFFF;
	screenQuad[2].x = width;
	screenQuad[2].y = height;
	screenQuad[2].z = screenQuad[0].z;
	screenQuad[2].rhw = screenQuad[0].rhw;
	screenQuad[2].emissiveColor = 0xFFFFFFFF;
	screenQuad[3].x = width;
	screenQuad[3].y = 0;
	screenQuad[3].z = screenQuad[0].z;
	screenQuad[3].rhw = screenQuad[0].rhw;
	screenQuad[3].emissiveColor = 0xFFFFFFFF;
	MakeQuadTexCoords(true);
}

void
CarPipe::RenderReflectionScene(void)
{
	RwRenderStateSet(rwRENDERSTATEFOGENABLE, 0);
	CRenderer__RenderEverythingBarRoads();
	CRenderer__RenderFadingInEntities();
}

void
CarPipe::RenderEnvTex(void)
{
	RwCameraEndUpdate(Camera);

	RwV2d oldvw, vw = { 2.0f, 2.0f };
	oldvw = reflectionCam->viewWindow;
	RwCameraSetViewWindow(reflectionCam, &vw);

	static RwMatrix *reflectionMatrix;
	if(reflectionMatrix == NULL){
		reflectionMatrix = RwMatrixCreate();
		reflectionMatrix->right.x = -1.0f;
		reflectionMatrix->right.y = 0.0f;
		reflectionMatrix->right.z = 0.0f;
		reflectionMatrix->up.x = 0.0f;
		reflectionMatrix->up.y = -1.0f;
		reflectionMatrix->up.z = 0.0f;
		reflectionMatrix->at.x = 0.0f;
		reflectionMatrix->at.y = 0.0f;
		reflectionMatrix->at.z = 1.0f;
	}
	RwMatrix *cammatrix = RwFrameGetMatrix(RwCameraGetFrame(Camera));
	reflectionMatrix->pos = cammatrix->pos;
	RwMatrixUpdate(reflectionMatrix);
	if(PipeSwitch == PIPE_NEO)
		RwFrameTransform(RwCameraGetFrame(reflectionCam), reflectionMatrix, rwCOMBINEREPLACE);
	else
		RwFrameTransform(RwCameraGetFrame(reflectionCam), RwFrameGetLTM(RwCameraGetFrame(Camera)), rwCOMBINEREPLACE);
	CColourSet &c = CTimeCycle::m_CurrentColours;
	RwRGBA color = { c.skytopr, c.skytopg, c.skytopb, 255 };
	RwCameraClear(reflectionCam, &color, rwCAMERACLEARIMAGE | rwCAMERACLEARZ);

	RwCameraBeginUpdate(reflectionCam);
	RwCamera *savedcam = Camera;
	Camera = reflectionCam;	// they do some begin/end updates with this in the called functions :/
	RenderReflectionScene();
	Camera = savedcam;

	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)1);
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDZERO);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDSRCCOLOR);
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, reflectionMask->raster);
	if(PipeSwitch == PIPE_NEO)
		RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRILIST, screenQuad, 4, screenindices, 6);
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, 0);

	RwCameraEndUpdate(reflectionCam);
	RwCameraSetViewWindow(reflectionCam, &oldvw);

	RwCameraBeginUpdate(Camera);
#ifdef DEBUGTEX
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, reflectionTex->raster);
	RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRILIST, screenQuad, 4, screenindices, 6);
#endif
}

//
//
//

CarPipe::CarPipe(void)
{
}

void
CarPipe::Init(void)
{
	CreateShaders();
	LoadTweakingTable();

	// some camera begin/end stuff in barroads with the rw cam
//	Nop(0x553C68, 0x553C9F-0x553C68);
//	Nop(0x553CA4, 3);
//	Nop(0x553CCA, 0x553CF4-0x553CCA);
}

void
CarPipe::CreateShaders(void)
{
	vertexShaderPass1 = RwD3D9CreateVertexShaderFromFile("neoVehiclePass1");
	vertexShaderPass2 = RwD3D9CreateVertexShaderFromFile("neoVehiclePass2");
	pixelShader = RwD3D9CreatePixelShaderFromFile("neoVehicle");
	pixelShaderYCG1 = RwD3D9CreatePixelShaderFromFile("neoVehicleYCG1");
	pixelShaderYCG2 = RwD3D9CreatePixelShaderFromFile("neoVehicleYCG2");

	vcsVertexShader = RwD3D9CreateVertexShaderFromFile("vcsVehicle");
	vcsPixelShader = RwD3D9CreatePixelShaderFromFile("vcsVehicle");
	vcsPixelShaderYCG1 = RwD3D9CreatePixelShaderFromFile("vcsVehicleYCG1");
	vcsPixelShaderYCG2 = RwD3D9CreatePixelShaderFromFile("vcsVehicleYCG2");
}

void
CarPipe::LoadTweakingTable(void)
{
	char *path;
	FILE *dat;
	dat = fopen("data/carTweakingTable.dat", "r");
	assert(dat);
	neoReadWeatherTimeBlock(dat, &fresnel);
	neoReadWeatherTimeBlock(dat, &power);
	neoReadWeatherTimeBlock(dat, &diffColor);
	neoReadWeatherTimeBlock(dat, &specColor);
	fclose(dat);
}

//
// Rendering
//

void
UploadLightColorWithSpecular(RpLight *light, int loc)
{
	float c[4];
	if(RpLightGetFlags(light) & rpLIGHTLIGHTATOMICS){
		Color s = CarPipe::specColor.Get();
		c[0] = light->color.red * s.a;
		c[1] = light->color.green * s.a;
		c[2] = light->color.blue * s.a;
		c[3] = 1.0f;
		RwD3D9SetVertexShaderConstant(loc, (void*)c, 1);
	}else
		pipeUploadZero(loc);
}

void
CarPipe::ShaderSetup(RpAtomic *atomic)
{
	float worldMat[16], combined[16];
	DirectX::XMMATRIX texMat;
	RwCamera *cam = (RwCamera*)RWSRCGLOBAL(curCamera);

	pipeGetComposedTransformMatrix(atomic, combined);
	RwToD3DMatrix(&worldMat, RwFrameGetLTM(RpAtomicGetFrame(atomic)));
	RwD3D9SetVertexShaderConstant(LOC_combined, (void*)&combined, 4);
	RwD3D9SetVertexShaderConstant(LOC_world, (void*)&worldMat, 4);
	texMat = DirectX::XMMatrixIdentity();
	RwD3D9SetVertexShaderConstant(LOC_tex, (void*)&texMat, 4);

	RwMatrix *camfrm = RwFrameGetLTM(RwCameraGetFrame(cam));
	RwD3D9SetVertexShaderConstant(LOC_eye, (void*)RwMatrixGetPos(camfrm), 1);

	UploadLightColorWithSpecular(pAmbient, LOC_ambient);
	UploadLightColorWithSpecular(pDirect, LOC_directCol);
	pipeUploadLightDirection(pDirect, LOC_directDir);
	for(int i = 0 ; i < 6; i++)
		if(i < NumExtraDirLightsInWorld && RpLightGetType(pExtraDirectionals[i]) == rpLIGHTDIRECTIONAL){
			pipeUploadLightDirection(pExtraDirectionals[i], LOC_lightDir+i);
			UploadLightColorWithSpecular(pExtraDirectionals[i], LOC_lightCol+i);
		}else{
			pipeUploadZero(LOC_lightDir+i);
			pipeUploadZero(LOC_lightCol+i);
		}
	Color spec = specColor.Get();
	spec.r *= spec.a;
	spec.g *= spec.a;
	spec.b *= spec.a;
	RwD3D9SetVertexShaderConstant(LOC_directSpec, (void*)&spec, 1);
}

void
CarPipe::DiffusePass(RxD3D9ResEntryHeader *header, RpAtomic *atomic)
{
	RxD3D9InstanceData *inst = (RxD3D9InstanceData*)&header[1];
	int noRefl;

	RwD3D9SetTexture(reflectionTex, 1);

	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)1);
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);

	if(PipeSwitch == PIPE_NEO)
		RwD3D9SetVertexShader(vertexShaderPass1);
	else
		RwD3D9SetVertexShader(vcsVertexShader);

	noRefl = CVisibilityPlugins::GetAtomicId(atomic) & 0x6000;

	for(uint32 i = 0; i < header->numMeshes; i++){
		RpMaterial *material = inst->material;
		pipeSetTexture(material->texture, 0);
		if(PipeSwitch == PIPE_NEO){
			if(material->texture && YCOCGPLUGINDATACONST(material->texture)->bYCoCgType != 0)
				RwD3D9SetPixelShader(YCOCGPLUGINDATACONST(material->texture)->bYCoCgType == 2 ? pixelShaderYCG2 : pixelShaderYCG1);
			else
				RwD3D9SetPixelShader(pixelShader);
		}else{
			if(material->texture && YCOCGPLUGINDATACONST(material->texture)->bYCoCgType != 0)
				RwD3D9SetPixelShader(YCOCGPLUGINDATACONST(material->texture)->bYCoCgType == 2 ? vcsPixelShaderYCG2 : vcsPixelShaderYCG1);
			else
				RwD3D9SetPixelShader(vcsPixelShader);
		}

		int matfx = RpMatFXMaterialGetEffects(material);
		float envcoeff = 0.0f;
		if(!noRefl && matfx == rpMATFXEFFECTENVMAP)
			envcoeff = RpMatFXMaterialGetEnvMapCoefficient(material);

		Color c = diffColor.Get();
		Color diff(c.r*c.a, c.g*c.a, c.b*c.a, 1.0f-c.a);
		RwRGBAReal mat;
		RwRGBARealFromRwRGBA(&mat, &inst->material->color);
		mat.red = mat.red*diff.a + diff.r;
		mat.green = mat.green*diff.a + diff.g;
		mat.blue = mat.blue*diff.a + diff.b;
		RwD3D9SetVertexShaderConstant(LOC_matCol, (void*)&mat, 1);

		RwD3D9SetVertexShaderConstant(LOC_surfProps, &material->surfaceProps, 1);

		float reflProps[4];
		if(PipeSwitch == PIPE_NEO)
			reflProps[0] = material->surfaceProps.specular;
		else
			reflProps[0] = envcoeff;
		reflProps[1] = fresnel.Get();
		reflProps[2] = 1.0;
		reflProps[3] = power.Get();
		RwD3D9SetVertexShaderConstant(LOC_reflProps, (void*)reflProps, 1);

		D3D9RenderDual(1, header, inst);
		inst++;
	}
}

void
CarPipe::SpecularPass(RxD3D9ResEntryHeader *header, RpAtomic *atomic)
{
	RwUInt32 src, dst, fog, zwrite, alphatest;
	RxD3D9InstanceData *inst = (RxD3D9InstanceData*)&header[1];
	int noRefl;

	RwRenderStateGet(rwRENDERSTATEZWRITEENABLE, &zwrite);
	RwRenderStateGet(rwRENDERSTATEFOGENABLE, &fog);
	RwRenderStateGet(rwRENDERSTATESRCBLEND, &src);
	RwRenderStateGet(rwRENDERSTATEDESTBLEND, &dst);
	RwRenderStateGet(rwRENDERSTATEALPHATESTFUNCTION, &alphatest);

	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);
	RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)FALSE);
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDONE);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDONE);
	RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTION, (void*)rwALPHATESTFUNCTIONALWAYS);
	RwD3D9SetTexture(NULL, 0);
	RwD3D9SetTexture(NULL, 1);
	RwD3D9SetTexture(NULL, 2);
	RwD3D9SetTexture(NULL, 3);
	RwD3D9SetVertexShader(vertexShaderPass2);
	RwD3D9SetPixelShader(NULL);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)1);

	noRefl = CVisibilityPlugins::GetAtomicId(atomic) & 0x6000;

	for(uint32 i = 0; i < header->numMeshes; i++){
		if(!noRefl && inst->material->surfaceProps.specular != 0.0f)
			D3D9Render(header, inst);
		inst++;
	}
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)zwrite);
	RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)fog);
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)src);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)dst);
	RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTION, (void*)alphatest);
}

void
CarPipe::RenderCallback(RwResEntry *repEntry, void *object, RwUInt8 type, RwUInt32 flags)
{
		{
			static bool keystate = false;
			if(GetAsyncKeyState(VK_F7) & 0x8000){
				if(!keystate){
					keystate = true;
					LoadTweakingTable();
				}
			}else
				keystate = false;
		}
	RxD3D9ResEntryHeader *header = (RxD3D9ResEntryHeader*)&repEntry[1];
	ShaderSetup((RpAtomic*)object);

	if(header->indexBuffer != NULL)
		RwD3D9SetIndices(header->indexBuffer);
	_rwD3D9SetStreams(header->vertexStream, header->useOffsets);
	RwD3D9SetVertexDeclaration(header->vertexDeclaration);

	DiffusePass(header, (RpAtomic*)object);
	if(PipeSwitch == PIPE_NEO)
		SpecularPass(header, (RpAtomic*)object);
	RwD3D9SetTexture(NULL, 1);
	RwD3D9SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	RwD3D9SetTexture(NULL, 2);
	RwD3D9SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(2, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
}

// TODO: patch this in somewhere else
WRAPPER void InitialiseGame(void) { EAXJMP(0x53E580); }
void
InitialiseGame_hook(void)
{
	ONCE;
	InitialiseGame();
	neoInit();
}


// sun glare
WRAPPER void CVehicle__DoSunGlare(void *this_) { EAXJMP(0x6DD6F0); }

void __declspec(naked) doglare(void)
{
	_asm {
		mov	ecx,esi
		call	CVehicle__DoSunGlare
		mov     [esp+0D4h], edi
		push	6ABD04h
		retn
	}
}

static StaticPatcher	Patcher([](){
	// not neo, but enable sun glare
	Memory::InjectHook(0x6ABCFD, doglare, PATCH_JUMP);


	Memory::InjectHook(0x748CFB, InitialiseGame_hook);

	Memory::Patch(0x5D9FE3 +1, CarPipe::RenderCallback);
	// remove some of the SA vehicle pipe
	Memory::Nop(0x5DA620, 5);	// for all mats CCustomCarEnvMapPipeline::CustomPipeMaterialSetup
	Memory::Nop(0x53DFCE, 5);	// CCarFXRenderer::PreRenderUpdate
	Memory::Patch<uint8>(0x5D9900, 0xCC);	// disable CCustomCarEnvMapPipeline::CustomPipeRenderCB
});
