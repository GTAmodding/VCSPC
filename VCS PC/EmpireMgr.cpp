#include "StdAfx.h"
#include "EmpireMgr.h"

#include "ModelInfo.h"
#include "World.h"
#include "Pools.h"

CEmpire			CEmpireManager::m_empire[NUM_EMPIRES];
tEmpireType		CEmpireManager::m_empireType[NUM_EMPIRE_TYPES];

static inline unsigned int	PackKey(unsigned short nType, char nSubgroup)
{
	return nType << 16 | nSubgroup; 
}

static inline unsigned int	PackKey(unsigned short nType, char nSubgroup, unsigned char nIndex)
{
	return nType << 16 | nSubgroup << 8 | nIndex;
}

void CEmpireManager::Initialise()
{
	if ( FILE* hFile = CFileMgr::OpenFile("DATA\\EMPIRES.DAT", "r") )
	{
		enum
		{
			SECTION_NOTHING,
			SECTION_PLACES,
			SECTION_TYPES,
			SECTION_DATA
		} curFileSection = SECTION_NOTHING;
		CAtomicModelInfo*						pCurrentModelForDataParsing = nullptr;
		std::map<unsigned int,unsigned char>	countMap;

		while ( const char* pLine = CFileLoader::LoadLine(hFile) )
		{
			if ( pLine[0] && pLine[0] != '#' )
			{
				switch ( curFileSection )
				{
				case SECTION_NOTHING:
					{
						if ( SECTION4(pLine, 'p', 'l', 'a', 'c') )
							curFileSection = SECTION_PLACES;
						else if ( SECTION4(pLine, 't', 'y', 'p', 'e') )
							curFileSection = SECTION_TYPES;
						else if ( SECTION4(pLine, 'd', 'a', 't', 'a') )
							curFileSection = SECTION_DATA;
						break;
					}
				case SECTION_PLACES:
					{
						if ( SECTION3(pLine, 'e', 'n', 'd') )
							curFileSection = SECTION_NOTHING;
						else
						{
							CSimpleTransform		tempTransform;
							int						nIndex;

							sscanf(pLine, "%d %f %f %f %f", &nIndex, &tempTransform.m_translate.x, &tempTransform.m_translate.y, &tempTransform.m_translate.z, &tempTransform.m_heading);
							assert(nIndex >= 0 && nIndex < NUM_EMPIRES);

							tempTransform.m_heading *= static_cast<float>(DEG_TO_RAD);
							m_empire[nIndex].SetTransform(tempTransform);
							m_empire[nIndex].MarkAsDefined();
						}
						break;
					}
				case SECTION_TYPES:
					{
						if ( SECTION3(pLine, 'e', 'n', 'd') )
							curFileSection = SECTION_NOTHING;
						else
						{
							char			cNormalName[24], cDamagedName[24];
							int				nIndex;
							int				nNormalModel, nDamagedModel, n1stAuxModel = -1, n2ndAuxModel = -1, n3rdAuxModel = -1;;
							char			cType;

							sscanf(pLine, "%d %c %s %s", &nIndex, &cType, cNormalName, cDamagedName);
							assert(nIndex >= 0 && nIndex < NUM_EMPIRE_TYPES);

							CModelInfo::GetModelInfo(cNormalName, &nNormalModel);
							CModelInfo::GetModelInfo(cDamagedName, &nDamagedModel);
							
							// Get additional models
							CModelInfo::GetModelInfo(std::string(cNormalName + std::string("_n")).c_str(), &n1stAuxModel);		// Neon
							CModelInfo::GetModelInfo(std::string(cNormalName + std::string("_f")).c_str(), &n2ndAuxModel);		// Fence
							CModelInfo::GetModelInfo(std::string(cDamagedName + std::string("_a")).c_str(), &n3rdAuxModel);		// Alpha

							m_empireType[nIndex].nNormalBuilding[toupper(cType) - 'A'] = nNormalModel;
							m_empireType[nIndex].nDamagedBuilding[toupper(cType) - 'A'] = nDamagedModel;
							m_empireType[nIndex].nAdditionalModel[toupper(cType) - 'A'][0] = n1stAuxModel;
							m_empireType[nIndex].nAdditionalModel[toupper(cType) - 'A'][1] = n2ndAuxModel;
							m_empireType[nIndex].nAdditionalModel[toupper(cType) - 'A'][2] = n3rdAuxModel;

							// Collisions are MINE, bitch!
							// EDIT: Crashes
							//CModelInfo::ms_modelInfoPtrs[nNormalModel]->bDoWeOwnTheColModel = false;
							//CModelInfo::ms_modelInfoPtrs[nDamagedModel]->bDoWeOwnTheColModel = false;
						}
						break;
					}
				case SECTION_DATA:
					{
						if ( SECTION3(pLine, 'e', 'n', 'd') )
						{
							/*if ( pCurrentModelForDataParsing )
								pCurrentModelForDataParsing->GetEmpireData()->ReduceContainerSize();*/
							curFileSection = SECTION_NOTHING;
						}
						else
						{
							if ( pLine[0] == '%' )
							{
								/*if ( pCurrentModelForDataParsing )
									pCurrentModelForDataParsing->GetEmpireData()->ReduceContainerSize();*/
								countMap.clear();
	
								pCurrentModelForDataParsing = CModelInfo::GetModelInfo(pLine+2)->AsAtomicModelInfoPtr();
								pCurrentModelForDataParsing->InitEmpireData();
							}
							else
							{		
								CSimpleTransform			tempData;
								unsigned int				nTypeID;
								char						nSubgroup;

								assert(pCurrentModelForDataParsing);
								sscanf(pLine, "%d %c %f %f %f %f", &nTypeID, &nSubgroup, &tempData.m_translate.x, &tempData.m_translate.y, &tempData.m_translate.z, &tempData.m_heading);
								nSubgroup = static_cast<char>(tolower(nSubgroup));

								// Normalize the angle
								while ( tempData.m_heading < 0.0f )
									tempData.m_heading += 360.0f;

								// Get the index and increase it afterwards
								unsigned char		nIndex = countMap[PackKey(static_cast<unsigned short>(nTypeID), nSubgroup)];
								pCurrentModelForDataParsing->GetEmpireData()->AddEntry(static_cast<unsigned short>(nTypeID), nSubgroup, nIndex++, tempData);
								countMap[PackKey(static_cast<unsigned short>(nTypeID), nSubgroup)] = nIndex;
							}
						}
						break;
					}
				}

			}
		}


		CFileMgr::CloseFile(hFile);
	}
}

