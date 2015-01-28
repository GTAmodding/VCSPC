#ifndef __BREAKABLE
#define __BREAKABLE

// Breakable geometry plugin
struct Breakable
{
	RwBool					posToObjPos;
	RwUInt16				vertexCount;
	RwV3d*					vertexOffset;
	RwV2d*					texCoorsOffset;
	RwRGBA*					vertexColorsOffset;
	RwUInt16				faceCount;
	void*					facesOffset;
	void*					materialsOffset;
	RwUInt16				materialCount;
	RwTexture*				texturesOffset;
	RwChar					(*texNamesOffset)[32];
	RwChar					(*maskNamesOffset)[32];
	RwSurfaceProperties*	materialPropOffset;
};

#define BREAKABLEPLUGIN(base) RWPLUGINOFFSET(Breakable*, (base), g_BreakablePlugin)
#define BREAKABLEPLUGINCONST(base) RWPLUGINOFFSETCONST(Breakable*, (base), g_BreakablePlugin)

extern RwInt32&		g_BreakablePlugin;

static_assert(sizeof(Breakable) == 52, "Wrong size: Breakable");

#endif