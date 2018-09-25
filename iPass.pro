#-------------------------------------------------
#
# Project created by QtCreator 2018-04-07T17:30:37
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = iPass
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_MESSAGELOGCONTEXT

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    dialog/passwordofapp.cpp \
    dialog/lockofapp.cpp \
    dialog/changepasswordofapp.cpp \
    dialog/addnewaccount.cpp \
    dialog/settingsofapp.cpp \
    dialog/generatepassword.cpp \
    dialog/userguide.cpp \
    dialog/contact.cpp \
    dialog/stacklayout.cpp \
    global/global.cpp \
    other/myeventfilter.cpp \
    dialog/searchofapp.cpp \
    other/mylistwidget.cpp \
    other/simplecrypt.cpp

HEADERS += \
        mainwindow.h \
    dialog/passwordofapp.h \
    dialog/lockofapp.h \
    dialog/changepasswordofapp.h \
    dialog/addnewaccount.h \
    dialog/settingsofapp.h \
    dialog/generatepassword.h \
    dialog/userguide.h \
    dialog/contact.h \
    dialog/stacklayout.h \
    global/global.h \
    other/myeventfilter.h \
    dialog/searchofapp.h \
    other/mylistwidget.h \
    other/simplecrypt.h

CONFIG += mobility

RESOURCES += \
    image.qrc

FORMS += \
    dialog/passwordofapp.ui \
    dialog/lockofapp.ui \
    dialog/changepasswordofapp.ui \
    dialog/addnewaccount.ui \
    dialog/settingsofapp.ui \
    dialog/generatepassword.ui \
    dialog/userguide.ui \
    dialog/contact.ui \
    dialog/stacklayout.ui

RC_ICONS = Password.ico
