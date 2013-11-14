#include "StdAfx.h"

void CGridref::Draw()
{
	if ( hud.ShouldDisplayGridref() )
	{
		char	ZoneID[4];
		BYTE	zoneX;
		BYTE	zoneY;

		GetIDByPlayerPosition(&zoneX, &zoneY);
		CFont::SetTextBackground(0, 0);
		CFont::SetTextUseProportionalValues(false);
		CFont::SetFontStyle(FONT_Pricedown);
		CFont::SetTextAlignment(ALIGN_Center);
		CFont::SetTextOutline(1);
		CFont::SetTextBorderRGBA(CRGBA(0, 0, 0, 255));
		CFont::SetTextLetterSize(_width(0.35), _height(0.65));
		CFont::SetTextColour(CRGBA(BaseColors[2]));
		sprintf(ZoneID, "%c%d", zoneX + 100, zoneY);
		CFont::PrintString(_x(150.0), _y(7.5), ZoneID);
		CFont::PrintString(_x(115.0), _y(7.5), GetNameByID(zoneX, zoneY));
		CFont::SetTextOutline(0);
	}
}

void CGridref::GetIDByPlayerPosition(BYTE* gridrefX, BYTE* gridrefY)
{
	DWORD dwFunc = FUNC_CGridref__GetIDByPlayerPosition;
	_asm
	{
		mov		eax, gridrefY
		push	eax
		mov		eax, gridrefX
		push	eax
		call	dwFunc
		add		esp, 8
	}
}

char* CGridref::GetNameByID(BYTE gridrefX, BYTE gridrefY)
{
	if ( gridrefX > 9 || gridrefY > 9 )
		return 0;
	return gridref->artistNames[gridrefY + 10 * gridrefX];
}