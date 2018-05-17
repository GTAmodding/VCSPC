#include "StdAfx.h"
#include "NeoCarpipe.h"

#include <DirectXMath.h>
#include "Rs.h"
#include "TxdStore.h"
#include "Clock.h"
#include "Weather.h"
#include "PipelineCommon.h"
#include "gtamain.h"
#include "Renderer.h"
#include "VisibilityPlugins.h"
#include "YCoCg.h"
#include "TimeCycle.h"
#include "debugmenu_public.h"

int updateEnvMap = true;
int chromeCheat = false;
int debugEnvTex = false;
int drawEnvCoronas = true;

enum PipeSwitch {
	PIPE_NEO,
	PIPE_VCS
};

RwTexDictionary *neoTxd;

int CarPipe::PipeSwitch;

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

InterpolatedFloat CarPipe::fresnel(0.4f);
InterpolatedFloat CarPipe::power(18.0f);
InterpolatedLight CarPipe::diffColor(Color(0.0f, 0.0f, 0.0f, 0.0f));
InterpolatedLight CarPipe::specColor(Color(0.7f, 0.7f, 0.7f, 1.0f));
void *CarPipe::vertexShaderPass1;
void *CarPipe::vertexShaderPass2;
void *CarPipe::pixelShader;
void *CarPipe::pixelShaderYCG1;
void *CarPipe::pixelShaderYCG2;

void *CarPipe::vcsVehiclePass1VS;
void *CarPipe::vcsVehiclePass2VS;
void *CarPipe::simplePS;

// reflection map
RwCamera *CarPipe::reflectionCam;
RwTexture *CarPipe::reflectionMask;
RwTexture *CarPipe::reflectionTex;
RwIm2DVertex CarPipe::screenQuad[4];
RwImVertexIndex CarPipe::screenindices[6] = { 0, 1, 2, 0, 2, 3 };

CarPipe carpipe;

void
CarPipeInit(void)
{
	CTxdStore::PushCurrentTxd();
	CTxdStore::SetCurrentTxd(CTxdStore::FindTxdSlot("particle"));
	carpipe.Init();
	CarPipe::SetupEnvMap();
	CTxdStore::PopCurrentTxd();
}

//
// Reflection map
//

int envMapSize = 128;
RwTexture *coronastar;

void
CarPipe::SetupEnvMap(void)
{
	reflectionMask = RwTextureRead("CarReflectionMask", NULL);
	coronastar = RwTextureRead("coronastar", NULL);

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
	RpWorldAddCamera(Scene.world, reflectionCam);

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
		return;
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

static RwIm2DVertex coronaVerts[4*4];
static RwImVertexIndex coronaIndices[6*4];
static int numCoronaVerts, numCoronaIndices;

static void
AddCorona(float x, float y, float sz)
{
	float nearz, recipz;
	RwIm2DVertex *v;
	nearz = RwIm2DGetNearScreenZ();
	recipz = 1.0f / RwCameraGetNearClipPlane((RwCamera*)RWSRCGLOBAL(curCamera));

	v = &coronaVerts[numCoronaVerts];
	RwIm2DVertexSetScreenX(&v[0], x);
	RwIm2DVertexSetScreenY(&v[0], y);
	RwIm2DVertexSetScreenZ(&v[0], nearz);
	RwIm2DVertexSetRecipCameraZ(&v[0], recipz);
	RwIm2DVertexSetU(&v[0], 0.0f, recipz);
	RwIm2DVertexSetV(&v[0], 0.0f, recipz);
	RwIm2DVertexSetIntRGBA(&v[0], 0xFF, 0xFF, 0xFF, 0xFF);

	RwIm2DVertexSetScreenX(&v[1], x);
	RwIm2DVertexSetScreenY(&v[1], y + sz);
	RwIm2DVertexSetScreenZ(&v[1], nearz);
	RwIm2DVertexSetRecipCameraZ(&v[1], recipz);
	RwIm2DVertexSetU(&v[1], 0.0f, recipz);
	RwIm2DVertexSetV(&v[1], 1.0f, recipz);
	RwIm2DVertexSetIntRGBA(&v[1], 0xFF, 0xFF, 0xFF, 0xFF);

	RwIm2DVertexSetScreenX(&v[2], x + sz);
	RwIm2DVertexSetScreenY(&v[2], y + sz);
	RwIm2DVertexSetScreenZ(&v[2], nearz);
	RwIm2DVertexSetRecipCameraZ(&v[2], recipz);
	RwIm2DVertexSetU(&v[2], 1.0f, recipz);
	RwIm2DVertexSetV(&v[2], 1.0f, recipz);
	RwIm2DVertexSetIntRGBA(&v[2], 0xFF, 0xFF, 0xFF, 0xFF);

	RwIm2DVertexSetScreenX(&v[3], x + sz);
	RwIm2DVertexSetScreenY(&v[3], y);
	RwIm2DVertexSetScreenZ(&v[3], nearz);
	RwIm2DVertexSetRecipCameraZ(&v[3], recipz);
	RwIm2DVertexSetU(&v[3], 1.0f, recipz);
	RwIm2DVertexSetV(&v[3], 0.0f, recipz);
	RwIm2DVertexSetIntRGBA(&v[3], 0xFF, 0xFF, 0xFF, 0xFF);


	coronaIndices[numCoronaIndices++] = numCoronaVerts;
	coronaIndices[numCoronaIndices++] = numCoronaVerts + 1;
	coronaIndices[numCoronaIndices++] = numCoronaVerts + 2;
	coronaIndices[numCoronaIndices++] = numCoronaVerts;
	coronaIndices[numCoronaIndices++] = numCoronaVerts + 2;
	coronaIndices[numCoronaIndices++] = numCoronaVerts + 3;
	numCoronaVerts += 4;
}

void
DrawEnvMapCoronas(RwV3d at)
{
	const float BIG = 89.0f;
	const float SMALL = 38.0f;

	float x;
	numCoronaVerts = 0;
	numCoronaIndices = 0;
	x = CGeneral::GetATanOfXY(-at.y, at.x)/(2*M_PI) - 1.0f;
	x *= BIG+SMALL;
	AddCorona(x, 12.0f, SMALL);	x += SMALL;
	AddCorona(x, 0.0f, BIG);	x += BIG;
	AddCorona(x, 12.0f, SMALL);	x += SMALL;
	AddCorona(x, 0.0f, BIG);	x += BIG;
	RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRILIST, coronaVerts, numCoronaVerts, coronaIndices, numCoronaIndices);
}

