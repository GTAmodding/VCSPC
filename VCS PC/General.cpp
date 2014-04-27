#include "stdafx.h"
#include "General.h"

// Wrappers
WRAPPER bool CalcScreenCoors(const CVector& vecIn, CVector* vecOut) { WRAPARG(vecIn); WRAPARG(vecOut); EAXJMP(0x71DAB0); }
WRAPPER void LoadingScreenLoadingFile(const char* pText) { WRAPARG(pText); EAXJMP(0x5B3680); }

WRAPPER void CEntity::UpdateRW() { EAXJMP(0x446F90); }
WRAPPER void CEntity::RegisterReference(CEntity** pAddress) { WRAPARG(pAddress); EAXJMP(0x571B70); }
WRAPPER void CEntity::CleanUpOldReference(CEntity** pAddress) { WRAPARG(pAddress); EAXJMP(0x571A00); }
WRAPPER void CEntity::Render() { EAXJMP(0x534310); }

WRAPPER unsigned int CKeyGen::GetUppercaseKey(const char* pEntry) { WRAPARG(pEntry); EAXJMP(0x53CF30); }

bool& CGame::bMissionPackGame = *(bool*)0xB72910;

/*void CMatrix::UpdateRW()
{
	if ( pMatrix )
	{
		pMatrix->right.x = matrix.right.x;
		pMatrix->right.y = matrix.right.y;
		pMatrix->right.z = matrix.right.z;

		pMatrix->up.x = matrix.up.x;
		pMatrix->up.y = matrix.up.y;
		pMatrix->up.z = matrix.up.z;

		pMatrix->at.x = matrix.at.x;
		pMatrix->at.y = matrix.at.y;
		pMatrix->at.z = matrix.at.z;

		pMatrix->pos.x = matrix.pos.x;
		pMatrix->pos.y = matrix.pos.y;
		pMatrix->pos.z = matrix.pos.z;

		RwMatrixUpdate(pMatrix);
	}
}*/