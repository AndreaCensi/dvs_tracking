/************************************************************************
 *
 *  Module:       UsbIoWriter.h
 *  Long name:    CUsbIoWriter class
 *  Description:  Writes a data stream to a pipe using a thread
 *
 *  Runtime Env.: Win32, Part of UsbioLib
 *  Author(s):    Guenter Hildebrandt, Udo Eberhardt, Mario Guenther
 *  Company:      Thesycon GmbH, Ilmenau
 ************************************************************************/

#ifndef _UsbIoWriter_h_
#define _UsbIoWriter_h_

#include "UsbIoThread.h"


//
// CUsbIoWriter
//
// implements a worker-thread that continuously 
// writes a data stream to a pipe
//
class CUsbIoWriter : public CUsbIoThread
{
public:
  
  //
  // standard constructor
  //
  CUsbIoWriter();

  //
  // destructor
  //
  virtual ~CUsbIoWriter();


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

}; // class CUsbIoWriter


#endif //_UsbIoWriter_h_

/*************************** EOF **************************************/
