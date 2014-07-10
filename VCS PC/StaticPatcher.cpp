#include "StdAfx.h"
#include "StaticPatcher.h"

std::vector<std::function<void()>>*			StaticPatcher::m_pFunctions;

StaticPatcher::StaticPatcher(std::function<void()> pInitializer)
{
	if ( !m_pFunctions )
		m_pFunctions = new std::vector<std::function<void()>>;

	m_pFunctions->push_back(pInitializer);
}

void StaticPatcher::Apply()
{
	if ( m_pFunctions )
	{
		for ( auto it = m_pFunctions->cbegin(); it != m_pFunctions->cend(); it++ )
			(*it)();

		delete m_pFunctions;
	}
}