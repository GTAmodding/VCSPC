#include "StdAfx.h"
#include "Script.h"

#include "General.h"
#include "Vehicle.h"
#include "CConfiscatedWeapons.h"
#include "Text.h"
#include "CCamera.h"
#include "Radar.h"
#include "UserDisplay.h"
#include "Streaming.h"
#include "World.h"
#include "Pools.h"
#include "EmpireMgr.h"
#include "PcSave.h"
#include "GroupedBuildings.h"

CScriptFunction		CRunningScript::ms_scriptFunction[NUM_SCRIPTS];

// Wrappers
WRAPPER bool CTheScripts::IsPlayerOnAMission() { EAXJMP(0x464D50); }
WRAPPER CRunningScript* CTheScripts::StartNewScript(void* nScriptIP) { WRAPARG(nScriptIP); EAXJMP(0x464C20); }

WRAPPER void CRunningScript::GetStringParam(char* textPtr, BYTE len) { WRAPARG(textPtr); WRAPARG(len); EAXJMP(0x463D50); }
WRAPPER void CRunningScript::CollectParameters(WORD numParams) { WRAPARG(numParams); EAXJMP(0x464080); }
WRAPPER void CRunningScript::StoreParameters(WORD numParams) { WRAPARG(numParams); EAXJMP(0x464370); }
WRAPPER void CRunningScript::UpdateCompareFlag(bool result) { WRAPARG(result); EAXJMP(0x4859D0); }
WRAPPER unsigned short CRunningScript::GetGlobalVarOffset() { EAXJMP(0x464700); }
WRAPPER void* CRunningScript::GetPointerToScriptVariable(int nParam = 0) { WRAPARG(nParam); EAXJMP(0x464790); }
WRAPPER void CRunningScript::SetIP(int IP) { WRAPARG(IP); EAXJMP(0x464DA0); }

void CRunningScript::Init()
{
	((void(__thiscall*)(CRunningScript*))0x4648E0)(this);

	ms_scriptFunction[this - ScriptsArray].Init(this);
}

CScriptFunction& CRunningScript::GetExtrasForScript()
{
	return ms_scriptFunction[this - ScriptsArray];
}

signed char CRunningScript::CollectParametersForScriptFunction(signed short numParams)
{
	if ( !numParams )
		return 0;

	SCRIPT_VAR*		varsPtr = scriptParams;
	WORD			loopCounter = numParams;
	signed char		bOccupiedVars = 0;
	do
	{
		eOperandType operand = static_cast<eOperandType>(*static_cast<BYTE*>(CurrentIP));
		CurrentIP = static_cast<BYTE*>(CurrentIP) + 1;
		
		switch ( operand )
		{
		case imm32f:
		case imm32:
			{
				signed int value = *static_cast<signed int*>(CurrentIP);
				CurrentIP = static_cast<BYTE*>(CurrentIP) + 4;
				varsPtr->iParam = value;
				++bOccupiedVars;
				break;
			}
		case globalVar:
			{
				WORD variable = *static_cast<WORD*>(CurrentIP);
				CurrentIP = static_cast<BYTE*>(CurrentIP) + 2;
				*varsPtr = *reinterpret_cast<SCRIPT_VAR*>(static_cast<BYTE*>(scmBlock) + variable);
				++bOccupiedVars;
				break;
			}
		case localVar:
			{
				WORD variable = *static_cast<WORD*>(CurrentIP);
				CurrentIP = static_cast<BYTE*>(CurrentIP) + 2;
				if ( bIsMission )
					*varsPtr = scriptLocals[variable];
				else
					*varsPtr = LocalVar[variable];
				++bOccupiedVars;
				break;
			}
		case imm8:
			{
				signed int value = *static_cast<signed char*>(CurrentIP);
				CurrentIP = static_cast<BYTE*>(CurrentIP) + 1;
				varsPtr->iParam = value;
				++bOccupiedVars;
				break;
			}
		case imm16:
			{
				signed int value = *static_cast<signed short*>(CurrentIP);
				CurrentIP = static_cast<BYTE*>(CurrentIP) + 2;
				varsPtr->iParam = value;
				++bOccupiedVars;
				break;
			}
		case globalArr:
			{
				WORD variable = *static_cast<WORD*>(CurrentIP);
				WORD index = *(static_cast<WORD*>(CurrentIP) + 1);
				CurrentIP = static_cast<BYTE*>(CurrentIP) + 2;
				WORD index2 = *(static_cast<WORD*>(CurrentIP) + 1);
				CurrentIP = static_cast<BYTE*>(CurrentIP) + 4;

				if ( ((CHAR)(((WORD)(index2) >> 8) & 0xFF)) >= 0 )		// !?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?
				{
					SCRIPT_VAR indexVariable;
					if ( bIsMission )
						indexVariable = scriptLocals[index];
					else
						indexVariable = LocalVar[index];

					*varsPtr = *reinterpret_cast<SCRIPT_VAR*>(static_cast<BYTE*>(scmBlock) + 4 * indexVariable.dwParam + variable);
				}
				else
				{
					SCRIPT_VAR indexVariable = *reinterpret_cast<SCRIPT_VAR*>(static_cast<BYTE*>(scmBlock) + index);
					*varsPtr = *reinterpret_cast<SCRIPT_VAR*>(static_cast<BYTE*>(scmBlock) + 4 * indexVariable.dwParam + variable);
				}
				++bOccupiedVars;
				break;
			}
		case localArr:
			{
				WORD variable = *static_cast<WORD*>(CurrentIP);
				WORD index = *(static_cast<WORD*>(CurrentIP) + 1);
				CurrentIP = static_cast<BYTE*>(CurrentIP) + 2;
				WORD index2 = *(static_cast<WORD*>(CurrentIP) + 1);
				CurrentIP = static_cast<BYTE*>(CurrentIP) + 4;

				DWORD finalIndex;

				if ( ((CHAR)(((WORD)(index2) >> 8) & 0xFF)) >= 0 )
				{
					SCRIPT_VAR indexVariable;
					if ( bIsMission )
						indexVariable = scriptLocals[index];
					else
						indexVariable = LocalVar[index];

					finalIndex = indexVariable.dwParam + variable;
				}
				else
				{
					SCRIPT_VAR indexVariable = *reinterpret_cast<SCRIPT_VAR*>(static_cast<BYTE*>(scmBlock) + index);
					finalIndex = indexVariable.dwParam + variable;
				}
				if ( bIsMission )
					*varsPtr = scriptLocals[finalIndex];
				else
					*varsPtr = LocalVar[finalIndex];
				++bOccupiedVars;
				break;
			}
		case sstring:
		case globalVarSString:
		case localVarSString:
		case globalVarSArrString:
		case localVarSArrString:
		case vstring:
		case lstring:
		case globalVarVString:
		case localVarVString:
		case globalVarVArrString:
		case localVarVArrString:
			{
				CurrentIP = static_cast<BYTE*>(CurrentIP) - 1;
				BYTE len = ( operand == sstring || operand == globalVarSString || operand == localVarSString
										|| operand == globalVarSArrString || operand == localVarSArrString ) ? 8 : 16;
				GetStringParam(reinterpret_cast<char*>(varsPtr), len);
				varsPtr += ( (len - 1) / sizeof(varsPtr) );

				bOccupiedVars += (len / 4);
			}
		}
		++varsPtr;
		--loopCounter;
	}
	while ( loopCounter );

	return bOccupiedVars;
}

