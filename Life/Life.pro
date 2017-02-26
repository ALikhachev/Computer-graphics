QT += core gui widgets

TARGET = Life

SOURCES += main.cpp\
    mainwindow.cpp \
    board_view.cpp \
    board_settings_view.cpp \
    about_view.cpp \
    board.cpp

HEADERS  += mainwindow.h \
    board_view.h \
    board_settings_view.h \
    board_settings.h \
    about_view.h \
    board.h

RESOURCES += \
    toolbar.qrc
