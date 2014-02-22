#include "StdAfx.h"
#include "Text.h"

#include "General.h"

WRAPPER void CText::Load(bool bKeepMissionText) { WRAPARG(bKeepMissionText); EAXJMP(0x6A01A0); }
WRAPPER void CText::Unload(bool bKeepMissionText) { WRAPARG(bKeepMissionText); EAXJMP(0x69FF20); }
WRAPPER const char* CKeyArray::Search(const char* pKey, unsigned char* bFound) { WRAPARG(pKey); WRAPARG(bFound); EAXJMP(0x6A0000); }

const char*	(__thiscall *CLEOGetTextFunc)(CText*, const char*);

const char* CText::Get(const char* pEntry)
{
	static char		GxtErrorString[24];
	const char*		outputText;

	if ( CLEOGetTextFunc )
	{
		outputText = CLEOGetTextFunc(this, pEntry);
		if ( !*outputText )
		{
			_snprintf(GxtErrorString, sizeof(GxtErrorString), "%s missing", pEntry);
			return GxtErrorString;
		}
		return outputText;
	}
	else
	{
		unsigned char bFound;
		if ( pEntry[0] == '\0' || pEntry[0] == ' ' ) 
			return "";

		outputText = tkeyMain.Search(pEntry, &bFound);

		if ( !bFound )
		{
			if ( missionLoaded || CGame::IsMissionPackGame() || haveTabl )
			{
				outputText = tkeyMission.Search(pEntry, &bFound);
				if ( !bFound )
				{
					_snprintf(GxtErrorString, sizeof(GxtErrorString), "%s missing", pEntry);
					return GxtErrorString;
				}
			}
		}
	}
	return outputText;
}

void CText::ReloadFontsFiles(bool bKeepMissionText)
{
	UNREFERENCED_PARAMETER(bKeepMissionText);
#ifdef INCLUDE_MULTIFONTFILES
	Load(bKeepMissionText);
	if ( bCanReloadFonts && bLastFontsID != GetFontsIDByLanguage() )
	{
		int	slot = CTxdStore::FindTxdSlot("fontDetails");
		if ( slot == -1 )
			slot = CTxdStore::AddTxdSlot("fontDetails");
		CTxdStore::LoadTxd(slot, GetFontsTXDByLanguage());
		CTxdStore::AddRef(slot);
		CTxdStore::PushCurrentTxd();
		CTxdStore::SetCurrentTxd(slot);
		texFonts[0].Load("font2", "font2m");
		texFonts[1].Load("font1", "font1m");
		CFont::LoadFontValues();
		CTxdStore::PopCurrentTxd();
	}
#endif
}