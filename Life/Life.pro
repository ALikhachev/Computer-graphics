QT += core gui widgets

TARGET = Life

SOURCES += main.cpp\
    mainwindow.cpp \
    board_view.cpp \
    board_settings_view.cpp \
    board_settings.cpp \
    about_view.cpp

HEADERS  += mainwindow.h \
    utils.h \
    board_view.h \
    board_settings_view.h \
    board_settings.h \
    about_view.h

RESOURCES += \
    toolbar.qrc
