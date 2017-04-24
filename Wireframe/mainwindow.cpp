#include "mainwindow.h"

#include <QErrorMessage>
#include <QFile>
#include <QStatusBar>
#include <QMenuBar>
#include <QFileDialog>
#include <QApplication>
#include <QSettings>
#include <QScreen>

#include "about_view.h"
#include "generatrix_view.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("Wireframe by Alexander Likhachev");
    this->statusBar()->show();
    this->initActions();
    this->updateRecentFileActions();
    this->setCentralWidget(new GeneratrixView);
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

void MainWindow::initActions() {
    QToolBar *toolbar = this->addToolBar(tr("Main toolbar"));
    this->initFileMenu(toolbar);
    this->initViewMenu(toolbar);

    QAction *about_action = this->menuBar()->addAction(tr("&?"), this, SLOT(showAbout()));
    about_action->setIcon(QIcon(":/icons/about.png"));
    about_action->setStatusTip("About this application");
}

void MainWindow::initFileMenu(QToolBar *toolbar) {
    QMenu *file_menu = this->menuBar()->addMenu(tr("&File"));
    QAction *open_action = file_menu->addAction(QIcon::fromTheme("document-open", QIcon(":/icons/open.png")),
                tr("&Open"), this, &MainWindow::selectAndOpenScene, QKeySequence(QString("Ctrl+O")));
    toolbar->addAction(open_action);
    open_action->setStatusTip("Load scene from specified file");

    QAction *save_action = file_menu->addAction(QIcon::fromTheme("document-save", QIcon(":/icons/save.png")),
                tr("&Save"), this, &MainWindow::updateScene, QKeySequence(QString("Ctrl+S")));
    toolbar->addAction(save_action);
    save_action->setStatusTip("Save scene");

    QAction *save_as_action = file_menu->addAction(tr("Save &as..."), this, &MainWindow::selectAndSaveScene, QKeySequence(QString("Ctrl+Shift+S")));
    save_as_action->setStatusTip("Save scene into specified file");

    this->separator_action = file_menu->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i) {
        this->recent_file_acts[i] = new QAction(this);
        this->recent_file_acts[i]->setVisible(false);
        connect(this->recent_file_acts[i], &QAction::triggered, this, &MainWindow::openRecentFile);
        file_menu->addAction(this->recent_file_acts[i]);
    }

    file_menu->addSeparator();
    QAction *quit_action = file_menu->addAction(tr("&Quit"), this, &MainWindow::close, QKeySequence(QString("Ctrl+Q")));
    quit_action->setStatusTip("Quit the application");
}

void MainWindow::initViewMenu(QToolBar *toolbar) {
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
}

void MainWindow::selectAndOpenScene() {
    QString filename = QFileDialog::getOpenFileName();
    if (!filename.isEmpty()) {
        this->openScene(filename);
    }
}

void MainWindow::selectAndSaveScene() {
    QString filename = QFileDialog::getSaveFileName();
    if (!filename.isEmpty()) {
        this->saveScene(filename);
    }
}

void MainWindow::openScene(const QString &filename) {
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly)) {
        this->showError(QString("Cannot open file %1 to load data").arg(filename));
        return;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);
    this->setCurrentFile(filename);
    QApplication::restoreOverrideCursor();
}

void MainWindow::updateScene() {
    if (this->windowFilePath().isNull()) {
        this->selectAndSaveScene();
    } else {
        this->saveScene(this->windowFilePath());
    }
}

void MainWindow::saveScene(const QString &filename) {
    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly)) {
        this->showError(QString("Cannot open file %1 to write data").arg(filename));
        return;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);
    this->setCurrentFile(filename);
    QApplication::restoreOverrideCursor();
}

void MainWindow::showAbout() {
    AboutView about_view;
    about_view.setModal(true);
    about_view.show();
    about_view.exec();
}

void MainWindow::showConfiguration() {

}

void MainWindow::setCurrentFile(const QString &fileName) {
    this->setWindowFilePath(fileName);

    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while (files.size() > MaxRecentFiles) {
        files.removeLast();
    }

    settings.setValue("recentFileList", files);

    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
        if (mainWin) {
            mainWin->updateRecentFileActions();
        }
    }
}

void MainWindow::openRecentFile() {
    QAction *action = qobject_cast<QAction *>(this->sender());
    if (action) {
        this->openScene(action->data().toString());
    }
}

void MainWindow::updateRecentFileActions() {
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();

    int num_recent_files = qMin(files.size(), (int) MaxRecentFiles);

    for (int i = 0; i < num_recent_files; ++i) {
        const QString filename = strippedName(files[i]);
        QString text = tr("&%1 %2").arg(i + 1).arg(filename);
        this->recent_file_acts[i]->setText(text);
        this->recent_file_acts[i]->setData(files[i]);
        this->recent_file_acts[i]->setVisible(true);
        this->recent_file_acts[i]->setStatusTip(QString("Load scene from file %1").arg(filename));
    }
    for (int j = num_recent_files; j < MaxRecentFiles; ++j) {
        this->recent_file_acts[j]->setVisible(false);
    }

    this->separator_action->setVisible(num_recent_files > 0);
}

QString MainWindow::strippedName(const QString &full_file_name) {
    return QFileInfo(full_file_name).fileName();
}
