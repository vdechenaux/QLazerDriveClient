#-------------------------------------------------
#
# Project created by QtCreator 2015-12-14T20:57:59
#
#-------------------------------------------------

QT       -= gui

TARGET = QLazerDriveClient
TEMPLATE = lib

DEFINES += QLAZERDRIVECLIENT_LIBRARY

SOURCES += qlazerdriveclient.cpp

HEADERS += qlazerdriveclient.h\
        qlazerdriveclient_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
