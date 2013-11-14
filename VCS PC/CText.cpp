#include "StdAfx.h"

const char* CText::GetText(const char* entry)
{
	static char	missingText[24];
	char*		outputText;
	if ( CLEOGetTextFunc )
	{
		outputText = CLEOGetTextFunc(this, entry);
		if ( !*outputText )
		{
			_snprintf(missingText, 24, "%s missing", entry);
			return	missingText;
		}
		return outputText;
	}
	else
	{
		bool found;
		if ( *entry == '\0' || *entry == ' ' ) 
			return "";

		outputText = tkeyMain.Locate(entry, &found);

		if ( !found )
		{
			if ( missionLoaded || *mpackNumber || haveTabl )
			{
				outputText = tkeyMission.Locate(entry, &found);
				if ( !found )
				{
					_snprintf(missingText, 24, "%s missing", entry);
					return	missingText;
				}
			}
		}
	}
	return outputText;
}

void CText::ReloadFontsFiles(bool bUnk)
{
	UNREFERENCED_PARAMETER(bUnk);
#ifdef INCLUDE_MULTIFONTFILES
	Load(unkFlag);
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
		CFont::ReadFontsDAT();
		CTxdStore::PopCurrentTxd();
	}
#endif
}

void CText::TextNumberFormat(const char* pszText, int nNumber1, int nNumber2, int nNumber3, int nNumber4, int nNumber5, int nNumber6, char* pszOutBuf)
{
	DWORD dwFunc = (DWORD)FUNC_CText__TextNumberFormat;
	_asm
	{
		push	pszOutBuf
		push	nNumber6
		push	nNumber5
		push	nNumber4
		push	nNumber3
		push	nNumber2
		push	nNumber1
		push	pszText
		mov		ecx, this
		call	dwFunc
	}
}

void CText::Load(bool bUnk)
{
	DWORD dwFunc = (DWORD)FUNC_CText__Load;
	_asm
	{
		movzx	eax, bUnk
		push	eax
		mov		ecx, this
		call	dwFunc
	}
}

void CText::Clear(bool bUnk)
{
	DWORD dwFunc = (DWORD)FUNC_CText__Clear;
	_asm
	{
		movzx	eax, bUnk
		push	eax
		mov		ecx, this
		call	dwFunc
	}
}

char* CText::TKey::Locate(const char* key, bool* found)
{
	DWORD	dwFunc = (DWORD)FUNC_CText_TKey__Locate;
	_asm
	{
		push	found
		push	key
		mov		ecx, this
		call	dwFunc
	}
}