#ifndef __MATHS__H
#define __MATHS__H


#define RAD_TO_DEG								(180.0/M_PI)
#define DEG_TO_RAD								(M_PI/180.0)

class CVector
{
public:
	float	x, y, z;

	CVector()
	{}

	CVector(float fX, float fY, float fZ=0.0f)
		: x(fX), y(fY), z(fZ)
	{}

	CVector(const RwV3d& rwVec)
		: x(rwVec.x), y(rwVec.y), z(rwVec.z)
	{}

	CVector&		operator+=(const CVector& vec)
			{ x += vec.x; y += vec.y; z += vec.z;
			return *this; }
	CVector&		operator+=(const RwV3d& vec)
			{ x += vec.x; y += vec.y; z += vec.z;
			return *this; }
	CVector&		operator-=(const CVector& vec)
			{ x -= vec.x; y -= vec.y; z -= vec.z;
			return *this; }
	CVector&		operator-=(const RwV3d& vec)
			{ x -= vec.x; y -= vec.y; z -= vec.z;
			return *this; }

	inline float	Magnitude()
		{ return sqrt(x * x + y * y + z * z); }
	inline float	MagnitudeSqr()
		{ return x * x + y * y + z * z; }
	inline CVector&	Normalize()
		{ float	fInvLen = 1.0f / Magnitude(); x *= fInvLen; y *= fInvLen; z *= fInvLen; return *this; }

	friend inline float DotProduct(const CVector& vec1, const CVector& vec2)
		{ return vec1.x * vec2.x + vec1.x * vec2.y + vec1.z * vec2.z; }
	friend inline CVector CrossProduct(const CVector& vec1, const CVector& vec2)
		{ return CVector(	vec1.y * vec2.z - vec1.z * vec2.y,
							vec1.z * vec2.x - vec1.x * vec2.z,
							vec1.x * vec2.y - vec1.y * vec2.x); }

	friend inline CVector operator*(const CVector& in, float fMul)
		{ return CVector(in.x * fMul, in.y * fMul, in.z * fMul); }
	friend inline CVector operator+(const CVector& vec1, const CVector& vec2)
		{ return CVector(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z); }
	friend inline CVector operator+(const CVector& vec1, const RwV3d& vec2)
		{ return CVector(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z); }
	friend inline CVector operator-(const CVector& vec1, const CVector& vec2)
		{ return CVector(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z); }
	friend inline CVector operator-(const CVector& vec1, const RwV3d& vec2)
		{ return CVector(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z); }
	friend inline CVector operator-(const CVector& vec)
		{ return CVector(-vec.x, -vec.y, -vec.z); }
};

class CVector2D
{
public:
	float	x, y;

	CVector2D()
	{}

	CVector2D(float fX, float fY)
		: x(fX), y(fY)
	{}

	CVector2D&		operator+=(const CVector2D& vec)
			{ x += vec.x; y += vec.y;
			return *this; }
	CVector2D&		operator-=(const CVector2D& vec)
			{ x -= vec.x; y -= vec.y;
			return *this; }

	inline float	Magnitude()
		{ return sqrt(x * x + y * y); }
	inline float	MagnitudeSqr()
		{ return x * x + y * y; }
	inline CVector2D&	Normalize()
		{ float	fInvLen = 1.0f / Magnitude(); x *= fInvLen; y *= fInvLen; return *this; }

	friend inline float DotProduct(const CVector2D& vec1, const CVector2D& vec2)
		{ return vec1.x * vec2.x + vec1.x * vec2.y; }

	friend inline CVector2D operator*(const CVector2D& in, float fMul)
		{ return CVector2D(in.x * fMul, in.y * fMul); }
	friend inline CVector2D operator+(const CVector2D& vec1, const CVector2D& vec2)
		{ return CVector2D(vec1.x + vec2.x, vec1.y + vec2.y); }
	friend inline CVector2D operator-(const CVector2D& vec1, const CVector2D& vec2)
		{ return CVector2D(vec1.x - vec2.x, vec1.y - vec2.y); }
	friend inline CVector2D operator-(const CVector2D& vec)
		{ return CVector2D(-vec.x, -vec.y); }
};

