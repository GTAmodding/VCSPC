#include "StdAfx.h"
#include "Breakable.h"

BreakManager_c &g_breakMan = *(BreakManager_c*)0xBB4240;

WRAPPER void BreakManager_c::Update(float t) { EAXJMP(0x59E670); }
WRAPPER void BreakManager_c::Render(bool b) { EAXJMP(0x59E6A0); }

RwInt32&		g_BreakablePlugin = *(RwInt32*)0xBB4238;

// Fixes a R* bug
RwInt32 BreakableStreamGetSize(const void* object, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
	UNREFERENCED_PARAMETER(offsetInObject);
	UNREFERENCED_PARAMETER(sizeInObject);

	Breakable* pData = *(BREAKABLEPLUGIN(object));
	return pData ? sizeof(pData) + sizeof(*pData) +
		pData->vertexCount * ( sizeof(*pData->vertexOffset) + sizeof(*pData->texCoorsOffset) + sizeof(*pData->vertexColorsOffset) ) +
		pData->faceCount * ( 6 + 2 ) +
		pData->materialCount * ( sizeof(*pData->texNamesOffset) + sizeof(*pData->maskNamesOffset) + sizeof(*pData->materialPropOffset) )
		: 4;
}

static StaticPatcher	Patcher([](){
						Memory::Patch(0x59D126, BreakableStreamGetSize);
									});