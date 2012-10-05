/************************************************************************
 *
 *  Module:       usbio_i.h
 *  Long name:    USBIO Driver Interface
 *  Description:  Defines the programming interface (API) of the USBIO driver
 *
 *  Runtime Env.: Win32
 *  Author(s):    Guenter Hildebrandt, Udo Eberhardt
 *  Company:      Thesycon GmbH, Ilmenau
 ************************************************************************/

#ifndef _USBIO_I_H_
#define _USBIO_I_H_

//
// Version of the software interface exported by the USBIO driver.
//
// This will be incremented if changes are made at the programming interface level.
// The following convention exists:
// If changes are made to the programming interface that are compatible with
// previous versions then the minor version number (low order byte) will be incremented.
// If changes are made that cause an incompatibility with previous versions of the 
// interface then the major version number (high order byte) will be incremented.
//
// Applications should check if the driver supports the 
// required API version using IOCTL_USBIO_GET_DRIVER_INFO.
//
// current API version: 2.31
#define USBIO_API_VERSION   0x0231

//
// built-in (default) GUID for the USBIO interface
//
// IMPORTANT:
// Don't use this GUID for your own (custom) application. Generate a new one
// that is private to you and put it in the USBIO INF file and use it in 
// your application. Refer to the USBIO manual for further instructions.
// The tool guidgen.exe from the Microsoft Platform SDK can be used to create private GUIDs.
//


//GUID for DVS128
#define USBIO_IID_STR     "{7794c79a-40a7-4a6c-8a29-da141c20d78c}"
#define USBIO_IID_STR_W  L"{7794c79a-40a7-4a6c-8a29-da141c20d78c}"

#define USBIO_IID {0x7794c79a, 0x40a7, 0x4a6c, {0x8a, 0x29, 0xda, 0x14, 0x1c, 0x20, 0xd7, 0x8c}} //DVS128 ID



// 
// include the status codes
//
#include "usbioerr.h"


//
// USB Standard Device Requests
//

#define IOCTL_USBIO_GET_DESCRIPTOR            _USBIO_IOCTL_CODE(1,METHOD_OUT_DIRECT)
// InBuffer:  USBIO_DESCRIPTOR_REQUEST
// OutBuffer: descriptor data, variable size!

#define IOCTL_USBIO_SET_DESCRIPTOR            _USBIO_IOCTL_CODE(2,METHOD_IN_DIRECT)
// InBuffer:  USBIO_DESCRIPTOR_REQUEST
// OutBuffer: descriptor data, variable size!



#define IOCTL_USBIO_SET_FEATURE               _USBIO_IOCTL_CODE(3,METHOD_BUFFERED)
// InBuffer:  USBIO_FEATURE_REQUEST
// OutBuffer: none

#define IOCTL_USBIO_CLEAR_FEATURE             _USBIO_IOCTL_CODE(4,METHOD_BUFFERED)
// InBuffer:  USBIO_FEATURE_REQUEST
// OutBuffer: none



#define IOCTL_USBIO_GET_STATUS                _USBIO_IOCTL_CODE(5,METHOD_BUFFERED)
// InBuffer:  USBIO_STATUS_REQUEST
// OutBuffer: USBIO_STATUS_REQUEST_DATA



#define IOCTL_USBIO_GET_CONFIGURATION         _USBIO_IOCTL_CODE(6,METHOD_BUFFERED)
// InBuffer:  none
// OutBuffer: USBIO_GET_CONFIGURATION_DATA


#define IOCTL_USBIO_GET_INTERFACE             _USBIO_IOCTL_CODE(7,METHOD_BUFFERED)
// InBuffer:  USBIO_GET_INTERFACE
// OutBuffer: USBIO_GET_INTERFACE_DATA


#define IOCTL_USBIO_STORE_CONFIG_DESCRIPTOR   _USBIO_IOCTL_CODE(8,METHOD_BUFFERED)
// InBuffer:  USB_CONFIGURATION_DESCRIPTOR variable size!
// OutBuffer: none


#define IOCTL_USBIO_SET_CONFIGURATION         _USBIO_IOCTL_CODE(9,METHOD_BUFFERED)
// InBuffer:  USBIO_SET_CONFIGURATION
// OutBuffer: none

