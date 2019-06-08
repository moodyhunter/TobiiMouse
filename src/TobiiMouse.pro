#-------------------------------------------------
#
# Project created by QtCreator 2019-05-24T22:10:56
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TobiiMouse
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x040000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        mouse_integration.cpp \
        noise_cancellation.cpp \
        qthread_controller/qthreadcontroller.cpp \
        qthread_controller/qthreadworker.cpp \
        tobii_interactive.cpp \
        tobii_interactive_threadworkers.cpp

HEADERS += \
        mainwindow.h \
        mouse_integration.h \
        noise_cancellation.h \
        qthread_controller/qthreadcontroller.h \
        qthread_controller/qthreadworker.h \
        tobii/tobii.h \
        tobii/tobii_advanced.h \
        tobii/tobii_config.h \
        tobii/tobii_engine.h \
        tobii/tobii_licensing.h \
        tobii/tobii_streams.h \
        tobii/tobii_wearable.h \
        tobii_interactive.h \
        tobiimouse.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


# X11 Support
unix:!macx: LIBS += -lX11 -lXrandr -lXtst


# Tobii LIBs

INCLUDEPATH += $$PWD/lib/x64
DEPENDPATH += $$PWD/lib/x64

INCLUDEPATH += $$PWD/lib/x86
DEPENDPATH += $$PWD/lib/x86

LIBS += -L$$PWD/lib/x86/ -ltobii_stream_engine
LIBS += -L$$PWD/lib/x64/ -ltobii_stream_engine

