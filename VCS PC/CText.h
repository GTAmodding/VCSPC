#ifndef __CTEXT
#define __CTEXT

#define WIN32_LEAN_AND_MEAN

#define	FUNC_CText__GetText							0x6A0050
#define	FUNC_CText__TextNumberFormat				0x69DE90
#define FUNC_CText__Clear							0x69FF20
#define FUNC_CText__Load							0x6A01A0
#define FUNC_CText_TKey__Locate						0x6A0000

enum eLanguages
{
	LANG_English,
//	LANG_Spanish,
	LANG_Polish,
	NUM_LANGUAGES
};


class CText
{
private:
	struct TKey
	{
		struct
		{
			const char		*text;
			unsigned long	hash;
		} *data;
		DWORD		size;

		char *		Locate(const char *key, bool* found);
	};
	struct TDat
	{
		void*		data;
		DWORD		size;
	};
	struct Table
	{
		struct Entry
		{
			char		name[8];
			void*		offset;
		};
		Entry		tablEntries[200];
		WORD		size;
	};
	TKey			tkeyMain;
	TDat			tdatMain;
	TKey			tkeyMission;
	TDat			tdatMission;
	BYTE			_f20;
	bool			haveTabl;
	bool			cderrorInitialized;
	bool			missionLoaded;
	char			missionName[8];
	char			cderrorText[256];
	Table			table;

public:
	const char*		GetText(const char* entry);
	void			ReloadFontsFiles(bool bUnk);
	void			TextNumberFormat(const char* pszText, int nNumber1, int nNumber2, int nNumber3, int nNumber4, int nNumber5, int nNumber6, char* pszOutBuf);
	void			Load(bool bUnk);
	void			Clear(bool bUnk);
};

static_assert(sizeof(CText) == CText_ARRAYSIZE, "CText class has wrong size!");

#endif