void CRunningScript::StoreParametersFromScriptFunction()
{
	SCRIPT_VAR*		varsPtr = scriptParams;
	if ( *static_cast<BYTE*>(CurrentIP) )
	{
		do
		{
			eOperandType operand = static_cast<eOperandType>(*static_cast<BYTE*>(CurrentIP));
			CurrentIP = static_cast<BYTE*>(CurrentIP) + 1;
		
			switch ( operand )
			{
			case globalVar:
				{
					WORD variable = *static_cast<WORD*>(CurrentIP);
					CurrentIP = static_cast<BYTE*>(CurrentIP) + 2;
					*reinterpret_cast<SCRIPT_VAR*>(static_cast<BYTE*>(scmBlock) + variable) = *varsPtr;
					break;
				}
			case localVar:
				{
					WORD variable = *static_cast<WORD*>(CurrentIP);
					CurrentIP = static_cast<BYTE*>(CurrentIP) + 2;
					if ( bIsMission )
						scriptLocals[variable] = *varsPtr;
					else
						LocalVar[variable] = *varsPtr;
					break;
				}
			case globalArr:
				{
					WORD variable = *static_cast<WORD*>(CurrentIP);
					WORD index = *(static_cast<WORD*>(CurrentIP) + 1);
					CurrentIP = static_cast<BYTE*>(CurrentIP) + 2;
					WORD index2 = *(static_cast<WORD*>(CurrentIP) + 1);
					CurrentIP = static_cast<BYTE*>(CurrentIP) + 4;	

					if ( ((CHAR)(((WORD)(index2) >> 8) & 0xFF)) >= 0 )
					{
						SCRIPT_VAR indexVariable;
						if ( bIsMission )
							indexVariable = scriptLocals[index];
						else
							indexVariable = LocalVar[index];

						*reinterpret_cast<SCRIPT_VAR*>(static_cast<BYTE*>(scmBlock) + 4 * indexVariable.dwParam + variable) = *varsPtr;
					}
					else
					{
						SCRIPT_VAR indexVariable = *reinterpret_cast<SCRIPT_VAR*>(static_cast<BYTE*>(scmBlock) + index);
						*reinterpret_cast<SCRIPT_VAR*>(static_cast<BYTE*>(scmBlock) + 4 * indexVariable.dwParam + variable) = *varsPtr;
					}
					break;
				}
			case localArr:
				{
					WORD variable = *static_cast<WORD*>(CurrentIP);
					WORD index = *(static_cast<WORD*>(CurrentIP) + 1);
					CurrentIP = static_cast<BYTE*>(CurrentIP) + 2;
					WORD index2 = *(static_cast<WORD*>(CurrentIP) + 1);
					CurrentIP = static_cast<BYTE*>(CurrentIP) + 4;

					DWORD finalIndex;

					if ( ((CHAR)(((WORD)(index2) >> 8) & 0xFF)) >= 0 )
					{
						SCRIPT_VAR indexVariable;
						if ( bIsMission )
							indexVariable = scriptLocals[index];
						else
							indexVariable = LocalVar[index];

						finalIndex = indexVariable.dwParam + variable;
					}
					else
					{
						SCRIPT_VAR indexVariable = *reinterpret_cast<SCRIPT_VAR*>(static_cast<BYTE*>(scmBlock) + index);
						finalIndex = indexVariable.dwParam + variable;
					}
					if ( bIsMission )
						scriptLocals[finalIndex] = *varsPtr;
					else
						LocalVar[finalIndex] = *varsPtr;
					break;
				}
			case globalVarSString:
			case localVarSString:
			case globalVarSArrString:
			case localVarSArrString:
			case globalVarVString:
			case localVarVString:
			case globalVarVArrString:
			case localVarVArrString:
				{
					CurrentIP = static_cast<BYTE*>(CurrentIP) - 1;
					BYTE len = ( operand == globalVarSString || operand == localVarSString
											|| operand == globalVarSArrString || operand == localVarSArrString ) ? 8 : 16;
				
					strncpy(static_cast<char*>(GetPointerToScriptVariable()), reinterpret_cast<const char*>(varsPtr), len);
					varsPtr += ( (len - 1) / sizeof(varsPtr) );
				}
			}
			++varsPtr;
		}
		while ( *static_cast<BYTE*>(CurrentIP) );

	}
	CurrentIP = static_cast<BYTE*>(CurrentIP) + 1;
}

