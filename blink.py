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

def get_device():
    for bus in busses:
        devices = bus.devices
        for dev in devices:
            if dev.idVendor == VENDOR and dev.idProduct == PRODUCT:
                return dev
    return None

usbiocnt = 0L
def usbio(dh, f1, f2, dc):
    global usbiocnt
    usbiocnt += 1
    
    dout = array.array('B', [0]*4)

    dout[0] = 0xFF & 0x00
    dout[1] = 0xFF & (f1)
    dout[2] = 0xFF & (f2)
    dout[3] = 0xFF & (dc)  

    dh.bulkWrite(EP_OUT, dout.tostring())

    #if usbiocnt % PWMperADC == 0:
    if 1:

        din = dh.bulkRead(EP_IN, 4)
        l = len(din)
        if l != 4:
            print "unexpected bulk read length: %d" % l
        else:
            if usbiocnt % PWMperADC == 0:
                adc( (din[2] << 8) + din[3] )


def blink(dh,freq,dtyc):
    f1 = (freq >> 8) & 0xFF
    f2 = freq & 0xFF
    dc = dtyc & 0xFF
    usbio(dh,f1,f2,dc)

 
def main():

    dev = get_device()
    dh = dev.open()
    dh.claimInterface(IFACE)

    key_in = ""
    while key_in != 'q':
        key_in = raw_input('Enter frequency and duty cycle [f,dc]: ')
        if key_in != 'q':
            values = key_in.split(',') 
            freq = int(values[0])
            dtyc = int(values[1]) # in 0-100 %
            blink(dh,freq, dtyc)

    dh.releaseInterface()
    del dh
    return 0


if __name__ == '__main__':
   sys.exit( main() )
