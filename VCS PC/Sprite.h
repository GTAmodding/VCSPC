#ifndef __SPRITE
#define __SPRITE

#define WIN32_LEAN_AND_MEAN

#define FUNC_CSprite2d__DrawTexturedRect						0x728350
#define FUNC_CSprite2d__Load									0x7272B0
#define FUNC_CSprite2d__Reload								0x727270
#define FUNC_CSprite2d__Clean								0x727240
#define FUNC_CSprite2d__DrawRadioIcon						0x7282C0
#define FUNC_CSprite2d__SetVertices							0x727420

#define FUNC_CSprite2d__ReadLoadingTextures					0x5900B0

#define NUM_LOADING_SPLASHES		12

class CSprite2d
{
private:
	RwTexture*		 m_pTexture;

public:
	CSprite2d()
		: m_pTexture(nullptr)
	{
	};

	inline void			SetAddressing(int nAddressing)
		{ if ( m_pTexture ) RwTextureSetAddressing(m_pTexture, nAddressing); };

	void				DrawTexturedRect(CRect* pos, CRGBA* color);
	void				SetTexture(const char* name, const char* maskName);
	void				SetTextureNoMask(const char* name);
	bool				SetTextureFromSPTA(CPNGArchive& pArchive, const char* pName);
	void				Clean();
	void				DrawRadioIcon(float posX, float posY, float width, float height, CRGBA* colour);

	static void			SetVertices(const CRect& rect, const CRGBA& rgb1, const CRGBA& rgb2, const CRGBA& rgb3, const CRGBA& rgb4);
	static void			DrawRect(const CRect& rect, const CRGBA& colour);

	/*void				Reload(const char* name);
	void				Reload(const char* name, const char* maskName);*/
	static void			ReadLoadingTextures(bool introSplashFlag, unsigned char introSplashID);
};


#endif