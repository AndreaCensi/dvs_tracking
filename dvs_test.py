#!/usr/bin/env python

import sys, os
import array
import usb
import colorsys
import time
import math

busses = usb.busses()

VENDOR = 0x152A
PRODUCT = 0x8400
IFACE = 0
EP_IN = 0x81
EP_OUT = 0x02


def get_device():
    for bus in busses:
        devices = bus.devices
        for dev in devices:
            if dev.idVendor == VENDOR and dev.idProduct == PRODUCT:
                return dev
    return None
 
def main():

    dev = get_device()
    dh = dev.open()
    dh.claimInterface(IFACE)

    dout = 0xFF & 0xb3

    dh.bulkWrite(EP_OUT, dout.tostring())
    
    while 1:
        din = dh.bulkRead(EP_IN, 32)
        print(din)
        print('\n')

    dh.releaseInterface()
    del dh
    return 0


if __name__ == '__main__':
   sys.exit( main() )
