#ifndef __COLACCEL
#define __COLACCEL

class CColAccel
{
private:
	static const char*&		mp_cCacheName;
	static int&				m_iCachingColSize;
	static int&				m_iCacheState;
	static int&				m_iNumColItems;
	static int&				m_iNumSections;
	static int&				m_iNumColBounds;
	static int&				m_iNumIPLItems;
	static void*&			mp_caccColItems;
	static void*&			mp_caccIPLItems;
	static int*&			m_iSectionSize;
	static void*&			m_iplDefs;
	static void*&			m_colBounds;
	
public:
	static void				startCache();
	static void				endCache();
};

#endif