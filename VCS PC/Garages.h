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
	static void		PrintMessages();
};

static_assert(sizeof(CGarages) == CGarages_ARRAYSIZE, "CGarages class has wrong size!");

#endif