#include <QFormLayout>
#include <QSlider>

#include "widgets/watercolor_filter_widget.h"
#include "filter_registry.h"
#include "qsliderbox.h"

WatercolorFilterWidget::WatercolorFilterWidget(WatercolorFilter *f, QWidget *parent) : FilterWidget(parent),
    filter(f),
    settings(f->getSettings())
{
    QFormLayout *layout = new QFormLayout(this);
    QSliderBox *radius_slider = new QSliderBox(1, 10, this);
    radius_slider->setValue(this->settings->radius);
    layout->addRow(tr("Watercolor radius"), radius_slider);
    connect(radius_slider, &QSliderBox::valueChanged, [this] (int i) {
        this->settings->radius = i;
        this->filter->request();
    });
}

namespace {
    bool reg = FilterRegistry::getInstance().registerWidgetBuilder(QObject::tr("Watercolor"), [] (Filter *f, QWidget *parent) {
        WatercolorFilter *watercolor_filter = dynamic_cast<WatercolorFilter *>(f);
        return new WatercolorFilterWidget(watercolor_filter, parent);
    });
}
