QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Server
TEMPLATE = app


SOURCES += main.cpp \
    server.cpp \
    authmanager.cpp \
    sessionsmanager.cpp \
    session.cpp

HEADERS  += \
    server.h \
    NetworkConstants.h \
    authmanager.h \
    sessionsmanager.h \
    session.h

FORMS    += widget.ui
