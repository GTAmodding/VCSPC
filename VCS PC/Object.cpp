#include "StdAfx.h"
#include "Object.h"

#include "ModelInfo.h"

unsigned short	MI_PARKBENCH = 0xFFFF, MI_CANOPY_TEST = 0xFFFF, MI_CHAIR_TEST = 0xFFFF, MI_PAPERMACHINE = 0xFFFF,
				MI_HYDRANT = 0xFFFF;

void MatchAllModelStrings()
{
	MI_PARKBENCH = CModelInfo::GetModelInfoUInt16("parkbench1");
	MI_CANOPY_TEST = CModelInfo::GetModelInfoUInt16("kb_canopy_test");
	MI_CHAIR_TEST = CModelInfo::GetModelInfoUInt16("kb_chr_tbl_test");
	MI_PAPERMACHINE = CModelInfo::GetModelInfoUInt16("papermachn01");
	MI_HYDRANT = CModelInfo::GetModelInfoUInt16("fire_hydrant");
}

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


static StaticPatcher	Patcher([](){ 
						// Flying components have proper colour
						Memory::InjectHook(0x59F180, &CObject::Render_Stub, PATCH_JUMP);
						});