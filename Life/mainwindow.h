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
    void setXorMode();
    void setReplaceMode();

private:
    void createActions();

    QScrollArea scrollArea;

    BoardSettings settings;
    Board board;

    AboutView about;
    BoardView board_view;
    BoardSettingsView settings_view;

    QToolBar *toolBar;
    QAction *toolBarStateAction;
    QAction *statusBarStateAction;
    QAction *clear_board_action;
    QAction *run_once_action;
    QAction *run_loop_action;
    QAction *xor_mode_action;
    QAction *replace_mode_action;

    QTimer loop_timer;
};

#endif // MAINWINDOW_H
