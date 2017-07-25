#ifndef __CUSTOMBUILDINGRENDERER
#define __CUSTOMBUILDINGRENDERER

struct pluginExtraVertColour
{
	RwRGBA*			nightColours;
	RwRGBA*			dayColours;
	float			balanceParam;
};

class CCustomBuildingDNPipeline
{
private:
	static RwInt32&			ms_extraVertColourPluginOffset;

private:
	static RwInt32			pluginExtraVertColourStreamGetSizeCB(const void* object, RwInt32 offsetInObject, RwInt32 sizeInObject);
	static RwStream*		pluginExtraVertColourStreamWriteCB(RwStream* stream, RwInt32 binaryLength, const void* object, RwInt32 offsetInObject, RwInt32 sizeInObject);

public:
	static void CustomPipeRenderCB(RwResEntry *repEntry, void *object, RwUInt8 type, RwUInt32 flags);
	static void				Inject();
};

#define EXTRAVERTCOLOURPLUGIN(base) RWPLUGINOFFSET(pluginExtraVertColour, (base), ms_extraVertColourPluginOffset)
#define EXTRAVERTCOLOURPLUGINCONST(base) RWPLUGINOFFSETCONST(pluginExtraVertColour, (base), ms_extraVertColourPluginOffset)

#endif