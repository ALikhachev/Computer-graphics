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
    wgt = new Board(this);
    ui->verticalLayout->addWidget(wgt);
    wgt->show();
}

void MainWindow::setupShortcuts() {
    QShortcut *shortcut = new QShortcut(QKeySequence("Ctrl+Q"), this);
    QObject::connect(shortcut, SIGNAL(activated()), this, SLOT(on_actionExit_triggered()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    qApp->quit();
}
