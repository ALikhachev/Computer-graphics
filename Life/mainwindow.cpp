#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QShortcut>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    scrollArea(new QScrollArea),
    about(new About(this)),
    board(new Board(this))
{
    setWindowTitle("Life");

    createActions();

    scrollArea->setWidget(board.data());

    setCentralWidget(scrollArea.data());
}

void MainWindow::createActions() {
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QToolBar *toolbar = addToolBar(tr("File"));

    QAction *newAct = fileMenu->addAction(tr("&New"), this, &QWidget::close);
    newAct->setShortcut(tr("Ctrl+N"));
    const QIcon newIcon = QIcon::fromTheme("board-new", QIcon(":/icons/new.png"));
    newAct->setIcon(newIcon);
    toolbar->addAction(newAct);

    QAction *openAct = fileMenu->addAction(tr("&Open..."), this, &QWidget::close);
    openAct->setShortcut(tr("Ctrl+O"));
    const QIcon openIcon = QIcon::fromTheme("board-open", QIcon(":/icons/open.png"));
    openAct->setIcon(openIcon);
    toolbar->addAction(openAct);

    QAction *saveAct = fileMenu->addAction(tr("&Save"), this, &QWidget::close);
    saveAct->setShortcut(tr("Ctrl+S"));
    const QIcon saveIcon = QIcon::fromTheme("board-save", QIcon(":/icons/save.png"));
    saveAct->setIcon(saveIcon);
    toolbar->addAction(saveAct);

    QAction *saveAsAct = fileMenu->addAction(tr("&Save as..."), this, &QWidget::close);
    saveAsAct->setShortcut(tr("Ctrl+Shift+S"));

    fileMenu->addSeparator();

    QAction *exitAct = fileMenu->addAction(tr("&Exit..."), this, &QWidget::close);
    exitAct->setShortcut(tr("Ctrl+Q"));

    toolbar->addSeparator();

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));

    QAction *xorModeAct = editMenu->addAction(tr("&XOR mode"), this, &QWidget::close);
    xorModeAct->setCheckable(true);
    xorModeAct->setChecked(true);

    QAction *replaceModeAct = editMenu->addAction(tr("&Replace mode"), this, &QWidget::close);
    replaceModeAct->setCheckable(true);
    const QIcon replaceIcon = QIcon::fromTheme("replace-mode-switch", QIcon(":/icons/replace.png"));
    replaceModeAct->setIcon(replaceIcon);
    toolbar->addAction(replaceModeAct);

    editMenu->addSeparator();
    toolbar->addSeparator();

    QAction *clearBoardAct = editMenu->addAction(tr("&Clear"), this, &QWidget::close);
    clearBoardAct->setShortcut(tr("Ctrl+L"));
    const QIcon clearBoardIcon = QIcon::fromTheme("clear-board", QIcon(":/icons/clear.png"));
    clearBoardAct->setIcon(clearBoardIcon);
    toolbar->addAction(clearBoardAct);

    editMenu->addSeparator();

    QAction *parametersAct = editMenu->addAction(tr("&Parameters"), this, &QWidget::close);
    const QIcon settingsIcon = QIcon::fromTheme("settings", QIcon(":/icons/settings.png"));
    parametersAct->setIcon(settingsIcon);
    toolbar->addAction(parametersAct);

    toolbar->addSeparator();

    QMenu *viewMenu = menuBar()->addMenu(tr("&Edit"));
    viewMenu->addAction(tr("&Toolbar"), this, &QWidget::close);
    viewMenu->addAction(tr("&Statusbar"), this, &QWidget::close);

    QMenu *simulationMenu = menuBar()->addMenu(tr("&Simulation"));

    QAction *runOnceAct = simulationMenu->addAction(tr("&Run once"), this, &QWidget::close);
    const QIcon runOnceIcon = QIcon::fromTheme("run-once", QIcon(":/icons/run-once.png"));
    runOnceAct->setIcon(runOnceIcon);
    toolbar->addAction(runOnceAct);

    QAction *runLoopAct = simulationMenu->addAction(tr("&Run loop"), this, &QWidget::close);
    const QIcon runLoopIcon = QIcon::fromTheme("run", QIcon(":/icons/run.png"));
    runLoopAct->setIcon(runLoopIcon);
    toolbar->addAction(runLoopAct);

    QAction *helpAct = menuBar()->addAction(tr("&?"), this, SLOT(showAbout()));
    helpAct->setShortcut(tr("F1"));
    const QIcon helpIcon = QIcon::fromTheme("about", QIcon(":/icons/about.png"));
    helpAct->setIcon(helpIcon);
}

void MainWindow::showAbout() {
    about->show();
    about->activateWindow();
}
