#include <QFormLayout>
#include <QSlider>
#include <QCheckBox>

#include "widgets/scale_filter_widget.h"
#include "filter_registry.h"
#include "qsliderbox.h"


ScaleFilterWidget::ScaleFilterWidget(ScaleFilter *f, QWidget *parent) : FilterWidget(parent),
    filter(f),
    settings(f->getSettings())
{
    QFormLayout *layout = new QFormLayout(this);
    QSliderBox *scale_slider = new QSliderBox(1, 10, this);
    scale_slider->setValue(this->settings->scale_factor);
    QCheckBox *invert_scale_checkbox = new QCheckBox(this);
    invert_scale_checkbox->setChecked(this->settings->invert);
    layout->addRow(tr("Scale factor"), scale_slider);
    layout->addRow(tr("Downscale?"), invert_scale_checkbox);
    connect(scale_slider, &QSliderBox::valueChanged, [this] (int i) {
        this->settings->scale_factor = i;
        this->filter->request();
    });
    connect(invert_scale_checkbox, &QCheckBox::stateChanged, [this] (int i) {
        this->settings->invert = i;
        this->filter->request();
    });
}

namespace {
    bool reg = FilterRegistry::getInstance().registerWidgetBuilder(QObject::tr("Scale"), [] (Filter *f, QWidget *parent) {
        ScaleFilter *scale_filter = dynamic_cast<ScaleFilter *>(f);
        return new ScaleFilterWidget(scale_filter, parent);
    });
}
