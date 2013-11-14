#include "StdAfx.h"

// Static variables
CBaseModelInfo**					CModelInfo::ms_modelInfoPtrs = (CBaseModelInfo**)0xA9B0C8;

CDynamicStore<CPedModelInfoVCS>		CModelInfo::ms_pedModelStore;
CDynamicStore<CClumpModelInfo>		CModelInfo::ms_clumpModelStore;

CRGBA								CPedModelInfoVCS::ms_pedColourTable[NUM_PED_COLOURS];
BYTE								CPedModelInfoVCS::bLastPedPrimaryColour;
BYTE								CPedModelInfoVCS::bLastPedSecondaryColour;
BYTE								CPedModelInfoVCS::bLastPedTertiaryColour;
BYTE								CPedModelInfoVCS::bLastPedQuaternaryColour;

// Wrappers
WRAPPER CBaseModelInfo* CModelInfo::GetModelInfo(const char* pName, int* pOutID) { WRAPARG(pName); WRAPARG(pOutID); EAXJMP(0x4C5940); }

bool CModelInfo::IsBikeModel(int modelID)
{
	// Temp
	CBaseModelInfo*		pModel = ms_modelInfoPtrs[modelID];
	return pModel && pModel->GetModelType() == 6 && *(DWORD*)((BYTE*)pModel+0x3C) == 9;
}

CPedModelInfoVCS* CModelInfo::AddPedModel(int nModelID)
{
	CPedModelInfoVCS*	pNewEntry = ms_pedModelStore.Add();
	pNewEntry->Init();
	ms_modelInfoPtrs[nModelID] = pNewEntry;
	return pNewEntry;
}

CClumpModelInfo* CModelInfo::AddClumpModel(int nModelID)
{
	CClumpModelInfo*	pNewEntry = ms_clumpModelStore.Add();
	pNewEntry->Init();
	ms_modelInfoPtrs[nModelID] = pNewEntry;
	return pNewEntry;
}

void CModelInfo::ShutDown()
{
	// Iterate thru entire stores and call ShutDown() on each entry
	{
		// Ped model store
		auto		pEntry = ms_pedModelStore.GetFirstEntry();

		while ( pEntry )
		{
			pEntry->m_entry.Shutdown();
			pEntry = pEntry->m_pPrev;
		}
	}

	{
		// Clump model store
		auto		pEntry = ms_clumpModelStore.GetFirstEntry();

		while ( pEntry )
		{
			pEntry->m_entry.Shutdown();
			pEntry = pEntry->m_pPrev;
		}
	}
}

void CBaseModelInfo::AddRef()
{
	++usNumberOfRefs;
	CTxdStore::AddRef(usTextureDictionary);
}

