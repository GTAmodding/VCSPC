#include "StdAfx.h"
#include "EntryExitMgr.h"

#include "PcSave.h"

tEntryExitBimap				CEntryExit::strUniqueNames;

CPool<CEntryExit>*&			CEntryExitManager::mp_poolEntryExits = *(CPool<CEntryExit>**)0x96A7D8;
int&						CEntryExitManager::ms_entryExitStackPosn = *(int*)0x96A7C4;
CEntryExit** const			CEntryExitManager::ms_entryExitStack = (CEntryExit**)0x96A720;

unsigned int				CEntryExitManager::m_dwFallbackFrom, CEntryExitManager::m_dwFallbackTo;

WRAPPER int CEntryExitManager::AddOne(float fEntX, float fEntY, float fEntZ, float fEntA, float fEntRX, float fEntRY, float, float fExitX, float fExitY, float fExitZ, float fExitA, int nArea, int flags, int nExtraColors, int nTimeOn, int nTimeOff, int nUnkSky, const char* pGXTName)
{ WRAPARG(fEntX); WRAPARG(fEntY); WRAPARG(fEntZ); WRAPARG(fEntA); WRAPARG(fEntRX); WRAPARG(fEntRY); WRAPARG(fExitX); WRAPARG(fExitY); WRAPARG(fExitZ); WRAPARG(fExitA); WRAPARG(nArea); WRAPARG(flags); WRAPARG(nExtraColors); WRAPARG(nTimeOn); WRAPARG(nTimeOff); WRAPARG(nUnkSky); WRAPARG(pGXTName); EAXJMP(0x43FA00); }

bool CEntryExitManager::Load()
{
	if ( m_dwFallbackFrom && m_dwFallbackTo )
	{
		// Perform fallback
		
		// From version 1?
		if ( m_dwFallbackFrom == 1 )
		{
			// Read ms_entryExitStackPosn and tweak the index to reflect changes
			C_PcSave::PcClassLoadRoutine(&ms_entryExitStackPosn, sizeof(ms_entryExitStackPosn));

			auto*	pBimap = CEntryExit::GetBimap();

			for ( int i = 0; i < ms_entryExitStackPosn; i++ )
			{
				unsigned short		nOldIndex;
				C_PcSave::PcClassLoadRoutine(&nOldIndex, sizeof(nOldIndex));

				// ID -> name -> hash assignment
				assert(nOldIndex == 2 || nOldIndex == 4);
				auto it = pBimap->right.find(HashHelper.FullCRC(nOldIndex == 2 ? reinterpret_cast<unsigned char*>("SAFE005") : reinterpret_cast<unsigned char*>("SAFE003"), 7));

				assert(it != pBimap->right.end());
				if ( it != pBimap->right.end() )
					ms_entryExitStack[i] = it->second;
				else
				{
					--ms_entryExitStackPosn;
					--i;
				}
			}

			// Skip the rest
			signed short	nIndex;

			for ( C_PcSave::PcClassLoadRoutine(&nIndex, sizeof(nIndex)); nIndex != -1; C_PcSave::PcClassLoadRoutine(&nIndex, sizeof(nIndex)) )
			{
				unsigned int		buf;
				C_PcSave::PcClassLoadRoutine(&buf, sizeof(buf));
			}

			m_dwFallbackFrom = m_dwFallbackTo = 0;
			return true;
		}
	}

	// Newest version to load
	C_PcSave::PcClassLoadRoutine(&ms_entryExitStackPosn, sizeof(ms_entryExitStackPosn));
	auto*	pBimap = CEntryExit::GetBimap();

	for ( int i = 0;  i < ms_entryExitStackPosn; i++ )
	{
		unsigned int		nHash;
		C_PcSave::PcClassLoadRoutine(&nHash, sizeof(nHash));
		
		auto it = pBimap->right.find(nHash);

		assert(it != pBimap->right.end());
		if ( it != pBimap->right.end() )
			ms_entryExitStack[i] = it->second;
		else
		{
			--ms_entryExitStackPosn;
			--i;
		}
	}

	return true;
}

bool CEntryExitManager::Save()
{
	C_PcSave::PcClassSaveRoutine(&ms_entryExitStackPosn, sizeof(ms_entryExitStackPosn));
	auto*	pBimap = CEntryExit::GetBimap();

	for ( int i = 0; i < ms_entryExitStackPosn; i++ )
	{
		auto	it = pBimap->left.find(ms_entryExitStack[i]);
		C_PcSave::PcClassSaveRoutine(&it->second, sizeof(it->second));
	}


	return true;
}