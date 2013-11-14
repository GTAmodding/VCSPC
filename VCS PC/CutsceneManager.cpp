#include "StdAfx.h"

void CCutsceneMgr::SetProperCSPlrModel(char* pModel)
{
	if ( !_stricmp(pModel, "csplr") )
	{
		static const DWORD			dwModelHashes[] = {	0xB5AB388C, 0x2C6C35B3, 0x5B6B0525, 0xC50F9086,
														0xB208A010, 0x2B01F1AA, 0x5C06C13C, 0xCCB9DCAD,
														0xBBBEEC3B, 0x5FB0ED6C, 0x28B7DDFA, 0xB1BE8C40,
														0xC6B9BCD6, 0x58DD2975, 0x2FDA19E3 };

		static const char*			pcCutModels[] = {	"csplr", "csplr2", "csplr3", "csplr4", "csplr5",
														"csplr6", "csplr7", "csplr8", "csplr9", "csplr10",
														"csplr11", "csplr12", "csplr13", "csplr14", "csplr15" };

		static const bool			bSkinExists[] = {	true, true, false, true, false, 
														false, false, false, false, false, 
														false, false, false, false, true };

		const char*	pSkinToLoad = pcCutModels[1];
		DWORD		dwIGHash = CModelInfo::ms_modelInfoPtrs[0]->ulHashKey;

		for ( int i = 0; i < (sizeof(dwModelHashes)/sizeof(DWORD)); ++i )
		{
			if ( dwIGHash == dwModelHashes[i] && bSkinExists[i] )
			{
				pSkinToLoad = pcCutModels[i];
				break;
			}
		}
		strncpy(pModel, pSkinToLoad, 32);
	} 
}