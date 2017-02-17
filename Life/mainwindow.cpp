#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    wgt = new QCanvas(this);
    ui->verticalLayout->addWidget(wgt);
    wgt->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    qApp->quit();
}

//void MainWindow::paintEvent(QPaintEvent *event) {
//    QPainter painter(this);
//    QImage image("E:/green_nsu.png");
//    painter.drawImage(0, 0, image);
//}