void
CarPipe::RenderEnvTex(void)
{
	if(!updateEnvMap)
		return;

	RwCameraEndUpdate(Scene.camera);

	RwV2d oldvw, vw = { 2.0f, 2.0f };
	if(PipeSwitch == PIPE_NEO)
		RwCameraSetViewWindow(reflectionCam, &vw);
	else
		RwCameraSetViewWindow(reflectionCam, &Scene.camera->viewWindow);

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
	RwMatrix *cammatrix = RwFrameGetMatrix(RwCameraGetFrame(Scene.camera));
	reflectionMatrix->pos = cammatrix->pos;
	RwMatrixUpdate(reflectionMatrix);
	if(PipeSwitch == PIPE_NEO)
		RwFrameTransform(RwCameraGetFrame(reflectionCam), reflectionMatrix, rwCOMBINEREPLACE);
	else
		RwFrameTransform(RwCameraGetFrame(reflectionCam), RwFrameGetLTM(RwCameraGetFrame(Scene.camera)), rwCOMBINEREPLACE);
	RwRGBA color = { CTimeCycle::GetSkyTopRed(), CTimeCycle::GetSkyTopGreen(), CTimeCycle::GetSkyTopBlue(), 255 };
	if(CWeather::LightningFlash)
		color.red = color.green = color.blue = 255;
	RwCameraClear(reflectionCam, &color, rwCAMERACLEARIMAGE | rwCAMERACLEARZ);

	RwCameraBeginUpdate(reflectionCam);

	if(PipeSwitch == PIPE_NEO)
		RenderEnvSceneNeo();
	else{
		// little hack to make CClouds aware of our env map size
		int width = RsGlobal.MaximumWidth;
		int height = RsGlobal.MaximumHeight;
		RsGlobal.MaximumWidth = envMapSize;
		RsGlobal.MaximumHeight = envMapSize;
		RenderEnvScene();
		RsGlobal.MaximumWidth = width;
		RsGlobal.MaximumHeight = height;
	}

	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)1);
	if(PipeSwitch == PIPE_NEO){
		RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDZERO);
		RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDSRCCOLOR);
		RwRenderStateSet(rwRENDERSTATETEXTURERASTER, reflectionMask->raster);
		RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRILIST, screenQuad, 4, screenindices, 6);
	}else if(drawEnvCoronas){
		RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDONE);
		RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDONE);
		RwRenderStateSet(rwRENDERSTATETEXTURERASTER, coronastar->raster);
		DrawEnvMapCoronas(RwFrameGetLTM(RwCameraGetFrame(reflectionCam))->at);
	}
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, 0);

	RwCameraEndUpdate(reflectionCam);

	RwCameraBeginUpdate(Scene.camera);
	if(debugEnvTex){
		RwRenderStateSet(rwRENDERSTATETEXTURERASTER, reflectionTex->raster);
		RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRILIST, screenQuad, 4, screenindices, 6);
	}
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
}

