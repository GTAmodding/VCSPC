#include "StdAfx.h"
#include "Sprite.h"

// Wrappers
WRAPPER bool CSprite::CalcScreenCoors(const RwV3d& vecIn, RwV3d* vecOut, float* fWidth, float* fHeight, bool bCheckFarClip, bool bCheckNearClip) { EAXJMP(0x70CE30); }
WRAPPER void CSprite::FlushSpriteBuffer() { EAXJMP(0x70CF20); }
WRAPPER void CSprite::RenderOneXLUSprite_Rotate_Aspect(float x, float y, float z, float halfWidth, float halfHeight, 
						unsigned char red, unsigned char green, unsigned char blue, short alpha, float rhw, 
						float rotate, unsigned char aspect) { EAXJMP(0x70D490); }
WRAPPER void CSprite::RenderBufferedOneXLUSprite_Rotate_Aspect(float x, float y, float z, float halfWidth, float halfHeight, 
						unsigned char red, unsigned char green, unsigned char blue, short alpha, float rhw, 
						float rotate, unsigned char aspect) { EAXJMP(0x70E780); }
WRAPPER void CSprite::RenderOneXLUSprite(float x, float y, float z, float halfWidth, float halfHeight, 
						unsigned char red, unsigned char green, unsigned char blue, short alpha, float rhw, 
						unsigned char intensity, unsigned char udir, unsigned char vdir) { EAXJMP(0x70D000); }
WRAPPER void CSprite::RenderBufferedOneXLUSprite2D(float, float, float, float, const RwRGBA&, short, unsigned char)
						{ EAXJMP(0x70F440); }

WRAPPER void CSprite2d::SetVertices(const CRect& rect, const CRGBA& rgb1, const CRGBA& rgb2, const CRGBA& rgb3, const CRGBA& rgb4)
{ WRAPARG(rect); WRAPARG(rgb1); WRAPARG(rgb2); WRAPARG(rgb3); WRAPARG(rgb4); EAXJMP(0x727420); }
// TODO: Name params
WRAPPER void CSprite2d::SetVertices(const CRect&, const CRGBA&, const CRGBA&, const CRGBA&, const CRGBA&, float, float, float, float, float, float, float, float) { EAXJMP(0x727710); }

WRAPPER void CSprite2d::InitPerFrame() { EAXJMP(0x727350); }

void CSprite2d::SetTexture(const char* name, const char* maskName)
{
	if ( name )
	{
		if ( m_pTexture )
			RwTextureDestroy(m_pTexture);

		m_pTexture = RwTextureRead(name, nullptr);
	}
}

void CSprite2d::SetTextureNoMask(const char* name)
{
	if ( name )
	{
		if ( m_pTexture )
			RwTextureDestroy(m_pTexture);

		m_pTexture = RwTextureRead(name, nullptr);
	}
}

bool CSprite2d::SetTextureFromSPTA(CPNGArchive& pArchive, const char* pName)
{
	if ( pName )
	{
		if ( m_pTexture )
			RwTextureDestroy(m_pTexture);

		m_pTexture = pArchive.ReadTexture(pName);
		return m_pTexture != nullptr;
	}
	return false;
}

/*void CSprite2d::SetTextureFromCache(CPNGAccel* pCache, const char* pName)
{
	if ( pName )
	{
		if ( m_pTexture )
			RwTextureDestroy(m_pTexture);

		m_pTexture = pCache->ReadTexture(pName);
	}
}*/

void CSprite2d::Delete()
{
	if ( m_pTexture )
	{
		RwTextureDestroy(m_pTexture);
		m_pTexture = nullptr;
	}
}

void CSprite2d::Draw(const CRect& rect, const CRGBA& colour)
{
	SetVertices(rect, colour, colour, colour, colour);
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, m_pTexture ? m_pTexture->raster : nullptr);
	RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, aSpriteVertices, 4);
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, nullptr);
}

void CSprite2d::Draw(float fPosX, float fPosY, float fWidth, float fHeight, const CRGBA& colour)
{
	SetVertices(CRect(fPosX, fPosY + fHeight, fPosX + fWidth, fPosY), colour, colour, colour, colour);
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, m_pTexture ? m_pTexture->raster : nullptr);
	RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, aSpriteVertices, 4);
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, nullptr);
}

void CSprite2d::DrawRect(const CRect& rect, const CRGBA& colour)
{
	SetVertices(rect, colour, colour, colour, colour);
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, nullptr);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, reinterpret_cast<void*>(colour.a != 255));
	RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, aSpriteVertices, 4);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, FALSE);
}