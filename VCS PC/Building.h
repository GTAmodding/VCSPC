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

#endif