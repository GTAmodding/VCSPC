#include "StdAfx.h"

std::vector<CRegisteredCorona>	CCoronas::aCoronas;
bool							CCoronas::bRemakeReferences;
int&							CCoronas::NumCoronas = *(int*)0xC3E038;
int&							CCoronas::bChangeBrightnessImmediately = *(int*)0xC3E034;
float&							CCoronas::ScreenMult = *(float*)0x8D4B5C;

WRAPPER void CRegisteredCorona::Update() { EAXJMP(0x6FABF0); }

void CCoronas::RegisterCorona(unsigned int nID, CEntity* pAttachTo, unsigned char R, unsigned char G, unsigned char B, unsigned char A, const CVector& Position, float Size, float Range, RwTexture* pTex, unsigned char flareType, unsigned char reflectionType, unsigned char LOSCheck, unsigned char unused, float normalAngle, bool bNeonFade, float PullTowardsCam, bool bFadeIntensity, float FadeSpeed, bool bOnlyFromBelow, bool bWhiteCore)
{
	UNREFERENCED_PARAMETER(unused);

	CVector		vecPosToCheck;
	if ( pAttachTo )
	{
		// TODO: AllocateMatrix
		vecPosToCheck = *pAttachTo->GetMatrix() * Position;
	}
	else
		vecPosToCheck = Position;

	CVector*	pCamPos = TheCamera.GetCoords();
	if ( Range * Range >= (pCamPos->x - vecPosToCheck.x)*(pCamPos->x - vecPosToCheck.x) + (pCamPos->y - vecPosToCheck.y)*(pCamPos->y - vecPosToCheck.y) )
	{
		if ( bNeonFade )
		{
			float		fDistFromCam = CVector(*pCamPos - vecPosToCheck).Magnitude();

			if ( fDistFromCam < 35.0f )
				return;
			if ( fDistFromCam < 50.0f )
				A *= static_cast<unsigned char>((fDistFromCam-35.0f) * (2.0f/3.0f));
		}

		/*auto it = aCoronas.begin();

		for ( ; it != aCoronas.end(); it++ )
		{
			if ( it->Identifier == nID )
				break;
		}

		if ( it != aCoronas.end() )
		{
			if ( it->FadedIntensity == 0 && A == 0 )
			{
				it->Identifier = 0;
				--NumCoronas;
				return;
			}
		}
		else
		{
			if ( A )
			{
				bool	bFoundFreeSlot = false;
				for ( it = aCoronas.begin(); it != aCoronas.end(); it++ )
				{
					if ( !it->Identifier )
					{
						bFoundFreeSlot = true;
						break;
					}
				}*/

		bool										bFoundFreeSlot = false;
		std::vector<CRegisteredCorona>::iterator	freeSlotIt;
		auto										it = aCoronas.begin();

		for ( ; it != aCoronas.end(); it++ )
		{
			if ( it->Identifier == nID )
				break;
			if ( !bFoundFreeSlot && !it->Identifier )
			{
				bFoundFreeSlot = true;
				freeSlotIt = it;
			}
		}

		if ( it != aCoronas.end() )
		{
			if ( it->FadedIntensity == 0 && A == 0 )
			{
				it->Identifier = 0;
				--NumCoronas;
				return;
			}
		}
		else
		{
			if ( A )
			{
				if ( !bFoundFreeSlot )
				{		
					PrepareForSizeChange();
					aCoronas.push_back(CRegisteredCorona());
 					it = aCoronas.end() - 1;

					UpdatePointersInCode();
				}
				else
					it = freeSlotIt;
				it->FadedIntensity = bFadeIntensity ? 255 : 0;
				it->OffScreen = true;
				it->JustCreated = true;
				it->Identifier = nID;
				it->WhiteCore = false;
				++NumCoronas;
			}
			else
				return;
		}

		it->Red = R;
		it->Green = G;
		it->Blue = B;
		it->Intensity = A;
		it->Coordinates = Position;
		it->Size = Size;
		it->NormalAngle = normalAngle;
		it->Range = Range;
		it->pTex = pTex;
		it->FlareType = flareType;
		it->ReflectionType = reflectionType;
		it->LOSCheck = LOSCheck;
		it->RegisteredThisFrame = true;
		it->PullTowardsCam = PullTowardsCam;
		it->FadeSpeed = FadeSpeed;

		it->NeonFade = bNeonFade;
		it->OnlyFromBelow = bOnlyFromBelow;
		it->WhiteCore = bWhiteCore;

		if ( pAttachTo )
		{
			it->bIsAttachedToEntity = true;
			it->pEntityAttachedTo = pAttachTo;

			pAttachTo->RegisterReference(&it->pEntityAttachedTo);
		}
		else
		{
			it->bIsAttachedToEntity = false;
			it->pEntityAttachedTo = nullptr;
		}
	}
}

