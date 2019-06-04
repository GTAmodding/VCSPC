#ifndef __TEXT__ // Whoa, __TEXT was defined somewhere already :o
#define __TEXT__

// LCID helpers
#define LCID_PRIMARY_LANG(a)	((a) & 0x3FF)
#define LCID_SUBLANG(a)			(((a) >> 10) & 0x3F)

enum
{
	LANGUAGE_English,
	//LANGUAGE_German,
	//LANGUAGE_Spanish,
    LANGUAGE_Italian,
	LANGUAGE_Brazilian,
	LANGUAGE_Polish,
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
	unsigned short	size;
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

	static const char*		GetLanguageAcronymByIndex(unsigned char nLanguage);
	static unsigned char	GetLanguageIndexByAcronym(const char* pAcronym);
};

extern const char*	(__thiscall *CLEOGetTextFunc)(CText*, const char*);
extern CText&				TheText;

static_assert(sizeof(CText) == 0xA90, "Wrong size: CText");

#endif