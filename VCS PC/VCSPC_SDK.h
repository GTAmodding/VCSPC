/***********************************************************************************
*************** GRAND THEFT AUTO: VICE CITY STORIES PC EDITION *********************
***************      SOFTWARE DEVELOPMENT KIT HEADER FILE      *********************
***********************************************************************************/

//  This SDK allows you to take advantage of new VCS PC features when scripting either
//  ASI plugins or CLEO scripts. Functions are available in a form of C/C++ compliant
//  functions which can be referenced directly in the plugin code, and also in a form
// of SCM wrapper around these functions in order to allow convenient usage from CLEO
// scripts level.

#ifndef __VCSPCSDK
#define __VCSPCSDK

#ifdef __cplusplus
extern "C" {
#endif

#ifndef VCSPCSDKLINK
#define VCSPCSDKLINK __declspec(dllimport)
#endif

#define VCSAPI __stdcall


// Macroes

// This one future-proofs the SDK for standalone SA widescreen fix
#define VCS_GetGameMetrics(pOutWidth, pOutHeight) GetGameMetrics((pOutWidth), (pOutHeight))


// Exported functions


// This function returns fullscreen coordinates which change depending on selected
// aspect ratio. By using basic arithmetic on output value you can determine screen's
// center which then can be used in C/SCM draws etc.

// Can be used in both ASI and CLEO scripts
VCSPCSDKLINK void VCSAPI GetGameMetrics(float* pOutWidth, float* pOutHeight);


#ifdef __cplusplus
}
#endif

#endif