/************************************************************************
 *
 *  Module:       UsbIoPipe.cpp
 *  Long name:    CUsbIoPipe class
 *  Description:  USB In/Out Pipe class implementation
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
#include "usbiopipe.h"



  // standard constructor
CUsbIoPipe::CUsbIoPipe()
{
  // empty
}


// destructor
CUsbIoPipe::~CUsbIoPipe()
{
  // empty
}



DWORD CUsbIoPipe::Bind(
          int DeviceNumber,
          UCHAR EndpointAddress,
          HDEVINFO DeviceList /*=NULL*/,
          const GUID *InterfaceGuid, /*=NULL*/
					bool CheckApiVersion /*= true*/          )
{
  DWORD Status;

  if ( !IsOpen() ) {
    // open the device
    Status = Open(DeviceNumber, DeviceList, InterfaceGuid, CheckApiVersion);
    if (Status != USBIO_ERR_SUCCESS) {
      return Status;
    }
  }

  // bind to the specified endpoint
  Status = BindPipe(EndpointAddress);

  return Status;
}


DWORD CUsbIoPipe::BindPipe(UCHAR EndpointAddress)
{
  DWORD Status;

  // bind to an endpoint
  // zero the struct if any fields are added...
  USBIO_BIND_PIPE bp;
  ZeroMemory(&bp,sizeof(USBIO_BIND_PIPE));
  bp.EndpointAddress = EndpointAddress;
  Status = IoctlSync(
              IOCTL_USBIO_BIND_PIPE,
              &bp,
              sizeof(USBIO_BIND_PIPE),
              NULL,
              0,
              NULL
              );

  return Status;
}


DWORD CUsbIoPipe::Unbind()
{
  return IoctlSync(
              IOCTL_USBIO_UNBIND_PIPE,
              NULL,
              0,
              NULL,
              0,
              NULL
              );
}



DWORD CUsbIoPipe::ResetPipe()
{
  return IoctlSync(
              IOCTL_USBIO_RESET_PIPE,
              NULL,
              0,
              NULL,
              0,
              NULL
              );
}



DWORD CUsbIoPipe::AbortPipe()
{
  return IoctlSync(
              IOCTL_USBIO_ABORT_PIPE,
              NULL,
              0,
              NULL,
              0,
              NULL
              );
}



DWORD CUsbIoPipe::GetPipeParameters(USBIO_PIPE_PARAMETERS* PipeParameters)
{
  DWORD Status;

  // zero the struct if any fields are added...
  ZeroMemory(PipeParameters,sizeof(USBIO_PIPE_PARAMETERS));

  Status = IoctlSync(
              IOCTL_USBIO_GET_PIPE_PARAMETERS,
              NULL,
              0,
              PipeParameters,
              sizeof(USBIO_PIPE_PARAMETERS),
              NULL
              );

  return Status;
}



DWORD CUsbIoPipe::SetPipeParameters(const USBIO_PIPE_PARAMETERS* PipeParameters)
{
  DWORD Status;

  Status = IoctlSync(
              IOCTL_USBIO_SET_PIPE_PARAMETERS,
              PipeParameters,
              sizeof(USBIO_PIPE_PARAMETERS),
              NULL,
              0,
              NULL
              );

  return Status;
}



DWORD CUsbIoPipe::PipeControlTransferIn(
          void* Buffer,
          DWORD& ByteCount,
          const USBIO_PIPE_CONTROL_TRANSFER* ControlTransfer
          )
{
  DWORD Status;

  Status = IoctlSync(
              IOCTL_USBIO_PIPE_CONTROL_TRANSFER_IN,
              ControlTransfer,
              sizeof(USBIO_PIPE_CONTROL_TRANSFER),
              Buffer,
              ByteCount,
              &ByteCount
              );

  return Status;
}



DWORD CUsbIoPipe::PipeControlTransferOut(
          const void* Buffer,
          DWORD& ByteCount,
          const USBIO_PIPE_CONTROL_TRANSFER* ControlTransfer
          )
{
  DWORD Status;

  Status = IoctlSync(
              IOCTL_USBIO_PIPE_CONTROL_TRANSFER_OUT,
              ControlTransfer,
              sizeof(USBIO_PIPE_CONTROL_TRANSFER),
              (void*)Buffer,
              ByteCount,
              &ByteCount
              );

  return Status;
}


DWORD CUsbIoPipe::SetupPipeStatistics(ULONG AveragingInterval)
{
  USBIO_SETUP_PIPE_STATISTICS setup;
  ZeroMemory(&setup,sizeof(setup));

  setup.AveragingInterval = AveragingInterval;

  return IoctlSync(
              IOCTL_USBIO_SETUP_PIPE_STATISTICS,
              &setup,
              sizeof(setup),
              NULL,
              0,
              NULL
              );
}


DWORD CUsbIoPipe::QueryPipeStatistics(
        USBIO_PIPE_STATISTICS* PipeStatistics,
        ULONG Flags /*=0*/
        )
{
  USBIO_QUERY_PIPE_STATISTICS query;
  ZeroMemory(&query,sizeof(query));

  query.Flags = Flags;

  return IoctlSync(
              IOCTL_USBIO_QUERY_PIPE_STATISTICS,
              &query,
              sizeof(query),
              PipeStatistics,
              sizeof(USBIO_PIPE_STATISTICS),
              NULL
              );
}


DWORD CUsbIoPipe::ResetPipeStatistics()
{
  USBIO_PIPE_STATISTICS stats;

  return QueryPipeStatistics(
                &stats,
                USBIO_QPS_FLAG_RESET_ALL_COUNTERS
                );
}



