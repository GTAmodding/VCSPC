#include "StdAfx.h"
#include "Antennas.h"

#include "Timer.h"
#include "Font.h"
#include "Vehicle.h"

CAntenna		CAntennas::aAntennas[NUM_ANTENNAS];

void CAntenna::Update(const CVector& vecUp, const CVector& vecPos)
{
	m_vecPos[0] = vecPos;
	m_vecPos[1] = vecPos + vecUp * m_fNodeLength;

	CVector		vecSpringConstant = vecUp * CTimer::ms_fTimeStep * (0.19f/(1000.0f/600.0f));

	for ( int i = 2; i < NUM_ANTENNA_VERTS; ++i )
	{
		CVector         vecTempVec1 = m_vecPos[i] + m_vecForce[i] + vecSpringConstant;
		CVector         vecNewDelta = (vecTempVec1 - m_vecPos[i-1]).Normalise() * m_fNodeLength;
		CVector			vecDelta = m_vecPos[i-1] - m_vecPos[i-2];
		
        CVector         vecNewPos = (vecDelta + m_vecPos[i-1] + vecNewDelta + m_vecPos[i-1]) * 0.5f;

        m_vecForce[i] = (vecNewPos - m_vecPos[i]) * m_fStiffness/*0.9f*/;
        m_vecPos[i] = vecNewPos;
	}
}

void CAntennas::Init()
{
	for ( int i = 0; i < NUM_ANTENNAS; ++i )
		aAntennas[i].m_bExists = aAntennas[i].m_bUpdatedThisFrame = false;
}

void CAntennas::Update()
{
	for ( int i = 0; i < NUM_ANTENNAS; ++i )
	{
		if ( aAntennas[i].m_bExists && !aAntennas[i].m_bUpdatedThisFrame )
			aAntennas[i].m_bExists = false;
		aAntennas[i].m_bUpdatedThisFrame = false;
	}
}

void CAntennas::Render()
{
	for ( int i = 0; i < NUM_ANTENNAS; ++i )
	{
		if ( aAntennas[i].m_bExists )
		{
			RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, reinterpret_cast<void*>(TRUE));
			RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, reinterpret_cast<void*>(TRUE));
			RwRenderStateSet(rwRENDERSTATESRCBLEND, reinterpret_cast<void*>(rwBLENDSRCALPHA));
			RwRenderStateSet(rwRENDERSTATEDESTBLEND, reinterpret_cast<void*>(rwBLENDINVSRCALPHA));
			RwRenderStateSet(rwRENDERSTATETEXTURERASTER, nullptr);

			for ( int j = 0; j < NUM_ANTENNA_VERTS-1; ++j )
			{
				RwIm3DVertex		verts[2];

				RwIm3DVertexSetPos(&verts[0], aAntennas[i].m_vecPos[j].x, aAntennas[i].m_vecPos[j].y, aAntennas[i].m_vecPos[j].z);
				RwIm3DVertexSetPos(&verts[1], aAntennas[i].m_vecPos[j+1].x, aAntennas[i].m_vecPos[j+1].y, aAntennas[i].m_vecPos[j+1].z);
				/*RwIm3DVertexSetRGBA(&verts[0], 0xC8, 0xC8, 0xC8, 0x64);
				RwIm3DVertexSetRGBA(&verts[1], 0xC8, 0xC8, 0xC8, 0x64);*/
				RwIm3DVertexSetRGBA(&verts[0], 0xC8, 0xC8, 0xC8, 0x80);
				RwIm3DVertexSetRGBA(&verts[1], 0xC8, 0xC8, 0xC8, 0x80);

				if ( RwIm3DTransform(verts, 2, nullptr, 0) )
				{
					RwIm3DRenderLine(0, 1);
					RwIm3DEnd();
				}
			}
		}
	}

	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, FALSE);
}

void CAntennas::RegisterOne(unsigned int nIdentifier, CVector vecUp, CVector vecPos, float fLength, float fStiffness)
{
	int		i = 0;
	int		nFreeIndex = -1;

	for ( ; i < NUM_ANTENNAS; ++i )
	{
		if (!aAntennas[i].m_bExists )
		{
			if ( nFreeIndex == -1 )
				nFreeIndex = i;
		}
		else
		{
			if ( aAntennas[i].m_nIdentifier == nIdentifier )
				break;
		}
	}

	if ( i < NUM_ANTENNAS )
	{
		aAntennas[i].Update(vecUp, vecPos);
		aAntennas[i].m_bUpdatedThisFrame = true;
	}
	else
	{
		assert(nFreeIndex != -1);
		if ( nFreeIndex != -1 )
		{
			aAntennas[nFreeIndex].m_bExists = true;
			aAntennas[nFreeIndex].m_bUpdatedThisFrame = true;
			aAntennas[nFreeIndex].m_fNodeLength = fLength * (1.0f/NUM_ANTENNA_VERTS);
			aAntennas[nFreeIndex].m_fStiffness = fStiffness;
			aAntennas[nFreeIndex].m_nIdentifier = nIdentifier;

			for ( int j = 0; j < NUM_ANTENNA_VERTS; ++j )
			{
				aAntennas[nFreeIndex].m_vecPos[j] = vecUp * aAntennas[nFreeIndex].m_fNodeLength + vecPos;
				aAntennas[nFreeIndex].m_vecForce[j] = CVector(0.0, 0.0, 0.0);
			}
		}
	}
}

void CAntennas::DebugDisplay()
{
	for ( int i = 0; i < NUM_ANTENNAS; i++ )
	{
		if ( aAntennas[i].m_bExists )
		{
			CAntenna&		a = aAntennas[i];

			for ( int j = 0; j < NUM_ANTENNA_VERTS; j++ )
			{
				char			Temp[256];

				sprintf(Temp, "%d: Force %g %g %g", j, a.m_vecForce[j].x, a.m_vecForce[j].y, a.m_vecForce[j].z);

				CFont::SetProportional(true);
				CFont::SetEdge(1);
				CFont::SetFontStyle(FONT_Eurostile);
				CFont::SetScale(_width(0.3f), _height(0.5f));
				CFont::SetOrientation(ALIGN_Left);
				CFont::SetColor(CRGBA(255, 255, 255, 255));
				CFont::SetDropColor(CRGBA(0, 0, 0, 255));
				CFont::PrintString(_xleft(10.0f), _y(10.0f + 10.0f * j), Temp);
			}
		}
	}
}


void CAntennas::Inject()
{
	// Jump from the end of CGlass::Init()
	Memory::InjectHook(0x720560, Init, PATCH_JUMP);

	// Jump from the end of CGlass::Update()
	Memory::InjectHook(0x71B0F0, Update, PATCH_JUMP);

	// Replaces a nullsub CMovingThings::Render()
	Memory::InjectHook(0x53E184, Render);
}

static StaticPatcher	Patcher([](){ 
						CAntennas::Inject();
						InjectMethod(0x6AAB8B, CAutomobile::RenderAntennas, PATCH_NOTHING);
									});