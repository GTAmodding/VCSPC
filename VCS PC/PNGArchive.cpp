#include "StdAfx.h"
#include "PNGArchive.h"

static RwBool SPTAClose(void* data)
{
	// Unused
	UNREFERENCED_PARAMETER(data);
	return TRUE;
}

static RwUInt32 SPTARead(void* data, void* buffer, RwUInt32 length)
{
	return fread(buffer, length, 1, static_cast<FILE*>(data));
}

static RwBool SPTAWrite(void* data, const void* buffer, RwUInt32 length)
{
	// Unused
	UNREFERENCED_PARAMETER(data);
	UNREFERENCED_PARAMETER(buffer);
	UNREFERENCED_PARAMETER(length);
	return TRUE;
}

static RwBool SPTASkip(void* data, RwUInt32 offset)
{
	return fseek(static_cast<FILE*>(data), offset, SEEK_CUR);
}

/*static RwBool CacheClose(void* data)
{
	// Unused
	UNREFERENCED_PARAMETER(data);
	return TRUE;
}

static RwUInt32 CacheRead(void* data, void* buffer, RwUInt32 length)
{
	memcpy(buffer, *(void**)data, length);
	*(DWORD*)data += length;
	return length;
}

static RwBool CacheWrite(void* data, const void* buffer, RwUInt32 length)
{
	// Unused
	UNREFERENCED_PARAMETER(data);
	UNREFERENCED_PARAMETER(buffer);
	UNREFERENCED_PARAMETER(length);
	return TRUE;
}

static RwBool CacheSkip(void* data, RwUInt32 offset)
{
	*(DWORD*)data += offset;
	return TRUE;
}

CPNGAccel::CPNGAccel()
	: pCurrentDir(nullptr)
{
	CPNGArchive::NewAccel(this);
}

bool CPNGAccel::CachePNG(CPNGArchive& pArchive, const char* pName)
{
	void*	pMemory = pArchive.ReadToCache(pName);
	
	if ( pMemory )
	{
		m_pCachedPNG[pCurrentDir].AddEntry(pName, pMemory);
		return true;
	}

	return false;
}

RwTexture* CPNGAccel::ReadTexture(const char* pTextureName)
{
	auto		it = m_pCachedPNG[pCurrentDir].pTextures.find(pTextureName);
	
	if ( it != m_pCachedPNG[pCurrentDir].pTextures.end() )
	{
		void*		pPNGBuf = it->second;

		// Patch RtPNGImageRead
		Memory::Patch<BYTE>(0x7CF9CA, rwSTREAMCUSTOM);

		RwStreamCustom		StreamStruct;

		StreamStruct.sfnclose = CacheClose;
		StreamStruct.sfnread = CacheRead;
		StreamStruct.sfnskip = CacheSkip;
		StreamStruct.sfnwrite = CacheWrite;
		StreamStruct.data = &pPNGBuf;

		// Load it
		RwInt32		dwWidth, dwHeight, dwDepth, dwFlags;
		RwImage*	pImage = RtPNGImageRead(reinterpret_cast<RwChar*>(&StreamStruct));
		RwRaster*	pRaster;		

		assert(pImage);
		if ( !pImage )
			return nullptr;
		RwImageFindRasterFormat(pImage, rwRASTERTYPETEXTURE, &dwWidth, &dwHeight, &dwDepth, &dwFlags);
		pRaster = RwRasterCreate(dwWidth, dwHeight, dwDepth, dwFlags);
		assert(pRaster);
		if ( !pRaster )
			return nullptr;
		RwRasterSetFromImage(pRaster, pImage);

		RwTexture*	pTexture = RwTextureCreate(pRaster);
		RwTextureSetName(pTexture, pTextureName);
		RwImageDestroy(pImage);

		// Revert RtPNGImageRead
		Memory::Patch<BYTE>(0x7CF9CA, rwSTREAMFILENAME);

		return pTexture;
	}
	return nullptr;
}*/

void CPNGArchive::SetDirectory(const char* pDirName)
{
	if ( m_hArchiveHandle )
	{
		/*if ( pDirName )
			strncpy(cCurrentDir, pDirName, 16);
		else
			cCurrentDir[0] = '\0';*/

		m_bSetDir = true;
		rewind(m_hArchiveHandle);

		// First dir is always the root dir
		int					dwRootDirOffset;
		int					dwCurrentOffset = 4;
		bool				bReadingFirstDir = true;
		fread(&dwRootDirOffset, 4, 1, m_hArchiveHandle);
		if ( pDirName && dwRootDirOffset != 4)
		{
			while ( dwRootDirOffset > ftell(m_hArchiveHandle) )
			{
				DWORD		dwCurrentDirOffset;
				char		DirectoryName[16];

				if ( !bReadingFirstDir )
				{
					fread(&dwCurrentDirOffset, 4, 1, m_hArchiveHandle);
					dwCurrentOffset += 4;
				}
				else
				{
					dwCurrentDirOffset = dwRootDirOffset;
					bReadingFirstDir = false;
				}
				fseek(m_hArchiveHandle, dwCurrentDirOffset, SEEK_SET);
				fread(DirectoryName, 16, 1, m_hArchiveHandle);

				if ( !_strnicmp(DirectoryName, pDirName, 16) )
					return;

				fseek(m_hArchiveHandle, dwCurrentOffset, SEEK_SET);
			}
		}
		else
			fseek(m_hArchiveHandle, dwRootDirOffset+16, SEEK_SET);
	}
}

