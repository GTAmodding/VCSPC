#include "stdafx.h"
#include "Rs.h"

const DWORD RsGlobalFrameLimits[] = { 0, 25, 30, 50, 60 };

WRAPPER RsEventStatus RsEventHandler(RsEvent eventID, void* param) { WRAPARG(eventID); WRAPARG(param); EAXJMP(0x619B60); }

RwChar* RsPathnameCreate(const RwChar* srcBuffer)
{
	size_t		len = strlen(srcBuffer) + 1;
	RwChar*		pMem = static_cast<RwChar*>(malloc(len));

	if ( pMem )
	{
		strncpy(pMem, srcBuffer, len);
		while ( RwChar* pChar = strchr(pMem, '/') )
			*pChar = '\\';
	}
	return pMem;
}

void RsPathnameDestroy(RwChar* buffer)
{
	if ( buffer )
		free(buffer);
}

BOOL RsCameraBeginUpdate(RwCamera* pCamera)
{
	if ( RwCameraBeginUpdate(pCamera) )
		return TRUE;

	RsEventHandler(rsACTIVATE, nullptr);
	return FALSE;
}