#include "StdAfx.h"
#include "YCoCg.h"

#include "Rs.h"

RwInt32 gYCoCgPluginOffset;

void* YCoCgCreatePixelShader()
{
	return RwD3D9CreatePixelShaderFromFile("gen_YCoCg");
}

static void* YCoCgPluginCtor(void *object, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
	YCOCGPLUGINDATA(object)->bIsYCoCg = FALSE;
	return object;
}

static void* YCoCgPluginDtor(void *object, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
	return object;
}

static void* YCoCgPluginCopy(void *dstObject, const void *srcObject, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
	YCOCGPLUGINDATA(dstObject)->bIsYCoCg = YCOCGPLUGINDATACONST(srcObject)->bIsYCoCg;
	return dstObject;
}

static RwStream* YCoCgPluginRead(RwStream *stream, RwInt32 binaryLength, void *object, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
	DWORD	dwBuf;
	RwStreamRead(stream, &dwBuf, sizeof(dwBuf));
	YCOCGPLUGINDATA(object)->bIsYCoCg = dwBuf == MAKEFOURCC('G', 'C', 'C', 'Y');
	return stream;
}

static RwStream* YCoCgPluginWrite(RwStream *stream, RwInt32 binaryLength, const void *object, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
	if ( YCOCGPLUGINDATA(object)->bIsYCoCg )
	{
		const DWORD		dwBuf = MAKEFOURCC('G', 'C', 'C', 'Y');
		RwStreamWrite(stream, &dwBuf, sizeof(dwBuf));						
	}
	return stream;
}

static RwInt32 YCoCgPluginGetSize(const void *object, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
	return YCOCGPLUGINDATACONST(object)->bIsYCoCg ? sizeof(DWORD) : -1;
}

BOOL YCoCgPluginAttach()
{
	gYCoCgPluginOffset = RwTextureRegisterPlugin(sizeof(YCoCgPlugin), MAKEFOURCC('F', 'G', 'A', 'M'),
							YCoCgPluginCtor, YCoCgPluginDtor, YCoCgPluginCopy);

	RwTextureRegisterPluginStream(MAKEFOURCC('F', 'G', 'A', 'M'),
							YCoCgPluginRead, YCoCgPluginWrite, YCoCgPluginGetSize);

	return gYCoCgPluginOffset > 0;
}