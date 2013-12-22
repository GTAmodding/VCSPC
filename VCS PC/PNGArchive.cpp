#include "StdAfx.h"

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
				struct
				{
					DWORD	dwOffsetToTexture;
					char	TextureName[16];
				}	ArchiveEntry;

				fread(&ArchiveEntry, 20, 1, m_hArchiveHandle);
				if ( !_strnicmp(ArchiveEntry.TextureName, pTexName, 16) )
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
					RwImage*	pImage = RtPNGImageRead(reinterpret_cast<RwChar*>(&StreamStruct));
					RwRaster*	pRaster;		

					RwImageFindRasterFormat(pImage, rwRASTERTYPETEXTURE, &dwWidth, &dwHeight, &dwDepth, &dwFlags);
					pRaster = RwRasterCreate(dwWidth, dwHeight, dwDepth, dwFlags);
					RwRasterSetFromImage(pRaster, pImage);

					pTexture = RwTextureCreate(pRaster);
					RwTextureSetName(pTexture, pTexName);
					RwImageDestroy(pImage);

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