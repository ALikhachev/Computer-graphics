#include <QAction>
#include <QMenu>
#include <QMenuBar>
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
    QAction *newAct = fileMenu->addAction(tr("&New"), this, &QWidget::close);
    newAct->setShortcut(tr("Ctrl+N"));
    QAction *openAct = fileMenu->addAction(tr("&Open..."), this, &QWidget::close);
    openAct->setShortcut(tr("Ctrl+O"));
    QAction *saveAct = fileMenu->addAction(tr("&Save"), this, &QWidget::close);
    saveAct->setShortcut(tr("Ctrl+S"));
    QAction *saveAsAct = fileMenu->addAction(tr("&Save as..."), this, &QWidget::close);
    saveAsAct->setShortcut(tr("Ctrl+Shift+S"));
    fileMenu->addSeparator();
    QAction *exitAct = fileMenu->addAction(tr("&Exit..."), this, &QWidget::close);
    exitAct->setShortcut(tr("Ctrl+Q"));

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    QAction *xorModeAct = editMenu->addAction(tr("&XOR"), this, &QWidget::close);
    xorModeAct->setCheckable(true);
    xorModeAct->setChecked(true);
    QAction *replaceModeAct = editMenu->addAction(tr("&Replace"), this, &QWidget::close);
    editMenu->addSeparator();
    replaceModeAct->setCheckable(true);
    QAction *clearBoardAct = editMenu->addAction(tr("&Clear"), this, &QWidget::close);
    clearBoardAct->setShortcut(tr("Ctrl+L"));
    editMenu->addSeparator();
    editMenu->addAction(tr("&Parameters"), this, &QWidget::close);

    QMenu *viewMenu = menuBar()->addMenu(tr("&Edit"));
    viewMenu->addAction(tr("&Toolbar"), this, &QWidget::close);
    viewMenu->addAction(tr("&Statusbar"), this, &QWidget::close);

    QMenu *simulationMenu = menuBar()->addMenu(tr("&Simulation"));
    simulationMenu->addAction(tr("&Run once"), this, &QWidget::close);
    simulationMenu->addAction(tr("&Run loop"), this, &QWidget::close);

    QAction *helpAct = menuBar()->addAction(tr("&?"), this, SLOT(showAbout()));
    helpAct->setShortcut(tr("F1"));
}

void MainWindow::showAbout() {
    about->show();
    about->activateWindow();
}
