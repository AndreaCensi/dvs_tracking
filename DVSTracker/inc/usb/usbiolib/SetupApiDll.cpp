/************************************************************************
 *
 *  Module:       SetupApiDll.cpp
 *  Long name:    CSetupApiDll class
 *  Description:  handling of the setupapi.dll
 *
 *  Runtime Env.: Win32
 *  Author(s):    Guenter Hildebrandt, Udo Eberhardt
 *  Company:      Thesycon GmbH, Ilmenau
 ************************************************************************/

#include "stdafx.h"

// for shorter and faster windows.h
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif
#include <windows.h>
#include <tchar.h>
#include "SetupApiDll.h"


// helper macro
#ifdef  UNICODE
#define _T_FUNC_NAME(x)	x "W"
#else
#define _T_FUNC_NAME(x)	x "A"
#endif


// standard constructor
CSetupApiDll::CSetupApiDll()
{
	mDllHandle = NULL;
	InitPointers();
}


// destructor
CSetupApiDll::~CSetupApiDll()
{
	Release();
}


// init function pointers
void CSetupApiDll::InitPointers()
{
  SetupDiGetClassDevs = NULL;
  SetupDiDestroyDeviceInfoList = NULL;
  SetupDiEnumDeviceInterfaces = NULL;
  SetupDiGetDeviceInterfaceDetail = NULL;
	CM_Get_Parent = NULL;
	CM_Get_Device_ID = NULL;
}



// load setupapi.dll
BOOL CSetupApiDll::Load()
{
  if ( mDllHandle!=NULL ) {
    // library is already loaded
    return TRUE;
  }

  // try to load the DLL
  mDllHandle = ::LoadLibrary(_T("setupapi.dll"));
  if ( mDllHandle==NULL ) {
    // failed to load library
    return FALSE;
  }
  // DLL successfully loaded
  BOOL succ = TRUE;
  
	// try to get the function pointers we need
  SetupDiGetClassDevs = (F_SetupDiGetClassDevs*)::GetProcAddress(mDllHandle, _T_FUNC_NAME("SetupDiGetClassDevs"));
  if ( SetupDiGetClassDevs==NULL ) succ = FALSE;

  SetupDiDestroyDeviceInfoList = (F_SetupDiDestroyDeviceInfoList*)::GetProcAddress(mDllHandle, "SetupDiDestroyDeviceInfoList");
  if ( SetupDiDestroyDeviceInfoList==NULL ) succ = FALSE;

  SetupDiEnumDeviceInterfaces = (F_SetupDiEnumDeviceInterfaces*)::GetProcAddress(mDllHandle, "SetupDiEnumDeviceInterfaces");
  if ( SetupDiEnumDeviceInterfaces==NULL ) succ = FALSE;

  SetupDiGetDeviceInterfaceDetail = (F_SetupDiGetDeviceInterfaceDetail*)::GetProcAddress(mDllHandle, _T_FUNC_NAME("SetupDiGetDeviceInterfaceDetail"));
  if ( SetupDiGetDeviceInterfaceDetail==NULL ) succ = FALSE;

  CM_Get_Parent = (F_CM_Get_Parent*)::GetProcAddress(mDllHandle,"CM_Get_Parent");
  if ( CM_Get_Parent==NULL ) succ = FALSE;

	CM_Get_Device_ID = (F_CM_Get_Device_ID*)::GetProcAddress(mDllHandle,_T_FUNC_NAME("CM_Get_Device_ID"));
  if ( CM_Get_Device_ID==NULL ) succ = FALSE;

  // all pointers ok?
  if ( !succ ) {
    // no, release DLL
    Release();
  }
  
	return succ;
}



// release setupapi.dll
void CSetupApiDll::Release()
{
	if ( mDllHandle!=NULL ) {
		::FreeLibrary(mDllHandle);
		mDllHandle = NULL;
		InitPointers();
	}
}





/*************************** EOF **************************************/
