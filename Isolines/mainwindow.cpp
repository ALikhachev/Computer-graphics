#include "mainwindow.h"

#include <QToolBar>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QGuiApplication>
#include <QScreen>
#include <QErrorMessage>
#include <QFileDialog>

#include "about_view.h"
#include "function_viewer.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    config(new Configuration),
    function_viewer(new FunctionViewer(config, this))
{
    this->setCentralWidget(this->function_viewer);
    this->setupActions();
    this->resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
}

MainWindow::~MainWindow()
{

}

void MainWindow::showError(const QString &text) const {
    QErrorMessage error_message;
    error_message.setModal(true);
    error_message.showMessage(text);
    error_message.exec();
}

void MainWindow::setupActions() {
    this->statusBar()->show();
    QToolBar *toolbar = this->addToolBar(tr("Main toolbar"));
    QMenu *file_menu = this->menuBar()->addMenu(tr("&File"));
    QAction *open_action = file_menu->addAction(
                QIcon::fromTheme("document-open", QIcon(":/icons/open.png")),
                tr("&Open"), this, &MainWindow::openConfig, QKeySequence(QString("Ctrl+O")));
    toolbar->addAction(open_action);
    open_action->setStatusTip("Load configuration from specified file");
    file_menu->addSeparator();
    QAction *quit_action = file_menu->addAction(tr("&Quit..."), this, &MainWindow::close, QKeySequence(QString("Ctrl+Q")));
    quit_action->setStatusTip("Quit the application");

    QMenu *view_menu = this->menuBar()->addMenu(tr("&View"));
    this->toolbar_switch = view_menu->addAction(tr("&Toolbar"), this, [this, toolbar] {
        bool b = toolbar->isHidden();
        toolbar->setHidden(!b);
        this->toolbar_switch->setChecked(b);
    });
    this->toolbar_switch->setCheckable(true);
    this->toolbar_switch->setChecked(true);
    this->toolbar_switch->setStatusTip("Show/hide toolbar");

    this->statusbar_switch = view_menu->addAction(tr("&Statusbar"), this, [this] {
        bool b = this->statusBar()->isHidden();
        this->statusBar()->setHidden(!b);
        this->statusbar_switch->setChecked(b);
    });
    this->statusbar_switch->setCheckable(true);
    this->statusbar_switch->setChecked(true);
    this->statusbar_switch->setStatusTip("Show/hide statusbar");

    QMenu *isolines_menu = this->menuBar()->addMenu(tr("&Isolines"));
    toolbar->addAction(this->interpolate_colors = isolines_menu->addAction(tr("Interpolate colors"), this, [this] {
        this->config->setInterpolate(!this->config->interpolate());
    }));
    this->interpolate_colors->setCheckable(true);
    this->interpolate_colors->setChecked(this->config->interpolate());
    connect(this->config.data(), &Configuration::interpolateChanged, this, [this] (bool b) {
        this->interpolate_colors->setChecked(b);
    });
    this->interpolate_colors->setStatusTip("Enable/disable color interpolation");

    QAction *about_action = this->menuBar()->addAction(tr("&?"), this, SLOT(showAbout()));
    about_action->setIcon(QIcon(":/icons/about.png"));
    about_action->setStatusTip("About this application");
}

void MainWindow::openConfig() {
    QString filename = QFileDialog::getOpenFileName();
    if (filename.length() == 0) {
        return;
    }
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly)) {
        showError(QString("Cannot open file %1 to load data").arg(filename));
        return;
    }
    QTextStream in(&f);
    if (!this->config->load(in)) {
        showError(QString("Incorrect configuration file %1").arg(filename));
    }
}

void MainWindow::showAbout() {
    AboutView about_view;
    about_view.setModal(true);
    about_view.show();
    about_view.exec();
}
