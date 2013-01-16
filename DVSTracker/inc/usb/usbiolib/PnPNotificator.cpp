/************************************************************************
 *
 *  Module:       PnPNotificator.cpp
 *  Long name:    PnPNotificator class implementation
 *  Description:  implements Win32 device PnP notifications
 *
 *  Runtime Env.: Win32
 *  Author(s):    Frank Senf, Udo Eberhardt
 *  Company:      Thesycon GmbH, Ilmenau
 ************************************************************************/

#include "stdafx.h"

// define WINVER to 5.00 to enable RegisterDeviceNotification and 
// UnregisterDeviceNotification in WINDOWS.H and associated stuff in DBT.H
#ifdef WINVER
#undef WINVER
#endif
#define WINVER 0x0500
#include <windows.h>
#include <dbt.h>
#include <tchar.h>
#include <process.h>  // for _beginthreadex
#include "PnPNotificator.h"


// ctor
CPnPNotificator::CPnPNotificator()
{
  ::InitializeCriticalSection(&mCritSect);
  mInitialized = false;
  mNotificationList = NULL;
  mThreadHandle = NULL;
  mNotifyHandler = NULL;
  mWndOwner = NULL;
  mhWnd = NULL;
  mThreadReadyEvent = NULL;
}


// dtor
CPnPNotificator::~CPnPNotificator()
{
  // ensure everything is cleaned up
  Shutdown();
  ::DeleteCriticalSection(&mCritSect);
}



//
// Initialize the CPnPNotificator object. This creates and starts
// the internal worker thread. The worker thread will register a new
// window class and create a hidden window that will receive all 
// PnP notification messages.
// hInstance is an instance handle that identifies the owner of the
// hidden window to be created. In a DLL provide the hInstance value passed
// to DllMain. In an executable provide the hInstance value passed to WinMain.
//
// Returns true if successful, false otherwise.
//
bool
CPnPNotificator::Initialize(
  HINSTANCE hInstance,
  CPnPNotifyHandler* NotifyHandler
  )
{
  // check parameters
  if ( (hInstance==NULL) || (NotifyHandler==NULL) ) {
    return false;
  }

  ::EnterCriticalSection(&mCritSect);

  bool succ = true;

  // check state
  if ( mInitialized ) {
    // already initialized/running
    succ = false;
  } else {

    // save parameters
    mWndOwner = hInstance;
    mNotifyHandler = NotifyHandler;
    
    // create manual reset event
    mThreadReadyEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
    
    // create our worker thread
    unsigned int tid;
    mThreadHandle = (HANDLE)_beginthreadex( 
                                NULL,                 // void *security,
                                0,                    // unsigned stack_size, 
                                beginthread_routine,  // unsigned ( __stdcall *start_address )( void * ),
                                this,                 // void *arglist,
                                0,                    // unsigned initflag,
                                &tid                  // unsigned *thrdaddr
                                );
    if ( mThreadHandle == NULL ) {
      // failed
      succ = false;
    } else {

      // wait until worker thread has finished initialization
      // we use a timeout of 10 seconds
      DWORD err = ::WaitForSingleObject(mThreadReadyEvent,10*1000);
      if ( err != 0 ) {
        // failed
        succ = false;
      }
    }

    // destroy event, it's no longer needed
    ::CloseHandle(mThreadReadyEvent);
    mThreadReadyEvent = NULL;
  }

  // successfully initialized, update state
  mInitialized = true;
  
  ::LeaveCriticalSection(&mCritSect);

  return succ;

} // Initialize



