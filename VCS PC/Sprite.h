#ifndef __SPRITE
#define __SPRITE

#include "PNGArchive.h"
#include "General.h"

#define NUM_LOADING_SPLASHES		12

class CSprite
{
public:
	static bool		CalcScreenCoors(const RwV3d& vecIn, RwV3d* vecOut, float* fWidth, float* fHeight, bool bCheckFarClip, bool bCheckNearClip);
	static void		FlushSpriteBuffer();

	static void		RenderOneXLUSprite_Rotate_Aspect(float x, float y, float z, float halfWidth, float halfHeight, 
						unsigned char red, unsigned char green, unsigned char blue, short alpha, float rhw, 
						float rotate, unsigned char aspect);

	static void		RenderBufferedOneXLUSprite_Rotate_Aspect(float x, float y, float z, float halfWidth, float halfHeight, 
						unsigned char red, unsigned char green, unsigned char blue, short alpha, float rhw, 
						float rotate, unsigned char aspect);
};

class CSprite2d
{
private:
	RwTexture*		 m_pTexture;

public:
	inline CSprite2d()
		: m_pTexture(nullptr)
	{}

	inline ~CSprite2d()
		{ /*Delete();*/ }

	inline RwTexture*	GetTexture()
		{ return m_pTexture; }

	inline void			SetAddressing(int nAddressing)
		{ if ( m_pTexture ) RwTextureSetAddressing(m_pTexture, nAddressing); }

	void				SetTexture(const char* name, const char* maskName);
	void				SetTextureNoMask(const char* name);
	bool				SetTextureFromSPTA(CPNGArchive& pArchive, const char* pName);
	//void				SetTextureFromCache(CPNGAccel* pCache, const char* pName);
	void				Delete();
	void				Draw(const CRect& rect, const CRGBA& colour);
	void				Draw(float fPosX, float fPosY, float fWidth, float fHeight, const CRGBA& colour);

	static void			InitPerFrame();
	static void			SetVertices(const CRect& rect, const CRGBA& rgb1, const CRGBA& rgb2, const CRGBA& rgb3, const CRGBA& rgb4);
	static void			SetVertices(const CRect&, const CRGBA&, const CRGBA&, const CRGBA&, const CRGBA&, float, float, float, float, float, float, float, float);
	static void			DrawRect(const CRect& rect, const CRGBA& colour);
};

extern RwIm2DVertex* const	aSpriteVertices;

#endif