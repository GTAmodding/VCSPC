#ifndef __WIDESCREENSUPPORT
#define __WIDESCREENSUPPORT

#include "General.h"

class WidescreenSupport
{
public:
	static int&		nCTRubberSlider;
	static int&		nCTRubberSliderMinPos;
	static int&		nCTRubberSliderMaxPos;
	static float*&	fHorizontalAspectRatio;
	static float*&	fVerticalAspectRatio;
	static float	fScreenWidthMultiplier;
	static float	fScreenWidthDivider;
	static float	fScreenHeightMultiplier;
	static float	f4;
	static float	f45;
	static float	f50;
	static float	f55;
	static float	f60;
	static float	f70;
	static float	f95;
	static float	f160;
	static float	f350;
	static float	f555;
	static float	f580;
	static float	f0pt3;
	static float	f0pt49;
	static float	f0pt8;
	static float	f0pt56;
	static float	fMenuMessageWidth;
	static float	fCTSliderRight;
	static float	fCTSliderLeft;
	static float	fScreenCoorsFix;
//	static float	fSpawningFix;
	static float	fMapZonePosX2;
//	static float	fSkyMultFix;

	static float	f0pt7_h;
	//static float	f0pt95_h;
	//static float	f1pt2_h;
	//static float	f2pt1_h;
	//static float	f1_h;
	//static float	f28_h;
	//static float	f97_centh;

	static float	fProperWidthMultiplier;
	static float	fProperHeightMultiplier;

//	static float	fTextDrawsWidthMultiplier;

	//static const float	fFOVMultiplier;

	static void				Recalculate(int nWidth, int nHeight, bool bAlways);
	static float			SetAspectRatio();
	static float			GetTextPosition();
//	static float			GetSkyWidth();
	static unsigned char	GetTextBoxPos();
//	static void		SetFieldOfView(float FOV);

	static inline float GetScreenWidthMultiplier()
		{ return fScreenWidthMultiplier; }

	static inline float GetScreenWidthDivider()
		{ return fScreenWidthDivider; }

	static inline float GetScreenHeightMultiplier()
		{ return fScreenHeightMultiplier; }

	static CVector2D		GetFullscreenImageDimensions(float fImageAspectRatio, float fScreenAspectRatio, bool bFitToScreen);
};

#endif