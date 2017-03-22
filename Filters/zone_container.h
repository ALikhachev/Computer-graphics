#ifndef ZONE_CONTAIINER_H
#define ZONE_CONTAINER_H

#include <QWidget>

#include "filter_zone.h"

class ZoneContainer : public QWidget
{
    Q_OBJECT
public:
    explicit ZoneContainer(QWidget *parent = 0);
    void setSourceImage(QImage);
    void clear();

private:
    FilterZone *zone_a;
    FilterZone *zone_b;
    FilterZone *zone_c;
};

#endif // ZONE_CONTAINER_H
