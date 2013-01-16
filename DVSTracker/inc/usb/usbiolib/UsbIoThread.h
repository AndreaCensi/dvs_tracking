/************************************************************************
 *
 *  Module:       UsbIoThread.h
 *  Long name:    CUsbIoThread class
 *  Description:  Input/Output Thread, base class for Reader and Writer
 *
 *  Runtime Env.: Win32, Part of UsbioLib
 *  Author(s):    Guenter Hildebrandt, Udo Eberhardt, Mario Guenther
 *  Company:      Thesycon GmbH, Ilmenau
 ************************************************************************/

#ifndef _UsbIoThread_h_
#define _UsbIoThread_h_

#include "UsbIoPipe.h"


//
// CUsbIoThread
//
// basic implementation of a worker-thread
//
class CUsbIoThread : public CUsbIoPipe
{
public:

  //
  // standard constructor
  //
  CUsbIoThread();

  //
  // destructor
  //
  virtual ~CUsbIoThread();



  //
  // allocate the internal buffer pool
  // returns TRUE for success, FALSE otherwise
  //
  BOOL
  AllocateBuffers(
    DWORD SizeOfBuffer,
    DWORD NumberOfBuffers
    );

  //
  // free the internal buffer pool
  //
  void
  FreeBuffers();



  //
  // start the worker-thread
  // returns TRUE for success, FALSE otherwise
  //
  BOOL 
  StartThread(
    DWORD MaxIoErrorCount =3
    );

  //
  // terminate the worker thread
  // blocks until the thread has been terminated by the operating system
  // returns TRUE for success, FALSE otherwise
  //
  BOOL 
  ShutdownThread();



// overridable handlers
protected:

  //
  // process data received on a pipe
  // called by the worker-thread
  // used by CUsbIoReader
  // should be overloaded by a derived class
  //
  virtual
  void 
  ProcessData(
    CUsbIoBuf* Buf
    );

  //
  // fill a buffer before it will be submitted on the pipe
  // called by the worker-thread
  // used by CUsbIoWriter
  // should be overloaded by a derived class
  //
  virtual
  void 
  ProcessBuffer(
    CUsbIoBuf* Buf
    );

  //
  // handle a failed read or write request
  // called by the worker-thread
  // used by CUsbIoReader and CUsbIoWriter
  // should be overloaded by a derived class
  //  
  virtual
  void 
  BufErrorHandler(
    CUsbIoBuf* Buf
    );

  //
  // notification handler 
  // called by the worker thread before the thread terminates itself
  // may be overloaded by a derived class
  //
  virtual
  void
  OnThreadExit();



// implementation
protected:
  
  //
  // the main routine that is executed by the worker-thread
  // must be implemented by a derived class
  //
  virtual
  void 
  ThreadRoutine() =0;

  //
  // this routine is called to terminate the thread
  // called by ShutdownThread
  // must be implemented by a derived class
  //
  virtual 
  void 
  TerminateThread() =0;



  //
  // system thread routine
  //
  static unsigned int __stdcall beginthread_routine(void* StartContext);


// member variables
public:

	// priority of the thread
	int mThreadPriority;
protected:

  // internal buffer pool
  CUsbIoBufPool BufPool;

  // thread handle, NULL if invalid
  HANDLE ThreadHandle;
  
  // thread ID
  unsigned int ThreadID;

  
  //
  // variables used by the worker-thread routine
  //
  
  // terminate flag
  volatile BOOL TerminateFlag;
  // error limit
  DWORD MaxErrorCount;

  // list of pending buffers
  CUsbIoBuf* FirstPending;
  CUsbIoBuf* LastPending;

}; // class CUsbIoThread


#endif // _UsbIoThread_h_

/*************************** EOF **************************************/
