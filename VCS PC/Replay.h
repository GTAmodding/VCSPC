#ifndef __REPLAY
#define __REPLAY

class CReplay
{
public:
	static void	Init(void);
	static void	Update(void);
	static bool	ShouldStandardCameraBeProcessed(void);
	static unsigned char&		Mode;
};

#endif