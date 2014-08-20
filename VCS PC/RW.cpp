#include "StdAfx.h"

WRAPPER RwTexDictionary* RwTexDictionaryGetCurrent() { EAXJMP(0x7F3A90); }
WRAPPER RwTexDictionary* RwTexDictionaryCreate() { EAXJMP(0x7F3600); }
WRAPPER RwTexDictionary* RwTexDictionarySetCurrent(RwTexDictionary* dict) { WRAPARG(dict); EAXJMP(0x7F3A70); }
WRAPPER RwTexture* RwTextureCreate(RwRaster* raster) { WRAPARG(raster); EAXJMP(0x7F37C0); }
WRAPPER RwTexture* RwTextureRead(const RwChar* name, const RwChar* maskName) { WRAPARG(name); WRAPARG(maskName); EAXJMP(0x7F3AC0); }
WRAPPER RwBool RwTextureDestroy(RwTexture* texture) { WRAPARG(texture); EAXJMP(0x7F3820); }
WRAPPER RwTexture* RwTextureSetName(RwTexture* texture, const RwChar* name) { WRAPARG(texture); WRAPARG(name); EAXJMP(0x7F38A0); }
WRAPPER RwRaster* RwRasterCreate(RwInt32 width, RwInt32 height, RwInt32 depth, RwInt32 flags) { WRAPARG(width); WRAPARG(height); WRAPARG(depth); WRAPARG(flags); EAXJMP(0x7FB230); }
WRAPPER RwBool RwRasterDestroy(RwRaster* raster) { WRAPARG(raster); EAXJMP(0x7FB020); }
WRAPPER RpClump* RpClumpCreate() { EAXJMP(0x74A290); }
WRAPPER RwBool RwImageDestroy(RwImage* image) { WRAPARG(image); EAXJMP(0x802740); }
WRAPPER RwImage* RtPNGImageRead(const RwChar* imageName) { WRAPARG(imageName); EAXJMP(0x7CF9B0); }
WRAPPER RwBool RwFrameDestroy(RwFrame* frame) { WRAPARG(frame); EAXJMP(0x7F05A0); }
WRAPPER RwMatrix* RwFrameGetLTM(RwFrame* frame) { WRAPARG(frame); EAXJMP(0x7F0990); }
WRAPPER RwBool RpAtomicDestroy(RpAtomic* atomic) { WRAPARG(atomic); EAXJMP(0x749DC0); }
WRAPPER RwCamera* RwCameraCreate() { EAXJMP(0x7EE4F0); }
WRAPPER RwFrame* RwFrameCreate() { EAXJMP(0x7F0410); }
WRAPPER RwCamera* RwCameraSetProjection(RwCamera* camera, RwCameraProjection projection) { WRAPARG(camera); WRAPARG(projection); EAXJMP(0x7EE3A0); }
WRAPPER void* RwIm3DTransform(RwIm3DVertex* pVerts, RwUInt32 numVerts, RwMatrix* ltm, RwUInt32 flags) { WRAPARG(pVerts); WRAPARG(numVerts); WRAPARG(ltm); WRAPARG(flags); EAXJMP(0x7EF450); }
WRAPPER RwBool RwIm3DRenderIndexedPrimitive(RwPrimitiveType primType, RwImVertexIndex* indices, RwInt32 numIndices) { WRAPARG(primType); WRAPARG(indices); WRAPARG(numIndices); EAXJMP(0x7EF550); }
WRAPPER RwBool RwIm3DRenderLine(RwInt32 vert1, RwInt32 vert2) { WRAPARG(vert1); WRAPARG(vert2); EAXJMP(0x7EF900); }
WRAPPER RwBool RwIm3DEnd() { EAXJMP(0x7EF520); }
WRAPPER RwBool RwMatrixDestroy(RwMatrix* mpMat) { WRAPARG(mpMat); EAXJMP(0x7F2A20); }
WRAPPER RpAtomic* AtomicDefaultRenderCallBack(RpAtomic* atomic) { WRAPARG(atomic); EAXJMP(0x7491C0); }
WRAPPER RwBool RwStreamFindChunk(RwStream *stream, RwUInt32 type, RwUInt32 *lengthOut, RwUInt32 *versionOut) { WRAPARG(stream); WRAPARG(type); WRAPARG(lengthOut); WRAPARG(versionOut); EAXJMP(0x7ED2D0); }
WRAPPER RpLight* RpLightCreate(RwInt32 type) { WRAPARG(type); EAXJMP(0x752110); }
WRAPPER RwMatrix* RwMatrixTranslate(RwMatrix* matrix, const RwV3d* translation, RwOpCombineType combineOp) { WRAPARG(matrix); WRAPARG(translation); WRAPARG(combineOp); EAXJMP(0x7F2450); }
WRAPPER RwMatrix* RwMatrixRotate(RwMatrix* matrix, const RwV3d* axis, RwReal angle, RwOpCombineType combineOp) { WRAPARG(matrix); WRAPARG(axis); WRAPARG(angle); WRAPARG(combineOp); EAXJMP(0x7F1FD0); }
WRAPPER RwFrame* RwFrameRotate(RwFrame* frame, const RwV3d* axis, RwReal angle, RwOpCombineType combine) { WRAPARG(frame); WRAPARG(axis); WRAPARG(angle); WRAPARG(combine); EAXJMP(0x7F1010); }
WRAPPER RwImage* RwImageCreate(RwInt32 width, RwInt32 height, RwInt32 depth) { WRAPARG(width); WRAPARG(height); WRAPARG(depth); EAXJMP(0x8026E0); }
WRAPPER RwImage* RwImageCreateResample(const RwImage* srcImage, RwInt32 width, RwInt32 height) { WRAPARG(srcImage); WRAPARG(width); WRAPARG(height); EAXJMP(0x80CD10); }
WRAPPER RwImage* RwImageAllocatePixels(RwImage* image) { WRAPARG(image); EAXJMP(0x8027A0); }
WRAPPER RwImage* RwImageResample(RwImage* dstImage, const RwImage* srcImage) { WRAPARG(dstImage); WRAPARG(srcImage); EAXJMP(0x80C600); }
WRAPPER RwImage* RwImageWrite(RwImage* image, const RwChar* imageName) { WRAPARG(image); WRAPARG(imageName); EAXJMP(0x803530); }
WRAPPER const RwChar* RwImageSetPath(const RwChar * path) { WRAPARG(path); EAXJMP(0x802EA0); }
WRAPPER RwImage* RwImageResize(RwImage* image, RwInt32 width, RwInt32 height) { WRAPARG(image); WRAPARG(width); WRAPARG(height); EAXJMP(0x802890); }

