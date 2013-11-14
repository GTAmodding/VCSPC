#ifndef __CSTREAMING
#define __CSTREAMING

#define FUNC_ReloadCopModels								0x40A150
#define FUNC_CStreaming__RequestModel						0x4087E0
#define FUNC_CStreaming__ReleaseModel						0x409C10
#define FUNC_CStreaming__ReleaseTexture						0x409C70
#define FUNC_CStreaming__RequestSpecialModel				0x409D10
#define FUNC_CStreaming__LoadRequestedModels				0x40EA10
#define FUNC_CStreaming__RandomizeTaxiDriverIDByTown		0x407D50
#define FUNC_CStreaming__ChangeDirectoryToUserDir			0x538860
#define FUNC_CStreaming__RemoveLeastUsedModel				0x40CFD0
#define FUNC_CStreaming__DeleteRwObjectsBehindCamera		0x40D7C0

#define NUM_IMG_FILES		64
#define NUM_STREAMS			(NUM_IMG_FILES+24)
#define MAX_DRAW_DISTANCE	450.0

#pragma message ("TODO: _reloadCopModels is temporary, needs to be reworked.")

struct LoadedObjectInfo
{
	WORD	nextIndex;
	WORD	prevIndev;
	WORD	field_4;
	BYTE	field_6;
	BYTE	field_7;
	DWORD	field_8;
	DWORD	field_C;
	BYTE	bLoaded;
	BYTE	field_11[3];
};

struct CdImage
{
	char		cName[40];
	bool		bNotPlayerIMG;
	HANDLE		hHandle;
};

int		CdStreamAddImage(const char* pName, bool bStandardIMG);

void	InitModelIndices();

class CStreaming
{
public:
	static CdImage	ms_cdImages[NUM_IMG_FILES];

public:
	static void		RequestModel(DWORD index, int a2);
	static void		ReleaseModel(DWORD index);
	static void		ReleaseTexture(DWORD index);
	static void		RequestSpecialModel(int index, const char* pName, int unk);
	static void		LoadRequestedModels(int unk);
	static int		RandomizeTaxiDriverIDByTown();
	static void		ChangeDirectoryToUserDir();
	static bool		RemoveLeastUsedModel(BYTE unkFlag);
	static void		DeleteRwObjectsBehindCamera(int memoryUsed);


	static void		RequestSpecialDriverModel(WORD carModel);
	static void		ReleaseSpecialDriverModel(WORD carModel);
	static int		GetSpecialDriverModelID(WORD carModel);
	static void		MakeSpaceFor(int neededMemory);
	static void		LoadCdDirectory();
	static void		LoadCdDirectory(const char* pArchiveName, long nArchiveIndex, CBlowFish* pEncryption);
};

static_assert(sizeof(LoadedObjectInfo) == LoadedObjectInfo_ARRAYSIZE, "LoadedObjectInfo struct has wrong size!");

#endif