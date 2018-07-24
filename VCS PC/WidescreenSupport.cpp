#include "StdAfx.h"
#include "WidescreenSupport.h"

#include "Frontend.h"
#include "Rs.h"
#include "Camera.h"

int& WidescreenSupport::nCTRubberSlider = CMenuManager::ms_nRubberSlider = 640/2;
int& WidescreenSupport::nCTRubberSliderMinPos = *(int*)0x57BCBD;
int& WidescreenSupport::nCTRubberSliderMaxPos = *(int*)0x57BCAB;
float*& WidescreenSupport::fHorizontalAspectRatio = WidthAspectRatio;
float*& WidescreenSupport::fVerticalAspectRatio = HeightAspectRatio;
float WidescreenSupport::fScreenWidthMultiplier = 640.0f;
float WidescreenSupport::fScreenWidthDivider = 1.0f/640.0f;
float WidescreenSupport::fScreenHeightMultiplier = 448.0f;
float WidescreenSupport::f4 = 4.0f/640.0f;
float WidescreenSupport::f45 = 45.0f/640.0f;
float WidescreenSupport::f50 = 50.0f/640.0f;
float WidescreenSupport::f55 = 55.0f/640.0f;
float WidescreenSupport::f60 = 60.0f/640.0f;
float WidescreenSupport::f70 = 70.0f/640.0f;
float WidescreenSupport::f95 = 95.0f/640.0f;
float WidescreenSupport::f160 = 160.0f/640.0f;
float WidescreenSupport::f350 = 350.0f/640.0f;
float WidescreenSupport::f555 = 555.0f/640.0f;
float WidescreenSupport::f580 = 580.0f/640.0f;
float WidescreenSupport::f0pt3 = 0.3f/640.0f;
float WidescreenSupport::f0pt49 = 0.49f/640.0f;
float WidescreenSupport::f0pt8 = 0.8f/640.0f;
float WidescreenSupport::f0pt56 = 0.56f/640.0f;
float WidescreenSupport::fMenuMessageWidth = 640.0f - 210.0f;
float WidescreenSupport::fCTSliderRight = 0.5f + (50.0f/640.0f);
float WidescreenSupport::fCTSliderLeft = 0.5f - (50.0/640.0f);
float WidescreenSupport::fScreenCoorsFix = 44800.0f/640.0f;
//float WidescreenSupport::fSpawningFix = 44800.0f/640.0f;
float WidescreenSupport::fMapZonePosX2 = 7.5f/640.0f;
//float WidescreenSupport::fSkyMultFix = 1.4f;

float WidescreenSupport::f0pt7_h = 0.7f/448.0f;
//float WidescreenSupport::f0pt95_h = 0.95f/448.0f;
//float WidescreenSupport::f1pt2_h = 1.2f/448.0f;
//float WidescreenSupport::f2pt1_h = 2.1f/448.0f;
//float WidescreenSupport::f1_h = 1.0f/448.0f;
//float WidescreenSupport::f28_h = 28.0f/448.0f;
//float WidescreenSupport::f97_centh = 0.5f - 127.0f/448.0f;

float WidescreenSupport::fProperWidthMultiplier = 1.0f;
float WidescreenSupport::fProperHeightMultiplier = 480.0f/448.0f;

//const float WidescreenSupport::fFOVMultiplier = 1.0f/((4.0f/3.0f)/1.1f);
//const float WidescreenSupport::fFOVMultiplier = 1.0f/((4.0f/3.0f)/1.0f);

float GetCoord(float a) {
	float base = RsGlobal.MaximumWidth > RsGlobal.MaximumHeight ? RsGlobal.MaximumHeight : RsGlobal.MaximumWidth;
	return static_cast<int>(a * base / 1080.0f);
}

