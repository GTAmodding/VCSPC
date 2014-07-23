#include "StdAfx.h"
#include "PedIntelligence.h"

WRAPPER void* CPedIntelligence::GetTaskSwim() const { EAXJMP(0x601070); }
WRAPPER CTaskSimpleJetPack* CPedIntelligence::GetTaskJetPack() const { EAXJMP(0x601110); }