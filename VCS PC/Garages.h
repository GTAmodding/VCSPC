#ifndef __CGARAGES
#define __CGARAGES

class CGarages
{
private:
	int				MessageNumber2;
	int				MessageNumber1;
	char			MessageIDString[8];
	signed int		MessageEndTime;
	signed int		MessageStartTime;
	unsigned int	NumGarages;

public:
	static void		Init_AfterRestart();
	static void		Update();
	static void		PrintMessages();
};

extern CGarages*			garages;

static_assert(sizeof(CGarages) == 0x1C, "Wrong size: CGarages");

#endif