#-------------------------------------------------
#
# Project created by QtCreator 2016-01-31T23:24:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXXFLAGS += -std=c++11

TARGET = Training
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    createexercisedialog.cpp \
    exercisewidget.cpp

HEADERS  += mainwindow.h \
    createexercisedialog.h \
    exercisewidget.h \
    struct.h
