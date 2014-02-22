#include "StdAfx.h"
#include "Coronas.h"

#include "CCamera.h"
#include "Timer.h"

std::map<unsigned int,CCoronasLinkedListNode*>	CCoronas::UsedMap;
CCoronasLinkedListNode							CCoronas::FreeList, CCoronas::UsedList;		
CCoronasLinkedListNode							CCoronas::aLinkedList[NUM_CORONAS];
CRegisteredCorona								CCoronas::aCoronas[NUM_CORONAS];
int&											CCoronas::bChangeBrightnessImmediately = *(int*)0xC3E034;
float&											CCoronas::ScreenMult = *(float*)0x8D4B5C;

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

		// Is corona already present?
		CRegisteredCorona*		pSuitableSlot;
		auto it = UsedMap.find(nID);

		if ( it != UsedMap.end() )
		{
			pSuitableSlot = it->second->GetFrom();

			if ( pSuitableSlot->FadedIntensity == 0 && A == 0 )
			{
				// Mark as free
				it->second->GetFrom()->Identifier = 0;				
				it->second->Add(&FreeList);
				UsedMap.erase(nID);
				return;
			}
		}
		else
		{
			if ( !A )
				return;

			// Adding a new element
			auto	pNewEntry = FreeList.First();
			if ( !pNewEntry )
			{
				LogToFile("ERROR: Not enough space for coronas!");
				return;
			}

			pSuitableSlot = pNewEntry->GetFrom();

			// Add to used list and push this index to the map
			pNewEntry->Add(&UsedList);
			UsedMap[nID] = pNewEntry;

			pSuitableSlot->FadedIntensity = bFadeIntensity ? 255 : 0;
			pSuitableSlot->OffScreen = true;
			pSuitableSlot->JustCreated = true;
			pSuitableSlot->Identifier = nID;
		}


		/*bool													bFoundFreeSlot = false;
		std::array<CRegisteredCorona,NUM_CORONAS>::iterator	freeSlotIt;
		auto													it = aCoronas.begin();

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
		}*/

		pSuitableSlot->Red = R;
		pSuitableSlot->Green = G;
		pSuitableSlot->Blue = B;
		pSuitableSlot->Intensity = A;
		pSuitableSlot->Coordinates = Position;
		pSuitableSlot->Size = Size;
		pSuitableSlot->NormalAngle = normalAngle;
		pSuitableSlot->Range = Range;
		pSuitableSlot->pTex = pTex;
		pSuitableSlot->FlareType = flareType;
		pSuitableSlot->ReflectionType = reflectionType;
		pSuitableSlot->LOSCheck = LOSCheck;
		pSuitableSlot->RegisteredThisFrame = true;
		pSuitableSlot->PullTowardsCam = PullTowardsCam;
		pSuitableSlot->FadeSpeed = FadeSpeed;

		pSuitableSlot->NeonFade = bNeonFade;
		pSuitableSlot->OnlyFromBelow = bOnlyFromBelow;
		pSuitableSlot->WhiteCore = bWhiteCore;

		if ( pAttachTo )
		{
			pSuitableSlot->bIsAttachedToEntity = true;
			pSuitableSlot->pEntityAttachedTo = pAttachTo;

			pAttachTo->RegisterReference(&pSuitableSlot->pEntityAttachedTo);
		}
		else
		{
			pSuitableSlot->bIsAttachedToEntity = false;
			pSuitableSlot->pEntityAttachedTo = nullptr;
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

	auto pNode = UsedList.First();
	if ( pNode )
	{
		while ( pNode != &UsedList )
		{
			unsigned int	nIndex = pNode->GetFrom()->Identifier;
			auto			pNext = pNode->GetNextNode();

			pNode->GetFrom()->Update();

			// Did it become invalid?
			if ( !pNode->GetFrom()->Identifier )
			{
				// Remove from used list
				pNode->Add(&FreeList);
				UsedMap.erase(nIndex);
			}

			pNode = pNext;
		}
	}
}

void CCoronas::UpdateCoronaCoors(unsigned int nID, const CVector& vecPosition, float fMaxDist, float fNormalAngle)
{
	CVector*	pCamPos = TheCamera.GetCoords();
	if ( fMaxDist * fMaxDist >= (pCamPos->x - vecPosition.x)*(pCamPos->x - vecPosition.x) + (pCamPos->y - vecPosition.y)*(pCamPos->y - vecPosition.y) )
	{
		auto	it = UsedMap.find(nID);

		if ( it != UsedMap.end() )
		{
			it->second->GetFrom()->Coordinates = vecPosition;
			it->second->GetFrom()->NormalAngle = fNormalAngle;
		}
	}
}

void CCoronas::Init()
{
	// Initialise the lists
	FreeList.Init();
	UsedList.Init();

	for ( int i = 0; i < NUM_CORONAS; i++ )
	{
		aLinkedList[i].Add(&FreeList);
		aLinkedList[i].SetEntry(&aCoronas[i]);
	}
}

/*void CCoronas::InvalidateAllReferences()
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
			{
				it->pEntityAttachedTo->RegisterReference(&it->pEntityAttachedTo);
				assert(*(int*)(&it->pEntityAttachedTo) == (int)it->pEntityAttachedTo);
			}
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
}*/

void CCoronas::Inject()
{
	// CCoronas::RenderReflections
	Memory::Patch<void*>(0x6FB648, &aCoronas->JustCreated + 1);
	Memory::Patch<void*>(0x6FB6CF, &aCoronas->FadedIntensity);

	// CCoronas::Render
	Memory::Patch<void*>(0x6FAF42, &aCoronas->pEntityAttachedTo);

	// CCoronas::RenderReflections
	Memory::Patch<void*>(0x6FB657, &aCoronas[NUM_CORONAS].JustCreated + 1);
	Memory::Patch<void*>(0x6FB9B8, &aCoronas[NUM_CORONAS].FadedIntensity);

	// CCoronas::Render
	Memory::Patch<DWORD>(0x6FAF4A, NUM_CORONAS);

	Memory::InjectHook(0x6FC180, CCoronas::RegisterCorona, PATCH_JUMP);
	Memory::InjectHook(0x6FC4D0, CCoronas::UpdateCoronaCoors, PATCH_JUMP);
	Memory::InjectHook(0x6FAAD9, CCoronas::Init, PATCH_JUMP);
	Memory::InjectHook(0x53C13B, CCoronas::Update);
}