void CPedModelInfoVCS::LoadPedColours()
{
	if ( FILE* hFile = CFileMgr::OpenFile("DATA\\PEDCOLS.DAT", "r") )
	{
		enum
		{
			SECTION_NOTHING,
			SECTION_RGB,
			SECTION_READPEDCOLS
		} curFileSection = SECTION_NOTHING;
		CRGBA*		pPedCols = ms_pedColourTable;
		//char		lineBuffer[512];

		while ( const char* pLine = CFileLoader::LoadLine(hFile) )
		{
			/*BYTE	bFirstChar = 0, bLastChar;

			while ( lineBuffer[bFirstChar] <= ' ' )
			{
				if ( !lineBuffer[bFirstChar] || lineBuffer[bFirstChar] == '\n' )
					break;
				++bFirstChar;
			}

			bLastChar = bFirstChar;

			while ( lineBuffer[bLastChar] && lineBuffer[bLastChar] != '\n' )
			{
				if ( lineBuffer[bLastChar] == ',' || lineBuffer[bLastChar] == '\r' )
					lineBuffer[bLastChar] = ' ';
				++bLastChar;
			}

			lineBuffer[bLastChar] = '\0';*/
			if ( pLine[0] && pLine[0] != '#' )
			{
				switch ( curFileSection )
				{
				case SECTION_NOTHING:
					{
						if ( SECTION3(pLine, 'c', 'o', 'l') )
							curFileSection = SECTION_RGB;
						else
						{
							if ( SECTION3(pLine, 'p', 'e', 'd') )
								curFileSection = SECTION_READPEDCOLS;
						}
						break;
					}

				case SECTION_RGB:
					{
						if ( SECTION3(pLine, 'e', 'n', 'd') )
							curFileSection = SECTION_NOTHING;
						else
						{
							DWORD	red, green, blue;
							sscanf(pLine, "%d %d %d", &red, &green, &blue);
							pPedCols->r = static_cast<BYTE>(red);
							pPedCols->g = static_cast<BYTE>(green);
							pPedCols->b = static_cast<BYTE>(blue);
							pPedCols->a = 255;
							++pPedCols;
						}
						break;
					}
				case SECTION_READPEDCOLS:
					{
						if ( SECTION3(pLine, 'e', 'n', 'd') )
							curFileSection = SECTION_NOTHING;
						else
						{
							char	modelName[24];
							DWORD	colours[NUM_POSSIBLE_COLOURS_FOR_PED][4];
							DWORD	valuesGet = sscanf(pLine, "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
														modelName,
														&colours[0][0], &colours[0][1], &colours[0][2], &colours[0][3],
														&colours[1][0], &colours[1][1], &colours[1][2], &colours[1][3],
														&colours[2][0], &colours[2][1], &colours[2][2], &colours[2][3],
														&colours[3][0], &colours[3][1], &colours[3][2], &colours[3][3],
														&colours[4][0], &colours[4][1], &colours[4][2], &colours[4][3],
														&colours[5][0], &colours[5][1], &colours[5][2], &colours[5][3],
														&colours[6][0], &colours[6][1], &colours[6][2], &colours[6][3],
														&colours[7][0], &colours[7][1], &colours[7][2], &colours[7][3],
														&colours[8][0], &colours[8][1], &colours[8][2], &colours[8][3],
														&colours[9][0], &colours[9][1], &colours[9][2], &colours[9][3],
														&colours[10][0], &colours[10][1], &colours[10][2], &colours[10][3],
														&colours[11][0], &colours[11][1], &colours[11][2], &colours[11][3],
														&colours[12][0], &colours[12][1], &colours[12][2], &colours[12][3],
														&colours[13][0], &colours[13][1], &colours[13][2], &colours[13][3],
														&colours[14][0], &colours[14][1], &colours[14][2], &colours[14][3],
														&colours[15][0], &colours[15][1], &colours[15][2], &colours[15][3]);
							CPedModelInfoVCS*	modelInfo = static_cast<CPedModelInfoVCS*>(CModelInfo::GetModelInfo(modelName, nullptr));
							modelInfo->m_bNumPossibleColours = static_cast<BYTE>(valuesGet - 1) / 4;
							for ( int i = 0; i < modelInfo->m_bNumPossibleColours; ++i )
							{
								modelInfo->m_primaryPedColours[i] = static_cast<BYTE>(colours[i][0]);
								modelInfo->m_secondaryPedColours[i] = static_cast<BYTE>(colours[i][1]);
								modelInfo->m_tertiaryPedColours[i] = static_cast<BYTE>(colours[i][2]);
								modelInfo->m_quaternaryPedColours[i] = static_cast<BYTE>(colours[i][3]);
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

void CPedModelInfoVCS::AssignColoursToModel(BYTE primaryColour, BYTE secondaryColour, BYTE tertiaryColour, BYTE quaternaryColour)
{
	bLastPedPrimaryColour = primaryColour;
	bLastPedSecondaryColour = secondaryColour;
	bLastPedTertiaryColour = tertiaryColour;
	bLastPedQuaternaryColour = quaternaryColour;
}

void CPedModelInfoVCS::SetEnvironmentMap(RpClump* clump)
{
	std::pair<void*,int>*	pData = materialRestoreData;
	RpClumpForAllAtomics(clump, SetEnvironmentMapCB, &pData);
	pData->first = nullptr;
}

RpAtomic* CPedModelInfoVCS::SetEnvironmentMapCB(RpAtomic* pAtomic, void* pData)
{
	if ( RpAtomicGetFlags(pAtomic) & rpATOMICRENDER )
		RpGeometryForAllMaterials(RpAtomicGetGeometry(pAtomic), SetEnvironmentMapCB, pData);
	return pAtomic;
}

RpMaterial* CPedModelInfoVCS::SetEnvironmentMapCB(RpMaterial* pMaterial, void* pData)
{
	int		colorIndexToUse;

	// TODO: Sort this thing
	switch ( *reinterpret_cast<int*>(&pMaterial->color) & 0xFFFFFF )
	{
	case 0x00FF3C:
		colorIndexToUse = bLastPedPrimaryColour;
		break;

	case 0xAF00FF:
		colorIndexToUse = bLastPedSecondaryColour;
		break;

	case 0xFFFF00:
		colorIndexToUse = bLastPedTertiaryColour;
		break;

	case 0xFF00FF:
		colorIndexToUse = bLastPedQuaternaryColour;
		break;

	default:
		return pMaterial;
	}
	std::pair<void*,int>**	pMaterialStack = reinterpret_cast<std::pair<void*,int>**>(pData);

	((*pMaterialStack)++)[0] = std::make_pair(&pMaterial->color, *reinterpret_cast<int*>(&pMaterial->color));

	pMaterial->color.red = ms_pedColourTable[colorIndexToUse].r;
	pMaterial->color.green = ms_pedColourTable[colorIndexToUse].g;
	pMaterial->color.blue = ms_pedColourTable[colorIndexToUse].b;

	return pMaterial;
}

void CPedModelInfoVCS::GetRandomPedColour(BYTE& colour1, BYTE& colour2, BYTE& colour3, BYTE& colour4)
{
	BYTE colours = m_bNumPossibleColours;
	if ( !colours )
	{
		colour1 = 1;
		colour2 = 1;
		colour3 = 1;
		colour4 = 1;
	}
	else
	{
		BYTE	nextColour = ( m_bLastColourChoice + 1 ) % colours;
		m_bLastColourChoice = nextColour;
		colour1 = m_primaryPedColours[nextColour];
		colour2 = m_secondaryPedColours[nextColour];
		colour3 = m_tertiaryPedColours[nextColour];
		colour4 = m_quaternaryPedColours[nextColour];
	}
}