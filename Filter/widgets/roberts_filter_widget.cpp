#include <QFormLayout>
#include <QSlider>

#include "widgets/Roberts_filter_widget.h"
#include "filter_registry.h"
#include "qsliderbox.h"

RobertsFilterWidget::RobertsFilterWidget(RobertsFilter *f, QWidget *parent) : FilterWidget(parent),
    filter(f),
    settings(f->getSettings())
{
    QFormLayout *layout = new QFormLayout(this);
    QSliderBox *threshold_slider = new QSliderBox(0, 600, this);
    threshold_slider->setValue(this->settings->threshold);
    layout->addRow(tr("Roberts threshold"), threshold_slider);
    connect(threshold_slider, &QSliderBox::valueChanged, [this] (int i) {
        this->settings->threshold = i;
        this->filter->request();
    });
}

namespace {
    bool reg = FilterRegistry::getInstance().registerWidgetBuilder(QObject::tr("Roberts"), [] (Filter *f, QWidget *parent) {
        RobertsFilter *Roberts_filter = dynamic_cast<RobertsFilter *>(f);
        return new RobertsFilterWidget(Roberts_filter, parent);
    });
}