void CRunningScript::ProcessVCSCommands(WORD opcode)
{
	switch ( opcode )
	{
	case 0x0050:
		{
			// gosub %1p%
			assert(SP < GOSUB_STACK_SIZE);
			CollectParameters(1);
			if ( !IsCustom )
				GetExtrasForScript().RegisterGosub(this);
			Stack[SP++] = CurrentIP;
			SetIP(scriptParams[0].iParam);
			return;
		}
	case 0x0051:
	case 0x0054:
	case 0x0055:
		{
			// return
			// return_true
			// return_false
			assert(SP);
			CurrentIP = Stack[--SP];

			if ( !IsCustom )
			{
				CScriptFunction&	scriptExtras = GetExtrasForScript();

				if ( scriptExtras.bAmountOfVariablesWePassed[SP] != -1 /*scriptExtras->bThisIndirectionLevelIsCall[SP]*/ )
				{
					if ( !bIsMission )
						memcpy(scriptParams, &LocalVar[scriptExtras.bAmountOfVariablesWePassed[SP]], 32 * sizeof(SCRIPT_VAR));
					else
						memcpy(scriptParams, &scriptLocals[scriptExtras.bAmountOfVariablesWePassed[SP]], 32 * sizeof(SCRIPT_VAR));

					scriptExtras.PopCall(this);
					StoreParametersFromScriptFunction();
				}
			}


			if ( opcode != 0x0051 )
				UpdateCompareFlag(opcode == 0x0054);
			return;
		}
	case 0x014D:
		{
			// add_pager_message %1g% %2d% %3d%
			char	cString[8];
			GetStringParam(cString, 8);
			CollectParameters(2);
			CUserDisplay::Pager.AddMessage(const_cast<char*>(TheText.Get(cString)), static_cast<short>(scriptParams[0].iParam), static_cast<short>(scriptParams[1].iParam));
			return;
		}

	case 0x0166:
		{
			// dim_blip %1d% %2d%
			CollectParameters(2);
			CRadar::ChangeBlipBrightness(scriptParams[0].iParam, scriptParams[1].iParam);
			//LogToFile("Opcode 0166 called with params: %d %d", scriptParams[0], scriptParams[1]);
			return;
		}
	case 0x0351:
		{	
			// dont_touch_fov_in_widescreen %1d%
			CollectParameters(1);
			CCamera::bDontTouchFOVInWidescreen = scriptParams[0].bParam != 0;
			LogToFile("Opcode 0351 called with params: %d", scriptParams[0]);
			return;
		}
	case 0x0352:
		{
			// enter_amazing_screenshot_mode %1d%
			CollectParameters(1);
#if defined DEVBUILD && !defined MAKE_ZZCOOL_MOVIE_DEMO
			EnterAmazingScreenshotMode(scriptParams[0].bParam != 0);
			LogToFile("Opcode 0352 called with params: %d", scriptParams[0]);
#endif
			return;
		}
	case 0x0353:
		{
			// is_today_the_day %1d%
			CollectParameters(1);
			switch ( scriptParams[0].iParam )
			{
			case 0:
				{
					// INDEPENDENCE_DAY (4.07)
					CDate		Today = GetCurrentDate();
					UpdateCompareFlag(Today >= CDate(4, 7) && Today < CDate(5, 7));
					break;
				}
			default:
				{
					UpdateCompareFlag(false);
					break;
				}
			}
			return;
		}
	case 0x0354:
		{
			// change_player %1d% model_to %2o%
			char	cString[8];
			CollectParameters(1);
			GetStringParam(cString, 8);

			ToLower(cString, 8);
			CStreaming::RequestSpecialModel(0, cString, 26);
			CStreaming::LoadAllRequestedModels(true);
			CPed* pPed = CWorld::Players[scriptParams[0].bParam].GetPed();
			DWORD iAnimGroupBackup = pPed->GetMoveAnimGroup();
			pPed->DeleteRwObject();
			pPed->ModelIndex() = -1;
			pPed->SetModelIndex(0);
			pPed->SetMoveAnimGroup(iAnimGroupBackup);
			LogToFile("Opcode 0352 called with params: %d %s", scriptParams[0], cString);
			return;
		}
	case 0x0355:
		{
			// set_actor %1d% colours_to %2d% %3d% %4d% %5d%
			CollectParameters(5);
			CPedData::pPedData[scriptParams[0].iParam >> 8].SetColours(scriptParams[1].bParam, scriptParams[2].bParam, scriptParams[3].bParam, scriptParams[4].bParam);
			LogToFile("Opcode 0355 called with params: %d %d %d %d %d", scriptParams[0], scriptParams[1], scriptParams[2], scriptParams[3], scriptParams[4]);
			return;
		}
	case 0x037A:
		{
			// call_function %1p% num_params %2d% params ...
			assert(SP < GOSUB_STACK_SIZE);
			CollectParameters(2);
			int			offsetToJmp	= scriptParams[0].iParam;
			signed char	passedParams = static_cast<signed char>(scriptParams[1].iParam);
			//BYTE	returnedParams = scriptParams[2].bParam;

			signed char occupiedVars = CollectParametersForScriptFunction(passedParams);
			GetExtrasForScript().RegisterCall(this, occupiedVars);

			if ( passedParams )
			{
				if ( !bIsMission )
					memcpy(LocalVar, scriptParams, 32 * sizeof(SCRIPT_VAR));
				else
					memcpy(scriptLocals, scriptParams, 32 * sizeof(SCRIPT_VAR));
			}
			Stack[SP++] = CurrentIP;
			SetIP(offsetToJmp);
			//LogToFile("Opcode 037A called with params: %d %d ...", offsetToJmp, passedParams);
			return;
		}
	case 0x03C1:
		{
			// display_onscreen_counter_with_two_numbers_place %1d% %2d% %3h% %4h% %5g% %6g%
			int*			pVariable[2];
			char			cString[8], cPlaceString[8];

			pVariable[0] = static_cast<int*>(GetPointerToScriptVariable());
			pVariable[1] = static_cast<int*>(GetPointerToScriptVariable());
			CollectParameters(2);
			GetStringParam(cPlaceString, 8);
			GetStringParam(cString, 8);
			CUserDisplay::OnscnTimer.AddCounterTwoVars(pVariable[0], pVariable[1], cString, scriptParams[1].wParam, cPlaceString, scriptParams[0].wParam);
			//LogToFile("Opcode 03C2 called with params: 0x%08X 0x%08X %d %s", pVariable[0], pVariable[1], scriptParams[0], cString);
			return;
		}
	case 0x03C2:
		{
			// display_onscreen_counter_with_two_numbers %1d% %2d% %3h% %4g%
			int*			pVariable[2];
			char			cString[8];

			pVariable[0] = static_cast<int*>(GetPointerToScriptVariable());
			pVariable[1] = static_cast<int*>(GetPointerToScriptVariable());
			CollectParameters(1);
			GetStringParam(cString, 8);
			CUserDisplay::OnscnTimer.AddCounterTwoVars(pVariable[0], pVariable[1], cString, scriptParams[0].wParam);
			//LogToFile("Opcode 03C2 called with params: 0x%08X 0x%08X %d %s", pVariable[0], pVariable[1], scriptParams[0], cString);
			return;
		}
	case 0x03DA:
		{
			// set_onscreen_counter_colour %1d% %2d% %3d% %4d%
			CollectParameters(4);
			CUserDisplay::OnscnTimer.SetColourOverride(scriptParams[0].bParam, scriptParams[1].bParam, scriptParams[2].bParam, scriptParams[3].bParam);
			return;
		}
	case 0x03DB:
		{
			// set_onscreen_counter_background_colour %1d% %2d% %3d% %4d%
			CollectParameters(4);
			CUserDisplay::OnscnTimer.SetShadowColourOverride(scriptParams[0].bParam, scriptParams[1].bParam, scriptParams[2].bParam, scriptParams[3].bParam);
			return;
		}
	case 0x03DD:
		{
			// set_onscreen_counter_show_dollar %1d% %2d%
			int*		pCounterVar = static_cast<int*>(GetPointerToScriptVariable());

			CollectParameters(1);
			CUserDisplay::OnscnTimer.SetDollarFormat(pCounterVar, scriptParams[0].bParam != false);
			return;
		}
	case 0x03DF:
		{
			// all_random_peds %1d%
			CollectParameters(1);
			*CPopulation__allRandomPedThisType = scriptParams[0].dwParam;
			LogToFile("Opcode 03DF called with params: %d", scriptParams[0]);
			return;
		}
	case 0x051F:
		{
			// get_car %1d% last_damage_vehicle %2d%
			CollectParameters(1);
			signed int	paramCopy = scriptParams[0].iParam;
			CVehicle*	damagedVehicle = CPools::GetVehiclePool()->GetAt(paramCopy);
			if ( damagedVehicle )
			{
				CEntity*	damagingVehicle = damagedVehicle->GetDamagingEntity();
				if ( damagingVehicle && damagingVehicle->nType == 2 )
					scriptParams[0].iParam = CPools::GetVehiclePool()->GetHandle(static_cast<CVehicle*>(damagingVehicle));
				else
					scriptParams[0].iParam = -1;
			}
			else
				scriptParams[0].iParam = -1;
			StoreParameters(1);
//			LogToFile("Opcode 051F called with params: %d = %d", paramCopy, scriptParams[0]);
			return;
		}
	case 0x0520:
		{
			// calc_screen_coors %1d% %2d% %3d% %4d% %5d%
			CollectParameters(3);
			const CVector	inputVector(scriptParams[0].fParam, scriptParams[1].fParam, scriptParams[2].fParam);
			CVector			outputVector;

			CalcScreenCoors(inputVector, &outputVector);
			scriptParams[0].fParam = outputVector.x * 640.0f / RsGlobal.MaximumWidth;
			scriptParams[1].fParam = outputVector.y * 448.0f / RsGlobal.MaximumHeight;

			StoreParameters(2);
			return;
		}
	case 0x0521:
		{
			// get_game_metrics %1d% %2d%
			scriptParams[0].fParam = WidescreenSupport::GetScreenWidthMultiplier();
			scriptParams[1].fParam = WidescreenSupport::GetScreenHeightMultiplier();

			StoreParameters(2);
			return;
		}
	case 0x052E:
		{
			//   any_weapons_to_return
			LogToFile("Opcode 052E called");
			UpdateCompareFlag( weaponsToReturn.GetAnyWeaponsToReturn() );
			return;
		}
	case 0x052F:
		{
			// return_player_weapons
			LogToFile("Opcode 052F called");
			weaponsToReturn.ReturnConfiscatedWeapons(CWorld::Players[0]);
			return;
		}
	case 0x690:
		{
			// get_empire_building_data %1d% %2d%
			CollectParameters(1);

			CBuilding*		pEmpireBuilding = CEmpireManager::GetEmpire(scriptParams[0].iParam)->GetBuilding();
			scriptParams[0].iParam = pEmpireBuilding /*&& pEmpireBuilding->GetMatrix()*/  /*pEmpireBuilding->m_pRwObject*/ ? CPools::GetBuildingPool()->GetHandle(pEmpireBuilding) : -1;

			StoreParameters(1);
			return;
		}
	case 0x691:
		{
			// get_number_of_building_data_entries %1d% %2d% %3d%
			CollectParameters(2);

			CBuilding*		pEmpireBuilding = CPools::GetBuildingPool()->GetAt(scriptParams[0].iParam);
			if ( pEmpireBuilding )
			{
				auto*	pModelInfo = CModelInfo::ms_modelInfoPtrs[pEmpireBuilding->GetModelIndex()]->AsAtomicModelInfoPtr();
				if ( pModelInfo )
				{
					auto*		pData = pModelInfo->GetEmpireData();
					scriptParams[0].iParam = pData ? pData->GetNumEntriesOfType(scriptParams[1].wParam) : 0;
				}
				else
					scriptParams[0].iParam = 0;
			}
			else
				scriptParams[0].iParam = 0;

			StoreParameters(1);
			return;
		}
	case 0x692:
		{
			// retrieve_building_data %1d% %2d% %3d% %4d% %5d% %6d% %7d%
			CollectParameters(3);

			CBuilding*		pEmpireBuilding = CPools::GetBuildingPool()->GetAt(scriptParams[0].iParam);
			if ( pEmpireBuilding )
			{
				auto*	pModelInfo = CModelInfo::ms_modelInfoPtrs[pEmpireBuilding->GetModelIndex()]->AsAtomicModelInfoPtr();
				if ( pModelInfo )
				{
					auto*		pData = pModelInfo->GetEmpireData();
					if ( pData )
					{
						// Make the data relative to building coords
						auto&				TheData = pData->GetData(scriptParams[1].wParam, scriptParams[2].bParam);
						CVector				vecOut;
						
						CMatrix*	pBuildingMatrix = pEmpireBuilding->GetMatrix();
						float		fBuildingHeading = pEmpireBuilding->GetHeading();


						if ( pBuildingMatrix )
							vecOut = *pBuildingMatrix * TheData.m_translate;
						else
						{
							vecOut.x = TheData.m_translate.x * cos(fBuildingHeading) - TheData.m_translate.y * sin(fBuildingHeading) + pEmpireBuilding->GetTransform().m_translate.x;
							vecOut.y = TheData.m_translate.x * sin(fBuildingHeading) + TheData.m_translate.y * cos(fBuildingHeading) + pEmpireBuilding->GetTransform().m_translate.y;
							vecOut.z = TheData.m_translate.z + pEmpireBuilding->GetTransform().m_translate.z;
						}

						//TheData.m_heading += fBuildingHeading * RAD_TO_DEG;

						//TheData.m_translate = *pEmpireBuilding->GetMatrix() * TheData.m_translate;
						//TheData.m_heading += pEmpireBuilding->GetHeading() * RAD_TO_DEG;

						// Store it
						scriptParams[0].fParam = vecOut.x;
						scriptParams[1].fParam = vecOut.y;
						scriptParams[2].fParam = vecOut.z;
						scriptParams[3].fParam = TheData.m_heading + static_cast<float>(pEmpireBuilding->GetHeading() * RAD_TO_DEG);
					}
				}
			}

			StoreParameters(4);
			return;
		}
	case 0x693:
		{
			// get_number_of_building_data_entries_subgroup %1d% %2d% %3d% %4g% %5d%
			char			cSubgroup[8];
			CollectParameters(3);
			GetStringParam(cSubgroup, 8);

			CBuilding*		pEmpireBuilding = CPools::GetBuildingPool()->GetAt(scriptParams[0].iParam);
			if ( pEmpireBuilding )
			{
				auto*	pModelInfo = CModelInfo::ms_modelInfoPtrs[pEmpireBuilding->GetModelIndex()]->AsAtomicModelInfoPtr();
				if ( pModelInfo )
				{
					auto*		pData = pModelInfo->GetEmpireData();
					scriptParams[0].iParam = pData ? pData->GetNumEntriesOfType(scriptParams[1].wParam, static_cast<char>(tolower(cSubgroup[0]))) : 0;
				}
				else
					scriptParams[0].iParam = 0;
			}
			else
				scriptParams[0].iParam = 0;

			StoreParameters(1);
			return;
		}
	case 0x694:
		{
			// retrieve_building_data_subgroup %1d% %2d% %3d% %4d% %5g% %6d% %7d% %8d% %9d%
			char			cSubgroup[8];
			CollectParameters(4);
			GetStringParam(cSubgroup, 8);

			CBuilding*		pEmpireBuilding = CPools::GetBuildingPool()->GetAt(scriptParams[0].iParam);
			if ( pEmpireBuilding )
			{
				auto*	pModelInfo = CModelInfo::ms_modelInfoPtrs[pEmpireBuilding->GetModelIndex()]->AsAtomicModelInfoPtr();
				if ( pModelInfo )
				{
					auto*		pData = pModelInfo->GetEmpireData();
					if ( pData )
					{
						// Make the data relative to building coords
						auto&				TheData = pData->GetData(scriptParams[1].wParam, static_cast<char>(tolower(cSubgroup[0])), scriptParams[2].bParam);
						CVector				vecOut;
						
						CMatrix*	pBuildingMatrix = pEmpireBuilding->GetMatrix();
						float		fBuildingHeading = pEmpireBuilding->GetHeading();


						if ( pBuildingMatrix )
							vecOut = *pBuildingMatrix * TheData.m_translate;
						else
						{
							vecOut.x = TheData.m_translate.x * cos(fBuildingHeading) - TheData.m_translate.y * sin(fBuildingHeading) + pEmpireBuilding->GetTransform().m_translate.x;
							vecOut.y = TheData.m_translate.x * sin(fBuildingHeading) + TheData.m_translate.y * cos(fBuildingHeading) + pEmpireBuilding->GetTransform().m_translate.y;
							vecOut.z = TheData.m_translate.z + pEmpireBuilding->GetTransform().m_translate.z;
						}

						//TheData.m_heading += fBuildingHeading * RAD_TO_DEG;

						//TheData.m_translate = *pEmpireBuilding->GetMatrix() * TheData.m_translate;
						//TheData.m_heading += pEmpireBuilding->GetHeading() * RAD_TO_DEG;

						// Store it
						scriptParams[0].fParam = vecOut.x;
						scriptParams[1].fParam = vecOut.y;
						scriptParams[2].fParam = vecOut.z;
						scriptParams[3].fParam = TheData.m_heading + static_cast<float>(pEmpireBuilding->GetHeading() * RAD_TO_DEG);
					}
				}
			}

			StoreParameters(4);
			return;
		}
	case 0x695:
		{
			// get_empire_coords %1d% %2d% %3d% %4d%
			CollectParameters(1);

			CVector*	pEmpirePos = &CEmpireManager::GetEmpire(scriptParams[0].iParam)->GetTransform()->m_translate;
			scriptParams[0].fParam = pEmpirePos->x;
			scriptParams[1].fParam = pEmpirePos->y;
			scriptParams[2].fParam = pEmpirePos->z;

			StoreParameters(3);
			return;
		}
	case 0x696:
		{
			// get_empire_heading %1d% %2d%
			CollectParameters(1);

			scriptParams[0].fParam = CEmpireManager::GetEmpire(scriptParams[0].iParam)->GetTransform()->m_heading;

			StoreParameters(1);
			return;
		}
	case 0x069C:
		{
			// set_empire_ownership %1d% %2d%
			CollectParameters(2);
			// TODO: Do
			return;
		}
	case 0x069D:
		{
			// set_empire_type %1d% %2d%
			CollectParameters(2);
			// TODO: More needed?
			CEmpireManager::GetEmpire(scriptParams[0].iParam)->SetType(scriptParams[1].iParam);
			return;
		}
	case 0x069E:
		{
			// set_empire_scale %1d% %2d%
			CollectParameters(2);
			// TODO: More needed?
			CEmpireManager::GetEmpire(scriptParams[0].iParam)->SetScale(scriptParams[1].iParam-1);
			return;
		}
	case 0x069F:
		{
			// set_empire_condition %1d% %2d%
			CollectParameters(2);
			// TODO: Do
			return;
		}
	case 0x06A0:
		{
			// set_empire_visually_damaged %1d% %2d%
			CollectParameters(2);
			// TODO: Do
			return;
		}
	case 0x06A1:
		{
			// set_empire_state %1d% %2d%
			CollectParameters(2);
			// TODO: State setting?
			CEmpire*	pEmpire = CEmpireManager::GetEmpire(scriptParams[0].iParam);

			// Refresh the building
			if ( pEmpire->IsPlaced() )
				pEmpire->Remove();

			pEmpire->Place();
			return;
		}
	case 0x06BA:
		{
			// task_turn_char_to_face_coord %1d% %2d% %3d% %4d%
			CollectParameters(4);

			CPed*		pThePed = CPools::GetPedPool()->GetAt(scriptParams[0].iParam);
			CVector		vecFaceVector = CVector(scriptParams[1].fParam, scriptParams[2].fParam) - *pThePed->GetCoords();
			float		fHeading = atan2(-vecFaceVector.x, vecFaceVector.y);

			pThePed->SetCurrentHeading(fHeading);
			pThePed->SetTargetHeading(fHeading);

			pThePed->SetHeading(fHeading);
			pThePed->UpdateRW();
			return;
		}
	case 0x06CB:
		{
			// set_building_group_model %1g% %2d%
			char	cGroupName[16];

			GetStringParam(cGroupName, 16);
			CollectParameters(1);

			int				nNumGroups = CGroupedBuildings::GetNumGroups();
			unsigned int	nHash = HashHelper.FullCRC(reinterpret_cast<unsigned char*>(cGroupName), strlen(cGroupName));

			for ( int i = 0; i < nNumGroups; i++ )
			{
				if ( nHash == CGroupedBuildings::GetGroup(i)->GetHash() )
				{
					CGroupedBuildings::GetGroup(i)->SetActiveBuilding(scriptParams[0].iParam);
					return;
				}
			}
			return;
		}
	case 0x06CC:
		{
			// get_building_group_data %1d% %2d%
			char	cGroupName[16];

			GetStringParam(cGroupName, 16);

			int				nNumGroups = CGroupedBuildings::GetNumGroups();
			unsigned int	nHash = HashHelper.FullCRC(reinterpret_cast<unsigned char*>(cGroupName), strlen(cGroupName));

			for ( int i = 0; i < nNumGroups; i++ )
			{
				if ( nHash == CGroupedBuildings::GetGroup(i)->GetHash() )
				{
					scriptParams[0].iParam = CPools::GetBuildingPool()->GetHandle(CGroupedBuildings::GetGroup(i)->GetBuilding());
					StoreParameters(1);
					return;
				}
			}
			return;
		}
	case 0x0821:
		{
			//   is_this_model_a_bike %1d%
			CollectParameters(1);
			UpdateCompareFlag( CModelInfo::IsBikeModel(scriptParams[0].iParam) );
			return;
		}
	}
}

