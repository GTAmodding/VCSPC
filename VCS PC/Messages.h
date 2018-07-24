#ifndef __MESSAGES
#define __MESSAGES

class CMessages
{
public:
	static void		InsertNumberInString(const char* pszText, int nNumber1, int nNumber2, int nNumber3, int nNumber4, int nNumber5, int nNumber6, char* pszOutBuf);
	static void		InsertPlayerControlKeysInString(char* pszText);
	static void		AddToPreviousBriefArray(const char* pszText, int nNumber1, int nNumber2, int nNumber3, int nNumber4, int nNumber5, int nNumber6, char* pszOutBuf);
	static void		AddBigMessage(const char* pText, unsigned int nTime, unsigned short nStyle);
	static void		AddMessageJumpQ(const char* pText, unsigned int nTime, unsigned short, bool);
	static char		StringCompare(char *str1, char *str2, unsigned __int16 len);
	static int		StringCopy(char * dest, char * src, unsigned __int16 len);
};

#endif