#define IOCTL_USBIO_UNCONFIGURE_DEVICE        _USBIO_IOCTL_CODE(10,METHOD_BUFFERED)
// InBuffer:  none
// OutBuffer: none


#define IOCTL_USBIO_SET_INTERFACE             _USBIO_IOCTL_CODE(11,METHOD_BUFFERED)
// InBuffer:  USBIO_INTERFACE_SETTING
// OutBuffer: none



//
// USB Class or Vendor specific Device Requests
//

#define IOCTL_USBIO_CLASS_OR_VENDOR_IN_REQUEST  _USBIO_IOCTL_CODE(12,METHOD_OUT_DIRECT)
// InBuffer:  USBIO_CLASS_OR_VENDOR_REQUEST
// OutBuffer: data, variable size!

#define IOCTL_USBIO_CLASS_OR_VENDOR_OUT_REQUEST _USBIO_IOCTL_CODE(13,METHOD_IN_DIRECT)
// InBuffer:  USBIO_CLASS_OR_VENDOR_REQUEST
// OutBuffer: data, variable size!



//
// Device related Driver Parameters
//

#define IOCTL_USBIO_GET_DEVICE_PARAMETERS       _USBIO_IOCTL_CODE(15,METHOD_BUFFERED)
// InBuffer:  none
// OutBuffer: USBIO_DEVICE_PARAMETERS                                                 

#define IOCTL_USBIO_SET_DEVICE_PARAMETERS       _USBIO_IOCTL_CODE(16,METHOD_BUFFERED)
// InBuffer:  USBIO_DEVICE_PARAMETERS
// OutBuffer: none



//
// Other 
//

#define IOCTL_USBIO_GET_CONFIGURATION_INFO      _USBIO_IOCTL_CODE(20,METHOD_BUFFERED)
// InBuffer:  none
// OutBuffer: USBIO_CONFIGURATION_INFO

#define IOCTL_USBIO_RESET_DEVICE                _USBIO_IOCTL_CODE(21,METHOD_BUFFERED)
// InBuffer:  none
// OutBuffer: none

#define IOCTL_USBIO_GET_CURRENT_FRAME_NUMBER    _USBIO_IOCTL_CODE(22,METHOD_BUFFERED)
// InBuffer:  none
// OutBuffer: USBIO_FRAME_NUMBER


#define IOCTL_USBIO_SET_DEVICE_POWER_STATE      _USBIO_IOCTL_CODE(23,METHOD_BUFFERED)
// InBuffer:  USBIO_DEVICE_POWER
// OutBuffer: none

#define IOCTL_USBIO_GET_DEVICE_POWER_STATE      _USBIO_IOCTL_CODE(24,METHOD_BUFFERED)
// InBuffer:  none
// OutBuffer: USBIO_DEVICE_POWER


#define IOCTL_USBIO_GET_BANDWIDTH_INFO          _USBIO_IOCTL_CODE(25,METHOD_BUFFERED)
// InBuffer:  none
// OutBuffer: USBIO_BANDWIDTH_INFO


#define IOCTL_USBIO_GET_DEVICE_INFO             _USBIO_IOCTL_CODE(26,METHOD_BUFFERED)
// InBuffer:  none
// OutBuffer: USBIO_DEVICE_INFO


#define IOCTL_USBIO_GET_DRIVER_INFO             _USBIO_IOCTL_CODE(27,METHOD_BUFFERED)
// InBuffer:  none
// OutBuffer: USBIO_DRIVER_INFO


#define IOCTL_USBIO_CYCLE_PORT                  _USBIO_IOCTL_CODE(28,METHOD_BUFFERED)
// InBuffer:  none
// OutBuffer: none


#define IOCTL_USBIO_ACQUIRE_DEVICE              _USBIO_IOCTL_CODE(50,METHOD_BUFFERED)
// InBuffer:  none
// OutBuffer: none

#define IOCTL_USBIO_RELEASE_DEVICE              _USBIO_IOCTL_CODE(51,METHOD_BUFFERED)
// InBuffer:  none
// OutBuffer: none

//
// Pipe related requests
//

