#include "stdafx.h"

// Wrappers
WRAPPER bool CalcScreenCoors(const CVector& vecIn, CVector* vecOut) { WRAPARG(vecIn); WRAPARG(vecOut); EAXJMP(0x71DAB0); }
WRAPPER void LoadingScreenLoadingFile(const char* pText) { WRAPARG(pText); EAXJMP(0x5B3680); }

WRAPPER void CEntity::UpdateRW() { EAXJMP(0x446F90); }

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

void CMatrix::SetRotateXOnly(float fAngle)
{
	matrix.right.x = 1.0f;
	matrix.right.y = 0.0f;
	matrix.right.z = 0.0f;

	matrix.up.x = 0.0f;
	matrix.up.y = cos(fAngle);
	matrix.up.z = sin(fAngle);

	matrix.at.x = 0.0f;
	matrix.at.y = -sin(fAngle);
	matrix.at.z = cos(fAngle);
}

void CMatrix::SetRotateYOnly(float fAngle)
{
	matrix.right.x = cos(fAngle);
	matrix.right.y = 0.0f;
	matrix.right.z = sin(fAngle);

	matrix.up.x = 0.0f;
	matrix.up.y = 1.0f;
	matrix.up.z = 0.0f;

	matrix.at.x = -sin(fAngle);
	matrix.at.y = 0.0f;
	matrix.at.z = cos(fAngle);
}

void CMatrix::SetRotateZOnly(float fAngle)
{
	matrix.at.x = 0.0f;
	matrix.at.y = 0.0f;
	matrix.at.z = 1.0f;

	matrix.up.x = -sin(fAngle);
	matrix.up.y = cos(fAngle);
	matrix.up.z = 0.0f;

	matrix.right.x = cos(fAngle);
	matrix.right.y = sin(fAngle);
	matrix.right.z = 0.0f;
}