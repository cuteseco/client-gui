#-------------------------------------------------
#
# Project created by QtCreator 2015-05-31T17:19:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CuteSeCo
TEMPLATE = app

DEFINES += PROJECT_PROGNAME="'\"CuteSeCo\"'"


SOURCES += main.cpp\
        cuteseco.cpp \
    about/aboutdialog.cpp \
    logger/logger.cpp \
    logger/loggertypedelegate.cpp

HEADERS  += cuteseco.h \
    about/aboutdialog.h \
    logger/logger.h \
    logger/logger_include.h \
    logger/loggertypedelegate.h

FORMS    += cuteseco.ui \
    about/aboutdialog.ui

RESOURCES += \
    resource.qrc
