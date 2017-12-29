QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sea-battle
TEMPLATE = app

SOURCES += main.cpp\
    GUI/menuwindow.cpp \
    GUI/networkconnectorwidget.cpp \
    GUI/networkgamemainwindow.cpp \
    GUI/testwindow.cpp \
    GUI/invitewidget.cpp \
    GUI/gamebutton.cpp \
    NetworkInterface/NetworkInterface.cpp \

HEADERS  += \
    GUI/gui_constants.h \
    GUI/menuwindow.h \
    GUI/networkconnectorwidget.h \
    GUI/networkgamemainwindow.h \
    GUI/testwindow.h \
    GUI/invitewidget.h \
    GUI/gamebutton.h \
    NetworkInterface/networkconstants.h \
    NetworkInterface/NetworkInterface.h \

FORMS    += \
    GUI/menuwindow.ui \
    GUI/networkgamemainwindow.ui \
    GUI/networkconnectorwidget.ui \
    GUI/testwindow.ui \
    GUI/invitewidget.ui


