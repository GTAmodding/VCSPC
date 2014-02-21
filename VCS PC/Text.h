#ifndef __TEXT__ // Whoa, __TEXT was defined somewhere already :o
#define __TEXT__

enum eLanguages
{
	LANGUAGE_English,
	//LANGUAGE_Spanish,
	LANGUAGE_Polish,
	LANGUAGE_Hungarian,
	NUM_LANGUAGES
};

enum eKeyboardSets
{
	KEYBOARD_QWERTY,
	KEYBOARD_AZERTY,
	KEYBOARD_QWERTZ
};

class CKeyArray
{
private:
	struct
	{
		const char*		text;
		unsigned long	hash;
	}*				data;
	unsigned int	size;

public:
	const char*		Search(const char* pKey, unsigned char* bFound);
};

class CData
{
private:
	void*				data;
	unsigned int		size;
};

class CMissionTextOffsets
{
private:
	struct Entry
	{
		char		name[8];
		void*		offset;
	}				tablEntries[200];
	unsigned int	size;
};

class CText
{
private:
	CKeyArray				tkeyMain;
	CData					tdatMain;
	CKeyArray				tkeyMission;
	CData					tdatMission;
	BYTE					_f20;
	bool					haveTabl;
	bool					cderrorInitialized;
	bool					missionLoaded;
	char					missionName[8];
	char					cderrorText[256];
	CMissionTextOffsets		table;

public:
	const char*				Get(const char* pEntry);
	void					ReloadFontsFiles(bool bKeepMissionText);
	void					Load(bool bKeepMissionText);
	void					Unload(bool bKeepMissionText);
};

static_assert(sizeof(CText) == 0xA90, "Wrong size: CText");

#endif