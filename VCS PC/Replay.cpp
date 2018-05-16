#include "StdAfx.h"
#include "Replay.h"

unsigned char&		CReplay::Mode = *(unsigned char*)0xA43088;

WRAPPER void CReplay::Init(void) { EAXJMP(0x45E220); }
WRAPPER void CReplay::Update(void) { EAXJMP(0x460500); }

bool CReplay::ShouldStandardCameraBeProcessed(void) { return Mode != 1; }
