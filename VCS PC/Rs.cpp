#include "stdafx.h"
#include "Rs.h"

#include "Camera.h"
#include "FxSystem.h"

const DWORD RsGlobalFrameLimits[] = { 0, 25, 30, 50, 60 };

bool& bAnisotSupported = *(bool*)0xC87FFC;

void*		gpCurrentShaderForDefaultCallbacks = nullptr;

WRAPPER RsEventStatus RsEventHandler(RsEvent eventID, void* param) { WRAPARG(eventID); WRAPARG(param); EAXJMP(0x619B60); }
WRAPPER void DoRWStuffEndOfFrame() { EAXJMP(0x53D840); }
WRAPPER void DefinedState2d() { EAXJMP(0x734750); }
WRAPPER void RsCameraShowRaster(RwCamera* pCamera) { EAXJMP(0x745240); }
WRAPPER RpHAnimHierarchy* GetAnimHierarchyFromSkinClump(RpClump* pClump) { WRAPARG(pClump); EAXJMP(0x734A40); }

WRAPPER void* GtaOperatorNew(size_t size) { WRAPARG(size); EAXJMP(0x82119A); }
WRAPPER void GtaOperatorDelete(void* ptr) { WRAPARG(ptr); EAXJMP(0x82413F); }
WRAPPER const char* GetFrameNodeName(RwFrame* pFrame) { WRAPARG(pFrame); EAXJMP(0x72FB30); }

static RpAtomic* GetFirstAtomicCallback(RpAtomic* pAtomic, void* pData)
{
	*static_cast<RpAtomic**>(pData) = pAtomic;
	return nullptr;
}

RpAtomic* AtomicInstanceCB(RpAtomic* pAtomic, void* pData)
{
	UNREFERENCED_PARAMETER(pData);
	RpAtomicInstance(pAtomic);
	return pAtomic;
}

RwChar* RsPathnameCreate(const RwChar* srcBuffer)
{
	size_t		len = strlen(srcBuffer) + 1;
	RwChar*		pMem = static_cast<RwChar*>(malloc(len));

	if ( pMem )
	{
		strncpy(pMem, srcBuffer, len);
		while ( RwChar* pChar = strchr(pMem, '/') )
			*pChar = '\\';
	}
	return pMem;
}

void RsPathnameDestroy(RwChar* buffer)
{
	if ( buffer )
		free(buffer);
}

BOOL RsCameraBeginUpdate(RwCamera* pCamera)
{
	if ( RwCameraBeginUpdate(pCamera) )
		return TRUE;

	RsEventHandler(rsACTIVATE, nullptr);
	return FALSE;
}

RpAtomic* GetFirstAtomic(RpClump* pClump)
{
	RpAtomic* pData = nullptr;
	RpClumpForAllAtomics(pClump, GetFirstAtomicCallback, &pData);
	return pData;
}

void CameraSize(RwCamera* camera, RwRect* rect, float fViewWindow, float fAspectRatio)
{
	if ( !camera )
		return;

	RwVideoMode		vidModeInfo;
	RwRect*			pWorkingRect;
	RwRect			newRect;
	RwRaster*		pFrameBuffer = RwCameraGetRaster(camera);

	RwEngineGetVideoModeInfo(&vidModeInfo, RwEngineGetCurrentVideoMode());

	if ( rect )
		pWorkingRect = rect;
	else
	{
		newRect.x = newRect.y = 0;
		pWorkingRect = &newRect;

		if ( vidModeInfo.flags & rwVIDEOMODEEXCLUSIVE )
		{
			newRect.w = vidModeInfo.width;
			newRect.h = vidModeInfo.height;
		}
		else
		{
			newRect.w = RwRasterGetWidth(pFrameBuffer);
			newRect.h = RwRasterGetHeight(pFrameBuffer);
		}

		if ( RwRasterGetWidth(pFrameBuffer) != pWorkingRect->w || RwRasterGetHeight(pFrameBuffer) != pWorkingRect->h )
		{
			RwRaster*		pZBuffer = RwCameraGetZRaster(camera);

			// omg this is nonstandard
			pFrameBuffer->width = pZBuffer->width = pWorkingRect->w;
			pFrameBuffer->height = pZBuffer->height = pWorkingRect->h;
		}
	}

	RwV2d		viewWindow;

	if ( TheCamera.m_WideScreenOn && !TheCamera.bDontTouchFOVInWidescreen )
	{
		if ( fAspectRatio <= 16.0f/9.0f )
		{
			// Preserve constant width
			viewWindow.x = fViewWindow;
			viewWindow.y = fViewWindow / fAspectRatio;
		}
		else
		{
			// Preserve constant height
			viewWindow.x = fViewWindow * fAspectRatio * (9.0f/16.0f); // Like for 16:9
			viewWindow.y = fViewWindow * (9.0f/16.0f);
		}
	}
	else
	{
		// HOR+ scaling
		viewWindow.x = fViewWindow * fAspectRatio;
		viewWindow.y = fViewWindow;
	}

	RwCameraSetViewWindow(camera, &viewWindow);
	RsGlobal.MaximumWidth = pWorkingRect->w;
	RsGlobal.MaximumHeight = pWorkingRect->h;
}

