#include <QFormLayout>
#include <QSlider>

#include "widgets/rotate_filter_widget.h"
#include "filter_registry.h"
#include "qsliderbox.h"

RotateFilterWidget::RotateFilterWidget(RotateFilter *f, QWidget *parent) : FilterWidget(parent),
    filter(f),
    settings(f->getSettings()),
    angle_slider(new QSliderBox(-180, 180, this))
{
    QFormLayout *layout = new QFormLayout(this);
    this->angle_slider->setValue(this->settings->angle);
    layout->addRow(tr("Rotation angle"), this->angle_slider);
    connect(this->angle_slider, &QSliderBox::valueChanged, [this] (int i) {
        this->settings->angle = i;
        this->filter->request();
    });
}

void RotateFilterWidget::settingsUpdate() {
    this->settings = this->filter->getSettings();
    this->angle_slider->setValue(this->settings->angle);
}

namespace {
    bool reg = FilterRegistry::getInstance().registerWidgetBuilder(QObject::tr("Rotate"), [] (Filter *f, QWidget *parent) {
        RotateFilter *rotate_filter = dynamic_cast<RotateFilter *>(f);
        return new RotateFilterWidget(rotate_filter, parent);
    });
}
