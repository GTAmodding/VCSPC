#ifndef __COLORMODCTRL
#define __COLORMODCTRL

class CColormodController
{
private:
	static bool			m_bColormodEnabled;

	// Imports from colormod.dll
	static RwReal*		ColorChroma;
	static RwRGBAReal*	ColorGamma;
	static RwRGBAReal*	ColorContrast;
	static RwRGBAReal*	ColorBrightness;

	static bool*		BloomEnable;
	static struct tBloomSpecs
	{
		RwReal		Darkness;
		RwReal		Chroma;
		RwReal		Power;
		RwReal		Size;
	}*					BloomSpecs;

public:
	static inline void	Desaturate()
	{
		if ( m_bColormodEnabled )
		{
			static bool bDesaturated = false;
			if ( !bDesaturated )
			{
				bDesaturated = true;
				*ColorChroma = 0.0f;
			}
			else
			{
				bDesaturated = false;
				*ColorChroma = 1.0f;
			}
		}
	}

	static void			Attach();
	static void			ApplySettings();
};

#endif