class CMatrix
{
public:
	RwMatrix	matrix;
	RwMatrix*	pMatrix;
	BOOL		haveRwMatrix;

public:
	inline CMatrix()
		: pMatrix(nullptr), haveRwMatrix(FALSE)
	{}

	inline CMatrix(RwMatrix* pMatrix, bool bHasMatrix=false)
		: pMatrix(nullptr)
	{ Attach(pMatrix, bHasMatrix); }

	inline CMatrix(const CMatrix& theMatrix)
		: pMatrix(nullptr), haveRwMatrix(FALSE), matrix(theMatrix.matrix)
	{}

	inline CMatrix(const CVector& vecRight, const CVector& vecUp, const CVector& vecAt, const CVector& vecPos)
	{
		matrix.right.x = vecRight.x;
		matrix.right.y = vecRight.y;
		matrix.right.z = vecRight.z;

		matrix.up.x = vecUp.x;
		matrix.up.y = vecUp.y;
		matrix.up.z = vecUp.z;

		matrix.at.x = vecAt.x;
		matrix.at.y = vecAt.y;
		matrix.at.z = vecAt.z;

		matrix.pos.x = vecPos.x;
		matrix.pos.y = vecPos.y;
		matrix.pos.z = vecPos.z;
	}

	inline ~CMatrix()
		{	if ( haveRwMatrix && pMatrix ) 
				RwMatrixDestroy(pMatrix); }

	friend inline CMatrix operator*(const CMatrix& Rot1, const CMatrix& Rot2)
		{ return CMatrix(	CVector(Rot1.matrix.right.x * Rot2.matrix.right.x + Rot1.matrix.right.y * Rot2.matrix.up.x + Rot1.matrix.right.z * Rot2.matrix.at.x + Rot2.matrix.pos.x,
								Rot1.matrix.right.x * Rot2.matrix.right.y + Rot1.matrix.right.y * Rot2.matrix.up.y + Rot1.matrix.right.z * Rot2.matrix.at.y + Rot2.matrix.pos.y,
								Rot1.matrix.right.x * Rot2.matrix.right.z + Rot1.matrix.right.y * Rot2.matrix.up.z + Rot1.matrix.right.z * Rot2.matrix.at.z + Rot2.matrix.pos.z),
						CVector(Rot1.matrix.up.x * Rot2.matrix.right.x + Rot1.matrix.up.y * Rot2.matrix.up.x + Rot1.matrix.up.z * Rot2.matrix.at.x + Rot2.matrix.pos.x,
								Rot1.matrix.up.x * Rot2.matrix.right.y + Rot1.matrix.up.y * Rot2.matrix.up.y + Rot1.matrix.up.z * Rot2.matrix.at.y + Rot2.matrix.pos.y,
								Rot1.matrix.up.x * Rot2.matrix.right.z + Rot1.matrix.up.y * Rot2.matrix.up.z + Rot1.matrix.up.z * Rot2.matrix.at.z + Rot2.matrix.pos.z),
						CVector(Rot1.matrix.at.x * Rot2.matrix.right.x + Rot1.matrix.at.y * Rot2.matrix.up.x + Rot1.matrix.at.z * Rot2.matrix.at.x + Rot2.matrix.pos.x,
								Rot1.matrix.at.x * Rot2.matrix.right.y + Rot1.matrix.at.y * Rot2.matrix.up.y + Rot1.matrix.at.z * Rot2.matrix.at.y + Rot2.matrix.pos.y,
								Rot1.matrix.at.x * Rot2.matrix.right.z + Rot1.matrix.at.y * Rot2.matrix.up.z + Rot1.matrix.at.z * Rot2.matrix.at.z + Rot2.matrix.pos.z),
						CVector(Rot1.matrix.pos.x * Rot2.matrix.right.x + Rot1.matrix.pos.y * Rot2.matrix.up.x + Rot1.matrix.pos.z * Rot2.matrix.at.x + Rot2.matrix.pos.x,
								Rot1.matrix.pos.x * Rot2.matrix.right.y + Rot1.matrix.pos.y * Rot2.matrix.up.y + Rot1.matrix.pos.z * Rot2.matrix.at.y + Rot2.matrix.pos.y,
								Rot1.matrix.pos.x * Rot2.matrix.right.z + Rot1.matrix.pos.y * Rot2.matrix.up.z + Rot1.matrix.pos.z * Rot2.matrix.at.z + Rot2.matrix.pos.z)); };

