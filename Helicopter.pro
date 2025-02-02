######################################################################
# Automatically generated by qmake (3.1) Thu Apr 29 15:15:52 2021
######################################################################

QT       += core gui network sql multimedia serialbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
TEMPLATE = app
TARGET = Helicopter
INCLUDEPATH += .

# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# Please consult the documentation of the deprecated API in order to know
# how to port your code away from it.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEFINES += QT_DEPRECATED_WARNINGS


include(Helicopter/Helicopter.pri)
include(Common/Common.pri)

win32: LIBS += -lCanApi4
INCLUDEPATH += C:/boost
LIBS += -L/C:/boost/libs

HEADERS += \
    FlightRecorder/checkbox.h \
    FlightRecorder/controlwidget.h \
    FlightRecorder/flightrecorder.h \
    FlightRecorder/fr_controls.h \
    FlightRecorder/plots.h \
    FlightRecorder/scales.h \
    Gen/Helicopter_SOD.h

SOURCES += \
    FlightRecorder/checkbox.cpp \
    FlightRecorder/controlwidget.cpp \
    FlightRecorder/flightrecorder.cpp \
    FlightRecorder/fr_controls.cpp \
    FlightRecorder/plots.cpp \
    FlightRecorder/scales.cpp \
    Gen/CanExchange.cpp
