#ifndef __CRUNNINGSCRIPT
#define __CRUNNINGSCRIPT

#define NUM_SCRIPTS					96
#define GOSUB_STACK_SIZE			8

union SCRIPT_VAR
{
	DWORD	dwParam;
	int		iParam;
	WORD	wParam;
	BYTE	bParam;
	float	fParam;
	void*	pParam;
	char*	pcParam;
};

enum eOperandType
{
	globalVar = 2,			
	localVar = 3,			
	globalArr = 7,
	localArr = 8,
	imm8 = 4,				
	imm16 = 5,				
	imm32 = 6,				
	imm32f = 1,				
	vstring = 0x0E,			
	sstring = 9,			
	globalVarVString = 0x10,
	localVarVString = 0x11,	
	globalVarSString = 0x0A,	
	localVarSString = 0x0B,
	globalVarSArrString = 0x0C,
	localVarSArrString = 0x0D,
	globalVarVArrString = 0x12,
	localVarVArrString = 0x13,
	lstring = 0x0F
};

class CRunningScript
{
	friend class			CScriptFunction;
public:
	static CScriptFunction	ms_scriptFunction[NUM_SCRIPTS];

private:
	CRunningScript*			Previous;
	CRunningScript*			Next;
	char					Name[8];
	void*					BaseIP;
	void*					CurrentIP;
	void*					Stack[GOSUB_STACK_SIZE];
	WORD					SP;
	SCRIPT_VAR				LocalVar[34];
	bool					bIsActive;
	bool					bCondResult;
	bool					bUseMissionCleanup;
	bool					bIsExternal;
	bool					bTextBlockOverride;
	signed char				extrnAttachType;
	DWORD					WakeTime;
	WORD					LogicalOp;
	bool					NotFlag;
	bool					bWastedBustedCheck;
	bool					bWastedOrBusted;
	void*					SceneSkipIP;
	bool					bIsMission;
	/*	CLEO class extension */
	BYTE					scmFunction[2];
	BYTE					IsCustom;

private:
	void					GetStringParam(char* textPtr, BYTE len);
	void					CollectParameters(WORD numParams);
	void					StoreParameters(WORD numParams);
	void					UpdateCompareFlag(bool result);
	unsigned short			GetGlobalVarOffset();
	void*					GetPointerToScriptVariable(int nParam);
	void					SetIP(int IP);

	signed char				CollectParametersForScriptFunction(signed short numParams);
	void					StoreParametersFromScriptFunction();

	CScriptFunction&		GetExtrasForScript();

public:
	void					Init();

	void					ProcessVCSCommands(WORD opcode);
};

extern SCRIPT_VAR*	scriptLocals;

class CScriptFunction
{
	friend class				CRunningScript;

private:
	CRunningScript*		parentScript;
//	bool				bApplyWidescreenFixOnThisScriptDraws;
	signed char			bAmountOfVariablesWePassed[GOSUB_STACK_SIZE];
	bool				bNOTFlagState[GOSUB_STACK_SIZE];
	SCRIPT_VAR*			savedVariablesSpace[GOSUB_STACK_SIZE];

public:
	void				Init(CRunningScript* parent);
	static void			SaveAllFunctions();
	static void			LoadAllFunctions();

	void				RegisterCall(CRunningScript* caller, signed char passedParams)
	{
		WORD stackID = caller->SP;
		if ( !savedVariablesSpace[stackID] )
			savedVariablesSpace[stackID] = new SCRIPT_VAR[32];

		if ( !caller->bIsMission )
			memcpy(savedVariablesSpace[stackID], &caller->LocalVar, sizeof(SCRIPT_VAR) * 32);
		else
			memcpy(savedVariablesSpace[stackID], scriptLocals, sizeof(SCRIPT_VAR) * 32);
		bAmountOfVariablesWePassed[stackID] = passedParams;
		bNOTFlagState[stackID] = caller->NotFlag;
//		bThisIndirectionLevelIsCall[stackID] = true;
	};

	void				PopCall(CRunningScript* caller)
	{
		if ( !caller->bIsMission )
			memcpy(&caller->LocalVar, savedVariablesSpace[caller->SP], sizeof(SCRIPT_VAR) * 32);
		else
			memcpy(scriptLocals, savedVariablesSpace[caller->SP], sizeof(SCRIPT_VAR) * 32);

		caller->NotFlag = bNOTFlagState[caller->SP];
	}

	void				RegisterGosub(CRunningScript* caller)
	{
		bAmountOfVariablesWePassed[caller->SP] = -1;
//		bThisIndirectionLevelIsCall[caller->SP] = false;
	}

	CScriptFunction()
	{
		memset(savedVariablesSpace, 0, sizeof(savedVariablesSpace));
	}
};

class CMappedVariable
{
private:
	void*	pVarData;

public:
	void*	AccessData()
		{ return pVarData; };

	CMappedVariable(long nSize = 1)
	{
		pVarData = new int[nSize];
	};

	~CMappedVariable()
	{
		delete[] pVarData;
	}
};

class CTheScripts
{
public:
	static void				Init();
	static CRunningScript*	StartNewScript(void* nScriptIP);

	static void				SaveAllScripts();
	static void				LoadAllScripts();
	static void				StartTestScript();
	static void				ZeroMissionVars();

	static bool				IsPlayerOnAMission();
};

extern SCRIPT_VAR*			scriptParams;
extern SCRIPT_VAR*			scriptLocals;
extern CRunningScript**		pActiveScripts;
extern CRunningScript*		ScriptsArray;

static_assert(sizeof(CRunningScript) == 0xE0, "Wrong size: CRunningScript");

#endif