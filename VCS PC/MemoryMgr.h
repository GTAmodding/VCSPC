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
	assert(nAddress < 0x58D7D0 || nAddress > 0x58D99F);
	assert(nAddress < 0x745AF5 || nAddress > 0x745C6F);
	assert(nAddress < 0x745A80 || nAddress > 0x745AEF);
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
	inline void		InjectHook(AT address, HT hook)
	{
		CheckMemoryBanList(address);
		DWORD		dwHook;
		_asm
		{
			mov		eax, hook
			mov		dwHook, eax
		}

		*(ptrdiff_t*)((DWORD)address + 1) = dwHook - (DWORD)address - 5;
	}

	template<typename AT, typename HT>
	inline void		InjectHook(AT address, HT hook, unsigned int nType)
	{
		CheckMemoryBanList(address);
		DWORD		dwHook;
		_asm
		{
			mov		eax, hook
			mov		dwHook, eax
		}

		*(BYTE*)address = nType == PATCH_JUMP ? 0xE9 : 0xE8;

		*(ptrdiff_t*)((DWORD)address + 1) = dwHook - (DWORD)address - 5;
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
	{
		DWORD		dwProtect[2];
		VirtualProtect((void*)address, nCount, PAGE_EXECUTE_READWRITE, &dwProtect[0]);
		memset((void*)address, 0x90, nCount);
		VirtualProtect((void*)address, nCount, dwProtect[0], &dwProtect[1]);
	}

	template<typename AT, typename HT>
	inline void		InjectHook(AT address, HT hook)
	{
		DWORD		dwProtect[2];

		VirtualProtect((void*)((DWORD)address + 1), 4, PAGE_EXECUTE_READWRITE, &dwProtect[0]);
		DWORD		dwHook;
		_asm
		{
			mov		eax, hook
			mov		dwHook, eax
		}

		*(ptrdiff_t*)((DWORD)address + 1) = (DWORD)dwHook - (DWORD)address - 5;
		VirtualProtect((void*)((DWORD)address + 1), 4, dwProtect[0], &dwProtect[1]);
	}

	template<typename AT, typename HT>
	inline void		InjectHook(AT address, HT hook, unsigned int nType)
	{
		DWORD		dwProtect[2];

		VirtualProtect((void*)address, 5, PAGE_EXECUTE_READWRITE, &dwProtect[0]);
		*(BYTE*)address = nType == PATCH_JUMP ? 0xE9 : 0xE8;

		DWORD		dwHook;
		_asm
		{
			mov		eax, hook
			mov		dwHook, eax
		}

		*(ptrdiff_t*)((DWORD)address + 1) = (DWORD)dwHook - (DWORD)address - 5;
		VirtualProtect((void*)address, 5, dwProtect[0], &dwProtect[1]);
	}

	namespace DynBase
	{
		template<typename T, typename AT>
		inline void		Patch(AT address, T value)
		{
			MemoryVP::Patch(DynBaseAddress(address), value);
		}

		template<typename AT>
		inline void		Nop(AT address, unsigned int nCount)
		{
			MemoryVP::Nop(DynBaseAddress(address), nCount);
		}

		template<typename AT, typename HT>
		inline void		InjectHook(AT address, HT hook)
		{
			MemoryVP::InjectHook(DynBaseAddress(address), hook);
		}

		template<typename AT, typename HT>
		inline void		InjectHook(AT address, HT hook, unsigned int nType)
		{
			MemoryVP::InjectHook(DynBaseAddress(address), hook, nType);
		}
	};
};

#endif