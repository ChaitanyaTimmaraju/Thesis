QT += core gui

CONFIG += c++11

TARGET = AmbientOcclusion
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    window.cpp \
    transform3d.cpp \
    input.cpp \
    camera3d.cpp

HEADERS += \
    window.h \
    vertex.h \
    transform3d.h \
    input.h \
    camera3d.h

RESOURCES += \
    resources.qrc

DISTFILES += \
    shaders/simple.frag \
    shaders/simple.vert
