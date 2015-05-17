#include "StdAfx.h"
#include "Tasks.h"

#include "Pools.h"

WRAPPER void CTaskSimpleJetPack::RenderJetPack(CPed* pPed) { WRAPARG(pPed); EAXJMP(0x67F6A0); }
WRAPPER CTask* CTaskManager::GetSimplestActiveTask() const { EAXJMP(0x6819D0); }

void* CTask::operator new(size_t size)
{
	UNREFERENCED_PARAMETER(size);
	return CPools::GetTaskPool()->New();
}

void CTask::operator delete(void* ptr)
{
	CPools::GetTaskPool()->Delete(static_cast<CTask*>(ptr));
}