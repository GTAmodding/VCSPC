#include "StdAfx.h"
#include "Script.h"

#include "General.h"
#include "Vehicle.h"
#include "CConfiscatedWeapons.h"
#include "Text.h"
#include "Camera.h"
#include "Radar.h"
#include "UserDisplay.h"
#include "Streaming.h"
#include "World.h"
#include "Pools.h"
#include "EmpireMgr.h"
#include "PcSave.h"
#include "GroupedBuildings.h"
#include "DLCManager.h"
#include "Pad.h"
#include "ControlsMgr.h"

CScriptFunction		CRunningScript::ms_scriptFunction[NUM_SCRIPTS];

int16 &CTheScripts::NumberOfScriptSearchLights = *(short*)0xA90830;

// Wrappers
WRAPPER bool CTheScripts::IsPlayerOnAMission() { EAXJMP(0x464D50); }
WRAPPER void CTheScripts::Process() { EAXJMP(0x46A000); }
WRAPPER CRunningScript* CTheScripts::StartNewScript(void* nScriptIP) { WRAPARG(nScriptIP); EAXJMP(0x464C20); }
WRAPPER void CTheScripts::RenderAllSearchLights(void) { EAXJMP(0x493E30); }
WRAPPER void CRunningScript::StoreParameters(int16 numParams) { WRAPARG(numParams); EAXJMP(0x464370); }
WRAPPER void CRunningScript::UpdateCompareFlag(bool result) { WRAPARG(result); EAXJMP(0x4859D0); }
WRAPPER unsigned short CRunningScript::GetGlobalVarOffset() { EAXJMP(0x464700); }
WRAPPER void* CRunningScript::GetPointerToScriptVariable(int32 nParam = 0) { WRAPARG(nParam); EAXJMP(0x464790); }
WRAPPER void CRunningScript::SetIP(int32 IP) { WRAPARG(IP); EAXJMP(0x464DA0); }


void CRunningScript::Init()
{
	((void(__thiscall*)(CRunningScript*))0x4648E0)(this);

	ms_scriptFunction[this - ScriptsArray].Init(this);
}

CScriptFunction& CRunningScript::GetExtrasForScript()
{
	return ms_scriptFunction[this - ScriptsArray];
}

signed char CRunningScript::CollectParametersForScriptFunction(int16 numParams)
{
	int8 bOccupiedVars = 0;
	SCRIPT_VAR*		currentVar = scriptParams;
	for ( ; numParams != 0; --numParams, ++currentVar )
	{
		eOperandType operand = ReadVariable<eOperandType>();
		
		switch ( operand )
		{
		case imm32f:
		case imm32:
			{
				int32 val = ReadVariable<int32>();
				currentVar->iParam = val;
				++bOccupiedVars;
				break;
			}
		case globalVar:
			{
				uint16 varIndex = ReadVariable<uint16>();
				*currentVar = *(SCRIPT_VAR*)(scmBlock + varIndex);
				++bOccupiedVars;
				break;
			}
		case localVar:
			{
				uint16 varIndex = ReadVariable<uint16>();
				*currentVar = *GetPointerToLocalVariable( varIndex );
				++bOccupiedVars;
				break;
			}
		case imm8:
			{
				int32 value = ReadVariable<int8>();
				currentVar->iParam = value;
				++bOccupiedVars;
				break;
			}
		case imm16:
			{
				int32 value = ReadVariable<int16>();
				currentVar->iParam = value;
				++bOccupiedVars;
				break;
			}
		case globalArr:
			{
				uint16 variable;
				int32 index;

				ReadArrayInformation( 1, &variable, &index );
				*currentVar = *(SCRIPT_VAR*)(scmBlock + (index*4) + variable);
				++bOccupiedVars;
				break;
			}
		case localArr:
			{
				uint16 variable;
				int32 index;

				ReadArrayInformation( 1, &variable, &index );
				*currentVar = *GetPointerToLocalArrayElement( variable, index, 1 );
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
				CurrentIP--;
				uint8 len = ( operand == sstring || operand == globalVarSString || operand == localVarSString
										|| operand == globalVarSArrString || operand == localVarSArrString ) ? 2 : 4;
				ReadTextLabelFromScript((char*)currentVar, len);
				currentVar += (len - 1);

				bOccupiedVars += len;
			}
		}
	}

	return bOccupiedVars;
}

