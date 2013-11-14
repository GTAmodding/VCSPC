#include "StdAfx.h"

void CGarages::PrintMessages()
{
	if ( CTimer::m_snTimeInMilliseconds < garages->MessageStartTime || CTimer::m_snTimeInMilliseconds >= garages->MessageEndTime )
		garages->MessageIDString[0] = '\0';
	else
	{
		CFont::SetTextLetterSize(_width(0.6f), _height(1.1f));
		CFont::SetTextUseProportionalValues(true);
		CFont::SetTextBackground(0, 0);
		CFont::func_7194E0(_width(230.0f));
		CFont::SetTextAlignment(ALIGN_Center);
		CFont::SetFontStyle(FONT_Eurostile);
		CFont::SetTextColour(CRGBA(BaseColors[3]));
		CFont::SetTextOutline(1);
		CFont::SetTextBorderRGBA(CRGBA(0, 0, 0, 255));

		char	tmpString[552];
		if ( garages->MessageNumber2 < 0 )
		{
			if ( garages->MessageNumber1 < 0 )
				CFont::PrintString(RsGlobal.MaximumWidth / 2, _y(275.0), gxt->GetText(garages->MessageIDString));
			else
			{
				gxt->TextNumberFormat(gxt->GetText(garages->MessageIDString), garages->MessageNumber1, -1, -1, -1, -1, -1, tmpString);
				CFont::PrintString(RsGlobal.MaximumWidth / 2, _y(275.0), tmpString);
			}
		}
		else
		{
			gxt->TextNumberFormat(gxt->GetText(garages->MessageIDString), garages->MessageNumber1, garages->MessageNumber2, -1, -1, -1, -1, tmpString);
			CFont::PrintString(RsGlobal.MaximumWidth / 2, _y(275.0), tmpString);
		}
	}
}