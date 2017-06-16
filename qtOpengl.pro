#-------------------------------------------------
#
# Project created by QtCreator 2017-05-15T21:04:15
#
#-------------------------------------------------

QT       += core gui opengl
LIBS += -lGLU -lglut -ldl -lGL -lGLEW

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtOpengl
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp \
    myglwidget.cpp \
    myglwidget_controls_and_camera.cpp \
    myglwidget_general.cpp \
    myglwidget_shapes_drawing.cpp

HEADERS  += \
    myglwidget.h
