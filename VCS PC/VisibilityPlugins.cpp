#include "StdAfx.h"
#include "VisibilityPlugins.h"

CLinkList<CVisibilityPlugins::AlphaObjectInfo>&		CVisibilityPlugins::m_alphaList = *(CLinkList<CVisibilityPlugins::AlphaObjectInfo>*)0xC88070;

void CVisibilityPlugins::RenderAlphaAtomics()
{
	RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, 0);
	RenderOrderedList(m_alphaList);
}