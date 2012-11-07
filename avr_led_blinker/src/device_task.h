
#ifndef _DEVICE_TASK_H_
#define _DEVICE_TASK_H_

//_____ I N C L U D E S ____________________________________________________

#include "conf_usb.h"

#if USB_DEVICE_FEATURE == DISABLED
  #error device_template_task.h is #included although USB_DEVICE_FEATURE is disabled
#endif


//_____ M A C R O S ________________________________________________________


//_____ D E C L A R A T I O N S ____________________________________________

extern void device_task_init(void);
extern void device_task(void);
extern void usb_sof_action(void);

#endif  // _DEVICE_TASK_H_