#define IOCTL_USBIO_BIND_PIPE                   _USBIO_IOCTL_CODE(30,METHOD_BUFFERED)
// InBuffer:  USBIO_BIND_PIPE
// OutBuffer: none

#define IOCTL_USBIO_UNBIND_PIPE                 _USBIO_IOCTL_CODE(31,METHOD_BUFFERED)
// InBuffer:  none
// OutBuffer: none



#define IOCTL_USBIO_RESET_PIPE                  _USBIO_IOCTL_CODE(32,METHOD_BUFFERED)
// InBuffer:  none
// OutBuffer: none

#define IOCTL_USBIO_ABORT_PIPE                  _USBIO_IOCTL_CODE(33,METHOD_BUFFERED)
// InBuffer:  none
// OutBuffer: none



#define IOCTL_USBIO_GET_PIPE_PARAMETERS         _USBIO_IOCTL_CODE(35,METHOD_BUFFERED)
// InBuffer:  none
// OutBuffer: USBIO_PIPE_PARAMETERS

#define IOCTL_USBIO_SET_PIPE_PARAMETERS         _USBIO_IOCTL_CODE(36,METHOD_BUFFERED)
// InBuffer:  USBIO_PIPE_PARAMETERS
// OutBuffer: none


#define IOCTL_USBIO_SETUP_PIPE_STATISTICS       _USBIO_IOCTL_CODE(37,METHOD_BUFFERED)
// InBuffer:  USBIO_SETUP_PIPE_STATISTICS
// OutBuffer: none


#define IOCTL_USBIO_QUERY_PIPE_STATISTICS       _USBIO_IOCTL_CODE(38,METHOD_BUFFERED)
// InBuffer:  USBIO_QUERY_PIPE_STATISTICS
// OutBuffer: USBIO_PIPE_STATISTICS



#define IOCTL_USBIO_PIPE_CONTROL_TRANSFER_IN    _USBIO_IOCTL_CODE(40,METHOD_OUT_DIRECT)
// InBuffer:  USBIO_PIPE_CONTROL_TRANSFER
// OutBuffer: data, variable size!

#define IOCTL_USBIO_PIPE_CONTROL_TRANSFER_OUT   _USBIO_IOCTL_CODE(41,METHOD_IN_DIRECT)
// InBuffer:  USBIO_PIPE_CONTROL_TRANSFER
// OutBuffer: data, variable size!



//
// options (device related)
//
#define USBIO_RESET_DEVICE_ON_CLOSE   0x00000001
#define USBIO_UNCONFIGURE_ON_CLOSE    0x00000002
#define USBIO_ENABLE_REMOTE_WAKEUP    0x00000004

//
// flags (pipe related)
//
#define USBIO_SHORT_TRANSFER_OK       0x00010000
#define USBIO_START_TRANSFER_ASAP     0x00020000

//
// max array size
//
#define USBIO_MAX_INTERFACES      32
#define USBIO_MAX_PIPES           32



// pack the following structures                                                     
#include <PSHPACK1.H>

// In Borland C++, we have to force byte alignment.
// This is necessary because there seems to be a problem with the
// implementations of pshpackX.h and poppack.h.
#ifdef __BORLANDC__
#pragma pack(1)
#endif


typedef enum _USBIO_PIPE_TYPE {
  PipeTypeControl = 0,
  PipeTypeIsochronous,
  PipeTypeBulk,
  PipeTypeInterrupt,
  // The following dummy value is included to force the compiler to make the 
  // size of the enum 4 bytes (32 bits). Some compilers, Borland C++ for 
  // instance, use the smallest possible integer for enums.
  PipeType_Force32bitEnum = 2000000000
} USBIO_PIPE_TYPE;


typedef enum _USBIO_REQUEST_RECIPIENT {
  RecipientDevice = 0,
  RecipientInterface,
  RecipientEndpoint,
  RecipientOther,
  // The following dummy value is included to force the compiler to make the 
  // size of the enum 4 bytes (32 bits). Some compilers, Borland C++ for 
  // instance, use the smallest possible integer for enums.
  Recipient_Force32bitEnum = 2000000000
} USBIO_REQUEST_RECIPIENT;


