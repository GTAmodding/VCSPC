#ifndef __RWWRAP
#define __RWWRAP

#define WIN32_LEAN_AND_MEAN

#define RW_RpClumpForAllAtomics				0x749B70
#define RW_RpGeometryForAllMaterials		0x74C790
#define RW_RwTextureCreate					0x7F37C0
#define RW_RwTextureRead					0x7F3AC0
#define RW_RwTextureDestroy					0x7F3820
#define RW_RwTextureSetRaster				0x7F35D0
#define RW_RwTextureSetName					0x7F38A0
#define RW_RwRasterCreate					0x7FB230
#define RW_RwRasterDestroy					0x7FB020
#define RW_RwRasterLock						0x7FB2D0
#define RW_RwRasterUnlock					0x7FAEC0
#define RW_RwRasterSetFromImage				0x804290
#define RW_RpClumpAddAtomic					0x74A490
#define RW_RwFrameSetIdentity				0x7F10B0
#define RW_RpClumpCreate					0x74A290
#define RW_RwCameraClear					0x7EE340
#define RW_RwImageDestroy					0x802740
#define RW_RwImageFindRasterFormat			0x8042C0
#define RW_RtPNGImageRead					0x7CF9B0

#define FUNC_DoRWStuffEndOfFrame			0x53D840

void	DoRWStuffEndOfFrame();

#endif