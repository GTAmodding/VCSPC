#include "StdAfx.h"

void VCSAPI GetGameMetrics(float* pOutWidth, float* pOutHeight)
{
	if ( pOutWidth )
		*pOutWidth = WidescreenSupport::GetScreenWidthMultiplier();
	if ( pOutHeight )
		*pOutHeight = WidescreenSupport::GetScreenHeightMultiplier();
}