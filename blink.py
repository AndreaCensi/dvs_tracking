#!/usr/bin/env python

import sys, os
import array
import usb
import colorsys
import time
import math

busses = usb.busses()

VENDOR = 0x03eb
PRODUCT = 0x2300
IFACE = 0
EP_IN = 0x81
EP_OUT = 0x02


deltah = 0.0003
T = 0.00
PWMperADC = 33
BAR = 80

# channel presets
f = array.array('I',[900,1250,1600,1,1950])
dc = array.array('I',[50,50,50,0,50])

def get_device():
    for bus in busses:
        devices = bus.devices
        for dev in devices:
            if dev.idVendor == VENDOR and dev.idProduct == PRODUCT:
                return dev
    return None

usbiocnt = 0L
def usbio(dh, c_id, f, dc): #c_id: channel id, f1,f2: frequency, dc: duty cycle
    global usbiocnt
    usbiocnt += 1

    f1 = (f >> 8) & 0xFF
    f2 = f & 0xFF
    
    dout = array.array('B', [0]*5)

    dout[0] = 0xFF & 0x00
    dout[1] = 0xFF & c_id
    dout[2] = f1
    dout[3] = f2
    dout[4] = 0xFF & dc  

    dh.bulkWrite(EP_OUT, dout.tostring())

def run_preset(dh):
    for i in range(5):
        usbio(dh,i,f[i],dc[i])
        
 
def main():
    dev = get_device()
    dh = dev.open()
    dh.claimInterface(IFACE)

    key_in = ""
    while key_in != 'q':
        key_in = raw_input('Set PWM [channel_ID,frequency,duty_cycle]: ')
        if key_in == 'preset':
            run_preset(dh)
        elif key_in != 'q':
            values = key_in.split(',')
            c_id = int(values[0])
            freq = int(values[1])
            dtyc = int(values[2]) # in 0-100 %
            usbio(dh,c_id,freq, dtyc)

    dh.releaseInterface()
    del dh
    return 0


if __name__ == '__main__':
   sys.exit( main() )