typedef enum _USBIO_REQUEST_TYPE {
  RequestTypeClass = 1,
  RequestTypeVendor,
  // The following dummy value is included to force the compiler to make the 
  // size of the enum 4 bytes (32 bits). Some compilers, Borland C++ for 
  // instance, use the smallest possible integer for enums.
  RequestType_Force32bitEnum = 2000000000
} USBIO_REQUEST_TYPE;


typedef enum _USBIO_DEVICE_POWER_STATE {
  DevicePowerStateD0 = 0,
  DevicePowerStateD1,
  DevicePowerStateD2,
  DevicePowerStateD3,
  // The following dummy value is included to force the compiler to make the 
  // size of the enum 4 bytes (32 bits). Some compilers, Borland C++ for 
  // instance, use the smallest possible integer for enums.
  DevicePowerState_Force32bitEnum = 2000000000
} USBIO_DEVICE_POWER_STATE;



//
// GET_BANDWIDTH_INFO
//
typedef struct _USBIO_BANDWIDTH_INFO {
  ULONG  TotalBandwidth;
  ULONG  ConsumedBandwidth;
  ULONG  reserved1;   // reserved for future use
  ULONG  reserved2;   // reserved for future use
} USBIO_BANDWIDTH_INFO;


//
// GET_DEVICE_INFO
//
typedef struct _USBIO_DEVICE_INFO {
  ULONG  Flags;       // 0 or any combination of USBIO_DEVICE_INFOFLAG_XXX
  ULONG  OpenCount;   // returns the number of open handles
  ULONG  reserved1;   // reserved for future use
  ULONG  reserved2;   // reserved for future use
} USBIO_DEVICE_INFO;

#define USBIO_DEVICE_INFOFLAG_HIGH_SPEED				0x00100000

//
// GET_DRIVER_INFO
//
typedef struct _USBIO_DRIVER_INFO {
  USHORT APIVersion;    // [MAJOR][MINOR] (BCD format)
  USHORT DriverVersion; // [MAJOR][MINOR]
  ULONG  DriverBuildNumber;
  ULONG  Flags;         // 0 or any combination of USBIO_INFOFLAG_XXX
} USBIO_DRIVER_INFO;

#define USBIO_INFOFLAG_CHECKED_BUILD      0x00000010
#define USBIO_INFOFLAG_DEMO_VERSION       0x00000020
#define USBIO_INFOFLAG_LIGHT_VERSION      0x00000100
#define USBIO_INFOFLAG_VS_LIGHT_VERSION   0x00000200



//
// GET_DESCRIPTOR
// SET_DESCRIPTOR
//
typedef struct _USBIO_DESCRIPTOR_REQUEST {
  USBIO_REQUEST_RECIPIENT Recipient;
  UCHAR   DescriptorType;
  UCHAR   DescriptorIndex;
  USHORT  LanguageId;
} USBIO_DESCRIPTOR_REQUEST;

//
// SET_FEATURE
// CLEAR_FEATURE
//
typedef struct _USBIO_FEATURE_REQUEST {
  USBIO_REQUEST_RECIPIENT Recipient;
  USHORT  FeatureSelector;
  USHORT  Index;
} USBIO_FEATURE_REQUEST;


//
// GET_STATUS
//
typedef struct _USBIO_STATUS_REQUEST {
  USBIO_REQUEST_RECIPIENT Recipient;
  USHORT  Index;
} USBIO_STATUS_REQUEST;

typedef struct _USBIO_STATUS_REQUEST_DATA {
  USHORT  Status;
} USBIO_STATUS_REQUEST_DATA;


//
// GET_CONFIGURATION
//
typedef struct _USBIO_GET_CONFIGURATION_DATA {
  UCHAR ConfigurationValue;
} USBIO_GET_CONFIGURATION_DATA;


//
// GET_INTERFACE
//
typedef struct _USBIO_GET_INTERFACE {
  USHORT Interface;
} USBIO_GET_INTERFACE;

typedef struct _USBIO_GET_INTERFACE_DATA {
  UCHAR AlternateSetting;
} USBIO_GET_INTERFACE_DATA;



