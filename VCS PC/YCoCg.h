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

inline RwUInt8 RwRasterGetYCoCgType(RwRaster *ras)
{
	return YCOCGPLUGINDATA(ras)->bYCoCgType;
}

inline RwUInt8 RwTextureGetYCoCgType(RwTexture *tex)
{
	assert(tex->raster);
	return YCOCGPLUGINDATA(tex->raster)->bYCoCgType;
}


#endif