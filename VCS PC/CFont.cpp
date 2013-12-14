#include "StdAfx.h"

CSprite2d			CFont::Sprite[NUM_FONT_SHEETS];
CFont::sFontSizes	CFont::Size[NUM_FONT_SHEETS];

WRAPPER void CFont::SetWrapx(float fWrap) { WRAPARG(fWrap); EAXJMP(0x7194D0); }

void CFont::ReadFontsDAT()
{
	DWORD dwFunc = (DWORD)FUNC_CFont__ReadFontsDAT;
	_asm call dwFunc
}

/*signed char Upcase(signed char character, signed char unk)
{
	if ( unk == 1 )
	{
		if ( character == 26 )
			return 154;

		if ( character >= 8 && character <= 9 )
		{
			unsigned char result = character;
			result += 86;
			return result;
		}

		if ( character == 4 )
			return 93;

		if ( character == 7 )
			return 206;

		if ( character == 14 )
			return 207;
	}
	if ( character != 1 || unk != 1 )
	{
		if ( character == 143 )
			return 205;

		if ( character == 31 )
			return 91;

		if ( character == 6 )
			return 10;

		if ( character == 62 )
			return 32;

		if ( character >= 16 && character <= 25 )
			return (character + 128);

		if ( character >= 33 && character <= 58 )
			return (character + 122);

		if ( character >= 65 && character <= 90 )
			return (character + 90);

		if ( character >= 96 && character <= 118 )
			return (character + 85);

		if ( character >= 119 && character <= 140 )
			return (character + 62);

		if ( character >= 141 && character <= 142 )
			return 204;
	}
	else
		return 208;
	return character;
}*/

unsigned char CFont::AssignBottomFontIndex(unsigned char character, unsigned char bFontType)
{
	UNREFERENCED_PARAMETER(bFontType);
	if ( character == 4 )
		return 155;
	if ( character >= 16 && character <= 26 )
		return character + 128;
	if ( character >= 59 && character <= 61 )
		return character + 97;
	/*if ( character == 59 )
		return 157;
	if ( character == 60 )
		return 158;
	if ( character == 61 )
		return 156;*/
	/*if ( unkValue == 1 )
	{
		if ( charToReturn == 26 )
			return 154;

		if ( charToReturn >= 8 )
		{
			if ( charToReturn <= 9 )
				return charToReturn + 86;
		}

		if ( charToReturn == 4 )
			return 93;

		if ( charToReturn == 7 )
			return 206;

		if ( charToReturn == 14 )
			return 207;
	}
	if ( charToReturn != 1 || unkValue != 1 )
	{
		if ( charToReturn == 143 )
			return 205;

		if ( charToReturn == 31 )
			return 91;

		if ( charToReturn == 6 )
			return 10;

		if ( charToReturn == 62 )
			return 32;

		if ( charToReturn >= 16 && charToReturn <= 25 )
			return charToReturn + 128;

		if ( charToReturn >= 33 && charToReturn <= 58 )
			return charToReturn + 122;

		if ( charToReturn >= 65 && charToReturn <= 90 )
			return charToReturn + 90;

		if ( ( charToReturn >= 96 && charToReturn <= 106 ) || ( charToReturn >= 110 && charToReturn <= 118 ) )
			return charToReturn + 85;

		if ( charToReturn >= 107 && charToReturn <= 109 )
			return charToReturn + 78;

		if ( charToReturn >= 128 && charToReturn <= 131 )
			return charToReturn + 71;

		if ( ( charToReturn >= 119 && charToReturn <= 127 ) || ( charToReturn >= 132 && charToReturn <= 140 ) )
			return charToReturn + 62;

		if ( charToReturn >= 141 && charToReturn <= 142 )
			return 204;
	}
	else
		return 208;*/
	return character;
}

void CFont::SetTextLetterSize(float scaleX, float scaleY)
{
	fontDetails->textLetterSizeX = scaleX;
	fontDetails->textLetterSizeY = scaleY;
}

void CFont::SetTextLetterSizeWithLanguageScaling(float scaleX, float scaleY)
{
	fontDetails->textLetterSizeY = scaleY;
	switch ( menu->GetLanguage() )
	{
	//case LANG_Spanish:
	//	fontDetails->textLetterSizeX = scaleX * 0.85f;
	//	break;
	case LANG_Polish:
		fontDetails->textLetterSizeX = scaleX * 0.95f;
		break;
	default:	// LANG_English
		fontDetails->textLetterSizeX = scaleX;
		break;
	}
}

