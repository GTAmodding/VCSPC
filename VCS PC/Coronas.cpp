#include "StdAfx.h"
#include "Coronas.h"

#include "Camera.h"
#include "Timer.h"
#include "Sprite.h"
#include "Weather.h"
#include "Vehicle.h"
#include "World.h"
#include "Scene.h"
#include "TxdStore.h"

std::map<unsigned int,CCoronasLinkedListNode*>	CCoronas::UsedMap;
CCoronasLinkedListNode							CCoronas::FreeList, CCoronas::UsedList;		
CCoronasLinkedListNode							CCoronas::aLinkedList[NUM_CORONAS];
CRegisteredCorona								CCoronas::aCoronas[NUM_CORONAS];
int&											CCoronas::bChangeBrightnessImmediately = *(int*)0xC3E034;
float&											CCoronas::ScreenMult = *(float*)0x8D4B5C;
float &CCoronas::LightsMult = *(float*)0x8D4B5C;

static std::vector<FlareDef>					SunFlareDef, HeadLightsFlareDef;

WRAPPER void CRegisteredCorona::Update() { EAXJMP(0x6FABF0); }

WRAPPER void CCoronas::RenderReflections(void) { EAXJMP(0x6FB630); }
WRAPPER void CCoronas::DoSunAndMoon(void) { EAXJMP(0x6FC5A0); }
WRAPPER void CCoronas::RenderSunReflection(void) { EAXJMP(0x6FBAA0); }

