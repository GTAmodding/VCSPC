#include "StdAfx.h"

CPedPool*&			CPools::ms_pPedPool = *(CPedPool**)0xB74490;
CVehiclePool*&		CPools::ms_pVehiclePool = *(CVehiclePool**)0xB74494;
CBuildingPool*&		CPools::ms_pBuildingPool = *(CBuildingPool**)0xB74498;
CDummyPool*&		CPools::ms_pDummyPool = *(CDummyPool**)0xB744A0;
CColModelPool*&		CPools::ms_pColModelPool = *(CColModelPool**)0xB744A4;