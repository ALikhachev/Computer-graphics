#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>

#include "function_viewer.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void setupActions();

    QAction *toolbar_switch;
    QAction *statusbar_switch;

    FunctionViewer *function_viewer;
};

#endif // MAINWINDOW_H
