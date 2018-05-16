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

// Debug checker to make sure reversed functions don't still get hacks and aren't executed
// TODO: guard against non-static instances?
class Reversed
{
public:
	uint32 start;
	uint32 end;
	Reversed *next;

	static Reversed *reversed;
	Reversed(uint32 start, uint32 end) {
#ifdef DEVBUILD
		this->start = start;
		this->end = end;
		this->next = reversed;
		reversed = this;
		uint32 size = end-start;
		DWORD dwProtect[2];
		VirtualProtect((void*)start, size, PAGE_EXECUTE_READWRITE, &dwProtect[0]);
		memset((void*)start, 0xCC, size);
		VirtualProtect((void*)start, size, dwProtect[0], &dwProtect[1]);
#endif
	}
	static void check(uint32 address) {
#ifdef DEVBUILD
		for(Reversed *r = reversed; r; r = r->next)
			if(address >= r->start && address <= r->end)
				assert(0 && "address in reversed function");
#endif
	}
};

namespace Memory
{
	template<typename T, typename AT>
	inline void		Patch(AT address, T value)
	{ Reversed::check((uint32)address); *(T*)address = value; }

	template<typename AT>
	inline void		Nop(AT address, unsigned int nCount)
	// TODO: Finish multibyte nops
	{ Reversed::check((uint32)address); memset((void*)address, 0x90, nCount); }

	template<typename AT, typename HT>
	inline void		InjectHook(AT address, HT hook)
	{
		Reversed::check((uint32)address);
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
		Reversed::check((uint32)address);
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