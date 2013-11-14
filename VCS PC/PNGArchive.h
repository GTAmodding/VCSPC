#ifndef __PNGARCHIVE
#define __PNGARCHIVE

/**** SILENT'S PNG TEXTURE ARCHIVE FORMAT DOCUMENTATION ****

+ 0 - array of absolute offsets to directories (reading is terminated when ftell(file) equals to the first offset)
+ 4*num_directories - array of directories - each entry consists of:
		+ 0 - char[16] - directory name (00's for root directory)
		+ 16 - word - number of textures in the directory
		+ 18 - array of textures - each entry consists of:
				+ 0 - absolute offset to raw PNG data
				+ 4 - char[16] - texture name (without extension)

Next section contains raw PNGs data, one after another (no padding) */

class CPNGArchive
{
private:
	FILE*				hArchiveHandle;

public:
	inline CPNGArchive()
		: hArchiveHandle(nullptr)
	{
	}
	inline CPNGArchive(const char* pFilePath)
		{ OpenArchive(pFilePath); }

	inline ~CPNGArchive()
		{ CloseArchive(); }

	inline void			OpenArchive(const char* pFilePath)
			{  hArchiveHandle = pFilePath[0] ? fopen(pFilePath, "rb") : nullptr; }
	inline void			CloseArchive()
	{ 
		if ( hArchiveHandle )
		{
			fclose(hArchiveHandle);
			hArchiveHandle = nullptr;
		}
	}

	void				SetDirectory(const char* pDirName);
	RwTexture*			ReadTexture(const char* pTextureName);
};

#endif