WRAPPER RwError* RwErrorSet(RwError* code) { WRAPARG(code); EAXJMP(0x808820); }

WRAPPER RwV3d* RwV3dTransformPoints(RwV3d* pointsOut, const RwV3d* pointsIn, RwInt32 numPoints, const RwMatrix* matrix) { WRAPARG(pointsOut); WRAPARG(pointsIn); WRAPARG(numPoints); WRAPARG(matrix); EAXJMP(0x7EDD90); }
WRAPPER RwFrame* RwFrameOrthoNormalize(RwFrame* frame) { WRAPARG(frame); EAXJMP(0x7F1170); }
WRAPPER RwVideoMode* RwEngineGetVideoModeInfo(RwVideoMode* modeinfo, RwInt32 modeIndex) { WRAPARG(modeinfo); WRAPARG(modeIndex); EAXJMP(0x7F2CF0); }
WRAPPER RwInt32 RwEngineGetCurrentVideoMode() { EAXJMP(0x7F2D20); }
WRAPPER RwInt32 RwEngineGetCurrentSubSystem() { EAXJMP(0x7F2C60); }
WRAPPER RwInt32 RwEngineGetNumVideoModes() { EAXJMP(0x7F2CC0); }

WRAPPER RwInt8 RpAnisotGetMaxSupportedMaxAnisotropy() { EAXJMP(0x748F20); }
WRAPPER RwTexture* RpAnisotTextureSetMaxAnisotropy(RwTexture* tex, RwInt8 val) { WRAPARG(tex); WRAPARG(val); EAXJMP(0x748F30); }
WRAPPER RwInt8 RpAnisotTextureGetMaxAnisotropy(RwTexture* tex) { WRAPARG(tex); EAXJMP(0x748F50); }

