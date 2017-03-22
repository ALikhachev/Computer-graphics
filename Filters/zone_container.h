#ifndef ZONE_CONTAIINER_H
#define ZONE_CONTAIINER_H

#include <QWidget>

#include "filter_zone.h"

class ZoneContainer : public QWidget
{
    Q_OBJECT
public:
    explicit ZoneContainer(QWidget *parent = 0);
    void setSourceImage(QImage);

private:
    FilterZone *zone_a;
    FilterZone *zone_b;
    FilterZone *zone_c;
};

#endif // ZONE_CONTAIINER_H
