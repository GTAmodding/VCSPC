#include "StdAfx.h"
#include "CustomBuildingRenderer.h"

RwInt32& CCustomBuildingDNPipeline::ms_extraVertColourPluginOffset = *(RwInt32*)0x8D12BC;

RwInt32 CCustomBuildingDNPipeline::pluginExtraVertColourStreamGetSizeCB(const void* object, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
	//return -1;
	pluginExtraVertColour*	pData = EXTRAVERTCOLOURPLUGIN(object);
	return pData->nightColours ? sizeof(pData->nightColours) + sizeof(*pData->nightColours) * RpGeometryGetNumVertices(static_cast<const RpGeometry*>(object)) : -1;
}

RwStream* CCustomBuildingDNPipeline::pluginExtraVertColourStreamWriteCB(RwStream* stream, RwInt32 binaryLength, const void* object, RwInt32 offsetInObject, RwInt32 sizeInObject)
{
	pluginExtraVertColour*	pData = EXTRAVERTCOLOURPLUGIN(object);
	RwStreamWrite(stream, pData->nightColours, sizeof(pData->nightColours));
	if ( pData->nightColours )
		RwStreamWrite(stream, pData->nightColours, sizeof(*pData->nightColours) * RpGeometryGetNumVertices(static_cast<const RpGeometry*>(object)));
	return stream;
}

void CCustomBuildingDNPipeline::Inject()
{
	Memory::Patch(0x5D7310, pluginExtraVertColourStreamGetSizeCB);
	Memory::Patch(0x5D7315, pluginExtraVertColourStreamWriteCB);
}

static StaticPatcher	Patcher([](){
						CCustomBuildingDNPipeline::Inject();
									});