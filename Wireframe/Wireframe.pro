#-------------------------------------------------
#
# Project created by QtCreator 2017-04-18T18:03:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Wireframe
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
        about_view.cpp \
    generatrix_view.cpp \
    bspline.cpp \
    configuration.cpp \
    generatrix_object.cpp \
    canvas3d.cpp \
    axis.cpp \
    line3d.cpp \
    wire_object.cpp \
    transform.cpp \
    settings.cpp \
    wireframe_widget.cpp

HEADERS  += mainwindow.h \
            about_view.h \
    generatrix_view.h \
    bspline.h \
    drawing.h \
    configuration.h \
    generatrix_object.h \
    canvas3d.h \
    axis.h \
    line3d.h \
    wire_object.h \
    transform.h \
    settings.h \
    wireframe_widget.h

RESOURCES += \
    images.qrc
