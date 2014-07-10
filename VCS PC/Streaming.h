#ifndef __CSTREAMING
#define __CSTREAMING	

#include "blowfish\Blowfish.h"

#define NUM_IMG_FILES		64
#define NUM_STREAMS			(NUM_IMG_FILES+24)
#define MAX_DRAW_DISTANCE	450.0

enum StreamingModelStatus
{
	StreamingModelNotLoaded = 0,
	StreamingModelLoaded,
	StreamingModelRequested,
	StreamingModelChanneled,
	StreamingModelFinishing,

	StreamingModelForceSize = INT_MAX
};

struct CdImage
{
	char			cName[40];
	bool			bNotPlayerIMG;

	// VCS PC class extension
	unsigned char	bEncryptionType;

	HANDLE			hHandle;
};

int		CdStreamAddImage(const char* pName, bool bStandardIMG, unsigned char bEncryptionType);

void	InitModelIndices();

class CStreamingInfo
{
public:
	short			nNext;
	short			nPrev;
	short			nNextModelOnDisk;
	unsigned char	uRequestFlags;
	unsigned char	uIMGIndex;
	unsigned int	uArchiveOffset;
	unsigned int	uArchiveSize;
	unsigned char	uLoadStatus;
};


class CStreaming
{
public:
	static CStreamingInfo* const	ms_aInfoForModel;
	static CdImage					ms_cdImages[NUM_IMG_FILES];
	static bool						ms_bCopBikeAllowed;

public:
	static void		RequestModel(int nIndex, int nPriority);
	static void		SetModelIsDeletable(int nIndex);
	static void		RequestSpecialModel(int nIndex, const char* pName, int nPriority);
	static void		LoadAllRequestedModels(bool bPriorityOnly);
	static int		RandomizeTaxiDriverIDByTown();
	static bool		RemoveLeastUsedModel(unsigned int nUnknown);
	static void		DeleteRwObjectsBehindCamera(int nMemUsed);
	static void		Init();

	static void		SetModelTxdIsDeletable(int nIndex);
	static void		StreamCopModels(int nTownID);
	static int		ChooseCopModel();
	static int		ChooseCopCarModel(BOOL bIgnoreBikes);
	static void		RequestSpecialDriverModel(WORD carModel);
	static void		ReleaseSpecialDriverModel(WORD carModel);
	static int		GetSpecialDriverModelID(WORD carModel);
	static void		MakeSpaceFor(int neededMemory);
	static void		LoadCdDirectory();
	static void		LoadCdDirectory(const char* pArchiveName, int nArchiveIndex, CBlowFish* pEncryption);
};

static_assert(sizeof(CStreamingInfo) == 0x14, "Wrong size: CStreamingInfo");

#endif