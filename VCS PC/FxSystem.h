#ifndef __FXSYSTEM
#define __FXSYSTEM

enum FxQuality_e
{
	FXQUALITY_LOW,
	FXQUALITY_MEDIUM,
	FXQUALITY_HIGH,
	FXQUALITY_HIGHEST
};

class Fx_c
{
private:
	// VCS PC class extension
	static unsigned char	m_bTextureFiltering;

public:
	void					SetFxQuality(FxQuality_e eQuality);
	FxQuality_e				GetFxQuality();

	// VCS PC class extension
	static void				SetTextureFilteringQuality(unsigned char nQuality)
		{ m_bTextureFiltering = nQuality; }

	static unsigned char	 GetTextureFilteringQuality()
		{ return m_bTextureFiltering; }

	static unsigned char	GetMaxTextureFilteringQuality();
};

extern Fx_c&	g_fx;

#endif