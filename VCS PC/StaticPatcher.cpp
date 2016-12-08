#include "StdAfx.h"
#include "StaticPatcher.h"

StaticPatcher* StaticPatcher::ms_head = nullptr;

StaticPatcher::StaticPatcher( Patcher func )
	: m_func( func )
{
	m_next = ms_head;
	ms_head = this;
}

void StaticPatcher::Apply()
{
	StaticPatcher* current = ms_head;
	while ( current != nullptr )
	{
		current->Run();
		current = current->m_next;
	}
	ms_head = nullptr;
}