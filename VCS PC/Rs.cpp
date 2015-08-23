#include "stdafx.h"
#include "Rs.h"

#include "Camera.h"
#include "FxSystem.h"
#include "Font.h"
#include "YCoCg.h"
#include "Frontend.h"

const DWORD RsGlobalFrameLimits[] = { 0, 25, 30, 50, 60 };

bool& bAnisotSupported = *(bool*)0xC87FFC;
RwPluginRegistry& textureTKList = *(RwPluginRegistry*)0x8E23CC;

bool bVerticalSync = true;

void*		gpPixelShaderForDefaultCallbacks = nullptr;

WRAPPER RsEventStatus RsEventHandler(RsEvent eventID, void* param) { WRAPARG(eventID); WRAPARG(param); EAXJMP(0x619B60); }
WRAPPER void DoRWStuffEndOfFrame() { EAXJMP(0x53D840); }
WRAPPER void DefinedState2d() { EAXJMP(0x734750); }
WRAPPER RpHAnimHierarchy* GetAnimHierarchyFromSkinClump(RpClump* pClump) { WRAPARG(pClump); EAXJMP(0x734A40); }

WRAPPER void* GtaOperatorNew(size_t size) { WRAPARG(size); EAXJMP(0x82119A); }
WRAPPER void GtaOperatorDelete(void* ptr) { WRAPARG(ptr); EAXJMP(0x82413F); }
WRAPPER const char* GetFrameNodeName(RwFrame* pFrame) { WRAPARG(pFrame); EAXJMP(0x72FB30); }

WRAPPER RwTexture* RwTexDictionaryFindHashNamedTexture(RwTexDictionary* pDict, unsigned int dwHash) { EAXJMP(0x734E50); }

static RpAtomic* GetFirstAtomicCallback(RpAtomic* pAtomic, void* pData)
{
	*static_cast<RpAtomic**>(pData) = pAtomic;
	return nullptr;
}

static RwObject* GetFirstObjectCallback(RwObject* pObject, void* pData)
{
	*static_cast<RwObject**>(pData) = pObject;
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

RwObject* GetFirstObject(RwFrame* pFrame)
{
	RwObject*	pObject = nullptr;
	RwFrameForAllObjects(pFrame, GetFirstObjectCallback, &pObject);
	return pObject;
}

BOOL RsRwInitialize(void* pRect)
{
	// Call SA RsRwInitialize
	if ( ((BOOL(*)(void*))0x619C90)(pRect) )
	{
		if ( RsGenericShadersInit() )
			return TRUE;
		
		return FALSE;
	}
	return FALSE;
}

void RsRwTerminate()
{
	RsGenericShadersTerminate();

	// Call SA RsRwTerminate
	((BOOL(*)())0x6195E0)();
}

BOOL PluginAttach()
{
	// Call SA PluginAttach
	if ( !((BOOL(*)())0x53D870)() )
		return FALSE;

	// Attaching custom plugins
	if ( !YCoCgPluginAttach() )
		return FALSE;

	return TRUE;
}

void ToggleVSync(bool bEnable)
{
	bVerticalSync = bEnable;
}

void RsCameraShowRaster(RwCamera* pCamera)
{
	RwCameraShowRaster( pCamera, RsGlobal.ps->window, bVerticalSync ? rwRASTERFLIPWAITVSYNC : rwRASTERFLIPDONTWAIT );
}

RwBool RsSelectDevice()
{
	if ( ((RwBool(*)())0x746190)() != FALSE )
	{
		ToggleVSync( FrontEndMenuManager.m_bAppliedVSync );
		return TRUE;
	}
	return FALSE;
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

	// Read plugins (never done in R* code)
	if ( _rwPluginRegistryReadDataChunks(&textureTKList, stream, pTexture) == nullptr )
		return nullptr;

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
		RwUInt8		bTexMaxAnisot = RpAnisotTextureGetMaxAnisotropy(pTexture);
		if ( bTexMaxAnisot >= 1 )
		{
			// Forced by textures
			switch ( setsFilterMode )
			{
			case 2:
				RpAnisotTextureSetMaxAnisotropy(pTexture, Min<RwInt8>(bTexMaxAnisot, 2));	// 2x
				break;
			case 3:
				RpAnisotTextureSetMaxAnisotropy(pTexture, Min<RwInt8>(bTexMaxAnisot, 4));	// 4x
				break;
			case 4:
				RpAnisotTextureSetMaxAnisotropy(pTexture, Min<RwInt8>(bTexMaxAnisot, 8));	// 8x
				break;
			case 5:
				RpAnisotTextureSetMaxAnisotropy(pTexture, Min<RwInt8>(bTexMaxAnisot, 16));	// 16x
				break;
			default:
				RpAnisotTextureSetMaxAnisotropy(pTexture, 1);	// No anisotropy
				break;
			}			
		}
		else if ( bTexMaxAnisot == 0 )
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
				RpAnisotTextureSetMaxAnisotropy(pTexture, 1);	// No anisotropy
				break;
			}			
		}
	}

	return pTexture;
}

