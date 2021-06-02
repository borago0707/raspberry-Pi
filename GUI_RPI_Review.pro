QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
TARGET = GUI_RPI_REVIEW
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    color_sensor.cpp \
    dcmotor.cpp \
    distance_sensor.cpp \
    encoder.cpp \
    linesensor.cpp \
    main.cpp \
    mainwindow.cpp \
    mobileplatform.cpp

HEADERS += \
    color_sensor.h \
    dcmotor.h \
    distance_sensor.h \
    encoder.h \
    linesensor.h \
    mainwindow.h \
    mobileplatform.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += /opt/qt5pi/sysroot/usr/include
LIBS +=  -L/opt/qt5pi/sysroot/usr/lib -lwiringPi -lpthread

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /home/pi/$${TARGET}
!isEmpty(target.path): INSTALLS += target

DESTDIR = ./bin
OBJECTS_DIR = ./temp
MOC_DIR = ./temp
RCC_DIR = ./temp
UI_DIR = ./temp
