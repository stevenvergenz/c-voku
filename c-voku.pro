#-------------------------------------------------
#
# Project created by QtCreator 2013-05-21T11:10:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = c-voku
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11 -O3


SOURCES += main.cpp\
        mainwindow.cpp \
    cell.cpp \
    grid.cpp \
    gridmodel.cpp \
    puzzleview.cpp \
    exception.cpp \
    metatableitemdelegate.cpp \
    logger.cpp \
    histframe.cpp

HEADERS  += mainwindow.h \
    cell.h \
    grid.h \
    gridmodel.h \
    puzzleview.h \
    exception.h \
    metatableitemdelegate.h \
    logger.h \
    histframe.h

FORMS    += mainwindow.ui
