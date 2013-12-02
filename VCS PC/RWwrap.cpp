#include "StdAfx.h"

WRAPPER RwTexDictionary* RwTexDictionaryGetCurrent() { EAXJMP(0x7F3A90); }
WRAPPER RwTexDictionary* RwTexDictionaryCreate() { EAXJMP(0x7F3600); }
WRAPPER RwTexDictionary* RwTexDictionarySetCurrent(RwTexDictionary* dict) { WRAPARG(dict); EAXJMP(0x7F3A70); }

// TODO: Sort these wrappers
RpClump* RpClumpForAllAtomics(RpClump* clump, RpAtomicCallBack callback, void *pData)
{
	DWORD		dwFunc = (DWORD)RW_RpClumpForAllAtomics;
	RpClump*	pResult;
	_asm
	{
		push	pData
		push	callback
		push	clump
		call	dwFunc
		mov		pResult, eax
		add		esp, 0Ch
	}
	return pResult;
}

RpGeometry* RpGeometryForAllMaterials(RpGeometry* geometry, RpMaterialCallBack fpCallBack, void *pData)
{
	DWORD		dwFunc = (DWORD)RW_RpGeometryForAllMaterials;
	RpGeometry*	pResult;
	_asm
	{
		push	pData
		push	fpCallBack
		push	geometry
		call	dwFunc
		mov		pResult, eax
		add		esp, 0Ch
	}
	return pResult;
}

RwTexture* RwTextureCreate(RwRaster* raster)
{
	DWORD		dwFunc = (DWORD)RW_RwTextureCreate;
	RwTexture*	pResult;
	_asm
	{
		push	raster
		call	dwFunc
		mov		pResult, eax
		add		esp, 4
	}
	return pResult;
}

RwTexture* RwTextureRead(const RwChar* name, const RwChar* maskName)
{
	DWORD		dwFunc = (DWORD)RW_RwTextureRead;
	RwTexture*	pResult;
	_asm
	{
		push	maskName
		push	name
		call	dwFunc
		mov		pResult, eax
		add		esp, 8
	}
	return pResult;
}

RwBool RwTextureDestroy(RwTexture* texture)
{
	DWORD		dwFunc = (DWORD)RW_RwTextureDestroy;
	_asm
	{
		push	texture
		call	dwFunc
		add		esp, 4
	}
	return TRUE;
}

RwTexture* RwTextureSetRaster(RwTexture* texture, RwRaster* raster)
{
	DWORD		dwFunc = (DWORD)RW_RwTextureSetRaster;
	RwTexture*	pResult;
	_asm
	{
		push	raster
		push	texture
		call	dwFunc
		mov		pResult, eax
		add		esp, 8
	}
	return pResult;
}

RwTexture* RwTextureSetName(RwTexture* texture, const RwChar* name)
{
	DWORD		dwFunc = RW_RwTextureSetName;
	RwTexture*	pResult;
	_asm
	{
		push	name
		push	texture
		call	dwFunc
		mov		pResult, eax
		add		esp, 8
	}
	return pResult;
}

RwRaster* RwRasterCreate(RwInt32 width, RwInt32 height, RwInt32 depth, RwInt32 flags)
{
	DWORD		dwFunc = (DWORD)RW_RwRasterCreate;
	RwRaster*	pResult;
	_asm
	{
		push	flags
		push	depth
		push	height
		push	width
		call	dwFunc
		mov		pResult, eax
		add		esp, 10h
	}
	return pResult;
}

RwBool RwRasterDestroy(RwRaster* raster)
{
	DWORD	dwFunc = (DWORD)RW_RwRasterDestroy;
	RwBool	bResult;
	_asm
	{
		push	raster
		call	dwFunc
		mov		bResult, eax
		add		esp, 4
	}
	return bResult;
}

RwUInt8* RwRasterLock(RwRaster* raster, RwUInt8 level, RwInt32 lockMode)
{
	DWORD		dwFunc = (DWORD)RW_RwRasterLock;
	RwUInt8*	pResult;
	_asm
	{
		push	lockMode
		movzx	eax, level
		push	eax
		push	raster
		call	dwFunc
		mov		pResult, eax
		add		esp, 0Ch
	}
	return pResult;
}

RwRaster* RwRasterUnlock(RwRaster* raster)
{
	DWORD		dwFunc = (DWORD)RW_RwRasterUnlock;
	RwRaster*	pResult;
	_asm
	{
		push	raster
		call	dwFunc
		mov		pResult, eax
		add		esp, 4
	}
	return pResult;
}

RwRaster* RwRasterSetFromImage(RwRaster* raster, RwImage* image)
{
	DWORD		dwFunc = RW_RwRasterSetFromImage;
	RwRaster*	pResult;
	_asm
	{
		push	image
		push	raster
		call	dwFunc
		mov		pResult, eax
		add		esp, 8
	}
	return pResult;
}

RpClump* RpClumpAddAtomic(RpClump* clump, RpAtomic* atomic)
{
	DWORD		dwFunc = (DWORD)RW_RpClumpAddAtomic;
	RpClump*	pResult;
	_asm
	{
		push	atomic
		push	clump
		call	dwFunc
		mov		pResult, eax
		add		esp, 8
	}
	return pResult;
}

RwFrame* RwFrameSetIdentity(RwFrame* frame)
{
	DWORD		dwFunc = (DWORD)RW_RwFrameSetIdentity;
	RwFrame*	pResult;
	_asm
	{
		push	frame
		call	dwFunc
		mov		pResult, eax
		add		esp, 4
	}
	return pResult;
}

RpClump* RpClumpCreate(void)
{
	DWORD		dwFunc = (DWORD)RW_RpClumpCreate;
	RpClump*	pResult;
	_asm
	{
		call	dwFunc
		mov		pResult, eax
	}
	return pResult;
}

RwCamera* RwCameraClear(RwCamera* camera, RwRGBA* colour, RwInt32 clearMode)
{
	DWORD		dwFunc = (DWORD)RW_RwCameraClear;
	RwCamera*	pResult;
	_asm
	{
		push	clearMode
		push	colour
		push	camera
		call	dwFunc
		mov		pResult, eax
		add		esp, 0Ch
	}
	return pResult;
}

RwBool RwImageDestroy(RwImage* image)
{
	DWORD	dwFunc = RW_RwImageDestroy;
	RwBool	bResult;
	_asm
	{
		push	image
		call	dwFunc
		mov		bResult, eax
		add		esp, 4
	}
	return bResult;
}

RwImage* RwImageFindRasterFormat(RwImage* ipImage, RwInt32 nRasterType, RwInt32* npWidth, RwInt32* npHeight, RwInt32* npDepth, RwInt32* npFormat)
{
	DWORD		dwFunc = RW_RwImageFindRasterFormat;
	RwImage*	pResult;
	_asm
	{
		push	npFormat
		push	npDepth
		push	npHeight
		push	npWidth
		push	nRasterType
		push	ipImage
		call	dwFunc
		mov		pResult, eax
		add		esp, 18h
	}
	return pResult;
}

WRAPPER RwImage* RtPNGImageRead(const RwChar* imageName) { WRAPARG(imageName); EAXJMP(0x7CF9B0); }
WRAPPER void DoRWStuffEndOfFrame() { EAXJMP(0x53D840); }