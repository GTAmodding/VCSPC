#include "StdAfx.h"
#include "Project2dfx.h"

#include "Coronas.h"
#include "ModelInfo.h"
#include "Pools.h"
#include "CCamera.h"
#include "Clock.h"

// Model IDs
static unsigned short	miBouy = 0xFFFF, miDoublestreetlght1 = 0xFFFF, miHigh_lampost = 0xFFFF, miLampost_coast = 0xFFFF, miLamppost1 = 0xFFFF, miLamproad3 = 0xFFFF, miMlamppost = 0xFFFF;
static unsigned short	miStreetlamp1 = 0xFFFF, miBollardlight = 0xFFFF, miLamppost2 = 0xFFFF, miStreetlamp2 = 0xFFFF, miDockslights = 0xFFFF;

std::vector<CLamppostInfo>*	CProject2dfx::m_pLampposts = nullptr;
bool						CProject2dfx::m_bCatchLamppostsNow = false;

static bool IsModelALamppost(unsigned short nModel)
{
	return nModel == miBouy || nModel == miDoublestreetlght1 || nModel == miHigh_lampost || nModel == miLampost_coast
		|| nModel == miLamppost1 || nModel == miLamproad3 || nModel == miMlamppost || nModel == miStreetlamp1 || nModel == miBollardlight
		|| nModel == miLamppost2 || nModel == miStreetlamp2 || nModel == miDockslights;
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

void CProject2dfx::Inject()
{
	Memory::InjectHook(0x53C131, Render);
	Memory::InjectHook(0x538439, LoadObjectInstHook, PATCH_JUMP);
}

void CProject2dfx::RegisterLamppost(CEntity* pObj)
{
	// Remember to change these manually when expanding
	unsigned short		nModelID = pObj->GetModelIndex();

	CVector				vecOffset[9];
	CRGBA				coronaColour[9];
	unsigned char		nNumCoronas = 1;	// Well, most of these have 1
	float				fCustomSize = 1.0f;	// Again, most are unaffected

	if ( nModelID == miBouy )
	{
		coronaColour[0].r = 255;
		coronaColour[0].g = 0;
		coronaColour[0].b = 0;

		vecOffset[0].x = -0.0095f;
		vecOffset[0].y = -0.0f;
		vecOffset[0].z = 11.915f;
	}
	else if ( nModelID == miDoublestreetlght1 )
	{
		coronaColour[0].r = 167;
		coronaColour[0].g = 255;
		coronaColour[0].b = 242;

		vecOffset[0].x = 2.725f;
		vecOffset[0].y = 1.0375f;
		vecOffset[0].z = 4.81f;
	}
	else if ( nModelID == miHigh_lampost )
	{
		coronaColour[0].r = 255;
		coronaColour[0].g = 255;
		coronaColour[0].b = 255;

		coronaColour[1].r = 255;
		coronaColour[1].g = 255;
		coronaColour[1].b = 255;


		vecOffset[0].x = -2.0f;
		vecOffset[0].y = 0.109f;
		vecOffset[0].z = 13.89f;

		vecOffset[1].x = 2.0f;
		vecOffset[1].y = 0.109f;
		vecOffset[1].z = 13.89f;

		nNumCoronas = 2;
		fCustomSize = 0.9f;
	}
	else if ( nModelID == miLampost_coast )
	{
		coronaColour[0].r = 255;
		coronaColour[0].g = 255;
		coronaColour[0].b = 255;

		vecOffset[0].x = 0.645f;
		vecOffset[0].y = 0.0f;
		vecOffset[0].z = 4.42f;
	}
	else if ( nModelID == miLamppost1 )
	{
		coronaColour[0].r = 255;
		coronaColour[0].g = 102;
		coronaColour[0].b = 0;

		vecOffset[0].x = -0.461f;
		vecOffset[0].y = 0.009f;
		vecOffset[0].z = 3.18f;
	}
	else if ( nModelID == miLamproad3 )
	{
		coronaColour[0].r = 255;
		coronaColour[0].g = 148;
		coronaColour[0].b = 52;

		vecOffset[0].x = -1.39f;
		vecOffset[0].y = 0.22f;
		vecOffset[0].z = 3.67f;
	}
	else if ( nModelID == miMlamppost )
	{
		coronaColour[0].r = 255;
		coronaColour[0].g = 148;
		coronaColour[0].b = 52;

		vecOffset[0].x = -1.4f;
		vecOffset[0].y = 0.01f;
		vecOffset[0].z = 4.1f;
	}
	else if ( nModelID == miStreetlamp1 )
	{
		coronaColour[0].r = 255;
		coronaColour[0].g = 255;
		coronaColour[0].b = 255;

		vecOffset[0].x = -0.027f;
		vecOffset[0].y = 0.025f;
		vecOffset[0].z = 2.74f;
	}
	else if ( nModelID == miBollardlight )
	{
		coronaColour[0].r = 255;
		coronaColour[0].g = 255;
		coronaColour[0].b = 255;

		vecOffset[0].x = -0.01f;
		vecOffset[0].y = -0.047f;
		vecOffset[0].z = 0.325f;
	}
	else if ( nModelID == miLamppost2 )
	{
		coronaColour[0].r = 255;
		coronaColour[0].g = 98;
		coronaColour[0].b = 12;

		vecOffset[0].x = 2.7f;
		vecOffset[0].y = 0.048f;
		vecOffset[0].z = 5.51f;
	}
	else if ( nModelID == miStreetlamp2 )
	{
		coronaColour[0].r = 255;
		coronaColour[0].g = 255;
		coronaColour[0].b = 255;

		vecOffset[0].x = -0.334f;
		vecOffset[0].y = 0.025f;
		vecOffset[0].z = 2.65f;
	}
	else if ( nModelID == miDockslights )
	{
		coronaColour[0].r = 253;
		coronaColour[0].g = 179;
		coronaColour[0].b = 50;

		coronaColour[1].r = 253;
		coronaColour[1].g = 179;
		coronaColour[1].b = 50;

		coronaColour[2].r = 54;
		coronaColour[2].g = 255;
		coronaColour[2].b = 87;

		coronaColour[3].r = 101;
		coronaColour[3].g = 255;
		coronaColour[3].b = 54;

		coronaColour[4].r = 255;
		coronaColour[4].g = 234;
		coronaColour[4].b = 0;

		coronaColour[5].r = 255;
		coronaColour[5].g = 234;
		coronaColour[5].b = 0;

		coronaColour[6].r = 12;
		coronaColour[6].g = 0;
		coronaColour[6].b = 255;

		coronaColour[7].r = 255;
		coronaColour[7].g = 0;
		coronaColour[7].b = 0;

		coronaColour[8].r = 255;
		coronaColour[8].g = 0;
		coronaColour[8].b = 246;


		vecOffset[0].x = -0.0164f;
		vecOffset[0].y = 1.7396f;
		vecOffset[0].z = 0.0048f;

		vecOffset[1].x = -0.052f;
		vecOffset[1].y = -0.5327f;
		vecOffset[1].z = 0.0048f;

		vecOffset[2].x = -0.0265f;
		vecOffset[2].y = 1.2247f;
		vecOffset[2].z = 0.0048f;

		vecOffset[3].x = -0.0593f;
		vecOffset[3].y = -0.9261f;
		vecOffset[3].z = 0.0048f;

		vecOffset[4].x = -0.0679f;
		vecOffset[4].y = -1.3319f;
		vecOffset[4].z = 0.0048f;

		vecOffset[5].x = -0.0372f;
		vecOffset[5].y = 0.3459f;
		vecOffset[5].z = 0.0048f;

		vecOffset[6].x = -0.0373f;
		vecOffset[6].y = 0.7619f;
		vecOffset[6].z = 0.0048f;

		vecOffset[7].x = -0.0396f;
		vecOffset[7].y = -0.0735f;
		vecOffset[7].z = 0.0048f;

		vecOffset[8].x = -0.0686f;
		vecOffset[8].y = -1.736f;
		vecOffset[8].z = 0.0048f;

		nNumCoronas = 9;
		fCustomSize = 0.75f;
	}

	CMatrix				dummyMatrix;
	CSimpleTransform&	objTransform = pObj->GetTransform();

	for ( int i = 0; i < nNumCoronas; ++i )
	{
		dummyMatrix.SetTranslateOnly(objTransform.m_translate.x, objTransform.m_translate.y, objTransform.m_translate.z);
		dummyMatrix.SetRotateZOnly(objTransform.m_heading);

		m_pLampposts->push_back(CLamppostInfo(dummyMatrix * vecOffset[i], coronaColour[i], fCustomSize));
	}
}

// Now unused
void CProject2dfx::EnumerateLampposts()
{
	{
		// Iterate through buildings pool
		CBuildingPool*		pPool = CPools::GetBuildingPool();
		CBuilding*			pObjs = pPool->GetSlots();

		for ( int i = 0; i < pPool->GetSize(); ++i, ++pObjs )
		{
			if ( !pPool->IsFree(i) )
			{
				if ( IsModelALamppost(pObjs->GetModelIndex()) )
					RegisterLamppost(pObjs);
			}
		}
	}

	{
		// Iterate through dummies pool
		CDummyPool*			pPool = CPools::GetDummyPool();
		CDummy*				pObjs = pPool->GetSlots();

		for ( int i = 0; i < pPool->GetSize(); ++i, ++pObjs )
		{
			if ( !pPool->IsFree(i) )
			{
				if ( IsModelALamppost(pObjs->GetModelIndex()) )
					RegisterLamppost(pObjs);
			}
		}
	}

	m_pLampposts->shrink_to_fit();
}

void CProject2dfx::Init()
{
	miBouy = CModelInfo::GetModelInfoUInt16("bouy");
	miDoublestreetlght1 = CModelInfo::GetModelInfoUInt16("doublestreetlght1");
	miHigh_lampost = CModelInfo::GetModelInfoUInt16("high_lampost");
	miLampost_coast = CModelInfo::GetModelInfoUInt16("lampost_coast");
	miLamppost1 = CModelInfo::GetModelInfoUInt16("lamppost1");
	miLamproad3 = CModelInfo::GetModelInfoUInt16("lamproad3");
	miMlamppost = CModelInfo::GetModelInfoUInt16("mlamppost");
	miStreetlamp1 = CModelInfo::GetModelInfoUInt16("Streetlamp1");
	miBollardlight = CModelInfo::GetModelInfoUInt16("bollardlight");
	miLamppost2 = CModelInfo::GetModelInfoUInt16("lamppost2");
	miStreetlamp2 = CModelInfo::GetModelInfoUInt16("Streetlamp2");
	miDockslights = CModelInfo::GetModelInfoUInt16("docks_lights");

	m_pLampposts = new std::vector<CLamppostInfo>;
	m_bCatchLamppostsNow = true;

	//EnumerateLampposts();
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
					float	fRadius = min((fDistSqr < 290.0f*290.0f) ? (1.0f/ 4.0f)*sqrt(fDistSqr) - 70.0f : (3.0f/164.0f)*sqrt(fDistSqr) - (5.0f/41.0f), 20.0f);

					CCoronas::RegisterCorona(reinterpret_cast<unsigned int>(&*it), nullptr, it->colour.r, it->colour.g, it->colour.b, bAlpha, it->vecPos, fRadius * it->fCustomSizeMult, CORONAS_FARCLIP, gpCoronaTexture[1], 0, 0, 0, 0, 0.0f, false, 1.5f, false, bAlpha, false, true);
				}
			}
		}
	}
}