/*
 * Copyright (C) 2013  LINK/2012 <dma_2012@hotmail.com>
 * Licensed under GNU GPL v3, see LICENSE at top level directory.
 *
 *  CAECustomBankLoader
 *  This is a custom bank loader for GTA San Andreas. The differences between the original and this are:
 *      [*] Dynamically allocated sound buffers instead of BankSlot.dat pre-allocation.
 *      [*] Dedicated thread to load banks
 *      [*] Capable of reading bank dumps split on the diskse
 *      [*] (TODO) Capable of reading wave files on the disk
 *
 *  Before you ask if BankSlot.dat pre-allocation isn't faster...
 *  No, it isn't because the game ends up allocating a temporary buffer of memory when reading the bank from the SFXPak.
 *
 */
#include "StdAfx.h"
#include "BankLoader.h"

// Request status
enum
{
    REQUEST_STATUS_NULL         = 0,
    REQUEST_STATUS_BEGIN        = 1,
    REQUEST_STATUS_CUSTOM       = 100,
    REQUEST_STATUS_IN_PROGRESS,
    REQUEST_STATUS_DONE
};

// Bank loading thread
static Queue queue;                         // Request queue
static HANDLE hSemaphore;                   // Thread semaphore
static HANDLE hThread;                      // Thread handle
DWORD WINAPI BankLoadingThread(void*);   // Thread body

// Bank information for lookup so there's no need to peek the SFXPak for the bank header
// This will eat 4MB of RAM (or less) from the process. But hey, what's 4MB nowdays?
static class CAEBankInfo* pBankInfo;


/*
 *  CAEBankInfo
 *      Stores information from a bank in memory, including it's filepath.
 */
class CAEBankInfo
{
    protected:
        friend DWORD WINAPI BankLoadingThread(void*);

        std::string           m_szFilepath; // BANK file
        BankHeader            m_Header;     // BANK header
        bool                  m_bHasBank;   // Structure has been initialized?
        short                 m_BankId;     // Global bank index
        CAEBankLookupItem*    m_pLookup;    // BANK lookup

    public:
        CAEBankInfo() : m_bHasBank(false), m_pLookup(0)
        {}

        // Sets the bank lookup and the bankid for this bank. Must be called before anything else.
        void SetLookup(CAEBankLookupItem* pLookup, int iBankId)
        {
            this->m_pLookup = pLookup;
            this->m_BankId  = static_cast<short>(iBankId);
        }

        // Loads header and other information from the BANK file
        bool GetBankFile(const char* szFilepath, unsigned int iOffset, signed int iSize);

        // Loads a Wave file into the bank (virtually)
        bool AddWaveFile(const char* szWavepath, unsigned short iSound);

        // Gets the bank header offset in the file
        unsigned int GetBankOffset()
        {
            return m_pLookup->m_dwOffset;
        }

        // Gets the bank sound buffer offset in the file
        unsigned int GetSoundOffset()
        {
            return GetBankOffset() + sizeof(BankHeader);
        }

        // Gets the bank sound buffer offset for the sound id @usSound in the file
        unsigned int GetSoundOffset(unsigned short usSound)
        {
            return GetSoundOffset() + m_Header.m_aSounds[usSound].m_dwOffset;
        }

        // Gets the bank sound buffer size
        unsigned int GetSoundSize()
        {
            return m_pLookup->m_dwSize;
        }

        // Gets the sound buffer size for the sound in @usSound
        unsigned int GetSoundSize(unsigned short usSound)
        {
            short nSoundsOnBank = this->m_Header.m_nSounds;

            // If it's the last sound on the bank we should use the bank size to find the 'diff'
            if(usSound >= nSoundsOnBank - 1)    // Diff between bank size and this sound offset
                return m_pLookup->m_dwSize - m_Header.m_aSounds[usSound].m_dwOffset;
            else                                // Diff between next sound offset and this sound offset
                return m_Header.m_aSounds[(usSound+1) % 400].m_dwOffset - m_Header.m_aSounds[usSound].m_dwOffset;
        }
};

/*
 *  CAECustomBankLoader::Finalize
 *      Finalizes all resources that the custom loader owns
 */
