#ifndef __CFONT
#define __CFONT

#define FUNC_CFont__ResetSlantedTextPos						0x719400
#define FUNC_CFont__SetTextSlanted							0x719420			
#define FUNC_CFont__SetTextColour							0x719430
#define FUNC_CFont__SetFontStyle							0x719490
#define FUNC_CFont__SetTextWrapX							0x7194F0
#define FUNC_CFont__SetTextBorderRGBA						0x719510
#define FUNC_CFont__SetTextShadow							0x719570
#define FUNC_CFont__SetTextOutline							0x719590
#define FUNC_CFont__SetTextUseProportionalValues			0x7195B0
#define FUNC_CFont__SetTextBackground						0x7195C0
#define FUNC_CFont__SetTextJustify							0x719600
#define FUNC_CFont__SetTextAlignment						0x719610

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
	static void				SetColor(CRGBA colour);
	static void				SetTextBorderRGBA(CRGBA colour);
	static void				SetTextShadow(unsigned char bShadow);
	static void				SetTextOutline(unsigned char bOutline);
	static void				ResetSlantedTextPos(float fOne, float fTwo);
	static void				SetTextSlanted(float fAngle);
	static void				SetCentreSize(float fUnk);
	static void				SetTextWrapX(float fWrap);
	static void				SetTextJustify(bool bJustify);
	static void				func_71A210();
	static void				SetTextUseProportionalValues(bool bUseProportionalValues);
	static void				SetTextBackground(unsigned char bUseBackground, unsigned char bBackgroundType);
	static void				SetTextAlignment(unsigned char bAlign);
	static void				PrintString(float posX, float posY, const char* pText);
	static void				PrintStringFromBottom(float posX, float posY, const char* pText);
	static void				SetWrapx(float fWrap);

	static void				Initialise();
	static void				Shutdown();
};

const char*		GetFontsDATByLanguage();
const char*		GetFontsTXDByLanguage();
eFontFileIDs	GetFontsIDByLanguage();

static_assert(sizeof(CFont::Details) == CFont_ARRAYSIZE && sizeof(CFont::sFontSizes) == 0xD2, "CFont class has wrong size!");

#endif