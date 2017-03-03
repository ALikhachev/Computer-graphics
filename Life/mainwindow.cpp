#include <QMenu>
#include <QMenuBar>
#include <QShortcut>
#include <QStatusBar>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include "save_view.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    scrollArea(new QScrollArea(this)),
    board(&settings, settings.width, settings.height, std::vector<Cell>(settings.width * settings.height)),
    board_view(new BoardView(&board, this)),
    loop_timer(new QTimer(this)),
    loadedFromFile(false)
{
    setWindowTitle("Life");

    createActions();

    scrollArea->setBackgroundRole(QPalette::Light);

    scrollArea->setWidget(board_view);

    setCentralWidget(scrollArea);

    connect(loop_timer, SIGNAL(timeout()), &board, SLOT(tick()));
    connect(&settings, SIGNAL(viewSettingsChanged()), this, SLOT(viewSettingsChanged()));
    connect(&settings, SIGNAL(modelSettingsChanged()), this, SLOT(modelSettingsChanged()));
}

void MainWindow::createActions() {
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    toolbar = addToolBar(tr("Toolbar"));

    QAction *newAct = fileMenu->addAction(tr("&New"), this, newGame);
    newAct->setShortcut(tr("Ctrl+N"));
    const QIcon newIcon = QIcon::fromTheme("board-new", QIcon(":/icons/new.png"));
    newAct->setIcon(newIcon);
    newAct->setStatusTip("Create a new Convey's Life game board");
    toolbar->addAction(newAct);

    QAction *openAct = fileMenu->addAction(tr("&Open..."), this, loadGame);
    openAct->setShortcut(tr("Ctrl+O"));
    const QIcon openIcon = QIcon::fromTheme("board-open", QIcon(":/icons/open.png"));
    openAct->setIcon(openIcon);
    openAct->setStatusTip("Load Convey's Life game board from file");
    toolbar->addAction(openAct);

    QAction *saveAct = fileMenu->addAction(tr("&Save"), this, saveGame);
    saveAct->setShortcut(tr("Ctrl+S"));
    const QIcon saveIcon = QIcon::fromTheme("board-save", QIcon(":/icons/save.png"));
    saveAct->setIcon(saveIcon);
    saveAct->setStatusTip("Save current game board to the file");
    toolbar->addAction(saveAct);

    QAction *saveAsAct = fileMenu->addAction(tr("&Save as..."), this, saveGameAs);
    saveAsAct->setStatusTip("Save current game board to specific file");
    saveAsAct->setShortcut(tr("Ctrl+Shift+S"));

    fileMenu->addSeparator();

    QAction *exitAct = fileMenu->addAction(tr("&Exit..."), this, &QWidget::close);
    saveAsAct->setStatusTip("Exit the game");
    exitAct->setShortcut(tr("Ctrl+Q"));

    toolbar->addSeparator();

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));

    xor_mode_action = editMenu->addAction(tr("&XOR mode"), this, setXorMode);
    xor_mode_action->setCheckable(true);
    const QIcon xor_mode_icon = QIcon::fromTheme("replace-mode-switch", QIcon(":/icons/xor.png"));
    xor_mode_action->setIcon(xor_mode_icon);
    xor_mode_action->setStatusTip("Enable XOR cell state setting mode");
    toolbar->addAction(xor_mode_action);

    replace_mode_action = editMenu->addAction(tr("&Replace mode"), this, setReplaceMode);
    replace_mode_action->setCheckable(true);
    replace_mode_action->setChecked(true);
    const QIcon replace_mode_icon = QIcon::fromTheme("replace-mode-switch", QIcon(":/icons/replace.png"));
    replace_mode_action->setIcon(replace_mode_icon);
    replace_mode_action->setStatusTip("Enable replace cell state setting mode");
    toolbar->addAction(replace_mode_action);

    editMenu->addSeparator();
    toolbar->addSeparator();

    clear_board_action = editMenu->addAction(tr("&Clear"), &board, &Board::clear);
    clear_board_action->setShortcut(tr("Ctrl+L"));
    const QIcon clear_board_icon = QIcon::fromTheme("clear-board", QIcon(":/icons/clear.png"));
    clear_board_action->setIcon(clear_board_icon);
    clear_board_action->setStatusTip("Clear the game board");
    toolbar->addAction(clear_board_action);

    editMenu->addSeparator();

    QAction *parametersAct = editMenu->addAction(tr("&Parameters"), this, showSettings);
    const QIcon settingsIcon = QIcon::fromTheme("settings", QIcon(":/icons/settings.png"));
    parametersAct->setIcon(settingsIcon);
    parametersAct->setStatusTip("Show game settings window");
    toolbar->addAction(parametersAct);

    toolbar->addSeparator();

    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
    toolBarStateAction = viewMenu->addAction(tr("&Show toolbar"), this, toggleToolBar);
    toolBarStateAction->setCheckable(true);
    toolBarStateAction->setChecked(true);
    toolBarStateAction->setStatusTip("Show/hide toolbar");

    statusBarStateAction = viewMenu->addAction(tr("&Show statusbar"), this, toggleStatusBar);
    statusBarStateAction->setCheckable(true);
    statusBarStateAction->setChecked(true);
    statusBarStateAction->setStatusTip("Show/hide statusbar");
    statusBar()->show();

    show_impacts_action = viewMenu->addAction(tr("&Show impacts"), this, toggleShowImpacts);
    show_impacts_action->setCheckable(true);
    show_impacts_action->setIcon(QIcon(":/icons/impacts.png"));
    show_impacts_action->setStatusTip("Show/hide impacts");
    toolbar->addAction(show_impacts_action);

    QMenu *simulationMenu = menuBar()->addMenu(tr("&Simulation"));

    run_once_action = simulationMenu->addAction(tr("&Run once"), &board, &Board::tick);
    const QIcon run_once_icon = QIcon::fromTheme("run-once", QIcon(":/icons/run-once.png"));
    run_once_action->setShortcut(QString("Space"));
    run_once_action->setIcon(run_once_icon);
    run_once_action->setStatusTip("Run game loop once");
    toolbar->addAction(run_once_action);

    run_loop_action = simulationMenu->addAction(tr("&Run loop"), this, toggleLoopMode);
    const QIcon run_loop_icon = QIcon::fromTheme("run", QIcon(":/icons/run.png"));
    run_loop_action->setIcon(run_loop_icon);
    run_loop_action->setCheckable(true);
    run_loop_action->setStatusTip("Run game loop");
    toolbar->addAction(run_loop_action);

    QAction *helpAct = menuBar()->addAction(tr("&?"), this, SLOT(showAbout()));
    helpAct->setShortcut(tr("F1"));
    const QIcon helpIcon = QIcon::fromTheme("about", QIcon(":/icons/about.png"));
    helpAct->setStatusTip("About this application");
    helpAct->setIcon(helpIcon);
}

