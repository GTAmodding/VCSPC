#include "StdAfx.h"
#include "Project2dfx.h"

#include "Coronas.h"
#include "ModelInfo.h"
#include "Pools.h"
#include "Camera.h"
#include "Clock.h"

std::vector<const CLamppostInfo>*				CProject2dfx::m_pLampposts = nullptr;
std::map<unsigned int, const CLamppostInfo>*	CProject2dfx::m_pFileContent;
bool											CProject2dfx::m_bCatchLamppostsNow = false;

static inline unsigned int PackKey(unsigned short nModel, unsigned short nIndex)
{
	return nModel << 16 | nIndex;
}

static void __declspec(naked) LoadObjectInstHook()
{
	_asm
	{
		push		eax
		call		CProject2dfx::PossiblyAddThisEntity
		add			esp, 34h
		retn
	}
}

bool CProject2dfx::IsModelALamppost(unsigned short nModel)
{
	auto	it = m_pFileContent->lower_bound(PackKey(nModel, 0));
	return it != m_pFileContent->end() && it->first >> 16 == nModel;
}

void CProject2dfx::Inject()
{
	Memory::InjectHook(0x53C131, Render);
	Memory::InjectHook(0x538439, LoadObjectInstHook, PATCH_JUMP);
}

void CProject2dfx::RegisterLamppost(CEntity* pObj)
{
	unsigned short		nModelID = pObj->GetModelIndex();
	CMatrix				dummyMatrix;
	CSimpleTransform&	objTransform = pObj->GetTransform();

	dummyMatrix.SetTranslateOnly(objTransform.m_translate.x, objTransform.m_translate.y, objTransform.m_translate.z);
	dummyMatrix.SetRotateZOnly(objTransform.m_heading);

	auto	itEnd = m_pFileContent->upper_bound(PackKey(nModelID, 0xFFFF));
	for ( auto it = m_pFileContent->lower_bound(PackKey(nModelID, 0)); it != itEnd; it++ )
		m_pLampposts->push_back(CLamppostInfo(dummyMatrix * it->second.vecPos, it->second.colour, it->second.fCustomSizeMult));
}

void CProject2dfx::Init()
{
	// Load p2dfx.dat
	if ( FILE* hFile = CFileMgr::OpenFile(CFileLoader::GetP2dfxPath(), "r") )
	{
		unsigned short	nModel = 0xFFFF, nCurIndexForModel = 0;
		m_pFileContent = new std::map<unsigned int, const CLamppostInfo>;

		while ( const char* pLine = CFileLoader::LoadLine(hFile) )
		{
			if ( pLine[0] && pLine[0] != '#' )
			{
				if ( pLine[0] == '%' )
				{
					nCurIndexForModel = 0;
					nModel = CModelInfo::GetModelInfoUInt16(pLine+2);
				}
				else
				{
					float			fOffsetX, fOffsetY, fOffsetZ;
					unsigned int	nRed, nGreen, nBlue;
					float			fCustomSize = 1.0f;

					sscanf(pLine, "%f %f %f %d %d %d %f", &fOffsetX, &fOffsetY, &fOffsetZ, &nRed, &nGreen, &nBlue, &fCustomSize);
					m_pFileContent->insert(std::make_pair(PackKey(nModel, nCurIndexForModel++), CLamppostInfo(CVector(fOffsetX, fOffsetY, fOffsetZ), CRGBA(static_cast<unsigned char>(nRed), static_cast<unsigned char>(nGreen), static_cast<unsigned char>(nBlue)), fCustomSize)));
				}

			}
		}

		m_pLampposts = new std::vector<const CLamppostInfo>;
		m_bCatchLamppostsNow = true;

		CFileMgr::CloseFile(hFile);
	}
}

CEntity* CProject2dfx::PossiblyAddThisEntity(CEntity* pEntity)
{
	if ( m_bCatchLamppostsNow && IsModelALamppost(pEntity->GetModelIndex()) )
		RegisterLamppost(pEntity);

	// Saves some hacking
	return pEntity;
}

void CProject2dfx::Shutdown()
{
	delete m_pLampposts;
}

void CProject2dfx::Render()
{
	if ( CClock::GetIsTimeInRange(20, 7) )
	{
		unsigned char	bAlpha;
		unsigned int	nTime = CClock::GetHours() * 60 + CClock::GetMinutes();

		if ( nTime >= 20 * 60 )
			bAlpha = static_cast<unsigned char>((5.0f/6.0f)*nTime - 1000.0f);
		else if ( nTime < 3 * 60 )
			bAlpha = 200;
		else
			bAlpha = static_cast<unsigned char>((-5.0f/6.0f)*nTime + 350.0f);

		for ( auto it = m_pLampposts->cbegin(); it != m_pLampposts->cend(); it++ )
		{
			if ( TheCamera.IsPositionVisible(it->vecPos, 5.0f) )
			{
				CVector*	pCamPos = &TheCamera.Cams[TheCamera.ActiveCam].Source;
				float		fDistSqr = (pCamPos->x - it->vecPos.x)*(pCamPos->x - it->vecPos.x) + (pCamPos->y - it->vecPos.y)*(pCamPos->y - it->vecPos.y) + (pCamPos->z - it->vecPos.z)*(pCamPos->z - it->vecPos.z);

				if ( fDistSqr > 280.0f*280.0f && fDistSqr < CORONAS_FARCLIP*CORONAS_FARCLIP )
				{
					float	fRadius = Min((fDistSqr < 290.0f*290.0f) ? (1.0f/ 4.0f)*sqrt(fDistSqr) - 70.0f : (3.0f/164.0f)*sqrt(fDistSqr) - (5.0f/41.0f), 20.0f);

					CCoronas::RegisterCorona(reinterpret_cast<unsigned int>(&*it), nullptr, it->colour.r, it->colour.g, it->colour.b, bAlpha, it->vecPos, fRadius * it->fCustomSizeMult, CORONAS_FARCLIP, gpCoronaTexture[1], 0, 0, 0, 0, 0.0f, false, 1.5f, false, bAlpha, false, true);
				}
			}
		}
	}
}