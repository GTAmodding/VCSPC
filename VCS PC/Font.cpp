#include "StdAfx.h"
#include "Font.h"

#include "Frontend.h"
#include "Text.h"
#include "TxdStore.h"
#include "Pad.h"

CSprite2d			CFont::Sprite[NUM_FONT_SHEETS];
CFont::sFontSizes	CFont::Size[NUM_FONT_SHEETS];
CFont::tDetails&	CFont::Details = *(CFont::tDetails*)0xC71A60;

BYTE&				CFont::PS2Symbol =* (BYTE*)0xC71A54;

float				CFont::PS2SpriteWidth[NUM_BUTTON_SPRITES];
CSprite2d			CFont::PS2Sprite[NUM_BUTTON_SPRITES];
bool				CFont::bX360Buttons;

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
WRAPPER void CFont::InitPerFrame() { EAXJMP(0x719800); }
WRAPPER int CFont::GetNumberLines(float, float, const char* pText) { WRAPARG(pText); EAXJMP(0x71A5E0); }

void CFont::GetButtonByIndex(int wButton, char* wcBuf, size_t& nStartPoint)
{
	switch ( wButton )
	{
	case BUTTON_UP:
		{
			wcBuf[nStartPoint++] = '~';
			wcBuf[nStartPoint++] = 'u';
			wcBuf[nStartPoint++] = '~';
			return;
		}
	case BUTTON_DOWN:
		{
			wcBuf[nStartPoint++] = '~';
			wcBuf[nStartPoint++] = 'd';
			wcBuf[nStartPoint++] = '~';
			return;
		}
	case BUTTON_LEFT:
		{
			wcBuf[nStartPoint++] = '~';
			wcBuf[nStartPoint++] = '<';
			wcBuf[nStartPoint++] = '~';
			return;
		}
	case BUTTON_RIGHT:
		{
			wcBuf[nStartPoint++] = '~';
			wcBuf[nStartPoint++] = '>';
			wcBuf[nStartPoint++] = '~';
			return;
		}
	case BUTTON_SELECT:
		{
			if ( bX360Buttons )
			{
				wcBuf[nStartPoint++] = 'B';
				wcBuf[nStartPoint++] = 'A';
				wcBuf[nStartPoint++] = 'C';
				wcBuf[nStartPoint++] = 'K';
			}
			else
			{
				wcBuf[nStartPoint++] = 'S';
				wcBuf[nStartPoint++] = 'E';
				wcBuf[nStartPoint++] = 'L';
				wcBuf[nStartPoint++] = 'E';
				wcBuf[nStartPoint++] = 'C';
				wcBuf[nStartPoint++] = 'T';
			}
			return;
		}
	case BUTTON_L3:
		{
			wcBuf[nStartPoint++] = '~';
			wcBuf[nStartPoint++] = 'A';
			wcBuf[nStartPoint++] = '~';
			return;
		}
	case BUTTON_R3:
		{
			wcBuf[nStartPoint++] = '~';
			wcBuf[nStartPoint++] = 'c';
			wcBuf[nStartPoint++] = '~';
			return;
		}
	case BUTTON_L2:
		{
			wcBuf[nStartPoint++] = '~';
			wcBuf[nStartPoint++] = 'm';
			wcBuf[nStartPoint++] = '~';
			return;
		}
	case BUTTON_R2:
		{
			wcBuf[nStartPoint++] = '~';
			wcBuf[nStartPoint++] = 'v';
			wcBuf[nStartPoint++] = '~';
			return;
		}
	case BUTTON_L1:
		{
			wcBuf[nStartPoint++] = '~';
			wcBuf[nStartPoint++] = 'K';
			wcBuf[nStartPoint++] = '~';
			return;
		}
	case BUTTON_R1:
		{
			wcBuf[nStartPoint++] = '~';
			wcBuf[nStartPoint++] = 'j';
			wcBuf[nStartPoint++] = '~';
			return;
		}
	case BUTTON_PS:
		{
			if ( bX360Buttons )
			{
				wcBuf[nStartPoint++] = 'G';
				wcBuf[nStartPoint++] = 'U';
				wcBuf[nStartPoint++] = 'I';
				wcBuf[nStartPoint++] = 'D';
				wcBuf[nStartPoint++] = 'E';
			}
			else
			{
				wcBuf[nStartPoint++] = 'H';
				wcBuf[nStartPoint++] = 'O';
				wcBuf[nStartPoint++] = 'M';
				wcBuf[nStartPoint++] = 'E';
			}
			return;
		}
	case BUTTON_CROSS:
		{
			wcBuf[nStartPoint++] = '~';
			wcBuf[nStartPoint++] = 'x';
			wcBuf[nStartPoint++] = '~';
			return;
		}
	case BUTTON_CIRCLE:
		{
			wcBuf[nStartPoint++] = '~';
			wcBuf[nStartPoint++] = 'o';
			wcBuf[nStartPoint++] = '~';
			return;
		}
	case BUTTON_SQUARE:
		{
			wcBuf[nStartPoint++] = '~';
			wcBuf[nStartPoint++] = 'q';
			wcBuf[nStartPoint++] = '~';
			return;
		}
	case BUTTON_TRIANGLE:
		{
			wcBuf[nStartPoint++] = '~';
			wcBuf[nStartPoint++] = 't';
			wcBuf[nStartPoint++] = '~';
			return;
		}

	case BUTTON_UPDOWN:
		{
			wcBuf[nStartPoint++] = '~';
			wcBuf[nStartPoint++] = 'u';
			wcBuf[nStartPoint++] = 'd';
			wcBuf[nStartPoint++] = '~';
			return;
		}
	case BUTTON_LEFTRIGHT:
		{
			wcBuf[nStartPoint++] = '~';
			wcBuf[nStartPoint++] = '<';
			wcBuf[nStartPoint++] = '>';
			wcBuf[nStartPoint++] = '~';
			return;
		}

	case BUTTON_THUMBL:
		{
			wcBuf[nStartPoint++] = '~';
			wcBuf[nStartPoint++] = '[';
			wcBuf[nStartPoint++] = '~';
			return;
		}
	case BUTTON_THUMBLX:
		{
			wcBuf[nStartPoint++] = '~';
			wcBuf[nStartPoint++] = '[';
			wcBuf[nStartPoint++] = 'x';
			wcBuf[nStartPoint++] = '~';
			return;
		}
	case BUTTON_THUMBLXL:
		{
			wcBuf[nStartPoint++] = '~';
			wcBuf[nStartPoint++] = '[';
			wcBuf[nStartPoint++] = '<';
			wcBuf[nStartPoint++] = '~';
			return;
		}
	case BUTTON_THUMBLXR:
		{
			wcBuf[nStartPoint++] = '~';
			wcBuf[nStartPoint++] = '[';
			wcBuf[nStartPoint++] = '>';
			wcBuf[nStartPoint++] = '~';
			return;
		}
	case BUTTON_THUMBLY:
		{
			wcBuf[nStartPoint++] = '~';
			wcBuf[nStartPoint++] = '[';
			wcBuf[nStartPoint++] = 'y';
			wcBuf[nStartPoint++] = '~';
			return;
		}
	case BUTTON_THUMBLYU:
		{
			wcBuf[nStartPoint++] = '~';
			wcBuf[nStartPoint++] = '[';
			wcBuf[nStartPoint++] = 'u';
			wcBuf[nStartPoint++] = '~';
			return;
		}
	case BUTTON_THUMBLYD:
		{
			wcBuf[nStartPoint++] = '~';
			wcBuf[nStartPoint++] = '[';
			wcBuf[nStartPoint++] = 'd';
			wcBuf[nStartPoint++] = '~';
			return;
		}
	
	case BUTTON_THUMBR:
		{
			wcBuf[nStartPoint++] = '~';
			wcBuf[nStartPoint++] = ']';
			wcBuf[nStartPoint++] = '~';
			return;
		}
	case BUTTON_THUMBRX:
		{
			wcBuf[nStartPoint++] = '~';
			wcBuf[nStartPoint++] = ']';
			wcBuf[nStartPoint++] = 'x';
			wcBuf[nStartPoint++] = '~';
			return;
		}
	case BUTTON_THUMBRXL:
		{
			wcBuf[nStartPoint++] = '~';
			wcBuf[nStartPoint++] = ']';
			wcBuf[nStartPoint++] = '<';
			wcBuf[nStartPoint++] = '~';
			return;
		}
	case BUTTON_THUMBRXR:
		{
			wcBuf[nStartPoint++] = '~';
			wcBuf[nStartPoint++] = ']';
			wcBuf[nStartPoint++] = '>';
			wcBuf[nStartPoint++] = '~';
			return;
		}
	case BUTTON_THUMBRY:
		{
			wcBuf[nStartPoint++] = '~';
			wcBuf[nStartPoint++] = '[';
			wcBuf[nStartPoint++] = 'y';
			wcBuf[nStartPoint++] = '~';
			return;
		}
	case BUTTON_THUMBRYU:
		{
			wcBuf[nStartPoint++] = '~';
			wcBuf[nStartPoint++] = ']';
			wcBuf[nStartPoint++] = 'u';
			wcBuf[nStartPoint++] = '~';
			return;
		}
	case BUTTON_THUMBRYD:
		{
			wcBuf[nStartPoint++] = '~';
			wcBuf[nStartPoint++] = ']';
			wcBuf[nStartPoint++] = 'd';
			wcBuf[nStartPoint++] = '~';
			return;
		}

	/*case BUTTON_SIXAXIS_1:
		{
			wcBuf[nStartPoint++] = '~';
			wcBuf[nStartPoint++] = ':';
			wcBuf[nStartPoint++] = '1';
			wcBuf[nStartPoint++] = '~';
			return;
		}
	case BUTTON_SIXAXIS_2:
		{
			wcBuf[nStartPoint++] = '~';
			wcBuf[nStartPoint++] = ':';
			wcBuf[nStartPoint++] = '2';
			wcBuf[nStartPoint++] = '~';
			return;
		}
	case BUTTON_SIXAXIS_3:
		{
			wcBuf[nStartPoint++] = '~';
			wcBuf[nStartPoint++] = ':';
			wcBuf[nStartPoint++] = '3';
			wcBuf[nStartPoint++] = '~';
			return;
		}
	case BUTTON_SIXAXIS_4:
		{
			wcBuf[nStartPoint++] = '~';
			wcBuf[nStartPoint++] = ':';
			wcBuf[nStartPoint++] = '4';
			wcBuf[nStartPoint++] = '~';
			return;
		}*/
	}
}

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

