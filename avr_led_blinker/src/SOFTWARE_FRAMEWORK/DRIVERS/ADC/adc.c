/* This source file is part of the ATMEL AVR32-UC3-SoftwareFramework-1.6.0 Release */

/*This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief ADC driver for AVR UC3.
 *
 * This file defines a useful set of functions for ADC on AVR UC3 devices.
 *
 * - Compiler:           IAR EWAVR32 and GNU GCC for AVR UC3
 * - Supported devices:  All AVR UC3 devices with an ADC can be used.
 * - AppNote:
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support and FAQ: http://support.atmel.no/
 *
 *****************************************************************************/

/* Copyright (c) 2009 Atmel Corporation. All rights reserved.
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

#include <avr32/io.h>
#include "compiler.h"
#include "adc.h"


void adc_configure(volatile avr32_adc_t * adc)
{
  Assert( adc!=NULL );

#ifdef USE_ADC_8_BITS
  adc->mr |= 1<<AVR32_ADC_LOWRES_OFFSET;
#endif
  // set Sample/Hold time to max so that the ADC capacitor should be loaded entirely
  adc->mr |= 0xF << AVR32_ADC_SHTIM_OFFSET;
  // set Startup to max so that the ADC capacitor should be loaded entirely
  adc->mr |= 0x1F << AVR32_ADC_STARTUP_OFFSET;
}

void adc_start(volatile avr32_adc_t * adc)
{
  Assert( adc!=NULL );

  // start conversion
  adc->cr = AVR32_ADC_START_MASK;
}

void adc_enable(volatile avr32_adc_t * adc, unsigned short channel)
{
  Assert( adc!=NULL );
  Assert( channel <= AVR32_ADC_CHANNELS_MSB );  // check if channel exist

  // enable channel
  adc->cher = (1 << channel);
}

void adc_disable(volatile avr32_adc_t * adc, unsigned short channel)
{
  Assert( adc!=NULL );
  Assert( channel <= AVR32_ADC_CHANNELS_MSB ); // check if channel exist

  if (adc_get_status(adc, channel) == ENABLED)
  {
    // disable channel
    adc->chdr |= (1 << channel);
  }
}

Bool adc_get_status(volatile avr32_adc_t * adc, unsigned short channel)
{
  Assert( adc!=NULL );
  Assert( channel <= AVR32_ADC_CHANNELS_MSB );  // check if channel exist

  return ((adc->chsr & (1 << channel)) ? ENABLED : DISABLED);
}

Bool adc_check_eoc(volatile avr32_adc_t * adc, unsigned short channel)
{
  Assert( adc!=NULL );
  Assert( channel <= AVR32_ADC_CHANNELS_MSB );  // check if channel exist

  // get SR register : EOC bit for channel
  return ((adc->sr & (1 << channel)) ? HIGH : LOW);
}

Bool adc_check_ovr(volatile avr32_adc_t * adc, unsigned short channel)
{
  Assert( adc!=NULL );
  Assert( channel <= AVR32_ADC_CHANNELS_MSB );  // check if channel exist

  // get SR register : OVR bit for channel
  return ((adc->sr & (1 << (channel + 8))) ? FAIL : PASS);
}

unsigned long adc_get_value(volatile avr32_adc_t * adc, unsigned short channel)
{
  Assert( adc!=NULL );
  Assert( channel <= AVR32_ADC_CHANNELS_MSB );  // check if channel exist

  // wait for end of conversion
  while(adc_check_eoc(adc, channel) != HIGH);
  return *((unsigned long * )((&(adc->cdr0)) + channel));
}


unsigned long adc_get_latest_value(volatile avr32_adc_t * adc)
{
  Assert( adc!=NULL );

  // Wait for the data ready flag
  while((adc->sr & AVR32_ADC_DRDY_MASK) != AVR32_ADC_DRDY_MASK);
  return adc->lcdr;
}

