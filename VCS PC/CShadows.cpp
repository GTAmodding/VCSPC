#include "StdAfx.h"

void CShadows::RenderIndicatorShadow(RpAtomic* pData, int, int, CVector* position, float radiusX, int, int, float radiusY)
{
	float	realShadowRadius = ( radiusX > -radiusY ) ? radiusX : -radiusY;

	C3DMarkers::PlaceMarkerSet(pData, 1, position, realShadowRadius, MARKER_SET_COLOR_R, MARKER_SET_COLOR_G, MARKER_SET_COLOR_B, MARKER_SET_COLOR_A, 2048, 0.2f);
}