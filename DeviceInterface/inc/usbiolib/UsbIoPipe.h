/************************************************************************
 *
 *  Module:       UsbIoPipe.h
 *  Long name:    CUsbIoPipe class
 *  Description:  USB In/Out Pipe class definition
 *
 *  Runtime Env.: Win32, Part of UsbioLib
 *  Author(s):    Guenter Hildebrandt, Udo Eberhardt, Mario Guenther
 *  Company:      Thesycon GmbH, Ilmenau
 ************************************************************************/

#ifndef _UsbIoPipe_h_
#define _UsbIoPipe_h_

#include "UsbIo.h"
#include "UsbIoBuf.h"


//
// CUsbIoPipe
//
// USBIO programming interface
// pipe-related functions
//
class CUsbIoPipe : public CUsbIo
{
public:

  //
  // standard constructor
  //
  CUsbIoPipe();
  
  //
  // destructor
  //
  virtual ~CUsbIoPipe();

  //
  // bind an endpoint
  // calls Open if not already open
  // returns status
  //
  DWORD
  Bind( 
    int DeviceNumber,
    UCHAR EndpointAddress,
    HDEVINFO DeviceList =NULL,
    const GUID* InterfaceGuid =NULL,
    bool CheckApiVersion = true
    );

	DWORD 
	BindPipe(
		UCHAR EndpointAddress
		);


  //
  // delete the binding to an endpoint
  // returns status
  //
  DWORD
  Unbind();



  //
  // submit a read request on the pipe
  // the call returns immediately (asynchronous behavior)
  // returns TRUE if the request was successfully submitted, FALSE otherwise
  //
  BOOL
  Read(
    CUsbIoBuf* Buf
    );

  //
  // submit a write request on the pipe
  // the call returns immediately (asynchronous behavior)
  // returns TRUE if the request was successfully submitted, FALSE otherwise
  //
  BOOL 
  Write(
    CUsbIoBuf* Buf
    );

  //
  // wait for completion of a read or write operation
  // returns either USBIO_ERR_TIMEOUT or the final completion status
  //
  DWORD
  WaitForCompletion(
    CUsbIoBuf* Buf,
    DWORD Timeout =INFINITE,
    HANDLE AdditionalEvent =NULL
    );



  //
  // submit a read request on the pipe and wait for its completion
  // the call returns after completion (synchronous behavior)
  // returns either USBIO_ERR_TIMEOUT or the completion status
  //
  DWORD
  ReadSync(
    void* Buffer,
    DWORD& ByteCount,
    DWORD Timeout =INFINITE
    );

  //
  // submit a write request on the pipe and wait for its completion
  // the call returns after completion (synchronous behavior)
  // returns either USBIO_ERR_TIMEOUT or the completion status
  //
  DWORD 
  WriteSync(
    void* Buffer,
    DWORD& ByteCount,
    DWORD Timeout =INFINITE
    );



  //
  // reset pipe, clear an error condition
  // returns status
  //
  DWORD
  ResetPipe();

  //
  // cancel all pending read/write requests on this pipe
  // returns status
  //
  DWORD
  AbortPipe();


  
  //
  // get pipe parameters
  // returns status
  //
  DWORD
  GetPipeParameters(
    USBIO_PIPE_PARAMETERS* PipeParameters
    );

  //
  // set pipe parameters
  // returns status
  //
  DWORD 
  SetPipeParameters(
    const USBIO_PIPE_PARAMETERS* PipeParameters
    );



  //
  // generate a control transfer (SETUP token) on the pipe
  // with a data phase in IN direction
  // returns status
  //
  DWORD
  PipeControlTransferIn(
    void* Buffer,
    DWORD& ByteCount,
    const USBIO_PIPE_CONTROL_TRANSFER* ControlTransfer
    );

  //
  // generate a control transfer (SETUP token) on the pipe
  // with a data phase in OUT direction
  // returns status
  //
  DWORD 
  PipeControlTransferOut(
    const void* Buffer,
    DWORD& ByteCount,
    const USBIO_PIPE_CONTROL_TRANSFER* ControlTransfer
    );


  //
  // setup pipe statistics
  //
  DWORD
  SetupPipeStatistics(
    ULONG AveragingInterval
    );

  //
  // query pipe statistics
  //
  DWORD 
  QueryPipeStatistics(
    USBIO_PIPE_STATISTICS* PipeStatistics,
    ULONG Flags =0
    );


  //
  // reset pipe statistics
  //
  DWORD 
  ResetPipeStatistics();


}; // class CUsbIoPipe


#endif // _UsbIoPipe_h_

/*************************** EOF **************************************/