void MainWindow::showAbout() {
    AboutView about_view;
    about_view.setModal(true);
    about_view.show();
    about_view.exec();
}

void MainWindow::showSettings() {
    BoardSettingsView settings_view(&settings);
    settings_view.setModal(true);
    settings_view.show();
    settings_view.exec();
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
    if (toolbar->isHidden()) {
        toolbar->show();
        toolBarStateAction->setChecked(true);
    } else {
        toolbar->hide();
        toolBarStateAction->setChecked(false);
    }
}

void MainWindow::toggleLoopMode() {
    if (loop_timer->isActive()) {
        run_loop_action->setChecked(false);
        run_once_action->setDisabled(false);
        clear_board_action->setDisabled(false);
        loop_timer->stop();
        board_view->toggleEditing(true);
    } else {
        run_loop_action->setChecked(true);
        run_once_action->setDisabled(true);
        clear_board_action->setDisabled(true);
        loop_timer->start(1000);
        board_view->toggleEditing(false);
    }
}

void MainWindow::setXorMode() {
    settings.isXorMode = true;
    xor_mode_action->setChecked(true);
    replace_mode_action->setChecked(false);
}

void MainWindow::setReplaceMode() {
    settings.isXorMode = false;
    xor_mode_action->setChecked(false);
    replace_mode_action->setChecked(true);
}

void MainWindow::toggleShowImpacts() {
    settings.show_impacts = !settings.show_impacts;
    show_impacts_action->setChecked(settings.show_impacts);
}

void MainWindow::modelSettingsChanged() {
    board.resize(settings.width, settings.height);
}

void MainWindow::viewSettingsChanged() {
    board_view->recountBoard();
    if (settings.cellSize < 13) {
        show_impacts_action->setEnabled(false);
        show_impacts_action->setChecked(false);
        settings.show_impacts = false;
    } else {
        show_impacts_action->setEnabled(true);
    }
}

void MainWindow::newGame() {
    if (loop_timer->isActive()) {
        toggleLoopMode();
    }
    if (board.isStateChanged()) {
        if (QMessageBox::Cancel == showSaveDialog()) {
            return;
        }
    }
    settings.resetToDefault();
    show_impacts_action->setChecked(false);
    xor_mode_action->setChecked(false);
    replace_mode_action->setChecked(true);
    board.clear();
    settings.updateModel();
    settings.updateView();
    loadedFromFile = false;
}

void MainWindow::loadGame() {
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
    board.load(in);
    loadedFromFile = true;
    this->filename = filename;
}

void MainWindow::saveGame() {
    QString filename = loadedFromFile ? this->filename : QFileDialog::getSaveFileName();
    if (filename.length() == 0) {
        return;
    }
    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly)) {
        showError(QString("Cannot open file %1 to save data").arg(filename));
        return;
    }
    QTextStream out(&f);
    board.save(out);
    loadedFromFile = true;
    this->filename = filename;
}

void MainWindow::saveGameAs() {
    QString filename = QFileDialog::getSaveFileName();
    if (filename.length() == 0) {
        return;
    }
    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly)) {
        showError(QString("Cannot open file %1 to save data").arg(filename));
        return;
    }
    QTextStream out(&f);
    board.save(out);
    loadedFromFile = true;
    this->filename = filename;
}

void MainWindow::showError(QString text) {
    QErrorMessage error_message;
    error_message.setModal(true);
    error_message.showMessage(text);
    error_message.exec();
}

int MainWindow::showSaveDialog() {
    QMessageBox msg_box;
    msg_box.setText("The document has been modified.");
    msg_box.setInformativeText("Do you want to save your changes?");
    msg_box.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msg_box.setDefaultButton(QMessageBox::Save);
    int ret = msg_box.exec();
    if (ret == QMessageBox::Save) {
        saveGame();
    }
    return ret;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (board.isStateChanged()) {
        if (QMessageBox::Cancel == showSaveDialog()) {
            event->ignore();
            return;
        }
    }
    QMainWindow::closeEvent(event);
}