void WidescreenSupport::Recalculate(int nWidth, int nHeight, bool bAlways)
{
	static int	savedResWidth = 0, savedResHeight = 0;

	if ( bAlways || nWidth != savedResWidth || nHeight != savedResHeight )
	{
		static const float	fPrecalculatedMultipliers[] = { 4.0f/3.0f*480.0f, 640.0f/*5.0/4.0*480.0*/, 15.0f/9.0f*480.0f, 16.0f/9.0f*480.0f, 16.0f/10.0f*480.0f };
		static const float	fPrecalculatedHeightMults[] = { 448.0f, (4.0f/5.0f)*640.0f/(480.0f/448.0f), 448.0f, 448.0f, 448.0f };
		float				fNewResolutionWidthMultiplier;
		float				fNewResolutionHeightMultiplier;

		savedResWidth = nWidth;
		savedResHeight = nHeight;

		if ( !FrontEndMenuManager.m_bAspectRatioMode )
		{
			if ( nWidth == 720 && nHeight == 576 )
			{
				fNewResolutionWidthMultiplier = fPrecalculatedMultipliers[0];
				fNewResolutionHeightMultiplier = fPrecalculatedHeightMults[0];
			}
			else
			{
				fNewResolutionWidthMultiplier = static_cast<float>(nWidth) / nHeight * 480.0f;
				if ( fNewResolutionWidthMultiplier < 640.0f )
				{
					fNewResolutionWidthMultiplier = 640.0f;
					fNewResolutionHeightMultiplier = static_cast<float>(nHeight) / nWidth * (640.0f/(480.0f/448.0f));
				}
				else
					fNewResolutionHeightMultiplier = 448.0f;
				/*if ( fNewResolutionWidthMultiplier < 622.5 )
					fNewResolutionWidthMultiplier = 622.5;*/
			}
		}
		else
		{
			fNewResolutionWidthMultiplier = fPrecalculatedMultipliers[FrontEndMenuManager.m_bAspectRatioMode - 1];
			fNewResolutionHeightMultiplier = fPrecalculatedHeightMults[FrontEndMenuManager.m_bAspectRatioMode - 1];
		}

		nCTRubberSlider = static_cast<int>(fNewResolutionWidthMultiplier/2.0f);
		nCTRubberSliderMinPos = static_cast<int>(fNewResolutionWidthMultiplier/2.0f) - 50;
		nCTRubberSliderMaxPos = static_cast<int>(fNewResolutionWidthMultiplier/2.0f) + 50;
		*fHorizontalAspectRatio = fScreenWidthDivider = 1.0f/fNewResolutionWidthMultiplier;
		*fVerticalAspectRatio = /*fScreenHeightDivider =*/ 1.0f/fNewResolutionHeightMultiplier;
		fScreenWidthMultiplier = fNewResolutionWidthMultiplier;
		fScreenHeightMultiplier = fNewResolutionHeightMultiplier;
		f4 = 4.0f/fNewResolutionWidthMultiplier;
		f45 = 45.0f/fNewResolutionWidthMultiplier;
		f50 = 50.0f/fNewResolutionWidthMultiplier;
		f55 = 55.0f/fNewResolutionWidthMultiplier;
		f60 = 60.0f/fNewResolutionWidthMultiplier;
		f70 = 70.0f/fNewResolutionWidthMultiplier;
		f95 = 95.0f/fNewResolutionWidthMultiplier;
		f160 = 160.0f/fNewResolutionWidthMultiplier;
		f350 = 350.0f/fNewResolutionWidthMultiplier;
		f555 = 555.0f/fNewResolutionWidthMultiplier;
		f580 = 580.0f/fNewResolutionWidthMultiplier;
		f0pt3 = 0.3f/fNewResolutionWidthMultiplier;
		f0pt49 = 0.49f/fNewResolutionWidthMultiplier;
		f0pt8 = 0.8f/fNewResolutionWidthMultiplier;
		f0pt56 = 0.56f/fNewResolutionWidthMultiplier;
		fMenuMessageWidth = fNewResolutionWidthMultiplier - 210.0f;
		fCTSliderRight = 0.5f + (50.0f/fNewResolutionWidthMultiplier);
		fCTSliderLeft = 0.5f - (50.0f/fNewResolutionWidthMultiplier);
		fScreenCoorsFix = 44800.0f/fNewResolutionWidthMultiplier;
		//fAimpointFix = (9.0f/12800.0f)*fNewResolutionWidthMultiplier + 0.1f;
		fMapZonePosX2 = 7.5f/fNewResolutionWidthMultiplier;

		f0pt7_h = 0.7f/fNewResolutionHeightMultiplier;
		//f0pt95_h = 0.95f/fNewResolutionHeightMultiplier;
		//f1pt2_h = 1.2f/fNewResolutionHeightMultiplier;
		//f2pt1_h = 2.1f/fNewResolutionHeightMultiplier;
		//f1_h = 1.0f/fNewResolutionHeightMultiplier;
		//f28_h = 28.0f/fNewResolutionHeightMultiplier;
		//f97_centh = 0.5f - 127.0f/fNewResolutionHeightMultiplier;

		fProperWidthMultiplier = static_cast<float>(nWidth)/fNewResolutionWidthMultiplier;
		fProperHeightMultiplier = static_cast<float>(nHeight)/fNewResolutionHeightMultiplier;
	}
}

