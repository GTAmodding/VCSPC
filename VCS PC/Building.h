#ifndef __BUILDING
#define __BUILDING

#include "General.h"

class NOVMT CBuilding : public CEntity
{
public:
	CBuilding()
		: CEntity(0)
	{
		// TODO: Reverse?
		_asm
		{
			mov		eax, 403E00h
			mov		ecx, this
			call	eax
		}
	}

	void* operator new(size_t size);
	void operator delete(void* ptr);

	inline void			PreRender_Stub()
	{ CBuilding::PreRender(); }

	virtual void		PreRender() override;
};


// TODO: Move maybe?
class NOVMT CDummy : public CEntity
{
public:
	explicit inline CDummy(int dummy)
		: CEntity(dummy)
	{
		// Dummy ctor
	}
};

class CBuildingAux
{
	friend class				CBuilding;

private:
	class CRealTimeShadow*		m_pShadow;

public:
	inline class CRealTimeShadow*	GetShadow()
		{ return m_pShadow; }
	inline void						SetShadow(class CRealTimeShadow* pShadow)
		{ m_pShadow = pShadow; }

	static void					Construct(CBuilding* pParent);
	static CBuilding*			Destruct(CBuilding* pParent);
};

#endif