void CScriptFunction::Init(CRunningScript* parent)
{
	parentScript = parent;
//	bApplyWidescreenFixOnThisScriptDraws = false;
	memset(bAmountOfVariablesWePassed, -1, sizeof(bAmountOfVariablesWePassed));
	
	BYTE	loopCounter = GOSUB_STACK_SIZE;
	do
	{
		--loopCounter;
//		bAmountOfVariablesWePassed[loopCounter] = -1;
		if ( savedVariablesSpace[loopCounter] )
			memset(savedVariablesSpace[loopCounter], 0, sizeof(savedVariablesSpace));
/*			delete[] savedVariablesSpace[loopCounter];

		savedVariablesSpace[loopCounter] = NULL;*/
	}
	while ( loopCounter );
}

void CScriptFunction::SaveAllFunctions()
{
	WORD				activeFunctions = 0;
	CRunningScript*		scriptsCtr = *pActiveScripts;
	CRunningScript*		lastActiveScript = nullptr;

	if ( scriptsCtr )
	{
		do
		{
			if ( !scriptsCtr->bIsExternal && scriptsCtr->extrnAttachType == -1 )
				++activeFunctions;
			lastActiveScript = scriptsCtr;
			scriptsCtr = scriptsCtr->Previous;
		}
		while ( scriptsCtr );
	}
	C_PcSave::PcClassSaveRoutine(&activeFunctions, sizeof(WORD));
	
	if ( activeFunctions )
	{
		do
		{
			unsigned short		scriptIndex = static_cast<unsigned short>(lastActiveScript - ScriptsArray);
			CScriptFunction&	function = CRunningScript::ms_scriptFunction[scriptIndex];

			if ( !lastActiveScript->bIsExternal && lastActiveScript->extrnAttachType == -1 )
			{
				C_PcSave::PcClassSaveRoutine(&scriptIndex, sizeof(unsigned short));
				for ( BYTE j = 0; j < GOSUB_STACK_SIZE; ++j )
				{
					signed char amountOfParams = function.bAmountOfVariablesWePassed[j];
					C_PcSave::PcClassSaveRoutine(&amountOfParams, sizeof(signed char));
					if ( amountOfParams != -1 )
					{
						C_PcSave::PcClassSaveRoutine(&function.bNOTFlagState[j], sizeof(bool));
						C_PcSave::PcClassSaveRoutine(function.savedVariablesSpace[j], sizeof(SCRIPT_VAR) * 32);
					}
				}
			}
			lastActiveScript = lastActiveScript->Next;
		}
		while ( lastActiveScript );
	}
	
	/*for ( BYTE i = 0; i < NUM_SCRIPTS; ++i )
	{
		CScriptFunction&	function = scriptFunctions[i];
		if ( function.parentScript->bIsActive )
		{
			WORD scriptIndex = ( (int)function.parentScript - (int)ScriptsArray ) / sizeof(CRunningScript);
			C_PcSave::PcClassSaveRoutine(&scriptIndex, sizeof(WORD));
			//C_PcSave::PcClassSaveRoutine(&function.parentScript, sizeof(CRunningScript*));
			for ( BYTE j = 0; j < GOSUB_STACK_SIZE; ++j )
			{
				signed char amountOfParams = function.bAmountOfVariablesWePassed[j];
				C_PcSave::PcClassSaveRoutine(&amountOfParams, sizeof(signed char));
				if ( amountOfParams != -1 )
					C_PcSave::PcClassSaveRoutine(function.savedVariablesSpace[j], sizeof(SCRIPT_VAR) * 32);
				/*bool bThisIsCall = function.bThisIndirectionLevelIsCall[j];
				C_PcSave::PcClassSaveRoutine(&bThisIsCall, sizeof(bool));
				if ( bThisIsCall )
				{
					C_PcSave::PcClassSaveRoutine(&function.bAmountOfVariablesWePassed[j], sizeof(BYTE));
					C_PcSave::PcClassSaveRoutine(function.savedVariablesSpace[j], sizeof(SCRIPT_VAR) * 32);
				}
			}
		}
	}*/
}