void CRunningScript::CollectParameters(int16 numParams)
{
	SCRIPT_VAR*		currentVar = scriptParams;
	for ( ; numParams != 0; --numParams, ++currentVar )
	{
		eOperandType operand = ReadVariable<eOperandType>();
		
		switch ( operand )
		{
		case imm32f:
		case imm32:
			{
				int32 value = ReadVariable<int32>();
				currentVar->iParam = value;
				break;
			}
		case globalVar:
			{
				uint16 varIndex = ReadVariable<uint16>();
				*currentVar = *(SCRIPT_VAR*)(scmBlock + varIndex);
				break;
			}
		case localVar:
			{
				uint16 varIndex = ReadVariable<uint16>();
				*currentVar = *GetPointerToLocalVariable( varIndex );
				break;
			}
		case imm8:
			{
				int32 value = ReadVariable<int8>();
				currentVar->iParam = value;
				break;
			}
		case imm16:
			{
				int32 value = ReadVariable<int16>();
				currentVar->iParam = value;
				break;
			}
		case globalArr:
			{
				uint16 variable;
				int32 index;

				ReadArrayInformation( 1, &variable, &index );
				*currentVar = *(SCRIPT_VAR*)(scmBlock + (index*4) + variable);
				break;
			}
		case localArr:
			{
				uint16 variable;
				int32 index;

				ReadArrayInformation( 1, &variable, &index );
				*currentVar = *GetPointerToLocalArrayElement( variable, index, 1 );
				break;
			}
		case sstring:
			{
				currentVar->pcParam = (char*)CurrentIP;
				CurrentIP += 8;
				break;
			}
		case lstring:
			{
				currentVar->pcParam = (char*)CurrentIP;
				CurrentIP += 16;
				break;
			}
		default:
			{
				assert(!"Unsupported variable argument in CRunningScript::CollectParameters");
				break;
			}
		}
	}
}

void CRunningScript::ReadTextLabelFromScript(char* textPtr, uint8 len)
{
	eOperandType operand = ReadVariable<eOperandType>();
		
	switch ( operand )
	{
	case sstring:
		{
			strncpy( textPtr, (char*)CurrentIP, 8 );
			CurrentIP += 8;
			break;
		}
	case globalVarSString:
		{
			uint16 varIndex = ReadVariable<uint16>();
			strncpy( textPtr, (char*)scmBlock + varIndex, 8 );
			break;
		}
	case localVarSString:
		{
			uint16 varIndex = ReadVariable<uint16>();
			strncpy( textPtr, (char*)GetPointerToLocalVariable( varIndex ), 8 );
			break;
		}
	case globalVarSArrString:
		{
			uint16 variable;
			int32 index;

			ReadArrayInformation( 1, &variable, &index );
			strncpy( textPtr, (char*)scmBlock + (8*index) + variable, 8 );
			break;
		}
	case localVarSArrString:
		{
			uint16 variable;
			int32 index;

			ReadArrayInformation( 1, &variable, &index );
			strncpy( textPtr, (char*)GetPointerToLocalArrayElement( variable, index, 2 ), 8 );
			break;
		}
	case globalVarVArrString:
		{
			uint16 variable;
			int32 index;

			ReadArrayInformation( 1, &variable, &index );
			strncpy( textPtr, (char*)scmBlock + (16*index) + variable, Min<uint8>( len, 16 ) );
			break;
		}
	case localVarVArrString:
		{
			uint16 variable;
			int32 index;

			ReadArrayInformation( 1, &variable, &index );
			strncpy( textPtr, (char*)GetPointerToLocalArrayElement( variable, index, 4 ), Min<uint8>( len, 16 ) );
			break;
		}
	case vstring:
		{
			uint8 length = ReadVariable<uint8>();
			memcpy( textPtr, CurrentIP, length );
			CurrentIP += length;
			if ( length < len )
				memset( textPtr+length, 0, len-length );
			break;
		}
	case lstring:
		{
			if ( len == 0 )
				break;

			memcpy( textPtr, CurrentIP, Min<uint8>( len, 16 ) );
			CurrentIP += 16;
			break;
		}
	case globalVarVString:
		{
			uint16 varIndex = ReadVariable<uint16>();
			strncpy( textPtr, (char*)scmBlock + varIndex, Min<uint8>( len, 16 ) );
			break;
		}
	case localVarVString:
		{
			uint16 varIndex = ReadVariable<uint16>();
			strncpy( textPtr, (char*)GetPointerToLocalVariable( varIndex ), Min<uint8>( len, 16 ) );
			break;
		}

	case globalVar:
		{
			uint16 varIndex = ReadVariable<uint16>();
			strncpy( textPtr, ((SCRIPT_VAR*)(scmBlock + varIndex))->pcParam, 8 );
			break;
		}
	case localVar:
		{
			uint16 varIndex = ReadVariable<uint16>();
			strncpy( textPtr, GetPointerToLocalVariable( varIndex )->pcParam, 8 );
			break;
		}
	case globalArr:
		{
			uint16 variable;
			int32 index;

			ReadArrayInformation( 1, &variable, &index );
			strncpy( textPtr, ((SCRIPT_VAR*)(scmBlock + (index*4) + variable))->pcParam, 8 );
			break;
		}
	case localArr:
		{
			uint16 variable;
			int32 index;

			ReadArrayInformation( 1, &variable, &index );
			strncpy( textPtr,  GetPointerToLocalArrayElement( variable, index, 1 )->pcParam, 8 );
			break;
		}
	default:
		{
			assert(!"Unsupported variable argument in CRunningScript::ReadTextLabelFromScript");
			break;
		}
	}
}

