#include "StdAfx.h"
#include "ModelInfo.h"

#include "Pools.h"
#include "TxdStore.h"

// Static variables
CBaseModelInfo**						CModelInfo::ms_modelInfoPtrs = (CBaseModelInfo**)0xA9B0C8;

CDynamicStore<CPedModelInfoVCS>			CModelInfo::ms_pedModelStore;
CDynamicStore<CAtomicModelInfo>			CModelInfo::ms_atomicModelStore;
CDynamicStore<CDamageAtomicModelInfo>	CModelInfo::ms_damageAtomicModelStore;
CDynamicStore<CTimeModelInfo>			CModelInfo::ms_timeModelStore;

CRGBA									CPedModelInfoVCS::ms_pedColourTable[NUM_PED_COLOURS];
BYTE									CPedModelInfoVCS::bLastPedPrimaryColour;
BYTE									CPedModelInfoVCS::bLastPedSecondaryColour;
BYTE									CPedModelInfoVCS::bLastPedTertiaryColour;
BYTE									CPedModelInfoVCS::bLastPedQuaternaryColour;

// Wrappers
WRAPPER CBaseModelInfo* CModelInfo::GetModelInfo(const char* pName, int* pOutID) { WRAPARG(pName); WRAPARG(pOutID); EAXJMP(0x4C5940); }

WRAPPER void CBaseModelInfo::Init() { EAXJMP(0x4C4B10); }
WRAPPER void CBaseModelInfo::Shutdown() { EAXJMP(0x4C4D50); }

WRAPPER void CClumpModelInfo::DeleteRwObject() { EAXJMP(0x4C4E70); }
WRAPPER RpAtomic* CClumpModelInfo::CreateInstance_(RwMatrix* pMatrix) { WRAPARG(pMatrix); EAXJMP(0x4C5110); }
WRAPPER RpAtomic* CClumpModelInfo::CreateInstance() { EAXJMP(0x4C5140); }
WRAPPER void CClumpModelInfo::SetAnimFile(const char* pName) { WRAPARG(pName); EAXJMP(0x4C5200); }
WRAPPER void CClumpModelInfo::ConvertAnimFileIndex() { EAXJMP(0x4C5250); }
WRAPPER void CClumpModelInfo::SetClump(RpClump* pClump) { WRAPARG(pClump); EAXJMP(0x4C4F70); }

WRAPPER void CAtomicModelInfo::DeleteRwObject() { EAXJMP(0x4C4440); }
WRAPPER RpAtomic* CAtomicModelInfo::CreateInstance_(RwMatrix* pMatrix) { WRAPARG(pMatrix); EAXJMP(0x4C44D0); }
WRAPPER RpAtomic* CAtomicModelInfo::CreateInstance() { EAXJMP(0x4C4530); }
WRAPPER void CAtomicModelInfo::SetAtomic(RpAtomic* pAtomic) { WRAPARG(pAtomic); EAXJMP(0x4C4360); }

WRAPPER RpAtomic* CDamageAtomicModelInfo::CreateInstance_(RwMatrix* pMatrix) { WRAPARG(pMatrix); EAXJMP(0x4C4910); }
WRAPPER RpAtomic* CDamageAtomicModelInfo::CreateInstance() { EAXJMP(0x4C4960); }

WRAPPER void CPedModelInfo::SetClump(RpClump* pClump) { WRAPARG(pClump); EAXJMP(0x4C7340); }

void CColModel::operator delete(void* mem)
{
	CPools::GetColModelPool()->Delete(reinterpret_cast<CColModel*>(mem));
}

bool CModelInfo::IsBikeModel(int modelID)
{
	// Temp
	CBaseModelInfo*		pModel = ms_modelInfoPtrs[modelID];
	return pModel && pModel->GetModelType() == 6 && *(DWORD*)((BYTE*)pModel+0x3C) == 9;
}

CPedModelInfoVCS* CModelInfo::AddPedModel(int nModelID)
{
	auto	pNewEntry = ms_pedModelStore.Add();
	pNewEntry->Init();
	ms_modelInfoPtrs[nModelID] = pNewEntry;
	return pNewEntry;
}

CAtomicModelInfo* CModelInfo::AddAtomicModel(int nModelID)
{
	auto	pNewEntry = ms_atomicModelStore.Add();
	pNewEntry->Init();
	ms_modelInfoPtrs[nModelID] = pNewEntry;
	return pNewEntry;
}

