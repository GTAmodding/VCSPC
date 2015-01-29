#ifndef __CONTROLLERCONFIGMGR
#define __CONTROLLERCONFIGMGR

#define CONTROLS_FILE_VERSION			1

class CMousePointerStateHelper
{
public:
	bool				m_bVerticalInvert;
	bool				m_bHorizontalInvert;
};

class CControllerConfigManager
{
public:
	void				SaveSettings(FILE* hFile);
	bool				LoadSettings(FILE* hFile);
	bool				GetIsKeyboardKeyJustDown(RsKeyCodes eKey);
	bool				GetIsKeyboardKeyDown(RsKeyCodes eKey);

	void				SaveToFile();
	bool				LoadFromFile();

	static void			ReinitControls();
};

// RawInput mouse handler
void RegisterRawInputMouse();
void RegisterMouseMovement(RAWINPUT* raw);


extern CControllerConfigManager&	ControlsManager;
extern CMousePointerStateHelper&	MousePointerStateHelper;

#endif