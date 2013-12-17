#include "StdAfx.h"

// Wrappers
WRAPPER void CSprite2d::SetVertices(const CRect& rect, const CRGBA& rgb1, const CRGBA& rgb2, const CRGBA& rgb3, const CRGBA& rgb4)
{ WRAPARG(rect); WRAPARG(rgb1); WRAPARG(rgb2); WRAPARG(rgb3); WRAPARG(rgb4); EAXJMP(0x727420); }
WRAPPER void CSprite2d::InitPerFrame() { EAXJMP(0x727350); }

void CSprite2d::SetTexture(const char* name, const char* maskName)
{
	if ( name )
	{
		if ( maskName )
		{
			if ( m_pTexture )
				RwTextureDestroy(m_pTexture);

			m_pTexture = RwTextureRead(name, maskName);
		}
		else
			SetTextureNoMask(name);
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