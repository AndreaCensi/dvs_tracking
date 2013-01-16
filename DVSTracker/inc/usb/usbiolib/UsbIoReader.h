/************************************************************************
 *
 *  Module:       UsbIoReader.h
 *  Long name:    CUsbIoReader class
 *  Description:  Reads a data stream from a pipe using a thread
 *
 *  Runtime Env.: Win32, Part of UsbioLib
 *  Author(s):    Guenter Hildebrandt, Udo Eberhardt, Mario Guenther
 *  Company:      Thesycon GmbH, Ilmenau
 ************************************************************************/

#ifndef _UsbIoReader_h_
#define _UsbIoReader_h_

#include "UsbIoThread.h"


//
// CUsbIoReader
//
// implements a worker-thread that continuously 
// reads a data stream from a pipe
//
class CUsbIoReader : public CUsbIoThread
{
public:

  //
  // standard constructor
  //
  CUsbIoReader();

  //
  // destructor
  //
  virtual ~CUsbIoReader();


// implementation:
protected:

  //
  // ThreadRoutine, overloaded
  //
  virtual 
  void
  ThreadRoutine();

  //
  // TerminateThread, overloaded 
  //
  virtual 
  void 
  TerminateThread();

}; // class CUsbIoReader


#endif // _UsbIoReader_h_

/*************************** EOF **************************************/