void CRunningScript::StoreParametersFromScriptFunction()
{
	SCRIPT_VAR*		currentVar = scriptParams;

	while ( eOperandType operand = ReadVariable<eOperandType>() )
	{
		switch ( operand )
		{
		case globalVar:
			{
				uint16 varIndex = ReadVariable<uint16>();
				*(SCRIPT_VAR*)(scmBlock + varIndex) = *currentVar;
				break;
			}
		case localVar:
			{
				uint16 varIndex = ReadVariable<uint16>();
				*GetPointerToLocalVariable( varIndex ) = *currentVar;
				break;
			}
		case globalArr:
			{
				uint16 variable;
				int32 index;

				ReadArrayInformation( 1, &variable, &index );
				*reinterpret_cast<SCRIPT_VAR*>(scmBlock + 4 * index + variable) = *currentVar;
				break;
			}
		case localArr:
			{
				uint16 variable;
				int32 index;

				ReadArrayInformation( 1, &variable, &index );
				*GetPointerToLocalArrayElement( variable, index, 1 ) = *currentVar;
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
				CurrentIP--;
				uint8 len = ( operand == globalVarSString || operand == localVarSString
										|| operand == globalVarSArrString || operand == localVarSArrString ) ? 2 : 4;
			
				strncpy(static_cast<char*>(GetPointerToScriptVariable()), (char*)currentVar, len * 4);
				currentVar += (len-1);
			}
		}
		++currentVar;
	}
}

void CRunningScript::ReadArrayInformation( int32 move, uint16* varOffset, int32* varIndex )
{
	uint8* tempIP = CurrentIP;

	uint16 arrayVarIndex = *(uint16*)tempIP;
	tempIP += 2;
	int32 index;
	*varOffset = arrayVarIndex;

	uint16 arrayIndex = *(uint16*)tempIP;
	tempIP += 2;
	uint8 arraySize = *(uint8*)tempIP++;
	ArrayProperties arrayProperties = *(ArrayProperties*)tempIP++;

	if ( !arrayProperties.m_bIsIndexGlobalVariable )
	{
		index = bIsMission ? scriptLocals[arrayIndex].iParam : LocalVar[arrayIndex].iParam;
	}
	else
	{
		index = *(int32*)(scmBlock + arrayIndex);
	}
	*varIndex = index;

	if ( move )
		CurrentIP = tempIP;
}