float WidescreenSupport::SetAspectRatio()
{
	switch ( FrontEndMenuManager.m_bAspectRatioMode )
	{
	case 1:
		return ScreenAspectRatio = 4.0f/3.0f;
	case 2:
		return ScreenAspectRatio = 5.0f/4.0f;
	case 3:
		return ScreenAspectRatio = 15.0f/9.0f;
	case 4:
		return ScreenAspectRatio = 16.0f/9.0f;
	case 5:
		return ScreenAspectRatio = 16.0f/10.0f;
	default:
		return ScreenAspectRatio = static_cast<float>(RsGlobal.MaximumWidth) / RsGlobal.MaximumHeight;
	}
}

float WidescreenSupport::GetTextPosition()
{
	float			fScreenRatio = ScreenAspectRatio;
	double			dTextPos = (-3375.0 * fScreenRatio * fScreenRatio) + (10800.0 * fScreenRatio) - 8545.0;
	return dTextPos > 80.0 ? static_cast<float>(dTextPos) : 80.0f;
}

/*float WidescreenSupport::GetSkyWidth()
{
	return fSkyMultFix = (10.0/183.0) * (*ScreenAspectRatio * *fFOV) - 3.7;
}*/

unsigned char WidescreenSupport::GetTextBoxPos()
{
	if ( TheCamera.bDontTouchFOVInWidescreen )
		return 0;
	/*float			fScreenRatio = *ScreenAspectRatio;
	long double		dBorderProportionsFix = ((-216964.5/50000.0) * fScreenRatio * fScreenRatio) + ((1210981.5/100000.0) * fScreenRatio) - (826714.5/100000.0);
	long double		dTextBoxPos = (-144.0 * fScreenRatio) + 256.0;

	if ( dBorderProportionsFix > 0.0 )
		dTextBoxPos -= dTextBoxPos * dBorderProportionsFix;
	return dTextBoxPos > 0.0 ? dTextBoxPos : 0;*/

	float			fScreenRatio = ScreenAspectRatio;
	float			dScreenHeightWeWannaCut = ((-9.0f/16.0f) * fScreenRatio + 1.0f);
	float			dBorderProportionsFix = ((-144643.0f/50000.0f) * fScreenRatio * fScreenRatio) + ((807321.0f/100000.0f) * fScreenRatio) - (551143.0f/100000.0f);

	if ( dBorderProportionsFix < 0.0 )
		dBorderProportionsFix = 0.0;

	return dScreenHeightWeWannaCut > 0.0 ? static_cast<unsigned char>((RsGlobal.MaximumHeight / 2) * (dScreenHeightWeWannaCut - dBorderProportionsFix)) : 0;
}

CVector2D WidescreenSupport::GetFullscreenImageDimensions(float fImageAspectRatio, float fScreenAspectRatio, bool bFitToScreen)
{
	if ( bFitToScreen )
	{
		if ( fScreenAspectRatio < fImageAspectRatio )
			return CVector2D(static_cast<float>(RsGlobal.MaximumWidth), RsGlobal.MaximumHeight * fScreenAspectRatio / fImageAspectRatio);
		return CVector2D(RsGlobal.MaximumWidth * fImageAspectRatio / fScreenAspectRatio, static_cast<float>(RsGlobal.MaximumHeight));
	}

	if ( fScreenAspectRatio > fImageAspectRatio )
		return CVector2D(static_cast<float>(RsGlobal.MaximumWidth), RsGlobal.MaximumHeight * fScreenAspectRatio / fImageAspectRatio);
	return CVector2D(RsGlobal.MaximumWidth * fImageAspectRatio / fScreenAspectRatio, static_cast<float>(RsGlobal.MaximumHeight));
}