void CScriptFunction::LoadAllFunctions()
{
	WORD		activeFunctions;
	C_PcSave::PcClassLoadRoutine(&activeFunctions, sizeof(WORD));

	do
	{
		--activeFunctions;
		WORD				scriptIndex;
		C_PcSave::PcClassLoadRoutine(&scriptIndex, sizeof(WORD));
		CScriptFunction&	function = CRunningScript::ms_scriptFunction[scriptIndex];

		function.parentScript = scriptIndex + ScriptsArray;
		for ( BYTE j = 0; j < GOSUB_STACK_SIZE; ++j )
		{
			signed char amountOfParams;
			C_PcSave::PcClassLoadRoutine(&amountOfParams, sizeof(signed char));
			function.bAmountOfVariablesWePassed[j] = amountOfParams;
			if ( amountOfParams != -1 )
			{
				if ( !function.savedVariablesSpace[j] )
					function.savedVariablesSpace[j] = new SCRIPT_VAR[32];

				C_PcSave::PcClassLoadRoutine(&function.bNOTFlagState[j], sizeof(bool));
				C_PcSave::PcClassLoadRoutine(function.savedVariablesSpace[j], sizeof(SCRIPT_VAR) * 32);
			}
		}
	}
	while ( activeFunctions );

	/*	for ( BYTE i = 0; i < activeFunctions; ++i )
	{
		CScriptFunction&	function = scriptFunctions[i];
		WORD	scriptIndex;
		C_PcSave::PcClassLoadRoutine(&scriptIndex, sizeof(WORD));
		function.parentScript = (CRunningScript*)(scriptIndex * sizeof(CRunningScript) + (int)ScriptsArray);
		//C_PcSave::PcClassLoadRoutine(&function.parentScript, sizeof(CRunningScript*));
		for ( BYTE j = 0; j < GOSUB_STACK_SIZE; ++j )
		{
			signed char amountOfParams;
			C_PcSave::PcClassLoadRoutine(&amountOfParams, sizeof(signed char));
			function.bAmountOfVariablesWePassed[j] = amountOfParams;
			if ( amountOfParams != -1 )
			{
				if ( !function.savedVariablesSpace[j] )
					function.savedVariablesSpace[j] = new SCRIPT_VAR[32];

				C_PcSave::PcClassLoadRoutine(&function.savedVariablesSpace[j], sizeof(SCRIPT_VAR) * 32);
			}
			/*bool	bThisIsCall;
			C_PcSave::PcClassLoadRoutine(&bThisIsCall, sizeof(BYTE));
			if ( bThisIsCall )
			{
				function.bThisIndirectionLevelIsCall[j] = bThisIsCall;
				C_PcSave::PcClassLoadRoutine(&function.bAmountOfVariablesWePassed[j], sizeof(BYTE));
				if ( !function.savedVariablesSpace[j] )
					function.savedVariablesSpace[j] = new SCRIPT_VAR[32];

				C_PcSave::PcClassLoadRoutine(&function.savedVariablesSpace[j], sizeof(SCRIPT_VAR) * 32);
			}
		}
	}*/
}

