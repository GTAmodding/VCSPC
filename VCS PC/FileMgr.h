#ifndef __FILEMGR
#define __FILEMGR

#define SECTION3(line, a, b, c) ((line)[0] == (a) && (line)[1] == (b) && (line)[2] == (c))
#define SECTION4(line, a, b, c, d) ((line)[0] == (a) && (line)[1] == (b) && (line)[2] == (c) && (line)[3] == (d))

class CFileMgr
{
public:
	static inline FILE*		OpenFile(const char* path, const char* mode)
				{ return fopen(path, mode); };
	static inline  int		CloseFile(FILE* stream)
				{ return fclose(stream); };
	static inline bool		ReadLine(FILE* stream, char* str, int num)
				{ return fgets(str, num, stream) != nullptr; };
	static inline size_t	Read(FILE* stream, void* buf, size_t len)
				{ return fread(buf, 1, len, stream); };
	static inline bool		Seek(FILE* stream, long pos, int from)
				{ return fseek(stream, pos, from) != 0; };
};

// Now uses multiple lists
typedef std::vector<const std::string>	tFileLoaderList;

// Thought they could fall into the same file...
class CFileLoader
{
private:
	// Custom loading method, so DLCs work well
	// Not sure how it affects the loading speed
	static tFileLoaderList*		m_pImagesList;
	static tFileLoaderList*		m_pObjectsList;
	static tFileLoaderList*		m_pScenesList;
	static tFileLoaderList*		m_pCollisionsList;

	// Path overloads for DLC support
	static char					m_cParticlesPath[64];
	static char					m_cPedgrpPath[64];
	static char					m_cPopcyclePath[64];
	static char					m_cTimecycPath[64];
	static char					m_cFrontendPath[64];

public:
	static inline void			BeginLevelLists()
		{	if ( !m_pImagesList ) m_pImagesList = new tFileLoaderList;
			if ( !m_pObjectsList )  m_pObjectsList = new tFileLoaderList;
			if ( !m_pScenesList )  m_pScenesList = new tFileLoaderList;
			if ( !m_pCollisionsList )  m_pCollisionsList = new tFileLoaderList; }

	static inline void			EndLevelLists()
		{	if ( m_pImagesList ) delete m_pImagesList;
			if ( m_pObjectsList ) delete m_pObjectsList;
			if ( m_pScenesList ) delete m_pScenesList;
			if ( m_pCollisionsList ) delete m_pCollisionsList; }

	static inline char*			GetParticlesPath()
			{ return m_cParticlesPath; }
	static inline char*			GetPedgrpPath()
			{ return m_cPedgrpPath; }
	static inline char*			GetPopcyclePath()
			{ return m_cPopcyclePath; }
	static inline char*			GetTimecycPath()
			{ return m_cTimecycPath; }
	static inline char*			GetFrontendPath()
		{ return m_cFrontendPath; }

	static void					LoadObjectTypes(const char* pFileName);
	static void					LoadScene(const char* pFileName);
	static void					LoadCollisionFile(const char* pFileName, unsigned char bUnk);

	static const char*			LoadLine(FILE* hFile);
	static void					LoadLevels();
	static bool					ParseLevelFile(const char* pFileName, char* pDLCName);
};

#endif