#include <QHBoxLayout>

#include "zone_container.h"

ZoneContainer::ZoneContainer(QWidget *parent) : QWidget(parent)
{
    this->resize(350 * 3 + 10 * 5, 350);
    QHBoxLayout *layout = new QHBoxLayout(this);
    zone_a = new FilterZone(this);
    zone_b = new FilterZone(this);
    zone_c = new FilterZone(this);
    layout->addWidget(zone_a);
    layout->addWidget(zone_b);
    layout->addWidget(zone_c);
    layout->setSpacing(10);
    this->setLayout(layout);
}
