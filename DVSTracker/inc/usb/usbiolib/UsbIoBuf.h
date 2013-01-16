/************************************************************************
 *
 *  Module:       UsbIoBuf.h
 *  Long name:    CUsbIoBuf class
 *  Description:  definition of an USB I/O buffer and buffer pool
 *
 *  Runtime Env.: Win32, Part of UsbioLib
 *  Author(s):    Guenter Hildebrandt, Udo Eberhardt, Mario Guenther
 *  Company:      Thesycon GmbH, Ilmenau
 ************************************************************************/

#ifndef _UsbIoBuf_h_
#define _UsbIoBuf_h_


//
// CUsbIoBuf
//
// used as a buffer descriptor
//
class CUsbIoBuf
{
friend class CUsbIoBufPool;
friend class CUsbIoPipe;

public:

  //
  // standard constructor
  //
  CUsbIoBuf();

  //
  // construct and attach an existing buffer
  //
  CUsbIoBuf(void* Buffer, DWORD BufferSize);

  //
  // construct and allocate a buffer internally
  //
  CUsbIoBuf(DWORD BufferSize);

  //
  // destructor
  //
  ~CUsbIoBuf();

  //
  // get buffer pointer, NULL if no buffer is attached
  //
  void* 
  Buffer() { return BufferMem; }

  //
  // get buffer size, in bytes
  //
  DWORD
  Size() { return BufferSize; }


// public member variables
public:

  //
  // number of bytes to transfer from/to the buffer
  // must be set *before* Read or Write is called
  //
  DWORD NumberOfBytesToTransfer;
  
  //
  // number of bytes transferred from/to the buffer
  // will be set *after* the read or write operation is completed
  //
  DWORD BytesTransferred;
  
  //
  // final status of the read or write operation
  // will be set *after* the read or write operation is completed
  //
  DWORD Status;

  //
  // link pointer, can be used to build a chain of objects
  // used by CUsbIoBufPool and CUsbIoReader/CUsbIoWriter
  //
  CUsbIoBuf* Next;

  //
  // set this flag to TRUE in the ProcessBuffer routine
  // to terminate read or write processing
  //
  BOOL OperationFinished;

  //
  // an additional field that can be used freely by an application
  // This is a DWORD with pointer size, it is 32 bits on x86 and 64 bits on x64.
  //
  DWORD_PTR Context;


// implementation:
protected:

  // init member variables
  // helper function, used by constructors
  void 
  Init();


  // buffer pointer, NULL if no buffer is attached 
  void* BufferMem;    

  // buffer size, in bytes
  DWORD BufferSize;

  // embedded overlapped struct, used for asynch. I/O
  OVERLAPPED Overlapped;  

  // TRUE if buffer was allocated by constructor
  BOOL BufferMemAllocated;  

}; // class CUsbIoBuf



//
// CUsbIoBufPool
//
// implements a pool of CUsbIoBuf objects
//
class CUsbIoBufPool
{
public:

  //
  // standard constructor
  //
  CUsbIoBufPool();

  //
  // destructor
  //
  ~CUsbIoBufPool();

  //
  // allocate all elements of the buffer pool
  // returns TRUE for success, FALSE otherwise
  //
  BOOL 
  Allocate(
    DWORD SizeOfBuffer,
    DWORD NumberOfBuffers
    );

  //
  // free all elements of the buffer pool
  //
  void
  Free();

  //
  // get a buffer from pool
  // returns NULL if the pool is empty
  //
  CUsbIoBuf* 
  Get();

  //
  // put a buffer back to the pool
  //
  void
  Put(
    CUsbIoBuf* Buf
    );

  //
  // returns the current number of buffers in the pool
  //
  long
  CurrentCount()    { return Count; }
  

// implementation:
protected:

  // protects the pool
  CRITICAL_SECTION CritSect;

  // head of buffer chain
  CUsbIoBuf* Head;  

  // current number of buffers in pool
  long Count;        

  // allocated buf objects
  CUsbIoBuf* BufArray;
  // allocated buffer memory
  char* BufferMemory;

}; // class CUsbIoBufPool


#endif // _UsbIoBuf_h_

/*************************** EOF **************************************/