void CCoronas::RegisterCorona(unsigned int nID, CEntity* pAttachTo, unsigned char R, unsigned char G, unsigned char B, unsigned char A, const CVector& Position, float Size, float Range, RwTexture* pTex, unsigned char flareType, unsigned char reflectionType, unsigned char LOSCheck, unsigned char unused, float normalAngle, bool bNeonFade, float PullTowardsCam, bool bFadeIntensity, float FadeSpeed, bool bOnlyFromBelow, bool bWhiteCore)
{
	UNREFERENCED_PARAMETER(unused);

	CVector		vecPosToCheck;
	if ( pAttachTo )
	{
		if ( !pAttachTo->GetMatrix() )
		{
			pAttachTo->AllocateMatrix();
			pAttachTo->GetTransform().UpdateMatrix(pAttachTo->GetMatrix());
		}
		vecPosToCheck = *pAttachTo->GetMatrix() * Position;
	}
	else
		vecPosToCheck = Position;

	CVector*	pCamPos = &TheCamera.GetCoords();
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
	ScreenMult = Min(1.0f, CTimer::ms_fTimeStep * 0.03f + ScreenMult);

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
		bChangeBrightnessImmediately = Max(0, bChangeBrightnessImmediately - 1);
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
	CVector*	pCamPos = &TheCamera.GetCoords();
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

void CCoronas::ReadFlareDef()
{
	if ( FILE* hFile = fopen("data\\flaredef.dat", "r") )
	{
		DWORD	dwCurrentParserState = 0;
		DWORD	dwCurrentFlareDef = 0;

		while ( const char* pLine = CFileLoader::LoadLine(hFile) )
		{
			if ( pLine[0] && pLine[0] != '#' )
			{
				switch ( dwCurrentParserState )
				{
				case 0:	// Looking for [FLARE]
					{
						if ( !strncmp(pLine, "[FLARE]", 7) )
						{
							dwCurrentParserState = 1;
							dwCurrentFlareDef++;
						}
						break;
					}
				case 1: // Parsing flaredef
					{
						if ( pLine[0] == '[' )
						{
							dwCurrentParserState = 0;
							break;
						}

						FlareDef	newFlareDef;
						DWORD		dwRed, dwGreen, dwBlue, dwAlpha;
						sscanf(pLine, "%f %f %d %d %d %d", &newFlareDef.DistanceToScreenCenter, &newFlareDef.Size,
												&dwRed, &dwGreen, &dwBlue, &dwAlpha);
						newFlareDef.Red = dwRed;
						newFlareDef.Green = dwGreen;
						newFlareDef.Blue = dwBlue;
						newFlareDef.Alpha = dwAlpha;
						if ( dwCurrentFlareDef == 1 )
							SunFlareDef.push_back(newFlareDef);
						else if ( dwCurrentFlareDef == 2 )
							HeadLightsFlareDef.push_back(newFlareDef);

						break;
					}
				}
			
			}
		}

		SunFlareDef.shrink_to_fit();
		HeadLightsFlareDef.shrink_to_fit();
		fclose(hFile);
	}
}

void CCoronas::Init()
{
	static const char coronaTexNames[10][26] = {
		"coronastar",
		"coronastar",
		"coronamoon",
		"coronareflect",
		"coronaheadlightline",
		"", "", "", "",
		"coronaringb"
	};
	static const char coronaMaskNames[10][26] = {
		"", "", "",
		"coronareflectm",
		"", "", "", "", "", ""
	};

	CTxdStore::PushCurrentTxd();
	CTxdStore::SetCurrentTxd(CTxdStore::FindTxdSlot("particle"));
	for(int i = 0; i < 10; i++)
		if(gpCoronaTexture[i] == NULL && *coronaTexNames[i] != '\0')
			gpCoronaTexture[i] = RwTextureRead(coronaTexNames[i], coronaMaskNames[i]);
	CTxdStore::PopCurrentTxd();
	

	// Initialise the lists
	FreeList.Init();
	UsedList.Init();

	for ( int i = 0; i < NUM_CORONAS; i++ )
	{
		aLinkedList[i].Add(&FreeList);
		aLinkedList[i].SetEntry(&aCoronas[i]);
	}

	ReadFlareDef();
}

void CCoronas::Render()
{
	int		nWidth = RwRasterGetWidth(RwCameraGetRaster(Scene.camera));
	int		nHeight = RwRasterGetHeight(RwCameraGetRaster(Scene.camera));

	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, FALSE);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDONE);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDONE);
	//RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);

	for ( int i = 0; i < NUM_CORONAS; i++ )
	{
		if ( aCoronas[i].Identifier )
		{
			if ( aCoronas[i].FadedIntensity > 0 || aCoronas[i].Intensity > 0 )
			{
				RwV3d	vecCoronaCoords, vecTransformedCoords;
				float	fComputedWidth, fComputedHeight;

				if ( CEntity* pEntity = aCoronas[i].pEntityAttachedTo )
				{
					CMatrix*		MatrixWeWant;
					if ( pEntity->nType == 2 && static_cast<CVehicle*>(pEntity)->GetClass() == VEHICLE_BIKE )
					{
						MatrixWeWant = &static_cast<CBike*>(pEntity)->m_bodyRollMatrix;
					}
					else
					{
						if ( !pEntity->GetMatrix() )
						{
							pEntity->AllocateMatrix();
							pEntity->GetTransform().UpdateMatrix(pEntity->GetMatrix());
						}
						MatrixWeWant = pEntity->GetMatrix();
					}
					CVector	vecEntityPos = *MatrixWeWant * aCoronas[i].Coordinates;
					vecCoronaCoords.x = vecEntityPos.x;
					vecCoronaCoords.y = vecEntityPos.y;
					vecCoronaCoords.z = vecEntityPos.z;
				}
				else
				{
					vecCoronaCoords.x = aCoronas[i].Coordinates.x;
					vecCoronaCoords.y = aCoronas[i].Coordinates.y;
					vecCoronaCoords.z = aCoronas[i].Coordinates.z;
				}

				if ( CSprite::CalcScreenCoors(vecCoronaCoords, &vecTransformedCoords, &fComputedWidth, &fComputedHeight, true, true) )
				{
					aCoronas[i].OffScreen = !( vecTransformedCoords.x >= 0.0 && vecTransformedCoords.x <= nWidth &&
											vecTransformedCoords.y >= 0.0 && vecTransformedCoords.y <= nHeight);

					if ( aCoronas[i].FadedIntensity > 0 && vecTransformedCoords.z <= aCoronas[i].Range )
					{
						float	fInvFarClip = 1.0f / vecTransformedCoords.z;
						float	fHalfRange = aCoronas[i].Range * 0.5f;
					
						short	nFadeIntensity = aCoronas[i].FadedIntensity * (vecTransformedCoords.z > fHalfRange ? 1.0f - (vecTransformedCoords.z - fHalfRange) / fHalfRange : 1.0f);

						RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)(aCoronas[i].LOSCheck == false));

						if ( aCoronas[i].pTex )
						{
							float	fColourFogMult = Min(40.0f, vecTransformedCoords.z) * CWeather::Foggyness * 0.025f + 1.0f;	// TODO: Check

//							if ( aCoronas[i].Identifier == 1 )	// Sun core
//								vecTransformedCoords.z = RwCameraGetFarClipPlane(Camera) * 0.95f;

							// This R* tweak broke the sun
							//RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
							RwRenderStateSet(rwRENDERSTATETEXTURERASTER, RwTextureGetRaster(aCoronas[i].pTex));

							RwV3d		vecCoronaCoordsAfterPull = vecCoronaCoords;
							CVector		vecTempVector(vecCoronaCoordsAfterPull);
							vecTempVector -= TheCamera.GetCoords();
							vecTempVector.Normalise();

							vecCoronaCoordsAfterPull.x -= (vecTempVector.x * aCoronas[i].PullTowardsCam);
							vecCoronaCoordsAfterPull.y -= (vecTempVector.y * aCoronas[i].PullTowardsCam);
							vecCoronaCoordsAfterPull.z -= (vecTempVector.z * aCoronas[i].PullTowardsCam);

							if ( CSprite::CalcScreenCoors(vecCoronaCoordsAfterPull, &vecTransformedCoords, &fComputedWidth, &fComputedHeight, true, true) )
								CSprite::RenderOneXLUSprite_Rotate_Aspect(vecTransformedCoords.x, vecTransformedCoords.y, vecTransformedCoords.z,
								aCoronas[i].Size * fComputedWidth, aCoronas[i].Size * fComputedHeight * fColourFogMult,
								aCoronas[i].Red / fColourFogMult, aCoronas[i].Green / fColourFogMult, aCoronas[i].Blue / fColourFogMult, nFadeIntensity,
								fInvFarClip * 20.0f, 0.0, 0xFF);

						}
					
						// TODO: Flare effect
					}
				}
				else
					aCoronas[i].OffScreen = true;
			}
		}
	}

	CSprite::FlushSpriteBuffer();
}