void
CarPipe::CreateShaders(void)
{
	vertexShaderPass1 = RwD3D9CreateVertexShaderFromFile("neoVehiclePass1");
	vertexShaderPass2 = RwD3D9CreateVertexShaderFromFile("neoVehiclePass2");
	pixelShader = RwD3D9CreatePixelShaderFromFile("neoVehicle");
	pixelShaderYCG1 = RwD3D9CreatePixelShaderFromFile("neoVehicleYCG1");
	pixelShaderYCG2 = RwD3D9CreatePixelShaderFromFile("neoVehicleYCG2");

	vcsVehiclePass1VS = RwD3D9CreateVertexShaderFromFile("vcsVehiclePass1");
	vcsVehiclePass2VS = RwD3D9CreateVertexShaderFromFile("vcsVehiclePass2");
	simplePS = RwD3D9CreatePixelShaderFromFile("simple");
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

bool
SkipInst(RxD3D9InstanceData *inst)
{
	bool hasAlpha = inst->material->color.alpha != 255;
	bool render = hasAlpha && CRenderer::bRenderTransparent ||
		!hasAlpha && CRenderer::bRenderOpaque;
	return !render;
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
CarPipe::NeoDiffusePass(RxD3D9ResEntryHeader *header, RpAtomic *atomic)
{
	RxD3D9InstanceData *inst = (RxD3D9InstanceData*)&header[1];
	int noRefl;

	RwD3D9SetTexture(reflectionTex, 1);

	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)1);
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);

	RwD3D9SetVertexShader(vertexShaderPass1);

	noRefl = CVisibilityPlugins::GetAtomicId(atomic) & 0x6000;

	for(uint32 i = 0; i < header->numMeshes; i++){
		RpMaterial *material = inst->material;
		if(SkipInst(inst)){
			inst++;
			continue;
		}

		pipeSetTexture(material->texture, 0);
		if(material->texture && YCOCGPLUGINDATACONST(material->texture)->bYCoCgType != 0)
			RwD3D9SetPixelShader(YCOCGPLUGINDATACONST(material->texture)->bYCoCgType == 2 ? pixelShaderYCG2 : pixelShaderYCG1);
		else
			RwD3D9SetPixelShader(pixelShader);

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
		reflProps[0] = material->surfaceProps.specular;
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
		if(!SkipInst(inst))
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
CarPipe::DiffusePass(RxD3D9ResEntryHeader *header, RpAtomic *atomic)
{
	RxD3D9InstanceData *inst = (RxD3D9InstanceData*)&header[1];

	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)1);
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);

	RwD3D9SetVertexShader(vcsVehiclePass1VS);

	for(uint32 i = 0; i < header->numMeshes; i++){
		RpMaterial *material = inst->material;
		if(SkipInst(inst)){
			inst++;
			continue;
		}

		pipeSetTexture(material->texture, 0);
		if(material->texture && YCOCGPLUGINDATACONST(material->texture)->bYCoCgType != 0)
			RwD3D9SetPixelShader(gpGenericPS[YCOCGPLUGINDATACONST(material->texture)->bYCoCgType == 2 ? GEN_PS_YCG2 : GEN_PS_YCG1]);
		else
			RwD3D9SetPixelShader(simplePS);

		RwRGBAReal mat;
		RwRGBARealFromRwRGBA(&mat, &inst->material->color);
		RwD3D9SetVertexShaderConstant(LOC_matCol, (void*)&mat, 1);
		RwD3D9SetVertexShaderConstant(LOC_surfProps, &material->surfaceProps, 1);

		D3D9RenderDual(1, header, inst);
		inst++;
	}
}

