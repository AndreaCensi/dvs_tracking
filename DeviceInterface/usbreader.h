#ifndef USBREADER_H
#define USBREADER_H

#include "stdafx.h"

class USBReader : public CUsbIoReader
{
public:
    USBReader(/*void (*process)(char*)*/);
    ~USBReader();
protected:
    virtual void ProcessData(CUsbIoBuf *Buf);
private:
    void (*processEvent)(char*);
};

#endif // USBREADER_H
