QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app


INCLUDEPATH += ../License3C
SOURCES +=  \
    test_testlicense3c.cpp


SOURCES += \
    ../License3C/license3c.cpp

HEADERS += \
    ../License3C/License3C_global.h \
    ../License3C/license3c.h



INCLUDEPATH += $$PWD/../License3C/qt-secret/

win32:CONFIG (release, debug|release): LIBS += -L$$PWD/../License3C/libs/x64 -lQt-Secret -lQtBigInt
else:win32:CONFIG (debug, debug|release): LIBS += -L$$PWD/../License3C/libs/x64 -lQt-Secret -lQtBigInt
else:unix: LIBS += -L$$PWD/../License3C/libs/x64 -lQt-Secret -lQtBigInt
