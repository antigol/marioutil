QT       += core
QT       -= gui

TARGET = marioutil
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    pointmap.cpp \
    gslfunction.cpp \
    cmdlineparser.cpp

HEADERS += \
    pointmap.h \
    configfile.h \
    gslfunction.h \
    cmdlineparser.h

LIBS += -lgsl -lgslcblas
