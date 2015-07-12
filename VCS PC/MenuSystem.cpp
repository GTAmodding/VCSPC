#include "StdAfx.h"
#include "MenuSystem.h"

#include "Frontend.h"
#include "Font.h"
#include "Text.h"
#include "Messages.h"
#include "Pad.h"

CMenuSystem::Menu** const		CMenuSystem::MenuInUse = (CMenuSystem::Menu**)0xBA82D8;

void CMenuSystem::DisplayStandardMenu(unsigned char nMenuID, bool bActiveColour)
{
	Menu*			pCurrentMenu = MenuInUse[nMenuID];
	unsigned char	nColourDesaturation = bActiveColour ? 0 : 120;

	if ( pCurrentMenu->m_bBackground )
	{
		// Draw background
		float		fHorMargin = 8.0f;
		float		fTotalWidth = 0.0;
		for ( int i = 0; i < pCurrentMenu->m_nNumColumns; i++ )
		{
			if ( pCurrentMenu->m_aColumnTitle[i][0] )
				fHorMargin = 70.0f;
			fTotalWidth += pCurrentMenu->m_fWidth[i];
		}
		FrontEndMenuManager.DrawWindow(
			CRect(_xleft(pCurrentMenu->m_fPosX), _y(pCurrentMenu->m_nNumItems * 16.0f + fHorMargin + pCurrentMenu->m_fPosY),
			_xleft(8.0f + fTotalWidth + pCurrentMenu->m_fPosX), _y(pCurrentMenu->m_fPosY)), 
			pCurrentMenu->m_aTitle, nColourDesaturation, CRGBA(0, 0, 0, 200), false, true);
	}

	CFont::SetFontStyle(FONT_Eurostile);
	CFont::SetScale(_width(0.52f), _height(1.0f));
	CFont::SetDropShadowPosition(1);
	CFont::SetDropColor(CRGBA(0, 0, 0, 200));
	CFont::SetProportional(true);

	if ( pCurrentMenu->m_nNumItems )
	{
		float	fXPosForColumn = pCurrentMenu->m_fPosX + 4.0f;

		for ( int i = 0; i < pCurrentMenu->m_nNumColumns; i++ )
		{
			// TODO: Column title
			
			float	fCurrentPos = pCurrentMenu->m_fPosY + 4.0f;

			CFont::SetOrientation(pCurrentMenu->m_nOrientation[i]);
			if ( pCurrentMenu->m_nOrientation[i] == ALIGN_Right )
				fXPosForColumn += pCurrentMenu->m_fWidth[i];
			else if ( pCurrentMenu->m_nOrientation[i] == ALIGN_Center )
				fXPosForColumn += pCurrentMenu->m_fWidth[i] * 0.5f;

			for ( int j = 0; j < pCurrentMenu->m_nNumItems; j++ )
			{
				if ( pCurrentMenu->m_aFields[i][j][0] )
				{
					// Colour
					// TODO: Non-interactive menus?
					// TODO: Active entries
					if ( pCurrentMenu->m_nSelectedMenuItem == j )
						// VCS menu yellow
						CFont::SetColor(CRGBA(MENU_ACTIVE_R, MENU_ACTIVE_G, MENU_ACTIVE_B, 225));
					else
						// Grey/ menu blue for next columns
						CFont::SetColor(i == 0 ? CRGBA(255, 255, 255, 200) : CRGBA(MENU_INACTIVE_R, MENU_INACTIVE_G, MENU_INACTIVE_B, 200));

					// Formatting
					CMessages::InsertNumberInString(TheText.Get(pCurrentMenu->m_aFields[i][j]), pCurrentMenu->m_nNumber[0][i][j], pCurrentMenu->m_nNumber[1][i][j], -1, -1, -1, -1, gString);
					CMessages::InsertPlayerControlKeysInString(gString);

					CFont::PrintString(_xleft(fXPosForColumn), _y(fCurrentPos), gString);
					fCurrentPos += 16.0f;
				}
			}

			fXPosForColumn += pCurrentMenu->m_fWidth[i];
		}
	}
}



static StaticPatcher	Patcher([](){ 
						using namespace Memory;

						InjectHook(0x580E00, CMenuSystem::DisplayStandardMenu, PATCH_JUMP);

						// Don't scale fields at the time of creating the menu
						Nop(0x475546, 6);
						Patch<DWORD>(0x475558, 0x9090E8D9);
						Nop(0x47555C, 2);

						Nop(0x475587, 6);
						Nop(0x475596, 6);
						Patch<WORD>(0x47559C, 0x05D9);

						Patch<WORD>(0x580872, 0xC88B);
						InjectHook(0x580874, &CPad::FrontEndBackJustDown, PATCH_CALL);
						Patch<DWORD>(0x580879, 0x9090C084);
						Patch<WORD>(0x58087D, 0x7490);
									});