void CCoronas::RenderBuffered()
{
	int		nWidth = RwRasterGetWidth(RwCameraGetRaster(Scene.camera));
	int		nHeight = RwRasterGetHeight(RwCameraGetRaster(Scene.camera));

	// For buffered render
	RwRaster*	pLastRaster = nullptr;
	bool		bLastZWriteRenderState = true;

	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, FALSE);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDONE);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDONE);
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);

	CSprite::InitSpriteBuffer2D();

	for ( int i = 0; i < NUM_CORONAS; i++ )
	{
		if ( aCoronas[i].Identifier )
		{
			if ( aCoronas[i].FadedIntensity > 0 || aCoronas[i].Intensity > 0 )
			{
				RwV3d	vecCoronaCoords, vecTransformedCoords;
				float	fComputedWidth, fComputedHeight;

				if ( CEntity* pEntity = aCoronas[i].pEntityAttachedTo )
				{
					CMatrix*		MatrixWeWant;
					if ( pEntity->nType == 2 && static_cast<CVehicle*>(pEntity)->GetClass() == VEHICLE_BIKE )
					{
						MatrixWeWant = &static_cast<CBike*>(pEntity)->m_bodyRollMatrix;
					}
					else
					{
						if ( !pEntity->GetMatrix() )
						{
							pEntity->AllocateMatrix();
							pEntity->GetTransform().UpdateMatrix(pEntity->GetMatrix());
						}
						MatrixWeWant = pEntity->GetMatrix();
					}
					CVector	vecEntityPos = *MatrixWeWant * aCoronas[i].Coordinates;
					vecCoronaCoords.x = vecEntityPos.x;
					vecCoronaCoords.y = vecEntityPos.y;
					vecCoronaCoords.z = vecEntityPos.z;
				}
				else
				{
					vecCoronaCoords.x = aCoronas[i].Coordinates.x;
					vecCoronaCoords.y = aCoronas[i].Coordinates.y;
					vecCoronaCoords.z = aCoronas[i].Coordinates.z;
				}

				if ( CSprite::CalcScreenCoors(vecCoronaCoords, &vecTransformedCoords, &fComputedWidth, &fComputedHeight, true, true) )
				{
					aCoronas[i].OffScreen = !( vecTransformedCoords.x >= 0.0 && vecTransformedCoords.x <= nWidth &&
											vecTransformedCoords.y >= 0.0 && vecTransformedCoords.y <= nHeight);

					if ( aCoronas[i].FadedIntensity > 0 && vecTransformedCoords.z <= aCoronas[i].Range )
					{
						float	fInvFarClip = 1.0f / vecTransformedCoords.z;
						float	fHalfRange = aCoronas[i].Range * 0.5f;
					
						short	nFadeIntensity = aCoronas[i].FadedIntensity * (vecTransformedCoords.z > fHalfRange ? 1.0f - (vecTransformedCoords.z - fHalfRange) / fHalfRange : 1.0f);

						if ( bLastZWriteRenderState != aCoronas[i].LOSCheck == false )
						{
							bLastZWriteRenderState = aCoronas[i].LOSCheck == false;
							CSprite::FlushSpriteBuffer();

							RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)bLastZWriteRenderState);
						}
				
						if ( aCoronas[i].pTex )
						{
							float	fColourFogMult = Min(40.0f, vecTransformedCoords.z) * CWeather::Foggyness * 0.025f + 1.0f;	// TODO: Check

//							if ( aCoronas[i].Identifier == 1 )	// Sun core
//								vecTransformedCoords.z = RwCameraGetFarClipPlane(Camera) * 0.95f;

							// This R* tweak broke the sun
							//RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
							if ( pLastRaster != RwTextureGetRaster(aCoronas[i].pTex) )
							{
								pLastRaster = RwTextureGetRaster(aCoronas[i].pTex);
								CSprite::FlushSpriteBuffer();

								RwRenderStateSet(rwRENDERSTATETEXTURERASTER, pLastRaster);
							}

							RwV3d		vecCoronaCoordsAfterPull = vecCoronaCoords;
							CVector		vecTempVector(vecCoronaCoordsAfterPull);
							vecTempVector -= TheCamera.GetCoords();
							vecTempVector.Normalise();

							vecCoronaCoordsAfterPull.x -= (vecTempVector.x * aCoronas[i].PullTowardsCam);
							vecCoronaCoordsAfterPull.y -= (vecTempVector.y * aCoronas[i].PullTowardsCam);
							vecCoronaCoordsAfterPull.z -= (vecTempVector.z * aCoronas[i].PullTowardsCam);

							if ( CSprite::CalcScreenCoors(vecCoronaCoordsAfterPull, &vecTransformedCoords, &fComputedWidth, &fComputedHeight, true, true) )
							{
								CSprite::RenderBufferedOneXLUSprite_Rotate_Aspect(vecTransformedCoords.x, vecTransformedCoords.y, vecTransformedCoords.z,
									aCoronas[i].Size * fComputedWidth, aCoronas[i].Size * fComputedHeight * fColourFogMult,
									aCoronas[i].Red / fColourFogMult, aCoronas[i].Green / fColourFogMult, aCoronas[i].Blue / fColourFogMult, nFadeIntensity,
									fInvFarClip * 20.0f, 0.0, 0xFF);
							}

						}
					
						
						if ( aCoronas[i].FlareType != 0 )
						{
							const std::vector<FlareDef>&	vecFlares = aCoronas[i].FlareType == 1 ? SunFlareDef : HeadLightsFlareDef;

							if ( bLastZWriteRenderState != false )
							{
								bLastZWriteRenderState = false;
								CSprite::FlushSpriteBuffer();

								RwRenderStateSet(rwRENDERSTATEZTESTENABLE, FALSE);
							}
							if ( pLastRaster != RwTextureGetRaster(gpCoronaTexture[0]) )
							{
								pLastRaster = RwTextureGetRaster(gpCoronaTexture[0]);
								CSprite::FlushSpriteBuffer();

								RwRenderStateSet(rwRENDERSTATETEXTURERASTER, pLastRaster);
							}
							
							for ( auto it = vecFlares.cbegin(); it != vecFlares.cend(); it++ )
							{
								float	fRandomFlicker = (rand() * (1.0f/RAND_MAX) * 0.3f + 0.7f) * aCoronas[i].FadedIntensity * (1.0f/65536.0f);

								CColPoint	outColPoint;
								CEntity*	pOutEntity;

								RwRGBA		colour;
								colour.red = aCoronas[i].Red * it->Red * fRandomFlicker;
								colour.green = aCoronas[i].Green * it->Green * fRandomFlicker;
								colour.blue = aCoronas[i].Blue * it->Blue * fRandomFlicker;
								colour.alpha = 255;

								if ( !CWorld::ProcessLineOfSight(vecCoronaCoords, TheCamera.GetCoords(), outColPoint, pOutEntity, false, true, true, false, false, false, false, true) )
								{
									CSprite::RenderBufferedOneXLUSprite2D( (vecTransformedCoords.x - (nWidth/2)) * it->DistanceToScreenCenter + (nWidth/2), 
																		(vecTransformedCoords.y - (nHeight/2)) * it->DistanceToScreenCenter + (nHeight/2),
																		it->Size * 4.0f, it->Size * 4.0f, colour, 255, 255 );							
								}
							}
						}
					}
				}
				else
					aCoronas[i].OffScreen = true;
			}
		}
	}

	CSprite::FlushSpriteBuffer();
}

