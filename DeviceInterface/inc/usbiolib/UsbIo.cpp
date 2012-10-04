/************************************************************************
 *
 *  Module:       UsbIo.cpp
 *  Long name:    CUsbIo class
 *  Description:  CUsbIo base device class implementation
 *
 *  Runtime Env.: Win32, Part of UsbioLib
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
#include <stdio.h>
#include "usbio.h"


// static members
CSetupApiDll CUsbIo::smSetupApi;



// standard constructor
CUsbIo::CUsbIo()
{
  FileHandle = NULL;
  ZeroMemory(&Overlapped,sizeof(Overlapped));
	ZeroMemory(&mDevInfoData,sizeof(mDevInfoData));
  InitializeCriticalSection(&CritSect);
  CheckedBuildDetected = FALSE;
  DemoVersionDetected = FALSE;
  LightVersionDetected = FALSE;
  mDevDetail = NULL;
	mParentIdStr = NULL;
}


// destructor
CUsbIo::~CUsbIo()
{
  // close file handle
  Close();
  // free resources
  DeleteCriticalSection(&CritSect);

}



//static
HDEVINFO 
USBIOLIB_CALL
CUsbIo::CreateDeviceList(const GUID *InterfaceGuid)
{
  HDEVINFO h;

  // make sure the setupapi dll is loaded
  if ( !smSetupApi.Load() ) {
    return NULL;
  }

  h = (smSetupApi.SetupDiGetClassDevs)(
        (GUID*)InterfaceGuid,                 // LPGUID ClassGuid, 
        NULL,                                 // LPCTSTR Enumerator, 
        NULL,                                 // HWND hwndParent, 
        DIGCF_DEVICEINTERFACE | DIGCF_PRESENT // DWORD Flags
        );
  return ( (h==INVALID_HANDLE_VALUE) ? NULL : h );
}


//static
void
USBIOLIB_CALL
CUsbIo::DestroyDeviceList(HDEVINFO DeviceList)
{
  // make sure the setupapi dll is loaded
  if ( !smSetupApi.Load() ) {
    return;
  }

  if ( DeviceList!=NULL ) {
    (smSetupApi.SetupDiDestroyDeviceInfoList)(DeviceList);
  }
}


DWORD CUsbIo::Open(int DeviceNumber, HDEVINFO DeviceList, const GUID* InterfaceGuid, bool CheckApiVersion /*= true*/)
{
  DWORD Status;
  TCHAR NameBuffer[80];
  const TCHAR* Name;

  if ( FileHandle != NULL ) {
    // already open
    return USBIO_ERR_DEVICE_ALREADY_OPENED;
  } 
  
  if ( DeviceList == NULL ) {
    // use the old way, using a well-known device name
    // build device name
    _sntprintf(NameBuffer, sizeof(NameBuffer)/sizeof(NameBuffer[0]), _T("\\\\.\\") _T(USBIO_DEVICE_NAME) _T("%d"), DeviceNumber);
    Name = NameBuffer;
  } else {
    // use the device interface identified by InterfaceGuid
    // a GUID must be provided in this case
    Status = GetDeviceInstanceDetails(DeviceNumber, DeviceList, InterfaceGuid);
    if ( Status != USBIO_ERR_SUCCESS ) {
      return Status;
    }
    // get name
    Name = GetDevicePathName();
  }

	Status = OpenPath(Name,CheckApiVersion);
  return Status;
}

DWORD
CUsbIo::OpenPath(
	const TCHAR* Name,
	bool CheckApiVersion /*= true*/
	)
{
	DWORD Status;
	HANDLE h;
	
	if ( FileHandle != NULL ) {
		// already open
		return USBIO_ERR_DEVICE_ALREADY_OPENED;
	}

	// try to open the device driver
	h = ::CreateFile(
		Name,				// LPCTSTR lpFileName, 
		GENERIC_READ | GENERIC_WRITE,       // access mode
		FILE_SHARE_WRITE | FILE_SHARE_READ, // share mode
		NULL,                               // security desc.
		OPEN_EXISTING,                      // how to create
		FILE_FLAG_OVERLAPPED,               // file attributes
		NULL                                // template file
		);
	if ( h == INVALID_HANDLE_VALUE ) {
		Status = GetLastError();
		// map ERROR_FILE_NOT_FOUND to a specific USBIO error code
		if (Status == ERROR_FILE_NOT_FOUND) {
			Status = USBIO_ERR_DEVICE_NOT_FOUND;
		}
	} else {
		// save handle
		FileHandle = h;
		// init the event with auto reset, not signaled
		Overlapped.hEvent = CreateEvent(NULL ,FALSE ,FALSE ,NULL); 
		if ( Overlapped.hEvent == NULL ) {
			Status = USBIO_ERR_NO_MEMORY;
			Close();
		} else {

			// now get version info
			USBIO_DRIVER_INFO info;
			Status = GetDriverInfo(&info);
			if ( Status != USBIO_ERR_SUCCESS ) {
				// failed
				Close();
			} else {

				CheckedBuildDetected = (info.Flags&USBIO_INFOFLAG_CHECKED_BUILD) ? TRUE : FALSE;
				DemoVersionDetected = (info.Flags&USBIO_INFOFLAG_DEMO_VERSION) ? TRUE : FALSE;
				LightVersionDetected = (info.Flags&USBIO_INFOFLAG_LIGHT_VERSION) ? TRUE : FALSE;

				// success
				Status = USBIO_ERR_SUCCESS;
				// now check the API version
				if (CheckApiVersion) {
					// major version must match the driver version.
					if ( (info.APIVersion & 0xff00) != (USBIO_API_VERSION & 0xff00) ||
						(info.APIVersion & 0xff) < (USBIO_API_VERSION & 0xff)) {
							// wrong version
							Status = USBIO_ERR_VERSION_MISMATCH;
							Close();
					}
				} 
			}
		}
	}
	return Status;
}