BOOL CUsbIoPipe::Read(CUsbIoBuf* Buf)
{
  BOOL succ;

  succ = ReadFile(
          FileHandle,     // handle of file to read
          Buf->Buffer(),  // pointer to buffer that receives data
          Buf->NumberOfBytesToTransfer,  // number of bytes to read
          &Buf->BytesTransferred, // pointer to number of bytes read
          &Buf->Overlapped        // pointer to overlapped structure
          );
  if ( succ ) {
    Buf->Status = USBIO_ERR_SUCCESS;
  } else {
    Buf->Status = GetLastError();
    if ( Buf->Status == ERROR_IO_PENDING ) {
      succ = TRUE;
    }
  }

  return succ;
}



BOOL CUsbIoPipe::Write(CUsbIoBuf* Buf)
{
  BOOL succ;

  succ = WriteFile(
          FileHandle,     // handle of file to write 
          Buf->Buffer(),  // pointer to buffer to send data
          Buf->NumberOfBytesToTransfer,  // number of bytes to write
          &Buf->BytesTransferred, // pointer to number of bytes written
          &Buf->Overlapped        // pointer to overlapped structure
          );
  if ( succ ) {
    // the operation was completed with success
    Buf->Status = USBIO_ERR_SUCCESS;
  } else {
    Buf->Status = GetLastError();
    if ( Buf->Status == ERROR_IO_PENDING ) {
      // the operation is pending
      succ = TRUE;
    }
  }

  return succ;
}



DWORD CUsbIoPipe::WaitForCompletion(
        CUsbIoBuf* Buf, 
        DWORD Timeout /*=INFINITE*/,
        HANDLE AdditionalEvent /*=NULL*/
        )
{
  DWORD Status;
  
  if ( Buf->Status != ERROR_IO_PENDING ) {
    // the operation was already completed with success or with an error
    Status = Buf->Status;
  } else {
    // the operation is pending, we have to wait for completion
    // event objects to wait on
    HANDLE Events[2];
    Events[0] = Buf->Overlapped.hEvent;
    Events[1] = AdditionalEvent;
    DWORD EventCount = (AdditionalEvent==NULL) ? 1 : 2;
    // wait until one of the events is signalled or the timeout interval expired
    DWORD err = WaitForMultipleObjects(EventCount,Events,FALSE,Timeout);  
    switch ( err ) {
    case WAIT_TIMEOUT:
      {
        // timeout on wait
        Status = USBIO_ERR_TIMEOUT;
      }
      break;
    case WAIT_OBJECT_0:
      {
        // buffer was completed, get final status of the IO operation
        BOOL succ = GetOverlappedResult(
                        FileHandle,
                        &Buf->Overlapped,
                        &Buf->BytesTransferred,  // byte count
                        FALSE     // wait flag
                        );
        if ( succ ) {
          // success
          Buf->Status = USBIO_ERR_SUCCESS;
        } else {
          // error
          Buf->Status = GetLastError();
        }
        Status = Buf->Status;
      }
      break;
    case (WAIT_OBJECT_0+1):
      {
        // the AdditionalEvent is signalled
        Status = USBIO_ERR_ADDITIONAL_EVENT_SIGNALLED;
      }
      break;
    default:
      {
        // WaitForMultipleObjects failed for some reason, this should never happen
        Status = err;
      }
      break;
    }//switch
  }
  
  return Status;
}



DWORD CUsbIoPipe::ReadSync(
        void *Buffer,
        DWORD &ByteCount,
        DWORD Timeout /*=INFINITE*/ 
        )
{
  DWORD status;

  // init buffer descriptor
  CUsbIoBuf Buf(Buffer,ByteCount);
  Buf.NumberOfBytesToTransfer = ByteCount;
  // submit the buffer
  Read(&Buf);
  // wait for the transfer to complete
  status = WaitForCompletion(&Buf,Timeout);
  if ( status == USBIO_ERR_TIMEOUT ) {
    // a timeout is occurred
    // we must cancel the request because the 
    // Buf.OVERLAPPED struct is valid only in this function
    CancelIo();
    // now wait for the buffer to complete (infinite)
    //status = WaitForCompletion(&Buf,INFINITE);
    // in case of timeout the return status should be USBIO_ERR_TIMEOUT !
    WaitForCompletion(&Buf,INFINITE);
  }
  
  // return the number of bytes transferred
  ByteCount = Buf.BytesTransferred;
  
  return status;
}


DWORD CUsbIoPipe::WriteSync(
        void *Buffer,
        DWORD &ByteCount,
        DWORD Timeout /*=INFINITE*/ 
        )
{
  DWORD status;

  // init buffer descriptor
  CUsbIoBuf Buf(Buffer,ByteCount);
  Buf.NumberOfBytesToTransfer = ByteCount;
  // submit the buffer
  Write(&Buf);
  // wait for the transfer to complete
  status = WaitForCompletion(&Buf,Timeout);
  if ( status == USBIO_ERR_TIMEOUT ) {
    // a timeout is occurred
    // we must cancel the request because the 
    // Buf.OVERLAPPED struct is valid only in this function
    CancelIo();
    // now wait for the buffer to complete (infinite)
    //status = WaitForCompletion(&Buf,INFINITE);
    // in case of timeout the return status should be USBIO_ERR_TIMEOUT
    WaitForCompletion(&Buf,INFINITE);
  }
  
  // return the number of bytes transferred
  ByteCount = Buf.BytesTransferred;
  
  return status;
}


/*************************** EOF **************************************/
