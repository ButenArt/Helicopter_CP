SOURCES += $$PWD/CAN_common.cpp \
    $$PWD/CAN4.cpp \
    $$PWD/CAN_Interface.cpp \
    $$PWD/Logging.cpp \
    $$PWD/MOXA.cpp \
    $$PWD/PCAN.cpp \
    $$PWD/ServerInstructor.cpp \
    $$PWD/SettingsFile.cpp \
    $$PWD/Replay.cpp \
    $$PWD/UDP_OU.cpp \
    $$PWD/SystemTray.cpp \
    $$PWD/ServiceErrors.cpp \
    $$PWD/TCP_client.cpp \
    $$PWD/Controls.cpp \
    $$PWD/Audio.cpp \
    $$PWD/Server.cpp \
    $$PWD/Script.cpp \
    $$PWD/FCC.cpp \
    $$PWD/MotionSystem.cpp \
    $$PWD/manager.cpp

HEADERS  += \
    $$PWD/CAN4.h \
    $$PWD/CAN_Interface.h \
    $$PWD/CAN_common.h \
    $$PWD/MOXA.h \
    $$PWD/PCAN.h \
    $$PWD/Printer.h \
    $$PWD/ServerInstructor.h \
    $$PWD/avia_dictionary.h \
    $$PWD/common_dictionary.h \
    $$PWD/Logging.h \
    $$PWD/SettingsFile.h \
    $$PWD/Replay.h \
    $$PWD/UDP_OU.h \
    $$PWD/SystemTray.h \
    $$PWD/ServiceErrors.h \
    $$PWD/TCP_client.h \
    $$PWD/Controls.h \
    $$PWD/Audio.h \
    $$PWD/Server.h \
    $$PWD/Script.h \
    $$PWD/FCC.h \
    $$PWD/MotionSystem.h \
    $$PWD/log_duration.h \
    $$PWD/manager.h \
    $$PWD/queuetimer.h
	
RESOURCES += \
    $$PWD/Common.qrc \
    $$PWD/Common.qrc

DISTFILES +=

include(MOXA/MOXA.pri)
include(PCAN/PCAN.pri)
include(Joystick/Joystick.pri)
