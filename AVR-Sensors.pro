#-------------------------------------------------
#
# Project created by QtCreator 2015-03-05T14:41:34
#
#-------------------------------------------------

TARGET	=	AVR-Sensors
TEMPLATE	=	app

QT		+=	core gui serialport widgets script sql

SOURCES	+=	main.cpp\
			widgets/sensorwidget.cpp \
			widgets/eventwidget.cpp \
			widgets/devicewidget.cpp \
			windows/sensordialog.cpp \
			windows/eventdialog.cpp \
			windows/devicedialog.cpp \
			windows/mainwindow.cpp

HEADERS	+=	widgets/sensorwidget.hpp \
			widgets/eventwidget.hpp \
			widgets/devicewidget.hpp \
			windows/sensordialog.hpp \
			windows/eventdialog.hpp \
			windows/devicedialog.hpp \
			windows/mainwindow.hpp

FORMS	+=	interface/mainwindow.ui \
			interface/sensordialog.ui \
			interface/sensorwidget.ui \
			interface/eventwidget.ui \
			interface/devicewidget.ui \
			interface/eventdialog.ui \
			interface/devicedialog.ui

CONFIG	+=	c++11

RESOURCES	+=	common.qrc

QMAKE_CXXFLAGS	+=	-s -fomit-frame-pointer -march=native -m3dnow
