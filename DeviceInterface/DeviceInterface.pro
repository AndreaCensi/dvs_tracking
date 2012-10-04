TEMPLATE = app
CONFIG += console
CONFIG -= qt

INCLUDEPATH += \
    inc \
    inc/usbiolib

SOURCES += main.cpp \
    usbinterface.cpp \
    usbreader.cpp

HEADERS += \
    usbinterface.h \
    usbreader.h \
    stdafx.h

win32: LIBS += -L$$PWD/inc/usbiolib/Debug/Win32/ -lusbiolib

INCLUDEPATH += $$PWD/inc/usbiolib/Debug/Win32
DEPENDPATH += $$PWD/inc/usbiolib/Debug/Win32

win32: PRE_TARGETDEPS += $$PWD/inc/usbiolib/Debug/Win32/usbiolib.lib
