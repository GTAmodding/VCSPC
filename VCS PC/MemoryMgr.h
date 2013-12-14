#ifndef __MEMORYMGR
#define __MEMORYMGR

#define WRAPPER __declspec(naked)
#define EAXJMP(a) { _asm mov eax, a _asm jmp eax }
#define WRAPARG(a) UNREFERENCED_PARAMETER(a)

#define NOVMT __declspec(novtable)
#define SETVMT(a) *((DWORD_PTR*)this) = (DWORD_PTR)a

// A macro used to inject method pointers
#define InjectMethod(address, hook, nType) { void* __funcPtr; { _asm mov eax, offset hook _asm mov __funcPtr, eax } \
											Memory::InjectHook(address, __funcPtr, nType); }

enum
{
	PATCH_CALL,
	PATCH_JUMP,
	PATCH_NOTHING,
};

namespace Memory
{
	template<typename T, typename AT>
	inline void		Patch(AT address, T value)
	{ *(T*)address = value; }

	template<typename AT>
	inline void		Nop(AT address, unsigned int nCount)
	// TODO: Finish multibyte nops
	{ memset((void*)address, 0x90, nCount); }

	template<typename AT, typename HT>
	inline void		InjectHook(AT address, HT hook, unsigned int nType=PATCH_NOTHING)
	{
		switch ( nType )
		{
		case PATCH_JUMP:
			*(BYTE*)address = 0xE9;
			break;
		case PATCH_CALL:
			*(BYTE*)address = 0xE8;
			break;
		}
		*(DWORD*)((DWORD)address + 1) = (DWORD)hook - (DWORD)address - 5;
	}
};

namespace MemoryVP
{
	template<typename T, typename AT>
	inline void		Patch(AT address, T value)
	{
		DWORD		dwProtect[2];
		VirtualProtect((void*)address, sizeof(T), PAGE_EXECUTE_READWRITE, &dwProtect[0]);
		*(T*)address = value;
		VirtualProtect((void*)address, sizeof(T), dwProtect[0], &dwProtect[1]);
	}

	template<typename AT>
	inline void		Nop(AT address, unsigned int nCount)
	// TODO: Finish multibyte nops
	{
		DWORD		dwProtect[2];
		VirtualProtect((void*)address, nCount, PAGE_EXECUTE_READWRITE, &dwProtect[0]);
		memset((void*)address, 0x90, nCount);
		VirtualProtect((void*)address, nCount, dwProtect[0], &dwProtect[1]);
	}

	template<typename AT, typename HT>
	inline void		InjectHook(AT address, HT hook, unsigned int nType=PATCH_NOTHING)
	{
		DWORD		dwProtect[2];
		switch ( nType )
		{
		case PATCH_JUMP:
			VirtualProtect((void*)address, 5, PAGE_EXECUTE_READWRITE, &dwProtect[0]);
			*(BYTE*)address = 0xE9;
			break;
		case PATCH_CALL:
			VirtualProtect((void*)address, 5, PAGE_EXECUTE_READWRITE, &dwProtect[0]);
			*(BYTE*)address = 0xE8;
			break;
		default:
			VirtualProtect((void*)((DWORD)address + 1), 4, PAGE_EXECUTE_READWRITE, &dwProtect[0]);
			break;
		}
		*(DWORD*)((DWORD)address + 1) = (DWORD)hook - (DWORD)address - 5;
		if ( nType == PATCH_NOTHING )
			VirtualProtect((void*)((DWORD)address + 1), 4, dwProtect[0], &dwProtect[1]);
		else
			VirtualProtect((void*)address, 5, dwProtect[0], &dwProtect[1]);
	}
};

// Old code, remove asap
#define patch(a, v, s) _patch((void*)(a), (DWORD)(v), (s))
#define patchf(a, v) _patch((void*)(a), (float)(v))
#define nop(a, v) _nop((void*)(a), (v))
#define call(a, v, bAddCall) _call((void*)(a), (DWORD)(v), (bAddCall))
#define charptr(a, v) _charptr((void*)(a), (const char*)(v))

__declspec(deprecated) inline void _patch(void* pAddress, DWORD data, DWORD iSize)
{
	switch(iSize)
	{
		case 1: *(BYTE*)pAddress = (BYTE)data;
			break;
		case 2: *(WORD*)pAddress = (WORD)data;
			break;
		case 4: *(DWORD*)pAddress = (DWORD)data;
			break;
	}
}

__declspec(deprecated) inline void _patch(void* pAddress, float data)
{
	*(float*)pAddress = data;
}

__declspec(deprecated) inline void _nop(void* pAddress, DWORD size)
{
	DWORD dwAddress = (DWORD)pAddress;
	if ( size % 2 )
	{
		*(BYTE*)pAddress = 0x90;
		dwAddress++;
	}
	if ( size - ( size % 2 ) )
	{
		DWORD sizeCopy = size - ( size % 2 );
		do
		{
			*(WORD*)dwAddress = 0xFF8B;
			dwAddress += 2;
			sizeCopy -= 2;
		}
		while ( sizeCopy );
	}
}

__declspec(deprecated) inline void _call(void* pAddress, DWORD data, DWORD bPatchType)
{
	switch ( bPatchType )
	{
	case PATCH_JUMP:
		*(BYTE*)pAddress = (BYTE)0xE9;
		break;

	case PATCH_CALL:
		*(BYTE*)pAddress = (BYTE)0xE8;
		break;

	default:
		break;
	}

	*(DWORD*)((DWORD)pAddress + 1) = (DWORD)data - (DWORD)pAddress - 5;
}

__declspec(deprecated) inline void _charptr(void* pAddress, const char* pChar)
{
    *(DWORD*)pAddress = (DWORD)pChar;
}

#endif