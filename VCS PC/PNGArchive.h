#ifndef __PNGARCHIVE
#define __PNGARCHIVE

/**** SILENT'S PNG TEXTURE ARCHIVE FORMAT DOCUMENTATION ****

+ 0 - array of absolute offsets to directories (reading is terminated when ftell(file) equals the first offset)
+ 4*num_directories - array of directories - each entry consists of:
		+ 0 - char[16] - directory name (00's for root directory)
		+ 16 - word - number of textures in the directory
		+ 18 - array of textures - each entry consists of:
				+ 0 - absolute offset to raw PNG data
				+ 4 - char[16] - texture name (without extension)

Next section contains raw PNGs data, one after another (no padding) */

#define SPTA_TEX_NAME_LEN		16

class CPNGArchive;

struct SPTAEntryHeader
{
	DWORD	dwOffsetToTexture;
	char	TextureName[SPTA_TEX_NAME_LEN];
};

// PNG cache
/*class CPNGAccel
{
private:
	struct Entry
	{
		void*		pBuffer;

		// Automatic destruction
		~Entry() { delete pBuffer; }
	};
	struct Directory
	{
		std::map<std::string,void*>		pTextures;

		~Directory() { for ( auto it = pTextures.begin(); it != pTextures.end(); it++ ) delete it->second;
			pTextures.clear(); }

		inline void			AddEntry(const char* pName, void* pMem)
			{ pTextures[pName] = pMem; }
	};

	const char*									pCurrentDir;
	std::map<std::string,Directory>				m_pCachedPNG;

public:
	~CPNGAccel() { m_pCachedPNG.clear(); }
	CPNGAccel();

	inline void			SetDirectory(const char* pDirName)
		{ pCurrentDir = pDirName; }
	bool				CachePNG(CPNGArchive& pArchive, const char* pName);
	RwTexture*			ReadTexture(const char* pTextureName);
};*/

class CPNGArchive
{
private:
	FILE*								m_hArchiveHandle;
	bool								m_bSetDir;

public:
	inline CPNGArchive()
		: m_hArchiveHandle(nullptr), m_bSetDir(false)
	{}

	inline CPNGArchive(const char* pFilePath)
		{ OpenArchive(pFilePath); }

	inline ~CPNGArchive()
		{ CloseArchive(); }

	inline void			OpenArchive(const char* pFilePath)
			{  m_hArchiveHandle = pFilePath[0] ? fopen(pFilePath, "rb") : nullptr; }

	inline void			CloseArchive()
	{ 
		if ( m_hArchiveHandle )
		{
			fclose(m_hArchiveHandle);
			m_hArchiveHandle = nullptr;
		}
	}

	void				SetDirectory(const char* pDirName);
	RwTexture*			ReadTexture(const char* pTextureName);
	//void*				ReadToCache(const char* pTextureName);

/*	// PNG accel stuff
private:
	static std::vector<CPNGAccel*>		m_pngCache;

public:
	static inline void	Shutdown()
		{ m_pngCache.clear(); }
	static inline void	NewAccel(CPNGAccel* pAccel)
		{ m_pngCache.push_back(pAccel); }*/
};

#endif