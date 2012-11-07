/* This source file is part of the ATMEL AVR32-UC3-SoftwareFramework-1.6.0 Release */

/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief Main file of the USB enumeration example.
 *
 * - Compiler:           IAR EWAVR32 and GNU GCC for AVR32
 * - Supported devices:  All AVR32 devices with a USB module can be used.
 * - AppNote:
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support and FAQ: http://support.atmel.no/
 *
 ******************************************************************************/

/*! \mainpage AVR32 USB Software Framework for Dual-Role Devices
 *
 * \section intro License
 * Use of this program is subject to Atmel's End User License Agreement.
 *
 * Please read the \ref license at the bottom of this page.
 *
 * \section install Description
 * This embedded application source code illustrates how to implement a USB interfaced application
 * on the AVR32 microcontroller.
 *
 * As the AVR32 implements a device/host USB controller, the embedded application can operate
 * in one of the following USB operating modes:
 *   - USB device;
 *   - USB reduced-host controller;
 *   - USB dual-role device (depending on the ID pin).
 *
 * To optimize embedded code/RAM size and reduce the number of source modules, the application can be
 * configured to use one and only one of these operating modes.
 *
 * \section sample About the Sample Application
 * By default the sample code is delivered with a simple preconfigured dual-role USB application.
 * It means that the code generated allows to operate as a device or a host depending on the USB ID pin:
 *   - attached to a mini-B plug (ID pin unconnected) the application will be used in the device operating mode;
 *   - attached to a mini-A plug (ID pin tied to ground) the application will operate in reduced-host mode.
 *
 * \image html appli.jpg "Application Overview"
 *
 * Thus two instances of this application can be connected together with a mini-A - mini-B cable.
 *   - The host operating mode of the application (that we call A-device) can manage the connection
 * and the enumeration of a device application connected in device operating mode (that we call B-device).
 * Once the device has been enumerated, the host high-level application can operate USB applicative exchanges
 * with the B-device. Here the sample host application writes 64 bytes through a USB OUT pipe and reads back
 * this data with an IN pipe.
 *   - The device operating mode of the application (B-device) answers to the enumeration requests (setup requests)
 * of the host controller. Once it has been properly enumerated the high-level device application receives 64 bytes
 * through its OUT enpoint and when the host controller request for an IN exchange on its IN endpoints,
 * sends it back (loopback application).
 *
 * \note
 *    - When an EVK1101/EVK1103/EVK1104/EVK1105 is the host device, LED2 and LED3 will quickly blink. LED0 & LED1 will slowly blink with
 *    the following pattern: LED0 on/LED1 off, then LED0 off/LED1 on, then LED0 on/LED1 on.
 *    - When an EVK1100 is the host device, LED4(red light of the led tagged as LED5 on the PCB) will quickly blink.
 *    LED0, LED1, LED2 and LED3 will slowly blink in a complex pattern.
 *    - The host device may display the following messages on USART1(EVK1100/EVK1101)/USART0-CDC(EVK1105)/USART1-CDC(EVK1104)/USART2-CDC(EVK1103) depending
 *    on the current USB events: "Device connected", "Unsupported device",
 *    "Device enumerated", "USB suspended", "USB resumed". While the slave device
 *    never displays any message on USART.
 *
 * \section arch Architecture
 * As illustrated in the figure below, the application entry point is located in the enum_example.c file.
 * The main function first performs the initialization of services and tasks and then runs them in an infinite loop.
 *
 * The sample dual-role application is based on three different tasks:
 *   - The USB task (usb_task.c associated source file) is the task performing the USB low-level
 * enumeration process in device or host mode. Once this task has detected that the USB connection is fully
 * operational, it updates various status flags that can be checked within the high-level applicative tasks.
 *   - The device template task (device_template_task.c associated source file) performs the high-level device
 * application operation. Once the device is fully enumerated (device SET_CONFIGURATION request received), the task
 * checks for received data on its OUT endpoint and transmits this data on its IN endpoint.
 *   - The host template task (host_template_task.c associated file) performs the high-level host application operation.
 * If the B-device is correctly connected and enumerated, the task sends and receives data over the USB bus.
 *
 * \image html arch_full.jpg "Architecture Overview"
 *
 * \section config Configuration
 * The sample application is configured to implement both host and device functionalities.
 * Of course it can also be configured to be used only in device or reduced-host mode (see the conf_usb.h file).
 * Depending on the USB operating mode selected, the USB task will call either the USB host task (usb_host_task.c),
 * or the USB device task (usb_device_task.c) to manage USB specification chapter 9 requests.
 *
 * \note The B-device descriptors used for this sample application are not directly usable for enumeration
 *       with a standard PC host system. Please see \ref contactinfo for real device application examples
 *       (mass storage, HID mouse, HID keyboard, CDC, etc.).
 *
 * \section contactinfo Contact Information
 * For further information, visit
 * <A href="http://www.atmel.com/products/AVR32/">Atmel AVR32</A>.\n
 * Support and FAQ: http://support.atmel.no/
 *
 * \section license Copyright Notice
 * Copyright (c) 2009 Atmel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an Atmel
 * AVR product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 *
 */

//_____  I N C L U D E S ___________________________________________________

/*

#include "compiler.h"
//#include "print_funcs.h"
#include "intc.h"
#include "power_clocks_lib.h"
#include "conf_usb.h"
#include "usb_task.h"
#if USB_DEVICE_FEATURE == ENABLED
#include "device_task.h"
#endif

*/

//_____ D E F I N I T I O N S ______________________________________________

/*

int main(void)
{

  Enable_global_exception();
  INTC_init_interrupts();

  pcl_switch_to_osc(PCL_OSC0, FOSC0, OSC0_STARTUP);
  init_dbg_rs232(FOSC0);
  pcl_configure_usb_clock();

  usb_task_init();

#if USB_DEVICE_FEATURE == ENABLED
  device_task_init();
#endif


  while (TRUE)
  {
    usb_task();
  #if USB_DEVICE_FEATURE == ENABLED
    device_task();
  #endif

  }

}

*/
