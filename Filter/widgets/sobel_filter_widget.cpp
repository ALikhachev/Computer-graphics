#include <QFormLayout>
#include <QSlider>

#include "widgets/sobel_filter_widget.h"
#include "filter_registry.h"
#include "qsliderbox.h"

SobelFilterWidget::SobelFilterWidget(SobelFilter *f, QWidget *parent) : FilterWidget(parent),
    filter(f),
    settings(f->getSettings()),
    threshold_slider(new QSliderBox(0, 600, this))
{
    QFormLayout *layout = new QFormLayout(this);
    this->threshold_slider->setValue(this->settings->threshold);
    layout->addRow(tr("Sobel threshold"), this->threshold_slider);
    connect(this->threshold_slider, &QSliderBox::valueChanged, [this] (int i) {
        this->settings->threshold = i;
        this->filter->request();
    });
}

void SobelFilterWidget::settingsUpdate() {
    this->settings = this->filter->getSettings();
    this->threshold_slider->setValue(this->settings->threshold);
}

namespace {
    bool reg = FilterRegistry::getInstance().registerWidgetBuilder(QObject::tr("Sobel"), [] (Filter *f, QWidget *parent) {
        SobelFilter *sobel_filter = dynamic_cast<SobelFilter *>(f);
        return new SobelFilterWidget(sobel_filter, parent);
    });
}
