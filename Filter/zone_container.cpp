#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

#include "zone_container.h"
#include "filter_registry.h"

SettingsWidget::SettingsWidget(QWidget *parent) : QWidget(parent),
    last_filter(NULL),
    scroll_area(new QScrollArea(this))
{
    this->scroll_area->setFrameShape(QFrame::NoFrame);
    this->scroll_area->setWidget(new QWidget(this));
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel(tr("<b><font size=\"5\">Filter settings</font></b>")));
    layout->addWidget(scroll_area);
}

void SettingsWidget::showFilterWidget(Filter *f) {
    if (f != this->last_filter) {
        if (this->scroll_area->widget()) {
            delete this->scroll_area->widget();
        }
        this->scroll_area->setWidget(FilterRegistry::getInstance().getWidget(f, this->scroll_area));
        this->last_filter = f;
    }
}

ZoneContainer::ZoneContainer(std::vector<QSharedPointer<Filter>> &filters, QWidget *parent) : QWidget(parent),
    filters(filters),
    thread_pool(new QThreadPool(this)),
    clean(true),
    settings_widget(new SettingsWidget(this))
{
    this->thread_pool->setMaxThreadCount(1);
    this->setMinimumSize(352 * 3 + 10 * 5, 352 + 2 * 10 + 145);
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    QHBoxLayout *layout = new QHBoxLayout();
    main_layout->addLayout(layout);
    main_layout->addWidget(this->settings_widget);
    this->zone_a = new SourceZone(this);
    this->zone_b = new FilterZone(this);
    this->zone_c = new FilterZone(this);
    layout->addWidget(this->zone_a, 0, Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(this->zone_b, 0, Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(this->zone_c, 0, Qt::AlignTop | Qt::AlignLeft);
    layout->addStretch();
    layout->setSpacing(10);
    for (auto it = this->filters.begin(); it < this->filters.end(); ++it) {
        Filter *f = it->data();
        this->connect(f, &Filter::requested, [this, f, main_layout] {
            this->settings_widget->showFilterWidget(f);
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
