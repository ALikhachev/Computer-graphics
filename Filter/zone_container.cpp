#include <QHBoxLayout>

#include "zone_container.h"

ZoneContainer::ZoneContainer(std::vector<QSharedPointer<Filter>> &filters, QWidget *parent) : QWidget(parent),
    filters(filters)
{
    this->resize(352 * 3 + 10 * 5, 352 + 2 * 10);
    QHBoxLayout *layout = new QHBoxLayout(this);
    this->zone_a = new SourceZone(this);
    this->zone_b = new FilterZone(this);
    this->zone_c = new FilterZone(this);
    layout->addWidget(zone_a);
    layout->addWidget(zone_b);
    layout->addWidget(zone_c);
    layout->setSpacing(10);
    this->setLayout(layout);
    FilterWorker *filter_worker = new FilterWorker;
    filter_worker->moveToThread(&this->worker_thread);
    this->connect(&this->worker_thread, &QThread::finished, filter_worker, QObject::deleteLater);
    this->connect(filter_worker, &FilterWorker::resultReady, this->zone_c, &FilterZone::setImage);
    this->connect(this, filterImage, filter_worker, FilterWorker::doFilter);
    for (auto it = this->filters.begin(); it < this->filters.end(); ++it) {
        Filter *f = (*it).data();
        this->connect(f, &Filter::requested, [this, f] {
             emit filterImage(f, this->zone_b->getImage());
        });
    }
    this->connect(zone_a, &SourceZone::zoneSelected, this->zone_b, FilterZone::setImage);
    worker_thread.start();
}

ZoneContainer::~ZoneContainer() {
    worker_thread.quit();
    worker_thread.wait();
}

void ZoneContainer::setSourceImage(QImage image) {
    this->zone_a->setImage(image);
}

void ZoneContainer::clear() {
    this->zone_a->clear();
    this->zone_b->clear();
    this->zone_c->clear();
}

void ZoneContainer::copyBToC() {
    this->zone_c->setImage(this->zone_b->getImage());
}

void ZoneContainer::copyCToB() {
    this->zone_b->setImage(this->zone_c->getImage());
}