	friend inline CVector operator*(const CMatrix& matrix, const CVector& vec)
			{ return CVector(matrix.matrix.up.x * vec.y + matrix.matrix.right.x * vec.x + matrix.matrix.at.x * vec.z + matrix.matrix.pos.x,
								matrix.matrix.up.y * vec.y + matrix.matrix.right.y * vec.x + matrix.matrix.at.y * vec.z + matrix.matrix.pos.y,
								matrix.matrix.up.z * vec.y + matrix.matrix.right.z * vec.x + matrix.matrix.at.z * vec.z + matrix.matrix.pos.z); };

	inline CVector*	GetRight()
		{ return reinterpret_cast<CVector*>(&matrix.right); }
	inline CVector*	GetUp()
		{ return reinterpret_cast<CVector*>(&matrix.up); }
	inline CVector*	GetAt()
		{ return reinterpret_cast<CVector*>(&matrix.at); }
	inline CVector* GetPos()
		{ return reinterpret_cast<CVector*>(&matrix.pos); }

	inline void		SetTranslateOnly(float fX, float fY, float fZ)
		{ matrix.pos.x = fX; matrix.pos.y = fY; matrix.pos.z = fZ; }
	
	inline void		SetRotateX(float fAngle)
		{ SetRotateXOnly(fAngle); matrix.pos.x = 0.0f; matrix.pos.y = 0.0f; matrix.pos.z = 0.0f; }
	inline void		SetRotateY(float fAngle)
		{ SetRotateYOnly(fAngle); matrix.pos.x = 0.0f; matrix.pos.y = 0.0f; matrix.pos.z = 0.0f; }
	inline void		SetRotateZ(float fAngle)
		{ SetRotateZOnly(fAngle); matrix.pos.x = 0.0f; matrix.pos.y = 0.0f; matrix.pos.z = 0.0f; }
	inline void		SetRotate(float fAngleX, float fAngleY, float fAngleZ)
		{ SetRotateOnly(fAngleX, fAngleY, fAngleZ); matrix.pos.x = 0.0f; matrix.pos.y = 0.0f; matrix.pos.z = 0.0f; }
	inline void		SetTranslate(float fX, float fY, float fZ)	
		{	matrix.right.x = 1.0f; matrix.right.y = 0.0f; matrix.right.z = 0.0f;
			matrix.up.x = 0.0f; matrix.up.y = 1.0f; matrix.up.z = 0.0f;
			matrix.at.x = 0.0f; matrix.at.y = 0.0f; matrix.at.z = 1.0f;
			SetTranslateOnly(fX, fY, fZ); }

	inline void		SetUnity()
	{	
		matrix.right.x = 1.0f; matrix.right.y = 0.0f; matrix.right.z = 0.0f;
		matrix.up.x = 0.0f; matrix.up.y = 1.0f; matrix.up.z = 0.0f;
		matrix.at.x = 0.0f; matrix.at.y = 0.0f; matrix.at.z = 1.0f;
		matrix.pos.x = 0.0f; matrix.pos.y = 0.0f; matrix.pos.z = 0.0f;
	}

	inline void		RotateX(float fAngle)
	{
		CMatrix		RotationMatrix;
		RotationMatrix.SetRotateX(fAngle);
		*this = *this * RotationMatrix;
	}

