#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "zone_container.h"
#include "filter_registry.h"

SettingsWidget::SettingsWidget(QWidget *parent) : QWidget(parent),
    last_filter(NULL),
    scroll_area(new QScrollArea(this)),
    widget(NULL),
    ok_button(new QPushButton(tr("OK"), this)),
    cancel_button(new QPushButton(tr("Cancel"), this))
{
    this->scroll_area->setFrameShape(QFrame::NoFrame);
    this->scroll_area->setWidget(new QWidget(this));
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    QHBoxLayout *layout = new QHBoxLayout();
    main_layout->addWidget(new QLabel(tr("<b><font size=\"5\">Filter settings</font></b>")));
    main_layout->addWidget(scroll_area);
    layout->addWidget(this->ok_button);
    layout->addWidget(this->cancel_button);
    main_layout->addLayout(layout);
    this->ok_button->setEnabled(false);
    this->cancel_button->setEnabled(false);
    connect(this->ok_button, &QPushButton::released, this, [this] {
        emit saveFilterRequested(this->last_filter);
        this->cancel_button->setEnabled(true);
    });
    connect(cancel_button, &QPushButton::released, this, [this] {
        emit restoreFilterRequested();
        this->cancel_button->setEnabled(false);
    });
}

void SettingsWidget::clear() {
    this->widget = NULL;
    delete this->scroll_area->takeWidget();
    this->ok_button->setEnabled(false);
    this->cancel_button->setEnabled(false);
    this->last_filter = NULL;
}

void SettingsWidget::showFilterWidget(Filter *f) {
    this->ok_button->setEnabled(true);
    if (f != this->last_filter) {
        if (this->scroll_area->widget()) {
            delete this->scroll_area->widget();
            this->widget = NULL;
        }
        this->widget = FilterRegistry::getInstance().getWidget(f, this->scroll_area);
        this->scroll_area->setWidget(this->widget);
        this->last_filter = f;
    } else if (this->widget) {
        this->widget->settingsUpdate();
    }
}

ZoneContainer::ZoneContainer(std::vector<QSharedPointer<Filter>> &filters, QWidget *parent) : QWidget(parent),
    filters(filters),
    thread_pool(new QThreadPool(this)),
    clean(true),
    settings_widget(new SettingsWidget(this)),
    current_filter(NULL),
    saved_filter(NULL),
    saved_filter_settings(NULL)
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
            this->current_filter = f;
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
    this->connect(zone_a, &SourceZone::zoneSelected, [this] (QImage image) {
        this->zone_b->setImage(image);
        if (this->current_filter) {
            this->current_filter->request();
        }
    });
    connect(this->settings_widget, &SettingsWidget::saveFilterRequested, this, &ZoneContainer::saveFilterWithSettings);
    connect(this->settings_widget, &SettingsWidget::restoreFilterRequested, this, &ZoneContainer::restoreFilterWithSettings);
}

ZoneContainer::~ZoneContainer() {
    this->thread_pool->clear();
    this->thread_pool->waitForDone();
}

void ZoneContainer::setSourceImage(QImage image) {
    this->zone_a->setImage(image);
    this->clean = false;
}

bool ZoneContainer::saveResultImage(QString filename) {
    return this->zone_c->getImage().save(filename);
}

void ZoneContainer::clear() {
    this->zone_a->clear();
    this->zone_b->clear();
    this->zone_c->clear();
    this->thread_pool->clear();
    this->clean = true;
    this->settings_widget->clear();
    this->current_filter = NULL;
}

void ZoneContainer::copyBToC() {
    this->zone_c->setImage(this->zone_b->getImage());
}

void ZoneContainer::copyCToB() {
    this->zone_b->setImage(this->zone_c->getImage());
    if (this->current_filter) {
        this->current_filter->request();
    }
}

void ZoneContainer::saveFilterWithSettings(Filter *f) {
    this->saved_filter = f;
    if (f->getSettings()) {
        this->saved_filter_settings = f->getSettings()->clone();
    } else {
        this->saved_filter_settings = NULL;
    }
}

void ZoneContainer::restoreFilterWithSettings() {
    if (this->saved_filter_settings) {
        this->saved_filter->setSettings(this->saved_filter_settings);
        delete this->saved_filter_settings;
    }
    this->saved_filter->request();
}
