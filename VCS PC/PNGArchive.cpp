#include "StdAfx.h"

static RwBool SPTAClose(void* data)
{
	// Unused
	UNREFERENCED_PARAMETER(data);
	return TRUE;
}

static RwUInt32 SPTARead(void* data, void* buffer, RwUInt32 length)
{
	return fread(buffer, length, 1, (FILE*)data);
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
	return fseek((FILE*)data, offset, SEEK_CUR);
}

void CPNGArchive::SetDirectory(const char* pDirName)
{
	if ( hArchiveHandle )
	{
		/*if ( pDirName )
			strncpy(cCurrentDir, pDirName, 16);
		else
			cCurrentDir[0] = '\0';*/

		rewind(hArchiveHandle);

		// First dir is always the root dir
		int					dwRootDirOffset;
		int					dwCurrentOffset = 4;
		bool				bReadingFirstDir = true;
		fread(&dwRootDirOffset, 4, 1, hArchiveHandle);
		if ( pDirName && dwRootDirOffset != 4)
		{
			while ( dwRootDirOffset > ftell(hArchiveHandle) )
			{
				DWORD		dwCurrentDirOffset;
				char		DirectoryName[16];

				if ( !bReadingFirstDir )
				{
					fread(&dwCurrentDirOffset, 4, 1, hArchiveHandle);
					dwCurrentOffset += 4;
				}
				else
				{
					dwCurrentDirOffset = dwRootDirOffset;
					bReadingFirstDir = false;
				}
				fseek(hArchiveHandle, dwCurrentDirOffset, SEEK_SET);
				fread(DirectoryName, 16, 1, hArchiveHandle);

				if ( !_strnicmp(DirectoryName, pDirName, 16) )
					return;

				fseek(hArchiveHandle, dwCurrentOffset, SEEK_SET);
			}
		}
		else
			fseek(hArchiveHandle, dwRootDirOffset+16, SEEK_SET);
	}
}

RwTexture* CPNGArchive::ReadTexture(const char* pTextureName)
{
	RwTexture*	pTexture = nullptr;

	if ( hArchiveHandle )
	{
		// Push archive handle position
		DWORD		dwSavedPos = ftell(hArchiveHandle);
	
		RwChar*		pTexName = RsPathnameCreate(pTextureName);
		char*		pExtensionPos = strstr(pTexName, ".spta");
		if ( pExtensionPos )
			*pExtensionPos = '\0';

		WORD		wTexturesInDir;
		struct
		{
			DWORD	dwOffsetToTexture;
			char	TextureName[16];
		}			ArchiveEntry;

		fread(&wTexturesInDir, 2, 1, hArchiveHandle);

		for ( WORD i = 0; i < wTexturesInDir; ++ i )
		{
			fread(&ArchiveEntry, 20, 1, hArchiveHandle);
			if ( !_strnicmp(ArchiveEntry.TextureName, pTexName, 16) )
			{
				fseek(hArchiveHandle, ArchiveEntry.dwOffsetToTexture, SEEK_SET);

				// Patch RtPNGImageRead
				*(BYTE*)0x7CF9CA = rwSTREAMCUSTOM;

				RwStreamCustom		StreamStruct;

				StreamStruct.sfnclose = SPTAClose;
				StreamStruct.sfnread = SPTARead;
				StreamStruct.sfnskip = SPTASkip;
				StreamStruct.sfnwrite = SPTAWrite;
				StreamStruct.data = hArchiveHandle;

				// Load it
				RwInt32		dwWidth, dwHeight, dwDepth, dwFlags;
				RwImage*	pImage = RtPNGImageRead((const RwChar*)&StreamStruct);
				RwRaster*	pRaster;		

				RwImageFindRasterFormat(pImage, rwRASTERTYPETEXTURE, &dwWidth, &dwHeight, &dwDepth, &dwFlags);
				pRaster = RwRasterCreate(dwWidth, dwHeight, dwDepth, dwFlags);
				RwRasterSetFromImage(pRaster, pImage);

				pTexture = RwTextureCreate(pRaster);
				RwTextureSetName(pTexture, pTexName);
				RwImageDestroy(pImage);

				// Revert RtPNGImageRead
				*(BYTE*)0x7CF9CA = rwSTREAMFILENAME;
				break;
			}
		}
	
		RsPathnameDestroy(pTexName);

		fseek(hArchiveHandle, dwSavedPos, SEEK_SET);
	}
	return pTexture;
}