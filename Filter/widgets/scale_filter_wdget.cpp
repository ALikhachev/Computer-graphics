#include <QVBoxLayout>
#include <QSlider>
#include <QCheckBox>

#include "widgets/scale_filter_widget.h"
#include "filter_registry.h"


ScaleFilterWidget::ScaleFilterWidget(ScaleFilter *f, QWidget *parent) : FilterWidget(parent),
    filter(f)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QSlider *scale_slider = new QSlider(Qt::Horizontal, this);
    scale_slider->setRange(1, 10);
    layout->addWidget(scale_slider);
    QCheckBox *invert_scale_checkbox = new QCheckBox(this);
    layout->addWidget(invert_scale_checkbox);
}

namespace {
    bool reg = FilterRegistry::getInstance().registerWidgetBuilder(QObject::tr("Scale"), [] (Filter *f, QWidget *parent) {
        ScaleFilter *scale_filter = dynamic_cast<ScaleFilter *>(f);
        return new ScaleFilterWidget(scale_filter, parent);
    });
}
