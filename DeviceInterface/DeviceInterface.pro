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
    camwidget.cpp \
    event.cpp \
    eventprocessor.cpp \
    cluster.cpp \
    signalwrapper.cpp

HEADERS += \
    usbinterface.h \
    usbreader.h \
    stdafx.h \
    ringbuffer.h \
    camwidget.h \
    event.h \
    eventprocessor.h \
    cluster.h \
    signalwrapper.h \
    eventprocessorbase.h



win32: LIBS += -L$$PWD/inc/usbiolib/Debug/Win32/ -lusbiolib

INCLUDEPATH += $$PWD/inc/usbiolib/Debug/Win32
DEPENDPATH += $$PWD/inc/usbiolib/Debug/Win32

win32: PRE_TARGETDEPS += $$PWD/inc/usbiolib/Debug/Win32/usbiolib.lib
