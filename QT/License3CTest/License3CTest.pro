QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app


INCLUDEPATH += ../License3C
SOURCES +=  \
    tst_testlicense3c.cpp


SOURCES += \
    ../License3C/license3c.cpp

HEADERS += \
    ../License3C/License3C_global.h \
    ../License3C/license3c.h
