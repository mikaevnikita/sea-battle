#-------------------------------------------------
#
# Project created by QtCreator 2017-10-13T22:59:02
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NetworkInterface
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    NetworkInterface.cpp \
    invitewidget.cpp \
    gamebutton.cpp

HEADERS  += mainwindow.h \
    NetworkConstants.h \
    NetworkInterface.h \
    invitewidget.h \
    gamebutton.h

FORMS    += mainwindow.ui \
    invitewidget.ui
