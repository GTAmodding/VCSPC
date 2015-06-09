#ifndef __YCOCG_H
#define __YCOCG_H

struct YCoCgPlugin
{
	bool	bIsYCoCg;
};

#define YCOCGPLUGINDATA(texture) \
		RWPLUGINOFFSET(YCoCgPlugin, texture, gYCoCgPluginOffset)

#define YCOCGPLUGINDATACONST(texture) \
		RWPLUGINOFFSETCONST(YCoCgPlugin, texture, gYCoCgPluginOffset)


BOOL YCoCgPluginAttach();
void* YCoCgCreatePixelShader();

extern RwInt32 gYCoCgPluginOffset;


#endif