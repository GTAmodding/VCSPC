#include "StdAfx.h"

void* CBuilding::operator new(size_t size)
{
	UNREFERENCED_PARAMETER(size);
	return CPools::GetBuildingPool()->New();
}