#-------------------------------------------------
#
# Project created by QtCreator 2013-05-21T11:10:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = c-voku
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cell.cpp \
    grid.cpp

HEADERS  += mainwindow.h \
    cell.h \
    grid.h

FORMS    += mainwindow.ui