SCRIPT_VAR* CRunningScript::GetPointerToLocalArrayElement( uint16 offset, int32 index, uint8 size )
{
	uint32 fullOffset = offset + index*size;
	return bIsMission ? &scriptLocals[fullOffset] : &LocalVar[fullOffset];
}

SCRIPT_VAR* CRunningScript::GetPointerToLocalVariable( uint16 var )
{
	return bIsMission ? &scriptLocals[var] : &LocalVar[var];
}

int8 CRunningScript::ProcessCustomCommandsLow( int16 command )
{
	switch ( command )
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
			return 0;
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

				if ( scriptExtras.bAmountOfVariablesWePassed[SP] != -1 )
				{
					if ( !bIsMission )
						memcpy(scriptParams, &LocalVar[scriptExtras.bAmountOfVariablesWePassed[SP]], 32 * sizeof(SCRIPT_VAR));
					else
						memcpy(scriptParams, &scriptLocals[scriptExtras.bAmountOfVariablesWePassed[SP]], 32 * sizeof(SCRIPT_VAR));

					scriptExtras.PopCall(this);
					StoreParametersFromScriptFunction();
				}
			}


			if ( command != 0x0051 )
				UpdateCompareFlag(command == 0x0054);
			return 0;
		}
	case 0x014D:
		{
			// add_pager_message %1g% %2d% %3d%
			char	cString[8];
			ReadTextLabelFromScript(cString, 8);
			CollectParameters(2);
			CUserDisplay::Pager.AddMessage(const_cast<char*>(TheText.Get(cString)), static_cast<short>(scriptParams[0].iParam), static_cast<short>(scriptParams[1].iParam));
			return 0;
		}

	case 0x0166:
		{
			// dim_blip %1d% %2d%
			CollectParameters(2);
			CRadar::ChangeBlipBrightness(scriptParams[0].iParam, scriptParams[1].iParam);
			//LogToFile("Opcode 0166 called with params: %d %d", scriptParams[0], scriptParams[1]);
			return 0;
		}
	case GET_CONTROLLER_MODE:
		{
			// %1d% = get_controller_mode
			scriptParams[0].iParam = CPad::GetPad(0)->Mode;
			StoreParameters(1);
			return 0;
		}
	case 0x0351:
		{	
			// dont_touch_fov_in_widescreen %1d%
			CollectParameters(1);
			CCamera::bDontTouchFOVInWidescreen = scriptParams[0].bParam != 0;
			LogToFile("Opcode 0351 called with params: %d", scriptParams[0]);
			return 0;
		}
	case 0x0352:
		{
			// enter_amazing_screenshot_mode %1d%
			CollectParameters(1);
#if defined DEVBUILD && !defined MAKE_ZZCOOL_MOVIE_DEMO
			EnterAmazingScreenshotMode(scriptParams[0].bParam != 0);
			LogToFile("Opcode 0352 called with params: %d", scriptParams[0]);
#endif
			return 0;
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
			return 0;
		}
	case 0x0354:
		{
			// change_player %1d% model_to %2o%
			char	cString[8];
			CollectParameters(1);
			ReadTextLabelFromScript(cString, 8);

			ToLower(cString, 8);
			CStreaming::RequestSpecialModel(0, cString, 26);
			CStreaming::LoadAllRequestedModels(true);
			CPed* pPed = CWorld::Players[scriptParams[0].bParam].GetPed();
			int32 iAnimGroupBackup = pPed->GetMoveAnimGroup();
			pPed->DeleteRwObject();
			pPed->ModelIndex() = -1;
			pPed->SetModelIndex(0);
			pPed->SetMoveAnimGroup(iAnimGroupBackup);
			LogToFile("Opcode 0352 called with params: %d %s", scriptParams[0], cString);
			return 0;
		}
	case 0x0355:
		{
			// set_actor %1d% colours_to %2d% %3d% %4d% %5d%
			CollectParameters(5);
			CPools::GetPedPoolAux()->GetAt(scriptParams[0].iParam)->SetColours(scriptParams[1].bParam, scriptParams[2].bParam, scriptParams[3].bParam, scriptParams[4].bParam);
			//CPedEx::pPedData[scriptParams[0].iParam >> 8].SetColours(scriptParams[1].bParam, scriptParams[2].bParam, scriptParams[3].bParam, scriptParams[4].bParam);
			LogToFile("Opcode 0355 called with params: %d %d %d %d %d", scriptParams[0], scriptParams[1], scriptParams[2], scriptParams[3], scriptParams[4]);
			return 0;
		}
	case 0x037A:
		{
			// call_function %1p% num_params %2d% params ...
			assert(SP < GOSUB_STACK_SIZE);
			CollectParameters(2);
			int32			offsetToJmp	= scriptParams[0].iParam;
			int8	passedParams = static_cast<int8>(scriptParams[1].iParam);

			int8 occupiedVars = CollectParametersForScriptFunction(passedParams);
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
			return 0;
		}
	case 0x03C1:
		{
			// display_onscreen_counter_with_two_numbers_place %1d% %2d% %3h% %4h% %5g% %6g%
			int*			pVariable[2];
			char			cString[8], cPlaceString[8];

			pVariable[0] = static_cast<int*>(GetPointerToScriptVariable());
			pVariable[1] = static_cast<int*>(GetPointerToScriptVariable());
			CollectParameters(2);
			ReadTextLabelFromScript(cPlaceString, 8);
			ReadTextLabelFromScript(cString, 8);
			CUserDisplay::OnscnTimer.AddCounterTwoVars(pVariable[0], pVariable[1], cString, scriptParams[1].wParam, cPlaceString, scriptParams[0].wParam);
			//LogToFile("Opcode 03C2 called with params: 0x%08X 0x%08X %d %s", pVariable[0], pVariable[1], scriptParams[0], cString);
			return 0;
		}
	case 0x03C2:
		{
			// display_onscreen_counter_with_two_numbers %1d% %2d% %3h% %4g%
			int*			pVariable[2];
			char			cString[8];

			pVariable[0] = static_cast<int*>(GetPointerToScriptVariable());
			pVariable[1] = static_cast<int*>(GetPointerToScriptVariable());
			CollectParameters(1);
			ReadTextLabelFromScript(cString, 8);
			CUserDisplay::OnscnTimer.AddCounterTwoVars(pVariable[0], pVariable[1], cString, scriptParams[0].wParam);
			//LogToFile("Opcode 03C2 called with params: 0x%08X 0x%08X %d %s", pVariable[0], pVariable[1], scriptParams[0], cString);
			return 0;
		}
	case 0x03DA:
		{
			// set_onscreen_counter_colour %1d% %2d% %3d% %4d%
			CollectParameters(4);
			CUserDisplay::OnscnTimer.SetColourOverride(scriptParams[0].bParam, scriptParams[1].bParam, scriptParams[2].bParam, scriptParams[3].bParam);
			return 0;
		}
	case 0x03DB:
		{
			// set_onscreen_counter_background_colour %1d% %2d% %3d% %4d%
			CollectParameters(4);
			CUserDisplay::OnscnTimer.SetShadowColourOverride(scriptParams[0].bParam, scriptParams[1].bParam, scriptParams[2].bParam, scriptParams[3].bParam);
			return 0;
		}
	case 0x03DD:
		{
			// set_onscreen_counter_show_dollar %1d% %2d%
			int*		pCounterVar = static_cast<int*>(GetPointerToScriptVariable());

			CollectParameters(1);
			CUserDisplay::OnscnTimer.SetDollarFormat(pCounterVar, scriptParams[0].bParam != false);
			return 0;
		}
	case 0x03DF:
		{
			// all_random_peds %1d%
			CollectParameters(1);
			*CPopulation__allRandomPedThisType = scriptParams[0].dwParam;
			LogToFile("Opcode 03DF called with params: %d", scriptParams[0]);
			return 0;
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
			return 0;
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
			return 0;
		}
	case 0x0521:
		{
			// get_game_metrics %1d% %2d%
			scriptParams[0].fParam = WidescreenSupport::GetScreenWidthMultiplier();
			scriptParams[1].fParam = WidescreenSupport::GetScreenHeightMultiplier();

			StoreParameters(2);
			return 0;
		}
	case 0x052D:
		{
			//	is_dlc_active %1d%
			CollectParameters(1);
			UpdateCompareFlag(CDLCManager::GetDLC(scriptParams[0].iParam)->IsEnabled());
			return 0;
		}
	case 0x052E:
		{
			//   any_weapons_to_return
			LogToFile("Opcode 052E called");
			UpdateCompareFlag( weaponsToReturn.GetAnyWeaponsToReturn() );
			return 0;
		}
	case 0x052F:
		{
			// return_player_weapons
			LogToFile("Opcode 052F called");
			weaponsToReturn.ReturnConfiscatedWeapons(CWorld::Players[0]);
			return 0;
		}
	case 0x690:
		{
			// get_empire_building_data %1d% %2d%
			CollectParameters(1);

			CBuilding*		pEmpireBuilding = CEmpireManager::GetEmpire(scriptParams[0].iParam)->GetBuilding();
			scriptParams[0].iParam = pEmpireBuilding /*&& pEmpireBuilding->GetMatrix()*/  /*pEmpireBuilding->m_pRwObject*/ ? CPools::GetBuildingPool()->GetHandle(pEmpireBuilding) : -1;

			StoreParameters(1);
			return 0;
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
			return 0;
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
			return 0;
		}
	case 0x693:
		{
			// get_number_of_building_data_entries_subgroup %1d% %2d% %3d% %4g% %5d%
			char			cSubgroup[8];
			CollectParameters(3);
			ReadTextLabelFromScript(cSubgroup, 8);

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
			return 0;
		}
	case 0x694:
		{
			// retrieve_building_data_subgroup %1d% %2d% %3d% %4d% %5g% %6d% %7d% %8d% %9d%
			char			cSubgroup[8];
			CollectParameters(4);
			ReadTextLabelFromScript(cSubgroup, 8);

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
			return 0;
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
			return 0;
		}
	case 0x696:
		{
			// get_empire_heading %1d% %2d%
			CollectParameters(1);

			scriptParams[0].fParam = CEmpireManager::GetEmpire(scriptParams[0].iParam)->GetTransform()->m_heading;

			StoreParameters(1);
			return 0;
		}
	case 0x069C:
		{
			// set_empire_ownership %1d% %2d%
			CollectParameters(2);
			// TODO: Do
			return 0;
		}
	case 0x069D:
		{
			// set_empire_type %1d% %2d%
			CollectParameters(2);
			// TODO: More needed?
			CEmpireManager::GetEmpire(scriptParams[0].iParam)->SetType(scriptParams[1].iParam);
			return 0;
		}
	case 0x069E:
		{
			// set_empire_scale %1d% %2d%
			CollectParameters(2);
			// TODO: More needed?
			CEmpireManager::GetEmpire(scriptParams[0].iParam)->SetScale(scriptParams[1].iParam-1);
			return 0;
		}
	case 0x069F:
		{
			// set_empire_condition %1d% %2d%
			CollectParameters(2);
			// TODO: Do
			return 0;
		}
	case 0x06A0:
		{
			// set_empire_visually_damaged %1d% %2d%
			CollectParameters(2);
			// TODO: Do
			return 0;
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
			return 0;
		}
	case 0x06BA:
		{
			// task_turn_char_to_face_coord %1d% %2d% %3d% %4d%
			CollectParameters(4);

			CPed*		pThePed = CPools::GetPedPool()->GetAt(scriptParams[0].iParam);
			CVector		vecFaceVector = CVector(scriptParams[1].fParam, scriptParams[2].fParam) - pThePed->GetCoords();
			float		fHeading = atan2(-vecFaceVector.x, vecFaceVector.y);

			pThePed->SetCurrentHeading(fHeading);
			pThePed->SetTargetHeading(fHeading);

			pThePed->SetHeading(fHeading);
			pThePed->UpdateRW();
			return 0;
		}
	case 0x06CB:
		{
			// set_building_group_model %1g% %2d%
			char	cGroupName[16];

			ReadTextLabelFromScript(cGroupName, 16);
			CollectParameters(1);

			int				nNumGroups = CGroupedBuildings::GetNumGroups();
			unsigned int	nHash = HashHelper.FullCRC(reinterpret_cast<unsigned char*>(cGroupName), strlen(cGroupName));

			for ( int i = 0; i < nNumGroups; i++ )
			{
				if ( nHash == CGroupedBuildings::GetGroup(i)->GetHash() )
				{
					CGroupedBuildings::GetGroup(i)->SetActiveBuilding(scriptParams[0].iParam);
					break;
				}
			}
			return 0;
		}
	case 0x06CC:
		{
			// get_building_group_data %1d% %2d%
			char	cGroupName[16];

			ReadTextLabelFromScript(cGroupName, 16);

			int				nNumGroups = CGroupedBuildings::GetNumGroups();
			unsigned int	nHash = HashHelper.FullCRC(reinterpret_cast<unsigned char*>(cGroupName), strlen(cGroupName));

			for ( int i = 0; i < nNumGroups; i++ )
			{
				if ( nHash == CGroupedBuildings::GetGroup(i)->GetHash() )
				{
					scriptParams[0].iParam = CPools::GetBuildingPool()->GetHandle(CGroupedBuildings::GetGroup(i)->GetBuilding());
					StoreParameters(1);
					break;
				}
			}
			return 0;
		}
	case IS_KEYBOARD_KEY_PRESSED:
		{
			CollectParameters( 1 );

#if INCLUDE_DEBUG_COMMANDS
			UpdateCompareFlag( ControlsManager.GetIsKeyboardKeyDown( (RsKeyCodes)scriptParams[0].iParam ) );
#else
			UpdateCompareFlag( false );
#endif
	
			return 0;
		}
	case IS_KEYBOARD_KEY_JUST_PRESSED:
		{
			CollectParameters( 1 );

#if INCLUDE_DEBUG_COMMANDS
			UpdateCompareFlag( ControlsManager.GetIsKeyboardKeyJustDown( (RsKeyCodes)scriptParams[0].iParam ) );
#else
			UpdateCompareFlag( false );
#endif

			return 0;
		}
	case 0x0821:
		{
			//   is_this_model_a_bike %1d%
			CollectParameters(1);
			UpdateCompareFlag( CModelInfo::IsBikeModel(scriptParams[0].iParam) );
			return 0;
		}
	}

	return -1;
}

