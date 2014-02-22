#include "StdAfx.h"
#include "3DMarkers.h"

#include "Timer.h"

// Static variables
float		C3DMarkers::m_PosZMult;
const float	C3DMarkers::m_MovingMultiplier = 0.25f;

WRAPPER C3DMarker* C3DMarkers::PlaceMarker(unsigned int nIndex, unsigned short markerID, CVector& vecPos, float fSize, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, unsigned short pulsePeriod, float pulseFraction, short rotateRate, float normalX, float normalY, float normalZ, bool checkZ)
	{	WRAPARG(nIndex); WRAPARG(markerID); WRAPARG(vecPos); WRAPARG(fSize); WRAPARG(red); WRAPARG(green); WRAPARG(blue); WRAPARG(alpha);
		WRAPARG(pulsePeriod); WRAPARG(pulseFraction); WRAPARG(rotateRate); WRAPARG(normalX); WRAPARG(normalY); WRAPARG(normalZ);
		WRAPARG(checkZ); EAXJMP(0x725120); }

void C3DMarkers::PlaceMarkerSet(unsigned int nIndex, unsigned short markerID, CVector& vecPos, float fSize, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, unsigned short pulsePeriod, float pulseFraction)
{
	PlaceMarker(nIndex, markerID, vecPos, fSize, red, green, blue, static_cast<unsigned char>(alpha * (1.0f/3.0f)), pulsePeriod, pulseFraction, 1, 0.0, 0.0, 0.0, false);
	PlaceMarker(nIndex, markerID, vecPos, fSize * 0.9f, red, green, blue, static_cast<unsigned char>(alpha * (1.0f/3.0f)), pulsePeriod, pulseFraction, -1, 0.0, 0.0, 0.0, false);
}

long double C3DMarker::CalculateRealSize()
{
	long double		fVariable = (((m_nPulsePeriod - 1)) & ( CTimer::m_snTimeInMilliseconds /*- m_nStartTime*/ ));
	return (2 * M_PI) * fVariable / static_cast<long double>(m_nPulsePeriod);
}