WRAPPER RwBool RwD3D9ChangeMultiSamplingLevels(RwUInt32 numLevels) { WRAPARG(numLevels); EAXJMP(0x7F8A90); }
WRAPPER RwUInt32 RwD3D9EngineGetMaxMultiSamplingLevels() { EAXJMP(0x7F84E0); }
WRAPPER RwBool RwD3D9SetRenderTarget(RwUInt32 index, RwRaster* raster) { WRAPARG(index); WRAPARG(raster); EAXJMP(0x7F9E90); }

// Reversed RW functions
RwCamera* RwCameraBeginUpdate(RwCamera* camera)
{
	return camera->beginUpdate(camera);
}

RwCamera* RwCameraEndUpdate(RwCamera* camera)
{
	return camera->endUpdate(camera);
}

RwCamera* RwCameraSetViewWindow(RwCamera* camera, const RwV2d* viewWindow)
{
	RwFrame*	pCamFrame = RwCameraGetFrame(camera);

	camera->viewWindow.x = viewWindow->x;
	camera->viewWindow.y = viewWindow->y;
	camera->recipViewWindow.x = 1.0f / viewWindow->x;
	camera->recipViewWindow.y = 1.0f / viewWindow->y;

	if ( pCamFrame )
		RwFrameUpdateObjects(pCamFrame);

	return camera;
}

RwCamera* RwCameraClear(RwCamera* camera, RwRGBA* colour, RwInt32 clearMode)
{
	return RWSRCGLOBAL(stdFunc[rwSTANDARDCAMERACLEAR])(camera, colour, clearMode) != FALSE ? camera : NULL;
}

RwFrame* RwFrameForAllChildren(RwFrame* frame, RwFrameCallBack callBack, void* data)
{
	for ( RwFrame* curFrame = frame->child; curFrame; curFrame = curFrame->next )
	{
		if ( !callBack(curFrame, data) )
			break;
	}
	return frame;
}

RwFrame* RwFrameForAllObjects(RwFrame* frame, RwObjectCallBack callBack, void* data)
{
	for ( RwLLLink* link = rwLinkListGetFirstLLLink(&frame->objectList); link != rwLinkListGetTerminator(&frame->objectList); link = rwLLLinkGetNext(link) )
	{
		if ( !callBack(&rwLLLinkGetData(link, RwObjectHasFrame, lFrame)->object, data) )
			break;
	}

	return frame;
}

RwFrame* RwFrameSetIdentity(RwFrame* frame)
{
	RwMatrixSetIdentity(RwFrameGetMatrix(frame));
	RwFrameUpdateObjects(frame);
	return frame;
}

RwFrame* RwFrameUpdateObjects(RwFrame* frame)
{
	if ( !rwObjectTestPrivateFlags(&frame->root->object, rwFRAMEPRIVATEHIERARCHYSYNCLTM|rwFRAMEPRIVATEHIERARCHYSYNCOBJ) )
		rwLinkListAddLLLink(&RWSRCGLOBAL(dirtyFrameList), &frame->root->inDirtyListLink);

	rwObjectSetPrivateFlags(&frame->root->object, rwObjectGetPrivateFlags(&frame->root->object) | (rwFRAMEPRIVATEHIERARCHYSYNCLTM|rwFRAMEPRIVATEHIERARCHYSYNCOBJ));
	rwObjectSetPrivateFlags(&frame->object, rwObjectGetPrivateFlags(&frame->object) | (rwFRAMEPRIVATESUBTREESYNCLTM|rwFRAMEPRIVATESUBTREESYNCOBJ));
	return frame;
}

RwMatrix* RwMatrixUpdate(RwMatrix* matrix)
{
	matrix->flags &= ~(rwMATRIXTYPEMASK|rwMATRIXINTERNALIDENTITY);
	return matrix;
}