	inline void		RotateY(float fAngle)
	{
		CMatrix		RotationMatrix;
		RotationMatrix.SetRotateY(fAngle);
		*this = *this * RotationMatrix;
	}

	inline void		RotateZ(float fAngle)
	{
		CMatrix		RotationMatrix;
		RotationMatrix.SetRotateZ(fAngle);
		*this = *this * RotationMatrix;
	}

	inline void		Rotate(float fAngleX, float fAngleY, float fAngleZ)
	{
		CMatrix		RotationMatrix;
		RotationMatrix.SetRotate(fAngleX, fAngleY, fAngleZ);
		*this = *this * RotationMatrix;
	}

	inline void		SetRotateXOnly(float fAngle)
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

	inline void		SetRotateYOnly(float fAngle)
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

	inline void		SetRotateZOnly(float fAngle)
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

	inline void		SetRotateOnly(float fAngleX, float fAngleY, float fAngleZ)
	{
		matrix.right.x = cos(fAngleZ) * cos(fAngleY) - sin(fAngleZ) * sin(fAngleX) * sin(fAngleY);
		matrix.right.y = cos(fAngleZ) * sin(fAngleX) * sin(fAngleY) + sin(fAngleZ) * cos(fAngleY);
		matrix.right.z = -cos(fAngleX) * sin(fAngleY);

		matrix.up.x = -sin(fAngleZ) * cos(fAngleX);
		matrix.up.y = cos(fAngleZ) * cos(fAngleX);
		matrix.up.z = sin(fAngleX);

		matrix.at.x = sin(fAngleZ) * sin(fAngleX) * cos(fAngleY) + cos(fAngleZ) * sin(fAngleY);
		matrix.at.y = sin(fAngleZ) * sin(fAngleY) - cos(fAngleZ) * sin(fAngleX) * cos(fAngleY);
		matrix.at.z = cos(fAngleX) * cos(fAngleY);
	}

	inline void		Attach(RwMatrix* pMatrix, bool bHasMatrix)
	{
		if ( this->pMatrix && haveRwMatrix )
			RwMatrixDestroy(this->pMatrix);

		this->pMatrix = pMatrix;
		haveRwMatrix = bHasMatrix;

		Update();
	}

	inline void		AttachRw(RwMatrix* pMatrix, bool bHasMatrix)
	{
		if ( this->pMatrix && haveRwMatrix )
			RwMatrixDestroy(this->pMatrix);

		this->pMatrix = pMatrix;
		haveRwMatrix = bHasMatrix;

		UpdateRW();
	}

	inline void		Detach()
	{
		if ( pMatrix )
		{
			if ( haveRwMatrix )
				RwMatrixDestroy(pMatrix);
			pMatrix = nullptr;
		}
	}

	inline void		UpdateRW() const
	{
		if ( pMatrix )
			UpdateRwMatrix(pMatrix);
	}

	inline void		Update()
	{
		UpdateMatrix(pMatrix);
	}

	inline void		UpdateMatrix(RwMatrix* pMatrix)
	{
		matrix.right = pMatrix->right;
		matrix.up = pMatrix->up;
		matrix.at = pMatrix->at;
		matrix.pos = pMatrix->pos;
	}

	inline void		UpdateRwMatrix(RwMatrix* pMatrix) const
	{
		pMatrix->right = matrix.right;
		pMatrix->up = matrix.up;
		pMatrix->at = matrix.at;
		pMatrix->pos = matrix.pos;
		RwMatrixUpdate(pMatrix);
	}

	inline void		CopyToRwMatrix(RwMatrix* pMatrix) const
	{
		pMatrix->right = this->pMatrix->right;
		pMatrix->up = this->pMatrix->up;
		pMatrix->at = this->pMatrix->at;
		pMatrix->pos = this->pMatrix->pos;
		RwMatrixUpdate(pMatrix);
	}

	inline void		CopyOnlyMatrix(const CMatrix& from)
	{
		matrix = from.matrix;
	}
};

#endif