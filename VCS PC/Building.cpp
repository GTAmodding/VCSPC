#include "StdAfx.h"
#include "Building.h"

#include "Pools.h"
#include "RealTimeShadowMgr.h"
#include "Shadows.h"

void* CBuilding::operator new(size_t size)
{
	UNREFERENCED_PARAMETER(size);
	return CPools::GetBuildingPool()->New();
}

void CBuilding::operator delete(void* ptr)
{
	CPools::GetBuildingPool()->Delete(static_cast<CBuilding*>(ptr));
}

CBuildingEx* CBuilding::GetEx()
{
	return CPools::GetBuildingPoolAux()->GetAtPointer(this);
}

void CBuilding::PreRender()
{
	CEntity::PreRender();
	CShadows::StoreRealTimeShadowForBuilding(this);
}

void CBuilding::ClearForRestart()
{
	CRealTimeShadow*		pShadow = GetEx()->m_pShadow;

	if ( pShadow )
		g_realTimeShadowMan.ReturnRealTimeShadow(pShadow);
}

void CBuildingEx::Construct(CBuilding* pParent)
{
	CBuildingEx*	pSpace = pParent->GetEx();

	// ctor
	pSpace->m_pShadow = nullptr;

	/*static bool		bAllocatingNow = false;
	// DEBUG
	if ( !bAllocatingNow )
	//if ( pParent->m_nModelIndex == 657 )
	{
		bAllocatingNow = true;
		// Pad space
		CBuilding*		pBuild1 = new CBuilding;
		memset((BYTE*)pBuild1+4, 0xCC, sizeof(CBuilding)-4);

		CBuilding*		pBuild2 = new CBuilding;
		memset((BYTE*)pBuild2+4, 0xCC, sizeof(CBuilding)-4);

		CBuilding*		pBuild3 = new CBuilding;
		memset((BYTE*)pBuild3+4, 0xCC, sizeof(CBuilding)-4);

		CBuilding*		pBuild4 = new CBuilding;
		memset((BYTE*)pBuild4+4, 0xCC, sizeof(CBuilding)-4);

		CBuilding*		pBuild5 = new CBuilding;
		memset((BYTE*)pBuild5+4, 0xCC, sizeof(CBuilding)-4);

		CBuilding*		pBuild6 = new CBuilding;
		memset((BYTE*)pBuild6+4, 0xCC, sizeof(CBuilding)-4);
		bAllocatingNow = false;
	}*/
}

CBuilding* CBuildingEx::Destruct(CBuilding* pParent)
{
	CBuildingEx*	pSpace = pParent->GetEx();

	// dtor
	if ( pSpace->m_pShadow )
	{
		g_realTimeShadowMan.ReturnRealTimeShadow(pSpace->m_pShadow);
		pSpace->m_pShadow = nullptr;
	}

	return pParent;
}

static void __declspec(naked) CtorHack()
{
	_asm
	{
		mov		dword ptr [esi], 8585C8h
		push	esi
		call	CBuildingEx::Construct
		add		esp, 4
		mov		eax, esi
		pop		esi
		retn
	}
}

static void __declspec(naked) DtorHack()
{
	_asm
	{
		push	ecx
		call	CBuildingEx::Destruct
		add		esp, 4
		mov		ecx, eax
		push	535E90h
		retn
	}
}

static StaticPatcher	Patcher([](){ 
						Memory::InjectHook(0x403E16, &CtorHack, PATCH_JUMP);
						Memory::InjectHook(0x404134, &DtorHack);
						Memory::Patch(0x85860C, &CBuilding::PreRender_Stub);

						//Memory::Patch(0x55105F, 200000);
									});