RwImage* RwImageFindRasterFormat(RwImage* ipImage, RwInt32 nRasterType, RwInt32* npWidth, RwInt32* npHeight, RwInt32* npDepth, RwInt32* npFormat)
{
	RwRaster	outRaster;
	if ( RWSRCGLOBAL(stdFunc[rwSTANDARDIMAGEFINDRASTERFORMAT])(&outRaster, ipImage, nRasterType) != FALSE )
	{
		*npFormat = RwRasterGetFormat(&outRaster) | outRaster.cType;
		*npWidth = RwRasterGetWidth(&outRaster);
		*npHeight = RwRasterGetHeight(&outRaster);
		*npDepth = RwRasterGetDepth(&outRaster);
		return ipImage;
	}
	return NULL;
}

RwImage* RwImageSetFromRaster(RwImage* image, RwRaster* raster)
{
	if ( RWSRCGLOBAL(stdFunc[rwSTANDARDIMAGEGETRASTER])(image, raster, 0) != FALSE )
	{
		if ( raster->privateFlags & rwRASTERGAMMACORRECTED )
			image->flags |= rwIMAGEGAMMACORRECTED;
		return image;
	}
	return NULL;
}

RwUInt8* RwRasterLock(RwRaster* raster, RwUInt8 level, RwInt32 lockMode)
{
	RwUInt8*	out;
	return RWSRCGLOBAL(stdFunc[rwSTANDARDRASTERLOCK])(&out, raster, lockMode + (level << 8)) != FALSE ? out : NULL;
}

RwRaster* RwRasterUnlock(RwRaster* raster)
{
	RWSRCGLOBAL(stdFunc[rwSTANDARDRASTERUNLOCK])(NULL, raster, 0);
	return raster;
}

RwInt32 RwRasterGetNumLevels(RwRaster* raster)
{
	if ( RwRasterGetFormat(raster) & rwRASTERFORMATMIPMAP )
	{
		RwInt32		nOutLevels;
		return RWSRCGLOBAL(stdFunc[rwSTANDARDRASTERGETMIPLEVELS])(&nOutLevels, raster, 0) != FALSE ? nOutLevels : -1;
	}
	return 1;
}

RwRaster* RwRasterSetFromImage(RwRaster* raster, RwImage* image)
{
	if ( RWSRCGLOBAL(stdFunc[rwSTANDARDRASTERSETIMAGE])(raster, image, 0) != FALSE )
	{
		if ( image->flags & rwIMAGEGAMMACORRECTED )
			raster->privateFlags |= rwRASTERGAMMACORRECTED;
		return raster;
	}
	return NULL;
}

RwTexture* RwTextureSetRaster(RwTexture* texture, RwRaster* raster)
{
	if ( raster )
		return RWSRCGLOBAL(stdFunc[rwSTANDARDTEXTURESETRASTER])(texture, raster, 0) != FALSE ? texture : NULL;

	texture->raster = NULL;
	return texture;
}

RpClump* RpClumpAddAtomic(RpClump* clump, RpAtomic* atomic)
{
	rwLinkListAddLLLink(&clump->atomicList, &atomic->inClumpLink);
	atomic->clump = clump;
	return clump;
}

RpClump* RpClumpForAllAtomics(RpClump* clump, RpAtomicCallBack callback, void* pData)
{
	for ( RwLLLink* link = rwLinkListGetFirstLLLink(&clump->atomicList); link != rwLinkListGetTerminator(&clump->atomicList); link = rwLLLinkGetNext(link) )
	{
		if ( !callback(rwLLLinkGetData(link, RpAtomic, inClumpLink), pData) )
			break;
	}
	return clump;
}

RpClump* RpClumpRender(RpClump* clump)
{
	RpClump*	retClump = clump;

	for ( RwLLLink* link = rwLinkListGetFirstLLLink(&clump->atomicList); link != rwLinkListGetTerminator(&clump->atomicList); link = rwLLLinkGetNext(link) )
	{
		RpAtomic* curAtomic = rwLLLinkGetData(link, RpAtomic, inClumpLink);
		if ( RpAtomicGetFlags(curAtomic) & rpATOMICRENDER )
		{
			// Not sure why they need this
			RwFrameGetLTM(RpAtomicGetFrame(curAtomic));
			if ( !RpAtomicRender(curAtomic) )
				retClump = NULL;
		}
	}
	return retClump;
}

