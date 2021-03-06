#-------------------------------------------------
#
# Project created by QtCreator 2017-03-22T13:13:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Filters
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
    filter_zone.cpp \
    main_window.cpp \
    zone_container.cpp \
    filter.cpp \
    filter_registry.cpp \
    filters/scale_filter.cpp \
    filters/grayscale_filter.cpp \
    filters/negative_filter.cpp \
    filters/blur_filter.cpp \
    filter_widget.cpp \
    widgets/scale_filter_wdget.cpp \
    qsliderbox.cpp \
    filters/convolution_filter.cpp \
    widgets/blur_filter_widget.cpp \
    filters/gamma_correction_filter.cpp \
    widgets/gamma_correction_filter_widget.cpp \
    filters/sharpness_filter.cpp \
    filters/embossing_filter.cpp \
    filters/watercolor_filter.cpp \
    widgets/watercolor_filter_widget.cpp \
    filters/sobel_filter.cpp \
    widgets/sobel_filter_widget.cpp \
    filters/roberts_filter.cpp \
    widgets/roberts_filter_widget.cpp \
    filters/edge_filter.cpp \
    widgets/edge_filter_widget.cpp \
    filters/ordered_dithering_filter.cpp \
    filters/floyd_steinberg_filter.cpp \
    widgets/ordered_dithering_filter_widget.cpp \
    widgets/floyd_steinberg_filter_widget.cpp \
    filters/rotate_filter.cpp \
    widgets/rotate_filter_widget.cpp \
    about_view.cpp \

HEADERS  += \
    filter_zone.h \
    main_window.h \
    zone_container.h \
    filter.h \
    utils.h \
    filter_registry.h \
    filters/scale_filter.h \
    filters/grayscale_filter.h \
    filters/negative_filter.h \
    filters/blur_filter.h \
    filter_widget.h \
    widgets/scale_filter_widget.h \
    qsliderbox.h \
    filters/convolution_filter.h \
    widgets/blur_filter_widget.h \
    filters/gamma_correction_filter.h \
    widgets/gamma_correction_filter_widget.h \
    filters/sharpness_filter.h \
    filters/embossing_filter.h \
    filters/watercolor_filter.h \
    widgets/watercolor_filter_widget.h \
    filters/sobel_filter.h \
    widgets/sobel_filter_widget.h \
    filters/roberts_filter.h \
    widgets/roberts_filter_widget.h \
    filters/edge_filter.h \
    widgets/edge_filter_widget.h \
    filters/ordered_dithering_filter.h \
    filters/floyd_steinberg_filter.h \
    widgets/ordered_dithering_filter_widget.h \
    widgets/floyd_steinberg_filter_widget.h \
    filters/rotate_filter.h \
    widgets/rotate_filter_widget.h \
    about_view.h

RESOURCES += \
    icons.qrc \
    about.qrc

DISTFILES +=
