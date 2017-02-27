#include <QMenu>
#include <QMenuBar>
#include <QShortcut>
#include <QStatusBar>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    scrollArea(this),
    board(&settings, settings.width, settings.height, std::vector<Cell>(settings.width * settings.height)),
    about(this),
    board_view(&board, this),
    settings_view(NULL, this),
    loop_timer(this)
{
    setWindowTitle("Life");

    createActions();

    scrollArea.setBackgroundRole(QPalette::Light);

    scrollArea.setWidget(&board_view);

    setCentralWidget(&scrollArea);

    connect(&loop_timer, SIGNAL(timeout()), &board, SLOT(tick()));
}

void MainWindow::createActions() {
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    toolBar = addToolBar(tr("File"));

    QAction *newAct = fileMenu->addAction(tr("&New"), this, &QWidget::close);
    newAct->setShortcut(tr("Ctrl+N"));
    const QIcon newIcon = QIcon::fromTheme("board-new", QIcon(":/icons/new.png"));
    newAct->setIcon(newIcon);
    newAct->setStatusTip("Create a new Convey's Life game board");
    toolBar->addAction(newAct);

    QAction *openAct = fileMenu->addAction(tr("&Open..."), this, &QWidget::close);
    openAct->setShortcut(tr("Ctrl+O"));
    const QIcon openIcon = QIcon::fromTheme("board-open", QIcon(":/icons/open.png"));
    openAct->setIcon(openIcon);
    openAct->setStatusTip("Load Convey's Life game board from file");
    toolBar->addAction(openAct);

    QAction *saveAct = fileMenu->addAction(tr("&Save"), this, &QWidget::close);
    saveAct->setShortcut(tr("Ctrl+S"));
    const QIcon saveIcon = QIcon::fromTheme("board-save", QIcon(":/icons/save.png"));
    saveAct->setIcon(saveIcon);
    saveAct->setStatusTip("Save current game board to the file");
    toolBar->addAction(saveAct);

    QAction *saveAsAct = fileMenu->addAction(tr("&Save as..."), this, &QWidget::close);
    saveAsAct->setStatusTip("Save current game board to specific file");
    saveAsAct->setShortcut(tr("Ctrl+Shift+S"));

    fileMenu->addSeparator();

    QAction *exitAct = fileMenu->addAction(tr("&Exit..."), this, &QWidget::close);
    saveAsAct->setStatusTip("Exit the game");
    exitAct->setShortcut(tr("Ctrl+Q"));

    toolBar->addSeparator();

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));

    QAction *xorModeAct = editMenu->addAction(tr("&XOR mode"), this, &QWidget::close);
    xorModeAct->setCheckable(true);
    xorModeAct->setChecked(true);
    xorModeAct->setStatusTip("Switch XOR/Replace mode");

    QAction *replaceModeAct = editMenu->addAction(tr("&Replace mode"), this, &QWidget::close);
    replaceModeAct->setCheckable(true);
    const QIcon replaceIcon = QIcon::fromTheme("replace-mode-switch", QIcon(":/icons/replace.png"));
    replaceModeAct->setIcon(replaceIcon);
    replaceModeAct->setStatusTip("Switch XOR/Replace mode");
    toolBar->addAction(replaceModeAct);

    editMenu->addSeparator();
    toolBar->addSeparator();

    clear_board_action = editMenu->addAction(tr("&Clear"), &board, &Board::clear);
    clear_board_action->setShortcut(tr("Ctrl+L"));
    const QIcon clear_board_icon = QIcon::fromTheme("clear-board", QIcon(":/icons/clear.png"));
    clear_board_action->setIcon(clear_board_icon);
    clear_board_action->setStatusTip("Clear the game board");
    toolBar->addAction(clear_board_action);

    editMenu->addSeparator();

    QAction *parametersAct = editMenu->addAction(tr("&Parameters"), this, showSettings);
    const QIcon settingsIcon = QIcon::fromTheme("settings", QIcon(":/icons/settings.png"));
    parametersAct->setIcon(settingsIcon);
    parametersAct->setStatusTip("Show game settings window");
    toolBar->addAction(parametersAct);

    toolBar->addSeparator();

    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
    toolBarStateAction = viewMenu->addAction(tr("&Toolbar"), this, toggleToolBar);
    toolBarStateAction->setCheckable(true);
    toolBarStateAction->setChecked(true);
    toolBarStateAction->setStatusTip("Show/hide toolbar");
    statusBarStateAction = viewMenu->addAction(tr("&Statusbar"), this, toggleStatusBar);
    statusBarStateAction->setCheckable(true);
    statusBarStateAction->setChecked(true);
    statusBarStateAction->setStatusTip("Show/hide statusbar");
    statusBar()->show();

    QMenu *simulationMenu = menuBar()->addMenu(tr("&Simulation"));

    run_once_action = simulationMenu->addAction(tr("&Run once"), &board, &Board::tick);
    const QIcon run_once_icon = QIcon::fromTheme("run-once", QIcon(":/icons/run-once.png"));
    run_once_action->setIcon(run_once_icon);
    run_once_action->setStatusTip("Run game loop once");
    toolBar->addAction(run_once_action);

    run_loop_action = simulationMenu->addAction(tr("&Run loop"), this, toggleLoopMode);
    const QIcon run_loop_icon = QIcon::fromTheme("run", QIcon(":/icons/run.png"));
    run_loop_action->setIcon(run_loop_icon);
    run_loop_action->setCheckable(true);
    run_loop_action->setStatusTip("Run game loop");
    toolBar->addAction(run_loop_action);

    QAction *helpAct = menuBar()->addAction(tr("&?"), this, SLOT(showAbout()));
    helpAct->setShortcut(tr("F1"));
    const QIcon helpIcon = QIcon::fromTheme("about", QIcon(":/icons/about.png"));
    helpAct->setStatusTip("About this application");
    helpAct->setIcon(helpIcon);
}

void MainWindow::showAbout() {
    about.show();
    about.activateWindow();
}

void MainWindow::showSettings() {
    settings_view.show();
    settings_view.activateWindow();
}

void MainWindow::toggleStatusBar() {
    if (statusBar()->isHidden()) {
        statusBar()->show();
        statusBarStateAction->setChecked(true);
    } else {
        statusBar()->hide();
        statusBarStateAction->setChecked(false);
    }
}

void MainWindow::toggleToolBar() {
    if (toolBar->isHidden()) {
        toolBar->show();
        toolBarStateAction->setChecked(true);
    } else {
        toolBar->hide();
        toolBarStateAction->setChecked(false);
    }
}

void MainWindow::toggleLoopMode() {
    if (loop_timer.isActive()) {
        run_loop_action->setChecked(false);
        run_once_action->setDisabled(false);
        clear_board_action->setDisabled(false);
        loop_timer.stop();
        board_view.toggleEditing(true);
    } else {
        run_loop_action->setChecked(true);
        run_once_action->setDisabled(true);
        clear_board_action->setDisabled(true);
        loop_timer.start(1000);
        board_view.toggleEditing(false);
    }
}
