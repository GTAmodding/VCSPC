#ifndef __GANGS
#define __GANGS

class CGangInfo
{
public:
	signed char					m_nPedModelOverride;
	int							m_nGangWeapons[3];
};

class CGangs
{
private:
	static CGangInfo* const		Gang;

public:
	static void					Initialise();
};

static_assert(sizeof(CGangInfo) == 0x10, "Wrong size: CGangInfo");

#endif