#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>

#include "configuration.h"
#include "function_viewer.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void selectAndOpenConfig();
    void showAbout();
    void showConfiguration();
    void openRecentFile();

private:
    void showError(const QString &text) const;
    void setupActions();
    void setCurrentFile(const QString &filename);
    void openConfig(const QString &filename);
    void updateRecentFileActions();
    QString strippedName(const QString &full_filename);

    QAction *toolbar_switch;
    QAction *statusbar_switch;
    QAction *interpolate_colors;
    QAction *show_grid;
    QAction *show_isolines;
    QAction *show_entries;

    QSharedPointer<Configuration> config;
    FunctionViewer *function_viewer;

    const static int MaxRecentFiles = 5;
    QAction *separator_act;
    QAction *recent_file_acts[MaxRecentFiles];
};

#endif // MAINWINDOW_H
