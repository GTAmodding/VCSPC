#include "StdAfx.h"
#include "Building.h"

#include "Pools.h"

void* CBuilding::operator new(size_t size)
{
	UNREFERENCED_PARAMETER(size);
	return CPools::GetBuildingPool()->New();
}