void CUsbIo::Close()
{
	ZeroMemory(&mDevInfoData,sizeof(mDevInfoData));
	
  if ( FileHandle != NULL ) {
    ::CloseHandle(FileHandle);
    FileHandle = NULL;
  }
  if ( Overlapped.hEvent != NULL ) {
    ::CloseHandle(Overlapped.hEvent);
    Overlapped.hEvent = NULL;
  }
  if ( mDevDetail != NULL ) {
    delete [] (char*)mDevDetail;
    mDevDetail = NULL;
  }
  if ( mParentIdStr != NULL ) {
    delete [] mParentIdStr;
    mParentIdStr = NULL;
  }
}



DWORD
CUsbIo::GetDeviceInstanceDetails(int DeviceNumber, HDEVINFO DeviceList, const GUID* InterfaceGuid)
{
  DWORD Status;
  BOOL succ;

  // check parameters
  if ( DeviceList==NULL || InterfaceGuid==NULL ) {
    return USBIO_ERR_INVALID_FUNCTION_PARAM;
  }

  // make sure the setupapi dll is loaded
  if ( !smSetupApi.Load() ) {
    return USBIO_ERR_LOAD_SETUP_API_FAILED;
  }

  // delete old detail data if any
  if ( mDevDetail!=NULL ) {
    delete [] (char*)mDevDetail;
    mDevDetail = NULL;
  }

  // enumerate the interface
  // get the device information for the given device number
  SP_DEVICE_INTERFACE_DATA DevData;
  ZeroMemory(&DevData,sizeof(DevData));
  DevData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
  succ = (smSetupApi.SetupDiEnumDeviceInterfaces)(DeviceList, NULL, (GUID*)InterfaceGuid, DeviceNumber, &DevData );
  if ( !succ ) {
    Status = GetLastError();
    if ( Status==ERROR_NO_MORE_ITEMS ) {
      Status = USBIO_ERR_NO_SUCH_DEVICE_INSTANCE;
    }
    return Status;
  }

  // get length of the detailed information, allocate buffer
  DWORD ReqLen = 0;
  (smSetupApi.SetupDiGetDeviceInterfaceDetail)(DeviceList, &DevData, NULL, 0, &ReqLen, NULL);
  if ( ReqLen==0 ) {
    return USBIO_ERR_FAILED;
  }
  mDevDetail = (SP_DEVICE_INTERFACE_DETAIL_DATA*) new char[ReqLen];
  if ( mDevDetail==NULL ) {
    return USBIO_ERR_NO_MEMORY;
  }

  // now get the  detailed device information
  ZeroMemory(mDevDetail,ReqLen);
  mDevDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
  ZeroMemory(&mDevInfoData,sizeof(mDevInfoData));
	mDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
  succ = (smSetupApi.SetupDiGetDeviceInterfaceDetail)(DeviceList, &DevData, mDevDetail, ReqLen, &ReqLen, &mDevInfoData);
  if ( !succ ) {
    Status = GetLastError();
    return Status;
  }

  // success, mDevDetail contains the device instance details now
  return USBIO_ERR_SUCCESS;
}


const TCHAR*
CUsbIo::GetDevicePathName()
{
  if ( mDevDetail!=NULL ) {
    return mDevDetail->DevicePath;
  } else {
    return NULL;
  }
}