void CAECustomBankLoader::Finalize()
{
    // Cleanup resources
    CloseHandle(hThread);
    CloseHandle(hSemaphore);
    FinalizeQueue(&queue);
    delete[] pBankInfo;

    // Destroy any sound buffer still allocated
    for(int i = 0; i < this->m_usNumBankSlots; ++i)
    {
        operator delete((void*)(this->m_pBankSlots[i].m_dwOffsetOnBuffer));
        this->m_pBankSlots[i].m_dwOffsetOnBuffer = 0;
    }
}

/*
 *  CAECustomBankLoader::PostInitialise
 *      Called after CAEBankLoader::Initialise
 *      Custom initialization method
 */
bool CAECustomBankLoader::PostInitialise()
{
    // Initialise bank loading thread
	hSemaphore = CreateSemaphoreA(nullptr, 0, 50 + 1, "BankLoaderSem");
    if(hSemaphore)
    {
        hThread = CreateThread(nullptr, 0, BankLoadingThread, this, CREATE_SUSPENDED, nullptr);
        if(hThread)
        {
            // Make the loading thread have the same priority as the main thread
            SetThreadPriority(hThread, GetThreadPriority(GetCurrentThread()));
            ResumeThread(hThread);
        }
        else
        {
            // grrrh...
            CloseHandle(hSemaphore);
            return false;
        }
    }
    else
    {
        // Not good, what's going on?
        return false;
    }

    // Initialise bank loading queue
    InitialiseQueue(&queue, 50 + 1);

    // No sound buffer pre-allocation please...
    this->m_iSoundBuffersSize = 0;
    this->m_pSoundBuffers = nullptr;
    // ^ Trick: Use CAEBankSlot::m_dwOffsetOnBuffer with the actual allocated memory pointer and things will work!

    // Cleanup CAEBankSlot pre-allocation information
    for(int i = 0; i < this->m_usNumBankSlots; ++i)
    {
        // Those need to be set when you allocate your own buffer memory
        this->m_pBankSlots[i].m_dwOffsetOnBuffer = 0;
        this->m_pBankSlots[i].m_dwSlotBufferSize = 0;
    }

    // Now, pre-allocate bank information, that's better ;)
    pBankInfo = new CAEBankInfo[this->m_usNumBanks];
    this->InitialiseBankInfo();
    return true;
}

/*
 *  CAECustomBankLoader::InitialiseBankInfo
 *      Initialises bank information
 *      Make customizations to banks here!
 */

void CAECustomBankLoader::InitialiseBankInfo()
{
	// Open Mini SFX lookup
	// No need to cache this data, it can be read straight away
	if ( FILE* hFile = fopen("audio\\CONFIG\\minisfxLKup.dat", "rb") )
	{
		// If the lookup file doesn't exist, we're fucked
		for(int i = 0; i < this->m_usNumBanks; ++i)
		{
			auto*			p = &this->m_pBankLookup[i];
			unsigned char	bNameLen;

			fread(&bNameLen, sizeof(bNameLen), 1, hFile);
			std::string		a = std::string(bNameLen ? "AUDIO\\MINISFX\\" : "AUDIO\\SFX\\") + GetPakName(p->m_iPak);

			// If name length is 0 for this bank, it's not a Mini SFX bank
			if ( bNameLen )
			{
				char			miniSFXName[65];
				// Saves an operation on std::string!
				miniSFXName[0] = '\\';

				// Whoops?
				assert(bNameLen < 64);

				fread(miniSFXName+1, 1, bNameLen, hFile);
				// Hacky way to safely add a \0 at the end
				// Thanks to an assert before it's safe
				miniSFXName[bNameLen+1] = '\0';

				a += miniSFXName;
			}
			pBankInfo[i].SetLookup(p, i);
			pBankInfo[i].GetBankFile(a.c_str(), bNameLen ? 0 : p->m_dwOffset, bNameLen ? -1 : p->m_dwSize);

			// PS: You don't need to worry about bank lookup, GetBankFile will setup it!
		}

		fclose(hFile);
	}
}

/*
 *  CAEBankInfo::GetBankFile
 *      Reads bank information from file @szFilepath assuming the bank header is at @dwOffset and the banksize (without header) is dwSize.
 *      If dwSize is equal to -1 the size of the file minus the header size will be used.
 */
