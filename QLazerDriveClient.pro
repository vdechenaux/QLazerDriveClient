#-------------------------------------------------
#
# Project created by QtCreator 2015-12-14T20:57:59
#
#-------------------------------------------------

QT       += websockets

QT       -= gui

TARGET = QLazerDriveClient
TEMPLATE = lib

DEFINES += QLAZERDRIVECLIENT_LIBRARY

SOURCES += \
    qlazerdriveclient.cpp \
    qlazerdrivepacket.cpp \
    qlazerdriveplayer.cpp \
    qlazerdrivebonus.cpp

HEADERS += qlazerdriveclient_global.h \
    qlazerdriveclient.h \
    qlazerdrivepacket.h \
    qlazerdriveplayer.h \
    qlazerdrivebonus.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