const TCHAR*
CUsbIo::GetParentID()
{
	if (mDevInfoData.DevInst == NULL) {
		// not open or something failed
		return NULL;
	}

	// make sure the setupapi dll is loaded
  if ( !smSetupApi.Load() ) {
    return NULL;
  }
  
	// alloc string buffer, if it does not yet exist  
	unsigned int maxChars = MAX_DEVICE_ID_LEN;
  if ( mParentIdStr == NULL ) {
		mParentIdStr = new TCHAR[maxChars];
	  if ( mParentIdStr == NULL ) {
	    return NULL;
	  }
  }
  ZeroMemory(mParentIdStr,maxChars*sizeof(TCHAR));

	// query for parent
	DEVINST ParentHandle;
	CONFIGRET ret = (smSetupApi.CM_Get_Parent)(&ParentHandle,mDevInfoData.DevInst,0);
	if (ret != CR_SUCCESS) {
		return NULL;
	}
	// pass maxChars-1 to guarantee null termination
	ret = (smSetupApi.CM_Get_Device_ID)(ParentHandle,mParentIdStr,(maxChars-1)*sizeof(TCHAR),0);
	if (ret != CR_SUCCESS) {
		return NULL;
	}
	return mParentIdStr;

} // GetParentID



DWORD CUsbIo::GetDriverInfo(USBIO_DRIVER_INFO *DriverInfo)
{
  DWORD Status;

  Status = IoctlSync(
              IOCTL_USBIO_GET_DRIVER_INFO,
              NULL,
              0,
              DriverInfo,
              sizeof(USBIO_DRIVER_INFO),
              NULL
              );

  return Status;
}

DWORD CUsbIo::AcquireDevice()
{
 DWORD Status;

  Status = IoctlSync(
              IOCTL_USBIO_ACQUIRE_DEVICE,
              NULL,
              0,
              NULL,
              0,
              NULL
              );

  return Status;
}


DWORD CUsbIo::ReleaseDevice()
{
 DWORD Status;

  Status = IoctlSync(
              IOCTL_USBIO_RELEASE_DEVICE,
              NULL,
              0,
              NULL,
              0,
              NULL
              );

  return Status;
}


BOOL
CUsbIo::IsOperatingAtHighSpeed()
{
  USBIO_DEVICE_INFO info;
  ZeroMemory(&info,sizeof(info));

  DWORD Status = GetDeviceInfo(&info);
  if ( Status==USBIO_ERR_SUCCESS ) {
    return (info.Flags&USBIO_DEVICE_INFOFLAG_HIGH_SPEED) ? TRUE : FALSE;
  } else {
    // query failed
    return FALSE;
  }
}



DWORD CUsbIo::GetDeviceInfo(USBIO_DEVICE_INFO* DeviceInfo)
{
  return IoctlSync(
              IOCTL_USBIO_GET_DEVICE_INFO,
              NULL,
              0,
              DeviceInfo,
              sizeof(USBIO_DEVICE_INFO),
              NULL
              );
}


DWORD CUsbIo::GetBandwidthInfo(USBIO_BANDWIDTH_INFO* BandwidthInfo)
{
  return IoctlSync(
              IOCTL_USBIO_GET_BANDWIDTH_INFO,
              NULL,
              0,
              BandwidthInfo,
              sizeof(USBIO_BANDWIDTH_INFO),
              NULL
              );
}


DWORD CUsbIo::GetDescriptor(
        void* Buffer,
        DWORD& ByteCount,
        USBIO_REQUEST_RECIPIENT Recipient,
        UCHAR DescriptorType,
        UCHAR DescriptorIndex/*=0*/,
        USHORT LanguageId/*=0*/ 
        )
{
  DWORD Status;
  USBIO_DESCRIPTOR_REQUEST req;

  // zero the struct if any fields are added...
  ZeroMemory(&req,sizeof(req));
  req.Recipient = Recipient;
  req.DescriptorType = DescriptorType;
  req.DescriptorIndex = DescriptorIndex;
  req.LanguageId = LanguageId;
  
  Status = IoctlSync(
              IOCTL_USBIO_GET_DESCRIPTOR,
              &req,
              sizeof(req),
              Buffer,
              ByteCount,
              &ByteCount
              );

  return Status;
}



DWORD CUsbIo::SetDescriptor(
        const void* Buffer,
        DWORD& ByteCount,
        USBIO_REQUEST_RECIPIENT Recipient,
        UCHAR DescriptorType,
        UCHAR DescriptorIndex/*=0*/,
        USHORT LanguageId/*=0*/ 
        )
{
  DWORD Status;
  USBIO_DESCRIPTOR_REQUEST req;

  // zero the struct if any fields are added...
  ZeroMemory(&req,sizeof(req));
  req.Recipient = Recipient;
  req.DescriptorType = DescriptorType;
  req.DescriptorIndex = DescriptorIndex;
  req.LanguageId = LanguageId;
  
  Status = IoctlSync(
              IOCTL_USBIO_SET_DESCRIPTOR,
              &req,
              sizeof(req),
              (void*)Buffer,
              ByteCount,
              &ByteCount
              );

  return Status;
}



