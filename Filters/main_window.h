#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "zone_container.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    void setupActions();

public slots:
    void openImage();

private:
    void showError(QString text);

    QString opened_from_file_name;
    ZoneContainer *zone_container;
};

#endif // MAINWINDOW_H
