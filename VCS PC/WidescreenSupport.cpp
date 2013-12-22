#include "StdAfx.h"

long& WidescreenSupport::nCTRubberSlider = CMenuManager::ms_nRubberSlider = 640/2;
long*& WidescreenSupport::nCTRubberSliderMinPos = CTRubberMinPos;
long*& WidescreenSupport::nCTRubberSliderMaxPos = CTRubberMaxPos;
float*& WidescreenSupport::fHorizontalAspectRatio = WidthAspectRatio;
float*& WidescreenSupport::fVerticalAspectRatio = HeightAspectRatio;
float WidescreenSupport::fScreenWidthMultiplier = 640.0;
float WidescreenSupport::fScreenWidthDivider = 1.0/640.0;
float WidescreenSupport::fScreenHeightMultiplier = 448.0;
float WidescreenSupport::f4 = 4.0/640.0;
float WidescreenSupport::f40 = 40.0/640.0;
float WidescreenSupport::f45 = 45.0/640.0;
float WidescreenSupport::f50 = 50.0/640.0;
float WidescreenSupport::f55 = 55.0/640.0;
float WidescreenSupport::f60 = 60.0/640.0;
float WidescreenSupport::f70 = 70.0/640.0;
float WidescreenSupport::f95 = 95.0/640.0;
float WidescreenSupport::f100 = 100.0/640.0;
float WidescreenSupport::f160 = 160.0/640.0;
float WidescreenSupport::f350 = 350.0/640.0;
float WidescreenSupport::f555 = 555.0/640.0;
float WidescreenSupport::f580 = 580.0/640.0;
float WidescreenSupport::f0pt3 = 0.3/640.0;
float WidescreenSupport::f1pt3 = 1.3/640.0;
float WidescreenSupport::f0pt49 = 0.49/640.0;
float WidescreenSupport::f0pt42 = 0.42/640.0;
float WidescreenSupport::f0pt35 = 0.35/640.0;
float WidescreenSupport::f0pt7 = 0.7/640.0;
float WidescreenSupport::f0pt8 = 0.8/640.0;
float WidescreenSupport::f0pt56 = 0.56/640.0;
float WidescreenSupport::fMenuSliderPosX = 0.5 + (MENU_TEXT_POSITION_RCOLUMN/640.0);
float WidescreenSupport::fMenuSliderWidth = MENU_SLIDER_WIDTH/640.0;
float WidescreenSupport::fMenuMessageWidth = 640.0 - 210.0;
float WidescreenSupport::fCTSliderRight = 0.5 + (50.0/640.0);
float WidescreenSupport::fCTSliderLeft = 0.5 - (50.0/640.0);
float WidescreenSupport::fScreenCoorsFix = 44800.0/640.0;
//float WidescreenSupport::fSpawningFix = 44800.0/640.0;
float WidescreenSupport::fAimpointFix = 0.5;
float WidescreenSupport::fMapZonePosX2 = 7.5/640.0;
//float WidescreenSupport::fSkyMultFix = 1.4;

float WidescreenSupport::f0pt7_h = 0.7/448.0;
float WidescreenSupport::f0pt95_h = 0.95/448.0;
float WidescreenSupport::f1pt2_h = 1.2/448.0;
float WidescreenSupport::f2pt1_h = 2.1/448.0;
float WidescreenSupport::f1_h = 1.0/448.0;
float WidescreenSupport::f28_h = 28.0/448.0;
float WidescreenSupport::f97_centh = 0.5 - 127.0/448.0;
float WidescreenSupport::fMenuSliderPosY2 = MENU_SLIDER_POSY / 448.0;
float WidescreenSupport::fMenuSliderPosY3 = (MENU_SLIDER_POSY - 30.0) / 448.0;
float WidescreenSupport::fMenuSliderPosY4 = (MENU_SLIDER_POSY - MENU_SLIDER_WIDTH / 2.0) / 448.0;
float WidescreenSupport::fMenuSliderHeight2 = MENU_SLIDER_HEIGHT / 448.0;

float WidescreenSupport::fProperWidthMultiplier = 1.0;
float WidescreenSupport::fProperHeightMultiplier = 480.0/448.0;