DWORD CUsbIo::SetFeature(
        USBIO_REQUEST_RECIPIENT Recipient,
        USHORT FeatureSelector,
        USHORT Index/*=0*/ 
        )
{
  DWORD Status;
  USBIO_FEATURE_REQUEST req;

  // zero the struct if any fields are added...
  ZeroMemory(&req,sizeof(req));
  req.Recipient = Recipient;
  req.FeatureSelector = FeatureSelector;
  req.Index = Index;
  
  Status = IoctlSync(
              IOCTL_USBIO_SET_FEATURE,
              &req,
              sizeof(req),
              NULL,
              0,
              NULL
              );

  return Status;
}



DWORD CUsbIo::ClearFeature(
        USBIO_REQUEST_RECIPIENT Recipient,
        USHORT FeatureSelector,
        USHORT Index/*=0*/ 
        )
{
  DWORD Status;
  USBIO_FEATURE_REQUEST req;

  // zero the struct if any fields are added...
  ZeroMemory(&req,sizeof(req));
  req.Recipient = Recipient;
  req.FeatureSelector = FeatureSelector;
  req.Index = Index;
  
  Status = IoctlSync(
              IOCTL_USBIO_CLEAR_FEATURE,
              &req,
              sizeof(req),
              NULL,
              0,
              NULL
              );

  return Status;
}



DWORD CUsbIo::GetStatus(
        USHORT& StatusValue,
        USBIO_REQUEST_RECIPIENT Recipient,
        USHORT Index/*=0*/ )
{
  DWORD Status;
  USBIO_STATUS_REQUEST req;
  USBIO_STATUS_REQUEST_DATA data;

  // zero the structs if any fields are added...
  ZeroMemory(&req,sizeof(req));
  ZeroMemory(&data,sizeof(data));
  req.Recipient = Recipient;
  req.Index = Index;
  
  Status = IoctlSync(
              IOCTL_USBIO_GET_STATUS,
              &req,
              sizeof(req),
              &data,
              sizeof(data),
              NULL
              );

  StatusValue = data.Status;

  return Status;
}



DWORD CUsbIo::ClassOrVendorInRequest(
        void* Buffer,
        DWORD& ByteCount,
        const USBIO_CLASS_OR_VENDOR_REQUEST* Request 
        )
{
  DWORD Status;

  Status = IoctlSync(
              IOCTL_USBIO_CLASS_OR_VENDOR_IN_REQUEST,
              Request,
              sizeof(USBIO_CLASS_OR_VENDOR_REQUEST),
              Buffer,
              ByteCount,
              &ByteCount
              );

  return Status;
}



DWORD CUsbIo::ClassOrVendorOutRequest(
        const void* Buffer,
        DWORD& ByteCount,
        const USBIO_CLASS_OR_VENDOR_REQUEST* Request
        )
{
  DWORD Status;

  Status = IoctlSync(
              IOCTL_USBIO_CLASS_OR_VENDOR_OUT_REQUEST,
              Request,
              sizeof(USBIO_CLASS_OR_VENDOR_REQUEST),
              (void*)Buffer,
              ByteCount,
              &ByteCount
              );

  return Status;
}



DWORD CUsbIo::SetConfiguration(const USBIO_SET_CONFIGURATION* Conf)
{
  DWORD Status;

  Status = IoctlSync(
              IOCTL_USBIO_SET_CONFIGURATION,
              Conf,
              sizeof(USBIO_SET_CONFIGURATION),
              NULL,
              0,
              NULL
              );

  return Status;
}


DWORD CUsbIo::UnconfigureDevice()
{
  DWORD Status;

  Status = IoctlSync(
              IOCTL_USBIO_UNCONFIGURE_DEVICE,
              NULL,
              0,
              NULL,
              0,
              NULL
              );

  return Status;
}


DWORD CUsbIo::GetConfiguration(UCHAR& ConfigurationValue)
{
  DWORD Status;
  USBIO_GET_CONFIGURATION_DATA data;

  // zero the struct
  ZeroMemory(&data,sizeof(data));
  
  Status = IoctlSync(
              IOCTL_USBIO_GET_CONFIGURATION,
              NULL,
              0,
              &data,
              sizeof(data),
              NULL
              );

  ConfigurationValue = data.ConfigurationValue;

  return Status;
}


DWORD CUsbIo::GetConfigurationInfo(USBIO_CONFIGURATION_INFO *Info)
{
  DWORD Status;

  // zero the struct if any fields are added...
  ZeroMemory(Info,sizeof(USBIO_CONFIGURATION_INFO));

  Status = IoctlSync(
              IOCTL_USBIO_GET_CONFIGURATION_INFO,
              NULL,
              0,
              Info,
              sizeof(USBIO_CONFIGURATION_INFO),
              NULL
              );

  return Status;
}



DWORD CUsbIo::SetInterface(const USBIO_INTERFACE_SETTING* Setting)
{
  DWORD Status;

  Status = IoctlSync(
              IOCTL_USBIO_SET_INTERFACE,
              Setting,
              sizeof(USBIO_INTERFACE_SETTING),
              NULL,
              0,
              NULL
              );

  return Status;
}