// Shader helpers
static HMODULE thisModule = nullptr;
void* RwD3D9CreatePixelShaderFromResource(WORD wResource)
{
	if ( thisModule == nullptr )
		GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)RwD3D9CreatePixelShaderFromResource, &thisModule);

	void*		pPixelShader;

	HRSRC		resource = FindResource(thisModule, MAKEINTRESOURCE(wResource), RT_RCDATA);
	RwUInt32*	pRawShader = static_cast<RwUInt32*>(LoadResource(thisModule, resource));
	assert(pRawShader != nullptr);
	RwD3D9CreatePixelShader(pRawShader, reinterpret_cast<void**>(&pPixelShader));
	FreeResource(pRawShader);

	return pPixelShader;
}


void* RwD3D9CreateVertexShaderFromResource(WORD wResource)
{
	if ( thisModule == nullptr )
		GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)RwD3D9CreatePixelShaderFromResource, &thisModule);

	void*		pVertexShader;

	HRSRC		resource = FindResource(thisModule, MAKEINTRESOURCE(wResource), RT_RCDATA);
	RwUInt32*	pRawShader = static_cast<RwUInt32*>(LoadResource(thisModule, resource));
	assert(pRawShader != nullptr);
	RwD3D9CreateVertexShader(pRawShader, reinterpret_cast<void**>(&pVertexShader));
	FreeResource(pRawShader);

	return pVertexShader;
}


void* RwD3D9CreatePixelShaderFromFile(const char* pFileName)
{
	sprintf(gString, "data\\shaders\\%sPS.cso", pFileName);

	HANDLE	hFile = CreateFile(gString, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	HANDLE	hMapping = CreateFileMapping(hFile, nullptr, PAGE_READONLY, 0, 0, nullptr);
	void*	pMem = MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);

	assert(pMem != nullptr);
	if ( pMem == nullptr )
		return nullptr;

	void*		pPixelShader;

	RwD3D9CreatePixelShader(static_cast<RwUInt32*>(pMem), &pPixelShader);
	UnmapViewOfFile(pMem);
	CloseHandle(hMapping);
	CloseHandle(hFile);

	return pPixelShader;
}

void* RwD3D9CreateVertexShaderFromFile(const char* pFileName)
{
	sprintf(gString, "data\\shaders\\%sVS.cso", pFileName);

	HANDLE	hFile = CreateFile(gString, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	HANDLE	hMapping = CreateFileMapping(hFile, nullptr, PAGE_READONLY, 0, 0, nullptr);
	void*	pMem = MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);

	assert(pMem != nullptr);
	if ( pMem == nullptr )
		return nullptr;

	void*		pVertexShader;

	RwD3D9CreateVertexShader(static_cast<RwUInt32*>(pMem), &pVertexShader);
	UnmapViewOfFile(pMem);
	CloseHandle(hMapping);
	CloseHandle(hFile);

	return pVertexShader;
}

static void* gpGenericPS[NUM_GEN_PS];

// Generic shaders
BOOL RsGenericShadersInit()
{
	YCoCgCreatePixelShaders(gpGenericPS);
	return TRUE;
}

void RsGenericShadersTerminate()
{
	RwD3D9DeletePixelShader(gpGenericPS[GEN_PS_YCG1]);
	RwD3D9DeletePixelShader(gpGenericPS[GEN_PS_YCG2]);
}

void DoPreMenuBlackout()
{
	RwRGBA		color = { 0, 0, 0, 255 };

	RwCameraClear(Camera, &color, rwCAMERACLEARIMAGE);

	CFont::SetAlphaFade(255.0f);

	RsCameraShowRaster(Camera);
}

static char** pVideoModes = (char**)0xC920D0;
void FreeDisplayModesList()
{
	for ( int i = 0, j = GetNumDisplayModes(); i < j; i++ )
	{
		delete[] pVideoModes[i];
	}
	delete[] pVideoModes;
	pVideoModes = nullptr;
}

int GetNumDisplayModes()
{
	return RwEngineGetNumVideoModes();
}

