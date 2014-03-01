#ifndef __ANTENNAS
#define __ANTENNAS

#include "General.h"

#define NUM_ANTENNAS				8
#define NUM_ANTENNA_VERTS			10

class CAntenna
{
public:
	bool				m_bExists, m_bUpdatedThisFrame;
	unsigned int		m_nIdentifier;
	float				m_fNodeLength;
	float				m_fStiffness;
	CVector				m_vecPos[NUM_ANTENNA_VERTS];
	CVector				m_vecForce[NUM_ANTENNA_VERTS];

public:
	void				Update(const CVector& vecUp, const CVector& vecPos);
};

class CAntennas
{
private:
	static CAntenna		aAntennas[NUM_ANTENNAS];

public:
	static void			Init();
	static void			Update();
	static void			Render();
	static void			RegisterOne(unsigned int nIdentifier, CVector vecUp, CVector vecPos, float fLength, float fStiffness);

	static void			DebugDisplay();
	static void			Inject();
};

#endif