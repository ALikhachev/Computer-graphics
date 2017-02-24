#include <QShortcut>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    scrollArea(new QScrollArea),
    about(new About(this)),
    board(new Board(this))
{
    setWindowTitle("Life");

    setupShortcuts();

    scrollArea->setWidget(board.data());
    scrollArea->setVisible(true);

    setCentralWidget(scrollArea.data());
}

void MainWindow::setupShortcuts() {
    QShortcut *shortcutExit = new QShortcut(QKeySequence("Ctrl+Q"), this);
    QShortcut *shortcutAbout = new QShortcut(QKeySequence("F1"), this);
    QObject::connect(shortcutExit, SIGNAL(activated()), this, SLOT(on_actionExit_triggered()));
    QObject::connect(shortcutAbout, SIGNAL(activated()), this, SLOT(on_actionAbout_triggered()));
}

void MainWindow::on_actionExit_triggered()
{
    qApp->quit();
}

void MainWindow::on_actionAbout_triggered()
{
    about->show();
}
