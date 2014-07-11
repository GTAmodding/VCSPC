#ifndef __SHADOWS
#define __SHADOWS

#include "General.h"

class CShadows
{
public:
	static void		RenderIndicatorShadow(unsigned int nIndex, unsigned char, RwTexture*, CVector* pPos, float radiusX, float, float, float radiusY, short);
};

#endif