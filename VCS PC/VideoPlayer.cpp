#include "StdAfx.h"
#include "VideoPlayer.h"

#include "Rs.h"
#include "Sprite.h"

BINK*				CVideoPlayer::m_hBinkPlayer;
RwRaster*			CVideoPlayer::m_pVideoRaster;
CRect				CVideoPlayer::m_videoFrame;
unsigned char		CVideoPlayer::m_bSurfaceMask;
unsigned char		CVideoPlayer::m_bExtraThreadIndex;

void CVideoPlayer::UpdateVideoFrame(const CRect* pVideoFrame, const CVector2D& vecScale)
{
	if ( !pVideoFrame )
	{
		// Fullscreen
		RwRGBA	colour;

		colour.red = 0;
		colour.green = 0;
		colour.blue = 0;
		colour.alpha = 255;

		m_videoFrame.x1 = 0.5f * (RsGlobal.MaximumWidth - vecScale.x);
		m_videoFrame.y1 = 0.5f * (RsGlobal.MaximumHeight + vecScale.y);
		m_videoFrame.x2 = 0.5f * (RsGlobal.MaximumWidth + vecScale.x);
		m_videoFrame.y2 = 0.5f * (RsGlobal.MaximumHeight - vecScale.y);

		RwCameraClear(Camera, &colour, rwCAMERACLEARIMAGE);
	}
	else
	{
		// Window
		m_videoFrame = *pVideoFrame;
	}
}

void CVideoPlayer::Create(const char* pFileName, const CRect* pVideoFrame, bool bAudio, bool bBlackWhite)
{
	SYSTEM_INFO		info;
	unsigned int	openFlags;

	GetSystemInfo(&info);

	BinkStartAsyncThread(0, nullptr);
	if ( info.dwNumberOfProcessors > 1 )
	{
		BinkStartAsyncThread(1, nullptr);
		m_bExtraThreadIndex = 1;
	}
	else
		m_bExtraThreadIndex = 0;

	if ( bAudio )
	{
		BinkSoundUseDirectSound(nullptr);
		openFlags = 0;
	}
	else
	{
		BinkSetSoundTrack(0, nullptr);
		openFlags = BINKSNDTRACK;
	}
	if ( bBlackWhite )
		openFlags |= BINKGRAYSCALE;

	m_hBinkPlayer = BinkOpen(pFileName, openFlags);
	if ( m_hBinkPlayer )
	{
		BinkDoFrameAsync(m_hBinkPlayer, 0, m_bExtraThreadIndex);

		CVector2D	vecOutScale;
		if ( !pVideoFrame )
			vecOutScale = WidescreenSupport::GetFullscreenImageDimensions(static_cast<float>(m_hBinkPlayer->Width)/m_hBinkPlayer->Height, WidescreenSupport::SetAspectRatio(), true);
		else
		{
			vecOutScale.x = abs(pVideoFrame->x2 - pVideoFrame->x1);
			vecOutScale.y = abs(pVideoFrame->y2 - pVideoFrame->y1);
		}

		m_pVideoRaster = RwRasterCreate(m_hBinkPlayer->Width, m_hBinkPlayer->Height, 0, rwRASTERFORMATDEFAULT | rwRASTERTYPECAMERATEXTURE);
		m_bSurfaceMask = ( RwRasterGetFormat(m_pVideoRaster) == rwRASTERFORMAT565 ) ? BINKSURFACE565 : BINKSURFACE32;

		UpdateVideoFrame(pVideoFrame, vecOutScale);
	}
}

void CVideoPlayer::Release()
{
	if ( m_hBinkPlayer )
	{
		BinkClose(m_hBinkPlayer);
		m_hBinkPlayer = nullptr;

		BinkRequestStopAsyncThread(0);
		if ( m_bExtraThreadIndex )
			BinkRequestStopAsyncThread(1);
		BinkWaitStopAsyncThread(0);
		if ( m_bExtraThreadIndex )
			BinkWaitStopAsyncThread(1);

		RwRasterDestroy(m_pVideoRaster);
	}
}

