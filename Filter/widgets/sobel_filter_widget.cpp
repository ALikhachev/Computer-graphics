#include <QFormLayout>
#include <QSlider>

#include "widgets/sobel_filter_widget.h"
#include "filter_registry.h"
#include "qsliderbox.h"

SobelFilterWidget::SobelFilterWidget(SobelFilter *f, QWidget *parent) : FilterWidget(parent),
    filter(f),
    settings(f->getSettings())
{
    QFormLayout *layout = new QFormLayout(this);
    QSliderBox *threshold_slider = new QSliderBox(0, 600, this);
    threshold_slider->setValue(this->settings->threshold);
    layout->addRow(tr("Sobel threshold"), threshold_slider);
    connect(threshold_slider, &QSliderBox::valueChanged, [this] (int i) {
        this->settings->threshold = i;
        this->filter->request();
    });
}

namespace {
    bool reg = FilterRegistry::getInstance().registerWidgetBuilder(QObject::tr("Sobel"), [] (Filter *f, QWidget *parent) {
        SobelFilter *sobel_filter = dynamic_cast<SobelFilter *>(f);
        return new SobelFilterWidget(sobel_filter, parent);
    });
}
