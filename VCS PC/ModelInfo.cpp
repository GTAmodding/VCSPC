#include "StdAfx.h"
#include "ModelInfo.h"

#include "Pools.h"
#include "TxdStore.h"
#include "Streaming.h"

// Static variables
CColModel&								CTempColModels::ms_colModelWeapon = *(CColModel*)0x968FD0;

CBaseModelInfo** const					CModelInfo::ms_modelInfoPtrs = (CBaseModelInfo**)0xA9B0C8;

RwTexture*&								CVehicleModelInfo::ms_pRemapTexture = *(RwTexture**)0xB4E47C;
RwTexture*&								CVehicleModelInfo::ms_pLightsTexture = *(RwTexture**)0xB4E68C;
RwTexture*&								CVehicleModelInfo::ms_pLightsOnTexture = *(RwTexture**)0xB4E690;
bool* const								CVehicleModelInfo::ms_lightsOn = (bool*)0xB4E3E8;
unsigned char* const					CVehicleModelInfo::ms_currentCol = (unsigned char*)0xB4E3F0;
CRGBA* const							CVehicleModelInfo::ms_vehicleColourTable = (CRGBA*)0xB4E480;
const char*&							CVehicleModelInfo::ms_pCurrentCarcolsFile = *(const char**)0x5B68A1;

CDynamicStore<CVehicleModelInfoVCS>		CModelInfo::ms_vehicleModelStore;
CDynamicStore<CPedModelInfoVCS>			CModelInfo::ms_pedModelStore;
CDynamicStore<CAtomicModelInfo>			CModelInfo::ms_atomicModelStore;
CDynamicStore<CDamageAtomicModelInfo>	CModelInfo::ms_damageAtomicModelStore;
CDynamicStore<CTimeModelInfo>			CModelInfo::ms_timeModelStore;
CModelInfoDrawDistStorage				CModelInfo::ms_drawDistStorage;

CRGBA									CPedModelInfoVCS::ms_pedColourTable[NUM_PED_COLOURS];
unsigned char							CPedModelInfoVCS::ms_currentCol[4];

float&									ms_lodDistScale = *(float*)0x8CD800;

// Wrappers
WRAPPER void CModelInfo::Initialise(void) { EAXJMP(0x4C6810); }
WRAPPER CBaseModelInfo* CModelInfo::GetModelInfo(const char* pName, int* pOutID) { WRAPARG(pName); WRAPARG(pOutID); EAXJMP(0x4C5940); }
WRAPPER CWeaponModelInfo* CModelInfo::AddWeaponModel(int nModelID) { WRAPARG(nModelID); EAXJMP(0x4C6710); }

WRAPPER void CBaseModelInfo::Init() { EAXJMP(0x4C4B10); }
WRAPPER void CBaseModelInfo::Shutdown() { EAXJMP(0x4C4D50); }
WRAPPER void CBaseModelInfo::SetColModel(CColModel* pModel, bool bInitPaired) { WRAPARG(pModel); WRAPARG(bInitPaired);  EAXJMP(0x4C4BC0); }

WRAPPER void CClumpModelInfo::DeleteRwObject() { EAXJMP(0x4C4E70); }
WRAPPER RwObject* CClumpModelInfo::CreateInstance_(RwMatrix* pMatrix) { WRAPARG(pMatrix); EAXJMP(0x4C5110); }
WRAPPER RwObject* CClumpModelInfo::CreateInstance() { EAXJMP(0x4C5140); }
WRAPPER void CClumpModelInfo::SetAnimFile(const char* pName) { WRAPARG(pName); EAXJMP(0x4C5200); }
WRAPPER void CClumpModelInfo::ConvertAnimFileIndex() { EAXJMP(0x4C5250); }
WRAPPER void CClumpModelInfo::SetClump(RpClump* pClump) { WRAPARG(pClump); EAXJMP(0x4C4F70); }

WRAPPER void CAtomicModelInfo::DeleteRwObject() { EAXJMP(0x4C4440); }
WRAPPER RwObject* CAtomicModelInfo::CreateInstance_(RwMatrix* pMatrix) { WRAPARG(pMatrix); EAXJMP(0x4C44D0); }
WRAPPER RwObject* CAtomicModelInfo::CreateInstance() { EAXJMP(0x4C4530); }
WRAPPER void CAtomicModelInfo::SetAtomic(RpAtomic* pAtomic) { WRAPARG(pAtomic); EAXJMP(0x4C4360); }