void CCoronas::RenderDebug()
{
#ifndef NDEBUG
//	D3DPERF_BeginEvent(D3DCOLOR_ARGB(0xFF, 0xFF, 0, 0), L"coronas render");
#endif

#ifdef DEVBUILD
	if ( GetAsyncKeyState(VK_F4) & 0x8000 )
		Render();
	else
#endif
		RenderBuffered();

#ifndef NDEBUG
//	D3DPERF_EndEvent();
#endif
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
	using namespace Memory;

	// CCoronas::RenderReflections
	Patch<void*>(0x6FB648, &aCoronas->JustCreated + 1);
	Patch<void*>(0x6FB6CF, &aCoronas->FadedIntensity);

	// CCoronas::RenderReflections
	Patch<void*>(0x6FB657, &aCoronas[NUM_CORONAS].JustCreated + 1);
	Patch<void*>(0x6FB9B8, &aCoronas[NUM_CORONAS].FadedIntensity);

	InjectHook(0x6FC180, CCoronas::RegisterCorona, PATCH_JUMP);
	InjectHook(0x6FC4D0, CCoronas::UpdateCoronaCoors, PATCH_JUMP);
	// InjectHook(0x6FAAD9, CCoronas::Init, PATCH_JUMP);	// REVERSED
	// InjectHook(0x53C13B, CCoronas::Update);		// REVERSED
	// InjectHook(0x53E18E, CCoronas::RenderBuffered);	// REVERSED

	static const float	fSunDist = 150.0f;
	Patch<const void*>(0x6FC5AA, &fSunDist);
	Nop(0x6FC5AE, 6);
	static const float coreSz = 10.64/10.0f;
	static const float coronaSz = 20.0f/10.0;
	Patch(0x6FC65C + 2, &coreSz);
	Patch(0x6FC6E8 + 2, &coronaSz);
}

static StaticPatcher	Patcher([](){ 
						CCoronas::Inject();
									});