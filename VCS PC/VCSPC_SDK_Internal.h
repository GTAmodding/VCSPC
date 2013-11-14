#ifndef __SDKINTERNAL
#define __SDKINTERNAL

#define VCSPCSDKLINK __declspec(dllexport)

// Mod checks
inline bool IVHudPresent()
{
	return GetModuleHandle("gta_iv_hud.asi") != nullptr;
}

#endif