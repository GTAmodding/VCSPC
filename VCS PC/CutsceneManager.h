#ifndef __CUTSCENEMGR
#define __CUTSCENEMGR

enum eCutscenes
{
	CUT_JERA1,
	CUT_JERA2,
	CUT_JERA3,
	CUT_LOUA1,
	CUT_LOUA2,
	CUT_LOUA3,
	CUT_MARA1,
	CUT_MARA2,
	CUT_MARA5,
	CUT_PHILA1,
	CUT_PHILA2,
	CUT_PHILA3,
	CUT_PHILA4,
	NUM_CUTSCENES
};

struct CutsceneData
{
	const char*		pName;
	unsigned int	nIndex;
};

class CCutsceneMgr
{
public:
	static bool &ms_running;
	static bool &ms_cutsceneProcessing;

	static void		Initialise(void);
	static void		Update(void);
	static void		SetProperCSPlrModel(char* pModel);

};

#endif