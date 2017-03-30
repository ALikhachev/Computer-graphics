#include "main_window.h"

#include <QToolBar>
#include <QFile>
#include <QFileDialog>
#include <QScrollArea>
#include <QStatusBar>
#include <QMenuBar>
#include <QImageReader>
#include <QErrorMessage>

#include "filter_registry.h"
#include "about_view.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    progress_bar(new QProgressBar(this)),
    progress_bar_hide_timer(new QTimer(this)),
    filter_actions(new QActionGroup(this))
{
    this->resize(1280, 720);
    this->setMinimumSize(200, 200);
    this->zone_container = new ZoneContainer(FilterRegistry::getInstance().getFilters(), this);
    QScrollArea *scroll_area = new QScrollArea(this);
    scroll_area->setBackgroundRole(QPalette::Light);
    scroll_area->setWidget(this->zone_container);
    scroll_area->setWidgetResizable(true);
    this->setCentralWidget(scroll_area);
    this->setupActions();
    this->initFilters();
    this->statusBar()->addWidget(this->progress_bar);
    this->progress_bar->hide();
    this->progress_bar_hide_timer->setSingleShot(true);
    this->connect(this->progress_bar_hide_timer, &QTimer::timeout, this, [this] {
        this->progress_bar->hide();
    });
}

void MainWindow::initFilters() {
    QToolBar *filters_toolbar = this->addToolBar(tr("Filters"));
    for (auto it = FilterRegistry::getInstance().getFilters().begin(); it < FilterRegistry::getInstance().getFilters().end(); ++it) {
        QAction *action = this->filters_menu->addAction((*it)->getIcon(), (*it)->getName(), this, [it]{(*it)->request();});
        filters_toolbar->addAction(action);
        action->setStatusTip(tr("Apply %1 filter").arg((*it)->getName()));
        this->filter_actions->addAction(action);
    }
    this->connect(this->zone_container, &ZoneContainer::progressChanged, this, [this] (int value) {
        this->progress_bar->show();
        this->progress_bar->setValue(value);
        if (value == 100) {
            this->progress_bar_hide_timer->start(1500);
        }
    });
    this->filter_actions->setDisabled(true);
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

    QAction *save_act = file_menu->addAction(tr("&Save"), this, saveImage);
    save_act->setShortcut(tr("Ctrl+S"));
    const QIcon save_icon = QIcon::fromTheme("document-save", QIcon(":/icons/save.png"));
    save_act->setIcon(save_icon);
    save_act->setStatusTip("Save an image");
    toolbar->addAction(save_act);
    this->filter_actions->addAction(save_act);

    QAction *c_to_b_act = file_menu->addAction(tr("&Copy C to B"), this->zone_container, &ZoneContainer::copyCToB);
    const QIcon c_to_b_icon = QIcon(":/icons/left.png");
    c_to_b_act->setIcon(c_to_b_icon);
    c_to_b_act->setStatusTip("Copy image from zone C to zone B");
    toolbar->addAction(c_to_b_act);
    this->filter_actions->addAction(c_to_b_act);

    QAction *b_to_c_act = file_menu->addAction(tr("&Copy B to C"), this->zone_container, &ZoneContainer::copyBToC);
    const QIcon b_to_c_icon = QIcon(":/icons/right.png");
    b_to_c_act->setIcon(b_to_c_icon);
    b_to_c_act->setStatusTip("Copy image from zone B to zone C");
    toolbar->addAction(b_to_c_act);
    this->filter_actions->addAction(b_to_c_act);

    file_menu->addSeparator();
    QAction *exit_Act = file_menu->addAction(tr("&Quit..."), this, close);
    exit_Act->setStatusTip("Quit application");
    exit_Act->setShortcut(tr("Ctrl+Q"));

    this->filters_menu = this->menuBar()->addMenu(tr("F&ilters"));

    QAction *about_act = this->menuBar()->addAction(tr("&?"), this, SLOT(showAbout()));
    about_act->setShortcut(tr("F1"));
    about_act->setStatusTip("About this application");
    about_act->setIcon(QIcon(":/icons/about.png"));
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
    if (image.format() != QImage::Format_RGBA8888) {
        image = image.convertToFormat(QImage::Format_RGBA8888);
    }
    this->zone_container->setSourceImage(image);
    this->opened_from_file_name = filename;
    this->filter_actions->setDisabled(false);
}

void MainWindow::saveImage() {
    QString filename = QFileDialog::getSaveFileName(this, tr("Open image"), QDir::currentPath(), tr("Images (*.png *.bmp)"));
    if (filename.length() == 0) {
        return;
    }
    if (!this->zone_container->saveResultImage(filename)) {
        this->showError(tr("Cannot save result image!"));
    }
}

void MainWindow::showAbout() {
    AboutView about_view;
    about_view.setModal(true);
    about_view.show();
    about_view.exec();
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
    this->filter_actions->setDisabled(true);
}
