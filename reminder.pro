#-------------------------------------------------
#
# Project created by QtCreator 2014-08-05T09:53:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = reminder
TEMPLATE = app


SOURCES += main.cpp\
        reminder.cpp \
    timeout.cpp

HEADERS  += reminder.h \
    timeout.h

FORMS    += reminder.ui \
    timeout.ui

RESOURCES += \
    tray.qrc
