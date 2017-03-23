#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QSharedPointer>

#include "zone_container.h"
#include "filter.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    void setupActions();

public slots:
    void clearZones();
    void openImage();

private:
    void showError(QString text);
    void initFilters();

    QString opened_from_file_name;
    ZoneContainer *zone_container;
    std::vector<QSharedPointer<Filter>> filters;
};

#endif // MAIN_WINDOW_H
