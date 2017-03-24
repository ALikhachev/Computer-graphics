#include <QToolBar>
#include <QFile>
#include <QFileDialog>
#include <QScrollArea>
#include <QStatusBar>
#include <QMenuBar>
#include <QImageReader>
#include <QErrorMessage>

#include "main_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(1280, 720);
    this->setMinimumSize(200, 200);
    this->filters.push_back(QSharedPointer<Filter>(new GrayscaleFilter));
    this->zone_container = new ZoneContainer(this->filters, this);
    QScrollArea *scroll_area = new QScrollArea(this);
    scroll_area->setBackgroundRole(QPalette::Light);
    scroll_area->setWidget(this->zone_container);
    this->setCentralWidget(scroll_area);
    this->setupActions();
    this->initFilters();
}

void MainWindow::initFilters() {
    QToolBar *filters_toolbar = this->addToolBar(tr("Filters"));
    QMenu *filters_menu = this->menuBar()->addMenu(tr("F&ilters"));
    for (auto it = this->filters.begin(); it < this->filters.end(); ++it) {
        QAction *action = filters_menu->addAction((*it)->getIcon(), (*it)->getName(), this, [it]{(*it)->request();});
        filters_toolbar->addAction(action);
        action->setStatusTip(tr("Apply %1").arg((*it)->getName()));
    }
}

void MainWindow::setupActions() {
    this->statusBar()->show();
    QToolBar *toolbar = this->addToolBar(tr("Main"));

    QMenu *file_menu = this->menuBar()->addMenu(tr("&File"));

    QAction *new_act = file_menu->addAction(tr("&New"), this, clearZones);
    new_act->setShortcut(tr("Ctrl+N"));
    const QIcon new_icon = QIcon::fromTheme("document-new", QIcon(":/icons/new.png"));
    new_act->setIcon(new_icon);
    new_act->setStatusTip("Clear working area");
    toolbar->addAction(new_act);

    QAction *open_act = file_menu->addAction(tr("&Open"), this, openImage);
    open_act->setShortcut(tr("Ctrl+O"));
    const QIcon open_icon = QIcon::fromTheme("document-open", QIcon(":/icons/open.png"));
    open_act->setIcon(open_icon);
    open_act->setStatusTip("Open an image");
    toolbar->addAction(open_act);

    file_menu->addSeparator();
    QAction *exit_Act = file_menu->addAction(tr("&Quit..."), this, close);
    exit_Act->setStatusTip("Quit application");
    exit_Act->setShortcut(tr("Ctrl+Q"));
}

void MainWindow::openImage() {
    QString filename = QFileDialog::getOpenFileName(this, tr("Open image"), QDir::currentPath(), tr("Images (*.png *.bmp)"));
    if (filename.length() == 0) {
        return;
    }
    QImageReader reader(filename);
    if (!reader.canRead()) {
        showError(tr("Cannot open file %1 to load image").arg(filename));
        return;
    }
    QImage image = reader.read();
    if (image.isNull()) {
        showError(tr("Cannot decode image!"));
        return;
    }
    this->zone_container->setSourceImage(image);
    this->opened_from_file_name = filename;
}

void MainWindow::showError(QString text) {
    QErrorMessage error_message;
    error_message.setModal(true);
    error_message.showMessage(text);
    error_message.exec();
}

void MainWindow::clearZones() {
    this->opened_from_file_name = QString();
    this->zone_container->clear();
}
