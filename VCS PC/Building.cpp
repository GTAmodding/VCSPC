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

void CBuilding::PreRender()
{
	CEntity::PreRender();
	CShadows::StoreRealTimeShadowForBuilding(this);
}

void CBuildingAux::Construct(CBuilding* pParent)
{
	CBuildingAux*	pSpace = CPools::GetBuildingPoolAux()->GetAtPointer(pParent);

	// ctor
	pSpace->m_pShadow = nullptr;
}

CBuilding* CBuildingAux::Destruct(CBuilding* pParent)
{
	CBuildingAux*	pSpace = CPools::GetBuildingPoolAux()->GetAtPointer(pParent);

	// dtor
	if ( pSpace->m_pShadow )
		g_realTimeShadowMan.ReturnRealTimeShadow(pSpace->m_pShadow);

	return pParent;
}

static void __declspec(naked) CtorHack()
{
	_asm
	{
		mov		dword ptr [esi], 8585C8h
		push	esi
		call	CBuildingAux::Construct
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
		call	CBuildingAux::Destruct
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
									});