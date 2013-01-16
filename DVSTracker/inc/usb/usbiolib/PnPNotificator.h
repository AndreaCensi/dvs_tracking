/************************************************************************
 *
 *  Module:       PnPNotificator.h
 *  Long name:    PnPNotificator class definition
 *  Description:  implements Win32 device PnP notifications
 *
 *  Runtime Env.: Win32
 *  Author(s):    Frank Senf, Udo Eberhardt
 *  Company:      Thesycon GmbH, Ilmenau
 ************************************************************************/

#ifndef _PnPNotificator_h_
#define _PnPNotificator_h_


//
// CPnPNotifyHandler
//
// Abstract base class that defines the interface to be implemented
// to receive notifications from CPnPNotificator.
//
class CPnPNotifyHandler
{
public:

  // called if a WM_DEVICECHANGE is issued by the system
  virtual 
  void 
  HandlePnPMessage(
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
    ) = 0;

}; //class CPnPNotifyHandler



//
// CPnPNotificator
//
// This class implements a worker thread that uses a hidden 
// window to receive WM_DEVICECHANGE messages issued by the system.
//
class CPnPNotificator
{
public:
  // constructor
  CPnPNotificator();
  // destructor
  ~CPnPNotificator();


////////////////////////////////////////////////////////////
// interface
////////////////////////////////////////////////////////////
public:

  //
  // Initialize the CPnPNotificator object. This creates and starts
  // the internal worker thread. The worker thread will register a new
  // window class and create a hidden window that will receive all 
  // PnP notification messages.
  //
  // Returns true if successful, false otherwise.
  //
  bool
  Initialize(
    HINSTANCE hInstance,
    CPnPNotifyHandler* NotifyHandler
    );

  //
  // Shutdown the CPnPNotificator object. This will terminate the 
  // internal worker thread and destroy the hidden window.
  //
  // Returns true if successful, false otherwise.
  //
  bool
  Shutdown(void);


  //
  // Enable notifications for a particular class of device interfaces.
  //
  // Returns true if successful, false otherwise.
  //
  bool
  EnableDeviceNotifications(
    const GUID& InterfaceClassGuid
    );

  //
  // Disable notifications for a particular class of device interfaces.
  //
  // Returns true if successful, false otherwise.
  //
  bool
  DisableDeviceNotifications(
    const GUID& InterfaceClassGuid
    );



////////////////////////////////////////////////////////////
// implementation
////////////////////////////////////////////////////////////
protected:

  // this object stores a device notification that has been registered
  struct NotificationRecord 
  {
    // device interface class
    GUID InterfaceClassGuid;
    // handle returned by RegisterDeviceNotification
    HDEVNOTIFY hDevNotify;
    // pointer to next element
    NotificationRecord* Next;

    // constructor
    NotificationRecord()
      {
        ZeroMemory(this, sizeof(*this));
      }
  };


  // delete all notification records in mNotificationList
  void
  DeleteAllNotificationRecords(void);

  //
  // Register a new window class and create a hidden window.
  // Returns true if successful, false otherwise.
  //
  bool
  RegisterAndCreateHiddenWindow(
		const TCHAR* wndClassName,
		const TCHAR* wndTitle
		);

  //
  // Unregister our window class.
  // Returns true if successful, false otherwise.
  //
  bool
  UnregisterWindowClass(
		const TCHAR* wndClassName
		);

  //
  // Register the device interface type identified by Notification.InterfaceClassGuid 
  // so that our internal hidden window will receive notifications.
  // Returns true if successful, false otherwise.
  //
  bool
  RegisterNotification(
    NotificationRecord* Notification
    );

  //
  // Unregister the specified interface type
  // Returns true if successful, false otherwise.
  //
  bool
  UnregisterNotification(
    NotificationRecord* Notification
    );


  // thread routine called by C runtime
  static
  unsigned int
  __stdcall 
  beginthread_routine(
    void* StartContext
    );

  // worker thread main routine
  unsigned int
  ThreadRoutine(void);

  // window proc routine
  static 
  LRESULT 
  CALLBACK
  WndProc(
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
    );

  // window proc implementation
  // declared virtual in order to allow derived classes
  // to modify the behavior
  virtual
  LRESULT
  WindowProc(
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
    );

  // issue a PnP notification
  void
  IssuePnPNotification(
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
    );


////////////////////////////////////////////////////////////
// member variables
protected:

  // critical section that protects the object
  CRITICAL_SECTION mCritSect;

  // state 
  bool mInitialized;

  // single-linked list that holds all notification records currently active
  NotificationRecord* mNotificationList;
  
  // system thread object
  HANDLE mThreadHandle;

  // pointer to notify handler that will receive notifications
  CPnPNotifyHandler* volatile mNotifyHandler;

  // owner of the window
  volatile HINSTANCE mWndOwner;

  // handle of hidden window
  volatile HWND mhWnd;

  // event signaled by worker thread if initialization is finished
  volatile HANDLE mThreadReadyEvent;

};


#endif // _PnPNotificator_h_

/*************************** EOF **************************************/
