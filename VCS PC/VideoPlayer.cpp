#include "StdAfx.h"

BINK*				CVideoPlayer::m_hBinkPlayer;
BINKBUFFER*			CVideoPlayer::m_hBinkBuffer;
RwRaster*			CVideoPlayer::m_pVideoRaster;
CRect				CVideoPlayer::m_videoFrame;
unsigned char		CVideoPlayer::m_bSurfaceMask;
unsigned char		CVideoPlayer::m_bExtraThreadIndex;

void CVideoPlayer::UpdateVideoFrame(const CRect* pVideoFrame)
{
	if ( !pVideoFrame )
	{
		// Fullscreen
		RwRGBA	colour;

		colour.red = 0;
		colour.green = 0;
		colour.blue = 0;
		colour.alpha = 255;

		m_videoFrame.x1 = 0.5f * (RsGlobal.MaximumWidth - m_hBinkBuffer->StretchWidth);
		m_videoFrame.y1 = 0.5f * (RsGlobal.MaximumHeight + m_hBinkBuffer->StretchHeight);
		m_videoFrame.x2 = 0.5f * (RsGlobal.MaximumWidth + m_hBinkBuffer->StretchWidth);
		m_videoFrame.y2 = 0.5f * (RsGlobal.MaximumHeight - m_hBinkBuffer->StretchHeight);

		RwCameraClear(Scene, &colour, rwCAMERACLEARIMAGE);
	}
	else
	{
		// Window
		m_videoFrame = *pVideoFrame;
	}
}

void CVideoPlayer::Create(const char* pFileName, const CRect* pVideoFrame, bool bAudio)
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
	m_hBinkPlayer = BinkOpen(pFileName, openFlags);
	if ( m_hBinkPlayer )
	{
		BinkDoFrameAsync(m_hBinkPlayer, 0, m_bExtraThreadIndex);
		m_hBinkBuffer = BinkBufferOpen(RsGlobal.ps->window, m_hBinkPlayer->Width, m_hBinkPlayer->Height, BINKBUFFERSTRETCHXINT | BINKBUFFERSTRETCHYINT/*BINKBUFFERSTRETCHY*/);

		if ( !pVideoFrame )
		{
			unsigned int	dwBinkWindowScale[2];
			double			fVideoAspectRatio = static_cast<double>(m_hBinkPlayer->Width) / m_hBinkPlayer->Height;
			
			if ( static_cast<double>(RsGlobal.MaximumHeight) * fVideoAspectRatio > RsGlobal.MaximumWidth )
			{
				dwBinkWindowScale[0] = RsGlobal.MaximumWidth;
				dwBinkWindowScale[1] = static_cast<unsigned int>(RsGlobal.MaximumWidth / fVideoAspectRatio);
			}
			else
			{
				dwBinkWindowScale[0] = static_cast<unsigned int>(RsGlobal.MaximumHeight * fVideoAspectRatio);
				dwBinkWindowScale[1] = RsGlobal.MaximumHeight;
			}
			BinkBufferSetScale(m_hBinkBuffer, dwBinkWindowScale[0], dwBinkWindowScale[1]);
		}
		else
			BinkBufferSetScale(m_hBinkBuffer, static_cast<unsigned int>(abs(pVideoFrame->x2 - pVideoFrame->x1)), static_cast<unsigned int>(abs(pVideoFrame->y2 - pVideoFrame->y1)));

		m_pVideoRaster = RwRasterCreate(m_hBinkPlayer->Width, m_hBinkPlayer->Height, 32, rwRASTERTYPECAMERATEXTURE);
		m_bSurfaceMask = ( RwRasterGetFormat(m_pVideoRaster) == rwRASTERFORMAT565 ) ? BINKSURFACE565 : BINKSURFACE32;

		UpdateVideoFrame(pVideoFrame);
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

		BinkBufferClose(m_hBinkBuffer);
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
				if ( RsCameraBeginUpdate(Scene) )
				{
					if ( RwRasterLock(m_pVideoRaster, 0, rwRASTERLOCKREADWRITE) )
					{
						if ( BinkBufferLock(m_hBinkBuffer) )
						{
							BinkCopyToBuffer(m_hBinkPlayer, m_pVideoRaster->cpPixels, m_hBinkBuffer->BufferPitch, m_hBinkBuffer->Height, 0, 0, (m_hBinkBuffer->SurfaceType & (~BINKSURFACEMASK)) | m_bSurfaceMask);
							BinkBufferUnlock(m_hBinkBuffer);
						}
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
				if ( RwRasterLock(m_pVideoRaster, 0, rwRASTERLOCKREADWRITE) )
				{
					if ( BinkBufferLock(m_hBinkBuffer) )
					{
						BinkCopyToBuffer(m_hBinkPlayer, m_pVideoRaster->cpPixels, m_hBinkBuffer->BufferPitch, m_hBinkBuffer->Height, 0, 0, (m_hBinkBuffer->SurfaceType & (~BINKSURFACEMASK)) | m_bSurfaceMask);
						BinkBufferUnlock(m_hBinkBuffer);
					}
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
		if ( m_hBinkPlayer->Paused && wParam )
			BinkPause(m_hBinkPlayer, FALSE);
		else
		{
			if ( !m_hBinkPlayer->Paused && !wParam )
				BinkPause(m_hBinkPlayer, TRUE);
		}
	}
}