/************************************************************************
 *
 *  Module:       UsbIoWriter.cpp
 *  Long name:    CUsbIoWriter class
 *  Description:  Writes a data stream to a pipe using a thread
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
#include "UsbIoWriter.h"



CUsbIoWriter::CUsbIoWriter()
{
  // empty
}


CUsbIoWriter::~CUsbIoWriter()
{
  // empty
}



// thread main routine
void CUsbIoWriter::ThreadRoutine()
{
  CUsbIoBuf *Buf;
  DWORD ErrorCounter=0;
  DWORD Status;

  if ( BufPool.CurrentCount() == 0 ) {
    // no BufPool is allocated !
    return;
  }

  // initializiation
  FirstPending=NULL;
  LastPending=NULL;

  // thread main loop
  for (;;) {
    
    if ( TerminateFlag ) {
      // thread should be terminated
      break;
    }

    if ( ErrorCounter > MaxErrorCount ) {
      // max number of io errors reached
      break;
    } 

    // submit all available buffers to the driver
    while ( (Buf=BufPool.Get()) != NULL ) {
      // get data
      ProcessBuffer(Buf);
      if ( !Buf->OperationFinished ) {
        // issue write request
        Write(Buf);
        // chain buffer to pending list
        if ( FirstPending == NULL ) {
          FirstPending = Buf;
        } else {
          LastPending->Next = Buf;
        }
        LastPending = Buf;
        Buf->Next = NULL;
      } else {
        // put the buffer back to pool
        BufPool.Put(Buf);
        // leave the complete loop
        goto Exit;
      }
    }

    if ( FirstPending==NULL ) {
      // no pending request, break loop
      break;
    }

    // wait for completion on the first pending buffer
    Buf = FirstPending;
    Status = WaitForCompletion(Buf);// INFINITE
    if ( Status != USBIO_ERR_SUCCESS ) {
      if (Status != USBIO_ERR_POWER_DOWN) {
        ErrorCounter++;
      }
      BufErrorHandler(Buf);
    } else {
      ErrorCounter=0;
    }

    // remove buffer from pending list
    FirstPending = Buf->Next;
    // put the buffer back to pool
    BufPool.Put(Buf);

  } //for
Exit:
  // wait for all pending buffers to complete
  while ( (Buf=FirstPending) != NULL ) {
    WaitForCompletion(Buf);// INFINITE
    FirstPending = Buf->Next;
    BufPool.Put(Buf);
  }

}



void CUsbIoWriter::TerminateThread()
{
  // cancel all outstanding io requests
  AbortPipe();
}


 
/*************************** EOF **************************************/
