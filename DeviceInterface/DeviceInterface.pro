TEMPLATE = app
CONFIG += console

QT       += core \
    network

INCLUDEPATH += \
    inc \
    inc/usbiolib

SOURCES += main.cpp \
    usbinterface.cpp \
    usbreader.cpp \
    eventreader.cpp \
    udpclient.cpp \
    camwidget.cpp \
    event.cpp

HEADERS += \
    usbinterface.h \
    usbreader.h \
    stdafx.h \
    ringbuffer.h \
    eventreader.h \
    udpclient.h \
    camwidget.h \
    event.h



win32: LIBS += -L$$PWD/inc/usbiolib/Debug/Win32/ -lusbiolib

INCLUDEPATH += $$PWD/inc/usbiolib/Debug/Win32
DEPENDPATH += $$PWD/inc/usbiolib/Debug/Win32

win32: PRE_TARGETDEPS += $$PWD/inc/usbiolib/Debug/Win32/usbiolib.lib
