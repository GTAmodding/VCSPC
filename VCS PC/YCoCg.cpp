#include "StdAfx.h"
#include "YCoCg.h"

#include "Rs.h"


void* YCoCgCreatePixelShader()
{
	return RwD3D9CreatePixelShaderFromFile("gen_YCoCg");
}