int8 CRunningScript::ProcessCustomCommandsHigh( int16 command )
{
	switch ( command )
	{
	case SWITCH_CAR_RADIO:
		{
			CollectParameters(1);
			return 0;
		}
	case CLEAR_CHAR_WAIT_STATE:
		{
			CollectParameters(1);
			return 0;
		}
	case GET_RANDOM_CAR_OF_TYPE_WITH_PASSENGERS_IN_AREA:
		{
			CollectParameters(5);
			scriptParams[0].iParam = -1;
			StoreParameters(1);
			return 0;
		}
	case COMMAND_00F5:
		{
			CollectParameters(3);
			return 0;
		}
	case COMMAND_0324:
	case COMMAND_034F:
	case COMMAND_0347:
	case COMMAND_03DE:
		{
			CollectParameters(2);
			return 0;
		}
	case COMMAND_03AF:
		{
			CollectParameters(1);
			return 0;
		}
	case COMMAND_03D6:
	case COMMAND_03D7:
		{
			CollectParameters(2);
			return 0;
		}
	case COMMAND_0400:
		{
			CollectParameters(1);
			scriptParams[0].iParam = 0;
			StoreParameters(1);
			return 0;
		}
	case COMMAND_0442:
		{
			CollectParameters(2);
			return 0;
		}
	case COMMAND_041B:
	case COMMAND_04AE:
		{
			CollectParameters(2);
			return 0;
		}
	default:
		{
			//assert( !"Whoops (CRunningScript::ProcessCustomCommandsHigh)" );
			break;
		}
	}

	// Claim unimplemented opcodes are processed too so our dummies don't interrupt the script
	return 0;
}

