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
    modelloader.cpp \
    shaderhandler.cpp \
    firstpass.cpp \
    debughelperpass.cpp \
    secondpass.cpp

HEADERS += \
    window.h \
    vertex.h \
    transform3d.h \
    input.h \
    modelloader.h \
    shaderhandler.h \
    passinterface.h \
    firstpass.h \
    debughelperpass.h \
    secondpass.h

RESOURCES += \
    resources.qrc

DISTFILES += \
    models/cowVNT.obj \
    shaders/simple.geom \
    shaders/normalshader.vert \
    shaders/normalshader.frag \
    shaders/FirstPass.vert \
    shaders/FirstPass.frag \
    shaders/secondpass.vert \
    shaders/secondpass.frag \
    shaders/firstpass.geom
