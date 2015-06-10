#include "StdAfx.h"
#include "YCoCg.h"

#include "Rs.h"

RwInt32 gYCoCgPluginOffset;

void YCoCgCreatePixelShaders(void* pGenericShaders[])
{
	pGenericShaders[GEN_PS_YCG1] = RwD3D9CreatePixelShaderFromFile("gen_YCG1");
	pGenericShaders[GEN_PS_YCG2] = RwD3D9CreatePixelShaderFromFile("gen_YCG2");
}

static void* YCoCgPluginCtor(void *object, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
	YCOCGPLUGINDATA(object)->bYCoCgType = 0;
	return object;
}

/*static void* YCoCgPluginDtor(void *object, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
	return object;
}*/

static void* YCoCgPluginCopy(void *dstObject, const void *srcObject, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
	YCOCGPLUGINDATA(dstObject)->bYCoCgType = YCOCGPLUGINDATACONST(srcObject)->bYCoCgType;
	return dstObject;
}

static RwStream* YCoCgPluginRead(RwStream *stream, RwInt32 binaryLength, void *object, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
	DWORD	dwBuf;
	RwStreamRead(stream, &dwBuf, sizeof(dwBuf));
	if ( dwBuf == MAKEFOURCC('1', 'G', 'C', 'Y') )
		YCOCGPLUGINDATA(object)->bYCoCgType = 1;
	else if ( dwBuf == MAKEFOURCC('2', 'G', 'C', 'Y') )
		YCOCGPLUGINDATA(object)->bYCoCgType = 2;
	return stream;
}

static RwStream* YCoCgPluginWrite(RwStream *stream, RwInt32 binaryLength, const void *object, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
	if ( YCOCGPLUGINDATA(object)->bYCoCgType == 1 )
	{
		const DWORD		dwBuf = MAKEFOURCC('1', 'G', 'C', 'Y');
		RwStreamWrite(stream, &dwBuf, sizeof(dwBuf));						
	}
	else if ( YCOCGPLUGINDATA(object)->bYCoCgType == 2 )
	{
		const DWORD		dwBuf = MAKEFOURCC('2', 'G', 'C', 'Y');
		RwStreamWrite(stream, &dwBuf, sizeof(dwBuf));						
	}
	return stream;
}

static RwInt32 YCoCgPluginGetSize(const void *object, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
	return YCOCGPLUGINDATACONST(object)->bYCoCgType != 0 ? sizeof(DWORD) : -1;
}

BOOL YCoCgPluginAttach()
{
	gYCoCgPluginOffset = RwTextureRegisterPlugin(sizeof(YCoCgPlugin), MAKEFOURCC('F', 'G', 'A', 'M'),
							YCoCgPluginCtor, nullptr, YCoCgPluginCopy);

	RwTextureRegisterPluginStream(MAKEFOURCC('F', 'G', 'A', 'M'),
							YCoCgPluginRead, YCoCgPluginWrite, YCoCgPluginGetSize);

	return gYCoCgPluginOffset > 0;
}