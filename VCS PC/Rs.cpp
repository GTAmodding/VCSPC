#include "stdafx.h"
#include "Rs.h"

#include "CCamera.h"

const DWORD RsGlobalFrameLimits[] = { 0, 25, 30, 50, 60 };

WRAPPER RsEventStatus RsEventHandler(RsEvent eventID, void* param) { WRAPARG(eventID); WRAPARG(param); EAXJMP(0x619B60); }
WRAPPER void DoRWStuffEndOfFrame() { EAXJMP(0x53D840); }
WRAPPER void DefinedState2d() { EAXJMP(0x734750); }


RwChar* RsPathnameCreate(const RwChar* srcBuffer)
{
	size_t		len = strlen(srcBuffer) + 1;
	RwChar*		pMem = static_cast<RwChar*>(malloc(len));

	if ( pMem )
	{
		strncpy(pMem, srcBuffer, len);
		while ( RwChar* pChar = strchr(pMem, '/') )
			*pChar = '\\';
	}
	return pMem;
}

void RsPathnameDestroy(RwChar* buffer)
{
	if ( buffer )
		free(buffer);
}

BOOL RsCameraBeginUpdate(RwCamera* pCamera)
{
	if ( RwCameraBeginUpdate(pCamera) )
		return TRUE;

	RsEventHandler(rsACTIVATE, nullptr);
	return FALSE;
}

void CameraSize(RwCamera* camera, RwRect* rect, float fViewWindow, float fAspectRatio)
{
	float	foo = fViewWindow;
	float	faa = fAspectRatio;

	if ( !camera )
		return;

	RwVideoMode		vidModeInfo;
	RwRect*			pWorkingRect;
	RwRect			newRect;
	RwRaster*		pFrameBuffer = RwCameraGetRaster(camera);

	RwEngineGetVideoModeInfo(&vidModeInfo, RwEngineGetCurrentVideoMode());

	if ( rect )
		pWorkingRect = rect;
	else
	{
		newRect.x = newRect.y = 0;
		pWorkingRect = &newRect;

		if ( vidModeInfo.flags & rwVIDEOMODEEXCLUSIVE )
		{
			newRect.w = vidModeInfo.width;
			newRect.h = vidModeInfo.height;
		}
		else
		{
			newRect.w = RwRasterGetWidth(pFrameBuffer);
			newRect.h = RwRasterGetHeight(pFrameBuffer);
		}

		if ( RwRasterGetWidth(pFrameBuffer) != pWorkingRect->w || RwRasterGetHeight(pFrameBuffer) != pWorkingRect->h )
		{
			RwRaster*		pZBuffer = RwCameraGetZRaster(camera);

			// omg this is nonstandard
			pFrameBuffer->width = pZBuffer->width = pWorkingRect->w;
			pFrameBuffer->height = pZBuffer->height = pWorkingRect->h;
		}
	}

	RwV2d		viewWindow;

	if ( TheCamera.m_WideScreenOn && !TheCamera.bDontTouchFOVInWidescreen )
	{
		if ( fAspectRatio <= 16.0f/9.0f )
		{
			// Preserve constant width
			viewWindow.x = fViewWindow;
			viewWindow.y = fViewWindow / fAspectRatio;
		}
		else
		{
			// Preserve constant height
			viewWindow.x = fViewWindow * fAspectRatio * (9.0f/16.0f); // Like for 16:9
			viewWindow.y = fViewWindow * (9.0f/16.0f);
		}
	}
	else
	{
		// HOR+ scaling
		viewWindow.x = fViewWindow * fAspectRatio;
		viewWindow.y = fViewWindow;
	}

	RwCameraSetViewWindow(camera, &viewWindow);
	RsGlobal.MaximumWidth = pWorkingRect->w;
	RsGlobal.MaximumHeight = pWorkingRect->h;
}