//float WidescreenSupport::fTextDrawsWidthMultiplier;

//const float WidescreenSupport::fFOVMultiplier = 1.0/((4.0/3.0)/1.2);
const float WidescreenSupport::fFOVMultiplier = 1.0/((4.0/3.0)/1.1);

void WidescreenSupport::Recalculate(long nWidth, long nHeight, bool bAlways)
{
	static long	savedResWidth = 0, savedResHeight = 0;

	if ( bAlways || nWidth != savedResWidth || nHeight != savedResHeight )
	{
		static const double	fPrecalculatedMultipliers[] = { 4.0/3.0*480.0, 640.0/*5.0/4.0*480.0*/, 15.0/9.0*480.0, 16.0/9.0*480.0, 16.0/10.0*480.0 };
		static const double	fPrecalculatedHeightMults[] = { 448.0, (4.0/5.0)*640.0/(480.0/448.0), 448.0, 448.0, 448.0 };
		double				fNewResolutionWidthMultiplier;
		double				fNewResolutionHeightMultiplier;

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
				fNewResolutionWidthMultiplier = static_cast<float>(nWidth) / nHeight * 480.0;
				if ( fNewResolutionWidthMultiplier < 640.0 )
				{
					fNewResolutionWidthMultiplier = 640.0;
					fNewResolutionHeightMultiplier = static_cast<float>(nHeight) / nWidth * (640.0/(480.0/448.0));
				}
				else
					fNewResolutionHeightMultiplier = 448.0;
				/*if ( fNewResolutionWidthMultiplier < 622.5 )
					fNewResolutionWidthMultiplier = 622.5;*/
			}
		}
		else
		{
			fNewResolutionWidthMultiplier = fPrecalculatedMultipliers[FrontEndMenuManager.m_bAspectRatioMode - 1];
			fNewResolutionHeightMultiplier = fPrecalculatedHeightMults[FrontEndMenuManager.m_bAspectRatioMode - 1];
		}

		nCTRubberSlider = fNewResolutionWidthMultiplier/2.0;
		*nCTRubberSliderMinPos = fNewResolutionWidthMultiplier/2.0 - 50;
		*nCTRubberSliderMaxPos = fNewResolutionWidthMultiplier/2.0 + 50;
		*fHorizontalAspectRatio = fScreenWidthDivider = 1.0/fNewResolutionWidthMultiplier;
		*fVerticalAspectRatio = /*fScreenHeightDivider =*/ 1.0/fNewResolutionHeightMultiplier;
		fScreenWidthMultiplier = fNewResolutionWidthMultiplier;
		fScreenHeightMultiplier = fNewResolutionHeightMultiplier;
		f4 = 4.0/fNewResolutionWidthMultiplier;
		f40 = 40.0/fNewResolutionWidthMultiplier;
		f45 = 45.0/fNewResolutionWidthMultiplier;
		f50 = 50.0/fNewResolutionWidthMultiplier;
		f55 = 55.0/fNewResolutionWidthMultiplier;
		f60 = 60.0/fNewResolutionWidthMultiplier;
		f70 = 70.0/fNewResolutionWidthMultiplier;
		f95 = 95.0/fNewResolutionWidthMultiplier;
		f100 = 100.0/fNewResolutionWidthMultiplier;
		f160 = 160.0/fNewResolutionWidthMultiplier;
		f350 = 350.0/fNewResolutionWidthMultiplier;
		f555 = 555.0/fNewResolutionWidthMultiplier;
		f580 = 580.0/fNewResolutionWidthMultiplier;
		f0pt3 = 0.3/fNewResolutionWidthMultiplier;
		f1pt3 = 1.3/fNewResolutionWidthMultiplier;
		f0pt49 = 0.49/fNewResolutionWidthMultiplier;
		f0pt42 = 0.42/fNewResolutionWidthMultiplier;
		f0pt35 = 0.35/fNewResolutionWidthMultiplier;
		f0pt7 = 0.7/fNewResolutionWidthMultiplier;
		f0pt8 = 0.8/fNewResolutionWidthMultiplier;
		f0pt56 = 0.56/fNewResolutionWidthMultiplier;
		fMenuSliderPosX = 0.5 + (MENU_TEXT_POSITION_RCOLUMN/fNewResolutionWidthMultiplier);
		fMenuSliderWidth = MENU_SLIDER_WIDTH/fNewResolutionWidthMultiplier;
		fMenuMessageWidth = fNewResolutionWidthMultiplier - 210.0;
		fCTSliderRight = 0.5 + (50.0/fNewResolutionWidthMultiplier);
		fCTSliderLeft = 0.5 - (50.0/fNewResolutionWidthMultiplier);
		fScreenCoorsFix = 44800.0/fNewResolutionWidthMultiplier;
		fAimpointFix = (9.0/12800.0)*fNewResolutionWidthMultiplier + 0.1;
		fMapZonePosX2 = 7.5/fNewResolutionWidthMultiplier;

		f0pt7_h = 0.7/fNewResolutionHeightMultiplier;
		f0pt95_h = 0.95/fNewResolutionHeightMultiplier;
		f1pt2_h = 1.2/fNewResolutionHeightMultiplier;
		f2pt1_h = 2.1/fNewResolutionHeightMultiplier;
		f1_h = 1.0/fNewResolutionHeightMultiplier;
		f28_h = 28.0/fNewResolutionHeightMultiplier;
		f97_centh = 0.5 - 127.0/fNewResolutionHeightMultiplier;
		fMenuSliderPosY2 = 0.5 + (MENU_SLIDER_POSY - 224.0) / fNewResolutionHeightMultiplier;
		fMenuSliderPosY3 = 0.5 + (MENU_SLIDER_POSY - 254.0) / fNewResolutionHeightMultiplier;
		fMenuSliderPosY4 = 0.5 + (MENU_SLIDER_POSY - 224.0 - MENU_SLIDER_WIDTH / 2.0) / fNewResolutionHeightMultiplier;
		fMenuSliderHeight2 = MENU_SLIDER_HEIGHT / fNewResolutionHeightMultiplier;

		fProperWidthMultiplier = static_cast<float>(nWidth)/fNewResolutionWidthMultiplier;
		fProperHeightMultiplier = static_cast<float>(nHeight)/fNewResolutionHeightMultiplier;
	}
}

