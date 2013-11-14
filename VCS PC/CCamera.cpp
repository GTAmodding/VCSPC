#include "StdAfx.h"

bool CCamera::bDontTouchFOVInWidescreen;

void CCamera::GetWidescreenDimensions(CRect& rect)
{
	float			fScreenRatio = *ScreenAspectRatio;
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

void CCamera::CamShake(float strength)
{
	DWORD dwFunc = (DWORD)FUNC_CCamera__CamShake;
	_asm
	{
		mov		eax, strength
		push	eax
		mov		eax, this
		push	eax
		call	dwFunc
		add		esp, 8
	}
}

void CCamera::Find3rdPersonCamTargetVector(float dist, float posX, float posY, float posZ, CVector* unkVec, CVector* output)
{
	DWORD dwFunc = (DWORD)FUNC_CCamera__Find3rdPersonCamTargetVector;
	_asm
	{
		mov		eax, output
		push	eax
		mov		eax, unkVec
		push	eax
		mov		eax, posZ
		push	eax
		mov		eax, posY
		push	eax
		mov		eax, posX
		push	eax
		mov		eax, dist
		push	eax
		mov		ecx, this
		call	dwFunc
	}
}

int CCamera::GetFadeStage()
{
	DWORD dwFunc = (DWORD)FUNC_CCamera__GetFadeStage;
	_asm
	{
		mov		ecx, this
		call	dwFunc
	}
}