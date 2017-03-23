#ifndef ZONE_CONTAIINER_H
#define ZONE_CONTAINER_H

#include <QWidget>
#include <QSharedPointer>

#include "filter.h"
#include "filter_zone.h"

class ZoneContainer : public QWidget
{
    Q_OBJECT
public:
    explicit ZoneContainer(std::vector<QSharedPointer<Filter>> &, QWidget *parent = 0);
    void setSourceImage(QImage);
    void clear();

private:
    FilterZone *zone_a;
    FilterZone *zone_b;
    FilterZone *zone_c;
    std::vector<QSharedPointer<Filter>> &filters;
};

#endif // ZONE_CONTAINER_H