float WidescreenSupport::SetAspectRatio()
{
	switch ( FrontEndMenuManager.m_bAspectRatioMode )
	{
	case 1:
		return *ScreenAspectRatio = 4.0/3.0;
	case 2:
		return *ScreenAspectRatio = 5.0/4.0;
	case 3:
		return *ScreenAspectRatio = 15.0/9.0;
	case 4:
		return *ScreenAspectRatio = 16.0/9.0;
	case 5:
		return *ScreenAspectRatio = 16.0/10.0;
	default:
		return *ScreenAspectRatio = static_cast<float>(RsGlobal.MaximumWidth) / RsGlobal.MaximumHeight;
	}
}

float WidescreenSupport::GetTextPosition()
{
	float			fScreenRatio = *ScreenAspectRatio;
	long double		dTextPos = (-3375.0 * fScreenRatio * fScreenRatio) + (10800.0 * fScreenRatio) - 8545.0;
	return dTextPos > 80.0 ? dTextPos : 80.0;
}

/*float WidescreenSupport::GetSkyWidth()
{
	return fSkyMultFix = (10.0/183.0) * (*ScreenAspectRatio * *fFOV) - 3.7;
}*/

unsigned char WidescreenSupport::GetTextBoxPos()
{
	float			fScreenRatio = *ScreenAspectRatio;
	long double		dBorderProportionsFix = ((-216964.5/50000.0) * fScreenRatio * fScreenRatio) + ((1210981.5/100000.0) * fScreenRatio) - (826714.5/100000.0);
//	long double		dBorderProportionsFix = ((-144643.0/50000.0) * fScreenRatio * fScreenRatio) + ((807321.0/100000.0) * fScreenRatio) - (551143.0/100000.0);
	long double		dTextBoxPos = (-144.0 * fScreenRatio) + 256.0;

	if ( dBorderProportionsFix > 0.0 )
		dTextBoxPos -= dTextBoxPos * dBorderProportionsFix;
	return dTextBoxPos > 0.0 ? dTextBoxPos : 0;
}