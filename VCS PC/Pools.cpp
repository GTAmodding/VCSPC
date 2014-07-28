#include "StdAfx.h"
#include "Pools.h"

CPedPool*&					CPools::ms_pPedPool = *(CPedPool**)0xB74490;
CVehiclePool*&				CPools::ms_pVehiclePool = *(CVehiclePool**)0xB74494;
CBuildingPool*&				CPools::ms_pBuildingPool = *(CBuildingPool**)0xB74498;
CDummyPool*&				CPools::ms_pDummyPool = *(CDummyPool**)0xB744A0;
CColModelPool*&				CPools::ms_pColModelPool = *(CColModelPool**)0xB744A4;
CTaskPool*&					CPools::ms_pTaskPool = *(CTaskPool**)0xB744A8;

CEmpireBuildingDataPool*	CPools::ms_pEmpireBuildingDataPool;
CPedPoolEx*				CPools::ms_pPedPoolEx;
CBuildingPoolEx*			CPools::ms_pBuildingPoolEx;

void CPools::Initialise()
{
	((void(*)())0x550F10)();

	// Initialise custom pools
	ms_pEmpireBuildingDataPool = new CEmpireBuildingDataPool(75);

	// Initialise children pools
	if ( ms_pPedPool )
		ms_pPedPoolEx = new CPedPoolEx(ms_pPedPool);

	if ( ms_pBuildingPool )
		ms_pBuildingPoolEx = new CBuildingPoolEx(ms_pBuildingPool);
}

void CPools::ShutDown()
{
	((void(*)())0x5519F0)();

	// Shutdown custom pools
	delete ms_pEmpireBuildingDataPool;

	// Shutdown children pools
	delete ms_pPedPoolEx;
	delete ms_pBuildingPoolEx;
}