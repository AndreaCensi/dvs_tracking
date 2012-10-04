/************************************************************************
 *
 *  Module:       SetupApiDll.h
 *  Long name:    CSetupApiDll class
 *  Description:  handling of the setupapi.dll
 *
 *  Runtime Env.: Win32
 *  Author(s):    Guenter Hildebrandt, Udo Eberhardt
 *  Company:      Thesycon GmbH, Ilmenau
 ************************************************************************/

#ifndef _SetupApiDll_h_
#define _SetupApiDll_h_

// In Borland C++, we have to force byte alignment for setupapi.h.
// This is necessary because there seems to be a problem with the
// implementations of pshpackX.h and poppack.h.
#ifdef __BORLANDC__
#pragma pack(1)
#endif

// get setup API functions (only available in Win98 and later systems)
#include <setupapi.h>
#include <cfgmgr32.h>

// restore previous settings
#ifdef __BORLANDC__
#pragma pack()
#endif



//
// CSetupApiDll
//
class CSetupApiDll
{
public:

  // standard constructor
  CSetupApiDll();

  // destructor
  ~CSetupApiDll();


  // load setupapi.dll
  // returns TRUE if successful, FALSE otherwise
  BOOL Load();

  // release setupapi.dll
  void Release();


// implementation
protected:
  
  // init function pointers
  void InitPointers();


  // We have to repeat some prototypes from setupapi.h.
  // Unfortunately, the .h file does not define types for the functions.

  typedef
  WINSETUPAPI
  HDEVINFO 
  WINAPI
  F_SetupDiGetClassDevs(
    IN LPGUID ClassGuid,  OPTIONAL
    IN LPCTSTR Enumerator, OPTIONAL
    IN HWND   hwndParent, OPTIONAL
    IN DWORD  Flags
    );

  typedef 
  WINSETUPAPI
  BOOL
  WINAPI
  F_SetupDiDestroyDeviceInfoList(
    HDEVINFO DeviceInfoSet
    );

  typedef 
  WINSETUPAPI
  BOOL 
  WINAPI
  F_SetupDiEnumDeviceInterfaces(
    IN  HDEVINFO                  DeviceInfoSet,
    IN  PSP_DEVINFO_DATA          DeviceInfoData,     OPTIONAL
    IN  LPGUID                    InterfaceClassGuid,
    IN  DWORD                     MemberIndex,
    OUT PSP_DEVICE_INTERFACE_DATA DeviceInterfaceData
    );

  typedef
  WINSETUPAPI 
  BOOL 
  WINAPI
  F_SetupDiGetDeviceInterfaceDetail(
    IN  HDEVINFO                           DeviceInfoSet,
    IN  PSP_DEVICE_INTERFACE_DATA          DeviceInterfaceData,
    OUT PSP_DEVICE_INTERFACE_DETAIL_DATA	 DeviceInterfaceDetailData,     OPTIONAL
    IN  DWORD                              DeviceInterfaceDetailDataSize,
    OUT PDWORD                             RequiredSize,                  OPTIONAL
    OUT PSP_DEVINFO_DATA                   DeviceInfoData                 OPTIONAL
    );

	typedef
	CMAPI
	CONFIGRET
	WINAPI
	F_CM_Get_Parent(
			PDEVINST pdnDevInst,
			DEVINST dnDevInst,
			ULONG ulFlags
			);

	typedef
	CMAPI
	CONFIGRET
	WINAPI
	F_CM_Get_Device_ID(
			DEVINST dnDevInst,
			PTCHAR Buffer,
			ULONG BufferLen,
			ULONG ulFlags
			);

  // handle for DLL
  HMODULE mDllHandle;


  // function pointers
  // We make the pointers public to simplify access by external code.
public:
  F_SetupDiGetClassDevs*              SetupDiGetClassDevs;
  F_SetupDiDestroyDeviceInfoList*     SetupDiDestroyDeviceInfoList;
  F_SetupDiEnumDeviceInterfaces*      SetupDiEnumDeviceInterfaces;
  F_SetupDiGetDeviceInterfaceDetail*  SetupDiGetDeviceInterfaceDetail;
  F_CM_Get_Parent*										CM_Get_Parent;
	F_CM_Get_Device_ID*								  CM_Get_Device_ID;
}; // class CSetupApiDll

#endif // _SetupApiDll_h_

/*************************** EOF **************************************/
