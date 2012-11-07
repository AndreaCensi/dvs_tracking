/* This header file is part of the ATMEL AVR32-UC3-SoftwareFramework-1.6.0 Release */

#ifndef _CONF_USB_H_
#define _CONF_USB_H_

#include "compiler.h"
//#include "print_funcs.h"
#include "usb_ids.h"


//! @defgroup usb_general_conf USB application configuration
//!
//! @{

  // _________________ USB MODE CONFIGURATION ____________________________
  //
  //! @defgroup USB_op_mode USB operating modes configuration
  //! Defines to enable device or host USB operating modes
  //! supported by the application
  //! @{

    //! @brief ENABLE to activate the host software framework support
    //!
    //! Possible values ENABLED or DISABLED
#define USB_HOST_FEATURE            DISABLED
    //! @brief ENABLE to activate the device software framework support
    //!
    //! Possible values ENABLED or DISABLED
#define USB_DEVICE_FEATURE          ENABLED

  //! @}


  // _________________ DEVICE MODE CONFIGURATION __________________________
  //
  //! @defgroup USB_device_mode_cfg USB device operating mode configuration
  //!
  //! @{

#if USB_DEVICE_FEATURE == ENABLED

#define NB_ENDPOINTS          3 // Number of endpoints in the application including control endpoint
#define EP_TEMP_IN            1
#define EP_TEMP_OUT           2

    //! @defgroup device_cst_actions USB device custom actions
    //!
    //! @{
      // Write here the action to associate with each USB event.
      // Be careful not to waste time in order not to disturb the functions.
#define Usb_sof_action()                usb_sof_action()
#define Usb_wake_up_action()
#define Usb_resume_action()
#define Usb_suspend_action()
#define Usb_reset_action()
#define Usb_vbus_on_action()
#define Usb_vbus_off_action()
#define Usb_set_configuration_action()
    //! @}

extern void usb_sof_action(void);
extern void usb_suspend_action(void);

#endif  // USB_DEVICE_FEATURE == ENABLED

  //! @}


  //! USB interrupt priority level
#define USB_INT_LEVEL                   AVR32_INTC_INT0

  //! Debug trace macro
//#define LOG_STR(str)                    print_dbg(str)
#define LOG_STR(str)


//! @}


#endif  // _CONF_USB_H_
