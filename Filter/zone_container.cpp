#include <QHBoxLayout>
#include <QVBoxLayout>

#include "zone_container.h"
#include "filter_registry.h"

ZoneContainer::ZoneContainer(std::vector<QSharedPointer<Filter>> &filters, QWidget *parent) : QWidget(parent),
    filters(filters),
    thread_pool(new QThreadPool(this)),
    clean(true),
    settings_widget(NULL),
    last_filter(NULL)
{
    this->thread_pool->setMaxThreadCount(1);
    this->setMaximumWidth(352 * 3 + 10 * 5);
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    QHBoxLayout *layout = new QHBoxLayout();
    main_layout->addLayout(layout);
    this->zone_a = new SourceZone(this);
    this->zone_b = new FilterZone(this);
    this->zone_c = new FilterZone(this);
    layout->addWidget(this->zone_a);
    layout->addWidget(this->zone_b);
    layout->addWidget(this->zone_c);
    layout->setSpacing(10);
    for (auto it = this->filters.begin(); it < this->filters.end(); ++it) {
        Filter *f = it->data();
        this->connect(f, &Filter::requested, [this, f, main_layout] {
            if (f != this->last_filter) {
                if (this->settings_widget) {
                    delete this->settings_widget;
                }
                this->settings_widget = FilterRegistry::getInstance().getWidget(f, this);
                if (this->settings_widget) {
                    main_layout->addWidget(this->settings_widget);
                }
                this->last_filter = f;
            }
            FilterWorker *worker = new FilterWorker(f, this->zone_b->getImage());
            worker->setAutoDelete(true);
            connect(worker, &FilterWorker::imageReady, [this] (QImage image) {
                if (!this->clean) {
                    this->zone_c->setImage(image);
                }
            });
            this->connect(worker, &FilterWorker::progressChanged, this, [this](int progress) {
                emit progressChanged(progress);
            });
            this->thread_pool->clear();
            this->thread_pool->start(worker);
        });
    }
    this->connect(zone_a, &SourceZone::zoneSelected, this->zone_b, FilterZone::setImage);
}

ZoneContainer::~ZoneContainer() {
    this->thread_pool->clear();
    this->thread_pool->waitForDone();
}

void ZoneContainer::setSourceImage(QImage image) {
    this->zone_a->setImage(image);
    this->clean = false;
}

void ZoneContainer::clear() {
    this->zone_a->clear();
    this->zone_b->clear();
    this->zone_c->clear();
    this->thread_pool->clear();
    this->clean = true;
}

void ZoneContainer::copyBToC() {
    this->zone_c->setImage(this->zone_b->getImage());
}

void ZoneContainer::copyCToB() {
    this->zone_b->setImage(this->zone_c->getImage());
}