void CEmpireManager::Process()
{
	/*for ( int i = 0; i < NUM_EMPIRES; ++i )
	{
		if ( m_empire[i].IsDefined() )
		{
			//if ( !m_empire[i].IsPlaced() )
				
				//m_empire[i].Place();
		}
	}*/
}

void* CEmpireBuildingData::operator new(size_t size)
{
	UNREFERENCED_PARAMETER(size);
	return CPools::GetEmpireBuildingDataPool()->New();
}

void CEmpireBuildingData::operator delete(void* ptr)
{
	CPools::GetEmpireBuildingDataPool()->Delete(static_cast<CEmpireBuildingData*>(ptr));
}

void CEmpireBuildingData::AddEntry(unsigned short nTypeID, char nSubgroup, unsigned char nIndex, const CSimpleTransform& vecPos)
{
	m_buildingData[PackKey(nTypeID, nSubgroup, nIndex)] = vecPos;
}

int CEmpireBuildingData::GetNumEntriesOfType(unsigned short nType)
{
	return std::distance(m_buildingData.lower_bound(PackKey(nType, 'a', 0)), m_buildingData.upper_bound(PackKey(nType, 'z', 0xFF)));
}

int CEmpireBuildingData::GetNumEntriesOfType(unsigned short nType, char nSubgroup)
{
	return std::distance(m_buildingData.lower_bound(PackKey(nType, nSubgroup, 0)), m_buildingData.upper_bound(PackKey(nType, nSubgroup, 0xFF)));
}

CSimpleTransform& CEmpireBuildingData::GetData(unsigned short nType, unsigned char nIndex)
{
	auto it = m_buildingData.lower_bound(PackKey(nType, 'a', 0));
	std::advance(it, nIndex);
	return it->second;
}

CSimpleTransform& CEmpireBuildingData::GetData(unsigned short nType, char nSubgroup, unsigned char nIndex)
{
	return m_buildingData[PackKey(nType, nSubgroup, nIndex)];
}

void CEmpireBuildingData::ReduceContainerSize()
{
	/*for ( auto it = m_buildingData.begin(); it != m_buildingData.end(); it++ )
		it->second.Entry.shrink_to_fit();*/
}

void CEmpire::Place()
{
	if ( m_nType == -1 || m_nScale == -1 )
		return;

	// TODO: Damaged check
	int				nModelIndex = CEmpireManager::GetInfo(m_nType)->nNormalBuilding[m_nScale];
	if ( nModelIndex == 0 )
		return;

	CBuilding*		pEmpireBuilding = new CBuilding;

	pEmpireBuilding->SetModelIndexNoCreate(nModelIndex);
	pEmpireBuilding->SetCoords(m_placement.m_translate);
	pEmpireBuilding->SetHeading(m_placement.m_heading);

	m_pBuilding[0] = pEmpireBuilding;

	// TODO: Damaged check
	for ( int i = 0; i < 2; i++ )
	{
		int		nAuxModel = CEmpireManager::GetInfo(m_nType)->nAdditionalModel[m_nScale][i];
		if ( nAuxModel != -1 )
		{
			CBuilding*		pAdditionalBuilding = new CBuilding;

			pAdditionalBuilding->SetModelIndexNoCreate(nAuxModel);
			pAdditionalBuilding->SetCoords(m_placement.m_translate);
			pAdditionalBuilding->SetHeading(m_placement.m_heading);

			m_pBuilding[i+1] = pAdditionalBuilding;
		}
	}

	m_bPlaced = true;

	CColModel*		pColModel = CModelInfo::ms_modelInfoPtrs[nModelIndex]->GetColModel();
	if ( pColModel )
	{
		unsigned char	bIndex = pColModel->level;
		if ( bIndex )
		{
			CRect		boundingRect = pEmpireBuilding->GetBoundRect();
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

	for ( int i = 0; i < 3; i++ )
	{
		if ( m_pBuilding[i] )
			CWorld::Add(m_pBuilding[i]);
	}
}

void CEmpire::Remove()
{
	for ( int i = 0; i < 3; i++ )
	{
		if ( m_pBuilding[i] )
		{
			CWorld::Remove(m_pBuilding[i]);

			delete m_pBuilding[i];
			m_pBuilding[i] = nullptr;
		}
	}
	m_bPlaced = false;
}