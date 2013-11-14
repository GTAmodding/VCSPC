#ifndef _CCRC32_H
#define _CCRC32_H
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CCRC32{

	public:
		CCRC32(void);
		~CCRC32(void);

		void Initialize(void);

		bool FileCRC(const wchar_t *sFileName, unsigned long *ulOutCRC);
		bool FileCRC(const wchar_t *sFileName, unsigned long *ulOutCRC, unsigned long ulBufferSize);

		unsigned long FullCRC(const unsigned char *sData, unsigned long ulDataLength);
		void FullCRC(const unsigned char *sData, unsigned long ulLength, unsigned long *ulOutCRC);

		void PartialCRC(unsigned long *ulCRC, const unsigned char *sData, unsigned long ulDataLength);

	private:
		unsigned long Reflect(unsigned long ulReflect, const char cChar);
		unsigned long ulTable[256]; // CRC lookup table array.
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
