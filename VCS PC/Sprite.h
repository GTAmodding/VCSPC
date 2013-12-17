#ifndef __SPRITE
#define __SPRITE

#define NUM_LOADING_SPLASHES		12

class CSprite2d
{
private:
	RwTexture*		 m_pTexture;

public:
	inline CSprite2d()
		: m_pTexture(nullptr)
	{}

	inline ~CSprite2d()
		{ Delete(); }

	inline void			SetAddressing(int nAddressing)
		{ if ( m_pTexture ) RwTextureSetAddressing(m_pTexture, nAddressing); }

	void				SetTexture(const char* name, const char* maskName);
	void				SetTextureNoMask(const char* name);
	bool				SetTextureFromSPTA(CPNGArchive& pArchive, const char* pName);
	void				Delete();
	void				Draw(const CRect& rect, const CRGBA& colour);
	void				Draw(float fPosX, float fPosY, float fWidth, float fHeight, const CRGBA& colour);

	static void			InitPerFrame();
	static void			SetVertices(const CRect& rect, const CRGBA& rgb1, const CRGBA& rgb2, const CRGBA& rgb3, const CRGBA& rgb4);
	static void			DrawRect(const CRect& rect, const CRGBA& colour);
};


#endif