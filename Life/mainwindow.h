#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "qcanvas.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

//protected:
//    void paintEvent(QPaintEvent *);

private slots:
    void on_actionExit_triggered();

private:
    Ui::MainWindow *ui;

    QCanvas *wgt;
};

#endif // MAINWINDOW_H
