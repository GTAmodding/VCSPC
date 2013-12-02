#include "StdAfx.h"

// Wrappers
WRAPPER void CSprite2d::SetVertices(const CRect& rect, const CRGBA& rgb1, const CRGBA& rgb2, const CRGBA& rgb3, const CRGBA& rgb4)
{ WRAPARG(rect); WRAPARG(rgb1); WRAPARG(rgb2); WRAPARG(rgb3); WRAPARG(rgb4); EAXJMP(0x727420); }


static inline const char* GetTitlePCByLanguage()
{
	static const char* const	cTitlePCNames[] = { "title_pc_EN", /*"title_pc_ES",*/ "title_pc_PL" };
	return cTitlePCNames[menu->GetLanguage()];
}

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
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, nullptr);
	SetVertices(rect, colour, colour, colour, colour);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, reinterpret_cast<void*>(colour.a != 255));
	RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, aSpriteVertices, 4);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, FALSE);
}

void CSprite2d::ReadLoadingTextures(bool bIntroSplash, unsigned char nIntroSplashID)
{
	unsigned char		bTempIndexes[NUM_LOADING_SPLASHES], bFinalIndexes[NUM_LOADING_SPLASHES];
	LARGE_INTEGER		lPerformanceCount;

//	memset(bTempIndexes, 0, sizeof(bTempIndexes));
	for ( unsigned char i = 0; i < NUM_LOADING_SPLASHES; ++i )
		bTempIndexes[i] = i;

	CPNGArchive		LoadscsArchive("models\\txd\\loadscs.spta");
	LoadscsArchive.SetDirectory(nullptr);

	QueryPerformanceCounter(&lPerformanceCount);
	srand(lPerformanceCount.LowPart);

	for ( int i = 0, j = NUM_LOADING_SPLASHES-2; i < NUM_LOADING_SPLASHES; ++i, --j )
	{
		int		nRandomNumber;

		if ( i )
			nRandomNumber = static_cast<int>(rand() * (1.0f/(RAND_MAX+1.0f)) * (j+1));
		else
			nRandomNumber = 0;

		bFinalIndexes[i] = bTempIndexes[nRandomNumber];
		if ( nRandomNumber < j )
			memcpy(&bTempIndexes[nRandomNumber], &bTempIndexes[nRandomNumber+1], j - nRandomNumber);
	}

	for ( int i = 0; i < 7; ++i )
	{
		char		SplashName[20];
		if ( bIntroSplash )
		{
			if ( nIntroSplashID == 1 )
				strncpy(SplashName, "intro", sizeof(SplashName));
			else
				strncpy(SplashName, "outro", sizeof(SplashName));
		}
		else
		{
			if ( i )
				_snprintf(SplashName, sizeof(SplashName), "loadsc%d", bFinalIndexes[i]);
			else
				strncpy(SplashName, GetTitlePCByLanguage(), sizeof(SplashName));
		}
		loadingTextures[i].SetTextureFromSPTA(LoadscsArchive, SplashName);
	}
	LoadscsArchive.CloseArchive();
}