void CFont::SetAlphaFade(float fAlpha)
{
	Details.fAlphaFade = fAlpha;
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

static unsigned int		nTokenWidth;
static float			fSymbolSpriteWidth;

char* CFont::ParseToken(char* pText, CRGBA& rColor, bool bIgnoreColors, char* pChar)
{
	char*	pCharToWorkWith = pText + 1;
	nTokenWidth = 3;
	fSymbolSpriteWidth = 17.0f;

#ifndef GINPUT_COMPILE_SA_VERSION
	PS2Symbol = BUTTON_NONE;
#endif

	switch ( *pCharToWorkWith )
	{
	case 'u':	// DPad Up
	case 'U':
		if ( pCharToWorkWith[1] == 'd' || pCharToWorkWith[1] == 'D' )
		{
			nTokenWidth = 4;
			pCharToWorkWith++;
			PS2Symbol = BUTTON_UPDOWN;
		}
		else
			PS2Symbol = pXboxPad[0]->HasPadInHands() ? BUTTON_UP : BUTTON_PC_UP;
		break;
	case 'd':	// DPad Down
	case 'D':
		PS2Symbol = pXboxPad[0]->HasPadInHands() ? BUTTON_DOWN : BUTTON_PC_DOWN;
		break;
	case '<':	// DPad left
		if ( pCharToWorkWith[1] == '>' )
		{
			nTokenWidth = 4;
			pCharToWorkWith++;
			PS2Symbol = BUTTON_LEFTRIGHT;
		}
		else
			PS2Symbol = pXboxPad[0]->HasPadInHands() ? BUTTON_LEFT : BUTTON_PC_LEFT;
		break;
	case '>':	// DPad Right
		PS2Symbol = pXboxPad[0]->HasPadInHands() ? BUTTON_RIGHT : BUTTON_PC_RIGHT;
		break;

	case '[':	// Left thumbstick
		switch ( pCharToWorkWith[1] )
		{
		case '~':
			PS2Symbol = BUTTON_THUMBL;
			break;
		case 'x':
			nTokenWidth = 4;
			PS2Symbol = BUTTON_THUMBLX;
			break;
		case 'y':
			nTokenWidth = 4;
			PS2Symbol = BUTTON_THUMBLY;
			break;
		case '<':
			nTokenWidth = 4;
			PS2Symbol = BUTTON_THUMBLXL;
			break;
		case '>':
			nTokenWidth = 4;
			PS2Symbol = BUTTON_THUMBLXR;
			break;
		case 'u':
			nTokenWidth = 4;
			PS2Symbol = BUTTON_THUMBLYU;
			break;
		case 'd':
			nTokenWidth = 4;
			PS2Symbol = BUTTON_THUMBLYD;
			break;
		}
		pCharToWorkWith++;	// It'll either encounter ~ or another character from the code - safe in any case
		break;
	case ']':	// Right thumbstick
		switch ( *(++pCharToWorkWith) )
		{
		case '~':
			PS2Symbol = BUTTON_THUMBR;
			break;
		case 'x':
			nTokenWidth = 4;
			PS2Symbol = BUTTON_THUMBRX;
			break;
		case 'y':
			nTokenWidth = 4;
			PS2Symbol = BUTTON_THUMBRY;
			break;
		case '<':
			nTokenWidth = 4;
			PS2Symbol = BUTTON_THUMBRXL;
			break;
		case '>':
			nTokenWidth = 4;
			PS2Symbol = BUTTON_THUMBRXR;
			break;
		case 'u':
			nTokenWidth = 4;
			PS2Symbol = BUTTON_THUMBRYU;
			break;
		case 'd':
			nTokenWidth = 4;
			PS2Symbol = BUTTON_THUMBRYD;
			break;
		}
		break;
	/*case ':':	// Sixaxis
		nTokenWidth = 4;
		switch ( *(++pCharToWorkWith) )
		{
		case '1':
			PS2Symbol = BUTTON_SIXAXIS_1;
			break;
		case '2':
			PS2Symbol = BUTTON_SIXAXIS_2;
			break;
		case '3':
			PS2Symbol = BUTTON_SIXAXIS_3;
			break;
		case '4':
			PS2Symbol = BUTTON_SIXAXIS_4;
			break;
		}
		break;*/

	default:
		pCharToWorkWith = ((char*(*)(char*, CRGBA&, bool, char*))0x718F00)(pText, rColor, bIgnoreColors, pChar);
		fSymbolSpriteWidth = PS2SpriteWidth[PS2Symbol];
		return pCharToWorkWith;
	}

	fSymbolSpriteWidth = PS2SpriteWidth[PS2Symbol];

	while ( *pCharToWorkWith != '~' )
		++pCharToWorkWith;

	++pCharToWorkWith;

	return pCharToWorkWith;
}

void CFont::Initialise()
{
	int		nSlot = CTxdStore::AddTxdSlot("fonts");
	CTxdStore::LoadTxd(nSlot, "models\\fonts.txd");
	CTxdStore::AddRef(nSlot);
	CTxdStore::PushCurrentTxd();
	CTxdStore::SetCurrentTxd(nSlot);

	Sprite[0].SetTexture("font1", "");
	Sprite[1].SetTexture("font2", "");
	Sprite[2].SetTexture("font3", "");
	Sprite[3].SetTexture("font4", "");

	/*CPNGArchive		FontsSPTA("models\\fonts.spta");
	FontsSPTA.SetDirectory(nullptr);

	Sprite[0].SetTextureFromSPTA(FontsSPTA, "font1");
	Sprite[1].SetTextureFromSPTA(FontsSPTA, "font2");
	Sprite[2].SetTextureFromSPTA(FontsSPTA, "font3");
	Sprite[3].SetTextureFromSPTA(FontsSPTA, "font4");

	FontsSPTA.CloseArchive();*/

	// TODO: Dummy CFont stuff may not be needed at all?
	LoadFontValues();

	nSlot = CTxdStore::AddTxdSlot("buttons");
	CTxdStore::LoadTxd(nSlot, bX360Buttons ? "models\\360btns.txd" : "models\\ps3btns.txd");
	CTxdStore::AddRef(nSlot);
	CTxdStore::SetCurrentTxd(nSlot);

	PS2Sprite[BUTTON_UP].SetTexture("up", "");
	PS2Sprite[BUTTON_DOWN].SetTexture("down", "");
	PS2Sprite[BUTTON_LEFT].SetTexture("left", "");
	PS2Sprite[BUTTON_RIGHT].SetTexture("right", "");

	PS2Sprite[BUTTON_CROSS].SetTexture("cross", "");
	PS2Sprite[BUTTON_CIRCLE].SetTexture("circle", "");
	PS2Sprite[BUTTON_SQUARE].SetTexture("square", "");
	PS2Sprite[BUTTON_TRIANGLE].SetTexture("triangle", "");

	PS2Sprite[BUTTON_L1].SetTexture("l1", "");
	PS2Sprite[BUTTON_L2].SetTexture("l2", "");
	PS2Sprite[BUTTON_L3].SetTexture("l3", "");
	PS2Sprite[BUTTON_R1].SetTexture("r1", "");		
	PS2Sprite[BUTTON_R2].SetTexture("r2", "");	
	PS2Sprite[BUTTON_R3].SetTexture("r3", "");

	PS2Sprite[BUTTON_UPDOWN].SetTexture("dud", "");
	PS2Sprite[BUTTON_LEFTRIGHT].SetTexture("dlr", "");

	PS2Sprite[BUTTON_THUMBL].SetTexture("thumbl", "");
	PS2Sprite[BUTTON_THUMBLX].SetTexture("thumblx", "");
	PS2Sprite[BUTTON_THUMBLXL].SetTexture("thumblxl", "");
	PS2Sprite[BUTTON_THUMBLXR].SetTexture("thumblxr", "");
	PS2Sprite[BUTTON_THUMBLY].SetTexture("thumbly", "");
	PS2Sprite[BUTTON_THUMBLYU].SetTexture("thumblyu", "");
	PS2Sprite[BUTTON_THUMBLYD].SetTexture("thumblyd", "");

	PS2Sprite[BUTTON_THUMBR].SetTexture("thumbr", "");
	PS2Sprite[BUTTON_THUMBRX].SetTexture("thumbrx", "");
	PS2Sprite[BUTTON_THUMBRXL].SetTexture("thumbrxl", "");
	PS2Sprite[BUTTON_THUMBRXR].SetTexture("thumbrxr", "");
	PS2Sprite[BUTTON_THUMBRY].SetTexture("thumbry", "");
	PS2Sprite[BUTTON_THUMBRYU].SetTexture("thumbryu", "");
	PS2Sprite[BUTTON_THUMBRYD].SetTexture("thumbryd", "");

	/*nTexSlot = CTxdStore::FindTxdSlot("sixaxis");
	if ( nTexSlot == -1 )
		nTexSlot = CTxdStore::AddTxdSlot("sixaxis");

	CTxdStore::LoadTxd(nTexSlot, "models\\sixaxis.txd");
	CTxdStore::AddRef(nTexSlot);
	CTxdStore::SetCurrentTxd(nTexSlot);

	PS2Sprite[BUTTON_SIXAXIS_1].SetTexture("sixaxis1", "");
	PS2Sprite[BUTTON_SIXAXIS_2].SetTexture("sixaxis2", "");
	PS2Sprite[BUTTON_SIXAXIS_3].SetTexture("sixaxis3", "");
	PS2Sprite[BUTTON_SIXAXIS_4].SetTexture("sixaxis4", "");*/

	// pcbtns - SA only
	nSlot = CTxdStore::FindTxdSlot("pcbtns");
	if ( nSlot == -1 )
		nSlot = CTxdStore::AddTxdSlot("pcbtns");

	CTxdStore::LoadTxd(nSlot, "models\\pcbtns.txd");
	CTxdStore::AddRef(nSlot);
	CTxdStore::SetCurrentTxd(nSlot);

	PS2Sprite[BUTTON_PC_UP].SetTexture("up", "");
	PS2Sprite[BUTTON_PC_DOWN].SetTexture("down", "");
	PS2Sprite[BUTTON_PC_LEFT].SetTexture("left", "");
	PS2Sprite[BUTTON_PC_RIGHT].SetTexture("right", "");

	CTxdStore::PopCurrentTxd();

	// Recalculate widths
	for ( int i = 1; i < NUM_BUTTON_SPRITES; i++ )
	{
		float	fAspectRatio = static_cast<float>(RwRasterGetWidth(RwTextureGetRaster(PS2Sprite[i].GetTexture()))) / RwRasterGetHeight(RwTextureGetRaster(PS2Sprite[i].GetTexture()));
		PS2SpriteWidth[i] = 17.0f * fAspectRatio;
	}
}

void CFont::Shutdown()
{
	for ( int i = 0; i < NUM_FONT_SHEETS; i++ )
		Sprite[i].Delete();

	for ( int i = 0; i < NUM_BUTTON_SPRITES; i++ )
		PS2Sprite[i].Delete();

	int		nSlot = CTxdStore::FindTxdSlot("fonts");
	if ( nSlot != -1 )
		CTxdStore::RemoveTxdSlot(nSlot);

	nSlot = CTxdStore::FindTxdSlot("buttons");
	if ( nSlot != -1 )
		CTxdStore::RemoveTxdSlot(nSlot);
}

void CFont::GetButtonName(unsigned short eventNo, char* wcBuf, short nUnused)
{
	size_t		nStartPoint = strlen(wcBuf);
	WORD		wPadMode = CPad::GetPad(0)->Mode;

	UNREFERENCED_PARAMETER(nUnused);

	switch ( eventNo )
	{
	case 0:	// PED_FIREWEAPON
		switch ( wPadMode )
		{
		case 0:
			GetButtonByIndex(BUTTON_CIRCLE, wcBuf, nStartPoint);
			break;
		case 1:
			GetButtonByIndex(BUTTON_R2, wcBuf, nStartPoint);
			break;
		}
		break;
	case 1:	// PED_FIREWEAPON_ALT
		switch ( wPadMode )
		{
		case 0:
			GetButtonByIndex(BUTTON_L1, wcBuf, nStartPoint);
			break;
		}
		break;
	case 2:	// PED_CYCLE_WEAPON_RIGHT
		switch ( wPadMode )
		{
		case 0:
			GetButtonByIndex(BUTTON_R2, wcBuf, nStartPoint);
			break;
		case 1:
			GetButtonByIndex(BUTTON_RIGHT, wcBuf, nStartPoint);
			break;
		}
		break;
	case 3:	// PED_CYCLE_WEAPON_LEFT
		switch ( wPadMode )
		{
		case 0:
			GetButtonByIndex(BUTTON_L2, wcBuf, nStartPoint);
			break;
		case 1:
			GetButtonByIndex(BUTTON_LEFT, wcBuf, nStartPoint);
			break;
		}
		break;
	case 4:	// GO_FORWARD
		if ( bSouthpaw[0] )
			GetButtonByIndex(BUTTON_THUMBRYU, wcBuf, nStartPoint);
		else
			GetButtonByIndex(BUTTON_THUMBLYU, wcBuf, nStartPoint);
		break;
	case 5:	// GO_BACK
		if ( bSouthpaw[0] )
			GetButtonByIndex(BUTTON_THUMBRYD, wcBuf, nStartPoint);
		else
			GetButtonByIndex(BUTTON_THUMBLYD, wcBuf, nStartPoint);
		break;
	case 6:	// GO_LEFT
		if ( bSouthpaw[0] )
			GetButtonByIndex(BUTTON_THUMBRXL, wcBuf, nStartPoint);
		else
			GetButtonByIndex(BUTTON_THUMBLXL, wcBuf, nStartPoint);
		break;
	case 7:	// GO_RIGHT
		if ( bSouthpaw[0] )
			GetButtonByIndex(BUTTON_THUMBRXR, wcBuf, nStartPoint);
		else
			GetButtonByIndex(BUTTON_THUMBLXR, wcBuf, nStartPoint);
		break;
	case 8: // PED_SNIPER_ZOOM_IN
		switch ( wPadMode )
		{
		case 0:
			GetButtonByIndex(BUTTON_R2, wcBuf, nStartPoint);
			break;
		case 1:
			if ( bSwapSticksDuringAiming[0] )
				GetButtonByIndex(BUTTON_THUMBRYU, wcBuf, nStartPoint);
			else
				GetButtonByIndex(BUTTON_THUMBLYU, wcBuf, nStartPoint);
			break;
		}
		break;
	case 9:	// PED_SNIPER_ZOOM_OUT
		switch ( wPadMode )
		{
		case 0:
			GetButtonByIndex(BUTTON_L2, wcBuf, nStartPoint);
			break;
		case 1:
			if ( bSwapSticksDuringAiming[0] )
				GetButtonByIndex(BUTTON_THUMBRYD, wcBuf, nStartPoint);
			else
				GetButtonByIndex(BUTTON_THUMBLYD, wcBuf, nStartPoint);
			break;
		}
		break;
	case 10:	// VEHICLE_ENTER_EXIT
		GetButtonByIndex(BUTTON_TRIANGLE, wcBuf, nStartPoint);
		break;
	case 11:	// CAMERA_CHANGE_VIEW_ALL_SITUATIONS
		GetButtonByIndex(BUTTON_SELECT, wcBuf, nStartPoint);
		break;
	case 12:	// PED_JUMPING
		GetButtonByIndex(BUTTON_SQUARE, wcBuf, nStartPoint);
		break;
	case 13:	// PED_SPRINT
		GetButtonByIndex(BUTTON_CROSS, wcBuf, nStartPoint);
		break;
	case 14:	// PED_LOOKBEHIND
		if ( bSouthpaw[0] )
			GetButtonByIndex(BUTTON_L3, wcBuf, nStartPoint);
		else
			GetButtonByIndex(BUTTON_R3, wcBuf, nStartPoint);
		break;
	case 15:	// PED_DUCK
		if ( bSouthpaw[0] )
			GetButtonByIndex(BUTTON_R3, wcBuf, nStartPoint);
		else
			GetButtonByIndex(BUTTON_L3, wcBuf, nStartPoint);
		break;
	case 16:	// PED_ANSWER_PHONE
		GetButtonByIndex(BUTTON_L1, wcBuf, nStartPoint);
		break;
	case 18:	// VEHICLE_FIREWEAPON
		GetButtonByIndex(BUTTON_CIRCLE, wcBuf, nStartPoint);
		break;
	case 19:	// VEHICLE_FIREWEAPON_ALT
		switch ( wPadMode )
		{
		case 0:
			GetButtonByIndex(BUTTON_L1, wcBuf, nStartPoint);
			break;
		case 1:
			GetButtonByIndex(BUTTON_CROSS, wcBuf, nStartPoint);
			break;
		}
		break;
	case 20:	// VEHICLE_STEERLEFT
		if ( bSouthpaw[0] )
			GetButtonByIndex(BUTTON_THUMBRXL, wcBuf, nStartPoint);
		else
			GetButtonByIndex(BUTTON_THUMBLXL, wcBuf, nStartPoint);
		break;
	case 21:	// VEHICLE_STEERRIGHT
		if ( bSouthpaw[0] )
			GetButtonByIndex(BUTTON_THUMBRXR, wcBuf, nStartPoint);
		else
			GetButtonByIndex(BUTTON_THUMBLXR, wcBuf, nStartPoint);
		break;
	case 22:	// VEHICLE_STEERUP
		if ( bSouthpaw[0] )
			GetButtonByIndex(BUTTON_THUMBRYU, wcBuf, nStartPoint);
		else
			GetButtonByIndex(BUTTON_THUMBLYU, wcBuf, nStartPoint);
		break;
	case 23:	// VEHICLE_STEERDOWN
		if ( bSouthpaw[0] )
			GetButtonByIndex(BUTTON_THUMBRYD, wcBuf, nStartPoint);
		else
			GetButtonByIndex(BUTTON_THUMBLYD, wcBuf, nStartPoint);
		break;
	case 24:	// VEHICLE_ACCELERATE
		switch ( wPadMode )
		{
		case 0:
			GetButtonByIndex(BUTTON_CROSS, wcBuf, nStartPoint);
			break;
		case 1:
			GetButtonByIndex(BUTTON_R2, wcBuf, nStartPoint);
			break;
		}
		break;
	case 25:	// VEHICLE_BRAKE
		switch ( wPadMode )
		{
		case 0:
			GetButtonByIndex(BUTTON_SQUARE, wcBuf, nStartPoint);
			break;
		case 1:
			GetButtonByIndex(BUTTON_L2, wcBuf, nStartPoint);
			break;
		}
		break;
	case 26:	// VEHICLE_RADIO_STATION_UP
		switch ( wPadMode )
		{
		case 0:
			GetButtonByIndex(BUTTON_UP, wcBuf, nStartPoint);
			break;
		case 1:
			GetButtonByIndex(BUTTON_RIGHT, wcBuf, nStartPoint);
			break;
		}
		break;
	case 27:	// VEHICLE_RADIO_STATION_DOWN
		switch ( wPadMode )
		{
		case 0:
			GetButtonByIndex(BUTTON_DOWN, wcBuf, nStartPoint);
			break;
		case 1:
			GetButtonByIndex(BUTTON_LEFT, wcBuf, nStartPoint);
			break;
		}
		break;
	case 29:	// VEHICLE_HORN
		if ( bSouthpaw[0] )
			GetButtonByIndex(BUTTON_R3, wcBuf, nStartPoint);
		else
			GetButtonByIndex(BUTTON_L3, wcBuf, nStartPoint);
		break;
	case 30:	// TOGGLE_SUBMISSIONS
		if ( bSouthpaw[0] )
			GetButtonByIndex(BUTTON_L3, wcBuf, nStartPoint);
		else
			GetButtonByIndex(BUTTON_R3, wcBuf, nStartPoint);
		break;
	case 31:	// VEHICLE_HANDBRAKE
		switch ( wPadMode )
		{
		case 0:
			GetButtonByIndex(BUTTON_R1, wcBuf, nStartPoint);
			break;
		case 1:
			GetButtonByIndex(BUTTON_CROSS, wcBuf, nStartPoint);
			break;
		}
		break;
	case 32:	// PED_1RST_PERSON_LOOK_LEFT
		if ( bSouthpaw[0] )
			GetButtonByIndex(BUTTON_THUMBLXL, wcBuf, nStartPoint);
		else
			GetButtonByIndex(BUTTON_THUMBRXL, wcBuf, nStartPoint);
		break;
	case 33:	// PED_1RST_PERSON_LOOK_RIGHT
		if ( bSouthpaw[0] )
			GetButtonByIndex(BUTTON_THUMBLXR, wcBuf, nStartPoint);
		else
			GetButtonByIndex(BUTTON_THUMBRXR, wcBuf, nStartPoint);
		break;
	case 34:	// VEHICLE_LOOKLEFT
		switch ( wPadMode )
		{
		case 0:
			GetButtonByIndex(BUTTON_L2, wcBuf, nStartPoint);
			break;
		case 1:
			GetButtonByIndex(BUTTON_L1, wcBuf, nStartPoint);
			break;
		}
		break;
	case 35:	// VEHICLE_LOOKRIGHT
		switch ( wPadMode )
		{
		case 0:
			GetButtonByIndex(BUTTON_R2, wcBuf, nStartPoint);
			break;
		case 1:
			GetButtonByIndex(BUTTON_R1, wcBuf, nStartPoint);
			break;
		}
		break;
	case 36:	// VEHICLE_LOOKBEHIND
		switch ( wPadMode )
		{
		case 0:
			GetButtonByIndex(BUTTON_L2, BUTTON_R2, wcBuf, nStartPoint);
			break;
		case 1:
			GetButtonByIndex(BUTTON_L1, BUTTON_R1, wcBuf, nStartPoint);
			break;
		}
		break;
	case 38:	// VEHICLE_TURRETLEFT
		if ( bSouthpaw[0] )
			GetButtonByIndex(BUTTON_THUMBLXL, wcBuf, nStartPoint);
		else
			GetButtonByIndex(BUTTON_THUMBRXL, wcBuf, nStartPoint);
		break;
	case 39:	// VEHICLE_TURRETRIGHT
		if ( bSouthpaw[0] )
			GetButtonByIndex(BUTTON_THUMBLXR, wcBuf, nStartPoint);
		else
			GetButtonByIndex(BUTTON_THUMBRXR, wcBuf, nStartPoint);
		break;
	case 40:	// VEHICLE_TURRETUP
		if ( bSouthpaw[0] )
			GetButtonByIndex(BUTTON_THUMBLYD, wcBuf, nStartPoint);
		else
			GetButtonByIndex(BUTTON_THUMBRYD, wcBuf, nStartPoint);
		break;
	case 41:	// VEHICLE_TURRETDOWN
		if ( bSouthpaw[0] )
			GetButtonByIndex(BUTTON_THUMBLYU, wcBuf, nStartPoint);
		else
			GetButtonByIndex(BUTTON_THUMBRYU, wcBuf, nStartPoint);
		break;
	case 42:	// PED_CYCLE_TARGET_LEFT
		switch ( wPadMode )
		{
		case 0:
			GetButtonByIndex(BUTTON_L2, wcBuf, nStartPoint);
			break;
		case 1:
			GetButtonByIndex(BUTTON_L1, wcBuf, nStartPoint);
			break;
		}
		break;
	case 43:	// PED_CYCLE_TARGET_RIGHT
		switch ( wPadMode )
		{
		case 0:
			GetButtonByIndex(BUTTON_R2, wcBuf, nStartPoint);
			break;
		case 1:
			GetButtonByIndex(BUTTON_R1, wcBuf, nStartPoint);
			break;
		}
		break;
	case 44:	// PED_CENTER_CAMERA_BEHIND_PLAYER
		switch ( wPadMode )
		{
		case 0:
			GetButtonByIndex(BUTTON_L1, wcBuf, nStartPoint);
			break;
		case 1:
			GetButtonByIndex(BUTTON_R1, wcBuf, nStartPoint);
			break;
		}
		break;
	case 45:	// PED_LOCK_TARGET
		switch ( wPadMode )
		{
		case 0:
			GetButtonByIndex(BUTTON_R1, wcBuf, nStartPoint);
			break;
		case 1:
			GetButtonByIndex(BUTTON_L2, wcBuf, nStartPoint);
			break;
		}
		break;
	case 46:	// NETWORK_TALK
		GetButtonByIndex(BUTTON_PS, wcBuf, nStartPoint);
		break;
	case 47:	// CONVERSATION_YES
		switch ( wPadMode )
		{
		case 0:
			GetButtonByIndex(BUTTON_RIGHT, wcBuf, nStartPoint);
			break;
		case 1:
			GetButtonByIndex(BUTTON_UP, wcBuf, nStartPoint);
			break;
		}
		break;
	case 48:	// CONVERSATION_NO
		switch ( wPadMode )
		{
		case 0:
			GetButtonByIndex(BUTTON_LEFT, wcBuf, nStartPoint);
			break;
		case 1:
			GetButtonByIndex(BUTTON_DOWN, wcBuf, nStartPoint);
			break;
		}
		break;
	case 49:	// GROUP_CONTROL_FWD
		switch ( wPadMode )
		{
		case 0:
			GetButtonByIndex(BUTTON_UP, wcBuf, nStartPoint);
			break;
		case 1:
			GetButtonByIndex(BUTTON_CROSS, BUTTON_UP, wcBuf, nStartPoint);
			break;
		}
		break;
	case 50:	// GROUP_CONTROL_BWD
		switch ( wPadMode )
		{
		case 0:
			GetButtonByIndex(BUTTON_DOWN, wcBuf, nStartPoint);
			break;
		case 1:
			GetButtonByIndex(BUTTON_CROSS, BUTTON_DOWN, wcBuf, nStartPoint);
			break;
		}
		break;
	case 51:	// PED_1RST_PERSON_LOOK_UP
		if ( bSouthpaw[0] )
			GetButtonByIndex(BUTTON_THUMBLYU, wcBuf, nStartPoint);
		else
			GetButtonByIndex(BUTTON_THUMBRYU, wcBuf, nStartPoint);
		break;
	case 52:	// PED_1RST_PERSON_LOOK_DOWN
		if ( bSouthpaw[0] )
			GetButtonByIndex(BUTTON_THUMBLYD, wcBuf, nStartPoint);
		else
			GetButtonByIndex(BUTTON_THUMBRYD, wcBuf, nStartPoint);
		break;
	}

	wcBuf[nStartPoint] = '\0';
}

void CFont::GetButtonByIndex(int wButton1, int wButton2, char* wcBuf, size_t& nStartPoint)
{
	GetButtonByIndex(wButton1, wcBuf, nStartPoint);

	wcBuf[nStartPoint++] = ' ';
	wcBuf[nStartPoint++] = '+';
	wcBuf[nStartPoint++] = ' ';

	GetButtonByIndex(wButton2, wcBuf, nStartPoint);
}

void __declspec(naked) GetButtonNameHack()
{
	_asm
	{
		// TODO: Fix, this is incorrect
		cmp		ax, 46
		jz		GetButtonNameHack_PadInput
		cmp		ax, 17
		jz		GetButtonNameHack_KeyboardInput
		cmp		ax, 37
		jz		GetButtonNameHack_KeyboardInput

		mov		eax, [pXboxPad]
		cmp		[eax].bHasPadInHands, 0
		jz		GetButtonNameHack_KeyboardInput

GetButtonNameHack_PadInput:
		jmp		CFont::GetButtonName

GetButtonNameHack_KeyboardInput:
		mov		eax, 5303D0h
		jmp		eax
	}
}

char* __stdcall AddTokenToWidth(char* pText, float& fPos)
{
	CRGBA		TempColour;
	char*	pNewPtr = CFont::ParseToken(pText, TempColour, true, nullptr) - 1;
	if ( CFont::PS2Symbol != BUTTON_NONE )
	{
		CFont::PS2Symbol = BUTTON_NONE;
		fPos += fSymbolSpriteWidth * CFont::Details.textLetterSizeY;
	}

	return pNewPtr;
}

void __declspec(naked) TokenWidthHook()
{
	_asm
	{
		test	dl, dl
		jz		TokenWidthHook_Return
		sub		esi, nTokenWidth

TokenWidthHook_Return:
		ret
	}
}

void CFont::Inject()
{
	using namespace Memory;

	// Buttons
	Patch<const void*>(0x718AE1, PS2Sprite);

	Patch<BYTE>(0x5303E1, 0xEB);
	InjectHook(0x69E259, GetButtonNameHack);

	// Refreshing helps
	//InjectHook(0x47B8B2, &CMenuManager::NeedsToRefreshHelps);

	// Tokens in width
	InjectHook(0x719965, ParseToken);
	InjectHook(0x71A018, ParseToken);
	InjectHook(0x71A2C4, ParseToken);

	Patch<DWORD>(0x71A181, 0x0C24448D);
	Patch<WORD>(0x71A185, 0x5650);
	InjectHook(0x71A187, AddTokenToWidth, PATCH_CALL);
	Patch<DWORD>(0x71A18C, 0x08EBF08B);
	InjectHook(0x71A336, TokenWidthHook, PATCH_CALL);

	// Variable-width sprites
	Patch<const void*>(0x718A98, &fSymbolSpriteWidth);
	Patch<const void*>(0x719A55, &fSymbolSpriteWidth);
}


static StaticPatcher	Patcher([](){ 
						CFont::Inject();
					});