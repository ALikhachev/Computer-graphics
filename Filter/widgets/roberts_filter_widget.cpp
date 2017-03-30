#include <QFormLayout>
#include <QSlider>

#include "widgets/roberts_filter_widget.h"
#include "filter_registry.h"
#include "qsliderbox.h"

RobertsFilterWidget::RobertsFilterWidget(RobertsFilter *f, QWidget *parent) : FilterWidget(parent),
    filter(f),
    settings(f->getSettings()),
    threshold_slider(new QSliderBox(0, 600, this))
{
    QFormLayout *layout = new QFormLayout(this);
    this->threshold_slider->setValue(this->settings->threshold);
    layout->addRow(tr("Roberts threshold"), this->threshold_slider);
    connect(this->threshold_slider, &QSliderBox::valueChanged, [this] (int i) {
        this->settings->threshold = i;
        this->filter->request();
    });
}

void RobertsFilterWidget::settingsUpdate() {
    this->settings = this->filter->getSettings();
    this->threshold_slider->setValue(this->settings->threshold);
}

namespace {
    bool reg = FilterRegistry::getInstance().registerWidgetBuilder(QObject::tr("Roberts"), [] (Filter *f, QWidget *parent) {
        RobertsFilter *Roberts_filter = dynamic_cast<RobertsFilter *>(f);
        return new RobertsFilterWidget(Roberts_filter, parent);
    });
}
