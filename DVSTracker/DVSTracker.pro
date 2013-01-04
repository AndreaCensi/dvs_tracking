TEMPLATE = app
CONFIG += console

QT       += core \
    network \
    opengl

INCLUDEPATH += \
    inc \
    inc/usbiolib

SOURCES += main.cpp \
    usbreader.cpp \
    camwidget.cpp \
    event.cpp \
    dvs128interface.cpp \
    logger.cpp \
    transition.cpp \
    udpinterface.cpp \
    interval.cpp \
    tracker.cpp \
    frequencyaccumulator.cpp \
    filter.cpp \
    maxima.cpp \
    localmaximum.cpp \
    hypothesislogger.cpp \
    glcamwidget.cpp \
    particle.cpp \
    particlefilter.cpp \
    combinationanalyzer.cpp \
    combinationchoice.cpp

HEADERS += \
    usbreader.h \
    stdafx.h \
    ringbuffer.h \
    camwidget.h \
    event.h \
    dvs128interface.h \
    logger.h \
    transition.h \
    udpinterface.h \
    interval.h \
    map.h \
    tracker.h \
    frequencyaccumulator.h \
    filter.h \
    maxima.h \
    localmaximum.h \
    hypothesislogger.h \
    glcamwidget.h \
    particle.h \
    particlefilter.h \
    combinationanalyzer.h \
    combinationchoice.h



win32: LIBS += -L$$PWD/inc/usbiolib/Debug/Win32/ -lusbiolib

INCLUDEPATH += $$PWD/inc/usbiolib/Debug/Win32
DEPENDPATH += $$PWD/inc/usbiolib/Debug/Win32

win32: PRE_TARGETDEPS += $$PWD/inc/usbiolib/Debug/Win32/usbiolib.lib
