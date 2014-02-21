#pragma once

struct CAEBankLookupItem
{
  char 			m_iPak;
  unsigned int m_dwOffset;
  unsigned int m_dwSize;
};

struct CAEBankSlotItem
{
  unsigned int m_dwOffset;
  unsigned int m_dwLoopOffset;
  unsigned short m_usSampleRate;
  short m_usSoundHeadroom;
};


struct CAEBankSlot
{
  unsigned int m_dwOffsetOnBuffer;
  unsigned int m_dwSlotBufferSize;
  int _unused1;
  int _unused2;
  unsigned short m_usBankNum;
  unsigned short m_nSoundsOnBank;
  CAEBankSlotItem m_aBankItems[400];
};

struct CAESoundRequest
{
  CAEBankSlot *m_pBankSlot;
  unsigned int m_dwOffset;
  unsigned int m_dwSize;
  void *m_pBufferData;
  void *m_pBuffer;
  int m_iLoadingStatus;
  unsigned short m_usBank;
  short m_usBankSlot;
  unsigned short m_usSound;
  char m_iPak;
};

struct CAEBankLoader
{
  CAEBankSlot *m_pBankSlots;
  CAEBankLookupItem *m_pBankLookup;
  char *m_pPakFiles;
  short m_usNumBankSlots;
  short m_usNumBanks;
  short m_usNumPakFiles;
  short _pad0;
  char m_bInitialized;
  unsigned int m_iSoundBuffersSize;
  char *m_pSoundBuffers;
  int *m_StreamHandles;
  CAESoundRequest m_aSoundRequests[50];
  short _unk0;
  short m_nRequestsToLoad;
  short m_iRequestListNext;
  short m_iStreamingChannel;
  unsigned short m_aBankSlotSound[45];
  char _unused1[30];
};

struct BankHeader
{
    unsigned short        m_nSounds;
    CAEBankSlotItem       m_aSounds[400];
};

/*
 *  CAECustomBankLoader
 *      Custom bank loader for the game
 */
class CAECustomBankLoader : public CAEBankLoader
{
    public:
        static void Patch();        // Patch the game code to use our custom bank loader

        bool PostInitialise();      // Initialise stuff
        void Finalize();            // Finalizes stuff
        void Service();             // Custom bank loading methods

        void InitialiseBankInfo();  // Initialise bank information pre-allocation

        // Get SFXPak filename from it's index
        const char* GetPakName(unsigned char i)
        {
            return &this->m_pPakFiles[52 * i];
        }

};

static_assert(sizeof(CAESoundRequest)   == 0x20,    "Incorrect struct size: CAESoundRequest");
static_assert(sizeof(CAEBankSlot)       == 0x12D4,  "Incorrect struct size: CAEBankSlot");
static_assert(sizeof(CAEBankLookupItem) == 0x0C,    "Incorrect struct size: CAEBankLookupItem");
static_assert(sizeof(CAEBankLoader)     == 0x6E4,   "Incorrect struct size: CAEBankLoader");
