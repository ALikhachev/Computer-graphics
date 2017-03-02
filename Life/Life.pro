QT += core gui widgets

TARGET = Life

SOURCES += main.cpp\
    mainwindow.cpp \
    board_view.cpp \
    board_settings_view.cpp \
    about_view.cpp \
    board.cpp \
    board_settings.cpp \
    save_view.cpp

HEADERS  += mainwindow.h \
    board_view.h \
    board_settings_view.h \
    board_settings.h \
    about_view.h \
    board.h \
    save_view.h

RESOURCES += \
    toolbar.qrc \
    about.qrc
