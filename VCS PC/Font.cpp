#include "StdAfx.h"
#include "Font.h"

#include "Frontend.h"
#include "Text.h"

CSprite2d			CFont::Sprite[NUM_FONT_SHEETS];
CFont::sFontSizes	CFont::Size[NUM_FONT_SHEETS];
CFont::tDetails&	CFont::Details = *(CFont::tDetails*)0xC71A60;

WRAPPER void CFont::SetWrapx(float fWrap) { WRAPARG(fWrap); EAXJMP(0x7194D0); }
WRAPPER void CFont::LoadFontValues() { EAXJMP(0x7187C0); }
WRAPPER void CFont::RenderFontBuffer() { EAXJMP(0x719840); }
WRAPPER void CFont::SetColor(CRGBA color) { WRAPARG(color); EAXJMP(0x719430); }
WRAPPER void CFont::SetDropColor(CRGBA color) { WRAPARG(color); EAXJMP(0x719510); }
WRAPPER void CFont::SetDropShadowPosition(signed char bShadow) { WRAPARG(bShadow); EAXJMP(0x719570); }
WRAPPER void CFont::SetEdge(signed char bEdge) { WRAPARG(bEdge); EAXJMP(0x719590); }
WRAPPER void CFont::SetSlantRefPoint(float fOne, float fTwo) { WRAPARG(fOne); WRAPARG(fTwo); EAXJMP(0x719400); }
WRAPPER void CFont::SetSlant(float fAngle) { WRAPARG(fAngle); EAXJMP(0x719420); }
WRAPPER void CFont::SetCentreSize(float fSize) { WRAPARG(fSize); EAXJMP(0x7194E0); }
WRAPPER void CFont::SetRightJustifyWrap(float fWrap) { WRAPARG(fWrap); EAXJMP(0x7194F0); }
WRAPPER void CFont::SetJustify(bool bJustify) { WRAPARG(bJustify); EAXJMP(0x719600); }
WRAPPER void CFont::SetProportional(bool bProportional) { WRAPARG(bProportional); EAXJMP(0x7195B0); }
WRAPPER void CFont::SetBackground(unsigned char bUseBackground, unsigned char bBackgroundType) { WRAPARG(bUseBackground); WRAPARG(bBackgroundType); EAXJMP(0x7195C0); }
WRAPPER void CFont::SetOrientation(unsigned char bOrientation) { WRAPARG(bOrientation); EAXJMP(0x719610); }
WRAPPER void CFont::PrintString(float posX, float posY, const char* pText) { WRAPARG(posX); WRAPARG(posY); WRAPARG(pText); EAXJMP(0x71A700); }
WRAPPER void CFont::PrintStringFromBottom(float posX, float posY, const char* pText) { WRAPARG(posX); WRAPARG(posY); WRAPARG(pText); EAXJMP(0x71A820); }

unsigned char CFont::FindSubFontCharacter(char character, unsigned char bFontType)
{
	UNREFERENCED_PARAMETER(bFontType);
	if ( character == 4 )
		return 187;
	if ( character >= 16 && character <= 26 )
		return character + 160;
	if ( character >= 59 && character <= 61 )
		return character + 129;
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

void CFont::SetScale(float scaleX, float scaleY)
{
	Details.textLetterSizeX = scaleX;
	Details.textLetterSizeY = scaleY;
}

void CFont::SetScaleLang(float scaleX, float scaleY)
{
	Details.textLetterSizeY = scaleY;
	switch ( FrontEndMenuManager.GetLanguage() )
	{
	//case LANGUAGE_Spanish:
	//	Details.textLetterSizeX = scaleX * 0.85f;
	//	return;
	case LANGUAGE_Polish:
		Details.textLetterSizeX = scaleX * 0.95f;
		return;
	default:	// LANGUAGE_English
		Details.textLetterSizeX = scaleX;
		return;
	}
}

void CFont::SetFontStyle(unsigned char bFont)
{
	if ( bFont == FONT_Hud )
	{
		Details.bSpriteToUse = 3;
		Details.bFontStyle = 1;
	}
	else
	{
		Details.bSpriteToUse = bFont;
		Details.bFontStyle = 0;
	}
}

void CFont::Initialise()
{
	CPNGArchive		FontsSPTA("models\\fonts.spta");
	FontsSPTA.SetDirectory(nullptr);

	Sprite[0].SetTextureFromSPTA(FontsSPTA, "font1");
	Sprite[1].SetTextureFromSPTA(FontsSPTA, "font2");
	Sprite[2].SetTextureFromSPTA(FontsSPTA, "font3");
	Sprite[3].SetTextureFromSPTA(FontsSPTA, "font4");

	FontsSPTA.CloseArchive();

	// TODO: Dummy CFont stuff may not be needed at all?
	LoadFontValues();

	// TODO: Buttons
}

void CFont::Shutdown()
{
	for ( int i = 0; i < NUM_FONT_SHEETS; ++i )
		Sprite[i].Delete();
}