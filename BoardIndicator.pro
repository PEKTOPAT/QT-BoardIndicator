#-------------------------------------------------
#
# Project created by QtCreator 2020-06-04T13:32:16
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BoardIndicator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mytcpclient.cpp \
    ledindicator.cpp \
    boardindiatorobjects.cpp

HEADERS  += mainwindow.h \
    mytcpclient.h \
    ledindicator.h \
    boardindiatorobjects.h

FORMS    += mainwindow.ui
