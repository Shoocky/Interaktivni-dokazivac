#-------------------------------------------------
#
# Project created by QtCreator 2015-07-18T13:26:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Proba
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    treescene.cpp \
    node.cpp \
    parser.cpp \
    lexer.cpp

HEADERS  += mainwindow.h \
    treescene.h \
    node.h \
    parser.hpp \
    fol.hpp

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=c++11
