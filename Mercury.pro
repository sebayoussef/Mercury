#-------------------------------------------------
#
# Project created by QtCreator 2016-10-16T11:57:25
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += modules/include
TARGET = Mercury
TEMPLATE = app

PRECOMPILED_HEADER = modules/include/stdfax.h

SOURCES += main.cpp\
        mainwindow.cpp \
    modules/sources/app.cpp \
    modules/sources/client.cpp \
    modules/sources/client_main.cpp \
    modules/sources/connection_manager.cpp \
    modules/sources/file.cpp \
    modules/sources/heartbeat.cpp \
    modules/sources/job.cpp \
    modules/sources/logger.cpp \
    modules/sources/message.cpp \
    modules/sources/peer.cpp \
    modules/sources/server.cpp \
    modules/sources/server_main.cpp \
    modules/sources/socket_exception.cpp \
    modules/sources/thread.cpp \
    modules/sources/udp_socket.cpp \
    modules/sources/decoder.cpp

HEADERS  += mainwindow.h \
    modules/include/stdfax.h \
    modules/include/client.hpp \
    modules/include/connection_manager.hpp \
    modules/include/decoder.hpp \
    modules/include/file.hpp \
    modules/include/heartbeat.hpp \
    modules/include/job.hpp \
    modules/include/logger.hpp \
    modules/include/message.hpp \
    modules/include/peer.hpp \
    modules/include/server.hpp \
    modules/include/socket_exception.hpp \
    modules/include/thread.hpp \
    modules/include/udp_socket.hpp

FORMS    += mainwindow.ui