bool CVideoPlayer::PlayNextFullscreenFrame()
{
	if ( m_hBinkPlayer )
	{
		if ( !BinkWait(m_hBinkPlayer) )
		{
			if ( BinkDoFrameAsyncWait(m_hBinkPlayer, 1000) )
			{
				if ( RsCameraBeginUpdate(Camera) )
				{
					if ( RwUInt8* pPixels = RwRasterLock(m_pVideoRaster, 0, rwRASTERLOCKREADWRITE) )
					{
						BinkCopyToBuffer(m_hBinkPlayer, pPixels, RwRasterGetStride(m_pVideoRaster), RwRasterGetHeight(m_pVideoRaster), 0, 0, m_bSurfaceMask);
						RwRasterUnlock(m_pVideoRaster);
					}

					CRGBA	frameColour(255, 255, 255, 255);
					CSprite2d::SetVertices(m_videoFrame, frameColour, frameColour, frameColour, frameColour);
					RwRenderStateSet(rwRENDERSTATETEXTURERASTER, m_pVideoRaster);
					RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, aSpriteVertices, 4);
					RwRenderStateSet(rwRENDERSTATETEXTURERASTER, nullptr);
				}

				if ( m_hBinkPlayer->LastFrameNum >= m_hBinkPlayer->Frames )
					return false;

				BinkNextFrame(m_hBinkPlayer);

				if ( BinkShouldSkip(m_hBinkPlayer) )
				{
					BinkDoFrameAsync(m_hBinkPlayer, 0, m_bExtraThreadIndex);
					BinkDoFrameAsyncWait(m_hBinkPlayer, -1);
					BinkNextFrame(m_hBinkPlayer);
				}

				BinkDoFrameAsync(m_hBinkPlayer, 0, m_bExtraThreadIndex);

				DoRWStuffEndOfFrame();
			}
		}
		return true;
	}
	else
		return false;
}

void CVideoPlayer::PlayNextFrame()
{
	if ( m_hBinkPlayer )
	{
		if ( !BinkWait(m_hBinkPlayer) )
		{
			if ( BinkDoFrameAsyncWait(m_hBinkPlayer, 1000) )
			{
				if ( RwUInt8* pPixels = RwRasterLock(m_pVideoRaster, 0, rwRASTERLOCKREADWRITE) )
				{
					BinkCopyToBuffer(m_hBinkPlayer, pPixels, RwRasterGetStride(m_pVideoRaster), RwRasterGetHeight(m_pVideoRaster), 0, 0, m_bSurfaceMask);
					RwRasterUnlock(m_pVideoRaster);
				}

				BinkNextFrame(m_hBinkPlayer);

				if ( BinkShouldSkip(m_hBinkPlayer) )
				{
					BinkDoFrameAsync(m_hBinkPlayer, 0, m_bExtraThreadIndex);
					BinkDoFrameAsyncWait(m_hBinkPlayer, -1);
					BinkNextFrame(m_hBinkPlayer);
				}

				BinkDoFrameAsync(m_hBinkPlayer, 0, m_bExtraThreadIndex);
			}
		}
		CRGBA	frameColour(255, 255, 255, 255);
		CSprite2d::SetVertices(m_videoFrame, frameColour, frameColour, frameColour, frameColour);
		RwRenderStateSet(rwRENDERSTATETEXTURERASTER, m_pVideoRaster);
		RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, aSpriteVertices, 4);
		RwRenderStateSet(rwRENDERSTATETEXTURERASTER, nullptr);
	}
}

void CVideoPlayer::WindowProc(WPARAM wParam)
{
	if ( m_hBinkPlayer )
	{
		if ( wParam )
			BinkPause(m_hBinkPlayer, FALSE);
		else
			BinkPause(m_hBinkPlayer, TRUE);
	}
}