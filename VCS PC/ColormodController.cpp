#include "StdAfx.h"
#include "ColormodController.h"

bool			CColormodController::m_bColormodEnabled;

RwReal*			CColormodController::ColorChroma;
RwRGBAReal*		CColormodController::ColorGamma;
RwRGBAReal*		CColormodController::ColorContrast;
RwRGBAReal*		CColormodController::ColorBrightness;
bool*			CColormodController::BloomEnable;
CColormodController::tBloomSpecs* CColormodController::BloomSpecs;

void CColormodController::Attach()
{
	HMODULE		hColormodModule = LoadLibrary("colormod");

	LogToFile("Color Mod loaded: %s", hColormodModule != nullptr ? "YES" : "NO");

	if ( hColormodModule != nullptr )
	{
		m_bColormodEnabled = true;

		// Get addresses from colormod.dll
		ColorChroma = reinterpret_cast<RwReal*>(GetProcAddress(hColormodModule, "ColorChroma"));
		ColorGamma = reinterpret_cast<RwRGBAReal*>(GetProcAddress(hColormodModule, "ColorGamma"));
		ColorContrast = reinterpret_cast<RwRGBAReal*>(GetProcAddress(hColormodModule, "ColorContrast"));
		ColorBrightness = reinterpret_cast<RwRGBAReal*>(GetProcAddress(hColormodModule, "ColorBrightness"));
		BloomEnable = reinterpret_cast<bool*>(GetProcAddress(hColormodModule, "BloomEnable"));
		BloomSpecs = reinterpret_cast<tBloomSpecs*>(GetProcAddress(hColormodModule, "BloomSpecs"));

		// Apply stock settings
		ApplySettings();
	}
	else
		m_bColormodEnabled = false;
}

void CColormodController::ApplySettings()
{
	// Stock settings
	// TODO: If more is needed, do it
	// TODO2: In future, make it controllable like colorcycle?
	*ColorChroma = 1.0f;
	ColorGamma->red = 1.0f;
	ColorGamma->green = 1.0f;
	ColorGamma->blue = 1.0f;
	ColorContrast->red = 1.0f;
	ColorContrast->green = 1.0f;
	ColorContrast->blue = 1.0f;
	ColorBrightness->red = 0.0f;
	ColorBrightness->green = 0.0f;
	ColorBrightness->blue = 0.0f;

	*BloomEnable = true;
	BloomSpecs->Darkness = -0.5f;
	BloomSpecs->Chroma = 0.85f;
	BloomSpecs->Power = 0.85f;
	BloomSpecs->Size = 5.0f;
}