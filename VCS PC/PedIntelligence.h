#ifndef __PEDINTELLIGENCE
#define __PEDINTELLIGENCE

#include "Tasks.h"

class CPedIntelligence
{
public:
	class CPed*					m_pPed;
	CTaskManager			m_taskManager;

public:
	inline CTaskManager*	GetTaskManager()
		{ return &m_taskManager; }

	CTaskSimple*				GetTaskUseGun() const;
	CTaskSimple*				GetTaskSwim() const;
	class CTaskSimpleJetPack*	GetTaskJetPack() const;
	CTaskSimple*				GetTaskGangDriveby() const;
};

#endif