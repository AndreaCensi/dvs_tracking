TEMPLATE = app
CONFIG += console

QT       += core \
    network \
    opengl

INCLUDEPATH += \
    inc/usb \
    inc/usb/usbiolib \
    $$PWD/../../../../../../Downloads/opencv \
    $$PWD/../../../../../../Downloads/opencv/build/include

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
    combinationchoice.cpp \
    combinations.cpp \
    eventpacket.cpp \
    packetbuffer.cpp \
    poseestimation.cpp \
    poselogger.cpp

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
    combinationchoice.h \
    combinations.h \
    eventpacket.h \
    packetbuffer.h \
    poseestimation.h \
    poselogger.h



win32: LIBS += -L$$PWD/inc/usb/usbiolib/Debug/Win32/ -lusbiolib

INCLUDEPATH += $$PWD/inc/usb//usbiolib/Debug/Win32
DEPENDPATH += $$PWD/inc/usb/usbiolib/Debug/Win32

win32: PRE_TARGETDEPS += $$PWD/inc/usb/usbiolib/Debug/Win32/usbiolib.lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../Downloads/opencv/build/x86/vc10/lib/ -lopencv_core243
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../Downloads/opencv/build/x86/vc10/lib/ -lopencv_core243d

INCLUDEPATH += $$PWD/../../../../../../Downloads/opencv/build/x86/vc10
DEPENDPATH += $$PWD/../../../../../../Downloads/opencv/build/x86/vc10

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../Downloads/opencv/build/x86/vc10/lib/ -lopencv_calib3d243
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../Downloads/opencv/build/x86/vc10/lib/ -lopencv_calib3d243d

INCLUDEPATH += $$PWD/../../../../../../Downloads/opencv/build/x86/vc10
DEPENDPATH += $$PWD/../../../../../../Downloads/opencv/build/x86/vc10

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../Downloads/opencv/build/x86/vc10/lib/ -lopencv_features2d243
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../Downloads/opencv/build/x86/vc10/lib/ -lopencv_features2d243d

INCLUDEPATH += $$PWD/../../../../../../Downloads/opencv/build/x86/vc10
DEPENDPATH += $$PWD/../../../../../../Downloads/opencv/build/x86/vc10

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../Downloads/opencv/build/x86/vc10/lib/ -lopencv_imgproc243
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../Downloads/opencv/build/x86/vc10/lib/ -lopencv_imgproc243d

INCLUDEPATH += $$PWD/../../../../../../Downloads/opencv/build/x86/vc10
DEPENDPATH += $$PWD/../../../../../../Downloads/opencv/build/x86/vc10
