#ifndef __YCOCG_H
#define __YCOCG_H

struct YCoCgPlugin
{
	RwUInt8		bYCoCgType;
};

#define YCOCGPLUGINDATA(texture) \
		RWPLUGINOFFSET(YCoCgPlugin, texture, gYCoCgPluginOffset)

#define YCOCGPLUGINDATACONST(texture) \
		RWPLUGINOFFSETCONST(YCoCgPlugin, texture, gYCoCgPluginOffset)


BOOL YCoCgPluginAttach();
void YCoCgCreatePixelShaders(void* pGenericShaders[]);

extern RwInt32 gYCoCgPluginOffset;


#endif