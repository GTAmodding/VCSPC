#include "stdafx.h"
#include "General.h"

#include "Building.h"
#include "Pools.h"

// Wrappers
WRAPPER bool CalcScreenCoors(const CVector& vecIn, CVector* vecOut) { WRAPARG(vecIn); WRAPARG(vecOut); EAXJMP(0x71DAB0); }
WRAPPER void LoadingScreenLoadingFile(const char* pText) { WRAPARG(pText); EAXJMP(0x5B3680); }

WRAPPER void CEntity::UpdateRW() { EAXJMP(0x446F90); }
WRAPPER void CEntity::RegisterReference(CEntity** pAddress) { WRAPARG(pAddress); EAXJMP(0x571B70); }
WRAPPER void CEntity::CleanUpOldReference(CEntity** pAddress) { WRAPARG(pAddress); EAXJMP(0x571A00); }
WRAPPER void CEntity::Render() { EAXJMP(0x534310); }
WRAPPER void CEntity::PreRender() { EAXJMP(0x535FA0); }

WRAPPER unsigned int CKeyGen::GetUppercaseKey(const char* pEntry) { WRAPARG(pEntry); EAXJMP(0x53CF30); }

unsigned char& CGame::bMissionPackGame = *(unsigned char*)0xB72910;

class CRealTimeShadow* CEntity::GetRealTimeShadow()
{
	if ( nType == 1 )
		return CPools::GetBuildingPoolAux()->GetAtPointer(static_cast<CBuilding*>(this))->GetShadow();

	return static_cast<CPhysical*>(this)->GetRealTimeShadow();
}


void CEntity::SetRealTimeShadow(class CRealTimeShadow* pShadow)
{
	if ( nType == 1 )
		CPools::GetBuildingPoolAux()->GetAtPointer(static_cast<CBuilding*>(this))->SetShadow(pShadow);
	else
		static_cast<CPhysical*>(this)->SetRealTimeShadow(pShadow);
}

/*static void __declspec(naked) EntityCtorHack()
{
	_asm
	{
		mov		[esi].bIveBeenRenderedOnce, bl
		mov		eax, esi
		pop		esi
		pop		ebx
		retn
	}
}

static void __declspec(naked) EntityRenderHack()
{
	_asm
	{
		and		dword ptr [esi+1Ch], 0FFFFDFFFh
		or		byte ptr [esi].bIveBeenRenderedOnce, 1
		pop		esi
		pop		ecx
		retn
	}
}*/


/*static StaticPatcher	Patcher([](){ 
						Memory::InjectHook(0x532AD7, EntityCtorHack, PATCH_JUMP);
						Memory::InjectHook(0x5343E4, EntityRenderHack, PATCH_JUMP);
									});*/