bool CAEBankInfo::GetBankFile(const char* szFilepath, unsigned int dwOffset, signed int dwSize)
{
    char szFullPath[MAX_PATH];
    bool bResult = false;

    if(!m_pLookup || !GetFullPathNameA(szFilepath, sizeof(szFullPath), szFullPath, nullptr))
        return false;

    // Open the file to check if it exists and to read the bank header
    HANDLE hFile = CreateFileA(szFullPath, GENERIC_READ, FILE_SHARE_READ, nullptr,
                               OPEN_EXISTING, 0, nullptr);

    if(hFile != INVALID_HANDLE_VALUE)
    {
        OVERLAPPED ov = {0};
        ov.OffsetHigh = 0;
        ov.Offset     = dwOffset;

        // Read the bank header
        if(ReadFile(hFile, &this->m_Header, sizeof(this->m_Header), NULL, &ov))
        {
            // Check if dwSize is the file size...
            if(dwSize == -1) dwSize = GetFileSize(hFile, 0) - sizeof(m_Header);

            // Setup information
            this->m_pLookup->m_dwOffset = dwOffset;
            this->m_pLookup->m_dwSize = dwSize;
            this->m_szFilepath = szFullPath;
            this->m_bHasBank = bResult = true;
        }

        CloseHandle(hFile);
    }

    return bResult;
}


/*
 *  CAECustomBankLoader::Service
 *      Processes the bank loading
 */
void CAECustomBankLoader::Service()
{
    // Process sound requests
    for(int i = 0; i < 50 && this->m_nRequestsToLoad; ++i)
    {
        auto& r = this->m_aSoundRequests[i];
        unsigned short bankslot = r.m_usBankSlot;

        switch(r.m_iLoadingStatus)
        {
            // The request has just been requested (oh well...)
            case REQUEST_STATUS_BEGIN:
            {
                auto& b = this->m_pBankSlots[bankslot];

                // Mark the bankslot as free, we don't want anyone using it while we're touching it!
                r.m_pBuffer = (void*) b.m_dwOffsetOnBuffer;
                b.m_dwOffsetOnBuffer = 0;
                memset(b.m_aBankItems, 0, sizeof(b.m_aBankItems));
                this->m_pBankSlots[bankslot].m_usBankNum = 0xFFFF;
                this->m_aBankSlotSound[bankslot] = 0xFFFF;

                // Request the sound to the bank loading thread
                r.m_iLoadingStatus = REQUEST_STATUS_IN_PROGRESS;
                AddToQueue(&queue, i);
                ReleaseSemaphore(hSemaphore, 1, nullptr);

                break;
            }

            // The request has been completed, finish it
            case REQUEST_STATUS_DONE:
            {
                auto& b = this->m_pBankSlots[bankslot];

                // Mark the bankslot with the loaded bank/sound
                this->m_pBankSlots[bankslot].m_usBankNum = r.m_usBank;
                this->m_aBankSlotSound[bankslot] = r.m_usSound;
                b.m_dwOffsetOnBuffer = (uintptr_t)(r.m_pBuffer);
                memcpy(b.m_aBankItems, r.m_pBufferData, sizeof(b.m_aBankItems));

                // Special setup for single sounds
                if(r.m_usSound != 0xFFFF)
                {
                    b.m_nSoundsOnBank = 0xFFFF;
					b.m_aBankItems[r.m_usSound].m_dwOffset = 0;
                    b.m_aBankItems[(r.m_usSound + 1) % 400].m_dwOffset = b.m_dwSlotBufferSize;
                }

                // Cleanup request object
                r.m_iLoadingStatus = REQUEST_STATUS_NULL;
                r.m_usBankSlot = -1;
                r.m_usBank = 0xFFFF;
                r.m_usSound = 0xFFFF;
                r.m_pBuffer = r.m_pBufferData = nullptr;
                --this->m_nRequestsToLoad;

                break;
            }
        }
    }
}


/*
 *  BankLoadingThread
 *      Oh well, this is the hard worker that will load the bank files
 */
