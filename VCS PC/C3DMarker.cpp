#include "StdAfx.h"

// Static variables
float		C3DMarkers::m_PosZMult;
const float	C3DMarkers::m_MovingMultiplier = 0.25f;

WRAPPER C3DMarker* C3DMarkers::PlaceMarker(RpAtomic* pData, unsigned int markerID, CVector* position, float fSize, unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha, unsigned int pulsePeriod, float pulseFraction, int rotateRate, float normalX, float normalY, float normalZ, bool checkZ)
	{	WRAPARG(pData); WRAPARG(markerID); WRAPARG(position); WRAPARG(fSize); WRAPARG(red); WRAPARG(green); WRAPARG(blue); WRAPARG(alpha);
		WRAPARG(pulsePeriod); WRAPARG(pulseFraction); WRAPARG(rotateRate); WRAPARG(normalX); WRAPARG(normalY); WRAPARG(normalZ);
		WRAPARG(checkZ); EAXJMP(0x725120); }

void C3DMarkers::PlaceMarkerSet(RpAtomic* pData, unsigned int markerID, CVector* position, float fSize, unsigned int red, unsigned int green, unsigned int blue, unsigned char alpha, int pulsePeriod, float pulseFraction)
{
	PlaceMarker(pData, markerID, position, fSize, red, green, blue, static_cast<unsigned int>(alpha * (1.0f/3.0f)), pulsePeriod, pulseFraction, 1, 0.0, 0.0, 0.0, false);
	PlaceMarker(pData, markerID, position, fSize * 0.9f, red, green, blue, static_cast<unsigned int>(alpha * (1.0f/3.0f)), pulsePeriod, pulseFraction, -1, 0.0, 0.0, 0.0, false);
}

long double C3DMarker::CalculateRealSize()
{
	long double		fVariable = (((m_nPulsePeriod - 1)) & ( CTimer::m_snTimeInMilliseconds /*- m_nStartTime*/ ));
	return (2 * M_PI) * fVariable / static_cast<long double>(m_nPulsePeriod);
}