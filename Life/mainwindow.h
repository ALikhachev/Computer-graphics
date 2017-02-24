#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QMainWindow>
#include <QScrollArea>
#include <QScopedPointer>
#include <QToolBar>

#include "about.h"
#include "board.h"
#include "boardsettingsdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

private slots:
    void showAbout();
    void showSettings();
    void toggleStatusBar();
    void toggleToolBar();

private:
    void createActions();

    QScrollArea scrollArea;

    About about;
    Board board;
    BoardSettingsDialog settings;

    QToolBar *toolBar;
    QAction *toolBarStateAction;
    QAction *statusBarStateAction;
};

#endif // MAINWINDOW_H
