#-------------------------------------------------
#
# Project created by QtCreator 2018-02-16T16:15:13
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DS
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

FORMS    += widget.ui

win32:RC_ICONS += res/doom-mp.ico