void
CarPipe::EnvMapPass(RxD3D9ResEntryHeader *header, RpAtomic *atomic)
{
	RwUInt32 src, dst, fog, zwrite, alphatest;
	float envmat[16];
	RxD3D9InstanceData *inst = (RxD3D9InstanceData*)&header[1];

	pipeGetEnvMapMatrix(atomic, envmat);
	RwD3D9SetVertexShaderConstant(LOC_tex, (void*)&envmat, 4);

	RwRenderStateGet(rwRENDERSTATEZWRITEENABLE, &zwrite);
	RwRenderStateGet(rwRENDERSTATEFOGENABLE, &fog);
	RwRenderStateGet(rwRENDERSTATESRCBLEND, &src);
	RwRenderStateGet(rwRENDERSTATEDESTBLEND, &dst);
	RwRenderStateGet(rwRENDERSTATEALPHATESTFUNCTION, &alphatest);

	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);
	RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)FALSE);
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTION, (void*)rwALPHATESTFUNCTIONALWAYS);

	RwD3D9SetVertexShader(vcsVehiclePass2VS);
	RwD3D9SetPixelShader(simplePS);
	RwD3D9SetTexture(reflectionTex, 0);

	int noRefl = CVisibilityPlugins::GetAtomicId(atomic) & 0x6000;

	for(uint32 i = 0; i < header->numMeshes; i++){
		RpMaterial *material = inst->material;
		if(SkipInst(inst)){
			inst++;
			continue;
		}

		int matfx = RpMatFXMaterialGetEffects(material);
		if(!noRefl && matfx == rpMATFXEFFECTENVMAP){
			float envcoeff = 0.5f * RpMatFXMaterialGetEnvMapCoefficient(material);
			if(chromeCheat)
				envcoeff = 1.0f;
			RwD3D9SetVertexShaderConstant(LOC_reflProps, (void*)&envcoeff, 4);
			D3D9Render(header, inst);
		}
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
	RxD3D9ResEntryHeader *header = (RxD3D9ResEntryHeader*)&repEntry[1];
	ShaderSetup((RpAtomic*)object);

	if(header->indexBuffer != NULL)
		RwD3D9SetIndices(header->indexBuffer);
	_rwD3D9SetStreams(header->vertexStream, header->useOffsets);
	RwD3D9SetVertexDeclaration(header->vertexDeclaration);

	if(PipeSwitch == PIPE_NEO){
		NeoDiffusePass(header, (RpAtomic*)object);
		SpecularPass(header, (RpAtomic*)object);
	}else{
		DiffusePass(header, (RpAtomic*)object);
		EnvMapPass(header, (RpAtomic*)object);
	}
	RwD3D9SetTexture(NULL, 1);
	RwD3D9SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	RwD3D9SetTexture(NULL, 2);
	RwD3D9SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(2, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
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


	Memory::Patch(0x5D9FE3 +1, CarPipe::RenderCallback);
	// remove some of the SA vehicle pipe
	Memory::Nop(0x6D64E3, 6);	// don't set MatFX Env coefficient from Col lighting
	Memory::Nop(0x4C8899, 5);	// don't set MatFX Env coefficient to 0.25

	Memory::Nop(0x5DA620, 5);	// for all mats CCustomCarEnvMapPipeline::CustomPipeMaterialSetup
//	Memory::Nop(0x53DFCE, 5);	// CCarFXRenderer::PreRenderUpdate - no longer called from our RenderScene
	Memory::Patch<uint8>(0x5D9900, 0xCC);	// disable CCustomCarEnvMapPipeline::CustomPipeRenderCB

	if(DebugMenuLoad()){
		DebugMenuEntry *e;

		static const char *pipenames[] = { "Neo", "Leeds" };
		e = DebugMenuAddVar("Rendering|Vehicle Pipeline", "Style", &CarPipe::PipeSwitch, NULL, 1, 0, 1, pipenames);
		DebugMenuEntrySetWrap(e, true);

		DebugMenuAddCmd("Rendering|Vehicle Pipeline", "Reload Neo tweak table", CarPipe::LoadTweakingTable);

		DebugMenuAddVarBool32("Rendering|Vehicle Pipeline", "Chrome cars", &chromeCheat, NULL);
		DebugMenuAddVarBool32("Rendering|Vehicle Pipeline", "Draw Env Map Coronas", &drawEnvCoronas, NULL);
		DebugMenuAddVarBool32("Rendering|Vehicle Pipeline", "Show Env Map", &debugEnvTex, NULL);
		DebugMenuAddVarBool32("Rendering|Vehicle Pipeline", "Update Env Map", &updateEnvMap, NULL);
	}

});
