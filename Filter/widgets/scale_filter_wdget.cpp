#include <QFormLayout>
#include <QSlider>
#include <QCheckBox>

#include "widgets/scale_filter_widget.h"
#include "filter_registry.h"
#include "qsliderbox.h"


ScaleFilterWidget::ScaleFilterWidget(ScaleFilter *f, QWidget *parent) : FilterWidget(parent),
    filter(f),
    settings(f->getSettings()),
    scale_slider(new QSliderBox(1, 10, this)),
    invert_scale_checkbox(new QCheckBox(this))
{
    QFormLayout *layout = new QFormLayout(this);
    this->scale_slider->setValue(this->settings->scale_factor);
    this->invert_scale_checkbox->setChecked(this->settings->invert);
    layout->addRow(tr("Scale factor"), this->scale_slider);
    layout->addRow(tr("Downscale?"), this->invert_scale_checkbox);
    connect(this->scale_slider, &QSliderBox::valueChanged, [this] (int i) {
        this->settings->scale_factor = i;
        this->filter->request();
    });
    connect(this->invert_scale_checkbox, &QCheckBox::stateChanged, [this] (int i) {
        this->settings->invert = i;
        this->filter->request();
    });
}

void ScaleFilterWidget::settingsUpdate() {
    this->settings = this->filter->getSettings();
    this->scale_slider->setValue(this->settings->scale_factor);
    this->invert_scale_checkbox->setChecked(this->settings->invert);
}

namespace {
    bool reg = FilterRegistry::getInstance().registerWidgetBuilder(QObject::tr("Scale"), [] (Filter *f, QWidget *parent) {
        ScaleFilter *scale_filter = dynamic_cast<ScaleFilter *>(f);
        return new ScaleFilterWidget(scale_filter, parent);
    });
}