//
// Shutdown the CPnPNotificator object. This will terminate the 
// internal worker thread and destroy the hidden window.
//
// Returns true if successful, false otherwise.
//
bool
CPnPNotificator::Shutdown(void)
{
  bool succ = true;

  ::EnterCriticalSection(&mCritSect);

  if ( mInitialized ) {
    
    // terminate worker thread
    if ( mThreadHandle != NULL ) {

      // NOTE: DestroyWindow needs to be called in the context of the thread
      // that created the window. So we post a WM_CLOSE message to the window.
      // This message will be routed to DefWindowProc which then will call 
      // DestroyWindow.
      HWND hwnd = mhWnd;
      if ( hwnd != NULL ) {
        ::PostMessage(hwnd,WM_CLOSE,0,0);
      }
    
      // wait until worker thread has terminated itself
      // we use a timeout of 10 seconds
      DWORD err = ::WaitForSingleObject(mThreadHandle,10*1000);
      if ( err != 0 ) {
        // failed
        succ = false;
      }

      // close thread handle
      ::CloseHandle(mThreadHandle);
      mThreadHandle = NULL;
    }
  
    // update state
    mInitialized = false;
  }

  // delete all active notification records
  DeleteAllNotificationRecords();

  ::LeaveCriticalSection(&mCritSect);

  return succ;

} // Shutdown



//
// Enable notifications for a particular class of device interfaces.
//
// Returns true if successful, false otherwise.
//
bool
CPnPNotificator::EnableDeviceNotifications(
  const GUID& InterfaceClassGuid
  )
{
  bool succ = true;

  ::EnterCriticalSection(&mCritSect);

  if ( !mInitialized ) {
    succ = false;
  } else {
  
    // create a new record
    NotificationRecord* nr = new NotificationRecord;
    nr->InterfaceClassGuid = InterfaceClassGuid;  // struct copy

    // register
    if ( !RegisterNotification(nr) ) {
      // failed
      delete nr;
      succ = false;
    } else {
      // add to list
      nr->Next = mNotificationList;
      mNotificationList = nr;
    }
  }

  ::LeaveCriticalSection(&mCritSect);

  return succ;

}//EnableDeviceNotifications



//
// Disable notifications for a particular class of device interfaces.
//
// Returns true if successful, false otherwise.
//
bool
CPnPNotificator::DisableDeviceNotifications(
  const GUID& InterfaceClassGuid
  )
{
  bool succ = true;

  ::EnterCriticalSection(&mCritSect);

  // run through active list
  NotificationRecord* nr = mNotificationList;
  NotificationRecord* NewList = NULL;
  while ( nr != NULL ) {
    NotificationRecord* next = nr->Next;
    if ( 0 == memcmp(&nr->InterfaceClassGuid, &InterfaceClassGuid, sizeof(GUID)) ) {
      // remove this record
      if ( !UnregisterNotification(nr) ) {
        succ = false;
      }
      delete nr;
    } else {
      // keep this record
      nr->Next = NewList;
      NewList = nr;
    }
    nr = next;
  }
  // new active list
  mNotificationList = NewList;

  ::LeaveCriticalSection(&mCritSect);

  return succ;

}//DisableDeviceNotifications



// delete all notification records in mNotificationList
void
CPnPNotificator::DeleteAllNotificationRecords(void)
{
  NotificationRecord* nr = mNotificationList;
  mNotificationList = NULL;
  while ( nr != NULL ) {
    NotificationRecord* next = nr->Next;
    UnregisterNotification(nr);
    delete nr;
    nr = next;
  }

} //DeleteAllNotificationRecords



