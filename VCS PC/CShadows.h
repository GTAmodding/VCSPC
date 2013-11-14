#ifndef __CSHADOWS
#define __CSHADOWS

#define WIN32_LEAN_AND_MEAN

class CShadows
{
public:
	static void		RenderIndicatorShadow(RpAtomic* pData, int, int, CVector* position, float radiusX, int, int, float radiusY);
};

#endif