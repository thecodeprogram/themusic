#-------------------------------------------------
#
# Project created by QtCreator 2017-07-03T09:44:28
#
#-------------------------------------------------

QT       += core gui sql multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TheMusic
TEMPLATE = app

RC_ICONS += images/programIcon.ico


SOURCES += main.cpp\
        mainwindow.cpp \
    dbmanager.cpp \
    mythread.cpp \
    dialogaboutme.cpp \


HEADERS  += mainwindow.h \
    dbmanager.h / \
    mediaaxwidget.h.h \
    mythread.h \
    dialogaboutme.h \



FORMS    += mainwindow.ui \
    dialogaboutme.ui

RESOURCES += \
    resimler.qrc