void CCoronas::Update()
{
	ScreenMult = min(1.0f, CTimer::ms_fTimeStep * 0.03f + ScreenMult);

	static unsigned int		nSomeHackyMask = 0;
	unsigned int			nThisHackyMask = 0;

	if ( TheCamera.Cams[TheCamera.ActiveCam].LookingLeft )
		nThisHackyMask |= 1;

	if ( TheCamera.Cams[TheCamera.ActiveCam].LookingRight )
		nThisHackyMask |= 2;

	if ( TheCamera.Cams[TheCamera.ActiveCam].LookingBehind )
		nThisHackyMask |= 4;

	if ( TheCamera.GetLookDirection() )
		nThisHackyMask |= 8;

	if ( nSomeHackyMask == nThisHackyMask )
		bChangeBrightnessImmediately = max(0, bChangeBrightnessImmediately - 1);
	else
	{
		bChangeBrightnessImmediately = 3;
		nSomeHackyMask = nThisHackyMask;
	}

	for ( auto it = aCoronas.begin(); it != aCoronas.end(); it++ )
	{
		if ( it->Identifier )
			it->Update();
	}
}

void CCoronas::UpdateCoronaCoors(unsigned int nID, const CVector& vecPosition, float fMaxDist, float fNormalAngle)
{
	CVector*	pCamPos = TheCamera.GetCoords();
	if ( fMaxDist * fMaxDist >= (pCamPos->x - vecPosition.x)*(pCamPos->x - vecPosition.x) + (pCamPos->y - vecPosition.y)*(pCamPos->y - vecPosition.y) )
	{
		auto	it = aCoronas.begin();

		for ( ; it != aCoronas.end(); it++ )
		{
			if ( it->Identifier == nID )
				break;
		}

		if ( it != aCoronas.end() )
		{
			it->Coordinates = vecPosition;
			it->NormalAngle = fNormalAngle;
		}
	}
}

void CCoronas::Init()
{
	// TODO: Do, maybe later
}

void CCoronas::InvalidateAllReferences()
{
	for ( auto it = aCoronas.begin(); it != aCoronas.end(); it++ )
	{
		if ( it->pEntityAttachedTo )
			it->pEntityAttachedTo->CleanUpOldReference(&it->pEntityAttachedTo);
	}
}

void CCoronas::UpdatePointersInCode()
{
	auto	pNewPtr = aCoronas.data();
	auto	nSize = aCoronas.size();
	if ( bRemakeReferences )
	{
		for ( auto it = aCoronas.begin(); it != aCoronas.end(); it++ )
		{
			if ( it->pEntityAttachedTo )
				it->pEntityAttachedTo->RegisterReference(&it->pEntityAttachedTo);
		}

		// CCoronas::RenderReflections
		Memory::Patch<void*>(0x6FB648, &pNewPtr->JustCreated + 1);
		Memory::Patch<void*>(0x6FB6CF, &pNewPtr->FadedIntensity);

		// CCoronas::Render
		Memory::Patch<void*>(0x6FAF42, &pNewPtr->pEntityAttachedTo);

		bRemakeReferences = false;
	}

	// CCoronas::RenderReflections
	Memory::Patch<void*>(0x6FB657, &pNewPtr[nSize].JustCreated + 1);
	Memory::Patch<void*>(0x6FB9B8, &pNewPtr[nSize].FadedIntensity);

	// CCoronas::Render
	Memory::Patch<DWORD>(0x6FAF4A, nSize);
}