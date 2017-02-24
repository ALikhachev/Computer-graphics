QT += core gui widgets

TARGET = Life

SOURCES += main.cpp\
    mainwindow.cpp \
    board.cpp \
    about.cpp \
    boardsettings.cpp \
    boardsettingsdialog.cpp

HEADERS  += mainwindow.h \
    board.h \
    utils.h \
    about.h \
    boardsettings.h \
    boardsettingsdialog.h

RESOURCES += \
    toolbar.qrc
