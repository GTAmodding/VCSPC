#ifndef __BUILDING
#define __BUILDING 

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
};

#endif