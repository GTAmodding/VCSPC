#include "StdAfx.h"
#include "Object.h"

#include "ModelInfo.h"

void CObject::Render()
{
	if ( m_bDoNotRender )
		return;

	bool		bCallRestore;

	if ( m_wCarPartModelIndex != -1 && m_nObjectType == 3 && bObjectFlag7 && RwObjectGetType(m_pRwObject) == rpATOMIC )
	{
		auto* pData = materialRestoreData;

		CVehicleModelInfo::SetRemap(m_pPaintjobTex);
		CVehicleModelInfo::SetVehicleColour(m_nCarColor[0], m_nCarColor[1], m_nCarColor[2], m_nCarColor[3]);

		CVehicleModelInfo::SetEditableMaterialsCB(reinterpret_cast<RpAtomic*>(m_pRwObject), &pData);
		pData->first = nullptr;

		bCallRestore = true;
	}
	else
		bCallRestore = false;

	CEntity::Render();

	if ( bCallRestore )
		CVehicleModelInfo::ResetEditableMaterials();
}