//
// Register a new window class and create a hidden window.
// Returns true if successful, false otherwise.
//
bool
CPnPNotificator::RegisterAndCreateHiddenWindow(
	const TCHAR* wndClassName,
	const TCHAR* wndTitle
	)
{
  // register an application local window class
  WNDCLASSEX wcex;
  ZeroMemory(&wcex,sizeof(wcex));
  wcex.cbSize = sizeof(wcex); 
  //wcex.style      = 0;
  wcex.lpfnWndProc  = (WNDPROC)WndProc;
  //wcex.cbClsExtra   = 0;
  //wcex.cbWndExtra   = 0;
  wcex.hInstance    = mWndOwner;
  //wcex.hIcon      = NULL;
  //wcex.hCursor    = NULL;
  //wcex.hbrBackground  = NULL;
  //wcex.lpszMenuName = NULL;
  wcex.lpszClassName  = wndClassName;
  //wcex.hIconSm    = NULL;

  ATOM ClassAtom = ::RegisterClassEx(&wcex);
  if ( ClassAtom==0 ) {
    // error while registering new window class
    return false;
  }

  // create window, is initially hidden
  mhWnd = ::CreateWindow(
                wndClassName,		//LPCTSTR lpClassName,
                wndTitle,				//LPCTSTR lpWindowName,
                WS_OVERLAPPEDWINDOW,  // window style
                CW_USEDEFAULT,        // horizontal position of window
                CW_USEDEFAULT,        // vertical position of window
                CW_USEDEFAULT,        // window width
                CW_USEDEFAULT,        // window height
                NULL,         // handle to parent or owner window
                NULL,         // menu handle or child identifier
                mWndOwner, 
                NULL
                );
  if ( mhWnd==NULL ) {
    // error while creating the window
    // unregister class
    ::UnregisterClass(wndClassName, mWndOwner);
    return false;
  }

  // The window has been successfully created, attach this object
#if _MSC_VER <=1200
	// Visual C++ 6.0 and older
	::SetWindowLong(mhWnd,GWL_USERDATA,(LONG)this);
#else
	::SetWindowLongPtr(mhWnd,GWLP_USERDATA,(__int3264)(LONG_PTR)this);
#endif

  // success
  return true;

} // RegisterAndCreateHiddenWindow


//
// Unregister our window class.
// Returns true if successful, false otherwise.
//
bool
CPnPNotificator::UnregisterWindowClass(
	const TCHAR* wndClassName
	)
{
  bool succ = true;

  // unregister the window class
  if ( !::UnregisterClass(wndClassName, mWndOwner) ) {
    // error
    succ = false;
  }

  return succ;

} // UnregisterWindowClass



//
// Register the device interface type identified by Notification->InterfaceClassGuid 
// so that our internal hidden window will receive notifications.
// Returns true if successful, false otherwise.
//
bool
CPnPNotificator::RegisterNotification(
  NotificationRecord* Notification
  )
{
  // window must exist
  HWND hwnd = mhWnd;
  if ( hwnd == NULL ) {
    return false;
  }

  // register the window for receiving device change notifications
  DEV_BROADCAST_DEVICEINTERFACE Filter;
  ZeroMemory(&Filter, sizeof(Filter) );
  Filter.dbcc_size = sizeof(Filter);
  Filter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
  Filter.dbcc_classguid = Notification->InterfaceClassGuid; // struct copy

  Notification->hDevNotify = ::RegisterDeviceNotification(
                                  hwnd, 
                                  &Filter,
                                  DEVICE_NOTIFY_WINDOW_HANDLE
                                  );
  if ( Notification->hDevNotify==NULL ) {
    // error
    return false;
  }

  // success
  return true;

} //RegisterNotification



//
// Unregister the specified interface type
// Returns true if successful, false otherwise.
//
bool
CPnPNotificator::UnregisterNotification(
  NotificationRecord* Notification
  )
{
  bool succ = true;

  if ( Notification->hDevNotify != NULL ) {

    // running on Win2000 or better ...
    if ( !::UnregisterDeviceNotification(Notification->hDevNotify) ) {
      // error
      succ = false;
    }

    Notification->hDevNotify = NULL;
  }
  
  return succ;

}//UnregisterNotification




// thread routine called by C runtime
//static
unsigned int
__stdcall 
CPnPNotificator::beginthread_routine(
  void* StartContext
  )
{
  CPnPNotificator* This = (CPnPNotificator*)StartContext;
  
  // call member function
  // returning from there terminates the thread
  return This->ThreadRoutine();

} //beginthread_routine


