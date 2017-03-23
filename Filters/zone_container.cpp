#include <QHBoxLayout>

#include "zone_container.h"

ZoneContainer::ZoneContainer(std::vector<QSharedPointer<Filter>> &filters, QWidget *parent) : QWidget(parent),
    filters(filters)
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
    for (auto it = this->filters.begin(); it < this->filters.end(); ++it) {
        Filter *f = (*it).data();
        this->connect(f, &Filter::requested, [this, f]{
            this->zone_c->setImage(f->applyFilter(this->zone_a->getImage()));
        });
    }
}

void ZoneContainer::setSourceImage(QImage image) {
    this->zone_a->setImage(image);
}

void ZoneContainer::clear() {
    this->zone_a->clear();
    this->zone_b->clear();
    this->zone_c->clear();
}
