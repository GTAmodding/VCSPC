#include "StdAfx.h"
#include "PedIntelligence.h"

WRAPPER CTaskSimple* CPedIntelligence::GetTaskSwim() const { EAXJMP(0x601070); }
WRAPPER CTaskSimpleJetPack* CPedIntelligence::GetTaskJetPack() const { EAXJMP(0x601110); }
WRAPPER CTaskSimple* CPedIntelligence::GetTaskUseGun() const { EAXJMP(0x600F70); }

CTaskSimple* CPedIntelligence::GetTaskGangDriveby() const
{
	auto	pTask = m_taskManager.GetSimplestActiveTask();
	if ( pTask )
		return pTask->GetTaskType() == 1022 ? static_cast<CTaskSimple*>(pTask) : nullptr;
	return nullptr;
}