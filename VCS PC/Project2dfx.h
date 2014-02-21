#ifndef __PROJECT2DFX
#define __PROJECT2DFX

#define		CORONAS_FARCLIP			2500.0f

class CLamppostInfo
{
public:
	CVector			vecPos;
	CRGBA			colour;
	float			fCustomSizeMult;

public:
	CLamppostInfo(const CVector& pos, const CRGBA& col, float fCustomMult)
		: vecPos(pos), colour(col), fCustomSizeMult(fCustomMult)
	{}
};

class CProject2dfx
{
private:
	static std::vector<CLamppostInfo>*	m_pLampposts;
	static bool							m_bCatchLamppostsNow;

private:
	static void							EnumerateLampposts();
	static void							RegisterLamppost(CEntity* pObj);

public:
	static inline bool					LampsRegisteringOn()
				{ return m_bCatchLamppostsNow; }
	static inline void					EndRegistering()
		{ m_bCatchLamppostsNow = false; m_pLampposts->shrink_to_fit(); }

	static CEntity*						PossiblyAddThisEntity(CEntity* pEntity);
	static void							Inject();
	static void							Init();
	static void							Shutdown();
	static void							Render();
};

#endif