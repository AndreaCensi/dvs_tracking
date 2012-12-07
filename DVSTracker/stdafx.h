#ifndef STDAFX_H
#define STDAFX_H

// define the Windows versions supported by the application
#define _WIN32_WINNT 0x0500     //Windows 2000 or later
//#define _WIN32_WINNT 0x0501     //Windows XP or later
//#define _WIN32_WINNT 0x0600     //Windows Vista or later

// exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include <stdio.h>
#include <tchar.h>
#include <conio.h>
// get setup API functions (only available in Win98 and later systems)
#include <setupapi.h>


// definitions of used classes
#include "usbio_i.h"
#include "usbio.h"
#include "UsbIoReader.h"

#endif // STDAFX_H
