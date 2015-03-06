#-------------------------------------------------
#
# Project created by QtCreator 2015-03-05T14:41:34
#
#-------------------------------------------------

QT       += core gui serialport widgets script

SOURCES  += main.cpp\
		  interface/mainwindow.cpp \
		  interface/settingsdialog.cpp \
    interface/sensorwidget.cpp

HEADERS  += interface/mainwindow.hpp \
		  interface/settingsdialog.hpp \
    interface/sensorwidget.hpp

FORMS    += interface/mainwindow.ui \
		  interface/settingsdialog.ui \
    interface/sensorwidget.ui

CONFIG   += c++11

TARGET   = AVR-Sensors
TEMPLATE = app

QMAKE_CXXFLAGS += -O3
