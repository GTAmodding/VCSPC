#ifndef __CONTROLLERCONFIGMGR
#define __CONTROLLERCONFIGMGR

#define CONTROLS_FILE_VERSION			1

class CControllerConfigManager
{
public:
	void				SaveSettings(FILE* hFile);
	bool				LoadSettings(FILE* hFile);
	bool				GetIsKeyboardKeyJustDown(RsKeyCodes eKey);
	bool				GetIsKeyboardKeyDown(RsKeyCodes eKey);

	void				SaveToFile();
	void				LoadFromFile();
};


extern CControllerConfigManager&	ControlsManager;

#endif