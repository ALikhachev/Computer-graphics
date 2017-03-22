#include <QHBoxLayout>

#include "zone_container.h"

ZoneContainer::ZoneContainer(QWidget *parent) : QWidget(parent)
{
    this->resize(352 * 3 + 10 * 5, 352 + 2 * 10);
    QHBoxLayout *layout = new QHBoxLayout(this);
    this->zone_a = new FilterZone(this);
    this->zone_b = new FilterZone(this);
    this->zone_c = new FilterZone(this);
    layout->addWidget(zone_a);
    layout->addWidget(zone_b);
    layout->addWidget(zone_c);
    layout->setSpacing(10);
    this->setLayout(layout);
}

void ZoneContainer::setSourceImage(QImage image) {
    this->zone_a->setImage(image);
}
