#include "StdAfx.h"

static CPad* const	Pads = (CPad*)0xB73458;

WRAPPER void CPad::UpdatePads() { EAXJMP(0x541DD0); }

CPad* CPad::GetPad(int nPad)
{
	return &Pads[nPad];
}