int8 CRunningScript::ProcessOneCommand()
{
    typedef int8(CRunningScript::*ScriptCommandMethod)(int16);
 
    static ScriptCommandMethod* StockCommandsArray = *(ScriptCommandMethod**)0x469FEE;
    static const ScriptCommandMethod CustomCommandsArray[] = {
                    &CRunningScript::ProcessCustomCommandsLow, &CRunningScript::ProcessCustomCommandsHigh };
 
	uint16 command = ReadVariable<uint16_t>();
#ifdef DEVBUILD
	try {
#endif
    NotFlag = (command & 0x8000) != 0;
 
    int8 customResult = (this->*CustomCommandsArray[ (command & 0x7FFF) / 0x1000 ])( command & 0x7FFF );
    if ( customResult != -1 )
        return customResult;
 
    return (this->*StockCommandsArray[ (command & 0x7FFF) / 100 ])( command & 0x7FFF );
#ifdef DEVBUILD
	} catch (...) {
		LogToFile( "Error processing command 0x%04X in script %s, halting script...", command, Name );
		WakeTime = INT_MAX;
		return -1;
	}
#endif
}

void CScriptFunction::Init(CRunningScript* parent)
{
	parentScript = parent;
//	bApplyWidescreenFixOnThisScriptDraws = false;
	memset(bAmountOfVariablesWePassed, -1, sizeof(bAmountOfVariablesWePassed));
	
	int32	loopCounter = GOSUB_STACK_SIZE;
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
	uint16				activeFunctions = 0;
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
	C_PcSave::PcClassSaveRoutine(&activeFunctions, sizeof(uint16));
	
	if ( activeFunctions )
	{
		do
		{
			uint16		scriptIndex = static_cast<unsigned short>(lastActiveScript - ScriptsArray);
			CScriptFunction&	function = CRunningScript::ms_scriptFunction[scriptIndex];

			if ( !lastActiveScript->bIsExternal && lastActiveScript->extrnAttachType == -1 )
			{
				C_PcSave::PcClassSaveRoutine(&scriptIndex, sizeof(uint16));
				for ( int32 j = 0; j < GOSUB_STACK_SIZE; ++j )
				{
					int8 amountOfParams = function.bAmountOfVariablesWePassed[j];
					C_PcSave::PcClassSaveRoutine(&amountOfParams, sizeof(int8));
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
}

void CScriptFunction::LoadAllFunctions()
{
	uint16		activeFunctions;
	C_PcSave::PcClassLoadRoutine(&activeFunctions, sizeof(uint16));

	do
	{
		--activeFunctions;
		uint16				scriptIndex;
		C_PcSave::PcClassLoadRoutine(&scriptIndex, sizeof(uint16));
		CScriptFunction&	function = CRunningScript::ms_scriptFunction[scriptIndex];

		function.parentScript = scriptIndex + ScriptsArray;
		for ( int32 j = 0; j < GOSUB_STACK_SIZE; ++j )
		{
			int8 amountOfParams;
			C_PcSave::PcClassLoadRoutine(&amountOfParams, sizeof(int8));
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

void __declspec(naked) CollectParametersWrapped()
{
	_asm
	{
		push	ecx
		push	[esp+8]
		call	CRunningScript::CollectParameters
		pop		ecx
		retn	4
	}

}


void CRunningScript::Inject()
{
	using namespace Memory;

	Patch<WORD>( 0x469FB7, 0xCE8B );
	InjectHook( 0x469FB9, &ProcessOneCommand, PATCH_CALL );
	Patch<WORD>( 0x469FBE, 0x37EB );

	InjectHook( 0x464080, &CollectParametersWrapped, PATCH_JUMP );
	InjectHook( 0x463D50, &ReadTextLabelFromScript, PATCH_JUMP );
}

static void __declspec(naked) GetPadMode()
{
	_asm
	{
		push	0
		call	CPad::GetPad
		add		esp, 4
		movzx	eax, [eax]CPad.Mode
		mov		ecx, [scriptParams]
		mov		[ecx], eax
		mov		ecx, esi
		retn
	}
}

static StaticPatcher	Patcher([](){ 
						//Memory::InjectHook(0x47F39B, GetPadMode, PATCH_CALL);
						//Memory::Nop(0x47F3A0, 7);
						
						CRunningScript::Inject();
									});