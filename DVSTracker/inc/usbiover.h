/************************************************************************
 *
 *  Module:       usbiover.h
 *  Long name:    USBIO Version
 *  Description:  Defines the USBIO package version
 *
 *  Runtime Env.: any
 *  Author(s):    Udo Eberhardt
 *  Company:      Thesycon GmbH, Ilmenau
 ************************************************************************/

#ifndef __USBIOVER_H__
#define __USBIOVER_H__

//
// Current software version (applies to all modules).
// This indicates the version of the implementation, not of interfaces.
//
#define CURRENT_VERSION_MJ    2      /* major version number */
#define CURRENT_VERSION_MN    55      /* minor version number */
#define CURRENT_VERSION_SUB   0     /* sub version number */

//
// Current version of USBIO package
//
#define USBIO_VERSION_MJ   CURRENT_VERSION_MJ
#define USBIO_VERSION_MN   CURRENT_VERSION_MN



//
// copyright information
//

#define VER_PRODUCTNAME_STR         "Generic USB Device Driver"
#define VER_COMPANYNAME_STR         ""
#define VER_LEGALCOPYRIGHT_STR      "\251 1998-2011"


#define USBIO_PRODUCTNAME_STR     VER_PRODUCTNAME_STR

#define USBIO_COMPANYNAME_STR     VER_COMPANYNAME_STR
#define USBIO_COPYRIGHT_STR       VER_LEGALCOPYRIGHT_STR


// support macro
#define _TO_STRING_(x)     __TO_STRING__(x)
#define __TO_STRING__(x)   #x


#endif  // __USBIOVER_H__

/*************************** EOF **************************************/
