#include "StdAfx.h"
#include "Garages.h"

#include "Timer.h"
#include "Font.h"
#include "Messages.h"
#include "Text.h"

WRAPPER void CGarages::Init_AfterRestart() { EAXJMP(0x448B60); }

void CGarages::PrintMessages()
{
	if ( CTimer::m_snTimeInMilliseconds < garages->MessageStartTime || CTimer::m_snTimeInMilliseconds >= garages->MessageEndTime )
		garages->MessageIDString[0] = '\0';
	else
	{
		CFont::SetScale(_width(0.6f), _height(1.1f));
		CFont::SetProportional(true);
		CFont::SetBackground(0, 0);
		CFont::SetCentreSize(_width(RsGlobal.MaximumWidth - 140.0f/*500.0f*/));
		CFont::SetOrientation(ALIGN_Center);
		CFont::SetFontStyle(FONT_Eurostile);
		CFont::SetColor(CRGBA(BaseColors[3]));
		CFont::SetEdge(1);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));

		if ( garages->MessageNumber2 < 0 )
		{
			if ( garages->MessageNumber1 < 0 )
				CFont::PrintString(static_cast<float>(RsGlobal.MaximumWidth / 2), _ymiddle(50.0f), TheText.Get(garages->MessageIDString));
			else
			{
				char	tmpString[256];

				CMessages::InsertNumberInString(TheText.Get(garages->MessageIDString), garages->MessageNumber1, -1, -1, -1, -1, -1, tmpString);
				CFont::PrintString(static_cast<float>(RsGlobal.MaximumWidth / 2), _ymiddle(50.0f), tmpString);
			}
		}
		else
		{
			char	tmpString[256];

			CMessages::InsertNumberInString(TheText.Get(garages->MessageIDString), garages->MessageNumber1, garages->MessageNumber2, -1, -1, -1, -1, tmpString);
			CFont::PrintString(static_cast<float>(RsGlobal.MaximumWidth / 2), _ymiddle(50.0f), tmpString);
		}
	}
}