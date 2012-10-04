/************************************************************************
 *
 *  Module:       UsbIoBuf.cpp
 *  Long name:    CUsbIoBuf class
 *  Description:  implementation of an USB I/O buffer and buffer pool
 *                used by CUsbIoPipe::Read and CUsbIoPipe::Write
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
#include "usbiobuf.h"


//
// There is a problem with ISO transfers on an OHCI Host Controller.
// This seems to be a bug in OPENHCI.SYS or in the Aladdin chipset.
// If a transfer buffer ends on a 4K boundary the Open Host Controller behaves wrong.
// In a read operation, the buffer is not filled but the return status is success.
// In a write operation arbitrary data is sent (not the content of the buffer).
// Refer to problems.txt for further details.
//
// This switch enables a work-around for this problem.
// Note: The work-around is needed only if ISO transfers are used.
// It can be disabled if Bulk and Interrupt transfers are used only.
//
#define ENABLE_OHCI_ISO_WORKAROUND    0



//
// CUsbIoBuf
//

CUsbIoBuf::CUsbIoBuf()
{
  Init();
} 
  

CUsbIoBuf::CUsbIoBuf(void *buffer, DWORD buffersize)
{
  Init();
  BufferMem = buffer;
  BufferSize = buffersize;
} 


CUsbIoBuf::CUsbIoBuf(DWORD buffersize)
{
  Init();
  if ( buffersize != 0 ) {
    BufferMem = new char[buffersize];
    BufferSize = buffersize;
    BufferMemAllocated = TRUE;
  }
} 


CUsbIoBuf::~CUsbIoBuf()
{
  if ( Overlapped.hEvent != NULL ) {
    ::CloseHandle(Overlapped.hEvent);
    Overlapped.hEvent = NULL;
  }
  if ( BufferMemAllocated && BufferMem!=NULL ) {
    delete [] BufferMem;
    BufferMem = NULL;
  }
}



void CUsbIoBuf::Init()
{
  NumberOfBytesToTransfer = 0;
  BytesTransferred = 0;
  Status = 0;
  Next = NULL;
  OperationFinished = FALSE;
  Context = 0;
  BufferMem = NULL;
  BufferSize = 0;
  BufferMemAllocated = FALSE;

  ZeroMemory(&Overlapped,sizeof(OVERLAPPED));
  Overlapped.hEvent = ::CreateEvent(NULL ,TRUE ,FALSE ,NULL); 
} 




//
// CUsbIoBufPool
//

CUsbIoBufPool::CUsbIoBufPool()
{
  Head = NULL;
  Count = 0;
  BufArray = NULL;
  BufferMemory = NULL;

  InitializeCriticalSection(&CritSect);
}


CUsbIoBufPool::~CUsbIoBufPool()
{
  Free();
  DeleteCriticalSection(&CritSect);
}



BOOL CUsbIoBufPool::Allocate(DWORD SizeOfBuffer, DWORD NumberOfBuffers)
{
  DWORD i;
  CUsbIoBuf *buf;
  BOOL succ = FALSE;
  DWORD Size;
  char *mem;

  EnterCriticalSection(&CritSect);

  if ( BufArray!=NULL ) {
    // is already allocated
  } else {
    
    BufArray = new CUsbIoBuf[NumberOfBuffers];
    if ( BufArray==NULL ) {
      // failed
    } else {
      // total size of memory needed
      Size = SizeOfBuffer*NumberOfBuffers;
#if ENABLE_OHCI_ISO_WORKAROUND
      // add 1 to force memory alignment at an odd boundary (see above)
      Size += 1;
#endif
      // alloc buffer memory
      BufferMemory = new char[Size];
      if ( BufferMemory==NULL ) {
        // failed
        delete [] BufArray;
        BufArray = NULL;
      } else {
        
        mem = BufferMemory;
#if ENABLE_OHCI_ISO_WORKAROUND
        // add 1 to force memory alignment at an odd boundary (see above)
        mem += 1;
#endif
        // init all buffer descriptors
        for (i=0;i<NumberOfBuffers;i++) {
          buf = &BufArray[i];
          buf->BufferMem = mem + (i*SizeOfBuffer);
          buf->BufferSize = SizeOfBuffer;
          // link to the list
          buf->Next = Head;
          Head = buf;
        }

        // success
        Count = NumberOfBuffers;
        succ = TRUE;
      }
    }
  }

  LeaveCriticalSection(&CritSect);

  return succ;
}



void CUsbIoBufPool::Free()
{
  EnterCriticalSection(&CritSect);

  if (BufArray != NULL) {
    delete [] BufArray;
    BufArray = NULL;
  }

  if (BufferMemory != NULL) {
    delete [] BufferMemory;
    BufferMemory = NULL;
  }

  Head = NULL;
  Count = 0;

  LeaveCriticalSection(&CritSect);
}



CUsbIoBuf* CUsbIoBufPool::Get()
{
  CUsbIoBuf *buf;

  EnterCriticalSection(&CritSect);

  buf = Head;
  if (buf == NULL) {
    // pool is empty
  } else {
    Head = buf->Next;
    buf->Next = NULL;
    InterlockedDecrement(&Count);
  }

  LeaveCriticalSection(&CritSect);

  return buf;
}



void CUsbIoBufPool::Put(CUsbIoBuf* Buf)
{
  EnterCriticalSection(&CritSect);

  Buf->Next = Head;
  Head = Buf;
  InterlockedIncrement(&Count);

  LeaveCriticalSection(&CritSect);
}

 

/*************************** EOF **************************************/
