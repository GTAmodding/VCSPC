#ifndef __GAME
#define __GAME

class CGame
{
public:
	static bool &bMissionPackGame;
	static int32 &currArea;
	static int32 &currLevel;
	static char *aDatFile;

	static bool Initialise(const char *fileName);
	static bool Init1(const char *fileName);
	static bool Init2(const char *fileName);
	static bool Init3(const char *fileName);
	static void Process(void);
};

#endif