WRAPPER RwObject* CDamageAtomicModelInfo::CreateInstance_(RwMatrix* pMatrix) { WRAPARG(pMatrix); EAXJMP(0x4C4910); }
WRAPPER RwObject* CDamageAtomicModelInfo::CreateInstance() { EAXJMP(0x4C4960); }

WRAPPER void CPedModelInfo::SetClump(RpClump* pClump) { WRAPARG(pClump); EAXJMP(0x4C7340); }

WRAPPER void CVehicleModelInfo::DeleteRwObject() { EAXJMP(0x4C9890); }
WRAPPER RwObject* CVehicleModelInfo::CreateInstance() { EAXJMP(0x4C9680); }
WRAPPER void CVehicleModelInfo::SetAnimFile(const char* pName) { WRAPARG(pName); EAXJMP(0x4C7670); }
WRAPPER void CVehicleModelInfo::ConvertAnimFileIndex() { EAXJMP(0x4C76D0); }
WRAPPER void CVehicleModelInfo::SetClump(RpClump* pClump) { WRAPARG(pClump); EAXJMP(0x4C95C0); }

WRAPPER RpAtomic* CVehicleModelInfo::SetEditableMaterialsCB(RpAtomic* pMaterial, void* pData) { WRAPARG(pMaterial); WRAPARG(pData); EAXJMP(0x4C83E0); }
WRAPPER void CVehicleModelInfo::LoadVehicleColours() { EAXJMP(0x5B6890); }

static void TXDLoadOverride(signed int nIndex, const char* pName)
{
	UNREFERENCED_PARAMETER(pName);

	CStreaming::RequestModel(nIndex + 20000, 26);
	CStreaming::LoadAllRequestedModels(true);
}

void CColModel::operator delete(void* ptr)
{
	CPools::GetColModelPool()->Delete(static_cast<CColModel*>(ptr));
}

void CModelInfoDrawDistStorage::AddToList(int nIndex, CBaseModelInfo* pModelInfo)
{
	if ( !m_nMin && !m_nMax )
		m_nMin = m_nMax = nIndex;
	else
	{
		if ( m_nMin > nIndex )
			m_nMin = nIndex;
		else if ( nIndex >= m_nMax )
			m_nMax = nIndex + 1;
	}
	
	m_pMalloc[nIndex] = Pack(pModelInfo->fLodDistanceUnscaled);

	// Recalculate
	pModelInfo->RecalcDrawDistance(pModelInfo->fLodDistanceUnscaled);
}

void CModelInfoDrawDistStorage::RecalcDrawDistances()
{
	for ( int i = m_nMin; i < m_nMax; ++i )
	{
		if ( CModelInfo::ms_modelInfoPtrs[i] )
		{
			unsigned char		nModelType = CModelInfo::ms_modelInfoPtrs[i]->GetModelType();
			if ( nModelType == 1 || nModelType == 3 || nModelType == 5 || nModelType == 8 )
				CModelInfo::ms_modelInfoPtrs[i]->RecalcDrawDistance(Unpack(m_pMalloc[i-m_nMin]));
		}
	}
}

bool CModelInfo::IsBikeModel(int modelID)
{
	// Temp
	CBaseModelInfo*		pModel = ms_modelInfoPtrs[modelID];
	return pModel && pModel->GetModelType() == 6 && *(DWORD*)((BYTE*)pModel+0x3C) == 9;
}

CVehicleModelInfoVCS* CModelInfo::AddVehicleModel(int nModelID)
{
	auto	pNewEntry = ms_vehicleModelStore.Add();
	pNewEntry->Init();
	ms_modelInfoPtrs[nModelID] = pNewEntry;
	return pNewEntry;
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

	ms_drawDistStorage.Shutdown();
}