RpGeometry* RpGeometryForAllMaterials(RpGeometry* geometry, RpMaterialCallBack fpCallBack, void* pData)
{
	for ( RwInt32 i = 0, j = geometry->matList.numMaterials; i < j; i++ )
	{
		if ( !fpCallBack(geometry->matList.materials[i], pData) )
			break;
	}
	return geometry;
}

RwInt32 RpHAnimIDGetIndex(RpHAnimHierarchy* hierarchy, RwInt32 ID)
{
	for ( RwInt32 i = 0, j = hierarchy->numNodes; i < j; i++ )
	{
		if ( ID == hierarchy->pNodeInfo[i].nodeID )
			return i;
	}
	return -1;
}

RwMatrix* RpHAnimHierarchyGetMatrixArray(RpHAnimHierarchy* hierarchy)
{
	return hierarchy->pMatrixArray;
}

RpLight* RpLightSetColor(RpLight* light, const RwRGBAReal* color)
{
	light->color.red = color->red;
	light->color.green = color->green;
	light->color.blue = color->blue;
	light->color.alpha = color->alpha;
	if ( color->red == color->green && color->red == color->blue )
		rwObjectSetPrivateFlags(light, rpLIGHTPRIVATENOCHROMA); 
	else
		rwObjectSetPrivateFlags(light, 0); 

	return light;
}

RwInt32 _rwerror(RwInt32 code, ...)
{
	return code;
}

void _rwObjectHasFrameSetFrame(void* object, RwFrame* frame)
{
	if ( rwObjectGetParent(object) )
		 rwLinkListRemoveLLLink(&((RwObjectHasFrame*)object)->lFrame);

	rwObjectSetParent(object, frame);
	if ( frame )
	{
		rwLinkListAddLLLink(&frame->objectList, &((RwObjectHasFrame*)object)->lFrame);
		RwFrameUpdateObjects(frame);
	}
}

// This is our own RwEngineInstance handle
void* RwEngineInstance;

// Wrapped some RwEngine functions in order to be able to obtain the handle from the game
// These are injected from InjectDelayedPatches
static RwBool (*varRwEngineOpen)(RwEngineOpenParams*);
static RwBool RwEngineOpen_VCS(RwEngineOpenParams *initParams)
{
	RwBool result = varRwEngineOpen(initParams);
	RwEngineInstance = *(void**)0xC97B24;
	return result;
}

static RwBool (*varRwEngineClose)();
static RwBool RwEngineClose_VCS()
{
	RwBool result = varRwEngineClose();
	RwEngineInstance = *(void**)0xC97B24;
	return result;
}

static RwBool (*varRwEngineInit)(const RwMemoryFunctions*, RwUInt32, RwUInt32);
static RwBool RwEngineInit_VCS(const RwMemoryFunctions *memFuncs, RwUInt32 initFlags, RwUInt32 resArenaSize)
{
	RwBool result = varRwEngineInit(memFuncs, initFlags, resArenaSize);
	RwEngineInstance = *(void**)0xC97B24;
	return result;
}

void InjectRwEngineWrappers()
{
	varRwEngineOpen = (RwBool(*)(RwEngineOpenParams*))(*(int*)0x619CF5 + 0x619CF4 + 5);
	varRwEngineClose = (RwBool(*)())(*(int*)0x6195E6 + 0x6195E5 + 5);
	varRwEngineInit = (RwBool(*)(const RwMemoryFunctions*, RwUInt32, RwUInt32))(*(int*)0x619C9F + 0x619C9E + 5);

	Memory::InjectHook(0x619CF4, RwEngineOpen_VCS);
	Memory::InjectHook(0x6195E5, RwEngineClose_VCS);
	Memory::InjectHook(0x619D1F, RwEngineClose_VCS);
	Memory::InjectHook(0x619C9E, RwEngineInit_VCS);
}