#include "StdAfx.h"
#include "Gangs.h"

CGangInfo* const		CGangs::Gang = (CGangInfo*)0xC091F0;

void CGangs::Initialise()
{
	Gang[0].m_nGangWeapons[0] = 22;
	Gang[0].m_nGangWeapons[1] = 28;
	Gang[0].m_nGangWeapons[2] = 0;

	Gang[1].m_nGangWeapons[0] = 22;
	Gang[1].m_nGangWeapons[1] = 0;
	Gang[1].m_nGangWeapons[2] = 0;

	Gang[2].m_nGangWeapons[0] = 5;
	Gang[2].m_nGangWeapons[1] = 22;
	Gang[2].m_nGangWeapons[2] = 0;

	Gang[3].m_nGangWeapons[0] = 31;
	Gang[3].m_nGangWeapons[1] = 0;
	Gang[3].m_nGangWeapons[2] = 0;

	Gang[4].m_nGangWeapons[0] = 22;
	Gang[4].m_nGangWeapons[1] = 28;
	Gang[4].m_nGangWeapons[2] = 0;

	Gang[5].m_nGangWeapons[0] = 22;
	Gang[5].m_nGangWeapons[1] = 0;
	Gang[5].m_nGangWeapons[2] = 0;

	Gang[6].m_nGangWeapons[0] = 22;
	Gang[6].m_nGangWeapons[1] = 0;
	Gang[6].m_nGangWeapons[2] = 0;

	Gang[7].m_nGangWeapons[0] = 22;
	Gang[7].m_nGangWeapons[1] = 28;
	Gang[7].m_nGangWeapons[2] = 0;
}


static StaticPatcher	Patcher([](){
						Memory::InjectHook(0x5DE680, CGangs::Initialise, PATCH_JUMP);
						});