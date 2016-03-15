#-------------------------------------------------
#
# Project created by QtCreator 2016-03-12T20:15:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = aioi4
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Utility/opencv_qt.cpp

HEADERS  += mainwindow.h \
    Utility/opencv_qt.h

FORMS    += mainwindow.ui

LIBS += `pkg-config opencv --libs`

RESOURCES += \
    resource.qrc