//
// SET_CONFIGURATION
// SET_INTERFACE
//
typedef struct _USBIO_INTERFACE_SETTING {
  USHORT  InterfaceIndex;
  USHORT  AlternateSettingIndex;
  ULONG   MaximumTransferSize;
} USBIO_INTERFACE_SETTING;

typedef struct _USBIO_SET_CONFIGURATION {
  USHORT ConfigurationIndex;
  USHORT NbOfInterfaces;
  USBIO_INTERFACE_SETTING InterfaceList[USBIO_MAX_INTERFACES];
} USBIO_SET_CONFIGURATION;



//
// CLASS_OR_VENDOR_IN_REQUEST
// CLASS_OR_VENDOR_OUT_REQUEST
//
typedef struct _USBIO_CLASS_OR_VENDOR_REQUEST {
  ULONG   Flags;
  USBIO_REQUEST_TYPE Type;
  USBIO_REQUEST_RECIPIENT Recipient;
  UCHAR   RequestTypeReservedBits;
  UCHAR   Request;
  USHORT  Value;
  USHORT  Index;
} USBIO_CLASS_OR_VENDOR_REQUEST;



//
// GET_DEVICE_PARAMETERS
// SET_DEVICE_PARAMETERS
//
typedef struct _USBIO_DEVICE_PARAMETERS {
  ULONG Options;
  ULONG RequestTimeout;   // in ms
} USBIO_DEVICE_PARAMETERS;



//
// GET_CONFIGURATION_INFO
//
typedef struct _USBIO_INTERFACE_CONFIGURATION_INFO {
  UCHAR InterfaceNumber;
  UCHAR AlternateSetting;
  UCHAR Class;
  UCHAR SubClass;
  UCHAR Protocol;
  UCHAR NumberOfPipes;
  UCHAR reserved1;  // reserved field, set to zero
  UCHAR reserved2;  // reserved field, set to zero
} USBIO_INTERFACE_CONFIGURATION_INFO;

typedef struct _USBIO_PIPE_CONFIGURATION_INFO {
  USBIO_PIPE_TYPE PipeType;  // type
  ULONG  MaximumTransferSize;// maximum Read/Write buffer size 
  USHORT MaximumPacketSize;  // FIFO size of the endpoint  
  UCHAR  EndpointAddress;    // including direction bit (bit 7)
  UCHAR  Interval;           // in ms, for interrupt pipe
  UCHAR  InterfaceNumber;    // interface that the EP belongs to
  UCHAR  reserved1;   // reserved field, set to zero
  UCHAR  reserved2;   // reserved field, set to zero
  UCHAR  reserved3;   // reserved field, set to zero
} USBIO_PIPE_CONFIGURATION_INFO;

typedef struct _USBIO_CONFIGURATION_INFO {
  ULONG NbOfInterfaces;
  ULONG NbOfPipes;
  USBIO_INTERFACE_CONFIGURATION_INFO InterfaceInfo[USBIO_MAX_INTERFACES];
  USBIO_PIPE_CONFIGURATION_INFO PipeInfo[USBIO_MAX_PIPES];
} USBIO_CONFIGURATION_INFO;



//
// GET_CURRENT_FRAME_NUMBER
//
typedef struct _USBIO_FRAME_NUMBER {
  ULONG FrameNumber;
} USBIO_FRAME_NUMBER;


//
// SET_DEVICE_POWER_STATE
// GET_DEVICE_POWER_STATE
//
typedef struct _USBIO_DEVICE_POWER {
  USBIO_DEVICE_POWER_STATE DevicePowerState;
} USBIO_DEVICE_POWER;



//
// BIND_PIPE
//
typedef struct _USBIO_BIND_PIPE {
  UCHAR EndpointAddress; // including direction bit
} USBIO_BIND_PIPE;


//
// GET_PIPE_PARAMETERS
// SET_PIPE_PARAMETERS
//
typedef struct _USBIO_PIPE_PARAMETERS {
  ULONG Flags;
} USBIO_PIPE_PARAMETERS;


//
// SETUP_PIPE_STATISTICS
//
typedef struct _USBIO_SETUP_PIPE_STATISTICS {
  ULONG AveragingInterval;  // in ms, max: 5000, 0 = statistics off
  ULONG reserved1;          // reserved for future use, set to zero
  ULONG reserved2;          // reserved for future use, set to zero
} USBIO_SETUP_PIPE_STATISTICS;