DWORD CUsbIo::GetInterface(
        UCHAR& AlternateSetting,
        USHORT Interface/*=0*/
        )
{
  DWORD Status;
  USBIO_GET_INTERFACE req;
  USBIO_GET_INTERFACE_DATA data;

  // zero the structs if any fields are added...
  ZeroMemory(&req,sizeof(req));
  ZeroMemory(&data,sizeof(data));
  req.Interface = Interface;
  
  Status = IoctlSync(
              IOCTL_USBIO_GET_INTERFACE,
              &req,
              sizeof(req),
              &data,
              sizeof(data),
              NULL
              );

  AlternateSetting = data.AlternateSetting;

  return Status;
}


DWORD CUsbIo::StoreConfigurationDescriptor(const USB_CONFIGURATION_DESCRIPTOR *Desc)
{
  DWORD Status;

  Status = IoctlSync(
              IOCTL_USBIO_STORE_CONFIG_DESCRIPTOR,
              Desc,
              Desc->wTotalLength,
              NULL,
              0,
              NULL
              );

  return Status;
}



DWORD CUsbIo::GetDeviceParameters(USBIO_DEVICE_PARAMETERS *DevParam)
{
  DWORD Status;

  // zero the struct if any fields are added...
  ZeroMemory(DevParam,sizeof(USBIO_DEVICE_PARAMETERS));

  Status = IoctlSync(
              IOCTL_USBIO_GET_DEVICE_PARAMETERS,
              NULL,
              0,
              DevParam,
              sizeof(USBIO_DEVICE_PARAMETERS),
              NULL
              );

  return Status;
}



DWORD CUsbIo::SetDeviceParameters(const USBIO_DEVICE_PARAMETERS *DevParam)
{
  DWORD Status;

  Status = IoctlSync(
              IOCTL_USBIO_SET_DEVICE_PARAMETERS,
              DevParam,
              sizeof(USBIO_DEVICE_PARAMETERS),
              NULL,
              0,
              NULL
              );

  return Status;
}



DWORD CUsbIo::ResetDevice()
{
  DWORD Status;

  Status = IoctlSync(
              IOCTL_USBIO_RESET_DEVICE,
              NULL,
              0,
              NULL,
              0,
              NULL
              );

  return Status;
}


DWORD CUsbIo::CyclePort()
{
  DWORD Status;

  Status = IoctlSync(
              IOCTL_USBIO_CYCLE_PORT,
              NULL,
              0,
              NULL,
              0,
              NULL
              );

  return Status;
}



DWORD CUsbIo::GetCurrentFrameNumber(DWORD &FrameNumber)
{
  DWORD Status;
  USBIO_FRAME_NUMBER data;

  // zero the struct if any fields are added...
  ZeroMemory(&data,sizeof(data));

  Status = IoctlSync(
              IOCTL_USBIO_GET_CURRENT_FRAME_NUMBER,
              NULL,
              0,
              &data,
              sizeof(data),
              NULL
              );

  FrameNumber = data.FrameNumber;

  return Status;
}


DWORD CUsbIo::GetDevicePowerState(USBIO_DEVICE_POWER_STATE& DevicePowerState)
{
  USBIO_DEVICE_POWER PowerRequest;
  DWORD err;

  // zero the struct if any fields are added...
  ZeroMemory(&PowerRequest,sizeof(PowerRequest));

  err = IoctlSync(
              IOCTL_USBIO_GET_DEVICE_POWER_STATE,
              NULL,
              0,
              &PowerRequest,
              sizeof(USBIO_DEVICE_POWER),
              NULL
              );
  if ( err == USBIO_ERR_SUCCESS ) {
    DevicePowerState = PowerRequest.DevicePowerState;
  }
  return err;
}


DWORD CUsbIo::SetDevicePowerState(USBIO_DEVICE_POWER_STATE DevicePowerState)
{
  USBIO_DEVICE_POWER PowerRequest;

  PowerRequest.DevicePowerState = DevicePowerState;

  return IoctlSync(
              IOCTL_USBIO_SET_DEVICE_POWER_STATE,
              &PowerRequest,
              sizeof(USBIO_DEVICE_POWER),
              NULL,
              0,
              NULL
              );
}




DWORD CUsbIo::GetDeviceDescriptor(USB_DEVICE_DESCRIPTOR* Desc)
{
  DWORD ByteCount = sizeof(USB_DEVICE_DESCRIPTOR);

  return GetDescriptor(
              Desc,
              ByteCount,
              RecipientDevice,
              USB_DEVICE_DESCRIPTOR_TYPE,
              0,
              0
              );
}



DWORD CUsbIo::GetConfigurationDescriptor(
        USB_CONFIGURATION_DESCRIPTOR* Desc,
        DWORD& ByteCount,
        UCHAR Index/*=0*/
        )
{

  return GetDescriptor(
              Desc,
              ByteCount,
              RecipientDevice,
              USB_CONFIGURATION_DESCRIPTOR_TYPE,
              Index,
              0
              );
}



