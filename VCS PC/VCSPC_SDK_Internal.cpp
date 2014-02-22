#include "StdAfx.h"
#include "VCSPC_SDK_Internal.h"

#include "WidescreenSupport.h"

void VCSAPI GetGameMetrics(float* pOutWidth, float* pOutHeight)
{
	if ( pOutWidth )
		*pOutWidth = WidescreenSupport::GetScreenWidthMultiplier();
	if ( pOutHeight )
		*pOutHeight = WidescreenSupport::GetScreenHeightMultiplier();
}