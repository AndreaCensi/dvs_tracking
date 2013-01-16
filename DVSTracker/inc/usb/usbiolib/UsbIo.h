/************************************************************************
 *
 *  Module:       UsbIo.h
 *  Long name:    CUsbIo class
 *  Description:  CUsbIo base device class definition
 *
 *  Runtime Env.: Win32, Part of UsbioLib
 *  Author(s):    Guenter Hildebrandt, Udo Eberhardt, Mario Guenther
 *  Company:      Thesycon GmbH, Ilmenau
 ************************************************************************/

#ifndef _UsbIo_h_
#define _UsbIo_h_

// windows.h compiles with W3 by default
// we may want to use W4
#pragma warning(push,3)

// get Win32 IOCTL support
#include <winioctl.h>

#pragma warning(pop)

// get setup API functions
#include "SetupApiDll.h"

// get driver interface definitions
#include "usbspec.h"
#include "usbio_i.h"

//
// define the calling convention used by USBIOLIB
//
#if (_MSC_VER >= 800)
#define USBIOLIB_CALL __stdcall
#else
#define USBIOLIB_CALL __cdecl
#endif


//
// Device names created by the USBIO driver
//
// !!! NOTE !!!
// This device names should not be used anymore. Use it only if compatibility to
// earlier versions of USBIO is required.
// Use device interfaces identified by GUIDs instead!
// If the old-style device names (USBIO_DeviceX) are required to ensure
// backward-compatibility to earlier versions of USBIO they must be enabled
// explicitly in USBIO.INF. See manual.
//
//
// old-style device naming:
//
// The string defined here MUST match to the string defined by the
// USBIO driver configuration parameter 'USBIO_DeviceBaseName' in the registry.
//
// One digit must be appended for device enumeration.
// e.g. \\.\USBIO_Device0, \\.\USBIO_Device1
//
#define USBIO_DEVICE_NAME      "USBIO_Device"


//
// CUsbIo
//
// USBIO programming interface
//
class CUsbIo
{
public:

  //
  // standard constructor
  //
  CUsbIo();

  //
  // destructor
  //
  virtual ~CUsbIo();


 
  //
  // create list of available devices
  // returns handle
  //
  static
  HDEVINFO 
	USBIOLIB_CALL
  CreateDeviceList(
    const GUID* InterfaceGuid
    );
  
  //
  // destroy device list
  //
  static 
  void 
	USBIOLIB_CALL
  DestroyDeviceList(
    HDEVINFO DeviceList
    );



  //
  // open device
  // returns status
  //
  DWORD
  Open(
    int DeviceNumber, 
    HDEVINFO DeviceList, 
		const GUID* InterfaceGuid,
		bool CheckApiVersion = true
    );

	// 
	// open the device with a known device path
	//
	DWORD
	OpenPath(
		const TCHAR* DevicePath,
		bool CheckApiVersion = true
		);

  //
  // close device
  //
  void
  Close();
  
  //
  // get detailed info on a device instance
  //
  DWORD
  GetDeviceInstanceDetails(
    int DeviceNumber, 
    HDEVINFO DeviceList, 
    const GUID* InterfaceGuid
    );

  //
  // get device path name
  //
  const TCHAR*
  GetDevicePathName();

  //
	// get the parent identifier
	//
	const TCHAR*
	GetParentID();


  //
  // Returns TRUE if the class instance is attached to a device.
  //
  BOOL
  IsOpen() 
    { return (FileHandle!=NULL); }

  //
  // Returns TRUE if the checked build (debug version) 
  // of the USBIO driver was detected on Open.
  //
  BOOL
  IsCheckedBuild() 
    { return CheckedBuildDetected; }

  //
  // Returns TRUE if the Demo version
  // of the USBIO driver was detected on Open.
  //
  BOOL
  IsDemoVersion()  
    { return DemoVersionDetected; }

  //
  // Returns TRUE if the Light version
  // of the USBIO driver was detected on Open.
  //
  BOOL
  IsLightVersion()
    { return LightVersionDetected; }

