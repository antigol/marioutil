QT       += core
QT       -= gui

TARGET = marioutil
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    pointmap.cpp

HEADERS += \
    pointmap.h

LIBS += -lgsl -lgslcblas
