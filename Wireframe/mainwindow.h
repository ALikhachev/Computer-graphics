#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QToolBar>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void selectAndOpenScene();
    void selectAndSaveScene();
    void openRecentFile();
    void showAbout();
    void showConfiguration();

private:
    void initActions();
    void initFileMenu(QToolBar *toolbar);
    void initViewMenu(QToolBar *toolbar);
    void showError(const QString &text) const;
    void openScene(const QString &filename);
    void updateScene();
    void saveScene(const QString &filename);
    void setCurrentFile(const QString &filename);
    void updateRecentFileActions();
    static QString strippedName(const QString &full_filename);

    const static int MaxRecentFiles = 5;
    QAction *separator_action;
    QAction *recent_file_acts[MaxRecentFiles];
    QAction *toolbar_switch;
    QAction *statusbar_switch;
};

#endif // MAINWINDOW_H
