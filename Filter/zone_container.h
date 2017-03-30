#ifndef ZONE_CONTAIINER_H
#define ZONE_CONTAINER_H

#include <QWidget>
#include <QScrollArea>
#include <QSharedPointer>
#include <QThreadPool>
#include <QPushButton>

#include "filter.h"
#include "filter_zone.h"
#include "filter_widget.h"

class SettingsWidget : public QWidget {
    Q_OBJECT
public:
    explicit SettingsWidget(QWidget *parent = 0);

public slots:
    void showFilterWidget(Filter *f);
    void clear();

signals:
    void saveFilterRequested(Filter *f);
    void restoreFilterRequested();

private:
    Filter *last_filter;
    QScrollArea *scroll_area;
    FilterWidget *widget;
    QPushButton *ok_button;
    QPushButton *cancel_button;
};

class ZoneContainer : public QWidget
{
    Q_OBJECT
public:
    explicit ZoneContainer(std::vector<QSharedPointer<Filter>> &, QWidget *parent = 0);
    ~ZoneContainer();
    void setSourceImage(QImage);
    void clear();
    bool saveResultImage(QString filename);

public slots:
    void copyBToC();
    void copyCToB();

private slots:
    void saveFilterWithSettings(Filter *f);
    void restoreFilterWithSettings();

signals:
    void filterImage(Filter *f, QImage image);
    void progressChanged(int);

private:
    SourceZone *zone_a;
    FilterZone *zone_b;
    FilterZone *zone_c;
    std::vector<QSharedPointer<Filter>> &filters;
    QThreadPool *thread_pool;
    bool clean;
    SettingsWidget *settings_widget;
    Filter *saved_filter;
    FilterSettings *saved_filter_settings;
};

#endif // ZONE_CONTAINER_H
