#ifndef __STATICPATCHER
#define __STATICPATCHER

class StaticPatcher
{
private:
	static std::vector<std::function<void()>>*	m_pFunctions;

public:
	StaticPatcher(std::function<void()> pInitializer);

	static void									Apply();
};

#endif