/*static CMappedVariable SaveVariable(long nIndex, long nSize = 1)
{
	CMappedVariable	cVar(nSize);

	*reinterpret_cast<int*>(cVar.AccessData()) = (reinterpret_cast<int*>(scmBlock))[nIndex];
	return cVar;
}*/

#define HashVariable(str) HashHelper.FullCRC(reinterpret_cast<unsigned char*>(str), sizeof(str)-1)

static void SaveVariable(char* pVarName, long nIndex, long nArraySize = 1)
{
	long	nHash = HashHelper.FullCRC(reinterpret_cast<unsigned char*>(pVarName), strlen(pVarName));

	C_PcSave::PcClassSaveRoutine(&nHash, sizeof(long));
	C_PcSave::PcClassSaveRoutine(&nArraySize, sizeof(char));
	C_PcSave::PcClassSaveRoutine(&reinterpret_cast<int*>(scmBlock)[nIndex], sizeof(int) * nArraySize);
}

void CTheScripts::Init()
{
	TheScriptsInitFunc();
}

void CTheScripts::SaveAllScripts()
{
	#include "SaveVars.inc"
}

void CTheScripts::LoadAllScripts()
{
	std::map<long,short>	cGlobalVarsMap;
	int						nSavedVars;

	#include "LoadVars.inc"

	Init();

	// Load variables
	C_PcSave::PcClassLoadRoutine(&nSavedVars, sizeof(int));
	for ( int i = 0; i < nSavedVars; ++i )
	{
		long											nHash;
		unsigned char									bVarSize;

		C_PcSave::PcClassLoadRoutine(&nHash, sizeof(long));
		C_PcSave::PcClassLoadRoutine(&bVarSize, sizeof(char));

		auto item = cGlobalVarsMap.find(nHash);
		if ( item != cGlobalVarsMap.end() )
			C_PcSave::PcClassLoadRoutine(&reinterpret_cast<int*>(scmBlock)[item->second], sizeof(int) * bVarSize);	
		else
		{
			// Just skip
			int*	pTempSpace = new int[bVarSize];
			C_PcSave::PcClassLoadRoutine(pTempSpace, sizeof(int) * bVarSize);
			delete[] pTempSpace;
		}
	}

	StartTestScript();
}

void CTheScripts::StartTestScript()
{
	StartNewScript(scmBlock);
}

void CTheScripts::ZeroMissionVars()
{
	unsigned long	dwNumOfVars = (*reinterpret_cast<unsigned long*>(static_cast<unsigned char*>(scmBlock)+3)) / 4;

	memset(&reinterpret_cast<int*>(scmBlock)[HIGHEST_VAR_USED+1], 0, (dwNumOfVars - HIGHEST_VAR_USED) * 4 - 4);
}