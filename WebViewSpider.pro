#-------------------------------------------------
#
# Project created by QtCreator 2018-04-13T16:49:27
#
#-------------------------------------------------

QT       += core gui webkitwidgets printsupport

CONFIG += C++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WebViewSpider
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    webview.cpp \
    webpage.cpp \
    networkaccessmanager.cpp \
    cookiejar.cpp \
    cookiehandler.cpp \
    exechandler.cpp \
    extracthandler.cpp \
    loadhandler.cpp \
    screenshothandler.cpp

HEADERS  += mainwindow.h \
    webview.h \
    webpage.h \
    networkaccessmanager.h \
    cookiejar.h \
    cookiehandler.h \
    exechandler.h \
    extracthandler.h \
    loadhandler.h \
    screenshothandler.h

FORMS    += mainwindow.ui

win32{RC_ICONS = app.ico}

unix{ICON = app.icns}