//
// QUERY_PIPE_STATISTICS
//
typedef struct _USBIO_QUERY_PIPE_STATISTICS {
  ULONG Flags;      // 0 or any combination of USBIO_QPS_FLAG_XXX
} USBIO_QUERY_PIPE_STATISTICS;

#define USBIO_QPS_FLAG_RESET_BYTES_TRANSFERRED    0x00000001
#define USBIO_QPS_FLAG_RESET_REQUESTS_SUCCEEDED   0x00000002
#define USBIO_QPS_FLAG_RESET_REQUESTS_FAILED      0x00000004
#define USBIO_QPS_FLAG_RESET_ALL_COUNTERS ( USBIO_QPS_FLAG_RESET_BYTES_TRANSFERRED | \
                                            USBIO_QPS_FLAG_RESET_REQUESTS_SUCCEEDED | \
                                            USBIO_QPS_FLAG_RESET_REQUESTS_FAILED \
                                          )

typedef struct _USBIO_PIPE_STATISTICS {
  ULONG ActualAveragingInterval;  // in ms, 0 = statistics off
  ULONG AverageRate;              // in bytes/s
  ULONG BytesTransferred_L; // total number of bytes transferred, lower 32 bits
  ULONG BytesTransferred_H; // total number of bytes transferred, upper 32 bits
  ULONG RequestsSucceeded;  // total number of I/O requests succeeded
  ULONG RequestsFailed;     // total number of I/O requests failed
  ULONG reserved1;          // reserved for future use
  ULONG reserved2;          // reserved for future use
} USBIO_PIPE_STATISTICS;


//
// PIPE_CONTROL_TRANSFER_IN
// PIPE_CONTROL_TRANSFER_OUT
//
typedef struct _USBIO_PIPE_CONTROL_TRANSFER {
  ULONG Flags;
  UCHAR SetupPacket[8];
} USBIO_PIPE_CONTROL_TRANSFER;



//
// Isochronous Transfers
//
// The data buffer passed to ReadFile/WriteFile has to contain a
// predefined header that describes the size and location of the 
// packets to be transferred. The USBIO_ISO_TRANSFER_HEADER consists
// of a fixed size part, that is USBIO_ISO_TRANSFER, and a variable size array
// of USBIO_ISO_PACKET descriptors.
//

typedef struct _USBIO_ISO_TRANSFER {
  ULONG NumberOfPackets;
  ULONG Flags;
  ULONG StartFrame;
  ULONG ErrorCount;
} USBIO_ISO_TRANSFER;

typedef struct _USBIO_ISO_PACKET {
  ULONG Offset;
  ULONG Length;
  ULONG Status;   
} USBIO_ISO_PACKET;

typedef struct _USBIO_ISO_TRANSFER_HEADER {
  USBIO_ISO_TRANSFER  IsoTransfer;
  USBIO_ISO_PACKET    IsoPacket[1]; // variably sized array
} USBIO_ISO_TRANSFER_HEADER;


// restore previous settings
#ifdef __BORLANDC__
#pragma pack()
#endif

// restore packing
#include <POPPACK.H>


/********************** END OF INTERFACE ********************************/

//
// Define the device type value. Note that values used by Microsoft
// are in the range 0-32767, and 32768-65535 are reserved for use
// by customers.
//
#define FILE_DEVICE_USBIO       0x8094

//
// Macros to generate IOCTL codes.
// Note that function codes 0-2047 are reserved for Microsoft, and
// 2048-4095 are reserved for customers.
//
#define _USBIO_IOCTL_BASE       0x800

#define _USBIO_IOCTL_CODE(FnCode,Method)    \
   ( (ULONG)CTL_CODE(                       \
      (ULONG)FILE_DEVICE_USBIO,             \
      (ULONG)(_USBIO_IOCTL_BASE+(FnCode)),  \
      (ULONG)(Method),                      \
      (ULONG)FILE_ANY_ACCESS                \
      ) )


#endif  // _USBIO_I_H_

/*************************** EOF **************************************/
