#ifndef __RW
#define __RW

// For all the stuff that's not exposed in the public headers

// wanted to use this to find rasters with alpha but it looks like that's unreliable
typedef struct _rwD3D9RasterExt _rwD3D9RasterExt;
struct _rwD3D9RasterExt
{
    void                    *texture;
    void                    *palette;
    RwUInt8                 alpha;              /* This texture has alpha */
    RwUInt8                 cube : 4;           /* This texture is a cube texture */
    RwUInt8                 face : 4;           /* The active face of a cube texture */
    RwUInt8                 automipmapgen : 4;  /* This texture uses automipmap generation */
    RwUInt8                 compressed : 4;     /* This texture is compressed */
    RwUInt8                 lockedMipLevel;
    void                    *lockedSurface;
    D3DLOCKED_RECT          lockedRect;
    D3DFORMAT               d3dFormat;          /* D3D format */
    LPDIRECT3DSWAPCHAIN9    swapChain;
    HWND                    window;
};

RwUInt8	RwRasterHasAlpha(RwRaster *raster);


typedef struct _rwD3D9StageStateCache _rwD3D9StageStateCache;
struct _rwD3D9StageStateCache
{
    RwRaster                *curTexRaster;
    RwTextureAddressMode    texAddressU;
    RwTextureAddressMode    texAddressV;
    RwTextureFilterMode     filterMode;
    RwUInt32                borderColor;
    RwInt32                 maxAnisotropy;
};

// actually inside the bigger state cache struct
extern _rwD3D9StageStateCache *rwD3D9StageCache;

#endif