RwTexture* CPNGArchive::ReadTexture(const char* pTextureName)
{
	RwTexture*	pTexture = nullptr;	

	if ( m_hArchiveHandle && m_bSetDir && pTextureName )
	{
		if ( pTextureName[0] )
		{
			assert(m_bSetDir == true);

			// Push archive handle position
			DWORD		dwSavedPos = ftell(m_hArchiveHandle);
	
			RwChar*		pTexName = RsPathnameCreate(pTextureName);
			char*		pExtensionPos = strstr(pTexName, ".spta");
			if ( pExtensionPos )
				*pExtensionPos = '\0';

			WORD		wTexturesInDir;

			fread(&wTexturesInDir, 2, 1, m_hArchiveHandle);

			for ( WORD i = 0; i < wTexturesInDir; ++ i )
			{
				SPTAEntryHeader	ArchiveEntry;

				fread(&ArchiveEntry, sizeof(SPTAEntryHeader), 1, m_hArchiveHandle);
				if ( !_strnicmp(ArchiveEntry.TextureName, pTexName, SPTA_TEX_NAME_LEN) )
				{
					fseek(m_hArchiveHandle, ArchiveEntry.dwOffsetToTexture, SEEK_SET);

					// Patch RtPNGImageRead
					Memory::Patch<BYTE>(0x7CF9CA, rwSTREAMCUSTOM);

					RwStreamCustom		StreamStruct;

					StreamStruct.sfnclose = SPTAClose;
					StreamStruct.sfnread = SPTARead;
					StreamStruct.sfnskip = SPTASkip;
					StreamStruct.sfnwrite = SPTAWrite;
					StreamStruct.data = m_hArchiveHandle;

					// Load it
					RwInt32		dwWidth, dwHeight, dwDepth, dwFlags;
					if ( RwImage* pImage = RtPNGImageRead(reinterpret_cast<RwChar*>(&StreamStruct)) )
					{
						RwImageFindRasterFormat(pImage, rwRASTERTYPETEXTURE, &dwWidth, &dwHeight, &dwDepth, &dwFlags);
						if ( RwRaster* pRaster = RwRasterCreate(dwWidth, dwHeight, dwDepth, dwFlags) )
						{
							RwRasterSetFromImage(pRaster, pImage);

							pTexture = RwTextureCreate(pRaster);
							if ( pTexture )
								RwTextureSetName(pTexture, pTexName);
						}
						RwImageDestroy(pImage);
					}

					// Revert RtPNGImageRead
					Memory::Patch<BYTE>(0x7CF9CA, rwSTREAMFILENAME);
					break;
				}
			}
	
			RsPathnameDestroy(pTexName);

			fseek(m_hArchiveHandle, dwSavedPos, SEEK_SET);
		}
	}
	return pTexture;
}

/*void* CPNGArchive::ReadToCache(const char* pTextureName)
{
	void*		pMemory = nullptr;
	if ( m_hArchiveHandle && m_bSetDir && pTextureName )
	{
		if ( pTextureName[0] )
		{
			assert(m_bSetDir == true);

			// Push archive handle position
			DWORD		dwSavedPos = ftell(m_hArchiveHandle);
	
			RwChar*		pTexName = RsPathnameCreate(pTextureName);
			char*		pExtensionPos = strstr(pTexName, ".spta");
			if ( pExtensionPos )
				*pExtensionPos = '\0';

			WORD		wTexturesInDir;

			fread(&wTexturesInDir, 2, 1, m_hArchiveHandle);

			for ( WORD i = 0; i < wTexturesInDir; ++ i )
			{
				SPTAEntryHeader	ArchiveEntry;

				fread(&ArchiveEntry, sizeof(SPTAEntryHeader), 1, m_hArchiveHandle);
				if ( !_strnicmp(ArchiveEntry.TextureName, pTexName, SPTA_TEX_NAME_LEN) )
				{
					fseek(m_hArchiveHandle, ArchiveEntry.dwOffsetToTexture, SEEK_SET);

					pMemory = new char[ArchiveEntry.dwSize];
					fread(pMemory, ArchiveEntry.dwSize, 1, m_hArchiveHandle);
					break;
				}
			}
	
			RsPathnameDestroy(pTexName);

			fseek(m_hArchiveHandle, dwSavedPos, SEEK_SET);
		}
	}
	return pMemory;
}*/