DWORD CUsbIo::GetStringDescriptor(
        USB_STRING_DESCRIPTOR* Desc,
        DWORD& ByteCount,
        UCHAR Index/*=0*/,
        USHORT LanguageId/*=0*/
        )
{

  return GetDescriptor(
              Desc,
              ByteCount,
              RecipientDevice,
              USB_STRING_DESCRIPTOR_TYPE,
              Index,
              LanguageId
              );
}




DWORD CUsbIo::IoctlSync(
        DWORD IoctlCode,
        const void *InBuffer,
        DWORD InBufferSize,
        void *OutBuffer,
        DWORD OutBufferSize,
        DWORD *BytesReturned
        )
{
  DWORD Status;
  DWORD BytesRet = 0;
  BOOL succ;

  // check if the driver was opened
  if ( FileHandle == NULL ) {
    return USBIO_ERR_DEVICE_NOT_OPEN;
  }

  // IOCTL requests must be serialized
  // bec. there is only one event object per instance
  EnterCriticalSection(&CritSect);

  // call the device driver
  succ = DeviceIoControl(
            FileHandle,         // driver handle
            IoctlCode,          // IOCTL code
            (void*)InBuffer,    // input buffer
            InBufferSize,       // input buffer size
            OutBuffer,          // output buffer
            OutBufferSize,      // output buffer size
            &BytesRet,          // number of bytes returned
            &Overlapped         // overlapped structure (async.)
            );
  if ( succ ) {
    // ioctl completed successfully
    Status = USBIO_ERR_SUCCESS;
  } else {
    Status = GetLastError();
    if ( Status == ERROR_IO_PENDING ) {
      // the operation is pending, wait for completion
      succ = GetOverlappedResult(
                FileHandle,
                &Overlapped,
                &BytesRet,  // byte count
                TRUE        // wait flag
                );
      if ( succ ) {
        // completed successfully
        Status = USBIO_ERR_SUCCESS;
      } else {
        Status = GetLastError();
      }
    }
  }

  LeaveCriticalSection(&CritSect);

  if ( BytesReturned != NULL ) {
    *BytesReturned = BytesRet;
  }

  return Status;
}



BOOL CUsbIo::CancelIo()
{
  // cancel all outstanding requests that were
  // issued by the calling thread on this handle
  return ::CancelIo(FileHandle);
}



// helper struct
struct _ErrorCodeTable {
  DWORD Code;
  const TCHAR *String;
};

