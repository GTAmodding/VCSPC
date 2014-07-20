#include "StdAfx.h"
#include "FxSystem.h"

#include "Rs.h"

Fx_c&	g_fx = *(Fx_c*)0xA9AE00;

unsigned char Fx_c::m_bTextureFiltering;

WRAPPER void Fx_c::SetFxQuality(FxQuality_e eQuality) { WRAPARG(eQuality); EAXJMP(0x49EA40); }
WRAPPER FxQuality_e Fx_c::GetFxQuality() { EAXJMP(0x49EA50); }

unsigned char Fx_c::GetMaxTextureFilteringQuality()
{
	static unsigned char		bCachedMaxQuality = 0;

	if ( bCachedMaxQuality == 0 )
	{
		if ( !bAnisotSupported )
			// Trilinear is max
			bCachedMaxQuality = 1;
		else
		{
			auto	maxAnisot = RpAnisotGetMaxSupportedMaxAnisotropy();
	
			if ( maxAnisot >= 16 )
				// 16x is max
				bCachedMaxQuality = 5;
	
			else if ( maxAnisot >= 8 )
				// 8x is max
				bCachedMaxQuality = 4;

			else if ( maxAnisot >= 4 )
				// 3x is max
				bCachedMaxQuality = 3;

			else if ( maxAnisot >= 2 )
				// 2x is max
				bCachedMaxQuality = 2;
			else
				// No 2x even? Shi
				bCachedMaxQuality = 1;
		}
	}
	return bCachedMaxQuality;
}