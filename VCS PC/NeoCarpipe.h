#ifndef __NEOCARPIPE
#define __NEOCARPIPE

extern RwTexDictionary *neoTxd;
extern int iCanHasNeoCar;

#define NUMWEATHERS 23


struct Color
{
	float r, g, b, a;
	Color(void) {}
	Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
};

class InterpolatedValue
{
public:
	virtual void Read(char *s, int line, int field) = 0;
	virtual void Copy(int dst, int src) = 0;
};

class InterpolatedFloat : public InterpolatedValue
{
public:
	float data[24][NUMWEATHERS];
	float curInterpolator;
	float curVal;

	InterpolatedFloat(float init);
	void Read(char *s, int line, int field);
	float Get(void);
	void Copy(int dst, int src);
};

class InterpolatedColor : public InterpolatedValue
{
public:
	Color data[24][NUMWEATHERS];
	float curInterpolator;
	Color curVal;

	InterpolatedColor(const Color &init);
	void Read(char *s, int line, int field);
	Color Get(void);
	void Copy(int dst, int src);
};

class InterpolatedLight : public InterpolatedColor
{
public:
	InterpolatedLight(const Color &init) : InterpolatedColor(init) {}
	void Read(char *s, int line, int field);
};

void neoReadWeatherTimeBlock(FILE *file, InterpolatedValue *interp);

class CustomPipe
{
public:
	RxPipeline *rwPipeline;
	RxD3D9AllInOneRenderCallBack originalRenderCB;
	void CreateRwPipeline(void);
	void SetRenderCallback(RxD3D9AllInOneRenderCallBack);
	void Attach(RpAtomic *atomic);
	static RpAtomic *setatomicCB(RpAtomic *atomic, void *data);
};

void CarPipeInit(void);
void neoInit(void);

class CarPipe : public CustomPipe
{
	void CreateShaders(void);

	static void MakeScreenQuad(void);
	static void MakeQuadTexCoords(bool textureSpace);
public:
	static int PipeSwitch;
	static InterpolatedFloat fresnel;
	static InterpolatedFloat power;
	static InterpolatedLight diffColor;
	static InterpolatedLight specColor;
	static void *vertexShaderPass1;
	static void *vertexShaderPass2;
	static void *pixelShader;
	static void *pixelShaderYCG1;
	static void *pixelShaderYCG2;

	static void *vcsVehiclePass1VS;
	static void *vcsVehiclePass2VS;
	static void *simplePS;
	// reflection map
	static RwCamera *reflectionCam;
	static RwTexture *reflectionMask;
	static RwTexture *reflectionTex;
	static RwIm2DVertex screenQuad[4];
	static RwImVertexIndex screenindices[6];

	CarPipe(void);
	void Init(void);
	static void LoadTweakingTable(void);
	static void RenderEnvTex(void);
	static void SetupEnvMap(void);
	static void RenderCallback(RwResEntry *repEntry, void *object, RwUInt8 type, RwUInt32 flags);
	static void ShaderSetup(RpAtomic *atomic);
	static void NeoDiffusePass(RxD3D9ResEntryHeader *header, RpAtomic *atomic);
	static void DiffusePass(RxD3D9ResEntryHeader *header, RpAtomic *atomic);
	static void EnvMapPass(RxD3D9ResEntryHeader *header, RpAtomic *atomic);
	static void SpecularPass(RxD3D9ResEntryHeader *header, RpAtomic *atomic);
};

#endif __NEOCARPIPE
