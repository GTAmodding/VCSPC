#include "StdAfx.h"

void CShadows::RenderIndicatorShadow(unsigned int nIndex, unsigned char, RwTexture*, CVector* pPos, float radiusX, float, float, float radiusY, short)
{
	C3DMarkers::PlaceMarkerSet(nIndex, 1, *pPos, radiusX > -radiusY ? radiusX : -radiusY, MARKER_SET_COLOR_R, MARKER_SET_COLOR_G, MARKER_SET_COLOR_B, MARKER_SET_COLOR_A, 2048, 0.2f);
}