void CFont::SetFontStyle(unsigned char bFont)
{
	if ( bFont == FONT_Hud )
	{
		fontDetails->bSpriteToUse = 3;
		fontDetails->bFontStyle = 1;
	}
	else
	{
		fontDetails->bSpriteToUse = bFont;
		fontDetails->bFontStyle = 0;
	}
}

void CFont::SetTextColour(CRGBA colour)
{
	DWORD dwFunc = (DWORD)FUNC_CFont__SetTextColour;
	_asm
	{
		push	colour
		call	dwFunc
		add		esp, 4
	}
}

void CFont::SetTextBorderRGBA(CRGBA colour)
{
	DWORD dwFunc = (DWORD)FUNC_CFont__SetTextBorderRGBA;
	_asm
	{
		mov		eax, colour
		push	eax
		call	dwFunc
		add		esp, 4
	}
}

void CFont::SetTextShadow(unsigned char bShadow)
{
	DWORD dwFunc = (DWORD)FUNC_CFont__SetTextShadow;
	_asm
	{
		movzx	eax, bShadow
		push	eax
		call	dwFunc
		add		esp, 4
	}
}

void CFont::SetTextOutline(unsigned char bOutline)
{
	DWORD dwFunc = (DWORD)FUNC_CFont__SetTextOutline;
	_asm
	{
		movzx	eax, bOutline
		push	eax
		call	dwFunc
		add		esp, 4
	}
}

void CFont::ResetSlantedTextPos(float fOne, float fTwo)
{
	DWORD dwFunc = (DWORD)FUNC_CFont__ResetSlantedTextPos;
	_asm
	{
		mov		eax, fTwo
		push	eax
		mov		eax, fOne
		push	eax
		call	dwFunc
		add		esp, 8
	}
}

void CFont::SetTextSlanted(float fAngle)
{
	DWORD dwFunc = (DWORD)FUNC_CFont__SetTextSlanted;
	_asm
	{
		mov		eax, fAngle
		push	eax
		call	dwFunc
		add		esp, 4
	}
}

void CFont::SetCentreSize(float fUnk)
{
	DWORD dwFunc = (DWORD)0x7194E0;
	_asm
	{
		mov		eax, fUnk
		push	eax
		call	dwFunc
		add		esp, 4
	}
}

void CFont::SetTextWrapX(float fWrap)
{
	DWORD dwFunc = (DWORD)FUNC_CFont__SetTextWrapX;
	_asm
	{
		mov		eax, fWrap
		push	eax
		call	dwFunc
		add		esp, 4
	}
}

void CFont::SetTextJustify(bool bJustify)
{
	DWORD dwFunc = (DWORD)FUNC_CFont__SetTextJustify;
	_asm
	{
		movzx	eax, bJustify
		push	eax
		call	dwFunc
		add		esp, 4
	}
}

void CFont::func_71A210()
{
	DWORD dwFunc = (DWORD)0x71A210;
	_asm call	dwFunc
}

void CFont::SetTextUseProportionalValues(bool bUseProportionalValues)
{
	DWORD dwFunc = (DWORD)FUNC_CFont__SetTextUseProportionalValues;
	_asm
	{
		movzx	eax, bUseProportionalValues
		push	eax
		call	dwFunc
		add		esp, 4
	}
}

void CFont::SetTextBackground(unsigned char bUseBackground, unsigned char bBackgroundType)
{
	DWORD dwFunc = (DWORD)FUNC_CFont__SetTextBackground;
	_asm
	{
		movzx	eax, bBackgroundType
		push	eax
		movzx	eax, bUseBackground
		push	eax
		call	dwFunc
		add		esp, 8
	}
}

void CFont::SetTextAlignment(unsigned char bAlign)
{
	DWORD dwFunc = (DWORD)FUNC_CFont__SetTextAlignment;
	_asm
	{
		movzx	eax, bAlign
		push	eax
		call	dwFunc
		add		esp, 4
	}
}

void CFont::PrintString(float posX, float posY, const char* text)
{
	DWORD dwFunc = (DWORD)FUNC_CFont__PrintString;
	_asm
	{
		mov		eax, text
		push	eax
		mov		eax, posY
		push	eax
		mov		eax, posX
		push	eax
		call	dwFunc
		add		esp, 0Ch
	}
}

void CFont::UnkPrintString(float posX, float posY, const char* text)
{
	DWORD dwFunc = (DWORD)FUNC_CFont__UnkPrintString;
	_asm
	{
		mov		eax, text
		push	eax
		mov		eax, posY
		push	eax
		mov		eax, posX
		push	eax
		call	dwFunc
		add		esp, 0Ch
	}
}