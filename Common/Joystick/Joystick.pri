SOURCES += \
    $$PWD\LogiControllerInput.cpp \
    $$PWD\LogiControllerInputUtils.cpp \
    $$PWD\LogiGameController.cpp \
    $$PWD\LogiGameControllerDI.cpp

HEADERS  += \
    $$PWD\LogiControllerInput.h \
    $$PWD\LogiControllerInputGlobals.h \
    $$PWD\LogiControllerInputUtils.h \
    $$PWD\LogiGameController.h \
    $$PWD\LogiGameControllerDI.h

DISTFILES +=

win32: LIBS += -lUser32
win32: LIBS += -lOleAut32
win32: LIBS += -lOle32

win32: QMAKE_CXXFLAGS_RELEASE -= -Zc:strictStrings
win32: QMAKE_CFLAGS_RELEASE -= -Zc:strictStrings
win32: QMAKE_CFLAGS -= -Zc:strictStrings
win32: QMAKE_CXXFLAGS -= -Zc:strictStrings
