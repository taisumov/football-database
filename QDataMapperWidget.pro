#-------------------------------------------------
#
# Project created by QtCreator 2015-08-16T23:58:29
#
#-------------------------------------------------

QT       += core
QT       += gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QDataMapperWidget
TEMPLATE = app


SOURCES += main.cpp\
    dialogaddmatch.cpp \
        mainwindow.cpp \
    database.cpp \
    dialogadddevice.cpp

HEADERS  += mainwindow.h \
    database.h \
    dialogadddevice.h \
    dialogaddmatch.h

FORMS    += mainwindow.ui \
    dialogadddevice.ui \
    dialogaddmatch.ui
