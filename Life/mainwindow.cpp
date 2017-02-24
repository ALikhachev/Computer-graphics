#include <QMenu>
#include <QMenuBar>
#include <QShortcut>
#include <QStatusBar>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    scrollArea(this),
    about(this),
    board(this),
    settings(NULL, this)
{
    setWindowTitle("Life");

    createActions();

    scrollArea.setWidget(&board);

    setCentralWidget(&scrollArea);
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

    QAction *clearBoardAct = editMenu->addAction(tr("&Clear"), this, &QWidget::close);
    clearBoardAct->setShortcut(tr("Ctrl+L"));
    const QIcon clearBoardIcon = QIcon::fromTheme("clear-board", QIcon(":/icons/clear.png"));
    clearBoardAct->setIcon(clearBoardIcon);
    clearBoardAct->setStatusTip("Clear the game board");
    toolBar->addAction(clearBoardAct);

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

    QAction *runOnceAct = simulationMenu->addAction(tr("&Run once"), this, &QWidget::close);
    const QIcon runOnceIcon = QIcon::fromTheme("run-once", QIcon(":/icons/run-once.png"));
    runOnceAct->setIcon(runOnceIcon);
    runOnceAct->setStatusTip("Run game loop once");
    toolBar->addAction(runOnceAct);

    QAction *runLoopAct = simulationMenu->addAction(tr("&Run loop"), this, &QWidget::close);
    const QIcon runLoopIcon = QIcon::fromTheme("run", QIcon(":/icons/run.png"));
    runLoopAct->setIcon(runLoopIcon);
    runLoopAct->setStatusTip("Run game loop");
    toolBar->addAction(runLoopAct);

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
    settings.show();
    settings.activateWindow();
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
