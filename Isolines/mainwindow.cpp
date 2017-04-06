#include "mainwindow.h"

#include <QToolBar>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QGuiApplication>
#include <QScreen>
#include <QErrorMessage>
#include <QFileDialog>

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
    toolbar->addAction(file_menu->addAction(tr("&New"), [] {

    }, QKeySequence(QString("Ctrl+N"))));
    toolbar->addAction(file_menu->addAction(tr("&Open"), this, &MainWindow::openConfig, QKeySequence(QString("Ctrl+O"))));
    file_menu->addSeparator();
    file_menu->addAction(tr("&Quit..."), this, &MainWindow::close, QKeySequence(QString("Ctrl+Q")));

    QMenu *view_menu = this->menuBar()->addMenu(tr("&View"));
    this->toolbar_switch = view_menu->addAction(tr("&Toolbar"), this, [this, toolbar] {
        bool b = toolbar->isHidden();
        toolbar->setHidden(!b);
        this->toolbar_switch->setChecked(b);
    });
    this->toolbar_switch->setCheckable(true);
    this->toolbar_switch->setChecked(true);
    this->statusbar_switch = view_menu->addAction(tr("&Statusbar"), this, [this] {
        bool b = this->statusBar()->isHidden();
        this->statusBar()->setHidden(!b);
        this->statusbar_switch->setChecked(b);
    });
    this->statusbar_switch->setCheckable(true);
    this->statusbar_switch->setChecked(true);

    QMenu *isolines_menu = this->menuBar()->addMenu(tr("&Isolines"));
    toolbar->addAction(this->interpolate_colors = isolines_menu->addAction(tr("Interpolate colors"), this, [this] {
        this->config->setInterpolate(!this->config->interpolate());
    }));
    this->interpolate_colors->setCheckable(true);
    this->interpolate_colors->setChecked(this->config->interpolate());
    connect(this->config.data(), &Configuration::interpolateChanged, this, [this] (bool b) {
        this->interpolate_colors->setChecked(b);
    });
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
