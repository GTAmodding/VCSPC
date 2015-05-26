#ifndef __MEMORYMGR
#define __MEMORYMGR

#define WRAPPER __declspec(naked)
#define DEPRECATED __declspec(deprecated)
#define EAXJMP(a) { _asm mov eax, a _asm jmp eax }
#define VARJMP(a) { _asm jmp a }
#define WRAPARG(a) UNREFERENCED_PARAMETER(a)

#define NOVMT __declspec(novtable)
#define SETVMT(a) *((DWORD_PTR*)this) = (DWORD_PTR)a

enum
{
	PATCH_CALL,
	PATCH_JUMP,
	PATCH_NOTHING,
};

// Debug checker to make sure reversed functions don't still get hacks
template<typename AT>
inline void CheckMemoryBanList(AT address)
{
#ifdef DEVBUILD
	unsigned int	nAddress = (unsigned int)address;

	assert(nAddress < 0x5794A0 || nAddress > 0x57B43F);
	assert(nAddress < 0x576FE0 || nAddress > 0x5773CF);
	assert(nAddress < 0x57CD50 || nAddress > 0x57D51F);
	assert(nAddress < 0x573440 || nAddress > 0x57367F);
	assert(nAddress < 0x72FC75 || nAddress > 0x72FD8F);
	assert(nAddress < 0x6FC185 || nAddress > 0x6FC4CF);
	assert(nAddress < 0x6FC4D5 || nAddress > 0x6FC57F);
	assert(nAddress < 0x6FADF0 || nAddress > 0x6FAEBF);
	assert(nAddress < 0x6FAEC0 || nAddress > 0x6FB62F);
	assert(nAddress < 0x744FB0 || nAddress > 0x7451AF);
	assert(nAddress < 0x5BA690 || nAddress > 0x5BA84F);
	assert(nAddress < 0x7189B0 || nAddress > 0x718A0F);
	assert(nAddress < 0x57E245 || nAddress > 0x57E4CF);
#else
	UNREFERENCED_PARAMETER(address);
#endif
}

namespace Memory
{
	template<typename T, typename AT>
	inline void		Patch(AT address, T value)
	{ CheckMemoryBanList(address); *(T*)address = value; }

	template<typename AT>
	inline void		Nop(AT address, unsigned int nCount)
	// TODO: Finish multibyte nops
	{ CheckMemoryBanList(address); memset((void*)address, 0x90, nCount); }

	template<typename AT, typename HT>
	inline void		InjectHook(AT address, HT hook, unsigned int nType=PATCH_NOTHING)
	{
		CheckMemoryBanList(address);
		DWORD		dwHook;
		_asm
		{
			mov		eax, hook
			mov		dwHook, eax
		}

		switch ( nType )
		{
		case PATCH_JUMP:
			*(BYTE*)address = 0xE9;
			break;
		case PATCH_CALL:
			*(BYTE*)address = 0xE8;
			break;
		}

		*(DWORD*)((DWORD)address + 1) = dwHook - (DWORD)address - 5;
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
		DWORD		dwHook;
		_asm
		{
			mov		eax, hook
			mov		dwHook, eax
		}

		*(DWORD*)((DWORD)address + 1) = (DWORD)dwHook - (DWORD)address - 5;
		if ( nType == PATCH_NOTHING )
			VirtualProtect((void*)((DWORD)address + 1), 4, dwProtect[0], &dwProtect[1]);
		else
			VirtualProtect((void*)address, 5, dwProtect[0], &dwProtect[1]);
	}
};

#endif