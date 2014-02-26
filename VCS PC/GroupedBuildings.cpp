#include "StdAfx.h"
#include "GroupedBuildings.h"

#include "ModelInfo.h"
#include "World.h"

int					CGroupedBuildings::m_nGroupsCount;
CGroupedBuilding	CGroupedBuildings::m_groups[NUM_GROUPED_BUILDINGS];

void CGroupedBuilding::RegisterOne(const char* pGroupName, const CVector& vecPos, float fAngle, int nNumObjs,
								const char* pModel1, const char* pModel2, const char* pModel3, const char* pModel4,
								const char* pModel5, const char* pModel6, const char* pModel7)
{
	m_pActualBuilding = nullptr;
	m_nCurrentID = -1;

	m_placement.m_translate = vecPos;
	m_placement.m_heading = fAngle;

	m_nGroupNameHash = HashHelper.FullCRC(reinterpret_cast<const unsigned char*>(pGroupName), strlen(pGroupName));
	m_nModelsInGroup = nNumObjs;

	// Prepare a helper array
	const char*		arr[] =  { pModel1, pModel2, pModel3, pModel4, pModel5, pModel6, pModel7 };

	for ( int i = 0; i < nNumObjs; i++ )
	{
		if ( _strnicmp(arr[i], "EMPTY", 6) )
			CModelInfo::GetModelInfo(arr[i],  &m_nModelID[i]);
		else
			m_nModelID[i] = -1;
	}
}

void CGroupedBuilding::SetActiveBuilding(int nIndex)
{
	if ( nIndex != m_nCurrentID )
	{
		m_nCurrentID = nIndex;

		if ( m_nModelID[nIndex] != -1 )
		{
			if ( !m_pActualBuilding )
			{
				// Place a new building
				CBuilding*		pTheBuilding = new CBuilding;

				pTheBuilding->SetModelIndexNoCreate(m_nModelID[nIndex]);
				pTheBuilding->SetCoords(m_placement.m_translate);
				pTheBuilding->SetHeading(m_placement.m_heading);

				pTheBuilding->m_areaCode = 13;

				m_pActualBuilding = pTheBuilding;

				CColModel*		pColModel = CModelInfo::ms_modelInfoPtrs[m_nModelID[nIndex]]->GetColModel();
				if ( pColModel )
				{
					unsigned char	bIndex = pColModel->level;
					if ( bIndex )
					{
						CRect		boundingRect = pTheBuilding->GetBoundRect();
						CRect*		pModelRect;
						// Pure laziness
						_asm
						{
							movzx		eax, bIndex
							push		eax
							mov			eax, 410800h
							call		eax
							add			esp, 4
							mov			pModelRect, eax
						}
						if ( boundingRect.x1 < pModelRect->x1 )
							pModelRect->x1 = boundingRect.x1;
						if ( boundingRect.x2 > pModelRect->x2 )
							pModelRect->x2 = boundingRect.x2;
						if ( boundingRect.y2 < pModelRect->y2 )
							pModelRect->y2 = boundingRect.y2;
						if ( boundingRect.y1 > pModelRect->y1 )
							pModelRect->y1 = boundingRect.y1;
					}
				}

			}
			else
			{
				CWorld::Remove(m_pActualBuilding);
				m_pActualBuilding->SetModelIndexNoCreate(m_nModelID[nIndex]);

			}
			CWorld::Add(m_pActualBuilding);
		}
		else
		{
			if ( m_pActualBuilding )
			{
				CWorld::Remove(m_pActualBuilding);
				m_pActualBuilding = nullptr;
			}
		}
	}
}


void CGroupedBuildings::Initialise()
{
	m_nGroupsCount = 0;

	if ( FILE* hFile = CFileMgr::OpenFile("DATA\\GROUPED_BUILDINGS.DAT", "r") )
	{
		while ( const char* pLine = CFileLoader::LoadLine(hFile) )
		{
			if ( pLine[0] && pLine[0] != '#' )
			{
				char		cGroupName[16], cModelName[NUM_MAX_BUILDINGS_IN_GROUP][24];
				int			nArgs;
				CVector		vecPos;
				float		fAngle;

				nArgs = sscanf(pLine, "%s %f %f %f %f %s %s %s %s %s %s %s", cGroupName, &vecPos.x, &vecPos.y, &vecPos.z, &fAngle,
					cModelName[0], cModelName[1], cModelName[2], cModelName[3], cModelName[4], cModelName[5], cModelName[6]);

				assert(nArgs-5 <= NUM_MAX_BUILDINGS_IN_GROUP);
				m_groups[m_nGroupsCount++].RegisterOne(cGroupName, vecPos, fAngle, nArgs-5, cModelName[0], cModelName[1], cModelName[2], cModelName[3], cModelName[4], cModelName[5], cModelName[6]);
			}
		}

		CFileMgr::CloseFile(hFile);
	}
}