void CModelInfo::Inject()
{
	Memory::InjectHook(0x5B8F17, CVehicleModelInfo::LoadAllVehicleColours);

	// GTA IV vehshare
	Memory::Patch<const char*>(0x5B6F3D, "vehshare");
	Memory::Patch<const char*>(0x5B6F58, "vehshare");
	Memory::Patch<const char*>(0x5B8F36, "vehshare");
	Memory::Patch<const char*>(0x5B8F4A, "vehshare");
	Memory::Patch<const char*>(0x5D5BC8, "vehshare");
	Memory::Patch<const char*>(0x6FD503, "vehshare");
	Memory::Patch<const char*>(0x6FD781, "vehshare");
	Memory::InjectHook(0x5B8F5E, TXDLoadOverride);
	Memory::Patch<BYTE>(0x4C75A0, 0xC3);
	Memory::Patch<BYTE>(0x4C75C0, 0xC3);

	// Fixed vehicle editable materials (by _DK)
	Memory::Patch<const void*>(0x4C8415, static_cast<RpMaterial*(*)(RpMaterial*, void*)>(CVehicleModelInfo::SetEditableMaterialsCB));

	Memory::Patch<BYTE>(0x84BCF0, 0xC3);
	Memory::Patch<BYTE>(0x8562B0, 0xC3);

	Memory::Patch<WORD>(0x4C6517, 0x22EB);
	Memory::Nop(0x4C6857, 7);

	Memory::InjectHook(0x5B6FD1, AddVehicleModel);
	Memory::InjectHook(0x5B74A7, AddPedModel);
	Memory::InjectHook(0x5B3F32, AddTimeModel);

	Memory::Patch<BYTE>(0x4C491B, offsetof(CDamageAtomicModelInfo, pAtomic2));
	Memory::Patch<BYTE>(0x4C496E, offsetof(CDamageAtomicModelInfo, pAtomic2));
	Memory::Patch<BYTE>(0x4C48D8, offsetof(CDamageAtomicModelInfo, pAtomic2));

	/*patch(0x4C640B, &CModelInfo::ms_damageAtomicModelStore.m_NumObjects, 4);
	patch(0x4C6428, &CModelInfo::ms_damageAtomicModelStore.m_NumObjects, 4);
	patch(0x4C6651, &CModelInfo::ms_damageAtomicModelStore.m_NumObjects, 4);
	patch(0x4C6662, &CModelInfo::ms_damageAtomicModelStore.m_NumObjects, 4);
	patch(0x4C682F, &CModelInfo::ms_damageAtomicModelStore.m_NumObjects, 4);
	//patch(0x84BC11, &CModelInfo::ms_damageAtomicModelStore, 4);
	//patch(c, &CModelInfo::ms_damageAtomicModelStore, 4);

	patch(0x4C6416, &CModelInfo::ms_damageAtomicModelStore.m_Objects, 4);
	patch(0x4C665D, &CModelInfo::ms_damageAtomicModelStore.m_Objects, 4);

	//Patch<BYTE>(0x4C5D1C, NUM_CLUMP_MODELS);
	//Patch<BYTE>(0x4C5866, NUM_CLUMP_MODELS);*/

	Memory::Patch<BYTE>(0x84BC10, 0xC3);
	Memory::Patch<BYTE>(0x856240, 0xC3);

	Memory::Patch<WORD>(0x4C64C8, 0x22EB);
	Memory::Nop(0x4C684B, 7);

	Memory::InjectHook(0x4C6616, CModelInfo::ShutDown, PATCH_JUMP);
}

void CBaseModelInfo::SetTexDictionary(const char* pDict)
{
	m_texDict = CTxdStore::GetTxdSlot(pDict);
}

void CBaseModelInfo::AddRef()
{
	++m_numRefs;
	CTxdStore::AddRef(m_texDict);
}

void CBaseModelInfo::RecalcDrawDistance(float fOldDist)
{
	fLodDistanceUnscaled = Min(fOldDist, 1371.5f * ms_lodDistScale - 668.5f);
}