//
// worker thread main routine
//
unsigned int
CPnPNotificator::ThreadRoutine(void)
{
	// use process and thread ID to create unique window class name
	// Note: this name is application local and should not cause conflicts with other applications.
	TCHAR wndClassName[120];
	_stprintf_s(wndClassName, TEXT("{3C8E6E6E-DE0A-4e6c-89DA-3EA5FD185B00}_PnPNotificatorWndClass_%u_%u"), ::GetCurrentProcessId(), ::GetCurrentThreadId());

	// use process and thread ID to create unique window title
	TCHAR wndTitle[120];
	_stprintf_s(wndTitle, TEXT("{3C8E6E6E-DE0A-4e6c-89DA-3EA5FD185B00}_PnPNotificatorWndTitle_%u_%u"), ::GetCurrentProcessId(), ::GetCurrentThreadId());
		
  // Register window class and create our hidden window.
  // This needs to be done in the context of the worker thread.
  bool succ = RegisterAndCreateHiddenWindow(wndClassName, wndTitle);
  
  // tell the main thread that initialization is done
  ::SetEvent(mThreadReadyEvent);

  if ( !succ ) {
    // failed
    return 0;
  } 


  // retrieve and process messages until WM_QUIT is encountered
  // NOTE: ::GetMessage returns 0 if WM_QUIT is detected.
  MSG msg;
  int ret;
  while ( (ret=::GetMessage(&msg, NULL, 0, 0)) != 0 ) {
    if ( ret == -1 ) {
      // error returned by ::GetMessage
      break;
    }
    ::TranslateMessage(&msg);
    ::DispatchMessage(&msg);
  }

  // window is already destroyed by DefWindowProc
  mhWnd = NULL;
    
  // unregister the window class
  UnregisterWindowClass(wndClassName);

  // exit thread
  return 0;

} // ThreadRoutine



// window proc routine
//static
LRESULT
CALLBACK
CPnPNotificator::WndProc(
  HWND hWnd,
  UINT uMsg,
  WPARAM wParam,
  LPARAM lParam
  )
{
  LRESULT retcode = 0;

  // retrieve the pointer to our CPnPNotificator instance
  CPnPNotificator* This = (CPnPNotificator*)(LONG_PTR)::GetWindowLongPtr(hWnd,GWLP_USERDATA);

  if ( This != NULL ) {
    retcode = This->WindowProc(hWnd, uMsg, wParam, lParam);
  } else {
    retcode = ::DefWindowProc(hWnd, uMsg, wParam, lParam);
  }

  return retcode;

} // WndProc



// window proc implementation
LRESULT
CPnPNotificator::WindowProc(
  HWND hWnd,
  UINT uMsg,
  WPARAM wParam,
  LPARAM lParam
  )
{
  LRESULT retcode = 0;

  switch ( uMsg ) {

  case WM_DESTROY:
    // post WM_QUIT to the window's message loop
    // this will terminate the worker thread
    ::PostQuitMessage(0);
    break;

  case WM_DEVICECHANGE:
    // this is a device PnP notification message
    // issue notification to handler
    IssuePnPNotification(uMsg, wParam, lParam);
    break;

  default:
    // let DefWindowProc handle the message
    // it handles standard messages like WM_CLOSE, WM_QUIT...
    retcode = ::DefWindowProc(hWnd, uMsg, wParam, lParam);

  } // switch

  return retcode;

} // WindowProc



// issue a PnP notification
void
CPnPNotificator::IssuePnPNotification(
  UINT uMsg,
  WPARAM wParam,
  LPARAM lParam
  )
{
  CPnPNotifyHandler* handler = mNotifyHandler;
  if ( handler != NULL ) {
    handler->HandlePnPMessage(uMsg, wParam, lParam);
  }

}//IssuePnPNotification




/*************************** EOF **************************************/