DWORD WINAPI BankLoadingThread(void* arg)
{
    CAECustomBankLoader& AEBankLoader = *(CAECustomBankLoader*)(arg);
    OVERLAPPED ov = {0};
    HANDLE hFile;

    for (;;)
    {
        // Wait for a request...
		WaitForSingleObject(hSemaphore, INFINITE);
        int i = GetFirstInQueue(&queue);

        // Setup references for helping us, neh
        auto& r = AEBankLoader.m_aSoundRequests[i];             // The sound request
        auto& b = AEBankLoader.m_pBankSlots[r.m_usBankSlot];    // The bank slot
        auto& f = pBankInfo[r.m_usBank];                        // The bank information

        //
        hFile = CreateFileA(f.m_szFilepath.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr,
                            OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, nullptr);

        if(hFile != INVALID_HANDLE_VALUE)
        {
            bool  bSingleSound= r.m_usSound != 0xFFFF;      // Is loading a single sound instead of the entire bank?
            auto  iSound      = r.m_usSound;                // Which sound?
            auto  dwSize      = bSingleSound? f.GetSoundSize(iSound)   : f.GetSoundSize();
            auto  dwOffset    = bSingleSound? f.GetSoundOffset(iSound) : f.GetSoundOffset();
            void* pBuffer     = (void*) r.m_pBuffer;

            // Setup reading offset
            ov.OffsetHigh = 0;
            ov.Offset     = dwOffset;

			// If old buffer is smaller or bigger by more than 8KB, realloc
			if ( dwSize > b.m_dwSlotBufferSize || (dwSize + 8192) < b.m_dwSlotBufferSize )
			{
				// Cleanup the old buffer
				operator delete(pBuffer);

				// Allocate new sound buffer
				pBuffer = operator new(dwSize);
			}

            if(!ReadFile(hFile, pBuffer, dwSize, nullptr, &ov))
            {
                // Failed to read!
            }

            // Setup the bankslot data
            r.m_pBuffer = pBuffer;
            r.m_pBufferData = f.m_Header.m_aSounds;
            b.m_dwSlotBufferSize = dwSize;
            b.m_nSoundsOnBank = f.m_Header.m_nSounds;

            CloseHandle(hFile);
        }
        else
        {
            // That's baddd
			assert(!"Baad");
        }

        // Done!
        RemoveFirstInQueue(&queue);
		r.m_iLoadingStatus = REQUEST_STATUS_DONE;
    }

	// Unreachable anyway
    //return TRUE;
}


/*
 *  CAECustomBankLoader::Patch
 *      Patches the game to use this custom bank loader
 */

// I'm going to use this little wrapper to avoid compiler warnings
static void __fastcall ServiceCaller(CAEBankLoader* loader)
{
    return static_cast<CAECustomBankLoader*>(loader)->Service();
}

// Return a null pointer
static void* ReturnNull()
{
    return nullptr;
}

static bool __fastcall InitialisationHook(void* pObject)
{
	// Stock initialisation func
	if ( ((bool(__thiscall*)(void*))0x4E08F0)(pObject) )
		return static_cast<CAECustomBankLoader*>(pObject)->PostInitialise();

	return false;
}

static void __fastcall DestructionHook(void* pObject)
{
	static_cast<CAECustomBankLoader*>(pObject)->Finalize();

	// Stock destruction func
	((void(__thiscall*)(void*))0x4DFB20)(pObject);
}

// Patcher
void CAECustomBankLoader::Patch()
{
	using namespace Memory;

    //
	InjectHook(0x4DFE30, ServiceCaller, PATCH_JUMP);
	InjectHook(0x4D99B3, InitialisationHook);
	InjectHook(0x4D9800, DestructionHook);
	InjectHook(0x4E065B, ReturnNull);
	InjectHook(0x4DFD9D, ReturnNull);
	Nop(0x4DFDC3, 5);
	Nop(0x4DFDCE, 7);
   /* MakeJMP (0x4DFE30, raw_ptr(ServiceCaller));
    MakeCALL(0x4E065B, raw_ptr(ReturnNull));    // Return null bankslot pre-allocated memory
    MakeCALL(0x4DFD9D, raw_ptr(ReturnNull));    // Return null streaming handle for SFXPak
    MakeNOP (0x4DFDC3, 5);                      // Don't free PakFiles.dat buffer
    MakeNOP (0x4DFDCE, 7);                      // ^*/
}
