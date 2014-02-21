#ifndef __FONT
#define __FONT

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
		float					fUnk4;
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

private:
	static void				LoadFontValues();

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

	static void				Initialise();
	static void				Shutdown();
};

const char*		GetFontsDATByLanguage();
const char*		GetFontsTXDByLanguage();
eFontFileIDs	GetFontsIDByLanguage();

static_assert(sizeof(CFont::Details) == 0x40 && sizeof(CFont::sFontSizes) == 0xD2, "Wrong size: CFont");

#endif