CDamageAtomicModelInfo* CModelInfo::AddDamageAtomicModel(int nModelID)
{
	auto	pNewEntry = ms_damageAtomicModelStore.Add();
	pNewEntry->Init();
	ms_modelInfoPtrs[nModelID] = pNewEntry;
	return pNewEntry;
}

CTimeModelInfo* CModelInfo::AddTimeModel(int nModelID)
{
	auto	pNewEntry = ms_timeModelStore.Add();
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
		// Atomic model store
		auto		pEntry = ms_atomicModelStore.GetFirstEntry();

		while ( pEntry )
		{
			pEntry->m_entry.Shutdown();
			pEntry = pEntry->m_pPrev;
		}
	}

	{
		// Damageatomic model store
		auto		pEntry = ms_damageAtomicModelStore.GetFirstEntry();

		while ( pEntry )
		{
			pEntry->m_entry.Shutdown();
			pEntry = pEntry->m_pPrev;
		}
	}
}

void CBaseModelInfo::SetTexDictionary(const char* pDict)
{
	int		nSlot = CTxdStore::FindTxdSlot(pDict);

	if ( nSlot == -1 )
		usTextureDictionary = CTxdStore::AddTxdSlot(pDict);
	else
		usTextureDictionary = nSlot;
}

void CBaseModelInfo::AddRef()
{
	++usNumberOfRefs;
	CTxdStore::AddRef(usTextureDictionary);
}

void CClumpModelInfo::Init()
{
	CBaseModelInfo::Init();
	nAnimIndex = -1;
}

void CAtomicModelInfo::Shutdown()
{
	delete pEmpireData;
	CBaseModelInfo::Shutdown();
}

void CDamageAtomicModelInfo::Init()
{
	CBaseModelInfo::Init();
	pAtomic2 = nullptr;
}

void CDamageAtomicModelInfo::DeleteRwObject()
{
	if ( pAtomic2 )
	{
		RpAtomicDestroy(pAtomic2);
		RwFrameDestroy(RpAtomicGetFrame(pAtomic2));
		pAtomic2 = nullptr;
	}
	CAtomicModelInfo::DeleteRwObject();
}

void CPedModelInfo::DeleteRwObject()
{
	CClumpModelInfo::DeleteRwObject();
	delete HitColModel;
	HitColModel = nullptr;
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

		while ( const char* pLine = CFileLoader::LoadLine(hFile) )
		{
			if ( pLine[0] && pLine[0] != '#' )
			{
				switch ( curFileSection )
				{
				case SECTION_NOTHING:
					{
						if ( SECTION3(pLine, 'c', 'o', 'l') )
							curFileSection = SECTION_RGB;
						else if ( SECTION3(pLine, 'p', 'e', 'd') )
							curFileSection = SECTION_READPEDCOLS;
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
							CPedModelInfoVCS*	modelInfo = static_cast<CPedModelInfoVCS*>(CModelInfo::GetModelInfo(modelName));
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

void CPedModelInfoVCS::SetPedColour(unsigned char primaryColour, unsigned char secondaryColour, unsigned char tertiaryColour, unsigned char quaternaryColour)
{
	bLastPedPrimaryColour = primaryColour;
	bLastPedSecondaryColour = secondaryColour;
	bLastPedTertiaryColour = tertiaryColour;
	bLastPedQuaternaryColour = quaternaryColour;
}

void CPedModelInfoVCS::SetEditableMaterials(RpClump* pClump)
{
	std::pair<void*,int>*	pData = materialRestoreData;
	RpClumpForAllAtomics(pClump, SetEditableMaterialsCB, &pData);
	pData->first = nullptr;
}

RpAtomic* CPedModelInfoVCS::SetEditableMaterialsCB(RpAtomic* pAtomic, void* pData)
{
	if ( RpAtomicGetFlags(pAtomic) & rpATOMICRENDER )
		RpGeometryForAllMaterials(RpAtomicGetGeometry(pAtomic), SetEditableMaterialsCB, pData);
	return pAtomic;
}

RpMaterial* CPedModelInfoVCS::SetEditableMaterialsCB(RpMaterial* pMaterial, void* pData)
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