char** GetDisplayModesList()
{
	static RwInt32 lastSubSystem = -1;
	if ( pVideoModes )
	{
		if ( lastSubSystem == RwEngineGetCurrentSubSystem() )
			return pVideoModes;

		FreeDisplayModesList();
	}
	lastSubSystem = RwEngineGetCurrentSubSystem();

	int i = 0;
	int j = GetNumDisplayModes();
	pVideoModes = new char*[j];

	for ( ; i < j; i++ )
	{
		RwVideoMode videoMode;
		RwEngineGetVideoModeInfo( &videoMode, i );
		if ( videoMode.flags & rwVIDEOMODEEXCLUSIVE )
		{
			pVideoModes[i] = new char[32];
			sprintf( pVideoModes[i], "%ux%ux%u (%uHz)", videoMode.width, videoMode.height, videoMode.depth, videoMode.refRate );
		}
		else
		{
			pVideoModes[i] = nullptr;
		}
	}
	return pVideoModes;
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

void SetUpGeneric_DNPipe(RxD3D9InstanceData* instanceData, RwTexture* texture)
{
	// TEMP
	RwD3D9SetVertexShader(instanceData->vertexShader);

	if ( gpPixelShaderForDefaultCallbacks == nullptr )
	{
		// Is YCoCg texture?
		if ( texture != nullptr && YCOCGPLUGINDATACONST(texture)->bYCoCgType != 0 )
			RwD3D9SetPixelShader(gpGenericPS[YCOCGPLUGINDATACONST(texture)->bYCoCgType == 2 ? GEN_PS_YCG2 : GEN_PS_YCG1]);
		else
			RwD3D9SetPixelShader(nullptr);
	}
}

void SetGenericShaders_InstanceData(RxD3D9InstanceData* pInstanceData)
{
	RwD3D9SetVertexShader(pInstanceData->vertexShader);

	if ( gpPixelShaderForDefaultCallbacks != nullptr )
	{
		RwD3D9SetPixelShader(gpPixelShaderForDefaultCallbacks);
		return;
	}

	RwTexture*	pMaterialTexture = RpMaterialGetTexture(pInstanceData->material);
	if ( pMaterialTexture != nullptr && YCOCGPLUGINDATACONST(pMaterialTexture)->bYCoCgType != 0 )
		RwD3D9SetPixelShader(gpGenericPS[YCOCGPLUGINDATACONST(pMaterialTexture)->bYCoCgType == 2 ? GEN_PS_YCG2 : GEN_PS_YCG1]);
	else
		RwD3D9SetPixelShader(nullptr);
}

void __declspec(naked) rxD3D9VertexShaderDefaultMeshRenderCallBack_Hook()
{
	_asm
	{
		mov		eax, [gpPixelShaderForDefaultCallbacks]
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
		mov		ecx, [gpPixelShaderForDefaultCallbacks] 
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
						using namespace Memory;

						InjectHook(0x730E60, &RwTextureGtaStreamRead, PATCH_JUMP);

						// Default to 0 AF, which will be 'use max'
						Patch<BYTE>(0x74902D, 0);

						// Make _rxD3D9VertexShaderDefaultMeshRenderCallBack use our own pixel shader
						Memory::Patch(0x7CB276, rxD3D9VertexShaderDefaultMeshRenderCallBack_Hook);
						//Memory::InjectHook(0x756DFE, rxD3D9DefaultRenderCallback_Hook, PATCH_JUMP);

						// Generic shaders for default render callbacks
						Patch<BYTE>(0x756E00, 0xEB);

						Patch<DWORD>(0x756FEF, 0x50F0468D);
						InjectHook(0x756FF3, SetGenericShaders_InstanceData, PATCH_CALL);
						Patch<DWORD>(0x756FF8, 0xEB04C483);
						Patch<BYTE>(0x756FFC, 0x14);

						Patch<DWORD>(0x7572C3, 0x50F0458D);
						InjectHook(0x7572C7, SetGenericShaders_InstanceData, PATCH_CALL);
						Patch<DWORD>(0x7572CC, 0xEB04C483);
						Patch<BYTE>(0x7572D0, 0x16);

						// No DirectPlay dependency
						Patch<BYTE>(0x74754A, 0xB8);
						Patch<DWORD>(0x74754B, 0x900);

						InjectHook(0x748E7A, FreeDisplayModesList);
						InjectHook(0x745AF0, GetDisplayModesList, PATCH_JUMP);

						// VSync
						InjectHook(0x619440, RsCameraShowRaster, PATCH_JUMP);

						// Generic shaders
						InjectHook(0x5BF395, RsRwInitialize);
						InjectHook(0x53D93D, RsRwTerminate);
						InjectHook(0x53ECA1, PluginAttach);

						// Temp
						Patch<DWORD>(0x5DA734, 0x0C24748B);
						Patch<DWORD>(0x5DA738, 0x142474FF);
						Patch<BYTE>(0x5DA73C, 0x56);
						InjectHook(0x5DA73D, SetUpGeneric_DNPipe, PATCH_CALL);
						Nop(0x5DA742, 6);
									});