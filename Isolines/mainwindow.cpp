#include "mainwindow.h"

#include <QToolBar>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QGuiApplication>
#include <QScreen>
#include <QErrorMessage>
#include <QFileDialog>
#include <QScopedPointer>
#include <QSettings>
#include <QApplication>

#include "about_view.h"
#include "configuration_dialog.h"
#include "function_viewer.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    config(new Configuration),
    function_viewer(new FunctionViewer(config, this))
{
    this->setWindowTitle("Isolines by Alexander Likhachev");
    this->setCentralWidget(this->function_viewer);
    this->setupActions();
    this->updateRecentFileActions();
    this->resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
    connect(this->function_viewer, &FunctionViewer::pointerFunctionValueUpdated, this, [this] (IsolinesMousePosition position) {
        if (position.out_of_screen) {
            this->statusBar()->clearMessage();
        } else {
            this->statusBar()->showMessage(QString("f(%1, %2) = %3").arg(
                                               QString::number(position.x, 'f', 2),
                                               QString::number(position.y, 'f', 2),
                                               QString::number(position.value, 'f', 2)
                                           ));
        }
    });
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
                tr("&Open"), this, &MainWindow::selectAndOpenConfig, QKeySequence(QString("Ctrl+O")));
    toolbar->addAction(open_action);
    open_action->setStatusTip("Load configuration from specified file");

    this->separator_act = file_menu->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i) {
        this->recent_file_acts[i] = new QAction(this);
        this->recent_file_acts[i]->setVisible(false);
        connect(this->recent_file_acts[i], &QAction::triggered, this, &MainWindow::openRecentFile);
        file_menu->addAction(this->recent_file_acts[i]);
    }

    file_menu->addSeparator();
    QAction *quit_action = file_menu->addAction(tr("&Quit..."), this, &MainWindow::close, QKeySequence(QString("Ctrl+Q")));
    quit_action->setStatusTip("Quit the application");

    QMenu *isolines_menu = this->menuBar()->addMenu(tr("&Isolines"));
    toolbar->addAction(this->interpolate_colors = isolines_menu->addAction(QIcon(":/icons/interpolation.png"),
                                                                           tr("Interpolate colors"), this, [this] {
        this->config->setInterpolate(!this->config->interpolate());
    }));
    this->interpolate_colors->setCheckable(true);
    this->interpolate_colors->setChecked(this->config->interpolate());
    connect(this->config.data(), &Configuration::interpolateChanged, this, [this] (bool b) {
        this->interpolate_colors->setChecked(b);
    });
    this->interpolate_colors->setStatusTip("Enable/disable color interpolation");

    toolbar->addAction(this->show_grid = isolines_menu->addAction(QIcon(":/icons/grid.png"), tr("Show grid"), this, [this] {
        this->config->setShowGrid(!this->config->showGrid());
    }));
    this->show_grid->setCheckable(true);
    this->show_grid->setChecked(this->config->showGrid());
    connect(this->config.data(), &Configuration::showGridChanged, this, [this] (bool b) {
        this->show_grid->setChecked(b);
    });
    this->show_grid->setStatusTip("Show/hide grid");

    toolbar->addAction(this->show_isolines = isolines_menu->addAction(QIcon(":/icons/isolines.png"), tr("Show isolines"), this, [this] {
        this->config->setShowIsolines(!this->config->showIsolines());
    }));
    this->show_isolines->setCheckable(true);
    this->show_isolines->setChecked(this->config->showIsolines());
    connect(this->config.data(), &Configuration::showIsolinesChanged, this, [this] (bool b) {
        this->show_isolines->setChecked(b);
        this->show_entries->setDisabled(!b);
    });
    this->show_isolines->setStatusTip("Show/hide isolines");

    toolbar->addAction(this->show_entries = isolines_menu->addAction(QIcon(":/icons/entries.png"), tr("Show cell entries"), this, [this] {
        this->config->setShowEntries(!this->config->showEntries());
    }));
    this->show_entries->setCheckable(true);
    this->show_entries->setChecked(this->config->showEntries());
    this->show_entries->setDisabled(!this->config->showIsolines());
    connect(this->config.data(), &Configuration::showEntriesChanged, this, [this] (bool b) {
        this->show_entries->setChecked(b);
    });
    this->show_entries->setStatusTip("Show/hide points of entry isolines into cells");

    QAction *edit_configuration_action = isolines_menu->addAction(QIcon(":/icons/config.png"),
                                                                  tr("Edit configuration"),
                                                                  this, &MainWindow::showConfiguration);
    toolbar->addAction(edit_configuration_action);
    edit_configuration_action->setStatusTip("Open configuration edit window");

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

    QAction *about_action = this->menuBar()->addAction(tr("&?"), this, SLOT(showAbout()));
    about_action->setIcon(QIcon(":/icons/about.png"));
    about_action->setStatusTip("About this application");
}

void MainWindow::selectAndOpenConfig() {
    QString filename = QFileDialog::getOpenFileName();
    if (!filename.isEmpty()) {
        this->openConfig(filename);
    }
}

void MainWindow::openConfig(const QString &filename) {
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly)) {
        this->showError(QString("Cannot open file %1 to load data").arg(filename));
        return;
    }
    QTextStream in(&f);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    if (!this->config->load(in)) {
        this->showError(QString("Incorrect configuration file %1").arg(filename));
    } else {
        this->setCurrentFile(filename);
        this->statusBar()->showMessage(tr("Configuration loaded"), 2000);
    }
    QApplication::restoreOverrideCursor();
}

void MainWindow::showAbout() {
    AboutView about_view;
    about_view.setModal(true);
    about_view.show();
    about_view.exec();
}

void MainWindow::showConfiguration() {
    ConfigurationDialog conf_dialog(this->config, this);
    conf_dialog.setModal(true);
    conf_dialog.show();
    conf_dialog.exec();
}

void MainWindow::setCurrentFile(const QString &fileName)
{
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

void MainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(this->sender());
    if (action) {
        this->openConfig(action->data().toString());
    }
}

void MainWindow::updateRecentFileActions()
{
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();

    int num_recent_files = qMin(files.size(), (int) MaxRecentFiles);

    for (int i = 0; i < num_recent_files; ++i) {
        QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
        this->recent_file_acts[i]->setText(text);
        this->recent_file_acts[i]->setData(files[i]);
        this->recent_file_acts[i]->setVisible(true);
    }
    for (int j = num_recent_files; j < MaxRecentFiles; ++j) {
        this->recent_file_acts[j]->setVisible(false);
    }

    this->separator_act->setVisible(num_recent_files > 0);
}

QString MainWindow::strippedName(const QString &full_file_name)
{
    return QFileInfo(full_file_name).fileName();
}
