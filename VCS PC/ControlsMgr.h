#ifndef __CONTROLLERCONFIGMGR
#define __CONTROLLERCONFIGMGR

#define CONTROLS_FILE_VERSION			1

class CControllerConfigManager
{
public:
	void				SaveSettings(FILE* hFile);
	bool				LoadSettings(FILE* hFile);

	void				SaveToFile();
	void				LoadFromFile();
};


#endif