RwTexture* RwTextureGtaStreamRead(RwStream* stream)
{
	RwUInt32	length, version;
	RwTexture*	pTexture;

	if ( !RwStreamFindChunk(stream, rwID_TEXTURENATIVE, &length, &version) )
		return nullptr;

	if ( !RWSRCGLOBAL(stdFunc[rwSTANDARDNATIVETEXTUREREAD])(stream, &pTexture, length) )
		return nullptr;

#ifdef TRACE_TEXTURE_FILTERMODES
	const char*		pModeNames[] = { "rwFILTERNAFILTERMODE",
    "rwFILTERNEAREST",                /**<Point sampled */
    "rwFILTERLINEAR",                 /**<Bilinear */
    "rwFILTERMIPNEAREST",             /**<Point sampled per pixel mip map */
    "rwFILTERMIPLINEAR",              /**<Bilinear per pixel mipmap */
    "rwFILTERLINEARMIPNEAREST",       /**<MipMap interp point sampled */
	"rwFILTERLINEARMIPLINEAR",        /**<Trilinear */ };

	static bool	bOSAnisot = false;

	if ( !bOSAnisot )
	{
		LogToFile("Max anisot value OS supports: %d", RpAnisotGetMaxSupportedMaxAnisotropy());
		bOSAnisot = true;
	}

	auto	anisotValue = RpAnisotTextureGetMaxAnisotropy(pTexture);

	LogToFile("Texture %s mode: %s max anisot: %d", RwTextureGetName(pTexture), pModeNames[RwTextureGetFilterMode(pTexture)], anisotValue);
#endif

	RwTextureFilterMode		textureFilter = RwTextureGetFilterMode(pTexture);
	unsigned char			setsFilterMode = Fx_c::GetTextureFilteringQuality();
	unsigned char			maxSetsFilterMode = Fx_c::GetMaxTextureFilteringQuality();

	if ( setsFilterMode > maxSetsFilterMode )
		setsFilterMode = maxSetsFilterMode;

	// Remove ancient filtermodes
	if ( textureFilter == rwFILTERNEAREST )
		textureFilter = rwFILTERLINEAR;
	else if ( textureFilter == rwFILTERMIPNEAREST )
		textureFilter = rwFILTERMIPLINEAR;

	// Force filtering by menu settings
	if ( setsFilterMode >= 1 )	// Trilinear or more?
	{
		/*if ( textureFilter == rwFILTERMIPLINEAR )
			textureFilter = rwFILTERLINEARMIPLINEAR;	// Bilinear -> Trilinear*/
	}
	else
	{
		// Bilinear selected in menu
		if ( textureFilter == rwFILTERLINEARMIPLINEAR )
			textureFilter = rwFILTERMIPLINEAR;			// Trilienar -> Bilinear
	}

	RwTextureSetFilterMode(pTexture, textureFilter);

	// Force anisot settings by menu settings
	if ( bAnisotSupported )
	{
		// This texture has anisotropy?
		if ( RpAnisotTextureGetMaxAnisotropy(pTexture) >= 1 )
		{
			// Find real anisot value by menu selection
			switch ( setsFilterMode )
			{
			case 2:
				RpAnisotTextureSetMaxAnisotropy(pTexture, 2);	// 2x
				break;
			case 3:
				RpAnisotTextureSetMaxAnisotropy(pTexture, 4);	// 4x
				break;
			case 4:
				RpAnisotTextureSetMaxAnisotropy(pTexture, 8);	// 8x
				break;
			case 5:
				RpAnisotTextureSetMaxAnisotropy(pTexture, 16);	// 16x
				break;
			default:
				RpAnisotTextureSetMaxAnisotropy(pTexture, 0);	// No anisotropy
				break;
			}			
		}
	}

	return pTexture;
}

void DoPreMenuBlackout()
{
	RwRGBA		color;

	color.red = 0;
	color.green = 0;
	color.blue = 0;
	color.alpha = 255;
	RwCameraClear(Camera, &color, rwCAMERACLEARIMAGE);

	RsCameraShowRaster(Camera);
}

RwBool MyClose(void* data)
{
	return fclose((FILE*)data) == 0;
}