  //
  // Returns TRUE if the device is operating at high speed.
  //
  BOOL
  IsOperatingAtHighSpeed();


  //
  // get driver info
  // returns status
  //
  DWORD
  GetDriverInfo(
    USBIO_DRIVER_INFO* DriverInfo
    );


  //
  // acquire the device for exclusive use
  // returns status
  //
  DWORD
  AcquireDevice();


  //
  // release the exclusive usage for this device
  // returns status
  //
  DWORD
  ReleaseDevice();

  //
  // get information on the USB device
  // returns status
  //
  DWORD
  GetDeviceInfo(
    USBIO_DEVICE_INFO* DeviceInfo
    );


  //
  // get information about the USB bandwidth consumption
  // returns status
  //
  DWORD
  GetBandwidthInfo(
    USBIO_BANDWIDTH_INFO* BandwidthInfo
    );


  //
  // get descriptor
  // returns status
  //
  DWORD 
  GetDescriptor(
    void* Buffer,
    DWORD& ByteCount,
    USBIO_REQUEST_RECIPIENT Recipient,
    UCHAR DescriptorType,
    UCHAR DescriptorIndex =0,
    USHORT LanguageId =0
    );

  //
  // support for standard descriptors: device descriptor
  // implemented as wrapper for GetDescriptor
  // returns status
  //
  DWORD
  GetDeviceDescriptor(
    USB_DEVICE_DESCRIPTOR* Desc
    );

  //
  // support for standard descriptors: configuration descriptor
  // implemented as wrapper for GetDescriptor
  // returns status
  //
  DWORD
  GetConfigurationDescriptor(
    USB_CONFIGURATION_DESCRIPTOR* Desc,
    DWORD& ByteCount,
    UCHAR Index =0
    );

  //
  // support for standard descriptors: string descriptor
  // implemented as wrapper for GetDescriptor
  // returns status
  //
  DWORD
  GetStringDescriptor(
    USB_STRING_DESCRIPTOR* Desc,
    DWORD& ByteCount,
    UCHAR Index =0,
    USHORT LanguageId =0
    );

  //
  // set descriptor
  // returns status
  //
  DWORD
  SetDescriptor(
    const void* Buffer,
    DWORD& ByteCount,
    USBIO_REQUEST_RECIPIENT Recipient,
    UCHAR DescriptorType,
    UCHAR DescriptorIndex =0,
    USHORT LanguageId =0
    );

  //
  // set feature
  // returns status
  //
  DWORD 
  SetFeature(
    USBIO_REQUEST_RECIPIENT Recipient,
    USHORT FeatureSelector,
    USHORT Index =0
    );
  
  //
  // clear feature
  // returns status
  //
  DWORD
  ClearFeature(
    USBIO_REQUEST_RECIPIENT Recipient,
    USHORT FeatureSelector,
    USHORT Index =0 
    );

  //
  // get status
  // returns status
  //
  DWORD 
  GetStatus(
    USHORT& StatusValue,
    USBIO_REQUEST_RECIPIENT Recipient,
    USHORT Index =0 
    );

  //
  // class or vendor specific IN request
  // returns status
  //
  DWORD
  ClassOrVendorInRequest(
    void* Buffer,
    DWORD& ByteCount,
    const USBIO_CLASS_OR_VENDOR_REQUEST* Request
    );

  //
  // class or vendor specific OUT request
  // returns status
  //
  DWORD
  ClassOrVendorOutRequest(
    const void* Buffer,
    DWORD& ByteCount,
    const USBIO_CLASS_OR_VENDOR_REQUEST *Request
    );



  //
  // set configuration
  // returns status
  //
  DWORD
  SetConfiguration(
    const USBIO_SET_CONFIGURATION* Conf
    );

  //
  // unconfigure device
  // returns status
  //
  DWORD
  UnconfigureDevice();

