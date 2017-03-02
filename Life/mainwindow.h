#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QMainWindow>
#include <QScrollArea>
#include <QScopedPointer>
#include <QToolBar>

#include "board.h"
#include "board_settings.h"
#include "about_view.h"
#include "board_view.h"
#include "board_settings_view.h"

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
    void toggleLoopMode();
    void toggleShowImpacts();
    void setXorMode();
    void setReplaceMode();
    void modelSettingsChanged();
    void viewSettingsChanged();
    void newGame();
    void loadGame();
    void saveGame();

private:
    void createActions();

    QScrollArea *scrollArea;

    BoardSettings settings;
    Board board;

    BoardView *board_view;

    QToolBar *toolbar;
    QAction *toolBarStateAction;
    QAction *statusBarStateAction;
    QAction *clear_board_action;
    QAction *run_once_action;
    QAction *run_loop_action;
    QAction *xor_mode_action;
    QAction *replace_mode_action;
    QAction *show_impacts_action;

    QTimer *loop_timer;
};

#endif // MAINWINDOW_H