void CClumpModelInfo::Init()
{
	CBaseModelInfo::Init();
	m_nAnimIndex = -1;
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

void CVehicleModelInfo::Init()
{
	CClumpModelInfo::Init();
	m_dwType = -1;
	m_wWheelModelId = -1;
	m_fBikeSteerAngle = 999.99f;
}

void CVehicleModelInfo::SetVehicleColour(unsigned char primaryColour, unsigned char secondaryColour, unsigned char tertiaryColour, unsigned char quaternaryColour)
{
	ms_currentCol[0] = primaryColour;
	ms_currentCol[1] = secondaryColour;
	ms_currentCol[2] = tertiaryColour;
	ms_currentCol[3] = quaternaryColour;
}

RpMaterial* CVehicleModelInfo::SetEditableMaterialsCB(RpMaterial* pMaterial, void* pData)
{
	std::pair<void*,int>**	pMaterialStack = reinterpret_cast<std::pair<void*,int>**>(pData);

	if ( RpMaterialGetTexture(pMaterial) )
	{
		if ( ms_pRemapTexture && RwTextureGetName(RpMaterialGetTexture(pMaterial))[0] == '#' )
		{
			*((*pMaterialStack)++) = std::make_pair(&pMaterial->texture, *reinterpret_cast<int*>(&pMaterial->texture));
			pMaterial->texture = ms_pRemapTexture;
			return pMaterial;
		}
		else if ( RpMaterialGetTexture(pMaterial) == ms_pLightsTexture )
		{
			int		nLightToChoose;
			switch ( RwRGBAGetRGB(pMaterial->color) )
			{
			case 0x00AFFF:
				nLightToChoose = 0;
				break;
			  case 0xC8FF00:
				nLightToChoose = 1;
				break;
			  case 0x00FFB9:
				nLightToChoose = 2;
				break;
			  case 0x003CFF:
				nLightToChoose = 3;
				break;
			  default:
				nLightToChoose = -1;
				break;
			}
			*((*pMaterialStack)++) = std::make_pair(&pMaterial->color, *reinterpret_cast<int*>(&pMaterial->color));

			pMaterial->color.red = 0xFF;
			pMaterial->color.green = 0xFF;
			pMaterial->color.blue = 0xFF;

			if ( nLightToChoose != -1 )
			{
				if ( ms_lightsOn[nLightToChoose] )
				{
					const RwSurfaceProperties		surfProps = { 16.0f, 0.0, 0.0 };

					*((*pMaterialStack)++) = std::make_pair(&pMaterial->texture, *reinterpret_cast<int*>(&pMaterial->texture));

					*((*pMaterialStack)++) = std::make_pair(&pMaterial->surfaceProps.ambient, *reinterpret_cast<int*>(&pMaterial->surfaceProps.ambient));
					*((*pMaterialStack)++) = std::make_pair(&pMaterial->surfaceProps.diffuse, *reinterpret_cast<int*>(&pMaterial->surfaceProps.diffuse));
					*((*pMaterialStack)++) = std::make_pair(&pMaterial->surfaceProps.specular, *reinterpret_cast<int*>(&pMaterial->surfaceProps.specular));
				
					pMaterial->texture = ms_pLightsOnTexture;
					pMaterial->surfaceProps = surfProps;
				}
			}
			return pMaterial;
		}
	}

	int		nColourToPaint;
	switch ( RwRGBAGetRGB(pMaterial->color) )
    {
      case 0x00FF3C:
        nColourToPaint = ms_currentCol[0];
        break;
      case 0xAF00FF:
        nColourToPaint = ms_currentCol[1];
        break;
      case 0xFFFF00:
        nColourToPaint = ms_currentCol[2];
        break;
	  case 0xFF00FF:
        nColourToPaint = ms_currentCol[3];
        break;
	  default:
		return pMaterial;
    }

	*((*pMaterialStack)++) = std::make_pair(&pMaterial->color, *reinterpret_cast<int*>(&pMaterial->color));

	pMaterial->color.red = ms_vehicleColourTable[nColourToPaint].r;
	pMaterial->color.green = ms_vehicleColourTable[nColourToPaint].g;
	pMaterial->color.blue = ms_vehicleColourTable[nColourToPaint].b;

	return pMaterial;
}

void CVehicleModelInfo::LoadAllVehicleColours()
{
	LoadVehicleColours();

	if ( auto* pColours = CFileLoader::GetCarcolsList() )
	{
		for ( auto it = pColours->cbegin(); it != pColours->cend(); it++ )
		{
			ms_pCurrentCarcolsFile = it->c_str();
			LoadVehicleColours();
		}
	}
}

void CVehicleModelInfo::ResetEditableMaterials()
{
	for ( auto* i = materialRestoreData; i->first; i++ )
		*static_cast<int*>(i->first) = i->second;
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
	ms_currentCol[0] = primaryColour;
	ms_currentCol[1] = secondaryColour;
	ms_currentCol[2] = tertiaryColour;
	ms_currentCol[3] = quaternaryColour;
}

void CPedModelInfoVCS::SetEditableMaterials(RpClump* pClump)
{
	auto*	pData = materialRestoreData;

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
	int		nColourToPaint;

	switch ( RwRGBAGetRGB(pMaterial->color) )
	{
	case 0x00FF3C:
		nColourToPaint = ms_currentCol[0];
		break;
	case 0xAF00FF:
		nColourToPaint = ms_currentCol[1];
		break;
	case 0xFFFF00:
		nColourToPaint = ms_currentCol[2];
		break;
	case 0xFF00FF:
		nColourToPaint = ms_currentCol[3];
		break;
	default:
		return pMaterial;
	}
	std::pair<void*,int>**	pMaterialStack = reinterpret_cast<std::pair<void*,int>**>(pData);

	*((*pMaterialStack)++) = std::make_pair(&pMaterial->color, *reinterpret_cast<int*>(&pMaterial->color));

	pMaterial->color.red = ms_pedColourTable[nColourToPaint].r;
	pMaterial->color.green = ms_pedColourTable[nColourToPaint].g;
	pMaterial->color.blue = ms_pedColourTable[nColourToPaint].b;

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