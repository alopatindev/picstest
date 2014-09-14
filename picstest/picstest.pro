#-------------------------------------------------
#
# Project created by QtCreator 2014-09-14T21:48:16
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = picstest
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    PictureWidget.cpp

HEADERS  += MainWindow.h \
    PictureWidget.h

FORMS    += MainWindow.ui

# CONFIG += c++11

QMAKE_CXXFLAGS += -std=c++11
