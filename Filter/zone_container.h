#ifndef ZONE_CONTAIINER_H
#define ZONE_CONTAINER_H

#include <QWidget>
#include <QSharedPointer>
#include <QThreadPool>

#include "filter.h"
#include "filter_zone.h"

class ZoneContainer : public QWidget
{
    Q_OBJECT
public:
    explicit ZoneContainer(std::vector<QSharedPointer<Filter>> &, QWidget *parent = 0);
    ~ZoneContainer();
    void setSourceImage(QImage);
    void clear();

public slots:
    void copyBToC();
    void copyCToB();

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
    QWidget *settings_widget;
    Filter *last_filter;
};

#endif // ZONE_CONTAINER_H
