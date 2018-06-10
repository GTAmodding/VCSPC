#include "StdAfx.h"
#include "Sprite.h"

// Wrappers
WRAPPER bool CSprite::CalcScreenCoors(const RwV3d& vecIn, RwV3d* vecOut, float* fWidth, float* fHeight, bool bCheckFarClip, bool bCheckNearClip) { EAXJMP(0x70CE30); }
WRAPPER void CSprite::FlushSpriteBuffer() { EAXJMP(0x70CF20); }
WRAPPER void CSprite::InitSpriteBuffer2D() { EAXJMP(0x70CFD0); }
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

WRAPPER float CSprite::CalcHorizonCoors() { EAXJMP(0x70E3E0); }

// Arguments:
// 0---1
// |   |
// 2---3
WRAPPER void CSprite2d::SetVertices(const CRect& rect, const CRGBA& rgb1, const CRGBA& rgb2, const CRGBA& rgb3, const CRGBA& rgb4)
{ WRAPARG(rect); WRAPARG(rgb1); WRAPARG(rgb2); WRAPARG(rgb3); WRAPARG(rgb4); EAXJMP(0x727420); }
// TODO: Name params
WRAPPER void CSprite2d::SetVertices(const CRect&, const CRGBA&, const CRGBA&, const CRGBA&, const CRGBA&, float, float, float, float, float, float, float, float) { EAXJMP(0x727710); }
WRAPPER void CSprite2d::SetVertices(float, float, float, float, float, float, float, float, const CRGBA&, const CRGBA&, const CRGBA&, const CRGBA&) { EAXJMP(0x727590); }
WRAPPER void CSprite2d::SetVertices(int numVerts, float const& v2d1, float const& v2d2, CRGBA const& pRGBA) { WRAPARG(numVerts);  WRAPARG(v2d1);  WRAPARG(v2d2);  WRAPARG(pRGBA); EAXJMP(0x727890); }

WRAPPER void CSprite2d::InitPerFrame() { EAXJMP(0x727350); }

WRAPPER void CSprite2d::Draw2DPolygon(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, CRGBA const& color) {
    WRAPARG(x1); WRAPARG(y1); WRAPARG(x2); WRAPARG(y2); WRAPARG(x3); WRAPARG(y3); WRAPARG(x4);
    WRAPARG(y4); WRAPARG(color); EAXJMP(0x7285B0);
}

WRAPPER void CSprite2d::Draw(CRect const& rect, CRGBA const& color, float u1, float v1, float u2, float v2, float u3, float v3, float u4, float v4) {
    WRAPARG(rect); WRAPARG(color); WRAPARG(u1); WRAPARG(v1); WRAPARG(u2); WRAPARG(v2); WRAPARG(u3);
    WRAPARG(v3); WRAPARG(u4); WRAPARG(v4); WRAPARG(color); EAXJMP(0x728420);
}


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

void CSprite2d::Draw(float fX1, float fY1, float fX2, float fY2, float fX3, float fY3, float fX4, float fY4, const CRGBA& colour)
{
	SetVertices(fX1, fY1, fX2, fY2, fX3, fY3, fX4, fY4, colour, colour, colour, colour);
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, m_pTexture ? m_pTexture->raster : nullptr);
	RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, aSpriteVertices, 4);
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, nullptr);
}

void CSprite2d::DrawRect(const CRect& rect, const CRGBA& colour)
{
	SetVertices(rect, colour, colour, colour, colour);
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, nullptr);
	// shouldn't we use the XLU version for this?
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, reinterpret_cast<void*>(colour.a != 255));
	RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, aSpriteVertices, 4);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, FALSE);
}

void CSprite2d::DrawRect(const CRect& rect, const CRGBA& c1, const CRGBA& c2, const CRGBA& c3, const CRGBA& c4)
{
	SetVertices(rect, c1, c2, c3, c4);
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, nullptr);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, 0);
	RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, aSpriteVertices, 4);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, FALSE);
}

void CSprite2d::DrawAnyRect(float fX1, float fY1, float fX2, float fY2, float fX3, float fY3, float fX4, float fY4, const CRGBA& c1, const CRGBA& c2, const CRGBA& c3, const CRGBA& c4)
{
	SetVertices(fX1, fY1, fX2, fY2, fX3, fY3, fX4, fY4, c1, c2, c3, c4);
	if(c1.a != 255 || c2.a != 255 || c3.a != 255 || c4.a != 255)
		RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
	else
		RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)FALSE);
	RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, aSpriteVertices, 4);
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, nullptr);
}
