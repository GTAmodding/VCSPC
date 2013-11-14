#include "StdAfx.h"

static inline const char* GetTitlePCByLanguage()
{
	static const char* const	cTitlePCNames[] = { "title_pc_EN", /*"title_pc_ES",*/ "title_pc_PL" };
	return cTitlePCNames[menu->GetLanguage()];
}

void CSprite2d::DrawTexturedRect(CRect* pos, CRGBA* color)
{
	DWORD dwFunc = (DWORD)FUNC_CSprite2d__DrawTexturedRect;
	_asm
	{
		mov		eax, color
		push	eax
		mov		eax, pos
		push	eax
		mov		ecx, this
		call	dwFunc
	}
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

		m_pTexture = RwTextureRead(name, NULL);
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

void CSprite2d::Clean()
{
	DWORD dwFunc = (DWORD)FUNC_CSprite2d__Clean;
	_asm
	{
		mov		ecx, this
		call	dwFunc
	}
}

void CSprite2d::DrawRadioIcon(float posX, float posY, float width, float height, CRGBA* colour)
{
	DWORD dwFunc = (DWORD)FUNC_CSprite2d__DrawRadioIcon;
	_asm
	{
		mov		eax, colour
		push	eax
		mov		eax, height
		push	eax
		mov		eax, width
		push	eax
		mov		eax, posY
		push	eax
		mov		eax, posX
		push	eax
		mov		ecx, this
		call	dwFunc
	}
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
				strncpy(SplashName, "intro", 20);
			else
				strncpy(SplashName, "outro", 20);
		}
		else
		{
			if ( i )
				_snprintf(SplashName, 20, "loadsc%d", bFinalIndexes[i]);
			else
				strncpy(SplashName, GetTitlePCByLanguage(), 20);
		}
		loadingTextures[i].SetTextureFromSPTA(LoadscsArchive, SplashName);
	}
	LoadscsArchive.CloseArchive();
}

void CSprite2d::SetVertices(const CRect& rect, const CRGBA& rgb1, const CRGBA& rgb2, const CRGBA& rgb3, const CRGBA& rgb4)
{
	DWORD dwFunc = (DWORD)FUNC_CSprite2d__SetVertices;
	_asm
	{
		push	rgb4
		push	rgb3
		push	rgb2
		push	rgb1
		push	rect
		call	dwFunc
		add		esp, 14h
	}
}

void CSprite2d::DrawRect(const CRect& rect, const CRGBA& colour)
{
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, nullptr);
	SetVertices(rect, colour, colour, colour, colour);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, reinterpret_cast<void*>(colour.a != 255));
	RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, aSpriteVertices, 4);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, FALSE);
}

/*void CSprite2d::Reload(const char* name)
{
	if ( m_pTexture )
	{
		RwTextureDestroy(m_pTexture);
		m_pTexture = NULL;
	}
	if ( name )
		m_pTexture = RwTextureRead(name, NULL);
}

void CSprite2d::Reload(const char* name, const char* maskName)
{
	if ( m_pTexture )
	{
		RwTextureDestroy(m_pTexture);
		m_pTexture = NULL;
	}
	if ( name && maskName )
		m_pTexture = RwTextureRead(name, maskName);
}*/