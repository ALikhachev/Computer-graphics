#include <QShortcut>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Life");
    setupShortcuts();
    about = new About(this);
    board = new Board(this);
    ui->verticalLayout->addWidget(board);
    board->show();
}

void MainWindow::setupShortcuts() {
    QShortcut *shortcutExit = new QShortcut(QKeySequence("Ctrl+Q"), this);
    QShortcut *shortcutAbout = new QShortcut(QKeySequence("F1"), this);
    QObject::connect(shortcutExit, SIGNAL(activated()), this, SLOT(on_actionExit_triggered()));
    QObject::connect(shortcutAbout, SIGNAL(activated()), this, SLOT(on_actionAbout_triggered()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    qApp->quit();
}

void MainWindow::on_actionAbout_triggered()
{
    about->show();
}