  //
  // get configuration
  // returns status
  //
  DWORD 
  GetConfiguration(
    UCHAR& ConfigurationValue 
    );

  //
  // get device configuration information
  // returns status
  //
  DWORD
  GetConfigurationInfo(
    USBIO_CONFIGURATION_INFO* Info
    );

  //
  // set interface
  // returns status
  //
  DWORD 
  SetInterface(
    const USBIO_INTERFACE_SETTING* Setting
    );

  //
  // get interface
  // returns status
  //
  DWORD 
  GetInterface(
    UCHAR& AlternateSetting,
    USHORT Interface =0
    );

  //
  // obsoleted 
  // returns status
  //
  DWORD
  StoreConfigurationDescriptor(
    const USB_CONFIGURATION_DESCRIPTOR* Desc
    );



  //
  // get device-related parameters
  // returns status
  //
  DWORD
  GetDeviceParameters(
    USBIO_DEVICE_PARAMETERS* DevParam
    );

  //
  // set device-related parameters
  // returns status
  //
  DWORD 
  SetDeviceParameters(
    const USBIO_DEVICE_PARAMETERS* DevParam
    );

  //
  // reset device
  // returns status
  //
  DWORD
  ResetDevice();

  //
  // cycle port
  // simulates a device disconnect/connect cycle
  // this will unload/load the USBIO driver
  // returns status
  //
  DWORD
  CyclePort();

  //
  // get current USB frame number from host controller
  // returns status
  //
  DWORD
  GetCurrentFrameNumber(
    DWORD& FrameNumber
    );

  //
  // get current device power state
  // returns status
  //
  DWORD
  GetDevicePowerState(
    USBIO_DEVICE_POWER_STATE& DevicePowerState
    );

  //
  // set device power state
  // returns status
  //
  DWORD
  SetDevicePowerState(
    USBIO_DEVICE_POWER_STATE DevicePowerState
    );



  //
  // a wrapper function for the Win32 function CancelIo
  // cancels all outstanding requests that were
  // issued by the calling thread on the file handle.
  // Note: Requests issued on the handle by other threads are NOT cancelled.
  //
  BOOL
  CancelIo();

  //
  // support function:
  // call a driver IOCTL function and wait for completion
  // returns status
  //
  DWORD
  IoctlSync(
    DWORD IoctlCode,
    const void *InBuffer,
    DWORD InBufferSize,
    void *OutBuffer,
    DWORD OutBufferSize,
    DWORD *BytesReturned
    );


	const SP_DEVINFO_DATA&
	GetDevInfoData()
		{
			return mDevInfoData;
		}

  //
  // translate an error code to a description string
  // prints to StringBuffer and returns StringBuffer
  //
  static
  TCHAR* 
	USBIOLIB_CALL
  ErrorText(
    TCHAR* StringBuffer,
    DWORD MaxCharCount,
    DWORD ErrorCode
    );


	//
	// returns a reference to the static CSetupApiDll member
	//
	static
	CSetupApiDll&
	SetupApiDll()
		{	return smSetupApi; }

		

// implementation
protected:

  // handle for device object, NULL if invalid
  HANDLE FileHandle;

  // embedded overlapped struct, used by IoctlSync
  OVERLAPPED Overlapped;
 
  // use to synchronize IOCTL operations on FileHandle
  CRITICAL_SECTION CritSect;

  // flags derived from USBIO_DRIVER_INFO
  BOOL CheckedBuildDetected;
  BOOL DemoVersionDetected;
  BOOL LightVersionDetected;

private:
  // detailed information on device instance, used internally
  SP_DEVICE_INTERFACE_DETAIL_DATA* mDevDetail;

	SP_DEVINFO_DATA mDevInfoData;
	TCHAR* mParentIdStr;


protected:

  // The setupapi dll is loaded only once per process.
  // Therefore, we use a static member.
  static CSetupApiDll smSetupApi;


}; // class CUsbIo

#endif // _UsbIo_h_

/*************************** EOF **************************************/
