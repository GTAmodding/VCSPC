#ifndef __ANIMMANAGER
#define __ANIMMANAGER

struct CAnimationStyleUnkData
{
	int		animID;
	int		unkFrameData;
};

struct CAnimationStyleDescriptor
{
	char						name[16];
	char						IFPName[16];
	DWORD						unk;
	DWORD						animsCount;
	const char**				namesPtr;
	CAnimationStyleUnkData*		unkDataPtr;
	//DWORD*			unkDataPtr;
};

static_assert(sizeof(CAnimationStyleDescriptor) == CAnimationStyleDescriptor_ARRAYSIZE, "CAnimationStyleDescriptor class has wrong size!");

#endif