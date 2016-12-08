#ifndef __STATICPATCHER
#define __STATICPATCHER

class StaticPatcher
{
private:
	using Patcher = void(*)();

	Patcher			m_func;
	StaticPatcher*	m_next;

	static StaticPatcher* ms_head;

private:
	void Run() { m_func(); }

public:
	StaticPatcher( Patcher func );

	static void		Apply();
};

#endif