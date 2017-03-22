#include <QToolBar>
#include <QScrollArea>
#include <QStatusBar>
#include <QMenuBar>

#include "main_window.h"
#include "zone_container.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(1280, 720);
    this->setMinimumSize(200, 200);
    QScrollArea *scroll_area = new QScrollArea(this);
    scroll_area->setWidget(new ZoneContainer(scroll_area));
    this->setCentralWidget(scroll_area);
    this->setupActions();
}

void MainWindow::setupActions() {
    this->statusBar()->show();
    QToolBar *toolbar = this->addToolBar(tr("Toolbar"));

    QMenu *file_menu = this->menuBar()->addMenu(tr("&File"));

    QAction *new_act = file_menu->addAction(tr("&New"), this, close);
    new_act->setShortcut(tr("Ctrl+N"));
    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/icons/new.png"));
    new_act->setIcon(newIcon);
    new_act->setStatusTip("Clear working area");
    toolbar->addAction(new_act);

    file_menu->addSeparator();
    QAction *exit_Act = file_menu->addAction(tr("&Quit..."), this, close);
    exit_Act->setStatusTip("Quit application");
    exit_Act->setShortcut(tr("Ctrl+Q"));
}