//static 
TCHAR* 
USBIOLIB_CALL
CUsbIo::ErrorText(TCHAR* StringBuffer, DWORD MaxCharCount, DWORD ErrorCode)
{
  // string table
  static const struct _ErrorCodeTable ErrorTable[] = {
    {USBIO_ERR_SUCCESS                , _T("No error.")},
    {USBIO_ERR_CRC                    , _T("HC Error: Wrong CRC.")},
    {USBIO_ERR_BTSTUFF                , _T("HC Error: Wrong bit stuffing.")},
    {USBIO_ERR_DATA_TOGGLE_MISMATCH   , _T("HC Error: Data toggle mismatch.")},
    {USBIO_ERR_STALL_PID              , _T("HC Error: stall PID.")},
    {USBIO_ERR_DEV_NOT_RESPONDING     , _T("HC Error: Device not responding.")},
    {USBIO_ERR_PID_CHECK_FAILURE      , _T("HC Error: PID check failed.")},
    {USBIO_ERR_UNEXPECTED_PID         , _T("HC Error: Unexpected PID.")},
    {USBIO_ERR_DATA_OVERRUN           , _T("HC Error: Data Overrun.")},
    {USBIO_ERR_DATA_UNDERRUN          , _T("HC Error: Data Underrun.")},
    {USBIO_ERR_RESERVED1              , _T("HC Error: Reserved1.")},
    {USBIO_ERR_RESERVED2              , _T("HC Error: Reserved2.")},
    {USBIO_ERR_BUFFER_OVERRUN         , _T("HC Error: Buffer Overrun.")},
    {USBIO_ERR_BUFFER_UNDERRUN        , _T("HC Error: Buffer Underrun.")},
    {USBIO_ERR_NOT_ACCESSED           , _T("HC Error: Not accessed.")},
    {USBIO_ERR_FIFO                   , _T("HC Error: FIFO error.")},
    {USBIO_ERR_XACT_ERROR             , _T("HC Error: XACT error.")},
    {USBIO_ERR_BABBLE_DETECTED        , _T("HC Error: Babble detected.")},
    {USBIO_ERR_DATA_BUFFER_ERROR      , _T("HC Error: Data buffer error.")},

    {USBIO_ERR_ENDPOINT_HALTED        , _T("USBD Error: Endpoint halted.")},
    {USBIO_ERR_NO_MEMORY              , _T("USBD Error: No system memory.")},
    {USBIO_ERR_INVALID_URB_FUNCTION   , _T("USBD Error: Invalid URB function.")},
    {USBIO_ERR_INVALID_PARAMETER      , _T("USBD Error: Invalid parameter.")},
    {USBIO_ERR_ERROR_BUSY             , _T("USBD Error: Busy.")},
    {USBIO_ERR_REQUEST_FAILED         , _T("USBD Error: Request failed.")},
    {USBIO_ERR_INVALID_PIPE_HANDLE    , _T("USBD Error: Invalid pipe handle.")},
    {USBIO_ERR_NO_BANDWIDTH           , _T("USBD Error: No bandwidth available.")},
    {USBIO_ERR_INTERNAL_HC_ERROR      , _T("USBD Error: Internal HC error.")},
    {USBIO_ERR_ERROR_SHORT_TRANSFER   , _T("USBD Error: Short transfer.")},
    {USBIO_ERR_BAD_START_FRAME        , _T("USBD Error: Bad start frame.")},
    {USBIO_ERR_ISOCH_REQUEST_FAILED   , _T("USBD Error: Isochronous request failed.")},
    {USBIO_ERR_FRAME_CONTROL_OWNED    , _T("USBD Error: Frame control owned.")},
    {USBIO_ERR_FRAME_CONTROL_NOT_OWNED, _T("USBD Error: Frame control not owned.")},
    {USBIO_ERR_NOT_SUPPORTED          , _T("USBD Error: Not supported.")},
    {USBIO_ERR_INVALID_CONFIGURATION_DESCRIPTOR, _T("USBD Error: Invalid configuration descriptor.")},

    {USBIO_ERR_INSUFFICIENT_RESOURCES   , _T("USBD Error: Insufficient resources.")},
    {USBIO_ERR_SET_CONFIG_FAILED        , _T("USBD Error: Set configuration failed.")},
    {USBIO_ERR_USBD_BUFFER_TOO_SMALL    , _T("USBD Error: Buffer too small.")},
    {USBIO_ERR_USBD_INTERFACE_NOT_FOUND , _T("USBD Error: Interface not found.")},
    {USBIO_ERR_INVALID_PIPE_FLAGS       , _T("USBD Error: Invalid pipe flags.")},
    {USBIO_ERR_USBD_TIMEOUT             , _T("USBD Error: Timeout.")},
    {USBIO_ERR_DEVICE_GONE              , _T("USBD Error: Device gone.")},
    {USBIO_ERR_STATUS_NOT_MAPPED        , _T("USBD Error: Status not mapped.")},


    {USBIO_ERR_CANCELED               , _T("USBD Error: cancelled.")},
    {USBIO_ERR_ISO_NOT_ACCESSED_BY_HW , _T("USBD Error: ISO not accessed by hardware.")},
    {USBIO_ERR_ISO_TD_ERROR           , _T("USBD Error: ISO TD error.")},
    {USBIO_ERR_ISO_NA_LATE_USBPORT    , _T("USBD Error: ISO NA late USB port.")},
    {USBIO_ERR_ISO_NOT_ACCESSED_LATE  , _T("USBD Error: ISO not accessed, submitted too late.")},

    {USBIO_ERR_FAILED                 , _T("Operation failed.")},
    {USBIO_ERR_INVALID_INBUFFER       , _T("Input buffer too small.")},
    {USBIO_ERR_INVALID_OUTBUFFER      , _T("Output buffer too small.")},
    {USBIO_ERR_OUT_OF_MEMORY          , _T("Out of memory.")},
    {USBIO_ERR_PENDING_REQUESTS       , _T("There are pending requests.")},
    {USBIO_ERR_ALREADY_CONFIGURED     , _T("USB device is already configured.")},
    {USBIO_ERR_NOT_CONFIGURED         , _T("USB device is not configured.")},
    {USBIO_ERR_OPEN_PIPES             , _T("There are open pipes.")},
    {USBIO_ERR_ALREADY_BOUND          , _T("Either handle or pipe is already bound.")},
    {USBIO_ERR_NOT_BOUND              , _T("Handle is not bound to a pipe.")},
    {USBIO_ERR_DEVICE_NOT_PRESENT     , _T("Device is removed.")},
    {USBIO_ERR_CONTROL_NOT_SUPPORTED  , _T("Control code is not supported.")},
    {USBIO_ERR_TIMEOUT                , _T("The request has been timed out.")},
    {USBIO_ERR_INVALID_RECIPIENT      , _T("Invalid recipient.")},
    {USBIO_ERR_INVALID_TYPE           , _T("Invalid pipe type or invalid request type.")},
    {USBIO_ERR_INVALID_IOCTL          , _T("Invalid I/O control code.")},
    {USBIO_ERR_INVALID_DIRECTION      , _T("Invalid direction of read/write operation.")},
    {USBIO_ERR_TOO_MUCH_ISO_PACKETS   , _T("Too much ISO packets.")},
    {USBIO_ERR_POOL_EMPTY             , _T("Request pool empty.")},
    {USBIO_ERR_PIPE_NOT_FOUND         , _T("Pipe not found.")},
    {USBIO_ERR_INVALID_ISO_PACKET     , _T("Invalid ISO packet.")},
    {USBIO_ERR_OUT_OF_ADDRESS_SPACE   , _T("Out of address space. Not enough system resources.")},
    {USBIO_ERR_INTERFACE_NOT_FOUND    , _T("Interface not found.")},
    {USBIO_ERR_INVALID_DEVICE_STATE   , _T("Invalid device state (stopped or power down).")},
    {USBIO_ERR_INVALID_PARAM          , _T("Invalid parameter.")},
    {USBIO_ERR_DEMO_EXPIRED           , _T("DEMO version has timed out. Reboot required!")},
    {USBIO_ERR_INVALID_POWER_STATE    , _T("Power state not allowed. Set to D0 first.")},
    {USBIO_ERR_POWER_DOWN             , _T("Device powered down.")},
    {USBIO_ERR_VERSION_MISMATCH       , _T("API Version does not match.")},
    {USBIO_ERR_SET_CONFIGURATION_FAILED,_T("Set configuration failed.")},
    {USBIO_ERR_INVALID_PROCESS,         _T("Invalid process.")},
    {USBIO_ERR_DEVICE_ACQUIRED,         _T("The device is acquired by another process for exclusive use.")},
    {USBIO_ERR_DEVICE_OPENED,           _T("A different process has opened the device.")},

    {USBIO_ERR_VID_RESTRICTION,         _T("Light version restriction: Unsupported Vendor ID.")},
    {USBIO_ERR_ISO_RESTRICTION,         _T("Light version restriction: Iso pipes are not supported.")},
    {USBIO_ERR_BULK_RESTRICTION,        _T("Light version restriction: Bulk pipes are not supported.")},
    {USBIO_ERR_EP0_RESTRICTION,         _T("Light version restriction: EP0 requests are not fully supported.")},
    {USBIO_ERR_PIPE_RESTRICTION,        _T("Light version restriction: Too many pipes.")},
    {USBIO_ERR_PIPE_SIZE_RESTRICTION,   _T("Light version restriction: Maximum FIFO size exceeded.")},
    {USBIO_ERR_CONTROL_RESTRICTION,     _T("Light version restriction: Control pipes are not supported.")},
    {USBIO_ERR_INTERRUPT_RESTRICTION,   _T("Light version restriction: Interrupt pipes are not supported.")},

    {USBIO_ERR_DEVICE_NOT_FOUND       , _T("Device not found or acquired for exclusive use.")},
    {USBIO_ERR_DEVICE_NOT_OPEN        , _T("Device not open.")},
    {USBIO_ERR_NO_SUCH_DEVICE_INSTANCE, _T("No such device instance.")},
    {USBIO_ERR_INVALID_FUNCTION_PARAM,  _T("An invalid parameter was passed.")},
    {USBIO_ERR_DEVICE_ALREADY_OPENED,   _T("The handle is already opened.")},
    {USBIO_ERR_INVALID_DESCRIPTOR,      _T("The device returned an invalid descriptor.")},
		{USBIO_ERR_NOT_SUPPORTED_UNDER_CE,  _T("This operation or feature is not supported under Windows CE.")}
  };

  static int Size = sizeof(ErrorTable)/sizeof(struct _ErrorCodeTable);
  const TCHAR *ErrorString = _T("Windows system error code.");
  int i;
  BOOL found=FALSE;

  if ( (StringBuffer==NULL) || (MaxCharCount==0) ) {
    return StringBuffer;
  }

  for (i=0;i<Size;i++) {
    if (ErrorTable[i].Code == ErrorCode) {
      ErrorString=ErrorTable[i].String;
      found = TRUE;
      break;
    }
  }

  // the following does not produce useful error messages
  // so we don't use it anymore
  /*
  char* MsgBuffer = NULL;
  if (!found) {
    if (0 != FormatMessage(
                FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_SYSTEM | 
                FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                ErrorCode,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                (LPTSTR)&MsgBuffer,    
                0,    
                NULL 
                )) {
      // found
      ErrorString = MsgBuffer; 
    }
  }
  */
  // print to string buffer
  _sntprintf(StringBuffer,MaxCharCount,_T("Error code 0x%08X: %s"),ErrorCode,ErrorString); 
  // make sure the string is zero-terminated
  StringBuffer[MaxCharCount-1] = 0;

/*
  // free resources
  if ( MsgBuffer!=NULL ) {
    LocalFree(MsgBuffer);
  }
*/

  return StringBuffer;

}//ErrorText



/*************************** EOF **************************************/
