#ifndef __FILEMGR
#define __FILEMGR

#define SECTION3(line, a, b, c) ((line)[0] == (a) && (line)[1] == (b) && (line)[2] == (c))
#define SECTION4(line, a, b, c, d) ((line)[0] == (a) && (line)[1] == (b) && (line)[2] == (c) && (line)[3] == (d))
#define SECTION5(line, a, b, c, d, e) ((line)[0] == (a) && (line)[1] == (b) && (line)[2] == (c) && (line)[3] == (d) && (line)[4] == (e))
#define SECTION7(line, a, b, c, d, e, f, g) ((line)[0] == (a) && (line)[1] == (b) && (line)[2] == (c) && (line)[3] == (d) && (line)[4] == (e) && (line)[5] == (f) && (line)[6] == (g))

class CFileMgr
{
public:
	static inline FILE*		OpenFile(const char* path, const char* mode)
				{ return fopen(path, mode); }
	static inline FILE*		OpenFileW(const wchar_t* path, const wchar_t* mode)
				{ return _wfopen(path, mode); }
	static inline int		CloseFile(FILE* stream)
				{ return fclose(stream); }
	static inline bool		ReadLine(FILE* stream, char* str, int num)
				{ return fgets(str, num, stream) != nullptr; }
	static inline size_t	Read(FILE* stream, void* buf, size_t len)
				{ return fread(buf, 1, len, stream); }
	static inline size_t	Write(FILE* stream, const void* ptr, size_t len)
				{ return fwrite(ptr, 1, len, stream); }
	static inline bool		Seek(FILE* stream, long pos, int from)
				{ return fseek(stream, pos, from) != 0; }

	static void				SetDirMyDocuments();
	static void				SetDir(const char* pDir);

	static wchar_t*			GetMyDocumentsDir();
	static char*			GetMyDocumentsDirMB();
};

// Now uses multiple lists
typedef std::vector<std::string>							tFileLoaderList;
typedef std::vector<std::pair<std::string,unsigned char>>	tFileLoaderList_IMG;

// Thought they could fall into the same file...
class CFileLoader
{
private:
	// Custom loading method, so DLCs work well
	// Not sure how it affects the loading speed
	static tFileLoaderList_IMG*	m_pImagesList;
	static tFileLoaderList*		m_pObjectsList;
	static tFileLoaderList*		m_pScenesList;
	static tFileLoaderList*		m_pCollisionsList;

	static tFileLoaderList*		m_pCarcolsList;
	static tFileLoaderList*		m_pVehAudioList;

	// Path overloads for DLC support
	static char					m_cParticlesPath[64];
	static char					m_cPedgrpPath[64];
	static char					m_cPopcyclePath[64];
	static char					m_cTimecycPath[64];
	static char					m_cFrontendPath[64];
	static char					m_cP2dfxPath[64];

	static unsigned char		m_bCurrentEncryptionType;

private:
	static inline void			BeginLevelLists()
		{	if ( !m_pImagesList ) m_pImagesList = new tFileLoaderList_IMG;
			if ( !m_pObjectsList )  m_pObjectsList = new tFileLoaderList;
			if ( !m_pScenesList )  m_pScenesList = new tFileLoaderList;
			if ( !m_pCollisionsList )  m_pCollisionsList = new tFileLoaderList; }
	static inline void			EndLevelLists()
		{	delete m_pImagesList;
			delete m_pObjectsList;
			delete m_pScenesList;
			delete m_pCollisionsList; }
	static inline void			BeginCarcols()
		{	if ( !m_pCarcolsList ) m_pCarcolsList = new tFileLoaderList; }
	static inline void			EndCarcols()
		{	delete m_pCarcolsList; }
	static inline void			BeginVehAudio()
		{	if ( !m_pVehAudioList ) m_pVehAudioList = new tFileLoaderList; }
	static inline void			EndVehAudio()
		{	delete m_pVehAudioList; }

public:
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
	static inline char*			GetP2dfxPath()
		{ return m_cP2dfxPath; }
	static inline tFileLoaderList*	GetCarcolsList()
		{ return m_pCarcolsList; }
	static inline tFileLoaderList*	GetVehAudioList()
		{ return m_pVehAudioList; }

	static inline void			SetEncryptionType(unsigned char bType)
		{ m_bCurrentEncryptionType = bType; }
	
	// Translates a path with device classes
	static std::string			TranslatePath(const char* pFileName, const char* pDLCName);

	static void					LoadObjectTypes(const char* pFileName);
	static void					LoadScene(const char* pFileName);
	static void					LoadCollisionFile(const char* pFileName, unsigned char bUnk);

	static void					LoadEntryExit(const char* pLine);
	static int					LoadObject(const char* pLine);
	static int					LoadWeaponObject(const char* pLine);
	static const char*			LoadLine(FILE* hFile);
	static void					LoadLevels();
	static bool					ParseLevelFile(const char* pFileName, char* pDLCName);
};

#endif