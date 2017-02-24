#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QScopedPointer>

#include "about.h"
#include "board.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

private slots:
    void on_actionExit_triggered();

    void on_actionAbout_triggered();

private:
    void setupShortcuts();

    QScopedPointer<QScrollArea> scrollArea;

    QScopedPointer<About> about;
    QScopedPointer<Board> board;
};

#endif // MAINWINDOW_H
