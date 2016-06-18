#ifndef __CRUNNINGSCRIPT
#define __CRUNNINGSCRIPT

#define NUM_SCRIPTS					96
#define GOSUB_STACK_SIZE			8

union SCRIPT_VAR
{
	uint32	dwParam;
	int32	iParam;
	uint16	wParam;
	uint8	bParam;
	float	fParam;
	void*	pParam;
	char*	pcParam;
};

enum eOperandType : int8
{
	globalVar = 2,			
	localVar = 3,			
	globalArr = 7,
	localArr = 8,
	imm8 = 4,				
	imm16 = 5,				
	imm32f = 6,				
	imm32 = 1,				
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
	struct ArrayProperties
	{
		uint8 m_nElementType : 7;
		bool m_bIsIndexGlobalVariable : 1;
	};

	

	CRunningScript*			Previous;
	CRunningScript*			Next;
	char					Name[8];
	uint8*					BaseIP;
	uint8*					CurrentIP;
	uint8*					Stack[GOSUB_STACK_SIZE];
	uint16					SP;
	SCRIPT_VAR				LocalVar[34];
	bool					bIsActive;
	bool					bCondResult;
	bool					bUseMissionCleanup;
	bool					bIsExternal;
	bool					bTextBlockOverride;
	int8					extrnAttachType;
	int32					WakeTime;
	uint16					LogicalOp;
	bool					NotFlag;
	bool					bWastedBustedCheck;
	bool					bWastedOrBusted;
	void*					SceneSkipIP;
	bool					bIsMission;
	/*	CLEO class extension */
	uint8					scmFunction[2];
	uint8					IsCustom;

private:
	void					ReadTextLabelFromScript(char* textPtr, uint8 len);
	void					CollectParameters(int16 numParams);
	void					StoreParameters(int16 numParams);
	void					UpdateCompareFlag(bool result);
	unsigned short			GetGlobalVarOffset();
	void*					GetPointerToScriptVariable(int32 nParam);
	void					SetIP(int32 IP);

	template<typename T>	
	inline T				ReadVariable()
	{ T var = *(T*)CurrentIP; CurrentIP += sizeof(T); return var; }

	void					ReadArrayInformation( int32 move, uint16* varOffset, int32* varIndex );
	SCRIPT_VAR*				GetPointerToLocalArrayElement( uint16 offset, int32 index, uint8 size );
	SCRIPT_VAR*				GetPointerToLocalVariable( uint16 var );

	signed char				CollectParametersForScriptFunction(int16 numParams);
	void					StoreParametersFromScriptFunction();

	CScriptFunction&		GetExtrasForScript();

public:
	void					Init();

	void					ProcessVCSCommands(int16 opcode);

	static void				Inject();
};

extern SCRIPT_VAR*	scriptLocals;

class CScriptFunction
{
	friend class				CRunningScript;

private:
	CRunningScript*		parentScript;
//	bool				bApplyWidescreenFixOnThisScriptDraws;
	int8				bAmountOfVariablesWePassed[GOSUB_STACK_SIZE];
	bool				bNOTFlagState[GOSUB_STACK_SIZE];
	SCRIPT_VAR*			savedVariablesSpace[GOSUB_STACK_SIZE];

public:
	void				Init(CRunningScript* parent);
	static void			SaveAllFunctions();
	static void			LoadAllFunctions();

	void				RegisterCall(CRunningScript* caller, int8 passedParams)
	{
		uint16 stackID = caller->SP;
		if ( !savedVariablesSpace[stackID] )
			savedVariablesSpace[stackID] = new SCRIPT_VAR[32];

		if ( !caller->bIsMission )
			memcpy(savedVariablesSpace[stackID], &caller->LocalVar, sizeof(SCRIPT_VAR) * 32);
		else
			memcpy(savedVariablesSpace[stackID], scriptLocals, sizeof(SCRIPT_VAR) * 32);
		bAmountOfVariablesWePassed[stackID] = passedParams;
		bNOTFlagState[stackID] = caller->NotFlag;
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


static_assert(sizeof(eOperandType) == 0x1, "Wrong size: eOperandType");
static_assert(sizeof(CRunningScript) == 0xE0, "Wrong size: CRunningScript");

#endif