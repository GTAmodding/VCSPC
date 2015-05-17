#ifndef __TASKS
#define __TASKS

class CTask
{
private:
	CTask*					m_pParentTask;

public:
	virtual					~CTask() { };
	virtual CTask*			Clone() const=0;
	virtual CTask*			GetSubTask() const=0;
	virtual bool			IsSimpleTask() const=0;
	virtual unsigned int	GetTaskType() const=0;
	virtual void			StopTimer(const class CEvent* pEvent) { UNREFERENCED_PARAMETER(pEvent); };
	virtual void			MakeAbortable(class CPed* pPed, int nTaskType, const class CEvent* pEvent)=0;

	void*					operator new(size_t size);
	void					operator delete(void* ptr);

	CTask()
		: m_pParentTask(nullptr)
	{
	}

	inline void				SetParentTask(CTask* pParent)
		{ m_pParentTask = pParent; }
};

class CTaskSimple : public CTask
{
public:
	virtual CTask*			GetSubTask() const override { return nullptr; };
	virtual bool			IsSimpleTask() const override { return true; };

	virtual bool			ProcessPed(class CPed* pPed)=0;
	virtual bool			SetPedPosition(class CPed* pPed) { UNREFERENCED_PARAMETER(pPed); return false; };
};

class CTaskComplex : public CTask
{
private:
	CTask*					m_pSubTask;

public:
	virtual CTask*			GetSubTask() const override { return m_pSubTask; };
	virtual bool			IsSimpleTask() const override { return false; };
	virtual void			MakeAbortable(class CPed* pPed, int nTaskType, const class CEvent* pEvent) override
	{ 
		m_pSubTask->MakeAbortable(pPed, nTaskType, pEvent);
	}

	virtual void			SetSubTask(CTask* pSubTask)
	{
		if ( pSubTask != m_pSubTask )
		{
			if ( m_pSubTask )
				delete m_pSubTask;
			m_pSubTask = pSubTask;
			if ( pSubTask )
				pSubTask->SetParentTask(this);
		}
	}
	virtual CTask*			CreateNextSubTask(class CPed* pPed)=0;
	virtual CTask*			CreateFirstSubTask(class CPed* pPed)=0;
	virtual CTask*			ControlSubTask(class CPed* pPed)=0;

	CTaskComplex()
		: m_pSubTask(nullptr)
	{
	}

	virtual ~CTaskComplex()
	{
		delete m_pSubTask;
		m_pSubTask = nullptr;
	}
};

class NOVMT CTaskSimpleJetPack : public CTaskSimple
{
public:
	void			RenderJetPack(class CPed* pPed);
};

class CTaskManager
{
public:
	CTask*					GetSimplestActiveTask() const;
};

//static_assert(sizeof(CTaskSimpleJetPack) == 0x70, "Wrong size: CTaskSimpleJetPack");

#endif