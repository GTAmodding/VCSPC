#ifndef __FONT
#define __FONT

#include "General.h"
#include "Sprite.h"

enum	// Align
{
	ALIGN_Center,
	ALIGN_Left,
	ALIGN_Right
};

enum	// Fonts
{
	FONT_RageItalic,
	FONT_Eurostile,
	FONT_PagerFont,
	FONT_Pricedown,
	FONT_Hud
};

// Unused
enum eFontFileIDs
{
	FONTF_European,
	FONTF_Polish,
	NUM_FONTFILES
};

enum eButtonSprites
{
	BUTTON_NONE = 0,

	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,

	BUTTON_CROSS,
	BUTTON_CIRCLE,
	BUTTON_SQUARE,
	BUTTON_TRIANGLE,

	BUTTON_L1,
	BUTTON_L2,
	BUTTON_L3,
	BUTTON_R1,
	BUTTON_R2,
	BUTTON_R3,

	BUTTON_UPDOWN,
	BUTTON_LEFTRIGHT,

	BUTTON_THUMBL,
	BUTTON_THUMBLX,
	BUTTON_THUMBLXL,
	BUTTON_THUMBLXR,
	BUTTON_THUMBLY,
	BUTTON_THUMBLYU,
	BUTTON_THUMBLYD,

	BUTTON_THUMBR,
	BUTTON_THUMBRX,
	BUTTON_THUMBRXL,
	BUTTON_THUMBRXR,
	BUTTON_THUMBRY,
	BUTTON_THUMBRYU,
	BUTTON_THUMBRYD,

	/*BUTTON_SIXAXIS_1,
	BUTTON_SIXAXIS_2,
	BUTTON_SIXAXIS_3,
	BUTTON_SIXAXIS_4,*/

	BUTTON_PC_UP,
	BUTTON_PC_DOWN,
	BUTTON_PC_LEFT,
	BUTTON_PC_RIGHT,

	NUM_BUTTON_SPRITES,

	// Sprite-less buttons
	BUTTON_START,
	BUTTON_SELECT,
	BUTTON_PS
};

#define NUM_FONT_SHEETS 4

class CFont
{
public:
	struct tDetails
	{
		CRGBA					textDrawBoxColor;
		float					textLetterSizeX;
		float					textLetterSizeY;
		float					fUnk1, fUnk2, fUnk3;
		bool					bUnk1, bUnk2, bUnk3;
		bool					bUseBackground;
		bool					bBackgroundType;
		bool					bPropOn;
		bool					bUnk4;
		float					fAlphaFade;
		CRGBA					rgbaColour;
		long					nUnk1, nUnk2, nUnk3;
		unsigned char			bSpriteToUse;
		unsigned char			bFontStyle;
		signed char				bFontShadow;
		CRGBA					borderRGBA;
		signed char				bFontOutline, bFontOutline2;
	};

#pragma pack(push, 1)
	struct sFontSizes
	{
		unsigned char			bPropValue[208];
		unsigned char			bSpaceChar;
		unsigned char			bUnpropValue;

	};
#pragma pack(pop)

public:
	// TODO: Make private?
	static CSprite2d		Sprite[NUM_FONT_SHEETS];
	static sFontSizes		Size[NUM_FONT_SHEETS];
	static tDetails&		Details;
	static BYTE&			PS2Symbol;

	static float			PS2SpriteWidth[NUM_BUTTON_SPRITES];
	static CSprite2d		PS2Sprite[NUM_BUTTON_SPRITES];
	static bool				bX360Buttons;

private:
	static void				LoadFontValues();
	static void				GetButtonByIndex(int wButton, char* wcBuf, size_t& nStartPoint);
	static void				GetButtonByIndex(int wButton1, int wButton2, char* wcBuf, size_t& nStartPoint);

public:
	static unsigned char	FindSubFontCharacter(char character, unsigned char bFontType);
	static void				SetScale(float scaleX, float scaleY);
	static void				SetScaleLang(float scaleX, float scaleY);
	static void				SetFontStyle(unsigned char bFont);
	static void				SetColor(CRGBA color);
	static void				SetDropColor(CRGBA color);
	static void				SetDropShadowPosition(signed char bShadow);
	static void				SetEdge(signed char bEdge);
	static void				SetSlantRefPoint(float fOne, float fTwo);
	static void				SetSlant(float fAngle);
	static void				SetCentreSize(float fSize);
	static void				SetRightJustifyWrap(float fWrap);
	static void				SetJustify(bool bJustify);
	static void				RenderFontBuffer();
	static void				SetProportional(bool bProportional);
	static void				SetBackground(unsigned char bUseBackground, unsigned char bBackgroundType);
	static void				SetOrientation(unsigned char bOrientation);
	static void				PrintString(float posX, float posY, const char* pText);
	static void				PrintStringFromBottom(float posX, float posY, const char* pText);
	static void				SetWrapx(float fWrap);
	static void				InitPerFrame();
	static void				SetAlphaFade(float fAlpha);
	static int				GetNumberLines(float, float, const char* pText);

	static void	__stdcall	GetButtonName(unsigned short eventNo, char* wcBuf, short nUnused);
	static char*			ParseToken(char* pText, CRGBA& rColor, bool bIgnoreColors, char* pChar);
	static void				Initialise();
	static void				Shutdown();

	static void				Inject();
};

const char*		GetFontsDATByLanguage();
const char*		GetFontsTXDByLanguage();
eFontFileIDs	GetFontsIDByLanguage();

static_assert(sizeof(CFont::Details) == 0x40 && sizeof(CFont::sFontSizes) == 0xD2, "Wrong size: CFont");

#endif