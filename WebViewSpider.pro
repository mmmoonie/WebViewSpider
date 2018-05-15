#-------------------------------------------------
#
# Project created by QtCreator 2018-04-13T16:49:27
#
#-------------------------------------------------

QT       += core gui webkitwidgets printsupport network

CONFIG += C++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WebViewSpider
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    webview.cpp \
    webpage.cpp \
    networkaccessmanager.cpp \
    cookiejar.cpp

HEADERS  += mainwindow.h \
    webview.h \
    webpage.h \
    networkaccessmanager.h \
    cookiejar.h

FORMS    += mainwindow.ui


#QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
#QMAKE_MAC_SDK = macosx10.9
