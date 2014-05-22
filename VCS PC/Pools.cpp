#include "StdAfx.h"
#include "Pools.h"

CPedPool*&					CPools::ms_pPedPool = *(CPedPool**)0xB74490;
CVehiclePool*&				CPools::ms_pVehiclePool = *(CVehiclePool**)0xB74494;
CBuildingPool*&				CPools::ms_pBuildingPool = *(CBuildingPool**)0xB74498;
CDummyPool*&				CPools::ms_pDummyPool = *(CDummyPool**)0xB744A0;
CColModelPool*&				CPools::ms_pColModelPool = *(CColModelPool**)0xB744A4;

CEmpireBuildingDataPool*	CPools::ms_pEmpireBuildingDataPool;
CPedPoolAux*				CPools::ms_pPedPoolAux;

void CPools::Initialise()
{
	((void(*)())0x550F10)();

	// Initialise custom pools
	ms_pEmpireBuildingDataPool = new CEmpireBuildingDataPool(25);

	// Initialise children pools
	if ( ms_pPedPool )
		ms_pPedPoolAux = new CPedPoolAux(ms_pPedPool);
}

void CPools::ShutDown()
{
	((void(*)())0x5519F0)();

	// Shutdown custom pools
	delete ms_pEmpireBuildingDataPool;

	// Shutdown children pools
	delete ms_pPedPoolAux;
}