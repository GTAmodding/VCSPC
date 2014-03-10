#include "StdAfx.h"
#include "CCamera.h"

bool CCamera::bDontTouchFOVInWidescreen;

WRAPPER void CamShakeNoPos(CCamera* pCamera, float fStrength) { WRAPARG(pCamera); WRAPARG(fStrength); EAXJMP(0x50A970); }

WRAPPER int CCamera::GetFadeStage() { EAXJMP(0x50AE20); }
WRAPPER int CCamera::GetLookDirection() { EAXJMP(0x50AE90); }
WRAPPER bool CCamera::IsPositionVisible(const CVector& vecPos, float fRadius) { WRAPARG(vecPos); WRAPARG(fRadius); EAXJMP(0x420D40); }

void CCamera::GetScreenRect(CRect& rect)
{
	float			fScreenRatio = ScreenAspectRatio;
	float			dScreenHeightWeWannaCut = ((-9.0f/16.0f) * fScreenRatio + 1.0f);
	float			dBorderProportionsFix = ((-144643.0f/50000.0f) * fScreenRatio * fScreenRatio) + ((807321.0f/100000.0f) * fScreenRatio) - (551143.0f/100000.0f);

	if ( dBorderProportionsFix < 0.0 )
		dBorderProportionsFix = 0.0;

	if ( dScreenHeightWeWannaCut > 0.0 )
	{
		rect.y1 = (RsGlobal.MaximumHeight / 2) * (dScreenHeightWeWannaCut - dBorderProportionsFix);
		rect.y2 = RsGlobal.MaximumHeight - ((RsGlobal.MaximumHeight / 2) * (dScreenHeightWeWannaCut + dBorderProportionsFix));
	}
	else
	{
		rect.y1 = 0.0;
		rect.y2 = static_cast<float>(RsGlobal.MaximumHeight);
	}
}