RwUInt32 MyRead(void* data, void* buffer, RwUInt32 length)
{
	return fread(buffer, 1, length, (FILE*)data);
}

RwBool MyWrite(void* data, const void* buffer, RwUInt32 length)
{
	return fwrite(buffer, 1, length, (FILE*)data) == length;
}

RwBool MySkip(void* data, RwUInt32 offset)
{
	return fseek((FILE*)data, offset, SEEK_CUR) == 0;
}

void ConvertAndDumpNativeMesh()
{
	WIN32_FIND_DATA		findData;

	HANDLE	hFoundFiles = FindFirstFile("meshdump\\in\\*.dff", &findData);
	if ( hFoundFiles != INVALID_HANDLE_VALUE )
	{
		do
		{
			RwStream*	pStream;
			RpClump*	pClump = nullptr;

			// Read DFF
			std::string	strDffName = "meshdump\\in\\";
			strDffName += findData.cFileName;

			/*char		path[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, path);

			RwStreamCustom		customReader;

			customReader.data = fopen(strDffName.c_str(), "rb");
			customReader.sfnclose = MyClose;
			customReader.sfnread = MyRead;
			customReader.sfnskip = MySkip;
			customReader.sfnwrite = MyWrite;*/

			pStream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, strDffName.c_str());
			if ( pStream )
			{
				if ( RwStreamFindChunk(pStream, rwID_CLUMP, nullptr, nullptr) )
					pClump = RpClumpStreamRead(pStream);

				RwStreamClose(pStream, nullptr);
				if ( pClump )
				{
					// Instance the model
					RwCameraBeginUpdate(Camera);
					RpClumpForAllAtomics(pClump, AtomicInstanceCB, nullptr);
					RwCameraEndUpdate(Camera);

					strDffName = "meshdump\\out\\";
					strDffName += findData.cFileName;

					pStream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMWRITE, strDffName.c_str());
					if ( pStream )
					{
						RpClumpForAllAtomics(pClump, (RpAtomic*(*)(RpAtomic*,void*))0x4C4F30, nullptr);
						RpAtomicStreamWrite(GetFirstAtomic(pClump), pStream);
						//RpClumpStreamWrite(pClump, pStream);
						RwStreamClose(pStream, nullptr);
					}
					RpClumpDestroy(pClump);
				}
			}
		}
		while ( FindNextFile(hFoundFiles, &findData) );
		FindClose(hFoundFiles);
	}
}

void SetPixelShaderHooked(void* shader)
{
	UNREFERENCED_PARAMETER(shader);
	RwD3D9SetPixelShader(gpCurrentShaderForDefaultCallbacks);
}

void __declspec(naked) rxD3D9VertexShaderDefaultMeshRenderCallBack_Hook()
{
	_asm
	{
		mov		eax, [gpCurrentShaderForDefaultCallbacks] 
		cmp		eax, dword ptr ds:[8E244Ch]	// _rwD3D9LastPixelShaderUsed
		je		rxD3D9VertexShaderDefaultMeshRenderCallBack_Hook_Return
		mov		dword ptr ds:[8E244Ch], eax
		push	eax
		mov		eax, dword ptr ds:[0C97C28h]	// RwD3D9Device
		push	eax
		mov		ecx, [eax]
		call	dword ptr [ecx+1ACh]

rxD3D9VertexShaderDefaultMeshRenderCallBack_Hook_Return:
		push	761053h
		retn
	}
}

void __declspec(naked) rxD3D9DefaultRenderCallback_Hook()
{
	_asm
	{
		mov		ecx, [gpCurrentShaderForDefaultCallbacks] 
		cmp		eax, ecx	// _rwD3D9LastPixelShaderUsed
		je		rxD3D9DefaultRenderCallback_Hook_Return
		mov		dword ptr ds:[8E244Ch], ecx
		push	ecx
		mov		eax, dword ptr ds:[0C97C28h]	// RwD3D9Device
		push	eax
		mov		ecx, [eax]
		call	dword ptr [ecx+1ACh]

rxD3D9DefaultRenderCallback_Hook_Return:
		push	756E17h
		retn
	}
}


static StaticPatcher	Patcher([](){ 
						Memory::InjectHook(0x730E60, &RwTextureGtaStreamRead, PATCH_JUMP);

						// Make _rxD3D9VertexShaderDefaultMeshRenderCallBack use our own pixel shader
						Memory::Patch(0x7CB276, rxD3D9VertexShaderDefaultMeshRenderCallBack_Hook);
						Memory::InjectHook(0x756DFE, rxD3D9DefaultRenderCallback_Hook, PATCH_JUMP);
						//Memory::InjectHook(0x5DA643, SetPixelShaderHooked);
						//Memory::InjectHook(0x5DA736, SetPixelShaderHooked);
									});