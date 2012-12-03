TEMPLATE = app
CONFIG += console

QT       += core \
    network

INCLUDEPATH += \
    inc \
    inc/usbiolib

SOURCES += main.cpp \
    usbreader.cpp \
    camwidget.cpp \
    event.cpp \
    eventprocessor.cpp \
    cluster.cpp \
    transitionhistory.cpp \
    dvs128interface.cpp \
    filter.cpp \
    logger.cpp \
    transition.cpp \
    udpinterface.cpp \
    vector2d.cpp

HEADERS += \
    usbreader.h \
    stdafx.h \
    ringbuffer.h \
    camwidget.h \
    event.h \
    eventprocessor.h \
    cluster.h \
    eventprocessorbase.h \
    transitionhistory.h \
    dvs128interface.h \
    filter.h \
    logger.h \
    transition.h \
    udpinterface.h \
    vector2d.h



win32: LIBS += -L$$PWD/inc/usbiolib/Debug/Win32/ -lusbiolib

INCLUDEPATH += $$PWD/inc/usbiolib/Debug/Win32
DEPENDPATH += $$PWD/inc/usbiolib/Debug/Win32

win32: PRE_TARGETDEPS += $$PWD/inc/usbiolib/Debug/Win32/usbiolib.lib
